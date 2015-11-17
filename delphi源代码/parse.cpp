/****************************************************/
/* File: parse.c                                    */
/* The parser implementation for the Dephi compiler  */
/* Compiler Construction: Principles and Practice   */
/* LLL                               */
/****************************************************/

#include "globals.h"
#include "util.h"
#include "scanner.h"
#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
static TokenType token; /* holds current token */

/* function prototypes for recursive calls */
TreeNode * dec_list(void);
TreeNode * factor(void);
TreeNode * func_declaration(void);
TreeNode * var_stmt(void);
TreeNode * ID_list(void);
TreeNode * ID_exp();
TreeNode * stmt_list(void);
TreeNode * statement(void);
TreeNode * selection_stmt(void);
TreeNode * read_stmt(void);
TreeNode * readln_stmt(void);
TreeNode * write_stmt(void);
TreeNode * writeln_stmt(void);
TreeNode * iteration_stmt(void);
TreeNode * assign_stmt(void);
TreeNode * simple_expression(void);
TreeNode * additive_expression(void);
TreeNode * term(void);
TreeNode * factor(void);
TreeNode * break_stmt(void);
TreeNode * continue_stmt(void);

static void syntaxError(char * message)
{
	fprintf(listing,"\n>>> ");
	fprintf(listing,"Syntax error at line %d: %s",lineno,message);
	Error = TRUE;
}

static void match(TokenType expected)
{
	if (token == expected) token = getToken();
	else {
		syntaxError("match unexpected token -> ");
		printToken(token,tokenString);
		printToken(expected,tokenString);
		fprintf(listing,"      ");
	}
}
TreeNode * dec_list(void)
{
	TreeNode * t = newStmtNode(DeclK),* t0 = NULL,* t1 = NULL,* t2 = NULL;
	t -> attr.name = copyString(tokenString);
	while(token != ENDFILE)
	{
		if(token == VAR)
		{
			//printf("!!!!!!!!!!!!!!!!here\n");
			t2 = var_stmt();
			if(t0 == NULL)
			{
				t -> child[0] = t2;
				t0 = t2;
			}
			else{
				t0 -> sibing = t2;
				t0 = t2;
			}
		}
		else if(token == BEGIN)
		{
			t2 = func_declaration();
			if(t1 == NULL)
			{
				t -> child[1] = t2;
				t1 = t2;
			}
			else
			{
				t1 -> sibing = t2;
				t1 = t2;
			}
		}
		else
		{
			syntaxError("unexpected  token -> ");
			printToken(token,tokenString);
			token = getToken();
		}

	}
	return t;
}
TreeNode * func_declaration(void)
{
	TreeNode * t = newStmtNode(FunK);
	match(BEGIN);
	if(t != NULL)
		t ->child[0] = stmt_list();
	match(END);
	return t;
}
TreeNode * var_stmt(void)
{
	TreeNode * t = newStmtNode(VarK);
	/*if(t != NULL && token == VAR)
	{
		//printf("@@@@@@@@@@@@@@%s\n",tokenString);
		t->attr.name = copyString(tokenString);
	}*/
	//printf("ttt = %d\n",t);
	if(t != NULL)
		t->attr.name = copyString(tokenString);
	match(VAR);
	match(COLON);
	if(t != NULL)
		t -> child[0] = ID_list();
	match(COLON);
	if(token == INT || token == BOOLEAN)
	{
		t -> attr.op = token;
		match(token);
	}
	else{
		syntaxError("unexpected  token -> ");
		printToken(token,tokenString);
		token = getToken();
	}
	match(SEMI);
	return t;
}
TreeNode * ID_list(void)
{
	TreeNode * t = NULL,*t1 = NULL,*t2 = NULL;
	while(token == ID)
	{
		t2 = ID_exp();
		if(t == NULL && t2 != NULL)
		{
			t = t2;
			t1 = t2;
		}
		else {
			t1 -> sibing = t2;
			t1 = t2;
		}
		//else
		//{
			/*syntaxError("unexpected  token -> ");
			printToken(token,tokenString);
			token = getToken();*/
		//}
		if(token == COMMA)
			match(COMMA);
	}
	return t;
}
TreeNode * ID_exp()
{
	TreeNode * t = newExpNode(IdK);
	if(t != NULL && token == ID)
		t->attr.name = copyString(tokenString);
	match(ID);
	return t;
}
TreeNode * stmt_list(void)
{
	TreeNode * t = NULL,*t1 = NULL,*t2 = NULL;
	while(token == ID || token == IF || token == WHILE || token == REPEAT || token == READ
		|| token == READLN || token == WRITE || token == WRITELN || token == BREAK || token == CONTINUE)
	{
		//if(token == BREAK)
		//fprintf(listing,"##########\n");
		t2 = statement();
		if(t == NULL && t2 != NULL)
		{

			t = t2;
			t1 = t2;
		}
		else if (t != NULL && t2 != NULL){
			t1 -> sibing = t2;
			t1 = t2;
		}
		/*else
		{
			syntaxError("unexpected  token -> ");
			printToken(token,tokenString);
			token = getToken();
		}*/
	}
	return t;
}
TreeNode * statement(void)
{
	TreeNode * t = NULL;
	switch(token)
	{
	case ID:
		t = assign_stmt();
		break;
	case IF:
		t = selection_stmt();
		break;
	case WHILE:
	case REPEAT:
		t = iteration_stmt();
		break;
	case READ:
		t = read_stmt();
		break;
	case READLN:
		t = readln_stmt();
		break;
	case WRITE:
		t = write_stmt();
		break;
	case WRITELN:
		t = writeln_stmt();
		break;
	case BREAK:
	    t = break_stmt();
	   break;
	case CONTINUE:
		t = continue_stmt();
		break;
	default : syntaxError("unexpected  token -> ");
		printToken(token,tokenString);
		token = getToken();
		break;
	}
	return t;
}
//Ñ¡ÔñÓï¾ä
TreeNode * selection_stmt(void)
{
	TreeNode * t = newStmtNode(IfK);
	if(t != NULL && token == IF)
		t->attr.name = copyString(tokenString);
	match(IF);
	match(LPAREN);
	if(t != NULL)
		t -> child[0] = simple_expression();
	match(RPAREN);
	match(THEN);
	if(t != NULL)
	{
		match(BEGIN);
		t -> child[1] = stmt_list();
		match(END);
		match(SEMI);
	}
	if (token==ELSE)
	{
		match(ELSE);
		match(BEGIN);
			t->child[2] = stmt_list();
		match(END);
		match(SEMI);
	}
	return t;
}
TreeNode * break_stmt(void)
{
	//fprintf(listing,"!!!!!!!!!!!!\n");
	TreeNode * t = newStmtNode(BreakK);
	if(t != NULL && token == BREAK)
		t -> attr.name == copyString(tokenString);
	match(BREAK);
	match(SEMI);
	return t;
}

