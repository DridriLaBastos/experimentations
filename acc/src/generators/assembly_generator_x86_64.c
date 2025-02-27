#include <stdio.h>

#include "log.h"
#include "assembly_generator.h"

RegId AsGen_Add(const RegId leftReg, const RegId rightReg)
{
    if (leftReg == INVALID_REG_ID)
    {
        LOG_ERROR("Trying to generate an add instruction but got an invalid src register");
        return INVALID_REG_ID;
    }

    if (rightReg == INVALID_REG_ID)
    {
        LOG_ERROR("Trying to generate an add instruction but got an invalid dest register");
        return INVALID_REG_ID;
    }

    return rightReg;
}

RegId AsGen_Sub(const RegId regLeft, const RegId regRight)
{
    return INVALID_REG_ID;
}

RegId AsGen_Mul(const RegId regLeft, const RegId regRight)
{
    return INVALID_REG_ID;
}

RegId AsGen_Div(const RegId regLeft, const RegId regRight)
{
    return INVALID_REG_ID;
}

RegId AsGen_Load(const int value)
{
    if ((unsigned int)value > 0xFFFF)
    {
        fprintf(stderr, "Value 0x%X to large for 16bits registers\n",value);
        return INVALID_REG_ID;
    }

    return INVALID_REG_ID;
}
