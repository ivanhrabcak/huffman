#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "binary_buffer.h"
#include "binary_tree.h"
#include "array_list.h"
#include "huffman.h"

void print_tree_values(binary_tree* tree) {
    if (!tree->is_value_null) {
        putchar(tree->value);
    }

    if (tree->left != NULL) {
        print_tree_values(tree->left);
    }

    if (tree->right != NULL) {
        print_tree_values(tree->right);
    }
}

int main() {
    // binary_tree* tree = initialize_node();
    // tree->is_value_null = 0;
    // tree->value = 'h';

    // binary_tree* left = initialize_node();
    // left->is_value_null = 0;
    // left->value = 'e';

    // binary_tree* right = initialize_node();
    // right->is_value_null = 0;
    // right->value = 'l';

    // tree->left = left;
    // tree->right = right;

    // binary_buffer* encoded_tree = encode_tree(tree);

    // for (int i = 0; i < encoded_tree->length; i++) {
    //     printf("%d\n", encoded_tree->buf[i]);
    // }

    // binary_tree* decoded_tree = decode_tree(encoded_tree);
    // print_tree_values(decoded_tree);
    // printf(" decoded_tree\n");
    // print_tree_values(tree);
    // printf(" original_tree\n");

    // binary_buffer* b = initialize_buffer();
    // write_byte(b, 39);
    // write_byte(b, 2);

    // uint32_t index = 0;
    // for (int i = 0; i < 9; i++) {
    //     printf("%d\n", read_bit(b, &index));
    // }

    // array_list* list = initialize_array_list(sizeof(binary_tree));
    // append(list, tree);
    // append(list, left);
    // append(list, right);

    // printf("Last node value is %c\n", ((binary_tree*) list->array[2])->value);
    // printf("Length after pushing %d\n", list->length);

    // for (int i = 0; i < 3; i++) {
    //     binary_tree* t = (binary_tree*) pop(list);
    //     printf("tree value => %c\n", t->value);
    //     printf("length %d\n", list->length);
    // }

    // free_list(list);

    encode_to_file("hello world!", "encoded.bin");

    char* decoded = decode_from_file("encoded.bin");
    printf("decoded: %s\n", decoded);
}