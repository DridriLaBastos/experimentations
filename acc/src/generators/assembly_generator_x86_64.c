#include <stdio.h>
#include <stdbool.h>

#include "log.h"
#include "generator_api.h"

static char* s64BitsRegisters  [] = {"rax", "rbx", "rcx", "rdx",  "r8", "r9",  "r10", "r11", "r12", "r13", "r14", "r15"};
static bool  registerAvailable [] = {true , true , true , true , true , true , true , true , true , true , true , true };

#define REG_NAME(regId) s64BitsRegisters[regId]

static RegId AllocateRegister(void)
{
    RegId allocated = 0;

    for (; allocated < sizeof(s64BitsRegisters) / sizeof(s64BitsRegisters[0]); allocated += 1)
    {
        if (registerAvailable[allocated])
        {
            registerAvailable[allocated] = false;
            return allocated;
        }
    }

    return INVALID_REG_ID;
}

static void FreeRegister(const RegId reg)
{
    registerAvailable[reg] = true;
}

RegId AsGenApi_Add(const RegId leftReg, const RegId rightReg)
{
    if (leftReg == INVALID_REG_ID)
    {
        //LOG_ERROR("Trying to generate an add instruction but got an invalid src register");
        return INVALID_REG_ID;
    }

    if (rightReg == INVALID_REG_ID)
    {
        //LOG_ERROR("Trying to generate an add instruction but got an invalid dest register");
        return INVALID_REG_ID;
    }

    printf("add %s, %s\n",REG_NAME(leftReg),REG_NAME(rightReg));
    FreeRegister(rightReg);

    return rightReg;
}

RegId AsGenApi_Sub(const RegId regLeft, const RegId regRight)
{
    return INVALID_REG_ID;
}

RegId AsGenApi_Mul(const RegId regLeft, const RegId regRight)
{
    return INVALID_REG_ID;
}

RegId AsGenApi_Div(const RegId regLeft, const RegId regRight)
{
    return INVALID_REG_ID;
}

RegId AsGenApi_Load(const int value)
{
    RegId allocated = AllocateRegister();

    if (allocated != INVALID_REG_ID)
    {
        printf("mov %s,%d\n",REG_NAME(allocated),value);
    }

    return allocated;
}

void AsGenApi_Preamble(void)
{
    puts(
        "global main\n"
        "main:\n"
        "push rbp # Save stask frame\n"
        "mov rbp, rsp\n"
    );
}

void AsGenApi_Postamble(void)
{
    puts(
        "pop rbp # Restore stack frame\n"
        "mov rax, 0\n"
        "ret\n"
    );
}
