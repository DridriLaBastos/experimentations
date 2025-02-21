#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"

typedef enum E_TOKEN_TYPE
{
	TOKEN_TYPE_PLUS, TOKEN_TYPE_MINUS, TOKEN_TYPE_STAR, TOKEN_TYPE_SLASH, TOKEN_TYPE_INTLIST
} TOKEN_TYPE;

struct Token
{
	TOKEN_TYPE type;
	int intValue;
};

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

	char* fileDataBuffer = (char*)malloc(fileSizeInByte);

	fread(fileDataBuffer,fileSizeInByte,1,file);
	
	free(fileDataBuffer);
	fclose(file);

	return EXIT_SUCCESS;
}
