#include <stdio.h>
#include <stdlib.h>

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
	printf("Hello, World!\n");
	return EXIT_SUCCESS;
}
