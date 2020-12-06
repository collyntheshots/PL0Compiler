#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parser.h"
#include "lex.h"

#define TOKEN list[next].tokType

// enum of the different kinds of symbols
typedef enum {
	CONST = 1, VAR, PROC
} kind;

int next = 0;
int totalSym = 0;
symbol *table;
lexeme *list;

void add(int kind, char *name, int val, int level, int addr, int mark);
bool sanityCheck(char *str, int lexL, int kind);
void program(void);
void block(int lexL);
int constDec(int lexL);
int varDec(int lexL);
int procDec(int lexL);
int getKind(char *str);
void statement(int lexL);
void condition(int lexL);
void expression(int lexL);
void term(int lexL);
void factor(int lexL);

// adds token to the symbol table and returns the new table
void add(int kind, char *name, int val, int level, int addr, int mark)
{
	table[totalSym].kind = kind;
	strcpy(table[totalSym].name, name);
	table[totalSym].val = val;
	table[totalSym].level = level;
	table[totalSym].addr = addr;
	table[totalSym].mark = mark;
	totalSym++;
}

// returns false if the symbol is in the table and true if not
bool sanityCheck(char *str, int lexL, int kind)
{
	int i;
	if (kind != 0)
	{
		for (i = totalSym; i >= 0; i--)
		{
			if (strcmp(str, table[i].name) == 0 && (lexL == table[i].level) && (table[i].mark == 0) && (table[i].kind == kind))
				return false;
		}
	}
	else
	{
		for (i = 0; i <= totalSym; i++)
		{
			if (strcmp(str, table[i].name) == 0 && (lexL == table[i].level) && (table[i].mark == 0))
				return false;
		}
	}
	return true;
}

void program(void)
{
	add(PROC, "main", 0, 0, 0, -1);
	block(0);
	if (TOKEN != periodsym)
	{
		printf("no period error\n");
		exit(1);
	}
}

void block(int lexL)
{
	int numSym = 0, i = 0;
	numSym += constDec(lexL);
	numSym += varDec(lexL);
	numSym += procDec(lexL+1);
	statment(lexL);
	for (i = numSym; i > 0; i--)
		table[i].mark = 1;
}

int constDec(int lexL)
{
	char cache[1000];
	int numConsts = 0;
	if (TOKEN == constsym)
	{
		do{
			next++;
			if (TOKEN != identsym)
			{
				printf("error no identifier after const\n");
				exit(1);
			}
			strcpy(cache, list[next].lex);
			if (!sanityCheck(cache, lexL, 0))
			{
				printf("error const identifier already defined\n");
				exit(1);
			}
			next++;
			if (TOKEN != eqsym)
			{
				printf("error expected '=' after identifier\n");
				exit(1);
			}
			next++;
			if (TOKEN != numbersym)
			{
				printf("error expected int value after '='\n");
				exit(1);
			}
			add(CONST, cache, atoi(list[next].lex), lexL, 0, 0);
			next++;
			numConsts++;
		} while (TOKEN == commasym);
		if (TOKEN != semicolonsym)
		{
			printf("error missing semicolon\n");
			exit(1);
		}
		next++;
	}
	return numConsts;
}

int varDec(int lexL)
{
	int numVars = 0;
	char cache[1000];
	if (TOKEN == varsym)
	{
		do{
			numVars++;
			next++;
			if (TOKEN != identsym)
			{
				printf("error expected identifier after var\n");
				exit(1);
			}
			strcpy(cache, list[next].lex);
			if (!sanityCheck(cache, lexL, 0))
			{
				printf("error var identifier already defined\n");
				exit(1);
			}
			add(VAR, cache, 0, 0, numVars+2, 0);
			next++;
		} while (TOKEN == commasym);
		if (TOKEN != semicolonsym)
		{
			printf("error expected semicolon\n");
			exit(1);
		}
		next++;
	}
	return numVars;
}

int procDec(int lexL)
{
	int numProc = 0;
	char cache[1000];
	if (TOKEN == procsym)
	{
		do{
			next++;
			if (TOKEN != identsym)
			{
				printf("error expected identifier after proc\n");
				exit(1);
			}
			strcpy(cache, list[next].lex);
			if (!sanityCheck(cache, lexL, 0))
			{
				printf("error var identifier already defined\n");
				exit(1);
			}
			add(PROC, cache, 0, lexL, 0, 0);
			next++;
			if (TOKEN != semicolonsym)
			{
				printf("error expected semicolon\n");
				exit(1);
			}
			next++;
			block(lexL);
			numPrco++;
		} while (TOKEN == procsym);
	}
	return numProc;
}

