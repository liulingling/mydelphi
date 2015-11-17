#include "globals.h"
#include "symtab.h"
#include "code.h"
#include "codegen.h"
//#include "cgen.h"

#define maxn 1010

#define SRC code

int stk[maxn];
int top = -1;
int midaddrloc;
int loop = 0;
int sgrd = 0;
void genStmt(TreeNode *tree);

void codeGen(TreeNode *tree)
{
	sgrd = 0;
	top = -1;
	fprintf(SRC,"    STACKS  SEGMENT   STACK\n%");
	fprintf(SRC,"             DW        %d DUP(?)\n",256);
	fprintf(SRC,"    STACKS  ENDS\n");


	fprintf(SRC,"    CODES  SEGMENT\n");
	fprintf(SRC,"           ASSUME    CS:CODES,DS:CODES\n");
	fprintf(SRC,"CR          equ  000DH\n");
	fprintf(SRC,"LF          equ  000AH\n");
	fprintf(SRC,"KBBack      equ  0008H\n");
	create_read_write(SRC);
	create_dataseg(SRC);

	fprintf(SRC,"    START:\n");
	genStmt(tree -> child[1]);


	fprintf(SRC,"        MOV       AX,4C00H\n");
	fprintf(SRC,"        INT       21H\n");
	fprintf(SRC,"    CODES  ENDS\n");
	fprintf(SRC,"    END       START\n");
}

