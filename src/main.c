#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Usage:\n");
        printf("    ./huffman [encode/decode] <file_to_encode> <output filename>\n");
        return 1;
    }

    char* action = argv[1];
    if (strcmp(action, "encode") == 0) {
        FILE* fptr;
        fptr = fopen(argv[2], "r");

        if (fptr == NULL) {
            printf("%s\n", strerror(errno));
            return 1;
        }

        char* file_contents = NULL;
        uint32_t size = 1;

        char c;
        while ((c = fgetc(fptr)) != EOF) {
            file_contents = realloc(file_contents, size);
            file_contents[size++] = c;
        }

        if (file_contents == NULL) {
            printf("Empty file!\n");
            return 1;
        }

        encode_to_file(file_contents, argv[3]);
    }
    

    char* decoded = decode_from_file("encoded.bin");
    printf("decoded: %s\n", decoded);
}