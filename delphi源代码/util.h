#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

/*printtToken function is to print every token to listing*/
void printToken(TokenType ,const char *);

/*function newExpNode is to create a new statement node for syntax tree construction*/
TreeNode * newStmtNode(StmtKind kind);


/*function newExpNode is to create a new statement node for syntax tree construction*/
TreeNode * newExpNode(ExpKind);

/*function copyString allocates and makes a new copy of an existing string*/
char * copyString(char *);

/*function printTree prints a syntax tree to the listing file using indentation to indicate subtrees*/
void printTree(TreeNode *);
#endif // UTIL_H_INCLUDED
