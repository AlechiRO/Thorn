#include <stdio.h>
#include <string.h>
#include "printer.h"
#include "error.h"



/*
Report an error
@param line Number of the line where the error occured
@param where
@param message Error message
*/
void report(int line, const char* where, const char* message) {
    printf("[line %d] Error %s: %s\n", line, where, message);
}

/*
Catch syntax error
@param lctx Pointer to lexer context struct
@param message Error message
*/
void error(lexer_context_s* lctx, const char* message) {
    report(lctx->line_number, "", message);
    lctx->had_error = 1;
}

/*
Checks whether there is an error and resets the error status
@param lctx Pointer to lexer context struct
@return 1 if there is an error or 0 if the script has no errors
*/
int check_error(lexer_context_s* lctx) {
    int error = lctx->had_error;
    lctx->had_error = 0;
    return error;
}



void parse_error(parser_context_s* pctx, token_s* token, const char* message) {
    handle_parse_error(token, message);
    pctx->had_error = 1;
    // Unwind call stack
    longjmp(pctx->panic_jmp, 1);
}

void handle_parse_error(token_s* token, const char* message) {
    if(token->type == TOKEN_EOF) 
        report(token->line, " at end", message);
    else {
        char location[256];
        snprintf(location, sizeof(location), " at '%.240s'", token->lexeme);
        report(token->line, location, message);
    } 
}


