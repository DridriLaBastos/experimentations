#include <ctype.h>
#include <assert.h>
#include <stdlib.h> //For NULL

#include "../log.h"
#include "../parsing.h"

#define IS_MATH_OP(c) ((c == '+') || (c == '-') || (c == '*') || (c == '/'))
#define PARSING_END_OF_FILE(info) (info->cursor >= info->dataSize)
#define PARSING_CURRENT_CHAR(info) (info->data[info->cursor])

void Parsing_Init(char* data, const size_t size, ParsingInfo* dest)
{
	dest->data = data;
	dest->dataSize = size;
	dest->cursor = 0;
	dest->line = 1;
	dest->column = 1;
	dest->deepestSubtree = NULL;
}

static void MoveForward(ParsingInfo* info)
{
	assert(info->cursor < info->dataSize);
	info->cursor += 1;
	const char c = PARSING_CURRENT_CHAR(info);
	if (c == '\n')
	{
		info->line += 1;
		info->column = 0;
	}
	else
	{
		info->column += 1;
	}
}

bool Parsing_SkipWhiteSpace(ParsingInfo* info)
{
	while((!PARSING_END_OF_FILE(info)) && (isspace(PARSING_CURRENT_CHAR(info))))
	{
		MoveForward(info);
	}

	return PARSING_END_OF_FILE(info);
}

void ScanDigits(ParsingInfo* info, TokenInfo* tokenInfo, char firstDigit)
{
	tokenInfo->intValue = firstDigit - '0';

	while (!PARSING_END_OF_FILE(info) && isdigit(PARSING_CURRENT_CHAR(info)))
	{
		tokenInfo->intValue = tokenInfo->intValue * 10 + (PARSING_CURRENT_CHAR(info) - '0');
		tokenInfo->size += 1;
		MoveForward(info);
	}
}

bool Parsing_GetNextToken(ParsingInfo* parsingInfo, TokenInfo* tokenInfo)
{
	if (PARSING_END_OF_FILE(parsingInfo))
	{
		return false;
	}

	if(Parsing_SkipWhiteSpace(parsingInfo))
	{
		return false;
	}

	tokenInfo->size = 1;
	tokenInfo->column = parsingInfo->column;
	tokenInfo->line = parsingInfo->line;

	const char current = PARSING_CURRENT_CHAR(parsingInfo);
	MoveForward(parsingInfo);

	switch(current)
	{
		case '+':
			tokenInfo->type = TOKEN_TYPE_PLUS;
			break;
		case '-':
			tokenInfo->type = TOKEN_TYPE_MINUS;
			break;
		case '*':
			tokenInfo->type = TOKEN_TYPE_STAR;
			break;
		case '/':
			tokenInfo->type = TOKEN_TYPE_SLASH;
			break;
		default:
		{
			if (isdigit(current))
			{
				tokenInfo->type = TOKEN_TYPE_INTLIST;
				ScanDigits(parsingInfo,tokenInfo,current);
			}
			else
			{
				tokenInfo->type = TOKEN_TYPE_INVALID;
				tokenInfo->intValue = current;
			}
		} break;
	}

	return true;
}

static AST_NODE_TYPE TokenTypeToAstType(TOKEN_TYPE type)
{
	switch (type)
	{
		case TOKEN_TYPE_INTLIST:
			return NODE_TYPE_INTLIST;
		case TOKEN_TYPE_PLUS:
			return NODE_TYPE_ADD;
		case TOKEN_TYPE_MINUS:
			return NODE_TYPE_SUB;
		case TOKEN_TYPE_STAR:
			return NODE_TYPE_MUL;
		case TOKEN_TYPE_SLASH:
			return NODE_TYPE_DIV;
		default:
			assert(false);
			break;
	}
}

static char errorDescription [256];

#define SET_ERROR_MSG(fmt,...) do { snprintf(errorDescription,sizeof(errorDescription),fmt,##__VA_ARGS__); } while(false)

char* Parsing_GetLastErrorDescription(void)
{
	return errorDescription;
}

bool Parsing_AstFeedToken(ParsingInfo* parsingInfo,TokenInfo* tokenInfo, AstNode** root)
{
	if (tokenInfo->type == TOKEN_TYPE_INVALID)
	{
		SET_ERROR_MSG("%ld:%ld : Invalid token", tokenInfo->line,tokenInfo->column);
		return false;
	}

	AstNode* node = Ast_AllocateNode(TokenTypeToAstType(tokenInfo->type),tokenInfo->intValue);
	AstNode* newRoot = *root;

	// Simple case, this is the first node added and its the root
	if (newRoot == NULL)
	{
		newRoot = node;
		parsingInfo->deepestSubtree = newRoot;

		//In that case the syntax forces the token to be a number
		if (tokenInfo->type != TOKEN_TYPE_INTLIST)
		{
			SET_ERROR_MSG("The first expected token should be a number but got a math operator");
			Ast_Free(node);
			return false;
		}
	}
	// More complex case : the node is added to the left and the rest of ast isbuild to the right
	else
	{
		if (parsingInfo->deepestSubtree->type == NODE_TYPE_INTLIST)
		{
			if (tokenInfo->type == TOKEN_TYPE_INTLIST)
			{
				SET_ERROR_MSG("%ld:%ld : Expected an integer after a math op but got another integer",tokenInfo->line,tokenInfo->column);
				Ast_Free(node);
				return false;
			}
			
			AstNode* child = parsingInfo->deepestSubtree;
			node->parent = child->parent;
			child->parent = node;
			node->left = child;

			parsingInfo->deepestSubtree = node;

			if (node->parent != NULL)
			{
				node->parent->right = node;
			}

			if (newRoot == node->left)
			{
				newRoot = node;
			}
		}
		else
		{
			if (tokenInfo->type != TOKEN_TYPE_INTLIST)
			{
				SET_ERROR_MSG("%ld:%ld : Expected integer after a math op",tokenInfo->line,tokenInfo->column);
				Ast_Free(node);
				return false;
			}
			parsingInfo->deepestSubtree->right = node;
			node->parent = parsingInfo->deepestSubtree;
			parsingInfo->deepestSubtree = node;
		}
	}

	SET_ERROR_MSG("%s","");
	*root = newRoot;
	return true;
}