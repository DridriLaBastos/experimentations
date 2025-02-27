#ifndef ASSEMBLY_GENERATOR_API
#define ASSEMBLY_GENERATOR_API

#include "assembly_generator.h"

RegId AsGenApi_Add(const RegId regLeft, const RegId regRight);
RegId AsGenApi_Sub(const RegId regLeft, const RegId regRight);
RegId AsGenApi_Mul(const RegId regLeft, const RegId regRight);
RegId AsGenApi_Div(const RegId regLeft, const RegId regRight);

RegId AsGenApi_Load(const int value);

void AsGenApi_Preamble(void);
void AsGenApi_Postamble(const RegId ret_value);

#endif //ASSEMBLY_GENERATOR_API