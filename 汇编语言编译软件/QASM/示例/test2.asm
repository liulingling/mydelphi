      STACKS  SEGMENT   STACK
              DW        128 DUP(?)
      STACKS  ENDS
       DATAS  SEGMENT               ;���ݶ�
;�������ﶨ����������
 
       DATAS  ENDS
       CODES  SEGMENT               ;�����
              ASSUME    CS:CODES,DS:DATAS
      START:
        
       CODES  ENDS
              END       START
