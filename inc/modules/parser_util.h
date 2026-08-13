#ifndef PARSER_UTIL_H
#define PARSER_UTIL_H

#include <stdint.h>
#include "parser.h"

int p_match(parser_context_s* pctx,token_type_e types[], uint32_t size);
int p_check(parser_context_s* pctx, token_type_e type);
token_s* p_advance(parser_context_s* pctx);
int p_is_at_end(parser_context_s* pctx);
token_s* p_previous(parser_context_s* pctx);
token_s* p_peek(parser_context_s* pctx);
token_s* consume(parser_context_s* pctx, token_type_e type, const char* message);

#endif