void genStmt(TreeNode *tree)
{
	if(tree == NULL)return ;
	midaddrloc = location;
	//top = 0;
	int lp;
	TreeNode *p0,*p1,*p2;
	if(tree -> nodekind == StmtK)
	{
		if(tree -> kind.stmt == IfK)
		{
			p0 = tree -> child[0];
			p1 = tree -> child[1];
			p2 = tree -> child[2];
			genStmt(p0);
			int lp1 = loop++,lp2 = loop++;


			fprintf(SRC,"        POP AX\n");
			fprintf(SRC,"        ADD AX , 0\n");
			fprintf(SRC,"        JZ LOOP%d\n",lp1);

			genStmt(p1);
			fprintf(SRC,"        JMP LOOP%d\n",lp2);


			fprintf(SRC,"    LOOP%d:\n",lp1);
			genStmt(p2);
			fprintf(SRC,"    LOOP%d:\n",lp2);
			genStmt(tree -> sibing);
		}
		else if(tree -> kind.stmt == RepeatK)
		{
			int lp1 = loop++,lp2 = loop++;
			stk[++top] = lp1;
			stk[++top] = lp2;
			fprintf(SRC,"    LOOP%d:\n",lp1);
			genStmt(tree->child[1]);
			genStmt(tree->child[0]);
			top--;
			top--;
			fprintf(SRC,"        POP AX\n");
			fprintf(SRC,"        CMP AX , 0H\n");
			fprintf(SRC,"        JZ LOOP%d\n",lp1);
			fprintf(SRC,"        LOOP%d:\n",lp2);
			genStmt(tree -> sibing);
		}
		else if(tree -> kind.stmt == WhileK)
		{
			int lp1 = loop++,lp2 = loop++;
			stk[++top] = lp1;
			stk[++top] = lp2;
			//fprintf(listing,"!!!!%d\n",top);
			fprintf(SRC,"    LOOP%d:\n",lp1);
			genStmt(tree->child[0]);
			fprintf(SRC,"        POP AX\n");
			fprintf(SRC,"        CMP AX , 0H\n");
			fprintf(SRC,"        JZ LOOP%d\n",lp2);
			genStmt(tree -> child[1]);
			//fprintf(listing,"!!!!%d\n",top);
			top--;
			top--;
			fprintf(SRC,"        JMP LOOP%d\n",lp1);
			fprintf(SRC,"    LOOP%d:\n",lp2);

			genStmt(tree->sibing);
		}
		else if(tree -> kind.stmt == BreakK)
		{
			if(top > 0)
				fprintf(SRC,"        JMP LOOP%d\n",stk[top]);
			else
			{
				//fprintf(listing,"    %d\n",top);
				Error = TRUE;
			}
		}
		else if(tree -> kind.stmt == ContinueK)
		{
			if(top > 0)
				fprintf(SRC,"        JMP LOOP%d\n",stk[top-1]);
			else Error = TRUE;
		}
		else if(tree -> kind.stmt == DeclK)
		{
			genStmt(tree -> child[1]);
		}
		else if(tree -> kind.stmt == AssignK)
		{
			genStmt(tree -> child[0]);
			fprintf(SRC,"        POP AX\n");
			fprintf(SRC,"        MOV %s , AX\n",tree->attr.name);
			genStmt(tree -> sibing);
		}
		else if(tree -> kind.stmt == FunK)
		{
			genStmt(tree -> child[0]);
		}
		else if(tree -> kind.stmt == ReadK)
		{
			sgrd = 1;
			genStmt(tree -> child[0]);
			sgrd = 0;
			genStmt(tree -> sibing);
		}
		else if(tree -> kind.stmt == ReadlnK)
		{
			sgrd = 1;
			genStmt(tree -> child[0]);
			sgrd = 0;
			genStmt(tree -> sibing);
		}
		else if(tree -> kind.stmt == WriteK)
		{
			sgrd = 2;
			genStmt(tree -> child[0]);
			sgrd = 0;
			genStmt(tree -> sibing);

		}
		else if(tree -> kind.stmt == WritelnK)
		{
			sgrd = 2;
			genStmt(tree -> child[0]);
			sgrd = 0;
			genStmt(tree -> sibing);
		}
	}
	else
	{
		if(tree -> kind.exp == OpK)
		{
			genStmt(tree -> child[0]);
			genStmt(tree -> child[1]);
			fprintf(SRC,"        POP BX\n");
			fprintf(SRC,"        POP AX\n");
			if(tree -> attr.op == PLUS)
			{
				fprintf(SRC,"        ADD AX , BX\n");
				fprintf(SRC,"        PUSH AX\n");
			}
			else if(tree -> attr.op == MINUS)
			{
				fprintf(SRC,"        SUB AX , BX\n");
				fprintf(SRC,"        PUSH AX\n");
			}
			else if(tree -> attr.op == MULTY)
			{
				fprintf(SRC,"        IMUL BX\n");
				fprintf(SRC,"        PUSH AX\n");
			}
			else if(tree->attr.op  == DIV)
			{
				fprintf(SRC,"        MOV DX , 0\n");
				fprintf(SRC,"        IDIV BX\n");
				fprintf(SRC,"        PUSH AX\n");
			}
			else if(tree -> attr.op == MOD)
			{
				fprintf(SRC,"        MOV DX , 0\n");
				fprintf(SRC,"        IDIV BX\n");
				fprintf(SRC,"        PUSH DX\n");
			}
			else if(tree -> attr.op == EQSN)
			{
				int lp1 = loop++,lp2 = loop++;
				fprintf(SRC,"        CMP AX , BX\n");
				fprintf(SRC,"        JNZ LOOP%d\n",lp1);

				fprintf(SRC,"        PUSH 1\n");
				fprintf(SRC,"        JMP LOOP%d\n",lp2);
				fprintf(SRC,"        LOOP%d:\n",lp1);
				fprintf(SRC,"        PUSH 0\n");
				fprintf(SRC,"        LOOP%d:\n",lp2);
			}
			else if(tree -> attr.op == GRT)
			{
				int lp1 = loop++,lp2 = loop++;
				fprintf(SRC,"        CMP AX , BX\n");
				fprintf(SRC,"        JS LOOP%d\n",lp1);
				fprintf(SRC,"        JZ LOOP%d\n",lp1);

				fprintf(SRC,"        PUSH 1\n");
				fprintf(SRC,"        JMP LOOP%d\n",lp2);

				fprintf(SRC,"        LOOP%d:\n",lp1);
				fprintf(SRC,"        PUSH 0\n");
				fprintf(SRC,"        LOOP%d:\n",lp2);
			}
			else if(tree -> attr.op == LST)
			{
				int lp1 = loop++,lp2 = loop++;
				fprintf(SRC,"        CMP AX , BX\n");
				fprintf(SRC,"        JS LOOP%d\n",lp1);

				fprintf(SRC,"        PUSH 0\n");
				fprintf(SRC,"        JMP LOOP%d\n",lp2);
				fprintf(SRC,"        LOOP%d:\n",lp1);
				fprintf(SRC,"        PUSH 1\n");
				fprintf(SRC,"        LOOP%d:\n",lp2);
			}
			else if(tree -> attr.op == GRTEQ)
			{
				int lp1 = loop++,lp2 = loop++;
				fprintf(SRC,"        CMP AX , BX\n");
				fprintf(SRC,"        JS LOOP%d\n",lp1);

				fprintf(SRC,"        PUSH 1\n");
				fprintf(SRC,"        JMP LOOP%d\n",lp2);

				fprintf(SRC,"        LOOP%d:\n",lp1);
				fprintf(SRC,"        PUSH 0\n");
				fprintf(SRC,"        LOOP%d:\n",lp2);

			}
			else if(tree -> attr.op == LSTEQ)
			{
				int lp1 = loop++,lp2 = loop++;

				fprintf(SRC,"        CMP AX , BX\n");
				fprintf(SRC,"        JS LOOP%d\n",lp1);
				fprintf(SRC,"        JZ LOOP%d\n",lp1);

				fprintf(SRC,"        PUSH 0\n");
				fprintf(SRC,"        JMP LOOP%d\n",lp2);
				fprintf(SRC,"        LOOP%d:\n",lp1);
				fprintf(SRC,"        PUSH 1\n");
				fprintf(SRC,"        LOOP%d:\n",lp2);

			}
			else if(tree -> attr.op == GRLST)
			{
				int lp1 = loop++,lp2 = loop++;
				fprintf(SRC,"        CMP AX , BX\n");
				fprintf(SRC,"        JZ LOOP%d\n",lp1);

				fprintf(SRC,"        PUSH 1\n");
				fprintf(SRC,"        JMP LOOP%d\n",lp2);
				fprintf(SRC,"        LOOP%d:\n",lp1);
				fprintf(SRC,"        PUSH 0\n");
				fprintf(SRC,"        LOOP%d:\n",lp2);
			}
		}
		else if(tree -> kind.exp == ConstK)
		{
			fprintf(SRC,"        PUSH %d\n",tree -> attr.val);
			genStmt(tree -> sibing);
		}
		else if(tree -> kind.exp == IdK)
		{
			if(sgrd == 0)
			{
				fprintf(SRC,"        PUSH %s\n",tree -> attr.name);
			}
			else if(sgrd == 1)// read
			{
				fprintf(SRC,"        LEA DI,%s\n",tree -> attr.name);
				fprintf(SRC,"        CALL      INPUT_DIGIT\n");
				fprintf(SRC,"        CALL      OUTPUT_CTLF\n");
			}
			else // print
			{
				fprintf(SRC,"        MOV AX,%s\n",tree -> attr.name);
				fprintf(SRC,"        CALL      Dec_ASCII\n");
				fprintf(SRC,"        CALL      OUTPUT_CTLF\n");
			}
			genStmt(tree -> sibing);
		}


	}
}
