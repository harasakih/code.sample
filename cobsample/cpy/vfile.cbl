      * VFILE.CBL
      *
           07  VFILE.
             09  NUM1                         PIC  9(04).
             09  REC-DATA                     PIC  X(72).
             09  NUM2                         PIC  ZZZ9.
             09  SPACK                        PIC  S9(01) COMP-3.
             09  UPACK                        PIC   9(01) COMP-3.
             09  SZONE                        PIC  S9(02).
             09  UZONE                        PIC   9(02).
             09  NCH                          PIC   N(05).
             09  SHUDAN.
               11  GROUP-1                    PIC  X(01).
             09  SHUDAN2.
               11  COMP0                      PIC  S9(04) COMP.
               11  COMP3                      PIC  S9(04) COMP-3.
               11  COMP5                      PIC  S9(04) COMP-5.
               11  BIN                        PIC  S9(04) BINARY.
             09  FILLER                       PIC  X(4096).
