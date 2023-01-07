#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "binary_tree.h"
#include "array_list.h"

array_list* initialize_array_list(size_t element_size) {
    array_list* l = (array_list*) malloc(sizeof(array_list));

    l->length = 0;
    l->array = NULL;
    l->element_size = element_size;

    return l;
}

void append(array_list* list, void* item) {
    list->length = list->length + 1;

    list->array = (void**) realloc(list->array, sizeof(void*) * list->length);
    list->array[list->length - 1] = item;
}

void* pop(array_list* list) {
    if (list->length == 0) {
        return NULL;
    }
    
    list->length--;

    void** original = list->array;

    void* output = malloc(list->element_size);
    void* first_element = original[0];

    memcpy(output, first_element, list->element_size);

    void** new_ptr = (void**) malloc(sizeof(void*) * list->length);

    for (int i = 1; i < list->length + 1; i++) {
        new_ptr[i-1] = original[i];
    }
    
    list->array = new_ptr;

    free(original);
    free(first_element);

    return output;
}
