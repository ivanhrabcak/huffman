#include <stdint.h>

#include "binary_buffer.h"

#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

typedef struct binary_tree binary_tree;

typedef struct binary_tree {
    char value;
    
    uint8_t is_value_null;

    binary_tree* left;
    binary_tree* right;
} binary_tree;

binary_tree* initialize_node();

binary_buffer* encode_internal(binary_tree* tree, binary_buffer* flags, uint16_t* amount_of_nodes);

binary_buffer* encode_tree(binary_tree* tree);

binary_tree* decode_tree(binary_buffer* tree, uint32_t* end_index);

void free_tree(binary_tree* tree);

#endif