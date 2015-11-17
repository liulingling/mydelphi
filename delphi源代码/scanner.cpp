/****************************************************/
/* File: scan.c                                     */
/* The scanner implementation for the Dephi compiler */
/* Compiler Construction: Principles and Practice   */
/* LLL                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scanner.h"

/* states in scanner DFA */
typedef enum
{	START,INCOMMENT,INNUM,INID,DONE,INPLUS, INSUB, INMUL,
	INDIV, INASSIGN,INLT,INGT,INNOT,INCOLON,INMOD}
   StateType;

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

/* BUFLEN = length of the input buffer for
   source code lines */
#define BUFLEN 256

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

/* getNextChar fetches the next non-blank character
   from lineBuf, reading in a new line if lineBuf is
   exhausted */
static int getNextChar(void)
{ if (!(linepos < bufsize))
  { lineno++;
    if (fgets(lineBuf,BUFLEN-1,source))
    { if (EchoSource) fprintf(listing,"%4d: %s",lineno,lineBuf);
      bufsize = strlen(lineBuf);
      linepos = 0;
      return lineBuf[linepos++];
    }
    else
    {
    	 EOF_flag = TRUE;
		 return EOF;
    }
  }
  else return lineBuf[linepos++];
}

/* ungetNextChar backtracks one character
   in lineBuf */
static void ungetNextChar(void)
{ if (!EOF_flag) linepos-- ;}

/* lookup table of reserved words */
static struct
    { char* str;
      TokenType tok;
    } reservedWords[MAXRESERVED]
   = {
   	{"while",	WHILE},
	{"do",		DO},
	{"if",		IF},
	{"then",    THEN},
	{"else",	ELSE},
	{"repeat",  REPEAT},
	{"until",   UNTIL},
	{"var",     VAR},
	{"begin",   BEGIN},
	{"end",     END},
	{"read",    READ},
	{"readln",  READLN},
	{"write",   WRITE},
	{"writeln", WRITELN},
	{"int",		INT},
	{"boolean", BOOLEAN},
	{"continue",CONTINUE},
	{"break",BREAK}
};

/* lookup an identifier to see if it is a reserved word */
/* uses linear search */
static TokenType reservedLookup (char * s)
{ int i;
  for (i=0;i<MAXRESERVED;i++)
    if (!strcmp(s,reservedWords[i].str))
      return reservedWords[i].tok;
  return ID;
}

/****************************************/
/* the primary function of the scanner  */
/****************************************/
/* function getToken returns the
 * next token in source file
 */
