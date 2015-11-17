#ifndef CODE_H_INCLUDED
#define CODE_H_INCLUDED
/* code emitting utilities */

/* Procedure emitComment prints a comment line
 * with comment c in the code file
 */
void emitComment( char * c );

void create_dataseg(FILE * code);
void create_read_write(FILE * code);

#endif // CODE_H_INCLUDED
