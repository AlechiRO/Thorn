#include <stdio.h>
#include <stdlib.h>
#include "AST.h"



/* 
Helper method to help with allocating memory for expressions
@param type Expression type
@return Pointer to an expression struct
*/
static expr_s* expr_alloc(expr_type_e type, arena_s* arena) {
    expr_s* expr = alloc_arena(arena, sizeof(expr_s));
    if (expr == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for expr struct!\n");
        exit(EXIT_FAILURE);
    }
    expr->type = type;
    return expr;
}

/*
Expression number literal constructor
@param val Double value of the literal
@return Pointer to an expr struct
*/
expr_s* initialize_expr_literal_num(double val, arena_s* arena) {
    expr_s* expr = expr_alloc(EXPR_LITERAL, arena); 
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
expr_s* initialize_expr_literal_str(char* val, arena_s* arena) {
    expr_s* expr = expr_alloc(EXPR_LITERAL, arena); 
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
expr_s* initialize_expr_literal_bool(int val, arena_s* arena) {
    expr_s* expr = expr_alloc(EXPR_LITERAL, arena); 
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
expr_s* initialize_expr_literal_null(arena_s* arena) {
    expr_s* expr = expr_alloc(EXPR_LITERAL, arena); 
    expr->expression.literal = (literal_expr_s) { 
        .type = EXPR_LITERAL_NULL
    };
    
    return expr;
}

/*
Expression unary constructor
@param op Token denoting the unary operator
@param right Pointer to the right expr struct
@return Pointer to an expr struct
*/
expr_s* initialize_expr_unary(const token_s* op, expr_s* right, arena_s* arena) {
    expr_s* expr = expr_alloc(EXPR_UNARY, arena); 
    expr->expression.unary = (unary_expr_s) { 
        .op = op,
        .right = right
    };
    
    return expr;
}

/*
Expression binary constructor
@param left Pointer to the left expr struct
@param op Token denoting the binary operator
@param right Pointer to the right expr struct
@return Pointer to an expr struct
*/
expr_s* initialize_expr_binary(expr_s* left, const token_s* op, expr_s* right, arena_s* arena) {
    expr_s* expr = expr_alloc(EXPR_BINARY, arena); 
    expr->expression.binary = (binary_expr_s) { 
        .left = left,
        .op = op,
        .right = right
    };
    
    return expr;
}


/*
Expression grouping constructor
@param e Pointer to the expression in the grouping
@return Pointer to an expr struct
*/
expr_s* initialize_expr_grouping(expr_s* e, arena_s* arena) {
    expr_s* expr = expr_alloc(EXPR_GROUPING, arena); 
    expr->expression.grouping = (grouping_expr_s) { 
        .expr = e
    };
    
    return expr;
}
