#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "binary_buffer.h"

binary_buffer* initialize_buffer() {
    binary_buffer* b = (binary_buffer*) malloc(sizeof(binary_buffer));
    b->length = 0;
    b->temp = 0;
    b->temp_ind = 0;
    b->buf = NULL;
}

void write_bit(binary_buffer* buf, uint8_t bit) {
    if (buf->temp_ind == 7) {
        buf->temp_ind = 0;
        buf->length++;

        buf->buf = (uint8_t*) realloc(buf->buf, buf->length);
        
        buf->buf[buf->length - 1] = buf->temp;
        
        buf->temp = 0;
    }

    if (bit) {
        buf->temp = buf->temp | (0b1 << buf->temp_ind);
    }
    
    buf->temp_ind++;
}

void flush(binary_buffer* buf) {
    // if (buf->temp == 0) {
        // return;
    // }

    buf->length++;
    buf->buf = (uint8_t*) realloc(buf->buf, buf->length);

    buf->buf[buf->length - 1] = buf->temp;

    buf->temp_ind = 0;
    buf->temp = 0;
}

uint8_t read_bit(binary_buffer* buf, uint32_t* index) {
    if (buf->temp_ind == 7) {
        buf->temp_ind = 0;
        *index = *index + 1;
    }

    return 0b00000001 & (buf->buf[*index] >> buf->temp_ind++);
}

void write_byte(binary_buffer* buf, uint8_t byte) {    
    buf->temp = byte;
    flush(buf);
}