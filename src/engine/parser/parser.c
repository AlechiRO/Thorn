#include <stdio.h>
#include <stdlib.h>
#include <parser.h>
#include "error.h"


parser_context_s* initialize_parser_context(token_list* tokens) {
    parser_context_s* pctx = malloc(sizeof(parser_context_s));

    if(pctx == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for parser context!\n");
        exit(EXIT_FAILURE);
    }
    pctx->tokens = tokens;
    pctx->current = 0;
    pctx->had_error = 0;

    return pctx;
}


token_s* p_peek(parser_context_s* pctx) {
    return token_list_get(pctx->tokens, pctx->current);
}

token_s* p_previous(parser_context_s* pctx) {
    return token_list_get(pctx->tokens, pctx->current > 0 ? pctx->current - 1 : 0);
}

int p_is_at_end(parser_context_s* pctx) {
    return p_peek(pctx)->type == TOKEN_EOF;
}

token_s* p_advance(parser_context_s* pctx) {
    if(!p_is_at_end(pctx))
        pctx->current++;
    return p_previous(pctx);
}

int p_check(parser_context_s* pctx, token_type_e type) {
    if(p_is_at_end(pctx)) 
        return 0;
    return p_peek(pctx)->type == type;
}

int p_match(parser_context_s* pctx,token_type_e types[], uint32_t size) {
    for(uint32_t i = 0; i < size; i++) {
        if(p_check(pctx, types[i])) {
            p_advance(pctx);
            return 1;
        }
    }
    return 0;
}


expr_s* expression(parser_context_s* pctx) {
    return equality(pctx);
}

token_s* consume(parser_context_s* pctx, token_type_e type, const char* message) {
    if(p_check(pctx, type))
        return p_advance(pctx);
    parse_error(pctx, p_peek(pctx), message);
    return NULL;
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

