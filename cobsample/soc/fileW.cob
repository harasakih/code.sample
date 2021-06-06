000010 IDENTIFICATION   DIVISION.
000020 PROGRAM-ID.      FILEW.
000030
000040 ENVIRONMENT      DIVISION.
000050 INPUT-OUTPUT     SECTION.
000060 FILE-CONTROL.
000070     SELECT  V-OTFILE  ASSIGN TO     VFILE
000080             ORGANIZATION  IS    SEQUENTIAL.
000090     SELECT  F-OTFILE  ASSIGN TO     FFILE
000100             ORGANIZATION  IS    SEQUENTIAL.
000110
000120 DATA             DIVISION.
000130 FILE             SECTION.
000140 FD      V-OTFILE
000150         RECORD    IS  VARYING IN SIZE
000160         DEPENDING ON  V-LL.
000170 01      V-REC01.
000171     COPY  VFILE.
000220
000230 FD      F-OTFILE.
000240 01      F-REC01.
000250     COPY  FFILE.
000280
000290 WORKING-STORAGE  SECTION.
000300
000310 01     WORK-A.
000320   03   V-LL            PIC     9(04)   COMP.
000330   03   II              PIC     9(04)   COMP.
000340   03   N-RECORD        PIC     9(04)   COMP  VALUE  10.
000350   03   ISEOF           PIC     9(04)   COMP.
000360
000370 01     WORK-B.
000380   03   FILLER   PIC  X(36)  VALUE  '0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ'.
000390   03   FILLER   PIC  X(26)  VALUE  'abcdefghijklmnopqrstuvwxyz'.
000400   03   FILLER   PIC  X(4096) VALUE SPACE.
000410
000420 PROCEDURE        DIVISION.
000430 MAIN             SECTION.
000440     PERFORM  V-SEC-W.
000450     PERFORM  F-SEC-W.
000460     EXIT PROGRAM.
000470 MAIN-EX.
000480/
000490 V-SEC-W  SECTION.
000500
000510     OPEN  OUTPUT  V-OTFILE.
000520
000530     PERFORM TEST BEFORE VARYING  II
000540       FROM  1  BY  1  UNTIL II  >  N-RECORD
000550       MOVE  II         TO    V-LL
000560       MOVE  II         TO    NUM1  OF  V-REC01
000570       MOVE  II         TO    NUM2  OF  V-REC01
000580       MOVE  WORK-B(II:72)     TO    REC-DATA  OF  V-REC01
000590       WRITE  V-REC01
000600     END-PERFORM.
000610
000620     CLOSE  V-OTFILE.
000630
000640 V-SEC-W-EX.
000650/
000660 F-SEC-W  SECTION.
000670
000680     OPEN  OUTPUT  F-OTFILE.
000690
000700     PERFORM TEST BEFORE VARYING  II
000710       FROM  1  BY  1  UNTIL II  >  N-RECORD
000720       MOVE  II         TO    NUM1  OF  F-REC01
000730       MOVE  II         TO    NUM2  OF  F-REC01
000740       MOVE  WORK-B(II:72)     TO    REC-DATA  OF  F-REC01
000750       WRITE  F-REC01
000760     END-PERFORM.
000770
000780     CLOSE  F-OTFILE.
000790 F-SEC-W-EX.
000800
000810 EXIT-PROGRAM.
