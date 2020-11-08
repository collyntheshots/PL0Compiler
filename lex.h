#ifndef __LEX_H
#define __LEX_H

#include <limits.h>

typedef struct lexeme{
	char lex[100];
	int tokType;
	int err;

} lexeme;

// Error
typedef enum {
	noletter = INT_MIN, numtoolong, nametoolong, invalidsymbol
} err;

// token type
typedef enum {
nulsym = 1, identsym, numbersym, plussym, minussym,
multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
periodsym, becomessym, beginsym, endsym, ifsym, thensym,
whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
readsym , elsesym } token_type;

lexeme* lex_analyze(char *inputfile);

#endif
