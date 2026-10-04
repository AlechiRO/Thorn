#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <CUnit/Basic.h>
#include <CUnit/CUnit.h>
#include <err.h>
#include <string.h>
#include "parser_util.h"

/*
Helper method to create a literal dummy object
@param type Type of literal
@return Pointer to a dummy literal object
*/
static literal_s* make_dummy_literal(literal_type_e type) {
    literal_s* literal = initialize_literal(type);
    switch (type) {
    case LITERAL_DOUBLE:
        literal->value.double_value = 0.0;
        break;
    case LITERAL_BOOLEAN:
        literal->value.boolean_value = 0;
    default:
        break;
    }
    return literal;
}
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
static void set_up(token_type_e types[], size_t size) {
    tokens = token_list_initialize();
    for(size_t i = 0; i < size; i++) {
        literal_s* literal = NULL;
        switch (types[i]) {
        case TOKEN_NUMBER:
            literal = make_dummy_literal(LITERAL_DOUBLE);
            break;
        case TOKEN_FALSE:
        case TOKEN_TRUE:
            literal = make_dummy_literal(LITERAL_BOOLEAN);
            break;
        default:
            break;
        }
        token_list_add(tokens, initialize_token(types[i], "", literal, 1));
    }

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

void test_primary_literal_number(void) {
    /* 
    Token Stream: 1 EOF
    */
    set_up((token_type_e[]){TOKEN_NUMBER}, 1);

    expr_s* e = primary(pctx);
    CU_ASSERT_EQUAL(e->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(e->expression.literal.type, EXPR_LITERAL_NUMBER); 
}

void test_primary_literal_boolean(void) {
    /* 
    Token Stream: false EOF
    */
    set_up((token_type_e[]){TOKEN_FALSE}, 1);

    expr_s* e = primary(pctx);
    CU_ASSERT_EQUAL(e->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(e->expression.literal.type, EXPR_LITERAL_BOOLEAN);
}

void test_primary_grouping(void) {
    /* 
    Token Stream: (-2 + 1 * 4) * EOF
    */
    set_up((token_type_e[]){TOKEN_ROUND_BRACE_LEFT, TOKEN_MINUS, TOKEN_NUMBER,
         TOKEN_PLUS, TOKEN_NUMBER, TOKEN_STAR, TOKEN_NUMBER, TOKEN_ROUND_BRACE_RIGHT, TOKEN_STAR}, 10);

    expr_s* e = primary(pctx);
    CU_ASSERT_EQUAL(pctx->current, 8);
    CU_ASSERT_EQUAL(e->type, EXPR_GROUPING);

    binary_expr_s binary1 = e->expression.grouping.expr->expression.binary;

    CU_ASSERT_EQUAL(binary1.left->type, EXPR_UNARY);
    CU_ASSERT_EQUAL(binary1.left->expression.unary.op->type, TOKEN_MINUS);

    CU_ASSERT_EQUAL(binary1.op->type, TOKEN_PLUS);
    CU_ASSERT_EQUAL(binary1.right->type, EXPR_BINARY);
    binary_expr_s binary2 = binary1.right->expression.binary;

    CU_ASSERT_EQUAL(binary2.op->type, TOKEN_STAR);

}

void test_primary_unclosed_parentheses(void) {
    /* 
    Token Stream: (-2 + 1 EOF
    */
    set_up((token_type_e[]){TOKEN_ROUND_BRACE_LEFT, TOKEN_MINUS, TOKEN_NUMBER, TOKEN_PLUS, TOKEN_NUMBER}, 5);

    if(setjmp(pctx->panic_jmp) == 0) {
        primary(pctx);
    } else {
        CU_ASSERT_TRUE(pctx->had_error);
    }
}

void test_unary_minus(void) {
    /* 
    Token Stream: -2 EOF
    */
    set_up((token_type_e[]){TOKEN_MINUS, TOKEN_NUMBER}, 2);

    expr_s* e = unary(pctx);
    CU_ASSERT_EQUAL(e->type, EXPR_UNARY);
    CU_ASSERT_EQUAL(e->expression.unary.op->type, TOKEN_MINUS);

}

void test_unary_bang(void) {
    /*
    Token Stream: !!!true EOF
    */
    set_up((token_type_e[]){TOKEN_BANG, TOKEN_BANG, TOKEN_BANG, TOKEN_TRUE}, 4);

    expr_s* e1 = unary(pctx);
    CU_ASSERT_EQUAL(e1->type, EXPR_UNARY);
    CU_ASSERT_EQUAL(e1->expression.unary.op->type, TOKEN_BANG);

    expr_s* e2 = e1->expression.unary.right;
    CU_ASSERT_EQUAL(e2->type, EXPR_UNARY);
    CU_ASSERT_EQUAL(e2->expression.unary.op->type, TOKEN_BANG);

    expr_s* e3 = e2->expression.unary.right;
    CU_ASSERT_EQUAL(e3->type, EXPR_UNARY);
    CU_ASSERT_EQUAL(e3->expression.unary.op->type, TOKEN_BANG);

    expr_s* e4 = e3->expression.unary.right;
    CU_ASSERT_EQUAL(e4->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(e4->expression.literal.type, EXPR_LITERAL_BOOLEAN);
}

void test_factor_three_numbers(void) {
    /* 
    Token Stream: 2 * 1 / 4 EOF
    */
    set_up((token_type_e[]){TOKEN_NUMBER, TOKEN_STAR, TOKEN_NUMBER, TOKEN_SLASH, TOKEN_NUMBER}, 5);

    expr_s* e1 = factor(pctx);
    CU_ASSERT_EQUAL(e1->expression.binary.op->type, TOKEN_SLASH);
    CU_ASSERT_EQUAL(e1->type, EXPR_BINARY)
    CU_ASSERT_EQUAL(e1->expression.binary.left->type, EXPR_BINARY);

    expr_s* e2 = e1->expression.binary.left;
    CU_ASSERT_EQUAL(e2->expression.binary.left->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(e2->expression.binary.op->type, TOKEN_STAR);

    expr_s* e3 = e1->expression.binary.right;
}

void test_term_two_numbers(void) {
    /* 
    Token Stream: 2 + 1 EOF
    */
    set_up((token_type_e[]){TOKEN_NUMBER, TOKEN_PLUS, TOKEN_NUMBER}, 3);

    expr_s* e = term(pctx);

    CU_ASSERT_EQUAL(e->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(e->expression.binary.left->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(e->expression.binary.op->type, TOKEN_PLUS);
    CU_ASSERT_EQUAL(e->expression.binary.right->type, EXPR_LITERAL);

    expr_s* t1 = e->expression.binary.left;
    expr_s* t2 = e->expression.binary.right;

    CU_ASSERT_EQUAL(t1->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(t2->type, EXPR_LITERAL);
}   

void test_comparison_two_operators(void) {
    /*
    Token Stream: 2 < 1 >= 4 EOF
    */
    set_up((token_type_e[]){TOKEN_NUMBER, TOKEN_LESS, TOKEN_NUMBER, TOKEN_GREATER_EQUAL, TOKEN_NUMBER}, 5);

    expr_s* e1 = comparison(pctx);
    CU_ASSERT_EQUAL(e1->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(e1->expression.binary.op->type, TOKEN_GREATER_EQUAL);

    expr_s* e2 = e1->expression.binary.right;
    CU_ASSERT_EQUAL(e2->type, EXPR_LITERAL);

    expr_s* e3 = e1->expression.binary.left;
    CU_ASSERT_EQUAL(e3->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(e3->expression.binary.op->type, TOKEN_LESS);

    expr_s* e4 = e3->expression.binary.right;
    CU_ASSERT_EQUAL(e4->type, EXPR_LITERAL);

    expr_s* e5 = e3->expression.binary.left;
    CU_ASSERT_EQUAL(e5->type, EXPR_LITERAL);
}

void test_equality_two_operators(void) {
    /*
    Token Stream: 2 != 4 == 1 EOF
    */
    set_up((token_type_e[]){TOKEN_NUMBER, TOKEN_BANG_EQUAL, TOKEN_NUMBER, TOKEN_EQUAL_EQUAL, TOKEN_NUMBER}, 5);

    expr_s* e1 = equality(pctx);
    CU_ASSERT_EQUAL(e1->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(e1->expression.binary.op->type, TOKEN_EQUAL_EQUAL);

    expr_s* e2 = e1->expression.binary.right;
    CU_ASSERT_EQUAL(e2->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(e2->expression.literal.type, EXPR_LITERAL_NUMBER);

    expr_s* e3 = e1->expression.binary.left;
    CU_ASSERT_EQUAL(e3->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(e3->expression.binary.op->type, TOKEN_BANG_EQUAL);

    expr_s* e4 = e3->expression.binary.left;
    CU_ASSERT_EQUAL(e4->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(e4->expression.literal.type, EXPR_LITERAL_NUMBER);

    expr_s* e5 = e3->expression.binary.right;
    CU_ASSERT_EQUAL(e5->type, EXPR_LITERAL);
    CU_ASSERT_EQUAL(e5->expression.literal.type, EXPR_LITERAL_NUMBER);
}

void test_expression_comma(void) {
    /*
    Token Stream: 1 == 2, 4 < 0 EOF
    */
    set_up((token_type_e[]){TOKEN_NUMBER, TOKEN_EQUAL_EQUAL, TOKEN_NUMBER, TOKEN_COMMA, TOKEN_NUMBER, TOKEN_LESS, TOKEN_NUMBER}, 7);

    expr_s* expr_comma = expression(pctx);
    CU_ASSERT_EQUAL(expr_comma->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(expr_comma->expression.binary.op->type, TOKEN_COMMA);

    expr_s* expr_equal = expr_comma->expression.binary.left;
    CU_ASSERT_EQUAL(expr_equal->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(expr_equal->expression.binary.op->type, TOKEN_EQUAL_EQUAL);

    expr_s* expr_one = expr_equal->expression.binary.left;
    CU_ASSERT_EQUAL(expr_one->type, EXPR_LITERAL);

    expr_s* expr_two = expr_equal->expression.binary.right;
    CU_ASSERT_EQUAL(expr_two->type, EXPR_LITERAL);

    expr_s* expr_less = expr_comma->expression.binary.right;
    CU_ASSERT_EQUAL(expr_less->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(expr_less->expression.binary.op->type, TOKEN_LESS);

    expr_s* expr_four = expr_less->expression.binary.left;
    CU_ASSERT_EQUAL(expr_four->type, EXPR_LITERAL);

    expr_s* expr_zero = expr_less->expression.binary.right;
    CU_ASSERT_EQUAL(expr_zero->type, EXPR_LITERAL);

}

void test_synchronize_previous_token_is_terminator(void) {
    /*
    Token Stream: \n 1 + 2 EOF
    */
    set_up((token_type_e[]){TOKEN_TERMINATOR, TOKEN_NUMBER, TOKEN_PLUS, TOKEN_NUMBER}, 4);

    synchronize(pctx);
    CU_ASSERT_EQUAL(pctx->current, 1);
}

void test_synchronize_eventual_terminator(void) {
    /*
    Token Stream: 1 + 2 \n return EOF
    */
    set_up((token_type_e[]){TOKEN_NUMBER, TOKEN_PLUS, TOKEN_NUMBER, TOKEN_TERMINATOR, TOKEN_RETURN}, 5);

    synchronize(pctx);
    CU_ASSERT_EQUAL(pctx->current, 4);
}

void test_synchronize_keyword(void) {
    /*
    Token Stream: 1 + 2 return 4 EOF
    */
    set_up((token_type_e[]){TOKEN_NUMBER, TOKEN_PLUS, TOKEN_NUMBER, TOKEN_RETURN, TOKEN_NUMBER}, 5);

    synchronize(pctx);
    CU_ASSERT_EQUAL(pctx->current, 3);
}

void test_parse_default(void) {
    /* 
    Token Stream: (-2 + 1 * 4) * 5 / 3 != 10 == false EOF
    */
    set_up((token_type_e[]){
    TOKEN_ROUND_BRACE_LEFT,
    TOKEN_MINUS,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_NUMBER,
    TOKEN_STAR,
    TOKEN_NUMBER,
    TOKEN_ROUND_BRACE_RIGHT,
    TOKEN_STAR,
    TOKEN_NUMBER,
    TOKEN_SLASH,
    TOKEN_NUMBER,
    TOKEN_BANG_EQUAL,
    TOKEN_NUMBER,
    TOKEN_EQUAL_EQUAL,
    TOKEN_FALSE
    }, 16);
    expr_s* main_expr = parse(pctx);

    CU_ASSERT_EQUAL(main_expr->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(main_expr->expression.binary.op->type, TOKEN_EQUAL_EQUAL);

    expr_s* false_expr = main_expr->expression.binary.right;
    CU_ASSERT_EQUAL(false_expr->type, EXPR_LITERAL);

    expr_s* bang_equal_expr = main_expr->expression.binary.left;
    CU_ASSERT_EQUAL(bang_equal_expr->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(bang_equal_expr->expression.binary.op->type, TOKEN_BANG_EQUAL);
    
    expr_s* ten_expr = bang_equal_expr->expression.binary.right;
    CU_ASSERT_EQUAL(ten_expr->type, EXPR_LITERAL);

    expr_s* div_expr = bang_equal_expr->expression.binary.left;
    CU_ASSERT_EQUAL(div_expr->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(div_expr->expression.binary.op->type, TOKEN_SLASH);
    
    expr_s* three_expr = div_expr->expression.binary.right;
    CU_ASSERT_EQUAL(three_expr->type, EXPR_LITERAL);

    expr_s* multiply_expr_2 = div_expr->expression.binary.left;
    CU_ASSERT_EQUAL(multiply_expr_2->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(multiply_expr_2->expression.binary.op->type, TOKEN_STAR);

    expr_s* five_expr = multiply_expr_2->expression.binary.right;
    CU_ASSERT_EQUAL(five_expr->type, EXPR_LITERAL);

    expr_s* group_expr = multiply_expr_2->expression.binary.left;
    CU_ASSERT_EQUAL(group_expr->type, EXPR_GROUPING);

    expr_s* plus_expr = group_expr->expression.grouping.expr;
    CU_ASSERT_EQUAL(plus_expr->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(plus_expr->expression.binary.op->type, TOKEN_PLUS)

    expr_s* multiply_expr_1 = plus_expr->expression.binary.right;
    CU_ASSERT_EQUAL(multiply_expr_2->type, EXPR_BINARY);
    CU_ASSERT_EQUAL(multiply_expr_2->expression.binary.op->type, TOKEN_STAR);

    expr_s* one_expr = multiply_expr_1->expression.binary.left;
    CU_ASSERT_EQUAL(one_expr->type, EXPR_LITERAL);

    expr_s* four_expr = multiply_expr_1->expression.binary.right;
    CU_ASSERT_EQUAL(four_expr->type, EXPR_LITERAL);

    expr_s* unary_expr = plus_expr->expression.binary.left;
    CU_ASSERT_EQUAL(unary_expr->type, EXPR_UNARY);
    CU_ASSERT_EQUAL(unary_expr->expression.unary.op->type, TOKEN_MINUS);
    
    expr_s* two_expr = unary_expr->expression.unary.right;
    CU_ASSERT_EQUAL(two_expr->type, EXPR_LITERAL);
}

void test_parse_synchronize(void) {
    /*
    Token Stream: (1 <= 2 == 1 \n for(i = 0; i < 4; i++) EOF
    */
    set_up((token_type_e[]){
    TOKEN_ROUND_BRACE_LEFT,
    TOKEN_NUMBER,
    TOKEN_LESS_EQUAL,
    TOKEN_NUMBER,
    TOKEN_EQUAL_EQUAL,
    TOKEN_NUMBER,
    TOKEN_TERMINATOR,
    TOKEN_FOR,
    TOKEN_ROUND_BRACE_LEFT,
    TOKEN_IDENTIFIER,
    TOKEN_EQUAL,
    TOKEN_NUMBER,
    TOKEN_SEMICOLON,
    TOKEN_IDENTIFIER,
    TOKEN_LESS,
    TOKEN_NUMBER,
    TOKEN_SEMICOLON,
    TOKEN_IDENTIFIER,
    TOKEN_INCREMENT,
    TOKEN_ROUND_BRACE_RIGHT
}, 20);

    expr_s* expr = parse(pctx);
    CU_ASSERT_EQUAL(pctx->current, 7);

}

int main(void) {

    // initialize registry
    if (CU_initialize_registry() != CUE_SUCCESS)
        errx(EXIT_FAILURE, "can't initialize test registry"); 

    /* Primary suite */
    CU_pSuite primary_suite = create_suite("primary suite", NULL, clean_up);
    CU_add_test(primary_suite, "primary parse literal number", test_primary_literal_number);
    CU_add_test(primary_suite, "primary parse literal boolean", test_primary_literal_boolean);
    CU_add_test(primary_suite, "primary parse grouping", test_primary_grouping);

    /* Primary error suite */
    CU_pSuite primary_error_suite = create_suite("primary error suite", NULL, clean_up);
    CU_add_test(primary_error_suite, "primary error unclosed parentheses", test_primary_unclosed_parentheses);

    /* Unary suite */
    CU_pSuite unary_suite = create_suite("unary suite", NULL, clean_up);
    CU_add_test(unary_suite, "unary minus", test_unary_minus);
    CU_add_test(unary_suite, "unary multiple bangs", test_unary_bang);

    /* Factor suite */
    CU_pSuite factor_suite = create_suite("factor suite", NULL, clean_up);
    CU_add_test(factor_suite, "factor three numbers", test_factor_three_numbers);

    /* Term suite */
    CU_pSuite term_suite = create_suite("term suite", NULL, clean_up);
    CU_add_test(term_suite, "term two numbers", test_term_two_numbers);

    /* Comparison suite */
    CU_pSuite comparison_suite = create_suite("comparison suite", NULL, clean_up);
    CU_add_test(comparison_suite, "comparison two operators", test_comparison_two_operators);
    
    /* Equality suite */
    CU_pSuite equality_suite = create_suite("equality suite", NULL, clean_up);
    CU_add_test(equality_suite, "equality two operators", test_equality_two_operators);

    /* Expression suite */
    CU_pSuite expression_suite = create_suite("expression suite", NULL, clean_up);
    CU_add_test(expression_suite, "comma separated expressions", test_expression_comma);

    /* Synchronize Suite */
    CU_pSuite synchronize_suite = create_suite("synchronize suite", NULL, clean_up);
    CU_add_test(synchronize_suite, "synchronize previous token is terminator", test_synchronize_previous_token_is_terminator);
    CU_add_test(synchronize_suite, "synchronize eventual terminator", test_synchronize_eventual_terminator);
    CU_add_test(synchronize_suite, "synchronize keyword", test_synchronize_keyword);

    /* Parse suite */
    CU_pSuite parse_suite = create_suite("parse suite", NULL, clean_up);
    CU_add_test(parse_suite, "parse default", test_parse_default);
    CU_add_test(parse_suite, "parse synchronize error", test_parse_synchronize);
    
    
    // run the tests
    CU_basic_run_tests();

    // record the number of failures
    int failures = CU_get_number_of_failures();

    // clean the registry
    CU_cleanup_registry();
    return failures == 0 ? 0 : 1;
}