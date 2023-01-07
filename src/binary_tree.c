#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "binary_buffer.h"
#include "binary_tree.h"

binary_tree* initialize_node() {
    binary_tree* tree = (binary_tree*) malloc(sizeof(binary_tree));

    tree->is_value_null = 1;
    tree->value = ' ';
    tree->left = NULL;
    tree->right = NULL;

    return tree;
}

binary_buffer* encode_internal(binary_tree* tree, binary_buffer* flags, uint16_t* amount_of_nodes) {
    *amount_of_nodes = *amount_of_nodes + 1;

    write_bit(flags, tree->left != NULL);
    write_bit(flags, tree->right != NULL);
    write_bit(flags, !tree->is_value_null);

    binary_buffer* encoded = initialize_buffer();

    if (!tree->is_value_null) {
        write_byte(encoded, tree->value);
    }

    if (tree->left != NULL) {
        binary_buffer* encoded_left = encode_internal(tree->left, flags, amount_of_nodes);
        for (int i = 0; i < encoded_left->length; i++) {
            write_byte(encoded, encoded_left->buf[i]);
        }

        free(encoded_left);
    }

    if (tree->right != NULL) {
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
        flush(flags);
    }

    uint8_t b2 = (amount_of_nodes & 0b1111111100000000) >> 8;
    uint8_t b1 = amount_of_nodes & 0b0000000011111111;

    printf("Encoding - there will be %d nodes\n", amount_of_nodes);

    binary_buffer* output = initialize_buffer();
    
    write_byte(output, b1);
    write_byte(output, b2);

    for (int i = 0; i < flags->length; i++) {
        write_byte(output, flags->buf[i]);
    }

    for (int i = 0; i < encoded_tree->length; i++) {
        write_byte(output, encoded_tree->buf[i]);
    }

    printf("encoding tree %d %d\n", encoded_tree->buf[0], encoded_tree->buf[1]);

    free(encoded_tree);
    free(flags);

    return output;
}

binary_tree* decode_internal(binary_buffer* tree, binary_buffer* flags, uint32_t* flags_index, uint32_t* tree_index) {
    uint8_t has_left = read_bit(flags, flags_index);
    uint8_t has_right = read_bit(flags, flags_index);
    uint8_t has_value = read_bit(flags, flags_index);

    uint8_t value = 0;
    binary_tree* left = NULL;
    binary_tree* right = NULL;

    if (has_value) {
        value = tree->buf[*tree_index];
        has_value = 1;
        *tree_index = *tree_index + 1;
    }

    if (has_left) {
        left = decode_internal(tree, flags, flags_index, tree_index);
    }

    if (has_right) {
        right = decode_internal(tree, flags, flags_index, tree_index);
    }

    binary_tree* output = initialize_node();

    output->is_value_null = !has_value;
    output->value = value;
    output->left = left;
    output->right = right;

    return output;
}

binary_tree* decode_tree(binary_buffer* tree, uint32_t* end_index) {
    uint8_t d1 = tree->buf[0];
    uint8_t d2 = tree->buf[1];

    // magic
    uint16_t amount_of_nodes = (uint16_t)((d2 << 8) + (d1 & 0x00ff));
    printf("Decoded - there will be %d nodes\n", amount_of_nodes);

    uint32_t flag_bits = amount_of_nodes * 3;
    uint32_t flag_bytes = (flag_bits + (8 - flag_bits % 8)) / 8;

    binary_buffer* flags = initialize_buffer();
    for (int i = 2; i < flag_bytes + 3; i++) {
        write_byte(flags, tree->buf[i]);
    }

    uint32_t flags_index = 0;

    *end_index = flag_bytes + 3;
    binary_tree* decoded_tree = decode_internal(tree, flags, &flags_index, end_index);

    
    *end_index = *end_index - 2;
    return decoded_tree;
}

void free_tree(binary_tree* tree) {
    if (tree->left != NULL) {
        free_tree(tree->left);
    }

    if (tree->right != NULL) {
        free_tree(tree->right);
    }

    free(tree);
}