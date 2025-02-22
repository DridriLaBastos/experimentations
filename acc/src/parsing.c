#include <ctype.h>
#include <assert.h>

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
	dest->column = 0;
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

void ScanDigits(ParsingInfo* info, TokenInfo* tokenInfo)
{
	tokenInfo->intValue = 0;
	tokenInfo->size = 0;

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

	const char current = PARSING_CURRENT_CHAR(parsingInfo);
	bool tokenFound = true;

	tokenInfo->size = 1;

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
				ScanDigits(parsingInfo,tokenInfo);
			}
			else
			{
				tokenFound = false;
			}
		} break;
	}

	if (tokenFound)
	{
		tokenInfo->column = parsingInfo->column;
		tokenInfo->line = parsingInfo->line;
		MoveForward(parsingInfo);
	}

	return tokenFound;
}