#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"
#include "codegen.h"

#define TOKEN list[next1].tokType
#define CODE_SIZE 500

// enum of the different kinds of symbols
typedef enum {
	CONST = 1, VAR, PROC
} kind;

extern int totalSym;
int next1 = 0;
int cx = 0;
symbol *table;
lexeme *list;
instruction *code;

void printCode(void);
int lookup(char *str, int kind, int lexL);
void program1(void);
void block1(int lexL);
void statement1(int lexL);
void condition1(int lexL);
void expression1(int regToEndUpIn, int lexL);
void term1(int regToEndUpIn, int lexL);
void factor1(int regToEndUpIn, int lexL);
void emit(int op, int r, int l, int m);

void printCode(void)
{
    int i;
    printf("\n");
    for (i = 1; i < cx; i++)
    {
        printf("%d %d %d %d %d\n", i, code[i].op, code[i].r, code[i].l, code[i].m);
    }
}

int lookup(char *str, int kind, int lexL)
{
	int i;
	for (i = totalSym; i >= 0; i--)
	{
		if ((strcmp(str, table[i].name) == 0) && (lexL == table[i].level) && (kind == table[i].kind))
			return i;
	}
	return -1;
}

void program1(void)
{
	int i = 1, j;
	for (j = 1; j < totalSym; j++ )
	{
		if (table[j].kind == PROC)
		{
			table[j].val = i;
			i++;
			emit(7, 0, 0, 0);
		}
	}
	block1(0);
	for (i = 0; code[i].op == 7; i++)
	{
		code[i].m = table[i].addr;
	}
	for (i = 0; i <= cx; i++)
	{
		if (code[i].op == 5)
		{
			for (j = 0; j <= cx; j++)
			{
				if (code[j].m == code[i].m)
				{
					code[i].m = table[j].addr;
				}
			}
		}
	}
	emit(9, 0, 0, 3);
}

void unmark(char *str, int lexL, int kind)
{
	int i;
	for(i = 0; i <= totalSym; i++)
		if ((strcmp(str, table[i].name) == 0) && (lexL == table[i].level) && (kind == table[i].kind))
			table[i].mark = 0;
}

void block1(int lexL)
{
	int numSymbols = 0, numVars = 0, i;
	if (TOKEN == constsym)
	{
		do
		{
			next1++;
			numSymbols++;
			unmark(list[next1].lex, lexL, CONST);
			next1 += 3;
		} while (TOKEN == commasym);
		next1++;
	}
	if (TOKEN == varsym)
	{
		do
		{
			next1++;
			numVars++;
			numSymbols++;
			unmark(list[next1].lex, lexL, VAR);
			next1++;
		} while (TOKEN == commasym);
		next1++;
	}
	if (TOKEN == procsym)
	{
		do
		{
			next1++;
			numSymbols++;
			unmark(list[next1].lex, lexL, PROC);
			next1 += 2;
			block1(lexL + 1);
			emit(2, 0, 0, 0);
			next1++;
		} while (TOKEN == procsym);
	}
	table[next1].addr = cx;
	emit(6, 0, 0, 3 + numVars);
	statement1(lexL);
	for (i = numSymbols; i > 0; i--)
		table[i].mark = 1;
}

void statement1(int lexL)
{
	int tx, temp, temp2;
	if (TOKEN == identsym)
	{
		tx = lookup(list[next1].lex, VAR, lexL);
		next1 += 2;
		expression1(0, lexL);
		emit(4, 0, table[tx].level, table[tx].addr);
	}
	if(TOKEN == callsym)
	{
		next1++;
		tx = lookup(list[next1].lex, PROC, lexL);
		emit(5, 0, lexL - table[tx].level, table[tx].addr);
		next1++;
	}
	if (TOKEN == beginsym)
	{
		next1++;
		statement1(lexL);
		while (TOKEN == semicolonsym)
		{
			next1++;
			statement1(lexL);
		}
		next1++;
	}
	if (TOKEN == ifsym)
	{
		next1++;
		condition1(lexL);
		temp = cx;
		emit(8, 0, 0, 0);
		next1++;
		statement1(lexL);
		if (TOKEN == elsesym)
		{
			next1++;
			temp2 = cx;
			emit(7, 0, 0, 0);
			code[temp].m = cx;
			statement1(lexL);
			code[temp2].m = cx;
		}
		else
		{
			code[temp].m = cx;
		}
	}
	if (TOKEN == whilesym)
	{
		next1++;
		temp = cx;
		condition1(lexL);
		next1++;
		temp2 = cx;
		emit(8, 0, 0, 0);
		statement1(lexL);
		emit(7, 0, 0, temp);
		code[temp2].m = cx;
	}
	if (TOKEN == readsym)
	{
		next1++;
		tx = lookup(list[next1].lex, VAR, lexL);
		next1++;
		emit(9, 0, 0, 2);
		emit(4, 0, lexL - table[tx].level, table[tx].addr);
	}
	if (TOKEN == writesym)
	{
		next1++;
		expression1(0, lexL);
		emit(9, 0, 0, 1);
		next1++;
	}
}

