#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
/* clases caracteres */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define PUNTO 4
#define FLOAT 50

/* cod Token */
#define INT_LIT 10
#define FLOAT_LIT 12
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26



int charClass;
char lexeme [100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

void addChar();
void getChar();
void getNonBlank();
int lex();
void factor();
void expr();
void term();

void error(){
	printf("error sintactico");
}

void factor() {
	printf("Enter <factor>\n");
	if (nextToken == IDENT || nextToken == INT_LIT|| nextToken ==FLOAT_LIT)
		lex();
	else {
		if (nextToken == LEFT_PAREN) {
			lex();
			expr();
			if (nextToken == RIGHT_PAREN)
				lex();
			else
				error();
		}
		else
			error();
	}
	printf("Exit <factor>\n");
}

void term() {
	printf("Enter <term>\n");
	factor();
	while (nextToken == MULT_OP || nextToken == DIV_OP) {
		lex();
		factor();
	}
	printf("Exit <term>\n");
}


void expr() {
	printf("Enter <expr>\n");
	term();
	while (nextToken == ADD_OP || nextToken == SUB_OP) {
		lex();
		term();
	}
	printf("Exit <expr>\n");
}



main() {
/* Open the input data file and process its contents */
    if ((in_fp = fopen("front.txt", "r")) == NULL)
        printf("ERROR - cannot open front.txt \n");
    else{
        getChar();
        do{
          lex();
          expr();
        }
        while (nextToken != EOF);
    }
}

    int lookup(char ch) {
        switch (ch) {
        case '(':
           addChar();
           nextToken = LEFT_PAREN;
        break;
        case ')':
           addChar();
           nextToken = RIGHT_PAREN;
        break;
        case '+':
           addChar();
           nextToken = ADD_OP;
        break;
        case '-':
           addChar();
           nextToken = SUB_OP;
        break;
        case '*':
           addChar();
           nextToken = MULT_OP;
        break;

        case '/':
          addChar();
          nextToken = DIV_OP;
          break;

        case '.':
		addChar();
		nextToken = PUNTO;
		break;

        default:
          addChar();
          nextToken = EOF;
          break;
      }
      return nextToken;
    }

/* addChar - a function to add nextChar to lexeme */
    void  addChar() {
        if
         (lexLen <= 98) {
            lexeme[lexLen++] = nextChar;
            lexeme[lexLen] = 0;
          }
        else
            printf("Error - lexeme is too long \n");
    }

/* getChar - a function to get the next character of
             input and determine its character class */
    void getChar() {
        if ((nextChar = getc(in_fp)) != EOF) {
            if (isalpha(nextChar))
                  charClass = LETTER;
            //else if ()
            else if (isdigit(nextChar))
                       charClass = DIGIT;
            else if (nextChar == '.')
			charClass = PUNTO;

            else charClass = UNKNOWN;
        }
        else
             charClass = EOF;
    }
/* getNonBlank - a function to call getChar until it
                 returns a non-whitespace character */
    void getNonBlank() {
        while (isspace(nextChar))
            getChar();
    }

/* lex - a simple lexical analyzer for arithmetic
         expressions */
    int lex() {
      lexLen = 0;
      int contar_punto = 0;
      int contar_letra = 0;
      int contar_numero =0;
      getNonBlank();
      switch (charClass) {
    /* Parse identifiers */
        case LETTER:
           addChar();
           getChar();
        while (charClass == LETTER ) {
           addChar();
           getChar();
        }
        nextToken = IDENT;
        break;
        /* Parse integer literals */
        case DIGIT:
           addChar();
           getChar();
        while (charClass == DIGIT || charClass == PUNTO || charClass == LETTER) {
			if (charClass == PUNTO) {
				contar_punto++;
			}
			if (charClass == LETTER){
                contar_letra++;
			}
			if (charClass == DIGIT){
				contar_numero++;
			}
			addChar();
			getChar();

		}
		if (contar_punto == 1 && contar_letra ==0) {
			nextToken = FLOAT_LIT;
		} else if (contar_punto > 1 || contar_letra >0) {
			nextToken = UNKNOWN;
		} else {
			nextToken = INT_LIT;
		}
		break;

        


        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
        break;
        /* EOF */
        case EOF:
              nextToken = EOF;
              lexeme[0] = 'E';
              lexeme[1] = 'O';
              lexeme[2] = 'F';
              lexeme[3] = 0;
        break;
      }

      printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
      return  nextToken;
   }
