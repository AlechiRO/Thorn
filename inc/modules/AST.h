#ifndef AST_H
#define AST_H

#include "token.h"

typedef struct Expr expr_s;

// Literal
typedef enum Literal_type {
    LITERAL_NUMBER,
    LITERAL_STRING,
    LITERAL_NULL,
    LITERAL_BOOLEAN
} literal_type_e;

typedef struct Literal_expr {
    union Literal_u {
        double number;
        char* string;
        int boolean;
    } payload;
    literal_type_e type;
} literal_expr_s;


// Grouping 

typedef struct Grouping_expr {
    expr_s* expr;
} grouping_expr_s;

//Unary
typedef struct Unary_expr {
    token_s* op;
    expr_s* expr;
} unary_expr_s;

//Binary
typedef struct Binary_expr {
    expr_s* left;
    token_s* op;
    expr_s* right;
} binary_expr_s;


// Expression
typedef enum Expr_type {
    LITERAL,
    GROUPING,
    UNARY,
    BINARY,
} expr_type_e;

// General expression struct
typedef struct Expr {
    union Expression {
        literal_expr_s literal;
        grouping_expr_s grouping;
        unary_expr_s unary;
        binary_expr_s binary;
    } expression;
    expr_type_e type;
} expr_s;

#endif