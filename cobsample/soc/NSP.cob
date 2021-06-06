000010 IDENTIFICATION         DIVISION.
000020 PROGRAM-ID.            NSPCOMP.
000030*
000040 ENVIRONMENT            DIVISION.
000050*
000060 DATA                   DIVISION.
000070 WORKING-STORAGE        SECTION.
000080*
000090 01  WORK-AREA-A.
000100     03  NSP            PIC  N(2) VALUE SPACE.
000110     03  ASP            PIC  X(2) VALUE SPACE.
000120*     03  ER-N-ASX       PIC  N(1) VALUE   '  '.     *> err ���p��, N���ڂ�VALUE�����
000130     03  OK-N-ASN       PIC  N(1) VALUE NC'�@'.     *> compile-ok  NC+�S�p��
000140*     03  ER-N-ASN       PIC  N(1) VALUE NC'  '.     *> err NC+���p��, ���{��ɂł��Ȃ�����������
000150*
000160     03  X-NSP                    PIC  X(6) VALUE '�P�@�`'.
000170     03  N-NSP  REDEFINES  X-NSP  PIC  N(3).
000180     03  X-ASP                    PIC  X(6) VALUE '�P  �`'.
000190     03  N-ASP  REDEFINES  X-ASP  PIC  N(3).
000200/
000210 PROCEDURE              DIVISION .
000220*
000230 MAIN-SEC               SECTION.
000240     DISPLAY 'NSP' NSP.
000250     DISPLAY 'ASP' ASP.
000260*
000270     IF NSP = ALL NC'�@'  THEN
000280       DISPLAY 'ANY:NSP is ALL NC-SP'
000290     ELSE
000300       DISPLAY 'NSP not ALL NC-SP'
000310     END-IF.
000320* COMPILE-ERROR
000330*     IF NSP = '�@'  THEN
000340*       DISPLAY 'NSP is NSP'
000350*     ELSE
000360*       DISPLAY 'NSP not NSP'
000370*     END-IF.
000380
000390     IF NSP = NC'�@'  THEN
000400       DISPLAY 'ANY:NSP is NC-SP'
000410     ELSE
000420       DISPLAY 'NSP not NC-SP'
000430     END-IF.
000440
000450     IF NSP = SPACE THEN
000460       DISPLAY 'ANY:NSP is SPACE'
000470     ELSE
000480       DISPLAY 'NSP not SPACE'
000490     END-IF.
000500
000510     IF ASP = ALL ' ' THEN
000520       DISPLAY 'ANY:ASP is ALL ASP'
000530     ELSE
000540       DISPLAY 'ASP not ALL ASP'
000550     END-IF.
000560
000570     IF ASP = ' ' THEN
000580       DISPLAY 'ANY:ASP is ASP'
000590     ELSE
000600       DISPLAY 'ASP not ASP'
000610     END-IF.
000620
000630     IF ASP = SPACE THEN
000640       DISPLAY 'ANY:ASP is SPACE'
000650     ELSE
000660       DISPLAY 'ASP not SPACE'
000670     END-IF.
000680/
000690     DISPLAY 'N-NSP' N-NSP.
000700     DISPLAY 'N-ASP' N-ASP.
000710     DISPLAY 'X-NSP' X-NSP.
000720     DISPLAY 'X-ASP' X-ASP.
000730*
000740     IF N-NSP = N-ASP THEN
000750       DISPLAY 'ASP:N-NSP eq N-ASP'
000760     ELSE
000770       DISPLAY 'NSP:N-NSP not N-ASP'
000780     END-IF.
000790
000800     IF X-NSP = X-ASP THEN
000810       DISPLAY 'X-NSP eq X-ASP'
000820     ELSE
000830       DISPLAY 'NSP,ASP:X-NSP not X-ASP'
000840     END-IF.
000850
000860/
000870*     MOVE  X'2020'  TO  NSP.               *> compile-error
000880     MOVE  SPACE    TO  NSP.                *> compile-ok
000890*     MOVE  '  '     TO  NSP.               *> compile-error ASP,ASP
000900*     MOVE  '��'     TO  NSP.               *> compile-error 'xx'
000910     MOVE NC'��'    TO  NSP.
000920
000921     MOVE  N-ASP  TO  N-NSP.
000922     DISPLAY  'N-NSP' N-NSP
000923
000930     CONTINUE.
000940 MAIN-SEC-E.     
000950     EXIT PROGRAM.