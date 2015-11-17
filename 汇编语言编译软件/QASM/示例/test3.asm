;这是自动生成的代码模板
      STACKS  SEGMENT   STACK       ;堆栈段
              DW        128 DUP(?)  ;注意这里只有128个字节
      STACKS  ENDS
       DATAS  SEGMENT               ;数据段
;请在这里定义您的数据
 
       DATAS  ENDS
       CODES  SEGMENT               ;代码段
              ASSUME    CS:CODES,DS:DATAS
      START:  MOV       AX,DATAS    ;初始化
              PUSH      1H
              PUSH      2H
              POP       AX
              POP       BX
              ADD       AX , BX
                                                                                    
       CODES  ENDS
              END       START
