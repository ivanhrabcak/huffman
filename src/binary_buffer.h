#include <stdint.h>

#ifndef _BINARY_F_
#define _BINARY_F_

typedef struct {
    uint32_t length;
    uint8_t *buf;
    uint8_t temp;
    uint8_t temp_ind;
} binary_buffer;

binary_buffer* initialize_buffer();

void write_bit(binary_buffer* buf, uint8_t bit);

void flush(binary_buffer* buf);

uint8_t read_bit(binary_buffer* buf, uint32_t* index);

void write_byte(binary_buffer* buf, uint8_t byte);

uint8_t has_next_bit(binary_buffer* buf, uint32_t index);

#endif