#include <stdio.h>
#include <stdlib.h>
#include <parser.h>


parser_context_s* initialize_parser_context(token_list* tokens) {
    parser_context_s* pctx = malloc(sizeof(parser_context_s));

    if(pctx == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for parser context!\n");
        exit(EXIT_FAILURE);
    }
    pctx->tokens = tokens;
    pctx->current = 0;

    return pctx;
}


token_s* peek(parser_context_s* pctx) {
    return token_list_get(pctx->tokens, pctx->current);
}

token_s* previous(parser_context_s* pctx) {
    return token_list_get(pctx->tokens, pctx->current - 1);
}

int is_at_end(parser_context_s* pctx) {
    return peek(pctx)->type == TOKEN_EOF;
}

token_s* advance(parser_context_s* pctx) {
    if(!is_at_end(pctx))
        pctx->current++;
        return previous(pctx);
}

int check(parser_context_s* pctx, token_type_e type) {
    if(is_at_end(pctx)) 
        return 0;
    return peek(pctx)->type == type;
}

int match(parser_context_s* pctx,token_type_e types[], uint32_t size) {
    for(int i = 0; i < size; i++) {
        if(check(pctx, types[i])) {
            advance(pctx);
            return 1;
        }
    }
    return 0;
}


expr_s* expression(parser_context_s* pctx) {
    return equality(pctx);
}

void consume(parser_context_s* pctx, token_type_e type, char* message) {

}

expr_s* primary(parser_context_s* pctx) {
    if(match(pctx, (token_type_e[]){TOKEN_FALSE}, 1))
        return initialize_expr_literal_bool(0);
    if(match(pctx, (token_type_e[]){TOKEN_TRUE}, 1))
        return initialize_expr_literal_bool(1);
    if(match(pctx, (token_type_e[]){TOKEN_NULL}, 1))
        return initialize_expr_literal_null();
    
    if(match(pctx, (token_type_e[]){TOKEN_NUMBER}, 1))
        return initialize_expr_literal_num(previous(pctx)->literal->value.double_value);
    if(match(pctx, (token_type_e[]){TOKEN_STRING_GLOB, TOKEN_STRING_DEFAULT}, 2)) 
        return initialize_expr_literal_str(previous(pctx)->literal->value.string_value);

    if(match(pctx, (token_type_e[]){TOKEN_ROUND_BRACE_LEFT}, 1)) {
        expr_s* expr = expression(pctx);
        consume(pctx, TOKEN_ROUND_BRACE_RIGHT, "Expect \')\' after expression.\n");
        return initialize_expr_grouping(expr);
    }
}   

expr_s* unary(parser_context_s* pctx) {
    if(match(pctx, (token_type_e[]){TOKEN_BANG, TOKEN_MINUS}, 2)) {
        token_s* op = previous(pctx);
        expr_s* right = unary(pctx);
        return initialize_expr_unary(op, right);
    }
    
    return primary(pctx);
}

expr_s* factor(parser_context_s* pctx) {
    expr_s* expr = unary(pctx);
    while(match(pctx, (token_type_e[]){TOKEN_SLASH, TOKEN_STAR}, 2)) {
        token_s* op = previous(pctx);
        expr_s* right = unary(pctx);
        expr = initialize_expr_binary(expr, op, right);
    }

    return expr;
}

expr_s* term(parser_context_s* pctx) {
    expr_s* expr = factor(pctx);
    while(match(pctx, (token_type_e[]){TOKEN_MINUS, TOKEN_PLUS}, 2)) {
        token_s* op = previous(pctx);
        expr_s* right = factor(pctx);
        expr = initialize_expr_binary(expr, op, right);
    }

    return expr;
}

expr_s* comparison(parser_context_s* pctx) {
    expr_s* expr = term(pctx);
    while(match(pctx, (token_type_e[]){TOKEN_GREATER, TOKEN_GREATER_EQUAL, TOKEN_LESS, TOKEN_LESS_EQUAL}, 4)) {
        token_s* op = previous(pctx);
        expr_s* right = term(pctx);
        expr_s* expr = initialize_expr_binary(expr, op, right);
    }

    return expr;
}

expr_s* equality(parser_context_s* pctx) {
    expr_s* expr = comparison(pctx);

    while(match(pctx, (token_type_e[]){TOKEN_BANG_EQUAL, TOKEN_EQUAL_EQUAL}, 2)) {
        token_s* op = previous(pctx);
        expr_s* right = comparison(pctx);
        expr = initialize_expr_binary(expr, op, right);
    }

    return expr;
}

