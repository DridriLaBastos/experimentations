#ifndef AST_H
#define AST_H

typedef enum E_AST_NODE_TYPE
{
	NODE_TYPE_ADD, NODE_TYPE_SUB, NODE_TYPE_MUL, NODE_TYPE_DIV, NODE_TYPE_INTLIST,
} AST_NODE_TYPE;

typedef struct S_AST_NODE
{
	AST_NODE_TYPE type;

	struct S_AST_NODE* left;
	struct S_AST_NODE* right;

	int intValue;
} AstNode;

void Ast_Free(AstNode* node);

AstNode* Ast_AllocateNode(AST_NODE_TYPE type, const int intValue);

void Ast_Print(AstNode* root);

#endif