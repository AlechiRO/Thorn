#include <stdlib.h>
#include "arena.h"

/*
Arena Chunk Constructor
@param capacity Chunk capacity
@return Pointer to arena chunk struct
*/
arena_chunk_s* intialize_arena_chunk(size_t capacity) {
    arena_chunk_s* chunk = malloc(sizeof(arena_chunk_s) + capacity);
    if(chunk == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for arena chunk!\n");
        exit(EXIT_FAILURE);
    }
    chunk->next = NULL;
    chunk->capacity = capacity;
    chunk->offset = 0;
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
arena_s* intialize_arena(size_t capacity) {
    arena_s* arena = malloc(sizeof(arena_s));
    if(arena == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for arena!\n");
        exit(EXIT_FAILURE);
    }
    arena_chunk_s* chunk = intialize_arena_chunk(DEFAULT_ARENA_CHUNK_SIZE > capacity ? DEFAULT_ARENA_CHUNK_SIZE : capacity);
    arena->head = chunk;
    arena-> curr = chunk;
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
    // Destroy all the chunks after the first chunk
    arena_chunk_s* curr = arena->head->next;
    while(curr != NULL) {
        arena_chunk_s* next = curr->next;
        destroy_arena_chunk(&curr);
        curr = next;
    }
    arena->head->offset = 0;
    arena->head->next = NULL;
}
