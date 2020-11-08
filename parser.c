#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "lex.h"



symbol* parse(lexeme *list)
{
	symbol *table = malloc(500 * sizeof(symbol));
	printf("symbols are parsed\n");
	return table;
}

int main(int argc, char **argv)
{
	lexeme *list = lex_analyze(argv[1]);
	parse(list);
	return 0;
}