void condition1(int lexL)
{
	if (TOKEN == oddsym)
	{
		next1++;
		expression1(0, lexL);
		emit(15, 0, 0, 0);
	}
	else
	{
		expression1(0, lexL);
		if (TOKEN == eqsym)
		{
			next1++;
			expression1(1, lexL);
			emit(17, 0, 0, 1);
		}
		if (TOKEN == neqsym)
		{
			next1++;
			expression1(1, lexL);
			emit(18, 0, 0, 1);
		}
		if (TOKEN == lessym)
		{
			next1++;
			expression1(1, lexL);
			emit(19, 0, 0, 1);
		}
		if (TOKEN == leqsym)
		{
			next1++;
			expression1(1, lexL);
			emit(20, 0, 0, 1);
		}
		if (TOKEN == gtrsym)
		{
			next1++;
			expression1(1, lexL);
			emit(21, 0, 0, 1);
		}
		if (TOKEN == geqsym)
		{
			next1++;
			expression1(1, lexL);
			emit(22, 0, 0, 1);
		}
	}
}

void expression1(int regToEndUpIn, int lexL)
{
	if (TOKEN == plussym)
	{
		next1++;
	}
	if (TOKEN == minussym)
	{
		next1++;
		term1(regToEndUpIn, lexL);
		emit(10, regToEndUpIn, 0, 0);
		while (TOKEN == plussym || TOKEN == minussym)
		{
			if (TOKEN == plussym)
			{
				next1++;
				term1(regToEndUpIn + 1, lexL);
				emit(11, regToEndUpIn, regToEndUpIn, regToEndUpIn + 1);
			}
			if (TOKEN == minussym)
			{
				next1++;
				term1(regToEndUpIn + 1, lexL);
				emit(12, regToEndUpIn, regToEndUpIn, regToEndUpIn + 1);
			}
		}
	}
	term1(regToEndUpIn, lexL);
	while (TOKEN == plussym || TOKEN == minussym)
	{
		if (TOKEN == plussym)
		{
			next1++;
			term1(regToEndUpIn + 1, lexL);
			emit(11, regToEndUpIn, regToEndUpIn, regToEndUpIn + 1);
		}
		if (TOKEN == minussym)
		{
			next1++;
			term1(regToEndUpIn + 1, lexL);
			emit(12, regToEndUpIn, regToEndUpIn, regToEndUpIn + 1);
		}
	}
}

void term1(int regToEndUpIn, int lexL)
{
	factor1(regToEndUpIn, lexL);
	while (TOKEN == multsym || TOKEN == slashsym)
	{
		if (TOKEN == multsym)
		{
			next1++;
			factor1(regToEndUpIn + 1, lexL);
			emit(13, regToEndUpIn, regToEndUpIn, regToEndUpIn + 1);
		}
		if (TOKEN == slashsym)
		{
			next1++;
			factor1(regToEndUpIn + 1, lexL);
			emit(14, regToEndUpIn, regToEndUpIn, regToEndUpIn + 1);
		}
	}
}

void factor1(int regToEndUpIn, int lexL)
{
	int tx, tx2;
	if (TOKEN == identsym)
	{
		tx = lookup(list[next1].lex, VAR, lexL);
		tx2 = lookup(list[next1].lex, CONST, lexL);
		if (tx2 >= tx)
			tx = tx2;
		if (TOKEN == constsym)
		{
			emit(1, regToEndUpIn, 0, table[tx].val);
		}
		if (TOKEN == varsym)
		{
			emit(3, regToEndUpIn, lexL - table[tx].level, table[tx].addr);
		}
		next1++;
	}
	else if (TOKEN == numbersym)
	{
		emit(1, regToEndUpIn, 0, atoi(list[next1].lex));
		next1++;
	}
	else
	{
		next1++;
		expression1(regToEndUpIn, lexL);
		next1++;
	}
}

void emit(int op, int r, int l, int m)
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
}

instruction *generate_code(symbol *t, lexeme *l)
{
    instruction *c = malloc(500 * sizeof(instruction));
    code = c;
    table = t;
    list = l;
    program1();
    return code;
}
