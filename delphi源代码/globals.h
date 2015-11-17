#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

/* MAXRSERVED number of reserved words*/
#define MAXRESERVED 18

/*SIZE is the size of the hashtable */
#define SIZE 211


typedef enum
{
	/*book - keeping tokens */
	ENDFILE,ERROR,
	/* reserved words */
	IF,THEN,ELSE,REPEAT,UNTIL,READ,WRITE,READLN,WRITELN,
	WHILE,DO,BEGIN,END,VAR,INT,BOOLEAN,BREAK,CONTINUE,
	/*multicharacter tokens */
	ID,NUM,
	/*special symbols*/
	ASSIGN,PLUS,MINUS,MULTY,DIV,LPAREN,RPAREN,SEMI,COMMA,COLON,
	EQSN,GRT,LST,GRTEQ,LSTEQ,GRLST,MOD
} TokenType;

extern FILE* source; /* source code text file */
extern FILE* listing; /* listing output text file */
extern FILE* code; /* code text file for TM simulator */

extern int lineno;/*source line number for listing */

/***********************************************/
/********** syntax tree for parsing*************/
/***********************************************/

typedef enum {StmtK,ExpK} NodeKind;
typedef enum {IfK,RepeatK,AssignK,ReadK,WriteK,WhileK,IterationK,
			 ReadlnK,WritelnK,FunK,Stmt_listK,VarK,DeclK,
			 BreakK,ContinueK} StmtKind;
typedef enum {OpK,ConstK,IdK} ExpKind;

/*ExpType is used for type checking */
typedef enum{Void,Integer,Boolean} ExpType;

#define MAXCHILDREN 4
typedef struct treeNode
{
	struct treeNode * child[MAXCHILDREN];
	struct treeNode * sibing;
	int lineno;
	NodeKind nodekind;
	union {StmtKind stmt; ExpKind exp;} kind;
	union {TokenType op;
			int val;
			char *name;}attr;
	ExpType type;
}TreeNode;

/*the list of line numbers of the source
 * code in which a variable is referenced
 */
typedef struct LineListRec
{
	int lineno;
	struct LineListRec * next;
} * LineList;
/*The record in the bucket lists for each variable,including name,
 * assigned memory location , and the list of line numbers is which
 *it appears in the source code
 */

typedef struct BucketListRec
{
	char *name;
	ExpType type;
	LineList lines;
	int memloc;/* memory location for variable */
	struct BucketListRec * next;
} * BucketList;

extern BucketList hashTable[SIZE];
/******************************************/
/****************flags for tracing*********/
/******************************************/

/* EchoSource = TRUE causes the source program to
 * be echoed to the listing file with line numbers
 * during parsing
 */
extern int EchoSource;

/* TraceScan = TRUE causes token information to be
 * printed to the listing file as each token is
 * recognized by the scanner
 */
extern int TraceScan;


/* TraceParse = TRUE causes the syntax tree to be
 * printed to the listing file in linearized form
 * (using indents for children)
 */
extern int TraceParse;

/* TraceAnalyze = TRUE causes symbol table inserts
* and lookups to be reported to the listing file
*/
extern int TraceAnalyzer;

/* TraceCode = TRUE causes comments to be written
* to the TM code file as code is generated
*/
extern int TraceCode;

/* Error = TRUE prevents further passes if an error occurs */
extern int Error;

extern int location;

#endif // GLOBALS_H_INCLUDED
