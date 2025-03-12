#include "AST.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

AST_Node* add_ast_Node(Token* token)
{
    AST_Node* ast_node = (AST_Node*)(malloc(sizeof(AST_Node)));
    ast_node -> token = *token;
    ast_node -> num_children = 0;
    ast_node->children = NULL;
    return ast_node;
}

void process_op(Token* op, AST_Node* operand_stack[] , int* operand_top, int* operator_top )
{
    AST_Node* right_operand = operand_stack[(*operand_top)--];
    AST_Node* left_operand = operand_stack[(*operand_top)--];

    AST_Node* new_node = (AST_Node*)(malloc(sizeof(AST_Node)));
    if(new_node ==  NULL) printf("NULL \t");
    new_node -> token = *op;
    new_node -> children = malloc(2 * sizeof(AST_Node*));
    new_node -> children[0] = left_operand;
    new_node -> children[1] = right_operand;
    new_node -> num_children = 2;
    //printf("%d\n" , *operand_top);

    operand_stack[++(*operand_top)] = new_node;
    (*operator_top)--;

}

// If its a operand , push to operand stack
// If its a operator, check if there are higher precedence operators in the stack first and process them.
// If there are higher precedence operators, pop the top two operands and link them to a new node and add it back to the operand stack
// If there are no higher precedence operators, process it.

AST* create_AST(Token_List list)
{

    AST* ast = (AST*)(malloc(sizeof(AST)));
    AST_Node* operand_stack[list.count];
    Token* operator_stack[list.count];

    int operand_top = -1 , operator_top = -1;
    for(int i = 0 ; i < list.count ; i++)
    {
        if(list.tokens[i] -> type == TOKEN_NUMBER || list.tokens[i] -> type == TOKEN_IDENTIFIER)
        {
            operand_stack[++operand_top] = add_ast_Node(list.tokens[i]);
        }

        else if(list.tokens[i] -> type == TOKEN_OPERATOR)
        {
            while(operator_top >= 0 && (precedence(operator_stack[operator_top]) >= precedence(list.tokens[i])))
            {
                printf("A\t");
                process_op(operator_stack[operator_top] , operand_stack, &operand_top , &operator_top);
            }

            operator_stack[++operator_top] = list.tokens[i];
        }
    }
    while(operator_top >= 0)
    {
        process_op(operator_stack[operator_top] , operand_stack , &operand_top, &operator_top);
    }
    ast -> root = operand_stack[operand_top];
    printf("AST created succesfully\n");
    return ast;

}

int precedence(Token* token)

{
    char* op[] = {"/" , "*", "+", "-"};
    int precedence_values[] = {2,2,1,1};
    if(token->value == NULL)
    {
        printf("Null value in token");
        return -1;
    }
    for(int i = 0 ; i < 4; i++)
    {
    if(strcmp(token->value, op[i]) == 0)
    {
        return precedence_values[i];
    }
    }
    return -1;
}


// Going for a depth first approach / Post Order.
// Recursively visiting the left and right child
// Process the current node.
double traverse_AST(AST_Node* root)
{
    if(root == NULL)
    {
        return 0;
    }

    if(root->token.type == TOKEN_NUMBER)
    {
        return atof(root->token.value);
    }

    double left = traverse_AST(root->children[0]);
    double right = traverse_AST(root->children[1]);
    
    if(strcmp(root->token.value, "+") == 0) return left + right;
    if(strcmp(root->token.value, "-") == 0) return left - right;
    if(strcmp(root->token.value, "*") == 0) return left * right;
    if(strcmp(root->token.value, "/") == 0) return left / right;

    return 0;
}