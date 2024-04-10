#include "spimcore.h"

// sets res to an unsigned value equivalent to n bits starting at and including bit s, s = 0 is lsb of val
unsigned *isolateBits(unsigned val, unsigned n, unsigned s)
{
    // chop off s bits
    val >>= s;

    // bitmask with n bits
    const unsigned mask = (1u << n) - 1;

    unsigned *res = (unsigned *)malloc(sizeof(unsigned));

    // sets all undesired bits to 0
    *res = val & mask;

    return res;
}
/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
    // cant access oustide Mem
    if (PC >= 65536 || PC < 0)
        return 1;

    // instruction is val saved at addr PC
    *instruction = Mem[(PC >> 2)];

    // illegal instruction
    if (*instruction == 0)
        return 1;

    return 0;
}
/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    op = isolateBits(instruction, 6, 26);
    r1 = isolateBits(instruction, 5, 21);
    r2 = isolateBits(instruction, 5, 16);
    r3 = isolateBits(instruction, 5, 11);
    funct = isolateBits(instruction, 6, 0);
    offset = isolateBits(instruction, 16, 0);
    jsec = isolateBits(instruction, 26, 0);
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls)
{
    // r-type op
    if (op == 0)
    {
        *(controls->RegWrite) = 1;
        *(controls->RegDst) = 1;
        *(controls->ALUSrc) = 0;
        *(controls->MemWrite) = 0;
        *(controls->MemRead) = 0;
        *(controls->MemtoReg) = 0;
    }

    // j-type op
    if (op == 2 || op == 3)
    {

    }

    //i-type ops

    //save word op
    if (op == 43)
    {
        *(controls->RegWrite) = 0;
        *(controls->RegDst) = 2;
        *(controls->ALUSrc) = 1;
        *(controls->ALUOp) = 2;
        *(controls->MemWrite) = 1;
        *(controls->MemRead) = 0;
        *(controls->MemtoReg) = 2;
    }

    // load word op
    if (op == 34)
    {
        *(controls->RegWrite) = 1;
        *(controls->RegDst) = 0;
        *(controls->ALUSrc) = 1;
        *(controls->ALUOp) = 2;
        *(controls->MemWrite) = 0;
        *(controls->MemRead) = 1;
        *(controls->MemtoReg) = 2;
    }

    // beq op
    if (op == 4)
    {
        *(controls->RegWrite) = 0;
        *(controls->RegDst) = 2;
        *(controls->ALUSrc) = 0;
        *(controls->ALUOp) = 6;
        *(controls->MemWrite) = 0;
        *(controls->MemRead) = 0;
        *(controls->MemtoReg) = 2;
    }

}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2)
{
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned *extended_value)
{
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem)
{
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
}