TokenType getToken(void)
{  /* index for storing into tokenString */
   int tokenStringIndex = 0;
   /* holds current token to be returned */
   TokenType currentToken;
   /* current state - always begins at START */
   StateType state = START;
   /* flag to indicate save to tokenString */
   int save;
   while (state != DONE)
   { int c = getNextChar();
     save = TRUE;
      switch (state)
     { case START:
         if (isdigit(c))
           state = INNUM;
         else if (isalpha(c))
           state = INID;
		 else if (c == '+')
			 state = INPLUS;
		 else if (c == '-')
			 state = INSUB;
		 else if (c == '*')
			 state = INMUL;
		 else if (c == '/')
			 state = INDIV;
		 else if (c == '%')
			 state = INMOD;
		 else if (c == '=')
			 state = INASSIGN;
		 else if (c == '<')
			 state = INLT;
		 else if (c == '>')
			 state = INGT;
		 else if( c == ':')
			 state = INCOLON;
		 else if ((c == ' ') || (c == '\t') || (c == '\n'))
           save = FALSE;
         else
         { state = DONE;
           switch (c)
           { case EOF:
               save = FALSE;
               currentToken = ENDFILE;
               break;
             case '(':
               currentToken = LPAREN;
               break;
             case ')':
               currentToken = RPAREN;
               break;
             case ',':
               currentToken = COMMA;
               break;
			 case ';':
				 currentToken = SEMI;
               break;
             default:
               currentToken = ERROR;
               break;
           }
         }
         break;
		 //注释部分处理
       case INCOMMENT:
         save = FALSE;
         if (c == EOF)
         { state = DONE;
           currentToken = ENDFILE;
         }
		 else if (c == '*')
		 {
			 state = INMUL;
		 }
         break;
		 //识别 +
	   case INPLUS:
		   state = DONE;
		   if (isalpha(c) || isdigit(c) || c==' ')
		   {
			   ungetNextChar();
			   currentToken = PLUS;
		   }
		   else
		   {
			   ungetNextChar();
			   save = FALSE;
			   currentToken = ERROR;
		   }
		   break;
		   //识别 -
	   case INSUB:
		   state = DONE;
		   if (isalpha(c) || isdigit(c) || c==' ')
		   {
			   ungetNextChar();
			   currentToken = MINUS;
		   }
		   else
		   {
			   ungetNextChar();
			   save = FALSE;
			   currentToken = ERROR;
		   }
		   break;
		   // %
		case INMOD:
			state = DONE;
			if (isalpha(c) || isdigit(c) || c==' ')
			{
				ungetNextChar();
				currentToken = MOD;
			}
			else
			{
			   ungetNextChar();
			   save = FALSE;
			   currentToken = ERROR;
			}
			break;
		   // 识别 *
	   case INMUL:
		   state = DONE;
		   if (isalpha(c) || isdigit(c) || c==' ')
		   {
			   ungetNextChar();
			   currentToken = MULTY;
		   }
		   else
		   {
			   ungetNextChar();
			   save = FALSE;
			   currentToken = ERROR;
		   }
		   break;
		   //识别 /
	   case INDIV:
		   state = DONE;
		   if (isalpha(c) || isdigit(c) || c==' ')
		   {
			   ungetNextChar();
			   currentToken = DIV;
		   }
		   else
		   {
			   ungetNextChar();
			   save = FALSE;
			   currentToken = ERROR;
		   }
		   break;
		   //识别 =
	   case INASSIGN:
		   state = DONE;
		   if (isalpha(c) || isdigit(c) || c==' ')
		   {
			   ungetNextChar();
			   currentToken = EQSN;
		   }
		   else
		   {
			   ungetNextChar();
			   save = FALSE;
			   currentToken = ERROR;
		   }
		   break;
		   //识别 < , <= ,<>
	   case INLT:
		   state = DONE;
		   if (c == '=')
			   currentToken = LSTEQ;
			else if(c == '>')
			{
				currentToken = GRLST;
			}
		    else if (isalpha(c) || isdigit(c) || c==' ')
		   {
			   ungetNextChar();
			   currentToken = LST;
		   }
		   else
		   {
			   ungetNextChar();
			   save = FALSE;
			   currentToken = ERROR;
		   }
		   break;
		   //识别 > , >=
	   case INGT:
		   state = DONE;
		   if (c == '=')
			   currentToken = GRTEQ;
		    else if (isalpha(c) || isdigit(c) || c==' ' || c=='\n')
		   {
			   ungetNextChar();
			   currentToken = GRT;
		   }
		   else
		   {
			   ungetNextChar();
			   save = FALSE;
			   currentToken = ERROR;
		   }
		   break;
		   //识别 := :
	   case INCOLON:
		   state = DONE;
		   if (c == '=')
			   currentToken = ASSIGN;
		   else if (isalpha(c) || isdigit(c) || c==' ' || c=='\n')
		   {
			   ungetNextChar();
			   currentToken = COLON;
		   }
		   else
		   {
			   ungetNextChar();
			   save = FALSE;
			   currentToken = ERROR;
		   }
		   break;
		   //识别数字
       case INNUM:
         if (!isdigit(c))
         { /* backup in the input */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = NUM;
         }
         break;
		 //识别表示符及关键字
       case INID:
         if (!isalpha(c) && !isdigit(c))
         { /* backup in the input */
           ungetNextChar();
           save = FALSE;
           state = DONE;
           currentToken = ID;
         }
         break;
       case DONE:
       default: /* should never happen */
         fprintf(listing,"Scanner Bug: state= %d\n",state);
         state = DONE;
         currentToken = ERROR;
         break;
     }
     if ((save) && (tokenStringIndex <= MAXTOKENLEN))
       tokenString[tokenStringIndex++] = (char) c;
     if (state == DONE)
     { tokenString[tokenStringIndex] = '\0';
       if (currentToken == ID)
         currentToken = reservedLookup(tokenString);
     }
   }
   if (TraceScan) {
     fprintf(listing,"\t%d: ",lineno);
     printToken(currentToken,tokenString);
   }
   return currentToken;
} /* end getToken */

