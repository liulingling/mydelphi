;最简单的汇编语言程序
      STACKS  SEGMENT   STACK       ;堆栈段
              DW        128 DUP(?)
      STACKS  ENDS
       DATAS  SEGMENT               ;数据段
      STRING  DB        '欢迎使用轻松汇编　Welcome!',13,10,'$'
       DATAS  ENDS
       CODES  SEGMENT               ;代码段
              ASSUME    CS:CODES,DS:DATAS
      START:  MOV       AX,DATAS    ;初始化
              MOV       DS,AX
              MOV       DX,OFFSET STRING        ;显示输出字符串
              MOV       AH,9
              INT       21H
              MOV       AX,4C00H
              INT       21H         ;自动          
       CODES  ENDS
              END       START


