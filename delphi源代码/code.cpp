#include "globals.h"
#include "code.h"
#include "symtab.h"


int emitloc = 0;

void create_dataseg(FILE * code)
{
	int i;
	for (i=0; i<SIZE; ++i)
	{
		BucketList l = hashTable[i];
		while (l != NULL)
		{
			fprintf(code, "        %10s    DW ?\n", l->name);
			l = l->next;
		}
	}
}
void create_read_write(FILE * code)
{
	fprintf(code,"  OUTPUT_CHR  PROC      NEAR\n");
	fprintf(code,"              MOV       AH , 02H\n");
	fprintf(code,"              INT       21H\n");
	fprintf(code,"              RET\n");
	fprintf(code,"  OUTPUT_CHR  ENDP\n\n\n");

	fprintf(code," OUTPUT_CTLF  PROC      NEAR\n");
	fprintf(code,"              PUSH      AX\n");
	fprintf(code,"              PUSH      DX\n");
	fprintf(code,"              MOV       AH,02H\n");
	fprintf(code,"              MOV       DL,0DH\n");
	fprintf(code,"              INT       21H\n");
	fprintf(code,"              MOV       DL,0AH\n");
	fprintf(code,"              INT       21H\n");
	fprintf(code,"              POP       DX\n");
	fprintf(code,"              POP       AX\n");
	fprintf(code,"              RET\n");
	fprintf(code," OUTPUT_CTLF  ENDP\n\n");

	fprintf(code,"   GETCURSOR  PROC      NEAR\n");
	fprintf(code,"              PUSH      AX\n");
	fprintf(code,"              PUSH      BX\n");
	fprintf(code,"              PUSH      CX\n");
	fprintf(code,"              XOR       BX,BX\n");
	fprintf(code,"              MOV       AH,3\n");
	fprintf(code,"              INT       10H\n");
	fprintf(code,"              MOV       CURSOR_ROW,DH\n");
	fprintf(code,"              MOV       CURSOR_COL,DL\n");
	fprintf(code,"              POP       CX\n");
	fprintf(code,"              POP       BX\n");
	fprintf(code,"              POP       AX\n");
	fprintf(code,"              RET\n");
	fprintf(code,"  CURSOR_ROW  DB        ?\n");
	fprintf(code,"  CURSOR_COL  DB        ?\n");
	fprintf(code,"   GETCURSOR  ENDP\n\n");

	fprintf(code,"   SETCURSOR  PROC      NEAR\n");
	fprintf(code,"              PUSH      CX\n");
	fprintf(code,"              PUSH      BX\n");
	fprintf(code,"              PUSH      AX\n");
	fprintf(code,"              MOV       DH,CURSOR_ROW\n");
	fprintf(code,"              MOV       DL,CURSOR_COL\n");
	fprintf(code,"              XOR       BX,BX\n");
	fprintf(code,"              MOV       AH,2\n");
	fprintf(code,"              INT       10H\n");
	fprintf(code,"              POP       AX\n");
	fprintf(code,"              POP       BX\n");
	fprintf(code,"              POP       CX\n");
	fprintf(code,"              RET\n");
	fprintf(code,"   SETCURSOR  ENDP\n\n\n");


	fprintf(code," INPUT_DIGIT  PROC      NEAR\n");
	fprintf(code,"              PUSH      DX\n");
	fprintf(code,"              PUSH      CX\n");
	fprintf(code,"              PUSH      BX\n");
	fprintf(code,"              PUSH      DI\n");
	fprintf(code,"              LEA       DI,@@SAVE_TMP\n");
	fprintf(code,"              PUSH      DI\n");
	fprintf(code,"              CLD\n");
	fprintf(code,"              MOV       CL,@@DIGITS\n");
	fprintf(code,"              XOR       CH,CH\n");
	fprintf(code,"              PUSH      CX\n");
	fprintf(code,"    @@INPUT:  CALL      GETCURSOR\n");
	fprintf(code,"              MOV       AH,1\n");
	fprintf(code,"              INT       21H\n");
	fprintf(code,"              CMP       AL,CR\n");
	fprintf(code,"              JZ        @@ASC_DEC\n\n");

	fprintf(code," @@SAVE_DIG:  AND       AL,0FH\n");
	fprintf(code,"              STOSB\n");
	fprintf(code,"              LOOP      @@INPUT\n");
	//fprintf(code,"\n");
	fprintf(code,"  @@ASC_DEC:  MOV       AX,CX\n");
	fprintf(code,"              POP       CX\n");
	fprintf(code,"              POP       SI\n");
	fprintf(code,"              SUB       CX,AX\n");
	fprintf(code,"              XOR       BP,BP\n");
	fprintf(code,"              XOR       DX,DX\n");
	fprintf(code,"              XOR       AX,AX\n");
	fprintf(code,"              JCXZ      @@SAVE_RET\n");
	fprintf(code,"              DEC       CX\n");
	fprintf(code,"              JCXZ      @@ONE_DIGIT\n");
	fprintf(code,"              MOV       BX,10\n");
	fprintf(code,"              \n");
	fprintf(code,"  @@MUL_TEN:  LODSB\n");
	fprintf(code,"              CBW\n");
	fprintf(code,"              ADD       AX,BP\n");
	fprintf(code,"              MUL       BX\n");
	fprintf(code,"              MOV       BP,AX\n");
	fprintf(code,"              LOOP      @@MUL_TEN\n");
	fprintf(code,"            @@ONE_DIGIT:  LODSB\n");
	fprintf(code,"              CBW\n");
	fprintf(code,"              ADD       AX,BP\n");
	fprintf(code," @@SAVE_RET:  POP       DI\n");
	fprintf(code,"              CMP       @@TYPE_DATA,'B'\n");
	fprintf(code,"              JZ        $+5\n");
	fprintf(code,"              STOSW\n");
	fprintf(code,"	            JMP       $+3\n");
	fprintf(code,"              STOSB\n");
	fprintf(code,"              POP       BX\n");
	fprintf(code,"              POP       CX\n");
	fprintf(code,"              POP       DX\n");
	fprintf(code,"              RET\n\n");

	fprintf(code,"    @@DIGITS  DB        ?\n");
	fprintf(code," @@TYPE_DATA  DB        'B'\n");
	fprintf(code,"  @@SAVE_TMP  DB        16 DUP(?)\n");
	fprintf(code," INPUT_DIGIT  ENDP\n\n\n\n");
	fprintf(code,"   DEC_ASCII  PROC      NEAR\n");
	fprintf(code,"              PUSH      DX\n");
	fprintf(code,"              PUSH      BX\n");
	fprintf(code,"              PUSH      DI\n");
	fprintf(code,"              MOV       BX,10\n");
	fprintf(code,"              LEA       DI,@@TEMP_SAVE[6]\n");
	fprintf(code,"              MOV       BYTE PTR [DI],'$'\n");
	fprintf(code,"              DEC       DI\n");
	fprintf(code,"              CLD\n");
	fprintf(code,"   @@DIVIDE:  XOR       DX,DX\n");
	fprintf(code,"              DIV       BX\n");
	fprintf(code,"              OR        DL,30H\n");
	fprintf(code,"              MOV       [DI],DL\n");
	fprintf(code,"              DEC       DI\n");
	fprintf(code,"              TEST      AX,0FFFFH\n");
	fprintf(code,"              JNZ       @@DIVIDE\n");
	fprintf(code,"              INC       DI\n");
	fprintf(code,"              PUSH      DI\n");
	fprintf(code,"              POP       DX\n");
	fprintf(code,"              MOV       AH,9\n");
	fprintf(code,"              INT       21H\n");
	fprintf(code,"              POP       DI\n");
	fprintf(code,"              POP       BX\n");
	fprintf(code,"              POP       DX\n");
	fprintf(code,"              RET\n");
	fprintf(code," @@TEMP_SAVE  DB        7 DUP(?)\n");
	fprintf(code,"   DEC_ASCII  ENDP\n");

}
