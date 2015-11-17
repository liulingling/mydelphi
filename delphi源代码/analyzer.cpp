/****************************************************/
/* File: analyze.c                                  */
/* Semantic analyzer implementation                 */
/* for the Dephi compiler                            */
/* Compiler Construction: Principles and Practice   */
/* LLL                                              */
/****************************************************/

#include "globals.h"
#include "symtab.h"
#include "analyzer.h"

/* counter for variable memory locations */
int location = 0;

/* Procedure traverse is a generic recursive
 * syntax tree traversal routine:
 * it applies preProc in preorder and postProc
 * in postorder to tree pointed to by t
 */
TokenType token;

static void insertNode(TreeNode * t,int sign);

static void traverse( TreeNode * t,int sign)
{
	if (t != NULL)
	{
		insertNode(t,sign);
		if(t -> nodekind == StmtK && t -> kind.stmt == VarK)
		{
			token = t -> attr.op;
			for(int i=0;i<MAXCHILDREN;i++)
				traverse(t->child[i],1);
		}
		else
		{
			//if(t -> nodekind == Stmt)
			for(int i=0;i<MAXCHILDREN;i++)
				traverse(t->child[i],sign);
		}
	   traverse(t->sibing,sign);
	}
}

/* nullProc is a do-nothing procedure to
 * generate preorder-only or postorder-only
 * traversals from traverse
 */
static void nullProc(TreeNode * t)
{
	if (t==NULL) return;
	else return;
}

/* Procedure insertNode inserts
 * identifiers stored in t into
 * the symbol table
 */
static void insertNode(TreeNode * t,int sign)
{
	switch (t->nodekind)
	{
		case StmtK:
		switch (t->kind.stmt)
		{
			case AssignK:
				if (st_lookup(t->attr.name) == -1)
				{
					/* not yet in table, so treat as new definition */
					fprintf(listing,"%s was undefinited\n",t -> attr.name);
					Error = true;
				}
				//else
					//st_insert(t->attr.name,t->lineno,0);
				break;
			default:
				break;
		}
		break;
		case ExpK:
		switch (t->kind.exp)
		{
			case IdK:
			if (st_lookup(t->attr.name) == -1)
			{


				/* not yet in table, so treat as new definition */
				if(sign == 1)/* variable definition */
				{
					st_insert(t->attr.name,t->lineno,location++);
					BucketList p = st_lookup1(t -> attr.name);
					//printf("%s \n",t -> attr.name);
					switch(token)
					{
						case BOOLEAN:
							//printf("%s boolean\n",t -> attr.name);
							t -> type = Boolean;
							p -> type = Boolean;
							break;
						case INT:
							//printf("%s int\n",t -> attr.name);
							t -> type = Integer;
							p -> type = Integer;
							break;
						default :
							t -> type = Void;
							p -> type = Void;
					}
				}
				else
				{
					fprintf(listing,"%s was undefinited\n",t -> attr.name);
					Error = true;
				}
			}
			else
			{
				if(sign == 1)
				{
					fprintf(listing,"%s mulity definited\n",t -> attr.name);
					Error = true;
				}
			}
			//else
			/* already in table, so ignore location,
				add line number of use only */
				//st_insert(t->attr.name,t->lineno,0);
			break;
			default:
			break;
		}
		break;
		default:
		break;
	}
}
/* Function buildSymtab constructs the symbol
 * table by preorder traversal of the syntax tree
 */
void buildSymtab(TreeNode * syntaxTree)
{
	traverse(syntaxTree,0);
	if (TraceAnalyzer)
	{
		fprintf(listing,"\nSymbol table:\n\n");
		printSymTab(listing);
	}
}

static void typeError(TreeNode * t, char * message)
{
	fprintf(listing,"Type error at line %d: %s\n",t->lineno,message);
	Error = TRUE;
}
/* Procedure checkNode performs
 * type checking at a single tree node
 */
static void checkNode(TreeNode * t)
{
	switch(t -> nodekind)
	{
		case StmtK :
			switch( t -> kind.stmt)
			{
				case AssignK:
				case FunK:
				case DeclK:
				case ReadK:
				case WriteK:
				case ReadlnK:
				case WritelnK: t -> type = Void; break;
				case WhileK:
				case RepeatK:
				case IfK:
				{
					t -> type = Void;
					treeNode *p0;
					p0 = t -> child[0];
					if(p0 -> type != Boolean && p0 -> type != Integer)
					{
						typeError(p0,"illegal boolean\n");
					}
				}break;
				default :
					t -> type = Void; break;
			}
		break;
		case ExpK :
			switch(t -> kind.exp)
			{
				case OpK:
				{
					switch(t -> attr.op)
					{
						case EQSN:
						case GRT:
						case LST:
						case GRTEQ:
						case LSTEQ:
						case GRLST:
							t -> type = Boolean;
							break;
						case PLUS:
						case MINUS:
						case MULTY:
						case DIV:
						case MOD:
						{
							treeNode *p0,*p1;
							p0 = t -> child[0];
							p1 = t -> child[1];
							if(p1 == NULL || p0 == NULL)
								typeError(t,"illegal 1 exp\n");
							else if(p1 -> type != Integer || p0 -> type != Integer)
								typeError(t,"illegal 2 exp\n");
							t -> type = Integer;
						}break;
					}
				}break;
				case ConstK:
				{
					t -> type = Integer;
				}break;
				case IdK:
				{
					BucketList p = st_lookup1(t -> attr.name);
					if(p != NULL)
						t -> type = p -> type;
					else t -> type = Void;
				}
					break;
			}
		break;
		default :
		break;
	}
}

/* Procedure typeCheck performs type checking
 * by a postorder syntax tree traversal
 */
void typeCheck(TreeNode * t)
{
	if(t != NULL)
	{
		for(int i=0;i < MAXCHILDREN ; i++)
			if(t->child[i] != NULL)
				typeCheck(t -> child[i]);
		checkNode(t);
		typeCheck(t -> sibing);
	}
}


