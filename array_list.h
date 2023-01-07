#include <stdint.h>

#include "binary_tree.h"

#ifndef _TREE_QUEUE_H_
#define _TREE_QUEUE_H_

typedef struct {
    uint32_t length;
    void** array;
    size_t element_size;
} array_list;

array_list* initialize_array_list(size_t element_size);

void append(array_list* list, void* item);

void* pop(array_list* list);

#endif