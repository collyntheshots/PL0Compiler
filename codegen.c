#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"
#include "codegen.h"

#define TOKEN list[lexL].tokType
#define CODE_SIZE 500

int lexL = 0;
int cx = 0;

instruction *emit(instruction *code, int op, int r, int l, int m);
int lookup(char *str, symbol *table);
instruction *program(symbol *table, lexeme *list, instruction *code);
instruction *block(symbol *table, lexeme *list, instruction *code);
instruction *statement(symbol *table, lexeme *list, instruction *code);
instruction *condition(symbol *table, lexeme *list, instruction *code);
instruction *expression(int regToEndUpIn, symbol *table, lexeme *list, instruction *code);
instruction *term(int regToEndUpIn, symbol *table, lexeme *list, instruction *code);
instruction *factor(int regToEndUpIn, symbol *table, lexeme *list, instruction *code);

int lookup(char *str, symbol *table)
{
	int tx;

	for (tx = 0; tx <= 500; tx++)
	{
		if (table[tx] == NULL)
			break;
		else if (strcmp(str, table[tx].name) == 0)
			return tx;
	}

	return (tx = -1);
}

instruction *program(symbol *table, lexeme *list, instruction *code)
{
	code = emit(code, 7, 0, 0, 1);
	code = block(table, list, code);
	code = emit(code, 9, 0, 0, 3);
	return code;
}

instruction *block(symbol *table, lexeme *list, instruction *code)
{
	int numVars = 0;
	if (TOKEN == constsym)
	{
		do
		{
			lexL + 4;
		} while (TOKEN == commasym);
		lexL++;
	}
	if (TOKEN == varsym)
	{
		do
		{
			numVars++;
			lexL + 2;
		} while (TOKEN == commasym);
		lexL++;
	}
	code = emit(code, 6, 0, 0, 3 + numVars);
	code = statement(table, list, code);
	return code;
}

instruction *statement(symbol *table, lexeme *list, instruction *code)
{
	int tx, temp;
	if (TOKEN == identsym)
	{
		tx = lookup(list[lexL].lex, table);
		code[lexL].op = identsym;
		lexL + 2;
		code = expression(0, table, list, code);
		code = emit(code, 4, 0, 0, table[tx].val);
	}
	if (TOKEN == beginsym)
	{
		lexL++;
		code = statement(table, list, code);
		while (TOKEN == semicolonsym)
		{
			lexL++;
			code = statement(table, list, code);
		}
		lexL++;
	}
	if (TOKEN == ifsym) ///////////////////////////////////
	{

		lexL++;
		code = condition(table, list, code);
		temp = cx;
		code = emit(code, 8, 0, 0, 0);
		lexL++;
		code = statement(table, list, code);
		code[temp].m = cx;
		//fix JPC from earlier, savedCodeIndex.M = currentCodeIndex
	}
	if (TOKEN == whilesym) //////////////////////////////////////
	{
		lexL++;
		temp = cx;
		code = condition(table, list, code);
		lexL++;
		temp = cx;
		code = emit(code, 8, 0, 0, 0);
		code = statement(table, list, code);
		code = emit(code, 7, 0, 0, cx);
		code[temp].m = cx;
		//fix JPC from earlier, savedCodeIndex.M = currentCodeIndex
	}
	if (TOKEN == readsym)
	{
		lexL++;
		tx = lookup(list[lexL].lex, table);
		lexL++;
		code = emit(code, 9, 0, 0, 2);
		code = emit(code, 4, 0, 0, table[tx].m);
	}
	if (TOKEN == writesym)
	{
		lexL++;
		tx = lookup(list[lexL].lex, table);
		if (TOKEN == varsym)
		{
			code = emit(code, 3, 0, 0, table[tx].val);
			code = emit(code, 9, 0, 0, 1);
		}
		if (TOKEN == constsym)
		{
			code = emit(code, 1, 0, 0, table[tx].val);
			code = emit(code, 9, 0, 0, 1);
		}
		lexL++;
	}
	return code;
}

