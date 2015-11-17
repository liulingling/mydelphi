;////////////////////////////////////////////////////////////////////
;// 字 符 矩 阵                                                    //
;// CopyRight CCSoft 2002.11.5                                     //
;//                                                                //
;// 运行后输入一个大写字符，然后看输出结果吧！                     //
;// 注意：一定是大写字符，否则后果难料。程序中注释详尽，可以参照   //
;// 改动一下，使之可以适应各种输入。                               //
;//                                                                //
;////////////////////////////////////////////////////////////////////

;///////////////////////macro define segment/////////////////////////
;output pstr
     MOUTPUT  MACRO     PSTR
              MOV       DX,OFFSET PSTR
              MOV       AH,09H
              INT       021H
              ENDM
;if pa(pr)pb,goto pl
       MLOOP  MACRO     PB,PA,PR,PL
              MOV       AL,PA
              CMP       AL,PB
              J&PR      PL
              ENDM
;///////////////////////stack segment////////////////////////////////
      STACKS  SEGMENT   STACK
              DB        128 DUP(?)
      STACKS  ENDS
;///////////////////////data segment/////////////////////////////////
       DATAS  SEGMENT
           N  DB        0           ;number to loop
       CLINE  DB        13,10,'$'   ;change line
          OC  DB        ?,'$'       ;put out a letter
          LD  DB        ?           ;record times l1 looped
          L1  DB        ?           ;first lay loop counter
         L2A  DB        ?           ;second lay loop counter
        L2AD  DB        0           ;record times l2a looped
         L2B  DB        ?
         L2C  DB        ?
         DS1  DB        9,?,?,'$'
       DATAS  ENDS
;///////////////////////code segment/////////////////////////////////
       CODES  SEGMENT
              ASSUME    CS:CODES,DS:DATAS
;///////////////////////////////////
       START  PROC      FAR
;initialize the programme
              MOV       AX,DATAS
              MOV       DS,AX
;read a character to draw
              MOV       AH,01H      ;get a character
              INT       021H
              SBB       AL,41H      ;get the number of the character
              MOV       N,AL
              MOV       CL,2
              IMUL      CL
              INC       AL
              MOV       L1,AL       ;l1=n*2+1
              MOV       LD,0        ;no loop done
              MOV       AL,N
              ADC       AL,41H
              MOV       OC,AL       ;set c n
              CALL      PINISET
;begin to draw!
              MOUTPUT   CLINE       ;change line to separate input and output
        LU1:  MLOOP     L1,1,A,EL1  ;if l1<1 then goto el1(exit loop 1)
              CALL      PL2A        ;output down characters
              CALL      PL2B        ;output flat characters
              CALL      PL2C        ;output up caharacters
;set the times 2a,2b,2c should looped
              CALL      PMARK       ;mark the line number
              MOUTPUT   CLINE       ;after a line is output,change lines
              INC       LD          ;looped 1 more
              DEC       L1
              CALL      PINISET
        LD1:  JMP       LU1
        EL1:  NOP
              MOV       AX,4C00H    ;return to dos
              INT       21H
       START  ENDP
;///////////////////////////////////
;initialize the l2a,l2b,l2c accroding to l1 and ld
     PINISET  PROC      NEAR
              MOV       AL,N
              CMP       AL,LD
              JB        NB1         ;if ld>n then goto nb1
              JE        NE1         ;if ld=n then goto ne1
              JA        NA1         ;if ld<n then goto na1
        NB1:  MOV       AL,LD
              SBB       AL,N
              INC       AL
              MOV       CL,2
              MUL       CL
              INC       AL
              MOV       L2B,AL      ;l2b=(ld-n+1)*2+1
              MOV       AL,N
              MOV       CL,2
              MUL       CL
              SBB       AL,LD
              MOV       L2A,AL      ;l2a=2*n-ld-1
              MOV       L2C,AL      ;l2a=2*n-ld-1
              JMP       PE
        NE1:  MOV       AL,N
              MOV       L2A,AL
              MOV       L2C,AL
              MOV       L2B,1
              JMP       PE
        NA1:  MOV       AL,N
              MOV       CL,2
              IMUL      CL
              INC       AL
              SBB       AL,LD
              SBB       AL,LD
              MOV       L2B,AL      ;l2b=2*n+1-ld-ld
              MOV       AL,LD
              MOV       L2A,AL      ;l2a=ld
              MOV       L2C,AL      ;l2a=ld
              JMP       PE
         PE:  NOP
              RET
     PINISET  ENDP
;///////////////////////////////////
;output the first part,down letters
        PL2A  PROC      NEAR
       LU2A:  MLOOP     L2A,1,A,EL2A            ;if l2a<1 then goto el2a(exit loop 2a)
              MOUTPUT   OC          ;output oc
              DEC       OC          ;oc=oc-1
              DEC       L2A         ;l2a=l2a-1
              JMP       LU2A
       EL2A:  NOP
              RET
        PL2A  ENDP
;///////////////////////////////////
;output the second part,flat letters
        PL2B  PROC      NEAR
       LU2B:  MLOOP     L2B,1,A,EL2B            ;if l2b<1 then goto el2b(exit loop 2b)
              MOUTPUT   OC          ;output oc
              DEC       L2B
              JMP       LU2B
       EL2B:  NOP
              RET
        PL2B  ENDP
;///////////////////////////////////
;output the third part,coresponded to the first part,up letters
        PL2C  PROC      NEAR
       LU2C:  MLOOP     L2C,1,A,EL2C            ;if l2c<1 then goto el2b(exit loop 2c)
              INC       OC          ;oc=oc+1
              MOUTPUT   OC          ;output oc
              DEC       L2C
              JMP       LU2C
       EL2C:  NOP
              RET
        PL2C  ENDP
;///////////////////////////////////
;mark the line number
       PMARK  PROC      NEAR
              PUSH      AX
              PUSH      DX

              MOV       AH,0
              MOV       AL,LD
              INC       AL
              MOV       DL,10

              DIV       DL
              ADC       AH,30H
              MOV       DS1+2,AH
              MOV       AH,0

              DIV       DL
              ADC       AH,30H
              MOV       DS1+1,AH
              MOV       AH,0

              MOV       DX,OFFSET DS1
              MOV       AH,9
              INT       21H
              POP       DX
              POP       AX
              RET
       PMARK  ENDP
       CODES  ENDS
              END       START
