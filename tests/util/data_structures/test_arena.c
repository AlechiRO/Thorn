#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <err.h>
#include <string.h>
#include "arena.h"

arena_s* arena;


/*
Helper function to initialize arena to default size
*/
static void clean_up(void) {
    destroy_arena(&arena);
}
/*
Helper function to free the memory used for arena
*/
static void set_up(void) {
    arena = initialize_arena(0);
}

/* 
Helper function to create a suite
@param name Pointer to the name of the suite
@return CUnit suite object
*/
static CU_pSuite create_suite(const char* name,  void(*set_up)(),  void(*tear)()) {
    CU_pSuite suite = CU_add_suite_with_setup_and_teardown(name, NULL, NULL, set_up, tear); 
    if (CU_get_error() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "%s", CU_get_error_msg());
    return suite;
}

void test_align_zero_bytes(void) {
    CU_ASSERT_EQUAL(align_16_bytes(0), 0);
}

void test_align_16_bytes(void) {
    CU_ASSERT_EQUAL(align_16_bytes(15), 16);
    CU_ASSERT_EQUAL(align_16_bytes(40), 48);
    CU_ASSERT_EQUAL(align_16_bytes(160), 160);
}

void test_initialize_arena_chunk_aligned(void) {
    arena_chunk_s* chunk = initialize_arena_chunk(1024);
    CU_ASSERT_PTR_NOT_NULL(chunk);
    CU_ASSERT_PTR_NULL(chunk->next);
    CU_ASSERT_EQUAL(chunk->capacity, 1024);
    CU_ASSERT_EQUAL(chunk->offset, 0);
    destroy_arena_chunk(&chunk);
}

void test_initialize_arena_chunk_not_aligned(void) {
    arena_chunk_s* chunk = initialize_arena_chunk(506);
    CU_ASSERT_PTR_NOT_NULL(chunk);
    CU_ASSERT_PTR_NULL(chunk->next);
    CU_ASSERT_EQUAL(chunk->capacity, 512);
    CU_ASSERT_EQUAL(chunk->offset, 0);
    destroy_arena_chunk(&chunk);
}

void test_destroy_arena_chunk(void) {
    arena_chunk_s* chunk = initialize_arena_chunk(1024);
    destroy_arena_chunk(&chunk);
    CU_ASSERT_PTR_NULL(chunk);
}

void test_destroy_arena_chunk_NULL(void) {
    arena_chunk_s* chunk = NULL;
    destroy_arena_chunk(&chunk);
    CU_ASSERT_PTR_NULL(chunk);
}

void test_initialize_arena_default(void) {
    arena = initialize_arena(1024);
    CU_ASSERT_PTR_NOT_NULL(arena);
    arena_chunk_s* head = arena->head;
    CU_ASSERT_PTR_NULL(head->next);
    CU_ASSERT_PTR_NOT_NULL(head);
    CU_ASSERT_EQUAL(head->capacity, 64 * 1024);
    CU_ASSERT_EQUAL(head->offset, 0); 
}

void test_initialize_arena_more_than_default(void) {
    arena = initialize_arena(128 * 1024);
    CU_ASSERT_PTR_NOT_NULL(arena);
    arena_chunk_s* head = arena->head;
    CU_ASSERT_PTR_NULL(head->next);
    CU_ASSERT_PTR_NOT_NULL(head);
    CU_ASSERT_EQUAL(head->capacity, 128 * 1024);
    CU_ASSERT_EQUAL(head->offset, 0); 
}

void test_destroy_arena(void) {
    arena = initialize_arena(1024);
    destroy_arena(&arena);
    CU_ASSERT_PTR_NULL(arena);
}

void test_destroy_arena_NULL(void) {
    arena = NULL;
    destroy_arena(&arena);
    CU_ASSERT_PTR_NULL(arena);
}

void test_alloc_arena_small(void) {
    void* object = alloc_arena(arena, 18 * 16 + 4);
    CU_ASSERT_PTR_NOT_NULL(object);
    CU_ASSERT_EQUAL(arena->head->offset, 19 * 16);
    CU_ASSERT_EQUAL(object, arena->head->data);
}

