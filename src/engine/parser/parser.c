#include <stdio.h>
#include <stdlib.h>
#include <parser.h>
#include "error.h"
#include "parser_util.h"



/*
Parse an expression
@param pctx Pointer to parser context struct
@return Pointer to expression struct
*/
expr_s* expression(parser_context_s* pctx) {
    return equality(pctx);
}

/*
Parse a literal or grouping expression
If nothing matches raise a parsing error
@param pctx Pointer to parser context struct
@return Pointer to a literal or grouping expression
*/
expr_s* primary(parser_context_s* pctx) {
    if(p_match(pctx, (token_type_e[]){TOKEN_FALSE}, 1))
        return initialize_expr_literal_bool(0, pctx->arena);
    if(p_match(pctx, (token_type_e[]){TOKEN_TRUE}, 1))
        return initialize_expr_literal_bool(1, pctx->arena);
    if(p_match(pctx, (token_type_e[]){TOKEN_NULL}, 1))
        return initialize_expr_literal_null(pctx->arena);
    
    if(p_match(pctx, (token_type_e[]){TOKEN_NUMBER}, 1))
        return initialize_expr_literal_num(p_previous(pctx)->literal->value.double_value, pctx->arena);
    if(p_match(pctx, (token_type_e[]){TOKEN_STRING_GLOB, TOKEN_STRING_DEFAULT}, 2)) 
        return initialize_expr_literal_str(p_previous(pctx)->literal->value.string_value, pctx->arena);

    if(p_match(pctx, (token_type_e[]){TOKEN_ROUND_BRACE_LEFT}, 1)) {
        expr_s* expr = expression(pctx);
        consume(pctx, TOKEN_ROUND_BRACE_RIGHT, "Expect \')\' after expression.");
        return initialize_expr_grouping(expr, pctx->arena);
    }
    parse_error(pctx, p_peek(pctx), "Expect expression.");
    return NULL;
}   

/*
Parse a unary expression
@param pctx Pointer to parser context struct
@return pointer to unary expression
*/
expr_s* unary(parser_context_s* pctx) {
    if(p_match(pctx, (token_type_e[]){TOKEN_BANG, TOKEN_MINUS}, 2)) {
        token_s* op = p_previous(pctx);
        expr_s* right = unary(pctx);
        return initialize_expr_unary(op, right, pctx->arena);
    }
    
    return primary(pctx);
}

/*
Parse a factor in an expression
@param pctx Pointer to parser context struct
@return Pointer to a factor expression
*/
expr_s* factor(parser_context_s* pctx) {
    expr_s* expr = unary(pctx);
    while(p_match(pctx, (token_type_e[]){TOKEN_SLASH, TOKEN_STAR}, 2)) {
        token_s* op = p_previous(pctx);
        expr_s* right = unary(pctx);
        expr = initialize_expr_binary(expr, op, right, pctx->arena);
    }

    return expr;
}

/*
Parse a term in an expression
@param pctx Pointer to parser context struct
@return Pointer to a term expression
*/
expr_s* term(parser_context_s* pctx) {
    expr_s* expr = factor(pctx);
    while(p_match(pctx, (token_type_e[]){TOKEN_MINUS, TOKEN_PLUS}, 2)) {
        token_s* op = p_previous(pctx);
        expr_s* right = factor(pctx);
        expr = initialize_expr_binary(expr, op, right, pctx->arena);
    }

    return expr;
}

/*
Parse a comparison in an expression
@param pctx Pointer to parser context struct
@return Pointer to a comparison expression
*/
expr_s* comparison(parser_context_s* pctx) {
    expr_s* expr = term(pctx);
    while(p_match(pctx, (token_type_e[]){TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_LESS, TOKEN_LESS_EQUAL}, 4)) {
        token_s* op = p_previous(pctx);
        expr_s* right = term(pctx);
        expr = initialize_expr_binary(expr, op, right, pctx->arena);
    }

    return expr;
}

/*
Parse an equality in an expression
@param pctx Pointer to parser context struct
@return Pointer to an equality expression
*/
expr_s* equality(parser_context_s* pctx) {
    expr_s* expr = comparison(pctx);

    while(p_match(pctx, (token_type_e[]){TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL}, 2)) {
        token_s* op = p_previous(pctx);
        expr_s* right = comparison(pctx);
        expr = initialize_expr_binary(expr, op, right, pctx->arena);
    }

    return expr;
}

/*
Synchronize the parser and skip to the next closest statement
@param pctx Pointer to parser context struct
*/
void synchronize(parser_context_s* pctx) {
    p_advance(pctx);

    while(!p_is_at_end(pctx)) {
        if(p_previous(pctx)->type == TOKEN_TERMINATOR)
            return;
        
        switch(p_peek(pctx)->type) {
            case TOKEN_FUNC :
            case TOKEN_VAR :
            case TOKEN_FOR :
            case TOKEN_IF :
            case TOKEN_WHILE:
            case TOKEN_RETURN:
                return;
        }
        p_advance(pctx);
    }
}

/*
Main parsing function
@param pctx Pointer to parser context struct
@return Pointer to an expression
*/
expr_s* parse(parser_context_s* pctx) {
    if(setjmp(pctx->panic_jmp) == 0) {
        return expression(pctx);
    } else {
        synchronize(pctx);
        return NULL;
    }
}

