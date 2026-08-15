#ifndef ARENA_H
#define ARENA_H

#include <stdio.h>
#include <stdint.h>
// Default arena chunk size is 64 KiB
#define DEFAULT_ARENA_CHUNK_SIZE (64 * 1024)  

typedef struct Arena_chunk {
    struct Arena_chunk* next;
    size_t capacity;
    size_t offset;
    size_t _padding;    // Pad to 32 bytes to ensure data[] starts on a 16 byte boundary 
    uint8_t data[];
} arena_chunk_s;


typedef struct Arena {
    arena_chunk_s* head;
    arena_chunk_s* curr;
} arena_s;


arena_s* initialize_arena(size_t capacity);
void* alloc_arena(arena_s* arena, size_t size);
void reset_arena(arena_s* arena);
void destroy_arena(arena_s** arena); 
arena_chunk_s* initialize_arena_chunk(size_t capacity);
void destroy_arena_chunk(arena_chunk_s** chunk);
size_t align_16_bytes(size_t size);
#endif