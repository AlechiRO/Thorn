#include <stdlib.h>
#include "arena.h"


/*
Arena Chunk Constructor
@param capacity Chunk capacity
@return Pointer to arena chunk struct
*/
arena_chunk_s* initialize_arena_chunk(size_t capacity) {
    capacity = align_16_bytes(capacity);
    arena_chunk_s* chunk = malloc(sizeof(arena_chunk_s) + capacity);
    if(chunk == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for arena chunk!\n");
        exit(EXIT_FAILURE);
    }
    chunk->next = NULL;
    chunk->capacity = capacity;
    chunk->offset = 0;
    return chunk;
}

/*
Arena Chunk Destructor 
@param chunk Pointer to the address of a arena chunk struct
*/
void destroy_arena_chunk(arena_chunk_s** chunk) {
    if(chunk == NULL || (*chunk) == NULL) {
        fprintf(stderr, "INFO: Pointer provided to arena chunk destructor was NULL!\n");
        return;
    }
    free(*chunk);
    (*chunk) = NULL;
}


/*
Arena Constructor 
@param capacity Arena capacity
@return Pointer to arena struct
*/
arena_s* initialize_arena(size_t capacity) {
    arena_s* arena = malloc(sizeof(arena_s));
    if(arena == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for arena!\n");
        exit(EXIT_FAILURE);
    }
    arena_chunk_s* chunk = initialize_arena_chunk(DEFAULT_ARENA_CHUNK_SIZE > capacity ? DEFAULT_ARENA_CHUNK_SIZE : capacity);
    arena->head = chunk;
    arena-> curr = chunk;
    return arena;
}

/*
Arena Destructor
Resets the arena and frees the head
@param Pointer to the address of an arena struct
*/
void destroy_arena(arena_s** arena) {
    if(arena == NULL || (*arena) == NULL) {
        fprintf(stderr, "INFO: Pointer provided to arena destructor was NULL!\n");
        return;
    }
    reset_arena(*arena);
    destroy_arena_chunk(&((*arena)->head));
    free(*arena);
    (*arena) = NULL;
}

/*
Reset the arena to the default state
@param arena Pointer to an arena struct
*/
void reset_arena(arena_s* arena) {
    if (arena == NULL || arena->head == NULL) 
        return;
    // Destroy all the chunks after the first chunk
    arena_chunk_s* curr = arena->head->next;
    while(curr != NULL) {
        arena_chunk_s* next = curr->next;
        destroy_arena_chunk(&curr);
        curr = next;
    }
    arena->head->offset = 0;
    arena->head->next = NULL;
    arena->curr = arena->head;
}

/*
Provides the smalles multiple of 16 larger than the provided size
@param size Size to be aligned
@return 16 byte aligned size
*/
size_t align_16_bytes(size_t size) {
    if(size == 0) 
        return 0;
    return (((size-1)>>4)<<4)+16;
}


/*
Allocate memory from a chunk
@param arena The memory arena
@param size Size to be allocated
@return Pointer to the allocated memory
*/
void* alloc_arena(arena_s* arena, size_t size) {
    if (arena == NULL || size == 0) 
        return NULL;
    // Align the size to 16 bytes
    arena_chunk_s* chunk = arena->curr;
    size = align_16_bytes(size);
    if(arena->curr->capacity - arena->curr->offset < size) {
        arena->curr->next = initialize_arena_chunk(DEFAULT_ARENA_CHUNK_SIZE > size ? DEFAULT_ARENA_CHUNK_SIZE : size);
        chunk = arena->curr->next;
        arena->curr = chunk;
    }

    void* result = &(chunk->data[chunk->offset]);
    chunk->offset += size;    
    return result;
}
