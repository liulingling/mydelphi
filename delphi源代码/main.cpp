/****************************************************/
/* File: main.c                                     */
/* Main program for Dephi compiler                   */
/* Compiler Construction: Principles and Practice   */
/* LLL                                */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scanner.h"
#include "parse.h"
#include "analyzer.h"
#include "symtab.h"
#include "code.h"
#include "codegen.h"



/* allocate global variables */
int lineno = 0;
FILE * source;
FILE * listing;
FILE * code;

/* allocate and set tracing flags */
int EchoSource = TRUE;
int TraceScan =  FALSE;
int TraceParse = TRUE;
int TraceAnalyzer = TRUE;
int Error = false;

int main( int argc, char * argv[] )
{

  char pgm[120] = {".\\src\\in9.txt"}; /* source code file name */
  char decm[120] = {".\\dec\\out9.asm"};
 /* if(argc != 2)
  {
      fprintf(stderr,"usage: %s<filename>\n",argv[0]);
      exit(1);
  }*/
 // strcpy(pgm,argv[1]);
  //scanf("%s",pgm);
  if (strchr (pgm, '.') == NULL)
     strcat(pgm,".txt");
  source = fopen(pgm,"r");
  code = fopen(decm,"w");
  if (source==NULL)
  { fprintf(stderr,"File %s not found\n",pgm);
    exit(1);
  }
  listing = stdout; /* send listing to screen */
  fprintf(listing,"\ndehphi COMPILATION: %s\n",pgm);


  //while (getToken()!= ENDFILE);
  TreeNode *  p = NULL ;
  if(!Error)
	p =  parse();

  if(!Error)
	printTree( p );
  if(!Error)
	buildSymtab( p );
  if(!Error)
	typeCheck( p );
  if(!Error)
	codeGen(p);
  if(Error)
	fprintf(listing,"error\n");
	//system("pause");
  fclose(source);
  return 0;

}

