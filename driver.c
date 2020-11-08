#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lex.h"
#include "parser.h"
#include "codegen.h"
#include "vm.h"

int main(int argc, char **argv)
{
	int printLex = 0, printAss = 0, printVM = 0, i;

	if (argc < 2)
		printf("error : please include the file name");
	else if (argc > 2) // cmdline flag handling
	{
		for (i = 2; i < argc; i++)
		{
			if (strcmp(argv[i], "-l") == 0)
				printLex = 1;
			else if (strcmp(argv[i], "-a") == 0)
				printAss = 1;
			else if (strcmp(argv[i], "-v") == 0)
				printVM = 1;
		}
	}

	FILE *ifp = fopen(argv[1], "r");

	char *inputfile = malloc(500 * sizeof(char));
	char c = fgetc(ifp);
	i = 0;
	while (1)
	{
		inputfile[i++] = c;
		c = fgetc(ifp);
		if (c == EOF)
			break;
	}
	inputfile[i] = '\0';

	printf("%s\n", inputfile);
	fclose(ifp);
	lexeme *list = lex_analyze(argv[1]);
	symbol *table = parse(list);
	if (table == NULL)
		return 0;
	else
		printf("No errors, program is syntactically correct\n");
	//instruction *code = generate_code(table, list);
	//virtual_machine(code);

	return 0;
}
