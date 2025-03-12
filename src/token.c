#include"token.h"
#include "AST.h"
#include <stdlib.h>
#include<string.h>
#include<stdio.h>

Token_List list = { .count = 0};
void add_token(Token_type type, const char* value)
{
    if(list.count >= 100) return;
    list.tokens[list.count] = malloc(sizeof(Token));
    list.tokens[list.count] -> type = type;
    list.tokens[list.count] -> value = strdup(value);

    list.count++;

}

void free_token_list()
{
    for(int i = 0 ; i < list.count ; i++)
    {
        free(list.tokens[i]->value);
        free(list.tokens[i]);
    }
    
}
void token_to_AST()
{
    AST* abs_t = create_AST(list);
    double result = traverse_AST(abs_t-> root);
    printf("Evaluated: %.2f" , result);
}
void print_token_list()
{
    for(int i = 0 ; i < list.count ; i++)
    {
        printf("Token type: %s\n" , token_type_to_string(list.tokens[i]->type));
        printf("Token added: %s\n" , list.tokens[i]->value);
    }
    
}
const char* token_type_to_string(Token_type type) {
    switch (type) {
        case TOKEN_IDENTIFIER: return "TOKEN_IDENTIFIER";
        case TOKEN_KEYWORD: return "TOKEN_KEYWORD";
        case TOKEN_NUMBER: return "TOKEN_NUMBER";
        case TOKEN_OPERATOR: return "TOKEN_OPERATOR";
        case TOKEN_PUNCTUATION: return "TOKEN_PUNCTUATION";

        // Add cases for other token types here
        default: return "UNKNOWN";
    }
}