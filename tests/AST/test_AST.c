#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <err.h>
#include <string.h>
#include "AST.h"

arena_s* arena;


/*
Helper function to free the memory used for arena
*/
static void clean_up(void) {
    destroy_arena(&arena);
}
/*
Helper function to initialize arena to default size
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

void test_initialize_expr_literal_num(void) {
    expr_s* expr = initialize_expr_literal_num(10, arena);
    CU_ASSERT_EQUAL(expr->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(expr->expression.literal.type, EXPR_LITERAL_NUMBER);
    CU_ASSERT_EQUAL(expr->expression.literal.payload.number, 10);
}

void test_initialize_expr_literal_str(void) {
    expr_s* expr = initialize_expr_literal_str("ascending the throne", arena);
    CU_ASSERT_EQUAL(expr->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(expr->expression.literal.type, EXPR_LITERAL_STRING);
    CU_ASSERT_TRUE(strcmp(expr->expression.literal.payload.string, "ascending the throne") == 0);
}

void test_initialize_expr_literal_bool(void) {
    expr_s* expr = initialize_expr_literal_bool(0, arena);
    CU_ASSERT_EQUAL(expr->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(expr->expression.literal.type, EXPR_LITERAL_BOOLEAN);
    CU_ASSERT_EQUAL(expr->expression.literal.payload.boolean, 0);
}

void test_initialize_expr_literal_null(void) {
    expr_s* expr = initialize_expr_literal_null(arena);
    CU_ASSERT_EQUAL(expr->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(expr->expression.literal.type, EXPR_LITERAL_NULL);
}

void test_initialize_expr_unary(void) {
    expr_s* right = initialize_expr_literal_num(10, arena);
    token_s* op = initialize_token(TOKEN_MINUS, "-", NULL, 1);
    expr_s* expr = initialize_expr_unary(op, right, arena);
    CU_ASSERT_EQUAL(expr->type, EXPR_UNARY);
    CU_ASSERT_EQUAL(expr->expression.unary.op, op);
    CU_ASSERT_EQUAL(expr->expression.unary.right, right);
    destroy_token(&op);
}

void test_initialize_expr_binary(void) {
    expr_s* left = initialize_expr_literal_num(1, arena);
    token_s* op = initialize_token(TOKEN_PLUS, "+", NULL, 1);
    expr_s* right = initialize_expr_literal_num(1, arena);
    expr_s* expr = initialize_expr_binary(left, op, right, arena);
    CU_ASSERT_EQUAL(expr->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(expr->expression.binary.left, left);
    CU_ASSERT_EQUAL(expr->expression.binary.op, op);
    CU_ASSERT_EQUAL(expr->expression.binary.right, right);
    destroy_token(&op);
}   

void test_initialize_expr_grouping(void) {
    expr_s* inner = initialize_expr_literal_num(2, arena);
    expr_s* expr = initialize_expr_grouping(inner, arena);
    CU_ASSERT_EQUAL(expr->type, EXPR_GROUPING);
    CU_ASSERT_EQUAL(expr->expression.grouping.expr, inner);
}

int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry"); 

    /* Initialize_expr_literal_num suite */
    CU_pSuite initialize_expr_literal_num_suite = create_suite("initialize_expr_literal suite", set_up, clean_up);
    CU_add_test(initialize_expr_literal_num_suite, "initialize expr literal number", test_initialize_expr_literal_num);

    /* Initialize_expr_literal_str suite */
    CU_pSuite initialize_expr_literal_str_suite = create_suite("initialize_expr_literal_str suite", set_up, clean_up);
    CU_add_test(initialize_expr_literal_str_suite, "initialize_expr_literal_str", test_initialize_expr_literal_str);
    
    /* Initialize_expr_literal_bool suite */
    CU_pSuite initialize_expr_litera_bool_suite = create_suite("initialize_expr_litera_bool suite", set_up, clean_up);
    CU_add_test(initialize_expr_litera_bool_suite, "initialize expr literal bool", test_initialize_expr_literal_bool);
    
    /* Initialize_expr_literal_null suite */
    CU_pSuite initialize_expr_literal_null_suite = create_suite("initialize_expr_literal_null suite", set_up, clean_up);
    CU_add_test(initialize_expr_literal_null_suite, "initialize expr literal null", test_initialize_expr_literal_null);

    /* Initialize_expr_unary suite */
    CU_pSuite initialize_expr_unary_suite = create_suite("initialize_expr_unary suite", set_up, clean_up);
    CU_add_test(initialize_expr_unary_suite, "initialize expr unary", test_initialize_expr_unary);

    /* Initialize_expr_binary suite */
    CU_pSuite initialize_expr_binary_suite = create_suite("initialize_expr_binary suite", set_up, clean_up);
    CU_add_test(initialize_expr_binary_suite, "initialize expr binary", test_initialize_expr_binary);

    /* Initialize_expr_grouping suite */
    CU_pSuite initialize_expr_grouping_suite = create_suite("initialize_expr_grouping suite", set_up, clean_up);
    CU_add_test(initialize_expr_grouping_suite, "initialize expr grouping", test_initialize_expr_grouping);

    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}