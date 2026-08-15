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
    arena = initialize_arena(0);
}
/*
Helper function to free the memory used for arena
*/
static void set_up(void) {
    destroy_arena(&arena);
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


int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry"); 

    /* Align_16_bytes suite */
    CU_pSuite align_16_bytes_suite = create_suite("align_16_bytes suite", NULL, NULL);
    CU_add_test(align_16_bytes_suite, "align zero bytes", test_align_zero_bytes);
    CU_add_test(align_16_bytes_suite, "align to 16 bytes", test_align_16_bytes);



    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}