#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <err.h>
#include <string.h>
#include "token.h"


literal_s* literal;
token_s* token;

/*
Helper function to free the memory for the Token
*/
static void clean_up(void) {
    destroy_token(&token);
}
/*
Helper function to initialize the Token
*/
static void set_up(void) {
    token = initialize_token(LITERAL_DOUBLE, NULL, NULL, 0);
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

void test_initialize_literal(void) {
    literal = initialize_literal(LITERAL_DOUBLE);
    (literal->value).double_value = 10;
    CU_ASSERT_PTR_NOT_NULL(literal);
    CU_ASSERT_EQUAL(literal->type, LITERAL_DOUBLE);
    CU_ASSERT_DOUBLE_EQUAL((literal->value).double_value, 10, 0.001);
    destroy_literal(&literal);
}

void test_destroy_literal(void) {
    literal = initialize_literal(LITERAL_DOUBLE);
    destroy_literal(&literal);
    CU_ASSERT_PTR_NULL(literal);
}

void test_initialize_token(void) {
    literal = initialize_literal(LITERAL_INT_LIST);
    int_list* list = int_list_initialize();
    (literal->value).int_list_reference = list;

    char* lexeme = malloc(sizeof("my_list"));
    strcpy(lexeme, "my_list");

    token = initialize_token(TOKEN_IDENTIFIER, lexeme, literal, 10);
    CU_ASSERT_PTR_NOT_NULL(token);
    CU_ASSERT_EQUAL(token->type, TOKEN_IDENTIFIER);
    CU_ASSERT_EQUAL(token->lexeme, lexeme);
    CU_ASSERT_EQUAL(token->literal, literal);

    // Clean up int list
    int_list_destroy(&list);
}

void test_destroy_token(void) {
    literal = initialize_literal(LITERAL_DOUBLE);
    (literal->value).double_value = 42;

    char* lexeme = malloc(sizeof("what_is_the_meaning_of_life"));
    strcpy(lexeme, "what_is_the_meaning_of_life");

    token->type = TOKEN_IDENTIFIER;
    token->lexeme = lexeme;
    token->literal = literal;
    token->line = 10;
    destroy_token(&token);
    CU_ASSERT_PTR_NULL(token);
}

void test_destroy_token_null_literal(void) {
    char* lexeme = malloc(sizeof("Aemond"));
    strcpy(lexeme, "Aemond");

    token->type = TOKEN_IDENTIFIER;
    token->lexeme = lexeme;
    token->line = 10;
    destroy_token(&token);
    CU_ASSERT_PTR_NULL(token);

}

int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry"); 

    // initialize literal suite
    CU_pSuite initialize_literal_suite = create_suite("initialize literal suite", NULL, NULL);
    CU_add_test(initialize_literal_suite, "initialize literal", test_initialize_literal);

    // destroy literal suite
    CU_pSuite destroy_literal_suite = create_suite("destroy literal stuite", NULL, NULL);
    CU_add_test(destroy_literal_suite, "destroy literal", test_destroy_literal);

    // initialize token suite
    CU_pSuite initialize_token_suite = create_suite("initialize token suite", NULL, clean_up);
    CU_add_test(initialize_token_suite, "initialize token", test_initialize_token);

    // destroy token suite

    CU_pSuite destroy_token_suite = create_suite("destroy token suite", set_up, NULL);
    CU_add_test(destroy_token_suite, "destroy token", test_destroy_token);
    CU_add_test(destroy_token_suite, "destroy token null literal", test_destroy_token_null_literal);

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}