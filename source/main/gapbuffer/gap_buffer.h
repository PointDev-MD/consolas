#ifndef GAP_BUFFER_H
#define GAP_BUFFER_H

#include <stddef.h>

typedef struct {
    char* buffer;
    size_t capacity;
    size_t gap_start;
    size_t gap_end;
} GapBuffer;

GapBuffer* gap_buffer_create(size_t initial_capacity);
void gap_buffer_free(GapBuffer* gb);
void gap_buffer_insert_char(GapBuffer* gb, char c);
void gap_buffer_delete_char(GapBuffer* gb);
void gap_buffer_move_cursor(GapBuffer* gb, int position);
void gap_buffer_print_debug(GapBuffer* gb);

#endif