TreeNode * continue_stmt(void)
{
	TreeNode * t = newStmtNode(ContinueK);
	if(t != NULL && token == BREAK)
		t -> attr.name == copyString(tokenString);
	match(CONTINUE);
	match(SEMI);
	return t;
}

TreeNode * read_stmt(void)
{
	TreeNode * t = newStmtNode(ReadK);
	if(t != NULL && token == READ)
		t->attr.name = copyString(tokenString);
	match(READ);
	match(LPAREN);
	if(t != NULL)
		//match(ID);
		t -> child[0] = ID_list();
	match(RPAREN);
	match(SEMI);
	return t;
}

TreeNode * readln_stmt(void)
{
	TreeNode * t = newStmtNode(ReadlnK);
	if(t != NULL && token == READLN)
		t->attr.name = copyString(tokenString);
	match(READLN);
	match(LPAREN);
	if(t != NULL)
		t -> child[0] = ID_list();
	match(RPAREN);
	match(SEMI);
	return t;
}

TreeNode * write_stmt(void)
{
	TreeNode * t = newStmtNode(WriteK);
	if(t != NULL && token == WRITE)
		t->attr.name = copyString(tokenString);
	//printf("%d\n",(int)p);
	match(WRITE);
	match(LPAREN);
	if(t != NULL)
		t -> child[0] = ID_list();
	match(RPAREN);
	match(SEMI);
	return t;
}

