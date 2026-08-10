#include <stdio.h>
#include <AST.h>

/*
Expression number literal constructor
@param val Double value of the literal
@return Pointer to an expr struct
*/
expr_s* expr_literal_num_initialize(double val) {
    expr_s* expr = malloc(sizeof(expr_s));

    if(expr == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for expr struct!\n");
        exit(EXIT_FAILURE);
    }

    expr->type = EXPR_LITERAL; 
    expr->expression.literal = (literal_expr_s) {
        .payload.number = val, 
        .type = EXPR_LITERAL_NUMBER
    };
    
    return expr;
}

/*
Expression string literal constructor
@param val String value of the literal
@return Pointer to an expr struct
*/
expr_s* expr_literal_str_initialize(char* val) {
    expr_s* expr = malloc(sizeof(expr_s));

    if(expr == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for expr struct!\n");
        exit(EXIT_FAILURE);
    }

    expr->type = EXPR_LITERAL; 
    expr->expression.literal = (literal_expr_s) {
        .payload.string = val, 
        .type = EXPR_LITERAL_STRING
    };
    
    return expr;
}

/*
Expression boolean literal constructor
@param val Boolean value of the literal
@return Pointer to an expr struct
*/
expr_s* expr_literal_bool_initialize(int val) {
    expr_s* expr = malloc(sizeof(expr_s));

    if(expr == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for expr struct!\n");
        exit(EXIT_FAILURE);
    }

    expr->type = EXPR_LITERAL; 
    expr->expression.literal = (literal_expr_s) {
        .payload.boolean = val, 
        .type = EXPR_LITERAL_BOOLEAN
    };
    
    return expr;
}

/*
Expression null literal constructor
@return Pointer to an expr struct
*/
expr_s* expr_literal_null_initialize(void) {
    expr_s* expr = malloc(sizeof(expr_s));

    if(expr == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for expr struct!\n");
        exit(EXIT_FAILURE);
    }

    expr->type = EXPR_LITERAL; 
    expr->expression.literal = (literal_expr_s) { 
        .type = EXPR_LITERAL_NUMBER
    };
    
    return expr;
}