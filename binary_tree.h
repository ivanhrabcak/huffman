#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "binary_buffer.h"

typedef struct binary_tree binary_tree;

typedef struct binary_tree {
    char value;
    
    uint8_t is_value_null;

    binary_tree* left;
    binary_tree* right;
} binary_tree;

binary_tree* initialize_node() {
    binary_tree* tree = (binary_tree*) malloc(sizeof(binary_tree));

    tree->is_value_null = 1;
    tree->value = ' ';
    tree->left = NULL;
    tree->right = NULL;
}

binary_buffer* encode_internal(binary_tree* tree, binary_buffer* flags, uint16_t* amount_of_nodes) {
    *amount_of_nodes = *amount_of_nodes + 1;

    write_bit(flags, tree->left != NULL);
    write_bit(flags, tree->right != NULL);
    printf("Writing flag value bit as %d\n", !tree->is_value_null);
    write_bit(flags, !tree->is_value_null);

    binary_buffer* encoded = initialize_buffer();

    if (!tree->is_value_null) {
        printf("Tree has value, writing value...\n");
        write_byte(encoded, tree->value);
    } else {
        printf("Node has no value (encoding)\n");
    }

    if (tree->left != NULL) {
        printf("Tree has left branch, recursing...\n");
        binary_buffer* encoded_left = encode_internal(tree->left, flags, amount_of_nodes);
        for (int i = 0; i < encoded_left->length; i++) {
            write_byte(encoded, encoded_left->buf[i]);
        }

        free(encoded_left);
    }

    if (tree->right != NULL) {
        printf("Tree has right branch, recursing...\n");
        binary_buffer* encoded_right = encode_internal(tree->right, flags, amount_of_nodes);
        for (int i = 0; i < encoded_right->length; i++) {
            write_byte(encoded, encoded_right->buf[i]);
        }

        free(encoded_right);
    }

    return encoded;
}

binary_buffer* encode_tree(binary_tree* tree) {
    uint16_t amount_of_nodes = 0;
    binary_buffer* flags = initialize_buffer();

    binary_buffer* encoded_tree = encode_internal(tree, flags, &amount_of_nodes);

    if (flags->temp != 0) {
        printf("Flushing because temp is %d\n", flags->temp);
        flush(flags);
    }

    printf("encoded tree flag real length %d\n", flags->length);

    uint8_t b2 = (amount_of_nodes & 0b1111111100000000) >> 8;
    uint8_t b1 = amount_of_nodes & 0b0000000011111111;

    printf("Encoding node count %d as %d and %d\n", amount_of_nodes, b1, b2);

    binary_buffer* output = initialize_buffer();
    
    printf("Length before first write => %d\n", output->length);
    write_byte(output, b1);
    printf("Length after first write => %d\n", output->length);
    write_byte(output, b2);

    printf("Length after second write => %d\n", output->length);

    printf("Buffer is now of length %d.\n", output->length);
    printf("Second byte is %d\n", output->buf[1]);

    printf("Will be writing %d flag bytes!\n", flags->length);

    printf("Flag bytes:\n [");
    for (int i = 0; i < flags->length; i++) {
        write_byte(output, flags->buf[i]);
        printf("%d, ", flags->buf[i]);
    }

    printf("]\n");

    for (int i = 0; i < encoded_tree->length; i++) {
        write_byte(output, encoded_tree->buf[i]);
    }

    free(encoded_tree);
    free(flags);

    return output;
}

binary_tree* decode_internal(binary_buffer* tree, binary_buffer* flags, uint32_t* flags_index, uint32_t* tree_index) {
    uint8_t has_left = read_bit(tree, flags_index);
    uint8_t has_right = read_bit(tree, flags_index);
    uint8_t has_value = read_bit(tree, flags_index);

    printf("%d %d %d => flag bits\n", has_left, has_right, has_value);

    uint8_t value = 0;
    binary_tree* left = NULL;
    binary_tree* right = NULL;

    if (has_value) {
        value = tree->buf[*tree_index];
        printf("wrote value!\n");
        has_value = 1;
        *tree_index = *tree_index + 1;
    } else {
        printf("This node has no value! bits are %d %d %d\n", has_left, has_right, has_value);
    }

    if (has_left) {
        left = decode_internal(tree, flags, flags_index, tree_index);
    }

    if (has_right) {
        right = decode_internal(tree, flags, flags_index, tree_index);
    }

    binary_tree* output = initialize_node();

    output->is_value_null = has_value;
    output->value = value;
    output->left = left;
    output->right = right;

    return output;
}

binary_tree* decode_tree(binary_buffer* tree) {
    uint8_t d1 = tree->buf[0];
    uint8_t d2 = tree->buf[1];

    printf("Decoding %d and %d!\n", d1, d2);

    // magic
    uint16_t amount_of_nodes = (uint16_t)((d2 << 8) + (d1 & 0x00ff));

    uint32_t flag_bits = amount_of_nodes * 3;
    uint32_t flag_bytes = (flag_bits + (8 - flag_bits % 8)) / 8;

    printf("there will be %d encoded nodes\n", amount_of_nodes);
    printf("that is %d bytes\n", flag_bytes);

    binary_buffer* flags = initialize_buffer();
    for (int i = 3; i < flag_bytes; i++) {
        write_byte(flags, tree->buf[i]);
    }

    printf("First flag byte is %d\n", tree->buf[3]);

    uint32_t flags_index = 0;
    uint32_t tree_index = flag_bytes;
    return decode_internal(tree, flags, &flags_index, &tree_index);
}

// void free_tree(binary_tree* tree) {
//     if (tree->left != NULL) {
//         free_tree(tree->left);
//     }

//     if (tree->right != NULL) {
//         free_tree(tree->right);
//     }

//     free(tree);
// }