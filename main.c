#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "binary_buffer.h"
#include "binary_tree.h"

void print_tree_values(binary_tree* tree) {
    if (!tree->is_value_null) {
        putchar(tree->value);
    } else {
        printf("no value!\n");
    }

    if (tree->left != NULL) {
        printf("recursing...\n");
        print_tree_values(tree->left);
    }

    if (tree->right != NULL) {
        printf("recursing...\n");
        print_tree_values(tree->right);
    }
}

int main() {
    binary_tree* tree = initialize_node();
    tree->is_value_null = 0;
    tree->value = 'h';

    binary_tree* left = initialize_node();
    left->is_value_null = 0;
    left->value = 'e';

    binary_tree* right = initialize_node();
    right->is_value_null = 0;
    right->value = 'l';

    tree->left = left;
    tree->right = right;

    binary_buffer* encoded_tree = encode_tree(tree);

    for (int i = 0; i < encoded_tree->length; i++) {
        printf("%d\n", encoded_tree->buf[i]);
    }

    binary_tree* decoded_tree = decode_tree(encoded_tree);
    print_tree_values(decoded_tree);
    printf(" decoded_tree\n");
    print_tree_values(tree);
    printf(" original_tree\n");
}