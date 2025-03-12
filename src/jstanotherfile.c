#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

char *read_file_contents(const char *filename);
void to_uppercase(char *str);
int reserved(char *a);
void process_identifier(const char *file_contents, int *i, int is_tokenizing);
void process_number(const char *file_contents, int *i, int is_tokenizing);
void process_string_literal(const char *file_contents, int *i, int is_tokenizing);
void process_token(const char *file_contents, int *i, int is_tokenizing);

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    if (argc < 3) {
        fprintf(stderr, "Usage: ./your_program <command> <filename>\n");
        return 1;
    }
    const char *command = argv[1];
    char *file_contents = read_file_contents(argv[2]);
    if (!file_contents) return 1;

    int is_tokenizing = strcmp(command, "tokenize") == 0;
    int is_parsing = strcmp(command, "parse") == 0;
    int line = 1;
    int error_flag = 0;

    if (is_tokenizing || is_parsing) {
        for (int i = 0; i < strlen(file_contents); i++) {
            if (isalpha(file_contents[i]) || file_contents[i] == '_') {
                process_identifier(file_contents, &i, is_tokenizing);
                continue;
            }
            if (isdigit(file_contents[i])) {
                process_number(file_contents, &i, is_tokenizing);
                continue;
            }
            if (file_contents[i] == '"') {
                process_string_literal(file_contents, &i, is_tokenizing);
                continue;
            }
            if (file_contents[i] == '\n') line++;
            process_token(file_contents, &i, is_tokenizing);
        }
        
        if(is_tokenizing)
        {
            printf("EOF  null\n");

        }
        if (error_flag) exit(65);
    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
        return 1;
    }

    free(file_contents);
    return 0;
}

void process_identifier(const char *file_contents, int *i, int is_tokenizing) {
    int start = *i, length = 0;
    while (*i < strlen(file_contents) && (isalnum(file_contents[*i]) || file_contents[*i] == '_')) {
        length++;
        (*i)++;
    }
    char identifier[length + 1];
    strncpy(identifier, file_contents + start, length);
    identifier[length] = '\0';

    if (is_tokenizing) {
        if (!reserved(identifier)) {
            printf("IDENTIFIER %s null\n", identifier);
        }
    } else {
        if (strcmp(identifier, "true") == 0 || strcmp(identifier, "false") == 0 || strcmp(identifier, "nil") == 0) {
            printf("%s\n", identifier);
        } else {
            printf("\n");
        }
    }
    (*i)--;
}

void process_number(const char *file_contents, int *i, int is_tokenizing) {
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

    if (is_tokenizing) {
        printf("NUMBER %s %.*f\n", number_str, dot_position == -1 ? 1 : length - 1, value);
    } else {
        printf("%.*f\n", decimal_places, value);
    }
    (*i)--;

}
void process_string_literal(const char *file_contents, int *i, int is_tokenizing) {
    int start = ++(*i), length = 0;
    while (*i < strlen(file_contents) && file_contents[*i] != '"') {
        length++;
        (*i)++;
    }
    char string_literal[length + 1];
    strncpy(string_literal, file_contents + start, length);
    string_literal[length] = '\0';

    if (is_tokenizing) {
        printf("STRING \"%s\" %s\n", string_literal);
    } else {
        printf("%s\n", string_literal);
    }
}
void process_token(const char *file_contents, int *i, int is_tokenizing) {
    switch (file_contents[*i]) {
        case '(': printf("LEFT_PAREN ( null\n"); break;
        case ')': printf("RIGHT_PAREN ) null\n"); break;
        case '{': printf("LEFT_BRACE { null\n"); break;
        case '}': printf("RIGHT_BRACE } null\n"); break;
        case '*': printf("STAR * null\n"); break;
        case ',': printf("COMMA , null\n"); break;
        case '+': printf("PLUS + null\n"); break;
        case '.': printf("DOT . null\n"); break;
        case '-': printf("MINUS - null\n"); break;
        case ';': printf("SEMICOLON ; null\n"); break;
        case '=': if (file_contents[*i + 1] == '=') { printf("EQUAL_EQUAL == null\n"); (*i)++; } else printf("EQUAL = null\n"); break;
        case '!': if (file_contents[*i + 1] == '=') { printf("BANG_EQUAL != null\n"); (*i)++; } else printf("BANG ! null\n"); break;
        case '<': if (file_contents[*i + 1] == '=') { printf("LESS_EQUAL <= null\n"); (*i)++; } else printf("LESS < null\n"); break;
        case '>': if (file_contents[*i + 1] == '=') { printf("GREATER_EQUAL >= null\n"); (*i)++; } else printf("GREATER > null\n"); break;
        case '/':
            if (file_contents[*i + 1] == '/') {
                while (*i < strlen(file_contents) && file_contents[*i] != '\n') (*i)++;
            } else {
                printf("SLASH / null\n");
            }
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

void to_uppercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

char *read_file_contents(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error reading file: %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    
    char *file_contents = (char*)(malloc(file_size + 1));
    if (!file_contents) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }
    
    fread(file_contents, 1, file_size, file);
    file_contents[file_size] = '\0';
    fclose(file);
    return file_contents;
}
