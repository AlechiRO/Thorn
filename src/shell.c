#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "printer.h"
#include "lexer.h"
#include "lexer_util.h"
#include "parser.h"
#include "parser_util.h"


int main(void) {
    line_s* line = initialize_line();
    string_token_type_hashmap* token_map = string_token_type_hashmap_initialize();
    load_keywords(token_map);
    // REPL State
    while(1) {
        default_prompt();
        
        // Read line from stdin 
        if(getline_wrap(line, stdin) == -1)
            break;

        lexer_context_s* lctx = lex(line, token_map);
        token_list* tokens  = lctx->tokens;
        for(int i = 0; i < token_list_get_size(tokens); i++) {
            token_s* token = token_list_get(tokens, i);
            printf("---------\nLexeme: %s | Type: %d\n", token->lexeme, token->type);
        }
        if(check_error(lctx)) {
            destroy_lexer_context(&lctx);
            break;
        }

        parser_context_s* pctx = initialize_parser_context(tokens);
        expr_s* expr = parse(pctx);
        
        destroy_parser_context(&pctx);
        destroy_lexer_context(&lctx);  
    }
    destroy_line(&line);
    string_token_type_hashmap_destroy(&token_map);
    return 0;
}