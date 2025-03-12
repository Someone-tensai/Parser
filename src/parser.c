#include "parser.h"
#include "token.h"
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>


void parse(const char* file_contents)
{
    int line = 1;
    int error_flag = 0;

    for (int i = 0; i < strlen(file_contents); i++) {
            
    if (isalpha(file_contents[i]) || file_contents[i] == '_') {
        process_identifier(file_contents, &i);
        continue;
    }
    if (isdigit(file_contents[i])) {
        process_number(file_contents, &i);
        continue;
    }
    if (file_contents[i] == '"') {
        process_string_literal(file_contents, &i);
        continue;
    }
    if (file_contents[i] == '\n') line++;
    process_token(file_contents, &i);

if (error_flag) exit(65);
}
print_token_list();
token_to_AST();
free_token_list();
}

void process_identifier(const char *file_contents, int *i) {
    int start = *i, length = 0;
    while (*i < strlen(file_contents) && (isalnum(file_contents[*i]) || file_contents[*i] == '_')) {
        length++;
        (*i)++;
    }
    char identifier[length + 1];
    strncpy(identifier, file_contents + start, length);
    identifier[length] = '\0';

    
    if (!reserved(identifier)) {

            printf("IDENTIFIER %s null\n", identifier);
            add_token(TOKEN_IDENTIFIER , identifier);
    }

    (*i)--;
}

void process_number(const char *file_contents, int *i) {
    int start = *i, length = 0, dot_position = -1;
    double value = 0, place = 1;

    while (*i < strlen(file_contents) && (isdigit(file_contents[*i]) || file_contents[*i] == '.')) {
        if (file_contents[*i] == '.') dot_position = *i;
        length++;
        (*i)++;
    }
    char number_str[length + 1];
    strncpy(number_str, file_contents + start, length);
    number_str[length] = '\0';
    value = atof(number_str);
    
    int decimal_places = (dot_position == -1) ? 1 : (length - (dot_position - start) - 1);

    
    printf("NUMBER %s %.*f\n", number_str, dot_position == -1 ? 1 : length - 1, value);
    add_token(TOKEN_NUMBER , number_str);
    (*i)--;

}
void process_string_literal(const char *file_contents, int *i) {
    int start = ++(*i), length = 0;
    while (*i < strlen(file_contents) && file_contents[*i] != '"') {
        length++;
        (*i)++;
    }
    char string_literal[length + 1];
    strncpy(string_literal, file_contents + start, length);
    string_literal[length] = '\0';
    printf("%s\n", string_literal);
    
}
void process_token(const char *file_contents, int *i) {
    switch (file_contents[*i]) {
        case '(': printf("LEFT_PAREN ( null\n"); break;
        case ')': printf("RIGHT_PAREN ) null\n"); break;
        case '{': printf("LEFT_BRACE { null\n"); break;
        case '}': printf("RIGHT_BRACE } null\n"); break;
        case '.': printf("DOT . null\n"); break;
        case ',': printf("COMMA , null\n"); break;
        case ';': printf("SEMICOLON ; null\n"); break;

        case '*':
        case '+': 
        case '-': 
        case '=': 
        case '!': 
        case '<': 
        case '>': 
        case '/':
            process_operator(file_contents[*i], file_contents, i);
            break;
        case ' ':
            break;
        case '\n':
        printf("New Line \n");
            break;

        default:
            fprintf(stderr, "Error: Unexpected character: %c\n", file_contents[*i]);
    }
}
int reserved(char *a) {
    char *resv[] = {"and", "class", "else", "false", "for", "fun", "if", "nil", "or", "print", "return", "super", "this", "true", "var", "while"};
    for (int i = 0; i < 16; i++) {
        char b[strlen(a)];
        strcpy(b,a);
        if (strcmp(a, resv[i]) == 0) {
            to_uppercase(a);
            printf("%s %s null\n", a, b);
            return 1;
        }
    }
    return 0;
}

void process_operator(char op, const char* file_contents, int* i)
{
    char str[3] = {op , '\0' , '\0'};

    if (op == '=' && file_contents[*i + 1] == '=') { str[1] = '='; (*i)++; }
    if (op == '!' && file_contents[*i + 1] == '=') { str[1] = '='; (*i)++; }
    if (op == '<' && file_contents[*i + 1] == '=') { str[1] = '='; (*i)++; }
    if (op == '>' && file_contents[*i + 1] == '=') { str[1] = '='; (*i)++; }
    printf("Operator %s " , str);
    add_token(TOKEN_OPERATOR , str);

}
void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}