TreeNode * writeln_stmt(void)
{
	TreeNode * t = newStmtNode(WritelnK);
	if(t != NULL && token == WRITELN)
		t->attr.name = copyString(tokenString);
	//printf("%d\n",(int)p);
	match(WRITELN);
	match(LPAREN);
	if(t != NULL)
		t -> child[0] = ID_list();
	match(RPAREN);
	match(SEMI);
	return t;
}
//Ñ­»·Óï¾ä
TreeNode * iteration_stmt(void)
{
	TreeNode * t = newStmtNode(IterationK);
	//printf("%d\n",(int)p);
	if (token==WHILE)								//whileÑ­»·
	{
		t->kind.stmt = WhileK;
		t->attr.name = copyString(tokenString);
		match(WHILE);

		match(LPAREN);
			t->child[0] = simple_expression();
		match(RPAREN);

		match(DO);

		match(BEGIN);
			t->child[1] = stmt_list();
		match(END);

		match(SEMI);
	}
	if (token == REPEAT)									//do-whileÑ­»·
	{
		t->kind.stmt = RepeatK;
		t->attr.name = copyString(tokenString);
		match(REPEAT);
			t->child[1] = stmt_list();
		match(UNTIL);
			if(t != NULL)
				t->child[0] =  simple_expression();
		match(SEMI);
	}
	return t;
}
TreeNode * assign_stmt(void)
{
	TreeNode * t = newStmtNode(AssignK);
	//printf("%d\n",(int)p);
	if(t != NULL && token == ID)
		t->attr.name = copyString(tokenString);
	match(ID);
	match(ASSIGN);
	if(t != NULL)
		t -> child[0] = simple_expression();
	match(SEMI);
	return t;
}
TreeNode * simple_expression(void)
{
	//printToken(token,tokenString);
	TreeNode * t = additive_expression();
	//fprintf(listing,"check ! %s\n",t->attr.name);
	//if(t->kind.exp == IdK)
	//	fprintf(listing,"check !! %s\n",t->attr.name);
	//printToken(token,tokenString);
	if (token == LST || token == GRT||token == LSTEQ || token == GRTEQ || token == EQSN)
	{
		TreeNode * p = newExpNode(OpK);
		if(p != NULL)
			p->attr.name = copyString(tokenString);
		//fprintf(listing,"check !! %s\n",p->attr.name);
		//printf("%d\n",(int)p);
		if (p != NULL)
		{
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		match(token);
		if (t!=NULL)
			t->child[1] = simple_expression();
		}
		return t;
}
TreeNode * additive_expression(void)
{
	TreeNode * t = term();
	if (token==PLUS || token==MINUS)
	{
		TreeNode * p = newExpNode(OpK);
		if (p!=NULL)
		{
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		match(token);
		if (t!=NULL)
			t->child[1] = additive_expression();
	}
	return t;

}
TreeNode * term(void)
{
	TreeNode * t = factor();
	if (token == MULTY || token == DIV || token == MOD)
	{
		TreeNode * p = newExpNode(OpK);
		if (p!=NULL)
		{
			p->child[0] = t;
			p->attr.op = token;
			t = p;
		}
		match(token);
		if (t!=NULL)
			t->child[1] = term();
	}
	return t;
}

TreeNode * factor(void)
{
	TreeNode * t = NULL;
	switch (token) {

    case LPAREN :
		match(LPAREN);
		t = simple_expression();
		match(RPAREN);
		break;
	case NUM :
		t = newExpNode(ConstK);
		if ((t!=NULL) && (token==NUM))
			t->attr.val = atoi(tokenString);
		match(NUM);
		break;
    case ID :
		t = newExpNode(IdK);
		if ((t!=NULL) && (token==ID))
			t->attr.name = copyString(tokenString);
		match(ID);
		break;
    default:
		syntaxError("unexpected token -> ");
		printToken(token,tokenString);
		token = getToken();
		break;
    }
  return t;
}

/****************************************/
/* the primary function of the parser   */
/****************************************/
/* Function parse returns the newly
 * constructed syntaxtree
 */
TreeNode * parse(void)
{ TreeNode * t = NULL;
  token = getToken();
  t = dec_list();
  if (token!=ENDFILE)
  {
  	Error = TRUE;
    syntaxError("Code ends before file\n");
  }
  return t;
}

