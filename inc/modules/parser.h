#ifndef PARSER_H
#define PARSER_H

#include <stdint.h>
#include <setjmp.h>
#include "generics.h"
#include "AST.h"

typedef struct Parser_context {
    token_list* tokens;
    uint32_t current;
    int had_error;
    arena_s* arena;
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
expr_s* parse(parser_context_s* pctx);


#endif