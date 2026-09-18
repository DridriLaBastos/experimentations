#ifndef ASSEMBLLY_GENERATOR_H
#define ASSEMBLLY_GENERATOR_H

#include "ast.h"

#define INVALID_REG_ID ((unsigned int)-1)

typedef unsigned int RegId;

void AsGen_Generate(const AstNode* root);

#endif //ASSEMBLLY_GENERATOR_H
