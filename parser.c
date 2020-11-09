#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parser.h"
#include "lex.h"

#define TOKEN list[lexL].tokType

// enum of the different kinds of symbols
typedef enum {
	CONST = 1, VAR, PROC
} kind;

int lexL = 0;
int numSym = 0;

void add(symbol *table, int kind, char *name, int val, int level, int addr, int mark);
bool sanityCheck(char *str, symbol *table);
symbol *program(lexeme *list, symbol *table);
symbol *block(lexeme *list, symbol *table);
symbol *constDec(lexeme *list, symbol *table);
symbol *varDec(lexeme *list, symbol *table);
symbol *statement(lexeme *list, symbol *table);
symbol *condition(lexeme *list, symbol *table);
symbol *expression(lexeme *list, symbol *table);
symbol *term(lexeme *list, symbol *table);
symbol *factor(lexeme *list, symbol *table);

// adds token to the symbol table and returns the new table
void add(symbol *table, int kind, char *name, int val, int level, int addr, int mark)
{
	table[numSym].kind = kind;
	strcpy(table[numSym].name, name);
	table[numSym].val = val;
	table[numSym].level = level;
	table[numSym].addr = addr;
	table[numSym].mark = mark;
	numSym++;
	//return table;
}

// returns false if the symbol is in the table and true if not
bool sanityCheck(char *str, symbol *table)
{
	int i;
	for (i = 0; i <= numSym; i++)
		if (strcmp(str, table[i].name) == 0)
			return false;
	return true;
}

symbol *program(lexeme *list, symbol *table)
{
	table = block(list, table);
	if (TOKEN != periodsym)
	{
		printf("no period error\n");
		exit(1);
		return NULL;
	}
	return table;
}

symbol *block(lexeme *list, symbol *table)
{
	if ((table = constDec(list, table)) == NULL)
		return NULL;
	if ((table = varDec(list, table)) == NULL)
		return NULL;
	if ((table = statement(list, table)) == NULL)
		return NULL;
	return table;
}

symbol *constDec(lexeme *list, symbol *table)
{
	char cache[1000];
	if (TOKEN == constsym)
	{
		do{
			lexL++;
			if (TOKEN != identsym)
			{
				printf("error no identifier after const\n");
				exit(1);
				return NULL;
			}
			strcpy(cache, list[lexL].lex);
			if (!sanityCheck(cache, table))
			{
				printf("error const identifier already defined\n");
				exit(1);
				return NULL;
			}
			lexL++;
			if (TOKEN != eqsym)
			{
				printf("error expected '=' after identifier\n");
				exit(1);
				return NULL;
			}
			lexL++;
			if (TOKEN != numbersym)
			{
				printf("error expected int value after '='\n");
				exit(1);
				return NULL;
			}
			add(table, 1, cache, atoi(list[lexL].lex), 0, 0, -1);
			lexL++;
		} while (TOKEN == commasym);
		if (TOKEN != semicolonsym)
		{
			printf("error missing semicolon\n");
			exit(1);
			return NULL;
		}
		lexL++;
	}
	return table;
}

symbol *varDec(lexeme *list, symbol *table)
{
	int numVars = 0;
	char cache[1000];
	if (TOKEN == varsym)
	{
		do{
			numVars++;
			lexL++;
			if (TOKEN != identsym)
			{
				printf("error expected identifier after var\n");
				exit(1);
				return NULL;
			}
			strcpy(cache, list[lexL].lex);
			if (!sanityCheck(cache, table))
			{
				printf("error var identifier already defined\n");
				exit(1);
				return NULL;
			}

			add(table, 2, cache, 0, 0, numVars+2, -1);
			lexL++;
		} while (TOKEN == commasym);

		if (TOKEN != semicolonsym)
		{
			printf("error expected semicolon\n");
			exit(1);
			return NULL;
		}
		lexL++;
	}
	return table;
}

// gets the "kind" of symbol, see enum kind up top for more information
int getKind(char *str, symbol *table)
{
	int i;
	for (i = 0; i <= numSym; i++)
		if (strcmp(table[i].name, str) == 0)
			return table[i].kind;

	return -1;
}

