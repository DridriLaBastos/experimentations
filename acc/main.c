#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "parsing.h"

int main(int argc, char const *argv[])
{
	if (argc != 2)
	{
		LOG_ERROR("No file given to compile");
		LOG_INFO("Usage : %s <file_to_compile>.c",argv[0]);
		return EXIT_FAILURE;
	}

	FILE* file = fopen(argv[1], "r");

	if (file == NULL)
	{
		LOG_ERROR("Unable to open the file '%s' : '%s'",argv[1],strerror(errno),errno);
		return EXIT_FAILURE;
	}

	fseek(file, 0, SEEK_END);
	const int fileSizeInByte = ftell(file);
	rewind(file);

	LOG_DEBUG("Compiling file '%s' [%d o]",argv[1],fileSizeInByte);

	char* fileDataBuffer = (char*)malloc(fileSizeInByte);
	fread(fileDataBuffer,fileSizeInByte,1,file);

	ParsingInfo info;

	Parsing_Init(fileDataBuffer,fileSizeInByte,&info);

	TokenInfo token;

	while (Parsing_GetNextToken(&info,&token))
	{
		switch (token.type)
		{
			case TOKEN_TYPE_PLUS:
				LOG_DEBUG("Found token '+' at %zu:%zu [%zu]",token.line,token.column,token.size);
				break;
			case TOKEN_TYPE_MINUS:
				LOG_DEBUG("Found token '-' at %zu:%zu [%zu]",token.line,token.column,token.size);
				break;
			case TOKEN_TYPE_STAR:
				LOG_DEBUG("Found token '*' at %zu:%zu [%zu]",token.line,token.column,token.size);
				break;
			case TOKEN_TYPE_SLASH:
				LOG_DEBUG("Found token '/' at %zu:%zu [%zu]",token.line,token.column,token.size);
				break;
			case TOKEN_TYPE_INTLIST:
				LOG_DEBUG("Found a list of integers at %zu:%zu [%zu] : %d",token.line,token.column,token.size,token.intValue);
				break;
			case TOKEN_TYPE_INVALID:
				LOG_WARN("Found an invalid token at %zu:%zu [%zu]",token.line,token.column,token.size);
				break;
			default:
				LOG_ERROR("Unknown token type");
				break;
		}
	}
	
	free(fileDataBuffer);
	fclose(file);

	return EXIT_SUCCESS;
}
