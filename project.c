
#include "spimcore.h"

// sets res to an unsigned value equivalent to n bits starting at and including bit s, s = 0 is lsb of val
unsigned isolateBits(unsigned val, unsigned n, unsigned s)
{
    // chop off s bits
    val >>= s;

    // bitmask with n bits
    const unsigned mask = (1u << n) - 1;

    // sets all undesired bits to 0
    unsigned  res = val & mask;

    return res;
}
/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned *ALUresult, char *Zero)
{
    *ALUresult = A + B;
    printf("%u + %u = %u\n", A, B, *ALUresult);
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned *Mem, unsigned *instruction)
{
    // cant access oustide Mem
    if (PC >= 65536 || PC < 0)
        printf("HALT fetch > PC\n");    //******
        return 1;

    // instruction is val saved at addr PC
    *instruction = Mem[(PC >> 2)];
   

    // illegal instruction
    if (*instruction == 0){
        printf("HALT fetch null\n");    //******
        return 1;
    }

    return 0;
}
/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1, unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    *op = isolateBits(instruction, 6, 26);
    *r1 = isolateBits(instruction, 5, 21);
    *r2 = isolateBits(instruction, 5, 16);
    *r3 = isolateBits(instruction, 5, 11);
    *funct = isolateBits(instruction, 6, 0);
    *offset = isolateBits(instruction, 16, 0);
    *jsec = isolateBits(instruction, 26, 0);
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls *controls)
{
    switch (op)
    {
    // r-type
    case 0:
        (*controls).RegWrite = 1;
        (*controls).RegDst = 1;
        (*controls).ALUSrc = 0;
        (*controls).ALUOp = 7;
        (*controls).MemWrite = 0;
        (*controls).MemRead = 0;
        (*controls).MemtoReg = 0;
        (*controls).Jump = 0;
        (*controls).Branch = 0;
        break;
    // j type
    case 2: // jump
        (*controls).RegWrite = 0;
        (*controls).RegDst = 0;
        (*controls).ALUSrc = 1;
        (*controls).ALUOp = 0;
        (*controls).MemWrite = 0;
        (*controls).MemRead = 1;
        (*controls).MemtoReg = 0;
        (*controls).Jump = 1;
        (*controls).Branch = 0;
        break;
    // i types
    case 4: // BEQ
        (*controls).RegWrite = 0;
        (*controls).RegDst = 2;
        (*controls).ALUSrc = 0;
        (*controls).ALUOp = 1;
        (*controls).MemWrite = 0;
        (*controls).MemRead = 0;
        (*controls).MemtoReg = 2;
        (*controls).Jump = 0;
        (*controls).Branch = 1;
        break;
    case 8: // addi
        (*controls).RegWrite = 1;
        (*controls).RegDst = 0;
        (*controls).ALUSrc = 1;
        (*controls).ALUOp = 0;
        (*controls).MemWrite = 0;
        (*controls).MemRead = 0;
        (*controls).MemtoReg = 0;
        (*controls).Jump = 0;
        (*controls).Branch = 0;
        break;
    case 10: // slti
        (*controls).RegWrite = 1;
        (*controls).RegDst = 0;
        (*controls).ALUSrc = 1;
        (*controls).ALUOp = 2;
        (*controls).MemWrite = 0;
        (*controls).MemRead = 0;
        (*controls).MemtoReg = 0;
        (*controls).Jump = 0;
        (*controls).Branch = 0;
        break;
    case 11: // sltiu
        (*controls).RegWrite = 1;
        (*controls).RegDst = 0;
        (*controls).ALUSrc = 1;
        (*controls).ALUOp = 3;
        (*controls).MemWrite = 0;
        (*controls).MemRead = 0;
        (*controls).MemtoReg = 0;
        (*controls).Jump = 0;
        (*controls).Branch = 0;
        break;
    case 15: // lui
        (*controls).RegWrite = 1;
        (*controls).RegDst = 0;
        (*controls).ALUSrc = 1;
        (*controls).ALUOp = 0;
        (*controls).MemWrite = 0;
        (*controls).MemRead = 0;
        (*controls).MemtoReg = 1;
        (*controls).Jump = 0;
        (*controls).Branch = 0;
        break;
    case 34: // load word
        (*controls).RegWrite = 1;
        (*controls).RegDst = 0;
        (*controls).ALUSrc = 1;
        (*controls).ALUOp = 0;
        (*controls).MemWrite = 0;
        (*controls).MemRead = 1;
        (*controls).MemtoReg = 1;
        (*controls).Jump = 0;
        (*controls).Branch = 0;
        break;
    case 43: // save word
        (*controls).RegWrite = 0;
        (*controls).RegDst = 2;
        (*controls).ALUSrc = 1;
        (*controls).ALUOp = 0;
        (*controls).MemWrite = 1;
        (*controls).MemRead = 0;
        (*controls).MemtoReg = 2;
        (*controls).Jump = 0;
        (*controls).Branch = 0;
        break;
    default:
        printf("HALT decode no matching code\n");  //******
        return 1;
    }
    return 0;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned *Reg, unsigned *data1, unsigned *data2)
{
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned *extended_value)
{
    *extended_value = offset & 0b00000000000000001111111111111111; //extend by 16 bits
    if(offset & 0b1000000000000000)
        *extended_value = *extended_value | 0b11111111111111110000000000000000; //offset is neg, add leading 1's to extedned val
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc, unsigned *ALUresult, char *Zero)
{
    ALU(data1, extended_value, funct, ALUresult, Zero);
    return 0;
}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned *memdata, unsigned *Mem)
{
    //Read Mem to memdata
    if(MemRead == 1 && (MemWrite == 0 || MemWrite == 2)){
        *memdata = Mem[ALUresult >> 2];
    }
    //Write data2 to Mem
    else if((MemRead == 0 || MemRead == 2) && MemWrite == 1){
        Mem[ALUresult >> 2] = data2;
    }
    //**HALT** cant do both at once
    else if(MemRead == 1 && MemWrite == 1){
        printf("HALT rw_memory 1 1\n");  //******
        return 1;
    } 
    //nothing to do
    return 0;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned *Reg)
{
    //not writing data to reg (sw, beq)
    if (RegWrite == 0)
        return;
    
    unsigned data = ALUresult;
    if(MemtoReg == 1)
        data = memdata;

    switch(RegDst){
        case 0:
            Reg[r2] = data;
            break;
        case 1:
            Reg[r3] = data;
            break;
        default:
            break;
    }
    return;
}

/* PC update */
/* 10 Points Adam */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    //printf("%u\n", *PC);  //******
  *PC += 4;
  if(Jump==1)
    *PC = (jsec<<2) | (*PC & 0xf0000000);
  if(Branch==1 && Zero==1)
    *PC += extended_value<<2;
    //printf("%u\n", *PC);  //******
}
