#include <stdio.h>
#include <stdlib.h>
#include "lex.h"
#include "parser.h"
#include "codegen.h"

int count = 0;

instruction *program(symbol *table, lexeme *list, instruction *code);
instruction *block(symbol *table, lexeme *list, instruction *code);
instruction *statement(symbol *table, lexeme *list, instruction *code);
instruction *condition(symbol *table, lexeme *list, instruction *code);
instruction *expression(symbol *table, lexeme *list, instruction *code);
instruction *term(symbol *table, lexeme *list, instruction *code);//review later
instruction *factor(symbol *table, lexeme *list, instruction *code);//review later



instruction *program(symbol *table, lexeme *list, instruction *code)
{
  emit(7, 0, 0, 1);

  code = block(table, list, code);
  //BLOCK
  emit(9, 0, 0, 3);
  return code;
}

instruction *generate_code(symbol *table, lexeme *list)
{
    instruction *code = malloc(500 * sizeof(instruction));
    printf("code has been generated\n");

    BLOCK
    {
        int numVars = 0;
        if (list[count].toktype == constsym)
        {
            do
            {
                count + 4;
            } while (list[count].toktype == commasym);
            count++;
        }
        if (list[count].toktype == varsym)
        {
            do
            {
                numVars++;
                count + 2;
            } while (list[count].toktype == commasym);
            count++;
        }
        emit(6, 0, 0, 3 + numVars);
        STATEMENT
    }
    STATEMENT
    {
        if (list[count].toktype == identsym)
        {
            //save symbol table index
				code[count].op = identsym;
            count + 2;
            //EXPRESSION
            emit(4, 0, 0, m comes from symbol table);
        }
        if (list[count].toktype == beginsym)
        {
            count++;
            //STATEMENT
            while (list[count].toktype == semicolonsym)
            {
                count++;
                //STATEMENT
            }
            count++;
        }
        if (list[count].toktype == ifsym)
        {
			  int temp = cx;
            count++;
            //CONDITION
            //save to code index
            emit(8, 0, 0, 0);
            count++;
            //STATEMENT
            //fix JPC from earlier, savedCodeIndex.M = currentCodeIndex
        }
        if (list[count].toktype == whilesym)
        {
            count++;
            //save code index for condition
            //CONDITION
            count++;
            //save code index for JMP
            emit(8, 0, 0, 0);
            //STATEMENT
            emit(7, 0, 0, savedCodeIndexforCondition)
            //fix JPC from earlier, savedCodeIndex.M = currentCodeIndex
        }
        if (list[count].toktype == readsym)
        {
            count++;
            //save symbol to table index
            count++;
            emit(9, 0, 0, 2);
            emit(4, 0, 0, m comes from symbol table);
        }
        if (list[count].toktype == writeesym)
        {
            count++;
            //save symbol to table index
            if (its a var)
            {
                emit(3, 0, 0, m comes from symbol table);
                emit(9, 0, 0, 1);
            }
            if (its a constant)
            {
                emit(1, 0, 0 m comes from symbol table);
                emit(9, 0, 0, 1);
            }
            count++;
        }
    }
    CONDITION
    {
        if (list[count].toktype == oddsym)
        {
            count++;
            //EXPRESSION
            emit(15, 0, 0, 0);
        }
        else
        {
            //EXPRESSION
            if (list[count].toktype == equalsym)
            {
                count++;
                //EXPRESSION
                emit(17, 0, 0, 1);
            }
            if (list[count].toktype == neqsym)
            {
                count++;
                //EXPRESSION
                emit(18, 0, 0, 1);
            }
            if (list[count].toktype == lesssym)
            {
                count++;
                //EXPRESSION
                emit(19, 0, 0, 1);
            }
            if (list[count].toktype == leqesym)
            {
                count++;
                //EXPRESSION
                emit(20, 0, 0, 1);
            }
            if (list[count].toktype == gtrsym)
            {
                count++;
                //EXPRESSION
                emit(21, 0, 0, 1);
            }
            if (list[count].toktype == geqesym)
            {
                count++;
                //EXPRESSION
                emit(22, 0, 0, 1);
            }
        }
    }
    EXPRESSION (regtoendupin = int where ans should go)
    {
        {
            if (list[count].toktype == plussym)
            {
                count++;
            }
            if (list[count].toktype == minussym)
            {
                count++;
                //TERM (regtoendupin+1, 0, 0)
                emit(10, regtoendupin, 0, 0);
                while ((list[count].toktype == plussym || list[count].toktype == minussym))
                {
                    if (list[count].toktype == plussym)
                    {}
    PROGRAM
    {

    }

                        count++;
                        //TERM (regtoendupin+1, 0, 0)
                        emit(11, regtoendupin, regtoendupin, regtoendupin + 1);
                    }
                    if (list[count].toktype == minussym)
                    {
                        count++;
                        //TERM (regtoendupin+1, 0, 0)
                        emit(12, regtoendupin, regtoendupin, regtoendupin + 1);
                    }
                }
                return; //??????
            }
            //TERM
            while ((list[count].toktype == plussym || list[count].toktype == minussym))
            {
                if (list[count].toktype == plussym)
                {
                    count++;
                    //TERM (regtoendupin+1)
                    emit(11, regtoendupin, regtoendupin, regtoendupin + 1);
                }
                if (list[count].toktype == minussym)
                {
                    count++;
                    //TERM (regtoendupin+1, 0, 0)
                    emit(12, regtoendupin, regtoendupin, regtoendupin + 1);
                }
            }
        }
        TERM(regtoendupin)
        {
            //FACTOR (regtoendupin)
            while (list[count].toktype == multsym || list[count].toktype == slashsym)
            {
                if (list[count].toktype == multsym)
                {
                    count++;
                    //FACTOR (regtoendupin+1)
                    emit(13, regtoendupin, regtoendupin, regtoendupin + 1);
                }
                if (list[count].toktype == slashsym)
                {
                    count++;
                    //FACTOR (regtoendupin+1)
                    emit(14, regtoendupin, regtoendupin, regtoendupin + 1);
                }
            }
        }
        FACTOR (regtoendupin)
        {
            if (list[count].toktype == identsym)
            {
                //save symbol table index
            }
            if (list[count].toktype == constsym)
            {
                emit(1, regtoendupin, 0, m from symbol table);
            }
            if (list[count].toktype == varsym)
            {
                emit(3, regtoendupin, 0, m from symbol table);
            }
            count++;
            else if (list[count].toktype == numbersym)
            {
                emit(1, regtoendupin, 0, number value);
                count++;
            }
            else
            {
                count++;
                //EXPRESSION(regtoendupin)
                count++;
            }
        }
        void emit(int op, int r, int l, int m)
        {
            if (cx > CODE_SIZE)
                error too much codeelse
                {
                    code[cx].op = op;
                    code[cx].r = r;
                    code[cx].l = l;
                    code[cx].m = m;
                    cx++;
                }
        }
        STATEMENT return code;
    }

instruction* generate_code(symbol *table, lexeme *list)
{
	instruction *code = malloc(500 * sizeof(instruction));
	printf("code has been generated\n");
	return code;
}
