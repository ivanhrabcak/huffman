#include <stdint.h>

#include "binary_tree.h"
#include "binary_buffer.h"
#include "array_list.h"

void increment_character_count(char c, array_list* list);
uint32_t lookup_character_count(char c, array_list* list);

binary_tree* generate_tree(char* s);

void generate_tree_paths(binary_tree* tree, array_list* paths, char** path);
char* lookup_tree_path_by_char(array_list* paths, char c);
char* lookup_char_by_tree_path(array_list* paths, char* path);

binary_buffer* huffman_encode(char* s, binary_tree* tree);
char* huffman_decode(binary_tree* tree, binary_buffer* encoded);

uint8_t encode_to_file(char* s, char* filename);
char* decode_from_file(char* filename);