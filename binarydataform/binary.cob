       IDENTIFICATION   DIVISION.
       PROGRAM-ID.      'BINARY'.

       ENVIRONMENT      DIVISION.
       INPUT-OUTPUT     SECTION.
       FILE-CONTROL.
      * SELECT  INFILE  ASSIGN TO       INFILE
      *   ORGANIZATION  IS              SEQUENTIAL.
        SELECT  F-OTFILE  ASSIGN TO       OTFILE
          ORGANIZATION  IS  LINE        SEQUENTIAL.

       DATA             DIVISION.
       FILE             SECTION.
      * FD      INFILE.
      *         RECORD  IS VARYING IN SIZE
      *         DEPENDING ON    IN-REC-LL.
      * 01      IN-REC.
      *     03  FILLER          OCCURS 32700 PIC X(01).
      * 01      IN-REC.
      *     03  FILLER          PIC     X(16).

       FD       F-OTFILE 
           RECORD  IS VARYING IN SIZE
           DEPENDING ON    OT-REC-LL.
       01    OT-REC.
             03  FILLER          OCCURS 4096  PIC X(01).
      * 01      OT-REC.
      *     03  FILLER          PIC     X(16).

       WORKING-STORAGE  SECTION.
       01    IN-REC-LL       PIC     9(04)   COMP.
       01    OT-REC-LL       PIC     9(04)   COMP.

      * 16Bytes/10
       01  WK-COMP.
         03  COMP04          PIC     S9(04)  COMP.
         03  FILLER          PIC     X(02)   VALUE X'FFFF'.
         03  COMP09          PIC     S9(09)  COMP.
         03  COMP18          PIC     S9(18)  COMP.
      * 24Bytes/18
       01  WK-BINARY.
         03  BINARY04        PIC     S9(04)  BINARY.
         03  FILLER          PIC     X(02)   VALUE X'FFFF'.
         03  BINARY09        PIC     S9(09)  BINARY.
         03  BINARY18        PIC     S9(18)  BINARY.
         03  FILLER          PIC     X(08)   VALUE ALL X'11'.
      * 32Bytes/20
       01  WK-COMP5.
         03  COMP504         PIC     S9(04)  COMP-5.
         03  FILLER          PIC     X(02)   VALUE X'FFFF'.
         03  COMP509         PIC     S9(09)  COMP-5.
         03  COMP518         PIC     S9(18)  COMP-5.
         03  FILLER          PIC     X(16)   VALUE ALL X'22'.
       01  WK-COMP3.
         03  COMP301         PIC     S9(01)  COMP-3.
         03  COMP305         PIC     S9(05)  COMP-3.

       PROCEDURE        DIVISION.
       MAIN             SECTION.

       DISPLAY "HELLO WORLD!!".
      * 1234
       MOVE     4660  TO  COMP04
                          BINARY04
                          COMP504.
      * 01234567
       MOVE     19088743  TO  COMP09
                              BINARY09
                              COMP509.
      * 00 00 00 01 23 45 67 80 
       MOVE 4886718336  TO  COMP18
                            BINARY18
                            COMP518.
      * PACK
       MOVE  1      TO  COMP301.
       MOVE  12345  TO  COMP305.
       MOVE  LOW-VALUE  TO  COMP301.

       OPEN  OUTPUT F-OTFILE.

       MOVE  8    TO OT-REC-LL.
       WRITE OT-REC  FROM WK-COMP.
       MOVE  24   TO OT-REC-LL.
       WRITE OT-REC  FROM WK-BINARY.
       MOVE  3232 TO OT-REC-LL.
       WRITE OT-REC  FROM WK-COMP5.
       MOVE  8    TO  OT-REC-LL
       WRITE OT-REC  FROM WK-COMP3.
       

       CLOSE F-OTFILE.
       EXIT-PROGRAM.
