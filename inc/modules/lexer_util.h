#ifndef LEXER_UTIL_h
#define LEXER_UTIL_h

#include "token.h"
#include "generics.h"

typedef struct Lexer_context lexer_context_s;

typedef struct line {
    char* buffer;
    size_t length;
    FILE* input;
}line_s;


line_s* initialize_line(void);
int getline_wrap(line_s* line, FILE* input);
void destroy_line(line_s** line);
void append_to_source(lexer_context_s* lctx, line_s* line);
void append_to_line(line_s* line, char* content);
lexer_context_s* initialize_lexer_context(void);
void destroy_lexer_context(lexer_context_s** lctx);
int is_at_end(lexer_context_s* lctx);
void add_token(lexer_context_s* lctx, token_type_e type, literal_s* literal);
char advance(lexer_context_s* lctx);
int match(lexer_context_s* lctx, char expected);
char peek(lexer_context_s* lctx);
char peek_next(lexer_context_s* lctx);
void match_multiple_tokens(lexer_context_s* lctx, char expected[], token_type_e types[], size_t size, token_type_e default_type);
int check_leading_whitespace(lexer_context_s* lctx);
void mark_token_whitespace(lexer_context_s* lctx, int whitespace, int token_count_before);
void load_keywords(string_token_type_hashmap* token_map);

#endif