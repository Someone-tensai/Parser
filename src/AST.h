#pragma once

#include"token.h"

typedef struct AST_Node AST_Node;
typedef struct AST_Node
{
    Token token;
    AST_Node** children;
    int num_children;
}AST_Node;


typedef struct AST
{
    AST_Node* root;
}AST;
// Add a node to the AST based on the token type
// Add variables as leaf nodes after the rest is added.

AST_Node* add_ast_Node(Token* token);
AST* create_AST(Token_List list);
int precedence(Token* token);
void process_op(Token* op, AST_Node* operand_stack[] , int* operand_top, int* operator_top);
double traverse_AST(AST_Node* root);
