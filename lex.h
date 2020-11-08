#ifndef __LEX_H
#define __LEX_H

typedef struct lexeme
{
	char *name;
	int value;
	int type;
	
} lexeme;


lexeme* lex_analyze(char *inputfile);

#endif