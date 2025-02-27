#include <stdio.h>
#include <stdbool.h>

#include "log.h"
#include "generator_api.h"

static char* s64BitsRegisters  [] = {"rax", "rbx", "rcx", "rdx",  "r8", "r9",  "r10", "r11", "r12", "r13", "r14", "r15"};
static bool  registerAvailable [] = {true , true , true , true , true , true , true , true , true , true , true , true };

#define REG_NAME(regId) s64BitsRegisters[regId]

#define REG_GUARD(REGID) if(REGID == INVALID_REG_ID) { return INVALID_REG_ID; }
#define MATH_OP_REG_GUARD(LEFT,RIGHT)   \
    REG_GUARD(LEFT)                     \
    REG_GUARD(RIGHT)                    

#define OUT_ASM(asm_fmt,...) fprintf(stdout, "\t" asm_fmt "\n",##__VA_ARGS__)

#define MATH_OP(MATH_MNEMONIC, LEFT, RIGHT) \
    MATH_OP_REG_GUARD(LEFT,RIGHT) \
    OUT_ASM(MATH_MNEMONIC " %s,%s",REG_NAME(LEFT),REG_NAME(RIGHT)); \
    FreeRegister(RIGHT); \
    return LEFT

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
    MATH_OP("add",leftReg,rightReg);
}

RegId AsGenApi_Sub(const RegId leftReg, const RegId rightReg)
{
    MATH_OP("sub",leftReg,rightReg);
}

RegId AsGenApi_Mul(const RegId leftReg, const RegId rightReg)
{
    MATH_OP("mul",leftReg,rightReg);
}

RegId AsGenApi_Div(const RegId leftReg, const RegId rightReg)
{
    MATH_OP_REG_GUARD(leftReg,rightReg);
    OUT_ASM("mov rax, %s",REG_NAME(leftReg));
    OUT_ASM("cqo");
    OUT_ASM("div %s",REG_NAME(rightReg));
    FreeRegister(rightReg);
    return leftReg;
}

RegId AsGenApi_Load(const int value)
{
    RegId allocated = AllocateRegister();

    if (allocated != INVALID_REG_ID)
    {
        OUT_ASM("mov %s,%d",REG_NAME(allocated),value);
    }

    return allocated;
}

void AsGenApi_Preamble(void)
{
    puts(
        "[BITS 64]\n"
        "global main\n"
        "main:\n"
        "push rbp ; Save stask frame\n"
        "mov rbp, rsp"
    );
}

void AsGenApi_Postamble(const RegId ret_value)
{
    puts("pop rbp ; Restore stack frame");
    if (ret_value == INVALID_REG_ID)
        puts("mov rax, 0");
    else
        fprintf(stdout, "mov rax, %s\n",REG_NAME(ret_value));
    
    puts("ret");
}
