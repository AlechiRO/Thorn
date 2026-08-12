#ifndef PARSER_H
#define PARSER_H

#include "parser_generics.h"
#include <stdint.h>
#include "AST.h"

typedef struct Parser_context {
    token_list* tokens;
    u_int32_t current;
} parser_context_s;


expr_s* equality(parser_context_s* pctx);
expr_s* comparison(parser_context_s* pctx);
expr_s* term(parser_context_s* pctx);
expr_s* factor(parser_context_s* pctx);
expr_s* unary(parser_context_s* pctx);
expr_s* expression(parser_context_s* pctx);
parser_context_s* initialize_parser_context(token_list* tokens);
expr_s* primary(parser_context_s* pctx); 

//Util
int match(parser_context_s* pctx,token_type_e types[], uint32_t size);
int check(parser_context_s* pctx, token_type_e type);
token_s* advance(parser_context_s* pctx);
int is_at_end(parser_context_s* pctx);
token_s* previous(parser_context_s* pctx);
token_s* peek(parser_context_s* pctx);


#endif