symbol *statement(lexeme *list, symbol *table)
{
	char cache[1000];
	if (TOKEN == identsym)
	{
		strcpy(cache, list[lexL].lex);
		if (sanityCheck(cache, table))
		{
			printf("error indentifier not defined -> %s\n", cache);
			exit(1);
			return NULL;
		}
		if (getKind(cache, table) != VAR)
		{
			printf("error indentifter not of type var\n");
			exit(1);
			return NULL;
		}
		lexL++;
		if (TOKEN != becomessym)
		{
			printf("error expected ':='\n");
			exit(1);
			return NULL;
		}
		lexL++;
		return table = expression(list, table);
	}
	if (TOKEN == beginsym)
	{
		lexL++;
		if ((table = statement(list, table)) == NULL)
		{
			printf("error\n");
			return NULL;
		}

		while (TOKEN == semicolonsym)
		{
			lexL++;
			if ((table = statement(list, table)) == NULL)
			{
				printf("error\n");
				return NULL;
			}
		}
		if(TOKEN != endsym)
		{
			printf("error in statement, endsym\n");
			return NULL;
		}
		lexL++;
		return table;
	}
	if (TOKEN == ifsym)
	{
		lexL++;
		if ((table = condition(list, table)) == NULL)
		{
			printf("error\n");
			return NULL;
		}
		if (TOKEN != thensym)
		{
			printf("error expected 'then'\n");
			exit(1);
			return NULL;
		}
		lexL++;
		if ((table = statement(list, table)) == NULL)
		{
			printf("error\n");
			return NULL;
		}
		return table;
	}
	if (TOKEN == whilesym)
	{
		lexL++;
		if ((table = condition(list, table)) == NULL)
		{
			printf("error\n");
			return NULL;
		}
		if (TOKEN != dosym)
		{
			printf("error expected 'do'\n");
			exit(1);
			return NULL;
		}
		lexL++;
		if ((table = statement(list, table)) == NULL)
		{
			printf("error\n");
			return NULL;
		}
		return table;
	}
	if (TOKEN == readsym)
	{
		lexL++;
		if (TOKEN != identsym)
		{
			printf("error expected identifier after read\n");
			exit(1);
			return NULL;
		}
		strcpy(cache, list[lexL].lex);
		if (sanityCheck(cache, table))
		{
			printf("error indentifier not defined -> %s\n", cache);
			exit(1);
			return NULL;
		}
		if (getKind(cache, table) != VAR)
		{
			printf("error indentifier not of type var\n");
			exit(1);
			return NULL;
		}
		lexL++;
		return table;
	}
	if (TOKEN == writesym)
	{
		lexL++;
		if (TOKEN != identsym)
		{
			printf("error expected identifier after write\n");
			exit(1);
			return NULL;
		}
		strcpy(cache, list[lexL].lex);
		if (sanityCheck(cache, table))
		{
			printf("error indentifier not defined -> %s\n", cache);
			exit(1);
			return NULL;
		}
		lexL++;
		return table;
	}
	return table;
}

symbol *condition(lexeme *list, symbol *table)
{
	if (TOKEN == oddsym)
	{
		lexL++;
		if ((table = expression(list, table)) == NULL)
		{
			printf("error\n");
			return NULL;
		}
	}
	else
	{
		if ((table = expression(list, table)) == NULL)
		{
			printf("error\n");
			return NULL;
		}
		if (TOKEN != eqsym || TOKEN != neqsym || TOKEN != lessym || TOKEN != leqsym || TOKEN != gtrsym || TOKEN != geqsym)
		{
			printf("error expected one of the following: '!=' '=' '<>' '<' '<=' '>' '>='\n");
			exit(1);
			return NULL;
		}
		lexL++;
		table = expression(list, table);
	}
	return table;
}

symbol *expression(lexeme *list, symbol *table)
{
	if (TOKEN == plussym || TOKEN == minussym)
	{
		lexL++;
	}
	if ((table = term(list, table)) == NULL)
	{
		printf("error\n");
		return NULL;
	}
	while (TOKEN == plussym || TOKEN == minussym)
	{
		lexL++;
		if ((table = term(list, table)) == NULL)
		{
			printf("error\n");
			return NULL;
		}
	}
	return table;
}

symbol *term(lexeme *list, symbol *table)
{
	if ((table = factor(list, table)) == NULL)
	{
		printf("error\n");
		return NULL;
	}
	while (TOKEN == multsym || TOKEN == slashsym)
	{
		lexL++;
		if ((table = factor(list, table)) == NULL)
		{
			printf("error\n");
			return NULL;
		}
	}
	return table;
}

symbol *factor(lexeme *list, symbol *table)
{
	char cache[1000];
	if (TOKEN == identsym)
	{
		strcpy(cache, list[lexL].lex);
		if (sanityCheck(cache, table))
		{
			printf("error indentifier not defined -> %s\n", cache);
			exit(1);
			return NULL;
		}
		lexL++;
	}
	else if (TOKEN == numbersym)
	{
		lexL++;
	}
	else if(TOKEN == lparentsym)
	{
		lexL++;
		if ((table = expression(list, table)) == NULL)
		{
			printf("error\n");
			return NULL;
		}
		if (TOKEN != rparentsym)
		{
			printf("error expected ')'\n");
			exit(1);
			return NULL;
		}
		lexL++;
	}
	else
	{
		printf("error expected '('\n");
		exit(1);
		return NULL;
	}
	return table;
}

symbol* parse(lexeme *list)
{
	symbol *table = malloc(500 * sizeof(symbol));
	table = program(list, table);
	printf("symbols are parsed\n");
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
