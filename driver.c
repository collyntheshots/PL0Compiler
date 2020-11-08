#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "parser.h"
#include "codegen.h"
#include "vm.h"

int main(int argc, char **argv)
{
	if (argc < 2)
		printf("error : please include the file name");
	
	FILE *ifp = fopen(argv[1], "r");
	
	char *inputfile = malloc(500 * sizeof(char));
	char c = fgetc(ifp);
	int i = 0;
	while (1)
	{
		inputfile[i++] = c;
		c = fgetc(ifp);
		if (c == EOF)
			break;
	}
	inputfile[i] = '\0';
	
	printf("%s\n", inputfile);
	
	lexeme *list = lex_analyze(inputfile);
	symbol *table = parse(list);
	instruction *code = generate_code(table, list);
	virtual_machine(code);
	
	return 0;
}