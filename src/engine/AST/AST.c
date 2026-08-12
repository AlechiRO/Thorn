#include <stdio.h>
#include <stdlib.h>
#include "AST.h"


/* 
Helper method to help with allocating memory for expressions
@param type Expression type
@return Pointer to an expression struct
*/
static expr_s* expr_alloc(expr_type_e type) {
    expr_s* expr = malloc(sizeof(expr_s));
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
expr_s* initialize_expr_literal_num(double val) {
    expr_s* expr = expr_alloc(EXPR_LITERAL); 
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
expr_s* initialize_expr_literal_str(char* val) {
    expr_s* expr = expr_alloc(EXPR_LITERAL); 
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
expr_s* initialize_expr_literal_bool(int val) {
    expr_s* expr = expr_alloc(EXPR_LITERAL); 
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
expr_s* initialize_expr_literal_null(void) {
    expr_s* expr = expr_alloc(EXPR_LITERAL); 
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
expr_s* initialize_expr_unary(const token_s* op, expr_s* right) {
    expr_s* expr = expr_alloc(EXPR_UNARY); 
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
expr_s* initialize_expr_binary(expr_s* left, const token_s* op, expr_s* right) {
    expr_s* expr = expr_alloc(EXPR_BINARY); 
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
expr_s* initialize_expr_grouping(expr_s* e) {
    expr_s* expr = expr_alloc(EXPR_GROUPING); 
    expr->expression.grouping = (grouping_expr_s) { 
        .expr = e
    };
    
    return expr;
}


/*
Recursive expression destructor
@param expr Pointer to the address of an expression struct
*/
void destroy_expr(expr_s** expr) {
    if(expr == NULL || (*expr) == NULL) {
        return;
    }
    expr_s* e = *expr;
    switch(e->type) {
        case EXPR_LITERAL : 
            break;
        case EXPR_GROUPING :
            destroy_expr(&(e->expression.grouping.expr));
            break;
        case EXPR_UNARY :
            destroy_expr(&(e->expression.unary.right));
            break;
        case EXPR_BINARY :
            destroy_expr(&(e->expression.binary.left));
            destroy_expr(&(e->expression.binary.right));
            break;
        default:
            break;
    } 
    free(e);
    *expr = NULL;    
}