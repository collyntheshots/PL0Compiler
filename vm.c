//Tristin Young
//Collyn Lindley
//COP 3402 Systems Software
//Professor Montagne
// Sep 20 2020

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "codegen.h"
#include "vm.h"
//defining max stack height / max code length as constants
#define MAX_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500

//creating max size for stack and text
instruction text[MAX_CODE_LENGTH];
//max size for stack
int stack[MAX_STACK_HEIGHT];
//creating 8 register files
int RF[8];
//creating a 'library' to determine an opcode's English equivalent
const char instructionName[22][4] = {"LIT", "RTN", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SYS", "NEG", "ADD", "SUB", "MUL",
                                     "DIV", "ODD", "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"};

//declaration of base function
int base(int l, int base);
void execute(instruction *code);
void printExecution(void);
void printCode(void);
//void initialize(void);
//void store(instruction *code);

//base function
int base(int l, int base)
{
    int b1 = base;
    while (l > 0)
    {
        b1 = stack[b1];
        l--;
    }
    return b1;
}
/*
instruction *initialize(instruction *code)
{
	int i;
	//setting all register file values to zero
	for (i = 0; i < 8; i++)
	{
		RF[i] = 0;
	}

	//setting all stack values to zero
	for (i = 0; i < MAX_STACK_HEIGHT; i++)
	{
		stack[i] = 0;
	}

	//setting all text values to zero
	for (i = 0; i < MAX_CODE_LENGTH; i++)
	{
		code[i].op = 0;
		code[i].r = 0;
		code[i].l = 0;
		code[i].m = 0;
	}

	return code;
}
*/
/*
void store(instruction *code)
{
	int pc = 0;
	//int t1, t2, t3, t4;
	//read_status = fscanf(inputPtr, "%d %d %d %d", &t1, &t2, &t3, &t4);
	do
	{
		text[pc].op = code[pc].opcode;
		text[pc].r = t2;
		text[pc].l = t3;
		text[pc].m = t4;
		//increment pc
		pc++;
		//read 4 variables at a time
		read_status = fscanf(inputPtr, "%d %d %d %d", &t1, &t2, &t3, &t4);
	//while we have not reached EOF
	} while (read_status != EOF);
}
*/

void execute(instruction *code)
{
	//printf("enter execute\n");
	int OP, R, L, M;
	//creating sp, bp, pc, and ir
	int sp = MAX_STACK_HEIGHT;
	int bp = sp - 1;
	int pc = 0;
	int ir = 0;
	int haltFlag = 1;

	//code = initialize();

	while (haltFlag == 1)
	{
		//printf("enter execute while loop %d\n", haltFlag);
		//assign vars their text counterpart
		OP = code[pc].op;
		R = code[pc].r;
		L = code[pc].l;
		M = code[pc].m;
		pc++;
		//switch for execution (22 opcodes, 22 cases)
		switch (OP)
		{
			case 1:
			   RF[R] = M;
			   break;
			case 2:
			   sp = bp + 1;
			   bp = stack[sp - 2];
			   pc = stack[sp - 3];
			   break;
			case 3:
			   RF[R] = stack[base(L, bp) - M];
			   break;
			case 4:
			   stack[base(L, bp) - M] = RF[R];
			   break;
			case 5:
			   stack[sp - 1] = base(L, bp);
			   stack[sp - 2] = bp;
			   stack[sp - 3] = pc;
			   bp = sp - 1;
			   pc = M;
			   break;
			case 6:
			   sp = sp - M;
			   break;
			case 7:
			   pc = M;
			   break;
			case 8:
			   if (RF[R] == 0)
			   {
			       pc = M;
			   }
			   break;

			case 9:
			   if (M == 1)
			   {
			       printf("%d", RF[R]);
			   }
			   else if (M == 2)
			   {
			       scanf("%d", &RF[R]);
			   }
			   else if (M == 3)
			   {
			       haltFlag = 0;
			   }
			   break;

			case 10:
			   RF[R] = -RF[R];
			   break;
			case 11:
			   RF[R] = RF[L] + RF[M];
			   break;
			case 12:
			   RF[R] = RF[L] - RF[M];
			   break;
			case 13:
			   RF[R] = RF[L] * RF[M];
			   break;
			case 14:
			   RF[R] = RF[L] / RF[M];
			   break;
			case 15:
			   RF[R] = RF[R] % 2;
			   break;
			case 16:
			   RF[R] = RF[L] % RF[M];
			   break;
			case 17:
			   if (RF[L] == RF[M])
			   {
			       RF[R] = 1;
			   }
			   else
			       RF[R] = 0;
			   break;
			case 18:
			   if (RF[L] != RF[M])
			   {
			       RF[R] = 1;
			   }
			   else
			       RF[R] = 0;
			   break;
			case 19:
			   if (RF[L] < RF[M])
			   {
			       RF[R] = 1;
			   }
			   else
			       RF[R] = 0;
			   break;
			case 20:
			   if (RF[L] <= RF[M])
			   {
			       RF[R] = 1;
			   }
			   else
			       RF[R] = 0;
			   break;
			case 21:
			   if (RF[L] > RF[M])
			   {
			       RF[R] = 1;
			   }
			   else
			       RF[R] = 0;
			   break;
			case 22:
			   if (RF[L] >= RF[M])
			   {
			       RF[R] = 1;
			   }
			   else
			       RF[R] = 0;
			   break;
		}

	}
	//printf("exit execute\n");
}

void printExecution(void)
{

}

void printCode(void)
{

}

void virtual_machine(instruction *code)
{
	//printf("in vm\n");
	execute(code);
	printf("print the stack\n");
	return;
}
