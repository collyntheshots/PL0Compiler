#ifndef __PARSER_H
#define __PARSER_H

#include "lex.h"

typedef struct symbol
{
	int kind;
	char name[12];
	int val;
	int level; // lexL
	int addr;
	int mark; // 0 == unmarked; 1 == marked

} symbol;

symbol* parse(lexeme *list);

#endif