void test_alloc_arena_double_small(void) {
    void* object = alloc_arena(arena, 10 * 16);
    object = alloc_arena(arena, 20 * 16);
    CU_ASSERT_PTR_NOT_NULL(object);
    CU_ASSERT_EQUAL(arena->head->offset, 30 * 16);
    CU_ASSERT_EQUAL(object, arena->head->data + 10 * 16);
}

void test_alloc_arena_large(void) {
    void* object = alloc_arena(arena, 100 * 1024);
    CU_ASSERT_PTR_NOT_NULL(object);
    CU_ASSERT_EQUAL(arena->head->next->offset, 100 * 1024);
    CU_ASSERT_EQUAL(object, arena->head->next->data);
}

void test_reset_arena_small(void) {
    void* object = alloc_arena(arena, 1024);
    reset_arena(arena);
    CU_ASSERT_EQUAL(arena->head->offset, 0);
}

void test_reset_arena_three_chunks(void) {
    void* object = alloc_arena(arena, 1024);
    object = alloc_arena(arena, 100 * 1024);
    object = alloc_arena(arena, 1024);
    reset_arena(arena);
    CU_ASSERT_EQUAL(arena->head->offset, 0);
    CU_ASSERT_PTR_NULL(arena->head->next);
}

int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry"); 

    /* Align_16_bytes suite */
    CU_pSuite align_16_bytes_suite = create_suite("align_16_bytes suite", NULL, NULL);
    CU_add_test(align_16_bytes_suite, "align zero bytes", test_align_zero_bytes);
    CU_add_test(align_16_bytes_suite, "align to 16 bytes", test_align_16_bytes);

    /* Initialize_arena_chunk suite */
    CU_pSuite initialize_arena_chunk_suite = create_suite("initialize_arena_chunk suite", NULL, NULL);
    CU_add_test(initialize_arena_chunk_suite, "initialize arena chunk aligned", test_initialize_arena_chunk_aligned);
    CU_add_test(initialize_arena_chunk_suite, "initialize arena chunk not aligned", test_initialize_arena_chunk_not_aligned);

    /* Destroy_arena_chunk suite */
    CU_pSuite destroy_arena_chunk_suite = create_suite("destroy_arena_chunk suite", NULL, NULL);
    CU_add_test(destroy_arena_chunk_suite, "destroy arena chunk", test_destroy_arena_chunk);
    CU_add_test(destroy_arena_chunk_suite, "destroy arena chunk NULL", test_destroy_arena_chunk_NULL);

    /* Initialize_arena suite */
    CU_pSuite initialize_arena_suite = create_suite("initialize_arena suite", NULL, clean_up);
    CU_add_test(initialize_arena_suite, "initialize arena default", test_initialize_arena_default);
    CU_add_test(initialize_arena_suite, "initialize arena more than default", test_initialize_arena_more_than_default);

    /* Destroy_arena suite */
    CU_pSuite destroy_arena_suite = create_suite("destroy_arena_suite", NULL, NULL);
    CU_add_test(destroy_arena_suite, "destroy arena", test_destroy_arena);
    CU_add_test(destroy_arena_suite, "destroy arena NULL", test_destroy_arena_NULL);
    
    /* Alloc_arena suite */
    CU_pSuite alloc_arena_suite = create_suite("alloc_arena suite", set_up, clean_up);
    CU_add_test(alloc_arena_suite, "alloc arena small", test_alloc_arena_small); 
    CU_add_test(alloc_arena_suite, "alloc arena two small sizes", test_alloc_arena_double_small);
    CU_add_test(alloc_arena_suite, "alloc arena large", test_alloc_arena_large);

    CU_pSuite reset_arena_suite = create_suite("reset_arena suite", set_up, clean_up);
    CU_add_test(reset_arena_suite, "reset arena small", test_reset_arena_small);
    CU_add_test(reset_arena_suite, "reset arena three chunks", test_reset_arena_three_chunks);
    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}