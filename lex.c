//Collyn Lindley
//Tristin Young
//COP 3402 Systems Software
//Professor Montagne
//Oct 19 2020

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

// holds struct and two enums
#include "lex.h"

int len = 0;

/*
==============================================================================
This program uses a struct to represent an individual token, it collects them
in an array called tokens and has the lexeme held in a char array lex, the
token type held in an int tokType, and an error flag to indicate which err
causes held in an int err
==============================================================================
*/

// Prints the input file from the command line, char *fName
// Prints error and exits program if file not opened properly
void printIn(char *fName)
{
	FILE *fp = fopen(fName, "r");
	int c = 0;
	if (fp == NULL)
	{
		// Prints to std error when file is not opened properly
		fprintf(stderr, "error opening file in printIn");
		exit(0);
		return;
	}

	printf("Source Program:\n");
	// Prints the contents of the file
	while((c = fgetc(fp)) != EOF)
		printf("%c", c);

	fclose(fp);
}

// Prints the error message that corresponds to the error code
void printErr(int err)
{
	if (err == noletter)
		printf("Identifier does not start with a letter\n");
	else if (err == numtoolong)
		printf("Number is too long\n");
	else if (err == nametoolong)
		printf("Name is too long\n");
	else if (err == invalidsymbol)
		printf("Invalid symbol -> ");
}

// Prints the Lexeme Table with the proper spacing
void printTable(lexeme *tokens)
{
	int i;
	printf("Lexeme Table:\n");
	printf("lexeme\ttoken type\n");
	for (i = 0; i < len; i++)
	{
		printf("%s\t", tokens[i].lex);
		if (tokens[i].err)
			printErr(tokens[i].err);
		else
			printf("%d\n", tokens[i].tokType);
	}
}

// Prints the Lexeme List with the proper spacing
void printList(lexeme *tokens)
{
	int i;
	char buff[100];
	printf("Lexeme List:\n");
	for (i = 0; i < len; i++)
	{
		if (tokens[i].err)
		{
			continue;
		}
		printf("%d ", tokens[i].tokType);
		if (tokens[i].tokType == numbersym)
		{
			printf("%s ", tokens[i].lex);
		}
		else if (tokens[i].tokType == identsym)
		{
			printf("%s ", tokens[i].lex);
		}
	}
	printf("\n");
}

// returns true if the whole str is a number and false if it is not
int isNumeric (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtod (s, &p);
    return *p == '\0';
}

// turns the input from the file into a predeterminded token type
// returns the int for the token type
int tokenize(char *input)
{

	if (input == NULL)
		return nulsym;
	else if (isNumeric(input))
		return numbersym;
	else if (!strcmp(input, "+"))
		return plussym;
	else if (!strcmp(input, "-"))
		return minussym;
	else if (!strcmp(input, "*"))
		return multsym;
	else if (!strcmp(input, "/"))
		return slashsym;
	else if (!strcmp(input, "odd"))
		return oddsym;
	else if (!strcmp(input, "="))
		return eqsym;
	else if (!strcmp(input, "<>"))
		return neqsym;
	else if (!strcmp(input, "<"))
		return lessym;
	else if (!strcmp(input, "<="))
		return leqsym;
	else if (!strcmp(input, ">"))
		return gtrsym;
	else if (!strcmp(input, ">="))
		return geqsym;
	else if (!strcmp(input, "("))
		return lparentsym;
	else if (!strcmp(input, ")"))
		return rparentsym;
	else if (!strcmp(input, ","))
		return commasym;
	else if (!strcmp(input, ";"))
		return semicolonsym;
	else if (!strcmp(input, "."))
		return periodsym;
	else if (!strcmp(input, ":="))
		return becomessym;
	else if (!strcmp(input, "begin"))
		return beginsym;
	else if (!strcmp(input, "end"))
		return endsym;
	else if (!strcmp(input, "if"))
		return ifsym;
	else if (!strcmp(input, "then"))
		return thensym;
	else if (!strcmp(input, "while"))
		return whilesym;
	else if (!strcmp(input, "do"))
		return dosym;
	else if (!strcmp(input, "call"))
		return callsym;
	else if (!strcmp(input, "const"))
		return constsym;
	else if (!strcmp(input, "var"))
		return varsym;
	else if (!strcmp(input, "proc"))
		return procsym;
	else if (!strcmp(input, "write"))
		return writesym;
	else if (!strcmp(input, "read"))
		return readsym;
	else if (!strcmp(input, "else"))
		return elsesym;
	else
		return identsym;
}

