      STACKS  SEGMENT   STACK
              DW        128 DUP(?)
      STACKS  ENDS
       DATAS  SEGMENT               ;数据段
;请在这里定义您的数据
 
       DATAS  ENDS
       CODES  SEGMENT               ;代码段
              ASSUME    CS:CODES,DS:DATAS
      START:
        
       CODES  ENDS
              END       START
