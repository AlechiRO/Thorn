#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <setjmp.h>
#include "generics.h"
#include "AST.h"

typedef struct Parser_context {
    token_list* tokens;
    u_int32_t current;
    int had_error;
    jmp_buf panic_jmp;
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
int p_match(parser_context_s* pctx,token_type_e types[], uint32_t size);
int p_check(parser_context_s* pctx, token_type_e type);
token_s* p_advance(parser_context_s* pctx);
int p_is_at_end(parser_context_s* pctx);
token_s* p_previous(parser_context_s* pctx);
token_s* p_peek(parser_context_s* pctx);


#endif