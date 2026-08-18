#ifndef TOKEN_H
#define TOKEN_H

#define ARRAY_LIST_ITEM_TYPE int
#define ARRAY_LIST_TAG int_list
#include "array_list.h"

#define ARRAY_LIST_ITEM_TYPE double
#define ARRAY_LIST_TAG double_list
#include "array_list.h"

#define ARRAY_LIST_ITEM_TYPE char*
#define ARRAY_LIST_TAG string_list
#include "array_list.h"

/* Token Enum */
typedef enum Token_type {

    // Literal Tokens
    TOKEN_NUMBER,               // 123.33 or 43     OK
    TOKEN_STRING_GLOB,          // "*.mp4" Tells the parser to enable globbing     OK
    TOKEN_STRING_DEFAULT,       // '*.mp4' Tells the parser to disable globbing    OK  
    TOKEN_IDENTIFIER,           // Literal that will be checked against the command and environment var HashMaps
    
    // Single Charecter Tokens
    TOKEN_PLUS,                 // +    OK
    TOKEN_MINUS,                // -    OK
    TOKEN_SLASH,                // /    OK
    TOKEN_STAR,                 // *    OK
    TOKEN_EQUAL,                // =    OK
    TOKEN_DOT,                  // .    OK
    TOKEN_PIPE,                 // |    OK
    TOKEN_SEMICOLON,            // ;    OK
    TOKEN_COLON,                // :    OK
    TOKEN_COMMA,                // ,    OK
    TOKEN_BANG,                 // !    OK
    TOKEN_WILDCARD,             // ?    OK
    TOKEN_LESS,                 // <    OK 
    TOKEN_GREATER,              // >    OK

    // Double Character Tokens
    TOKEN_ARROW,                // ->   OK
    TOKEN_APPEND_REDIRECT,      // >>   OK  
    TOKEN_POW,                  // **   OK
    TOKEN_INCREMENT,            // ++   OK
    TOKEN_DECREMENT,            // --   OK
    TOKEN_AND,                  // &&   OK
    TOKEN_OR,                   // ||   OK
    TOKEN_BANG_EQUAL,           // !=   OK
    TOKEN_EQUAL_EQUAL,          // ==   OK
    TOKEN_LESS_EQUAL,           // <=   OK
    TOKEN_GREATER_EQUAL,        // >=   OK

    // Braces Tokens
    TOKEN_ROUND_BRACE_LEFT,     // (   OK
    TOKEN_ROUND_BRACE_RIGHT,    // )   OK
    TOKEN_SQUARE_BRACE_LEFT,    // [   OK
    TOKEN_SQUARE_BRACE_RIGHT,   // ]   OK
    TOKEN_CURLY_BRACE_LEFT,     // {   OK
    TOKEN_CURLY_BRACE_RIGHT,    // }   OK

    // Special Tokens
    TOKEN_DOLLAR,               // $    OK
    TOKEN_ARGUMENT_NUMBER,      // $#   OK
    TOKEN_ARGUMENT_ARRAY,       // $@   OK
    TOKEN_DUP_OUT,              // >&   OK
    TOKEN_DUP_IN,               // <&   OK                      
    TOKEN_TERMINATOR,           // '\n' OK
                                // ~ before writing a comment OK

    // File Test Operators
    TOKEN_FILE_EXIST,           // -a   OK
    TOKEN_FILE_IS_DIR,          // -d   OK
    TOKEN_FILE_IS_NOT_EMPTY,    // -s   OK

    // Key Word Tokens
    TOKEN_IF,                   // if       OK
    TOKEN_ELSE,                 // else     OK
    TOKEN_WHILE,                // while    OK
    TOKEN_FOR,                  // for      OK
    TOKEN_TRUE,                 // true     OK
    TOKEN_FALSE,                // false    OK
    TOKEN_NULL,                 // null     OK
    TOKEN_FUNC,                 // func     OK
    TOKEN_RETURN,               // return   OK
    TOKEN_VAR,                  // var      OK

    // Terminator Tokens
    TOKEN_EOF,                  // END OF FILE  OK
    TOKEN_ERROR,                // (If sequence of chars can't be turned into a token)
} token_type_e;

typedef enum Literal_type {
    // Primitives
    LITERAL_DOUBLE,
    LITERAL_NULL,
    LITERAL_BOOLEAN,
    LITERAL_STRING,

    // Array Lists
    LITERAL_INT_LIST,
    LITERAL_DOUBLE_LIST,
    LITERAL_STRING_LIST,

    // Dictionaries - Int Keys
    DICT_INT_INT,
    DICT_INT_DOUBLE,
    DICT_INT_STRING,
    
    // Dictionaries - Double Keys
    DICT_DOUBLE_INT,
    DICT_DOUBLE_DOUBLE,
    DICT_DOUBLE_STRING,
    
    // Dictionaries - String Keys
    DICT_STRING_INT,
    DICT_STRING_DOUBLE,
    DICT_STRING_STRING,

} literal_type_e;

/*Lexeme struct*/
typedef struct Literal {
    literal_type_e type;
    union Value {
        // Typed Primitives
        double double_value;
        void* null_value;
        int boolean_value;
        char* string_value;

        // Typed Array Lists
        int_list* int_list_reference;
        double_list* double_list_reference;
        string_list* string_list_reference;

        // Typed Dictionaries - Keys: int

        //int_int_dict*    int_int_dict_reference;
        //int_double_dict* int_double_dict_reference;
        //int_string_dict* int_string_dict_reference;
        
        // Typed Dictionaries - Keys: double

        //double_int_dict*    double_int_dict_reference;
        //double_double_dict* double_double_dict_reference;
        //double_string_dict* double_string_dict_reference;
        
        // Typed Dictionaries - Keys: char*

        //string_int_dict*    string_int_dict_reference;
        //string_double_dict* string_double_dict_reference;
        //string_string_dict* string_string_dict_reference;
    } value;
} literal_s;


/* Token struct */
typedef struct Token {
    token_type_e type;
    char* lexeme;
    literal_s* literal; 
    int line;
    int leading_whitespace;
} token_s;

/* Literal Functions */

literal_s* initialize_literal(literal_type_e type);
void destroy_literal(literal_s** literal);

/* Token Functions */

token_s* initialize_token(token_type_e type, char* lexeme, literal_s* literal, int line);
void destroy_token(token_s** token);

#endif