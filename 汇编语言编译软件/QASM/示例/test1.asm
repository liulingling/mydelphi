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
              MOV       DS,AX
;��������������Ĵ���
 
              MOV       AX,4C00H    ;�˳�����
              INT       21H
       CODES  ENDS
              END       START
