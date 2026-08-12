#ifndef LEXER_GENERICS_H
#define LEXER_GENERICS_H
#include "token.h"

#define ARRAY_LIST_FREE_ITEM(item) destroy_token(&item)
#define ARRAY_LIST_ITEM_TYPE token_s*
#define ARRAY_LIST_TAG token_list
#include "array_list.h"

#define HASHMAP_FREE_KEY(k) free(k)
#define HASHMAP_KEY_TYPE char*
#define HASHMAP_VALUE_TYPE token_type_e
#define HASHMAP_TAG string_token_type_hashmap
#include "hashmap.h"

#endif