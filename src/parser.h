#ifndef PARSER_H
#define PARSER_H

void parse(const char* file_contents);
char *read_file_contents(const char *filename);
void to_uppercase(char *str);
int reserved(char *a);
void process_identifier(const char *file_contents, int *i);
void process_number(const char *file_contents, int *i);
void process_string_literal(const char *file_contents, int *i);
void process_token(const char *file_contents, int *i);
void process_operator(char op, const char* file_contents, int* i);

#endif 