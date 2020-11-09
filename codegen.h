#ifndef __CODEGEN_H
#define __CODEGEN_H

#include "lex.h"
#include "parser.h"

//struct for instructions
typedef struct instruction
{
	int op;
	int r;
	int l;
	int m;
} instruction;

instruction* generate_code(symbol *table, lexeme *list);

#endif
