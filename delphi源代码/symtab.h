#ifndef SYMTAB_H_INCLUDED
#define SYMTAB_H_INCLUDED
#include "globals.h"
void st_insert( char * name, int lineno, int loc );

int st_lookup(char *name);
BucketList st_lookup1(char *name);
void printSymTab(FILE * listing);
#endif // SYMTAB_H_INCLUDED
