;��򵥵Ļ�����Գ���
      STACKS  SEGMENT   STACK       ;��ջ��
              DW        128 DUP(?)
      STACKS  ENDS
       DATAS  SEGMENT               ;���ݶ�
      STRING  DB        '��ӭʹ�����ɻ�ࡡWelcome!',13,10,'$'
       DATAS  ENDS
       CODES  SEGMENT               ;�����
              ASSUME    CS:CODES,DS:DATAS
      START:  MOV       AX,DATAS    ;��ʼ��
              MOV       DS,AX
              MOV       DX,OFFSET STRING        ;��ʾ����ַ���
              MOV       AH,9
              INT       21H
              MOV       AX,4C00H
              INT       21H         ;�Զ�          
       CODES  ENDS
              END       START


