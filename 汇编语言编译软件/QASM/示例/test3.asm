;�����Զ����ɵĴ���ģ��
      STACKS  SEGMENT   STACK       ;��ջ��
              DW        128 DUP(?)  ;ע������ֻ��128���ֽ�
      STACKS  ENDS
       DATAS  SEGMENT               ;���ݶ�
;�������ﶨ����������
 
       DATAS  ENDS
       CODES  SEGMENT               ;�����
              ASSUME    CS:CODES,DS:DATAS
      START:  MOV       AX,DATAS    ;��ʼ��
              PUSH      1H
              PUSH      2H
              POP       AX
              POP       BX
              ADD       AX , BX
                                                                                    
       CODES  ENDS
              END       START