instruction *condition(symbol *table, lexeme *list, instruction *code)
{
	if (TOKEN == oddsym)
	{
		lexL++;
		code = expression(0, table, list, code);
		code = emit(code, 15, 0, 0, 0);
	}
	else
	{
		code = expression(0, table, list, code);
		if (TOKEN == eqsym)
		{
			lexL++;
			code = expression(1, table, list, code);
			code = emit(code, 17, 0, 0, 1);
		}
		if (TOKEN == neqsym)
		{
			lexL++;
			code = expression(1, table, list, code);
			code = emit(code, 18, 0, 0, 1);
		}
		if (TOKEN == lessym)
		{
			lexL++;
			code = expression(1, table, list, code);
			code = emit(code, 19, 0, 0, 1);
		}
		if (TOKEN == leqsym)
		{
			lexL++;
			code = expression(1, table, list, code);
			code = emit(code, 20, 0, 0, 1);
		}
		if (TOKEN == gtrsym)
		{
			lexL++;
			code = expression(1, table, list, code);
			code = emit(code, 21, 0, 0, 1);
		}
		if (TOKEN == geqsym)
		{
			lexL++;
			code = expression(1, table, list, code);
			code = emit(code, 22, 0, 0, 1);
		}
	}
	return code;
}

instruction *expression(int regToEndUpIn, symbol *table, lexeme *list, instruction *code)
{
	if (TOKEN == plussym)
	{
		lexL++;
	}
	if (TOKEN == minussym)
	{
		lexL++;
		code = term(regToEndUpIn, table, list, code);
		code = emit(code, 10, regToEndUpIn, 0, 0);
		while (TOKEN == plussym || TOKEN == minussym)
		{
			if (TOKEN == plussym)
			{
				lexL++;
				code = term(regToEndUpIn+1, table, list, code);
				code = emit(code, 11, regToEndUpIn, regToEndUpIn, regToEndUpIn+1);
			}
			if (TOKEN == minussym)
			{
				lexL++;
				code = term(regToEndUpIn+1, table, list, code);
				code = emit(code, 12, regToEndUpIn, regToEndUpIn, regToEndUpIn+1);
			}
		}
		return code;
	}
	code = term(regToEndUpIn, table, list, code);
	while (TOKEN == plussym || TOKEN == minussym)
	{
		if (TOKEN == plussym)
		{
			lexL++;
			code = term(regToEndUpIn+1, table, list, code);
			code = emit(code, 11, regToEndUpIn, regToEndUpIn, regToEndUpIn+1);
		}
		if (TOKEN == minussym)
		{
			lexL++;
			code = term(regToEndUpIn+1, table, list, code);
			code = emit(code, 12, regToEndUpIn, regToEndUpIn, regToEndUpIn+1);
		}
	}
	return code;
}

instruction *term(int regToEndUpIn, symbol *table, lexeme *list, instruction *code)
{
	code = factor(regToEndUpIn, table, list, code);
	while (TOKEN == multsym || TOKEN == slashsym)
	{
		if (TOKEN == multsym)
		{
			lexL++;
			code = factor(regToEndUpIn + 1, table, list, code);
			code = emit(code, 13, regToEndUpIn, regToEndUpIn, regToEndUpIn + 1);
		}
		if (TOKEN == slashsym)
		{
			lexL++;
			code = factor(regToEndUpIn + 1, table, list, code);
			code = emit(code, 14, regToEndUpIn, regToEndUpIn, regToEndUpIn + 1);
		}
	}
	return code;
}

instruction *factor(int regToEndUpIn, symbol *table, lexeme *list, instruction *code)
{
	int tx;
	if (TOKEN == identsym)
	{
		tx = lookup(list[lexL].lex, table);
		if (TOKEN == constsym)
		{
			code = emit(code, 1, regToEndUpIn, 0, table[tx].val);
		}
		if (TOKEN == varsym)
		{
			code = emit(code, 3, regToEndUpIn, 0, table[tx].m);
		}
		lexL++;
	}
	else if (TOKEN == numbersym)
	{
		code = emit(code, 1, regToEndUpIn, 0, atoi(list[lexL].lex));
		lexL++;
	}
	else
	{
		lexL++;
		code = expression(regToEndUpIn, table, list, code);
		lexL++;
	}
	return code;
}

instruction *emit(instruction *code, int op, int r, int l, int m)
{
	if (cx > CODE_SIZE)
		printf("error\n");
	else
	{
		code[cx].op = op;
		code[cx].r = r;
		code[cx].l = l;
		code[cx].m = m;
		cx++;
	}
	return code;
}

instruction *generate_code(symbol *table, lexeme *list)
{
    instruction *code = malloc(500 * sizeof(instruction));
	 code = program(table, list, code);
    printf("code has been generated\n");
    return code;
}
