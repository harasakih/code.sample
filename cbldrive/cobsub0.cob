000010 IDENTIFICATION         DIVISION.
000020 PROGRAM-ID.            COBSUB0.
000030*
000040 ENVIRONMENT            DIVISION.
000050
000060 DATA                   DIVISION.
000070 WORKING-STORAGE        SECTION.
000080*
000090 01  WORK-AREA-A.
000100     03  MSG                      PIC  X(10).
000110 01  COB2C-AREA.
           03  PRM1                     PIC  S9(08) COMP-5.
           03  PRM2                     PIC  X(10).
      *
000120 LINKAGE                SECTION.
000130 01  INPARM.
000140     03  EXEC-L                   PIC  S9(04)  COMP-5.
000150     03  EXEC-PARM                PIC  X(10).
000160
000170 PROCEDURE              DIVISION
000180                        WITH      C LINKAGE
000190                        USING     INPARM.
000200/
000210*
000220*
000230 MAIN-SEC               SECTION.
000240     MOVE               0        TO        PROGRAM-STATUS.
000250
000260     DISPLAY 'THIS IS COBSUB0 WITH PARM ' EXEC-L.
000270     DISPLAY '                VALUE  IS ' EXEC-PARM.
000280
           MOVE    5         TO        PRM1
           MOVE    'ABCDE'   TO        PRM2
000281     CALL    'CSUB0'
000282             WITH      C LINKAGE
000283             USING     COB2C-AREA.
           DISPLAY 'CSUB0 ' PRM1.
           DISPLAY 'CSUB0 ' PRM2.
000284
000290     CONTINUE.
000300     GOBACK.
000310 MAIN-SEC-E.
000320     EXIT PROGRAM.
