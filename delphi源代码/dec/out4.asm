      STACKS  SEGMENT   STACK
              DW        128 DUP(?)
      STACKS  ENDS
       CODES  SEGMENT
              ASSUME    CS:CODES,DS:CODES
          CR  EQU       000DH
          LF  EQU       000AH
      KBBACK  EQU       0008H
  OUTPUT_CHR  PROC      NEAR
              MOV       AH , 02H
              INT       21H
              RET
  OUTPUT_CHR  ENDP


 OUTPUT_CTLF  PROC      NEAR
              PUSH      AX
              PUSH      DX
              MOV       AH,02H
              MOV       DL,0DH
              INT       21H
              MOV       DL,0AH
              INT       21H
              POP       DX
              POP       AX
              RET
 OUTPUT_CTLF  ENDP

   GETCURSOR  PROC      NEAR
              PUSH      AX
              PUSH      BX
              PUSH      CX
              XOR       BX,BX
              MOV       AH,3
              INT       10H
              MOV       CURSOR_ROW,DH
              MOV       CURSOR_COL,DL
              POP       CX
              POP       BX
              POP       AX
              RET
  CURSOR_ROW  DB        ?
  CURSOR_COL  DB        ?
   GETCURSOR  ENDP

   SETCURSOR  PROC      NEAR
              PUSH      CX
              PUSH      BX
              PUSH      AX
              MOV       DH,CURSOR_ROW
              MOV       DL,CURSOR_COL
              XOR       BX,BX
              MOV       AH,2
              INT       10H
              POP       AX
              POP       BX
              POP       CX
              RET
   SETCURSOR  ENDP


 INPUT_DIGIT  PROC      NEAR
              PUSH      DX
              PUSH      CX
              PUSH      BX
              PUSH      DI
              LEA       DI,@@SAVE_TMP
              PUSH      DI
              CLD
              MOV       CL,@@DIGITS
              XOR       CH,CH
              PUSH      CX
    @@INPUT:  CALL      GETCURSOR
              MOV       AH,1
              INT       21H
              CMP       AL,CR
              JZ        @@ASC_DEC

 @@SAVE_DIG:  AND       AL,0FH
              STOSB
              LOOP      @@INPUT
  @@ASC_DEC:  MOV       AX,CX
              POP       CX
              POP       SI
              SUB       CX,AX
              XOR       BP,BP
              XOR       DX,DX
              XOR       AX,AX
              JCXZ      @@SAVE_RET
              DEC       CX
              JCXZ      @@ONE_DIGIT
              MOV       BX,10
              
  @@MUL_TEN:  LODSB
              CBW
              ADD       AX,BP
              MUL       BX
              MOV       BP,AX
              LOOP      @@MUL_TEN
            @@ONE_DIGIT:  LODSB
              CBW
              ADD       AX,BP
 @@SAVE_RET:  POP       DI
              CMP       @@TYPE_DATA,'B'
              JZ        $+5
              STOSW
              JMP       $+3
              STOSB
              POP       BX
              POP       CX
              POP       DX
              RET

    @@DIGITS  DB        ?
 @@TYPE_DATA  DB        'B'
  @@SAVE_TMP  DB        16 DUP(?)
 INPUT_DIGIT  ENDP



   DEC_ASCII  PROC      NEAR
              PUSH      DX
              PUSH      BX
              PUSH      DI
              MOV       BX,10
              LEA       DI,@@TEMP_SAVE[6]
              MOV       BYTE PTR [DI],'$'
              DEC       DI
              CLD
   @@DIVIDE:  XOR       DX,DX
              DIV       BX
              OR        DL,30H
              MOV       [DI],DL
              DEC       DI
              TEST      AX,0FFFFH
              JNZ       @@DIVIDE
              INC       DI
              PUSH      DI
              POP       DX
              MOV       AH,9
              INT       21H
              POP       DI
              POP       BX
              POP       DX
              RET
 @@TEMP_SAVE  DB        7 DUP(?)
   DEC_ASCII  ENDP
           A  DW        ?
           B  DW        ?
         ANS  DW        ?
      START:
              LEA       DI,A
              CALL      INPUT_DIGIT
              CALL      OUTPUT_CTLF
              PUSH      0
              POP       AX
              MOV       ANS , AX
              PUSH      1
              POP       AX
              MOV       B , AX
      LOOP0:
              PUSH      B
              PUSH      A
              POP       BX
              POP       AX
              CMP       AX , BX
              JS        LOOP2
              JZ        LOOP2
              PUSH      0
              JMP       LOOP3
      LOOP2:
              PUSH      1
      LOOP3:
              POP       AX
              CMP       AX , 0H
              JZ        LOOP1
              PUSH      ANS
              PUSH      B
              POP       BX
              POP       AX
              ADD       AX , BX
              PUSH      AX
              POP       AX
              MOV       ANS , AX
              PUSH      B
              PUSH      1
              POP       BX
              POP       AX
              ADD       AX , BX
              PUSH      AX
              POP       AX
              MOV       B , AX
              JMP       LOOP0
      LOOP1:
              MOV       AX,ANS
              CALL      DEC_ASCII
              CALL      OUTPUT_CTLF
              MOV       AX,4C00H
              INT       21H
       CODES  ENDS
              END       START
