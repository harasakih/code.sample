000180 IDENTIFICATION            DIVISION.                                      
000190 PROGRAM-ID.               TESTCOB.                                      
000200 ENVIRONMENT               DIVISION.                                      
000210 CONFIGURATION             SECTION.                                       
000220 INPUT-OUTPUT              SECTION.                                       
000230 FILE-CONTROL.                  
000240      SELECT  IN-FILE  ASSIGN   TO  INFILE                       
000250*         ORGANIZATION IS LINE SEQUENTIAL.                                
000260          ORGANIZATION IS      SEQUENTIAL.                                
000270                                                                          
000280      SELECT  OT-FILE  ASSIGN   TO  OTFILE                      
000290*         ORGANIZATION IS LINE SEQUENTIAL.                                
000300          ORGANIZATION IS      SEQUENTIAL.                                
000310                                                                          
000320/A-1-B--+----2----+----3----+----4----+----5----+----6----+----7          
000330*                                                                         
000340*---------------------------------------------------------------          
000350 DATA                      DIVISION.                                      
000360 FILE                      SECTION.                                       
000370*                                                                         
000380*  IN-FILEのレコード定義                                           
000390*                                                                         
000400 FD  IN-FILE.                                                       
000440 01  IN-RECORD           PIC X(256).                               
000450*                                                                         
000460*  出力ファイルのレコード定義                                           
000470*                                                                         
000480 FD  OT-FILE.                                                       
000500 01  OT-RECORD            PIC X(80).                                 
000510                                                                          
000520/A-1-B--+----2----+----3----+----4----+----5----+----6----+----7          
000530*                                                                         
000540*---------------------------------------------------------------          
000550 WORKING-STORAGE           SECTION.                                       
000560*                                                                         
000570* ファイルのレコード長                                                  
000580*                                                                         
000590 01  IN-REC-LENGTH         PIC 9(04) COMP.                           
       01  WORK-VALS.
         03   EOF                PIC S9(01) COMP.
000600                                                                          
001030*                                                                         
001040/A-1-B--+----2----+----3----+----4----+----5----+----6----+----7          
001050*                                                                         
001060*---------------------------------------------------------------          
001070 LINKAGE                   SECTION.                                       
001080* 01  EXEC-PARA-AREA.                                                      
001090*   03  EXEC-LENGTH               PIC S9(04) COMP.                         
001100*   03  EXEC-PARA                 PIC X(80).                               
001110/A-1-B--+----2----+----3----+----4----+----5----+----6----+----7          
001120*                                                                         
001130*---------------------------------------------------------------          
001140*                                                                         
001150 PROCEDURE   DIVISION.                          
001160 MAIN        SECTION.                                                 
001170*                                                                         
001180*                                                                         
001190     PERFORM   INIT-SEC                                                 
001200     PERFORM   MAIN-SEC  UNTIL  EOF  =  1                               
001210     PERFORM   TERM-SEC                                                 
001220*                                                                         
001230*     MOVE   ZERO   TO   RETURN-CODE                                       
001240     STOP  RUN.                                                           
001250*                                                                         
001260 MAIN-E.                                                          
001270/A-1-B--+----2----+----3----+----4----+----5----+----6----+----7          
001280*                                                                         
001290*---------------------------------------------------------------          
001300 INIT-SEC    SECTION.                                                 
001310*                                                                         
001320     OPEN  INPUT   IN-FILE                                         
001330     OPEN  OUTPUT  OT-FILE                                         
001520*                                                                         
001530     CONTINUE.                                                            
001540*                                                                         
001550 INIT-SEC-E.                                                          
001560*                                                                         
001570/A-1-B--+----2----+----3----+----4----+----5----+----6----+----7          
001580*                                                                         
001590*---------------------------------------------------------------          
001600 MAIN-SEC    SECTION.                                                   
001610*                                                                         
001890*   
           MOVE  1  TO   EOF                                                                       
001900     CONTINUE.                                                            
001910*                                                                         
001920 MAIN-SEC-E.                                                            
001930*                                                                         
004560/A-1-B--+----2----+----3----+----4----+----5----+----6----+----7          
004570*                                                                         
004580*---------------------------------------------------------------          
004590 TERM-SEC    SECTION.                                                 
004600*                                                                         
004630     CLOSE     IN-FILE.                                          
004640     CLOSE     OT-FILE.                                          
004650*                                                                         
004660 TERM-SEC-E.                                                          
004670/                                                                         
