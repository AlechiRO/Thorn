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
    token_list_destroy(&tokens);
}
/*
Helper function to initialize default token list
*/
static void set_up(void) {
    tokens = token_list_initialize();
    literal_s* one = initialize_literal(LITERAL_DOUBLE);
    one->value.double_value = 1;
    literal_s* two = initialize_literal(LITERAL_DOUBLE);
    two->value.double_value = 2;
    literal_s* three = initialize_literal(LITERAL_DOUBLE);
    three->value.double_value = 3;
    literal_s* seven = initialize_literal(LITERAL_DOUBLE);
    seven->value.double_value = 7;
    token_list_add(tokens, initialize_token(TOKEN_NUMBER, "1", one, 1));
    token_list_add(tokens, initialize_token(TOKEN_PLUS, "+", NULL, 1));
    token_list_add(tokens, initialize_token(TOKEN_NUMBER, "2", two, 1));
    token_list_add(tokens, initialize_token(TOKEN_STAR, "*", NULL, 1));
    token_list_add(tokens, initialize_token(TOKEN_NUMBER, "3", three, 1));
    token_list_add(tokens, initialize_token(TOKEN_EQUAL_EQUAL, "==", NULL, 1));
    token_list_add(tokens, initialize_token(TOKEN_NUMBER, "7", seven, 1));
    token_list_add(tokens, initialize_token(TOKEN_EOF, "", NULL, 1));
    pctx = initialize_parser_context(tokens);
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
    CU_ASSERT_PTR_NOT_NULL(pctx);
    CU_ASSERT_PTR_NOT_NULL(pctx->arena);
    CU_ASSERT_EQUAL(pctx->tokens, tokens);
    CU_ASSERT_EQUAL(pctx->current, 0);
    CU_ASSERT_EQUAL(pctx->had_error, 0);
}

void test_destroy_parser_context(void) {
    destroy_parser_context(&pctx);
    CU_ASSERT_PTR_NULL(pctx);
}

void test_p_peek_first_token(void) {
    token_s* curr = p_peek(pctx);
    CU_ASSERT_EQUAL(curr->type, TOKEN_NUMBER);
    CU_ASSERT_EQUAL(curr->literal->type, LITERAL_DOUBLE);
    CU_ASSERT_EQUAL(curr->literal->value.double_value, 1);
    CU_ASSERT_TRUE(strcmp(curr->lexeme, "1") == 0);
}

void test_p_peek_end_of_list(void) {
    pctx->current = token_list_get_size(pctx->tokens);
    token_s* curr = p_peek(pctx);
    CU_ASSERT_PTR_NULL(curr);
}

void test_p_previous_first_token(void) {
    token_s* curr = p_previous(pctx);
    CU_ASSERT_EQUAL(curr->type, TOKEN_NUMBER);
    CU_ASSERT_EQUAL(curr->literal->type, LITERAL_DOUBLE);
    CU_ASSERT_EQUAL(curr->literal->value.double_value, 1);
    CU_ASSERT_TRUE(strcmp(curr->lexeme, "1") == 0);
}

void test_p_previous_default(void) {
    pctx->current = 4;
    token_s* curr = p_previous(pctx);
    CU_ASSERT_EQUAL(curr->type, TOKEN_STAR);
    CU_ASSERT_TRUE(strcmp(curr->lexeme, "*") == 0);
}

void test_p_is_at_end_true(void) {
    pctx->current = token_list_get_size(pctx->tokens) - 1;
    CU_ASSERT_TRUE(p_is_at_end(pctx));
}

void test_p_is_at_end_false(void) {
    pctx->current = 4;
    CU_ASSERT_FALSE(p_is_at_end(pctx));
}

void test_p_advance_default(void) {
    pctx->current = 3;
    token_s* token = p_advance(pctx);
    CU_ASSERT_EQUAL(pctx->current, 4);
    CU_ASSERT_EQUAL(token->type, TOKEN_STAR);
    CU_ASSERT_TRUE(strcmp(token->lexeme, "*") == 0);
}

void test_p_advance_end_of_list(void) {
    pctx->current = token_list_get_size(pctx->tokens) - 1;
    token_s* token = p_advance(pctx);
    CU_ASSERT_EQUAL(pctx->current, token_list_get_size(pctx->tokens) - 1);
    CU_ASSERT_EQUAL(token->type, TOKEN_NUMBER);
    CU_ASSERT_TRUE(strcmp(token->lexeme, "7") == 0);
}

void test_p_check_default_true(void) {
    CU_ASSERT_TRUE(p_check(pctx, TOKEN_NUMBER));
}

