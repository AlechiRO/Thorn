#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <err.h>
#include <string.h>
#include "parser_util.h"

parser_context_s* pctx;
token_list* tokens;

/*
Helper function to free memory for parser context
*/
static void clean_up(void) {
    destroy_parser_context(&pctx);
}
/*
Helper function 
*/
static void set_up(void) {

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

void test_initialize_parser_context(void) {
    tokens = token_list_initialize();
    token_list_add(tokens, initialize_token(TOKEN_PIPE, "|", NULL, 1));
    token_list_add(tokens, initialize_token(TOKEN_PLUS, "+", NULL, 1));
    pctx = initialize_parser_context(tokens);
    CU_ASSERT_PTR_NOT_NULL(pctx);
}



int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry"); 

    /* Initialize_parser_context suite */
    CU_pSuite initialize_parser_context_suite = create_suite("initialize_parser_context suite", NULL, clean_up);
    CU_add_test(initialize_parser_context_suite, "initialize parser context", test_initialize_parser_context);

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}