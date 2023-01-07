#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "data_structures/binary_tree.h"
#include "data_structures/binary_buffer.h"
#include "data_structures/array_list.h"

typedef struct {
    char c;
    uint32_t count;
} character_count;

void increment_character_count(char c, array_list* list) {
    for (int i = 0; i < list->length; i++) {
        character_count* count = (character_count*) list->array[i];
        if (count->c != c) {
            continue;
        }

        count->count++;
        return;
    }

    character_count* count = (character_count*) malloc(sizeof(character_count));
    count->c = c;
    count->count = 1;

    append(list, count);
}

uint32_t lookup_character_count(char c, array_list* list) {
    for (int i = 0; i < list->length; i++) {
        character_count* count = (character_count*) list->array[i];
        if (count->c != c) {
            continue;
        }

        
        return count->count;
    }

    return 0;
}

int compare_character_counts(const void* c1, const void* c2) {
    const character_count* a = *((character_count**) c1);
    const character_count* b = *((character_count**) c2);

    if (a->count == b->count) {
        return b->c - a->c;
    }

    return a->count - b->count;
}

binary_tree* generate_tree(char* s) {
    size_t length = strlen(s);

    array_list* character_counts = initialize_array_list(sizeof(character_count));

    for (int i = 0; i < length; i++) {
        char c = s[i];
        increment_character_count(c, character_counts);
    }

    qsort(character_counts->array, character_counts->length, sizeof(void**), compare_character_counts);

    array_list* queue = initialize_array_list(sizeof(binary_tree));
    for (int i = 0; i < character_counts->length; i++) {
        character_count* count = (character_count*) character_counts->array[i];
        binary_tree* node = initialize_node();

        node->value = count->c;
        node->is_value_null = 0;

        append(queue, node);
    }

    while (queue->length != 1) {
        binary_tree* left = (binary_tree*) pop(queue);
        binary_tree* right = (binary_tree*) pop(queue);

        binary_tree* parent = initialize_node();
        
        parent->left = left;
        parent->right = right;

        append(queue, parent);
    }

    binary_tree* output = (binary_tree*) pop(queue);

    for (int i = 0; i < character_counts->length; i++) {
        free(character_counts->array[i]);
    }

    free(character_counts);
    free(queue);

    return output;
}

typedef struct {
    char c;
    char* path;
} tree_path;

void generate_tree_paths(binary_tree* tree, array_list* paths, char** path) {
    if (!tree->is_value_null) {
        tree_path* t_path = malloc(sizeof(tree_path));

        t_path->c = tree->value;
        t_path->path = malloc(strlen(*path) + 1);

        strcpy(t_path->path, *path);

        append(paths, t_path);
    }

    size_t path_len = 1;
    if (*path != NULL) {
        path_len = strlen(*path) + 1;
    }

    if (tree->left != NULL) {
        *path = realloc(*path, path_len + 1);
        
        (*path)[path_len - 1] = '0';
        (*path)[path_len] = '\0';

        generate_tree_paths(tree->left, paths, path);

        *path = realloc(*path, path_len);
        (*path)[path_len - 1] = '\0';
    }

    if (tree->right != NULL) {
        *path = realloc(*path, path_len + 1);
        
        (*path)[path_len - 1] = '1';
        (*path)[path_len] = '\0';

        generate_tree_paths(tree->right, paths, path);

        *path = realloc(*path, path_len);
        (*path)[path_len - 1] = '\0';
    }
}

char* lookup_tree_path_by_char(array_list* paths, char c) {
    for (int i = 0; i < paths->length; i++) {
        tree_path* p = (tree_path*) paths->array[i];
        if (p->c != c) {
            continue;
        }

        return p->path;
    }

    return NULL;
}

char* lookup_char_by_tree_path(array_list* paths, char* path) {
    for (int i = 0; i < paths->length; i++) {
        tree_path* p = (tree_path*) paths->array[i];
        if (strcmp(p->path, path) == 0) {
            return &p->c;
        }
    }

    return NULL;
}

binary_buffer* huffman_encode(char* s, binary_tree* tree) {
    array_list* paths = initialize_array_list(sizeof(tree_path));

    char* path = NULL;

    generate_tree_paths(tree, paths, &path);

    binary_buffer* encoded = initialize_buffer();

    size_t len = strlen(s);
    for (int i = 0; i < len; i++) {
        char c = s[i];
        
        char* character_path = lookup_tree_path_by_char(paths, c);
        size_t path_len = strlen(character_path);
        for (int k = 0; k < path_len; k++) {
            write_bit(encoded, character_path[k] == '1');
        }
    }

    if (encoded->temp_ind != 0) {
        flush(encoded);
    }

    free(path);
    for (int i = 0; i < paths->length; i++) {
        free(paths->array[i]);
    }

    free(paths);


    return encoded;
}

char* huffman_decode(binary_tree* tree, binary_buffer* encoded) {
    encoded->temp_ind = 0;

    array_list* paths = initialize_array_list(sizeof(tree_path));
    char* path = NULL;

    generate_tree_paths(tree, paths, &path);

    free(path);

    uint32_t encoded_index = 0;

    char* decoded = NULL;
    uint32_t decoded_length = 1;

    path = NULL;
    uint32_t path_length = 1;
    while (has_next_bit(encoded, encoded_index)) {
        path = realloc(path, path_length + 1);
        path[path_length] = '\0';
        path[path_length++ - 1] = read_bit(encoded, &encoded_index) == 1 ? '1' : '0';

        char* new_char = lookup_char_by_tree_path(paths, path);
        if (new_char != NULL) {
            decoded = realloc(decoded, decoded_length + 1);
            decoded[decoded_length] = '\0';
            decoded[decoded_length++ - 1] = *new_char;

            free(path);
            path = NULL;
            path_length = 1;
        }
    }

    return decoded;
}

uint8_t encode_to_file(char* s, char* filename) {
    FILE* fptr;

    fptr = fopen(filename, "wb+");
    if (fptr == NULL) {
        printf("Failed to open data.bin!");
        return 1;
    }

    binary_tree* tree = generate_tree(s);

    binary_buffer* encoded_tree = encode_tree(tree);
    for (int i = 0; i < encoded_tree->length; i++) {
        fputc(encoded_tree->buf[i], fptr);
    }

    binary_buffer* encoded_message = huffman_encode(s, tree);
    for (int i = 0; i < encoded_message->length; i++) {
        fputc(encoded_message->buf[i], fptr);
    }

    fclose(fptr);
    return 0;
}

char* decode_from_file(char* filename) {
    FILE* fptr;
    fptr = fopen(filename, "rb");

    if (fptr == NULL) {
        return NULL;
    }

    binary_buffer* encoded = initialize_buffer();
    int b;
    while ((b = fgetc(fptr)) != EOF) {
        // safe cast as the only value
        // outside of uint8_t range that
        // can be returned from fgetc is EOF.
        uint8_t byte = (uint8_t) b;

        write_byte(encoded, byte);
    }
    fclose(fptr);

    uint32_t message_start_index = 0;
    binary_tree* tree = decode_tree(encoded, &message_start_index);

    binary_buffer* encoded_message = initialize_buffer();
    for (int i = message_start_index + 2; i < encoded->length; i++) {
        write_byte(encoded_message, encoded->buf[i]);
    }

    char* decoded = huffman_decode(tree, encoded_message);

    free_tree(tree);
    free(encoded);
    free(encoded_message);

    return decoded;
}