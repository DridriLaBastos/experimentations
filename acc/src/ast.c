#include <stdio.h>
#include <stdlib.h>

#include "../ast.h"

void Ast_Free(AstNode* node)
{
	if (node == NULL)
	{
		return;
	}

	Ast_Free(node->left);
	Ast_Free(node->right);

	free(node);
}

AstNode* Ast_AllocateNode(AST_NODE_TYPE type, const int intValue)
{
	AstNode* node = (AstNode*)malloc(sizeof(AstNode));
	node->type = type;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	node->intValue = intValue;

	return node;
}

void Ast_InsertLeft(AstNode* parent, AstNode* left)
{
	parent->left = left;
	left->parent = parent;
}

void Ast_InsertRight(AstNode* parent, AstNode* right)
{
	parent->right = right;
	right->parent = parent;
}

char* GetNodeTypeDescription(AST_NODE_TYPE type)
{
	switch (type)
	{
		case NODE_TYPE_ADD:
			return "Add";
		case NODE_TYPE_SUB:
			return "Sub";
		case NODE_TYPE_MUL:
			return "Mul";
		case NODE_TYPE_DIV:
			return "Div";
		case NODE_TYPE_INTLIST:
			return "IntList";
		default:
			return "Unknown";
	}
}

static void PrintNode(AstNode* node)
{
	printf("%s",GetNodeTypeDescription(node->type));

	if (node->type == NODE_TYPE_INTLIST)
	{
		printf(" (%d)",node->intValue);
	}

	puts("");
}

static void InternalAstPrint(AstNode* root, int level)
{
	if (root)
	{
		for (int i = 0; i < level; i += 1)
		{
			printf("  ");
		}

		PrintNode(root);
		InternalAstPrint(root->left,level+1);
		InternalAstPrint(root->right,level+1);
	}
}

void Ast_Print(AstNode* root)
{
	if (root)
	{
		InternalAstPrint(root,0);
	}
}