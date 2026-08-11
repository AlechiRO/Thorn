#ifndef PARSER_H
#define PARSER_H

#include "parser_generics.h"
#include <stdint.h>

typedef struct Parser_context {
    token_list* tokens;
    u_int32_t current;
} parser_context_s;

#endif