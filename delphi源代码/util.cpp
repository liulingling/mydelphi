/****************************************************/
/* File: util.c                                     */
/* Utility function implementation                  */
/* for the Dephi compiler                            */
/* Compiler Construction: Principles and Practice   */
/* LLL                                */
/****************************************************/

#include "globals.h"
#include "util.h"

/* Procedure printToken prints a token
 * and its lexeme to the listing file
 */

void printToken( TokenType token, const char* tokenString )
{ switch (token)
  {
  	case VAR:
  	case WHILE:
    case DO:
	case IF:
	case THEN:
	case ELSE:
	case INT:
	case BOOLEAN:
	case REPEAT:
	case UNTIL:
	case READ:
	case WRITE:
	case READLN:
	case WRITELN:
	case BEGIN:
	case END:
	case BREAK:
	case CONTINUE:
		fprintf(listing,"reserved word: %s\n",tokenString);
		break;
	case MOD : fprintf(listing,"%%\n");break;
    case LPAREN: fprintf(listing,"(\n"); break;
    case RPAREN: fprintf(listing,")\n"); break;
    case PLUS: fprintf(listing,"+\n"); break;
    case MINUS: fprintf(listing,"-\n"); break;
    case MULTY: fprintf(listing,"*\n"); break;
    case DIV: fprintf(listing,"/\n"); break;
	case ASSIGN: fprintf(listing,":=\n"); break;
	case EQSN: fprintf(listing,"=\n"); break;
	case GRLST: fprintf(listing,"<>\n"); break;
	case LST: fprintf(listing,"<\n"); break;
	case GRT: fprintf(listing,">\n"); break;
	case LSTEQ: fprintf(listing,"<=\n"); break;
	case GRTEQ: fprintf(listing,">=\n"); break;
	case COMMA: fprintf(listing,",\n"); break;
	case COLON: fprintf(listing,":\n"); break;
	case SEMI: fprintf(listing,";\n"); break;
    case ENDFILE: fprintf(listing,"EOF\n"); break;
    case NUM:
      fprintf(listing,
          "NUM, val= %s\n",tokenString);
      break;
    case ID:
      fprintf(listing,
          "ID, name= %s\n",tokenString);
      break;
    case ERROR:
      fprintf(listing,
          "ERROR: %s\n",tokenString);
      break;
    default: /* should never happen */
      fprintf(listing,"Unknown token: %d\n",token);
  }
}
/* Function newStmtNode creates a new statement
 * node for syntax tree construction
 */
TreeNode * newStmtNode(StmtKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
  //printf("stmt %d\n",t);
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibing = NULL;
    t->nodekind = StmtK;
    t->kind.stmt = kind;
    t->lineno = lineno;
  }
  return t;
}

/* Function newExpNode creates a new expression
 * node for syntax tree construction
 */
TreeNode * newExpNode(ExpKind kind)
{ TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));
 // printf("exp = %d\n",t);
  int i;
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else {
    for (i=0;i<MAXCHILDREN;i++) t->child[i] = NULL;
    t->sibing = NULL;
    t->nodekind = ExpK;
    t->kind.exp = kind;
    t->lineno = lineno;
    t->type = Void;
  }
  return t;
}
/* Function copyString allocates and makes a new
 * copy of an existing string
 */
char * copyString(char * s)
{ int n;
  char * t;
  if (s==NULL) return NULL;
  n = strlen(s)+1;
  t =(char *) malloc(n);
  if (t==NULL)
    fprintf(listing,"Out of memory error at line %d\n",lineno);
  else strcpy(t,s);
  return t;
}

/* Variable indentno is used by printTree to
 * store current number of spaces to indent
 */
static int indentno = 0;

/* macros to increase/decrease indentation */
#define INDENT indentno+=2
#define UNINDENT indentno-=2

/* printSpaces indents by printing spaces */
static void printSpaces(void)
{ int i;
  for (i=0;i<indentno;i++)
    fprintf(listing," ");
}


/* procedure printTree prints a syntax tree to the
 * listing file using indentation to indicate subtrees
 */
void printTree( TreeNode * tree )
{
  int i;
  INDENT;
  while (tree != NULL) {
  	//printf("%d\n",(int)tree);
    printSpaces();
    if (tree->nodekind==StmtK)
    {
    	//printf("!!!!!!!!!!!!!!\n");
		switch (tree->kind.stmt) {
		case ReadK:
			fprintf(listing,"ReadK : %s\n", tree->attr.name);
			break;
		case WriteK:
			fprintf(listing,"WriteK : %s\n", tree->attr.name);
			break;
		case ReadlnK:
			fprintf(listing,"ReadlnK : %s\n", tree->attr.name);
			break;
		case WritelnK:
			fprintf(listing,"WritelnK : %s\n", tree->attr.name);
			break;
		case WhileK:
			fprintf(listing,"WhileK : %s\n", tree->attr.name);
			break;
		case IfK:
			fprintf(listing,"IfK : %s\n", tree->attr.name);
			break;
		case RepeatK:
			fprintf(listing,"RepeatK : %s\n", tree->attr.name);
			break;
		case AssignK:
			fprintf(listing,"AssignK : %s\n", tree->attr.name);
			break;
		case VarK:
			fprintf(listing,"VarK :\n");
			break;
		case DeclK:
			fprintf(listing,"\nDeclK :\n");
			break;
	    case Stmt_listK:
			fprintf(listing,"stmt_listK :\n");
			break;
		case FunK:
			fprintf(listing,"FunK :\n");
			break;
		case BreakK:
			fprintf(listing,"break:\n");
			break;
		case ContinueK:
			fprintf(listing,"continue:\n");
			break;
        default:
			//fprintf(listing,"Unknown stmt kind");
          break;
      }
    }
    else if (tree->nodekind==ExpK)
    {
    	//printf(" !!! %d\n",tree);
    	switch (tree->kind.exp) {
        case OpK:
          fprintf(listing,"Op: ");
          printToken(tree->attr.op,"\0");
          break;
        case ConstK:
          fprintf(listing,"Const: %d\n",tree->attr.val);
          break;
        case IdK:
          fprintf(listing,"Id: %s\n",tree->attr.name);
          break;
        default:
          fprintf(listing,"Unknown ExpNode kind\n");
          break;
      }
    }
    else  fprintf(listing,"Unknown node kind\n");

    for (i=0;i<MAXCHILDREN;i++)
		if(tree -> child[i] != NULL)
         printTree(tree -> child[i]);
    tree = tree->sibing;
  }
  UNINDENT;
}


