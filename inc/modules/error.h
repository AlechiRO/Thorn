#ifndef ERROR_H
#define ERROR_H
#include "lexer.h"
#include "parser.h"

void report(int line, const char* where, const char* message);
void error(lexer_context_s* lctx, const char* message);
int check_error(lexer_context_s* lctx);
void parse_error(parser_context_s* pctx, token_s* token, const char* message);
void handle_parse_error(token_s* token, const char* message);

#endif