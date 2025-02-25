#ifndef PARSING_H
#define PARSING_H

#include <stddef.h>
#include <stdbool.h>

#include "ast.h"

typedef enum E_TOKEN_TYPE
{
	TOKEN_TYPE_PLUS, TOKEN_TYPE_MINUS, TOKEN_TYPE_STAR, TOKEN_TYPE_SLASH, TOKEN_TYPE_INTLIST,
	TOKEN_TYPE_INVALID,
} TOKEN_TYPE;

typedef struct S_TOKEN_INFO
{
	TOKEN_TYPE type;
	int intValue;
	size_t column;
	size_t line;
	size_t size;
} TokenInfo;

typedef struct S_PARSING_INFO
{
	AstNode* deepestSubtree;
	char* data;
	size_t dataSize;
	size_t cursor;
	size_t line;
	size_t column;
} ParsingInfo;

void Parsing_Init(char* data, const size_t size, ParsingInfo* dest);

bool Parsing_GetNextToken(ParsingInfo* parsingInfo, TokenInfo* tokenInfo);
bool Parsing_AstFeedToken(ParsingInfo* parsingInfo,TokenInfo* tokenInfo, AstNode** root);
char* Parsing_GetLastErrorDescription(void);

#endif //PARSING_H