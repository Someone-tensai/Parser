#pragma once

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_KEYWORD,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION
}Token_type;

typedef struct
{
    Token_type type;
    char* value;
}Token;

typedef struct
{
    Token* tokens[100];
    int count;
}Token_List;


void add_token(Token_type type, const char* value);
void free_token_list();
const char* token_type_to_string(Token_type type);
void token_to_AST();
void print_token_list();
