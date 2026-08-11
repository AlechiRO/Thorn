#ifndef PARSER_GENERICS_H
#define PARSER_GENERICS_H

#include "token.h"

#define ARRAY_LIST_FREE_ITEM(item) destroy_token(&item)
#define ARRAY_LIST_ITEM_TYPE token_s*
#define ARRAY_LIST_TAG token_list
#include "array_list.h"

#endif