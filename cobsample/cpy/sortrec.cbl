000010* VFILE.CBL
000020*
000030     07  DUMMYREC.
000040       08  KEYAREA.                           *> 1-36
000050       09    RECNO                        PIC  9(08).
000060       09    DELIM0                       PIC  X(1).
000070       09    KEY1                         PIC  9(08).
000080       09    DELIM1                       PIC  X(01).
000090       09    KEY2                         PIC  9(08).
000100       09    DELIM2                       PIC  X(01).
000110       09    KEY3                         PIC  9(08).
000120       09    DELIM3                       PIC  X(01).
000130       08  DATAAREA.                      *> 37-1024
000140       09    BODY                         PIC  X(988).
000150*
000160     07  REC1           REDEFINES       DUMMYREC.
000170       08  KEYAREA.                           *> 1-36
000180       09    FILLER                       PIC  9(08).
000190       09    FILLER                       PIC  X(1).
000200       09    FILLER                       PIC  9(08).
000210       09    FILLER                       PIC  X(01).
000220       09    FILLER                       PIC  9(08).
000230       09    FILLER                       PIC  X(01).
000240       09    FILLER                       PIC  9(08).
000250       09    FILLER                       PIC  X(01).
000260       08  DATAAREA.                      *> 37-1024
000270       09    BODY1                        PIC  X(988).
000280*
000290     07  REC2           REDEFINES       DUMMYREC.
000300       08  KEYAREA.                           *> 1-36
000310       09    FILLER                       PIC  9(08).
000320       09    FILLER                       PIC  X(1).
000330       09    FILLER                       PIC  9(08).
000340       09    FILLER                       PIC  X(01).
000350       09    FILLER                       PIC  9(08).
000360       09    FILLER                       PIC  X(01).
000370       09    FILLER                       PIC  9(08).
000380       09    FILLER                       PIC  X(01).
000390       08  DATAAREA.                      *> 37-160
000400       09    BODY2                        PIC  X(124).
000410
000420*
000430     07  REC3           REDEFINES       DUMMYREC.
000440       08  KEYAREA.                           *> 1-36
000450       09    FILLER                       PIC  9(08).
000460       09    FILLER                       PIC  X(1).
000470       09    FILLER                       PIC  9(08).
000480       09    FILLER                       PIC  X(01).
000490       09    FILLER                       PIC  9(08).
000500       09    FILLER                       PIC  X(01).
000510       09    FILLER                       PIC  9(08).
000520       09    FILLER                       PIC  X(01).
000530       08  DATAAREA.                      *> 37-256
000540       09    BODY3                        PIC  X(220).
