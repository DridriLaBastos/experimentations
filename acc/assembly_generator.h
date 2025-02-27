#ifndef ASSEMBLLY_GENERATOR_H
#define ASSEMBLLY_GENERATOR_H

#include "ast.h"

#define INVALID_REG_ID ((unsigned int)-1)

typedef unsigned int RegId;

RegId AsGen_Add(const RegId regLeft, const RegId regRight);
RegId AsGen_Sub(const RegId regLeft, const RegId regRight);
RegId AsGen_Mul(const RegId regLeft, const RegId regRight);
RegId AsGen_Div(const RegId regLeft, const RegId regRight);

RegId AsGen_Load(const int value);

RegId AsGen_Generate(const AstNode* root);

#endif //ASSEMBLLY_GENERATOR_H
