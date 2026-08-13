#include <stdio.h>
#include <stdlib.h>
#include <parser.h>
#include "error.h"
#include "parser_util.h"




expr_s* expression(parser_context_s* pctx) {
    return equality(pctx);
}

expr_s* primary(parser_context_s* pctx) {
    if(p_match(pctx, (token_type_e[]){TOKEN_FALSE}, 1))
        return initialize_expr_literal_bool(0);
    if(p_match(pctx, (token_type_e[]){TOKEN_TRUE}, 1))
        return initialize_expr_literal_bool(1);
    if(p_match(pctx, (token_type_e[]){TOKEN_NULL}, 1))
        return initialize_expr_literal_null();
    
    if(p_match(pctx, (token_type_e[]){TOKEN_NUMBER}, 1))
        return initialize_expr_literal_num(p_previous(pctx)->literal->value.double_value);
    if(p_match(pctx, (token_type_e[]){TOKEN_STRING_GLOB, TOKEN_STRING_DEFAULT}, 2)) 
        return initialize_expr_literal_str(p_previous(pctx)->literal->value.string_value);

    if(p_match(pctx, (token_type_e[]){TOKEN_ROUND_BRACE_LEFT}, 1)) {
        expr_s* expr = expression(pctx);
        consume(pctx, TOKEN_ROUND_BRACE_RIGHT, "Expect \')\' after expression.");
        return initialize_expr_grouping(expr);
    }
    parse_error(pctx, p_peek(pctx), "Expect expression.");
    return NULL;
}   

expr_s* unary(parser_context_s* pctx) {
    if(p_match(pctx, (token_type_e[]){TOKEN_BANG, TOKEN_MINUS}, 2)) {
        token_s* op = p_previous(pctx);
        expr_s* right = unary(pctx);
        return initialize_expr_unary(op, right);
    }
    
    return primary(pctx);
}

expr_s* factor(parser_context_s* pctx) {
    expr_s* expr = unary(pctx);
    while(p_match(pctx, (token_type_e[]){TOKEN_SLASH, TOKEN_STAR}, 2)) {
        token_s* op = p_previous(pctx);
        expr_s* right = unary(pctx);
        expr = initialize_expr_binary(expr, op, right);
    }

    return expr;
}

expr_s* term(parser_context_s* pctx) {
    expr_s* expr = factor(pctx);
    while(p_match(pctx, (token_type_e[]){TOKEN_MINUS, TOKEN_PLUS}, 2)) {
        token_s* op = p_previous(pctx);
        expr_s* right = factor(pctx);
        expr = initialize_expr_binary(expr, op, right);
    }

    return expr;
}

expr_s* comparison(parser_context_s* pctx) {
    expr_s* expr = term(pctx);
    while(p_match(pctx, (token_type_e[]){TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_LESS, TOKEN_LESS_EQUAL}, 4)) {
        token_s* op = p_previous(pctx);
        expr_s* right = term(pctx);
        expr = initialize_expr_binary(expr, op, right);
    }

    return expr;
}

expr_s* equality(parser_context_s* pctx) {
    expr_s* expr = comparison(pctx);

    while(p_match(pctx, (token_type_e[]){TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL}, 2)) {
        token_s* op = p_previous(pctx);
        expr_s* right = comparison(pctx);
        expr = initialize_expr_binary(expr, op, right);
    }

    return expr;
}

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

expr_s* parse(parser_context_s* pctx) {
    if(setjmp(pctx->panic_jmp) == 0) {
        return expression(pctx);
    } else {
        synchronize(pctx);
        return NULL;
    }
}