// returns true if the char is invalid and false if valid
int isInvalid(char c)
{
	if(c == '!' ||c == '@' ||c == '#' ||c == '`' ||c == '~' ||c == '$'|| c == '%'|| c == '^' || c == '&' ||c == '_' ||c == '[' ||c == '{' || c == ']' || c == '}' || c == '?' || c == '\'' ||c == '|' || c == ':' )
		return 1;
	else
		return 0;
}

// Checks to see if the passed lexeme has an error, determined by the tType
// Returns the corresponding error code or zero if no error is found
int checkErr(char *lex, int tType)
{
	if (tType == identsym)
	{
		if (strlen(lex) >= 12)
			return nametoolong;
		else if (isdigit(lex[0]))
			return noletter;
	}
	else if (tType == numbersym)
	{
		if (strlen(lex) >= 6)
			return numtoolong;
	}
	return 0;
}

// Processes the file passed through as fName
lexeme *procFile(char *fName, lexeme *tokens)
{
	FILE *fp = fopen(fName, "r");

	// cache acts as a buffer to read the chars into
	char cache[1000];
	int c, i = 0;
	if (fp == NULL)
	{
		// Prints to std error when file is not opened properly
		fprintf(stderr, "error opening file in procFile");
		exit(0);
		return NULL;
	}

	// reads to the end of the file
	while((c = fgetc(fp)) != EOF)
	{
		// i is the index in the cache, thats why it resets to zero every loop
		i = 0;
		cache[i] = (char) c;
		// skip all white space
		if (isspace(cache[i]))
		{
			continue;
		}
		// comment handling
		else if (cache[i] == '/')
		{
			c = fgetc(fp);
			if ((char)c == '*')
			{
				while (1)
				{
					c = fgetc(fp);
					if ((char) c == '*')
					{
						c = fgetc(fp);
						if ((char) c == '/')
							break;
					}
				}
				continue;
			}
			// this is used to move the file pointer back a specific amount of
			// bytes, in this case its -1 bytes
			fseek(fp, -1, SEEK_CUR);
		}
		// becomes handling
		else if (cache[i] == ':')
		{
			if ((char)(c = fgetc(fp)) == '=')
				cache[++i] = (char)c;
		}
		// sets error code to invalid sym if the sym is invalid
		else if (isInvalid(cache[i]))
		{
			tokens[len].err = invalidsymbol;
			printErr(tokens[len].err);
			printf("%c\n", cache[i]);
			exit(tokens[len].err);
		}
		// number handling
		else if (isdigit(cache[i]))
		{
			c = fgetc(fp);
			// this is a special case where the token has a leading num but is not
			// a number but instead an invalid identifier
			if (isalpha((char)c))
			{
				while(isalpha((char)c))
				{
					cache[++i] = (char)c;
					c = fgetc(fp);
				}
			}
			// the only valid num is one that is all nums
			else
			{
				while(isdigit((char)c))
				{
					cache[++i] = (char)c;
					c = fgetc(fp);
				}
			}
			fseek(fp, -1, SEEK_CUR);
		}
		// indentifier handling
		else if (isalpha(cache[i]))
		{
			c = fgetc(fp);
			// a indentifier can be a combo of nums and letters
			while(isalpha((char)c) || isdigit((char)c))
			{
				cache[++i] = (char)c;
				c = fgetc(fp);
			}
			fseek(fp, -1, SEEK_CUR);
		}
		cache[i + 1] = '\0';
		strcpy((tokens[len].lex), cache);
		tokens[len].tokType = tokenize(cache);
		// if the err is already set to invalidsymbol we dont want to change that
		if (tokens[len].err != invalidsymbol)
		{
			tokens[len].err = checkErr(tokens[len].lex, tokens[len].tokType);
			if (tokens[len].err != 0)
			{
				printErr(tokens[len].err);
				exit(tokens[len].err);
			}
		}
		len++;
		// stop reading the program if "end." is found
		if (cache[0] == '.' && tokens[len-2].tokType == endsym)
			break;
	}

	fclose(fp);
	return tokens;
}

lexeme* lex_analyze(char *inputfile)
{
	lexeme *list = malloc(500 * sizeof(lexeme));
	list = procFile(inputfile, list);
	//printList(list);
	printf("lex is analyzed\n");
	return list;
}

/*
int main(int argc, char *argv[])
{
	lex_analyze(argv[1]);
	return 0;
}
*/


/*
int main(int argc, char *argv[])
{
	char *fName = argv[1];
	struct lexeme tokens[1000];
	int len;
	printIn(fName);
	printf("\n");
	len = procFile(fName, tokens);
	printf("\n");
	printTable(tokens, len);
	printf("\n");
	printList(tokens, len);
	return 0;
}
*/
