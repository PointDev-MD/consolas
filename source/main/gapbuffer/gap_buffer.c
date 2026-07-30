#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gap_buffer.h"

GapBuffer* gap_buffer_create(size_t initial_capacity) {
    GapBuffer* gb = (GapBuffer*)malloc(sizeof(GapBuffer));
    gb->buffer = (char*)malloc(sizeof(char) * initial_capacity);
    gb->capacity = initial_capacity;
    gb->gap_start = 0;
    gb->gap_end = initial_capacity;
    return gb;
}

void gap_buffer_free(GapBuffer* gb) {
    if (!gb) return;
    free(gb->buffer);
    free(gb);
}

static void gap_buffer_grow(GapBuffer* gb) {
    size_t new_capacity = gb->capacity * 2;
    char* new_buffer = (char*)malloc(sizeof(char) * new_capacity);

    memcpy(new_buffer, gb->buffer, gb->gap_start);

    size_t right_count = gb->capacity - gb->gap_end;
    size_t new_gap_end = new_capacity - right_count;

    memcpy(new_buffer + new_gap_end, gb->buffer + gb->gap_end, right_count);

    free(gb->buffer);
    gb->buffer = new_buffer;
    gb->capacity = new_capacity;
    gb->gap_end = new_gap_end;
}

void gap_buffer_insert_char(GapBuffer* gb, char c) {
    if (gb->gap_start == gb->gap_end) {
        gap_buffer_grow(gb);
    }
    gb->buffer[gb->gap_start++] = c;
}

void gap_buffer_delete_char(GapBuffer* gb) {
    if (gb->gap_start > 0) {
        gb->gap_start--;
    }
}

void gap_buffer_move_cursor(GapBuffer* gb, int position) {
    if (position < 0) position = 0;
    if ((size_t)position > gb->capacity - (gb->gap_end - gb->gap_start)) {
        position = gb->capacity - (gb->gap_end - gb->gap_start);
    }

    while ((size_t)position < gb->gap_start) {
        gb->gap_start--;
        gb->gap_end--;
        gb->buffer[gb->gap_end] = gb->buffer[gb->gap_start];
    }

    while ((size_t)position > gb->gap_start) {
        gb->buffer[gb->gap_start] = gb->buffer[gb->gap_end];
        gb->gap_start++;
        gb->gap_end++;
    }
}