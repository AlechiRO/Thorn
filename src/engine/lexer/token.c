#include <stdio.h>
#include <stdlib.h>
#include "token.h"

/*
Constructor
Allocate memory for literal struct
@param type Type of literal
@return Pointer to a new literal struct
*/
literal_s* initialize_literal(literal_type_e type) {
    literal_s* literal = malloc(sizeof(literal_s));
    if(literal == NULL) {
        fprintf(stderr, "FATAL: Could not allocate memory for literal struct!");
        exit(EXIT_FAILURE);
    }
    literal->type = type;

    return literal;
}

/*
Destructor
Free the memory allocated for the literal struct
@param literal Pointer to a pointer to a literal struct
*/
void destroy_literal(literal_s** literal) {
    if((*literal) == NULL || literal == NULL) {
        fprintf(stderr, "ERROR: Invalid token pointer!\n");
        return;
    }
    free(*literal);
    *literal = NULL;
}
/*
Constructor 
Allocate memory for token primitive fields and literal struct
@param type Type of the token
@param lexeme The actual text by which the token is identified
@param literal Struct containing data about the identifier's value
@param line Line on which the token is located in the source code
@return Pointer to a new token struct
*/
token_s* initialize_token(token_type_e type, char* lexeme, literal_s* literal, int line) {
    token_s* token = malloc(sizeof(token_s));
    if(token == NULL)
        return NULL;
    token->type = type;
    token->lexeme = lexeme;
    token->literal = literal;
    token->line = line;
    token->leading_whitespace = 0;
    return token;
}
/*
Destructor
Free the memory allocated for the token struct and litteral struct field         
@param list Pointer to a pointer to a token struct
*/
void destroy_token(token_s** token) {
    if((*token) == NULL || token == NULL) {
        fprintf(stderr, "ERROR: Invalid token pointer!\n");
        return;
    }
    literal_s** literal = &((*token)->literal);

    if(*literal != NULL && literal != NULL) 
        destroy_literal(literal);
    
    free((*token));
    *token = NULL;
}