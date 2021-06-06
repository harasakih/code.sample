000010 IDENTIFICATION   DIVISION.
000020 PROGRAM-ID.      FILEIO.
000030
000040 ENVIRONMENT      DIVISION.
000050 INPUT-OUTPUT     SECTION.
000060 FILE-CONTROL.
000070
000080 DATA             DIVISION.
000090 FILE             SECTION.
000100 WORKING-STORAGE  SECTION.
000110
000120 01     WORK-A.
000130   03   PACK1           PIC     9(05)   COMP-3.
000140   03   PACK2           PIC     9(05)   COMP-3.
000150   03   PACK3           PIC     9(05)   COMP-3.
000160
000170 PROCEDURE        DIVISION.
000180 MAIN             SECTION.
000190     MOVE  99999 TO  PACK1.
000200     ADD   1     TO  PACK1.
000210     DISPLAY '99999 + 1=' PACK1.
000211     MOVE  33333 TO  PACK2.
000212     MOVE  0     TO  PACK1.
000220     COMPUTE  PACK3 = PACK2 / PACK1.
000230     DISPLAY '3333 / 0=' PACK3
000240*
000250     EXIT PROGRAM.
000260 MAIN-EX.
000270/
000280 EXIT-PROGRAM.