// gets the "kind" of symbol, see enum kind up top for more information
int getKind(char *str)
{
	int i;
	for (i = 0; i <= totalSym; i++)
		if (strcmp(table[i].name, str) == 0)
			return table[i].kind;
	return -1;
}

void statement(int lexL)
{
	char cache[1000];
	if (TOKEN == identsym)
	{
		strcpy(cache, list[next].lex);
		if (!sanityCheck(cache, lexL, VAR))
		{
			printf("error indentifier not defined -> %s\n", cache);
			exit(1);
		}
		next++;
		if (TOKEN != becomessym)
		{
			printf("error expected ':='\n");
			exit(1);
		}
		next++;
		expression(lexL);
		return;
	}
	if (TOKEN == callsym)
	{
		next++;
		strcpy(cache, list[next].lex);
		if (sanityCheck(cache, lexL, PROC))
		{
			printf("error indentifier not defined -> %s\n", cache);
			exit(1);
		}
		next++;
		return;
	}
	if (TOKEN == beginsym)
	{
		next++;
		statement(lexL);
		while (TOKEN == semicolonsym)
		{
			next++;
			statement(lexL);
		}
		if (TOKEN != endsym)
		{
			printf("error in statement, endsym\n");
			exit(1);
		}
		next++;
		return;
	}
	if (TOKEN == ifsym)
	{
		next++;
		condition(lexL);
		if (TOKEN != thensym)
		{
			printf("error expected 'then'\n");
			exit(1);
		}
		next++;
		statement(lexL);
		if (TOKEN == elsesym)
		{
			next++;
			statement(lexL);
		}
		return;
	}
	if (TOKEN == whilesym)
	{
		next++;
		condition(lexL);
		if (TOKEN != dosym)
		{
			printf("error expected 'do'\n");
			exit(1);
		}
		next++;
		statement(lexL);
		return;
	}
	if (TOKEN == readsym)
	{
		next++;
		strcpy(cache, list[next].lex);
		if (sanityCheck(cache, lexL, VAR))
		{
			printf("error indentifier not defined -> %s\n", cache);
			exit(1);
		}
		next++;
		return;
	}
	if (TOKEN == writesym)
	{
		next++;
		expression(lexL);
		return;
	}
	return;
}

void condition(int lexL)
{
	if (TOKEN == oddsym)
	{
		next++;
		expression(lexL);
	}
	else
	{
		expression(lexL);
		if (TOKEN != eqsym || TOKEN != neqsym || TOKEN != lessym || TOKEN != leqsym || TOKEN != gtrsym || TOKEN != geqsym)
		{
			printf("error expected one of the following: '!=' '=' '<>' '<' '<=' '>' '>='\n");
			exit(1);
		}
		next++;
		expression(lexL);
	}
}

void expression(int lexL)
{
	if (TOKEN == plussym || TOKEN == minussym)
	{
		next++;
	}
	term(lexL);
	while (TOKEN == plussym || TOKEN == minussym)
	{
		next++;
		term(lexL);
	}
}

void term(int lexL)
{
	factor(lexL);
	while (TOKEN == multsym || TOKEN == slashsym)
	{
		next++;
		factor(lexL);
	}
}

void factor(int lexL)
{
	char cache[1000];
	if (TOKEN == identsym)
	{
		strcpy(cache, list[next].lex);
		if (!sanityCheck(cache, table, VAR) || !sanityCheck(cache, table, CONST))
		{
			printf("error indentifier not defined -> %s\n", cache);
			exit(1);
		}
		next++;
	}
	else if (TOKEN == numbersym)
	{
		next++;
	}
	else if (TOKEN == lparentsym)
	{
		next++;
		expression(lexL);
		if (TOKEN != rparentsym)
		{
			printf("error expected ')'\n");
			exit(1);
		}
		next++;
	}
	else
	{
		printf("error expected '('\n");
		exit(1);
	}
}

symbol* parse(lexeme *l)
{
	symbol *t = malloc(500 * sizeof(symbol));
	list = l;
	table = t;
	program();
	return table;
}

/*
int main(int argc, char **argv)
{
	lexeme *list = lex_analyze(argv[1]);
	parse(list);
	return 0;
}
*/
