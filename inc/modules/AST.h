#ifndef AST_H
#define AST_H

#include "token.h"

typedef struct Expr expr_s;

// Literal
typedef enum Expr_literal_type {
    EXPR_LITERAL_NUMBER,
    EXPR_LITERAL_STRING,
    EXPR_LITERAL_NULL,
    EXPR_LITERAL_BOOLEAN
} expr_literal_type_e;

typedef struct Literal_expr {
    union Literal_u {
        double number;
        char* string;
        int boolean;
    } payload;
    expr_literal_type_e type;
} literal_expr_s;


// Grouping 

typedef struct Grouping_expr {
    expr_s* expr;
} grouping_expr_s;

//Unary
typedef struct Unary_expr {
    const token_s* op;
    expr_s* right;
} unary_expr_s;

//Binary
typedef struct Binary_expr {
    expr_s* left;
    const token_s* op;
    expr_s* right;
} binary_expr_s;


// Expression
typedef enum Expr_type {
    EXPR_LITERAL,
    EXPR_GROUPING,
    EXPR_UNARY,
    EXPR_BINARY,
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

// Constructors

expr_s* expr_literal_num_initialize(double val);
expr_s* expr_literal_str_initialize(char* val);
expr_s* expr_literal_bool_initialize(int val);
expr_s* expr_literal_null_initialize(void);

expr_s* expr_unary_initialize(const token_s* op, expr_s* right);

expr_s* expr_binary_initialize(expr_s* left, const token_s* op, expr_s* right);

expr_s* expr_grouping_initialize(expr_s* e);

// Destructors

void expr_destroy(expr_s** expr);

#endif