void test_p_check_default_false(void) {
    CU_ASSERT_FALSE(p_check(pctx, TOKEN_STRING_DEFAULT));
}

void test_p_check_default_end_of_list(void) {
    pctx->current = token_list_get_size(pctx->tokens) - 1;
    CU_ASSERT_FALSE(p_check(pctx, EOF));
}

void test_p_match_one_token_true(void) {
    pctx->current = 5;
    CU_ASSERT_TRUE(p_match(pctx, (token_type_e[]){TOKEN_EQUAL_EQUAL}, 1));
}

void test_p_match_two_tokens_true(void) {
    pctx->current = 5;
    CU_ASSERT_TRUE(p_match(pctx, (token_type_e[]){TOKEN_APPEND_REDIRECT, TOKEN_EQUAL_EQUAL}, 2));
}

void test_p_match_two_tokens_false(void) {
    pctx->current = 5;
    CU_ASSERT_FALSE(p_match(pctx, (token_type_e[]){TOKEN_APPEND_REDIRECT, TOKEN_EQUAL}, 2));
}

void test_p_consume_matching_token(void) {
    pctx->current = 5;
    token_s* token = p_consume(pctx, TOKEN_EQUAL_EQUAL, "Unexpected token!");
    CU_ASSERT_EQUAL(pctx->current, 6);
    CU_ASSERT_EQUAL(token->type, TOKEN_EQUAL_EQUAL);
}

void test_p_consume_unexpected_token(void) {
    pctx->current = 5;
    if(setjmp(pctx->panic_jmp) == 0) {
        token_s* token = p_consume(pctx, TOKEN_ROUND_BRACE_RIGHT, "Unexpected token!");
    } else {
        CU_ASSERT_TRUE(1);
    }
}




int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry"); 

    /* Initialize_parser_context suite */
    CU_pSuite initialize_parser_context_suite = create_suite("initialize_parser_context suite", set_up, clean_up);
    CU_add_test(initialize_parser_context_suite, "initialize parser context", test_initialize_parser_context);

    /* Destroy_parser_context suite */
    CU_pSuite destroy_parser_context_suite = create_suite("destroy_parser_context suite", set_up, NULL);
    CU_add_test(destroy_parser_context_suite, "destroy parser context", test_destroy_parser_context);

    /* P_peek suite */
    CU_pSuite p_peek_suite = create_suite("p_peek suite", set_up, clean_up);
    CU_add_test(p_peek_suite, "p_peek first token", test_p_peek_first_token);
    CU_add_test(p_peek_suite, "p_peek end of list", test_p_peek_end_of_list);

    /* P_previous suite */
    CU_pSuite p_previous_suite = create_suite("p_previous suite", set_up, clean_up);
    CU_add_test(p_previous_suite, "p_previous first token", test_p_previous_first_token);
    CU_add_test(p_previous_suite, "p_previous default", test_p_previous_default);

    /* P_is_at_end suite */
    CU_pSuite p_is_at_end_suite = create_suite("p_is_at_end suite", set_up, clean_up);
    CU_add_test(p_is_at_end_suite, "p_is_at_end true", test_p_is_at_end_true);
    CU_add_test(p_is_at_end_suite, "p_is_at_end false", test_p_is_at_end_false);

    /* P_advance suite */
    CU_pSuite p_advance_suite = create_suite("p_advance suite", set_up, clean_up);
    CU_add_test(p_advance_suite, "p_advance default", test_p_advance_default);
    CU_add_test(p_advance_suite, "p_advance end of list", test_p_advance_end_of_list);

    /* P_check suite */
    CU_pSuite p_check_suite = create_suite("p_check suite", set_up, clean_up);
    CU_add_test(p_check_suite, "p_check default true", test_p_check_default_true);
    CU_add_test(p_check_suite, "p_check default false", test_p_check_default_false);
    CU_add_test(p_check_suite, "p_check end of list", test_p_check_default_end_of_list);

    /* P_match suite */
    CU_pSuite p_match_suite = create_suite("p_match suite", set_up, clean_up);
    CU_add_test(p_match_suite, "p_match one token true", test_p_match_one_token_true);
    CU_add_test(p_match_suite, "p_match two tokens true", test_p_match_two_tokens_true);
    CU_add_test(p_match_suite, "p_match two tokens false", test_p_match_two_tokens_false);

    /* P_consume suite */
    CU_pSuite p_consume_suite = create_suite("p_consume suite", set_up, clean_up);
    CU_add_test(p_consume_suite, "p_consume matching token", test_p_consume_matching_token);
    CU_add_test(p_consume_suite, "p_consume unexpected token", test_p_consume_unexpected_token);

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}