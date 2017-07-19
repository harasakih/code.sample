#
#	マクロ定義
#		$@	ターゲット（サフィックスあり）
#		$*	ターゲット（サフィックスなし）
#		$<	依存ファイル
#
# 新たに追加するサフィックス(.c .o .h .a)以外
.SUFFIXES: .obj .exe .cob .cbl

#	コマンドの定義
CC		= cl
RM		= rm
ECHO	= echo
COB		= cobol32

#	コンパイルオプション
# C_FLGS	= -wall
INC_DIR =
LIB_DIR =
C_FLGS	= /TC 
COB_FLGS=
LD_FLGS	=
COB_LIB	= F3BICIMP.LIB LIBC.LIB LIBCMT.LIB
C_LIB	=
LIBS	= $(COB_LIB) $(C_LIB)

#
#	デフォルト生成規則
#
.c.obj:
	$(CC) /c $(C_FLGS) $<

.cob.obj:
	$(COB) $(COB_FLGS) $<

.obj.exe:
	$(CC) $(LD_FLGS) /o $@ $< $(LIBS)

.c.exe:
	$(CC) $(LD_FLGS) /o $@ $< $(LIBS)

#	このファイルのターゲット＆ソース
ALL_EXE	=	cmain0.exe
ALL_OBJ	=	cmain0.obj cobsub0.obj csub0.obj

ALL2_EXE	=	cmain2.exe
ALL2_OBJ	=	cmain2.obj cobsub.obj cobsub1.obj cobsub2.obj

dummy: 
	$(ECHO) ###########################################################
	$(ECHO) ##                                                       ##
	$(ECHO) ##  VisualC++ NetCOBOL COMPILE make                      ##
	$(ECHO) ##                                                       ##
	$(ECHO) ###########################################################

all:	$(ALL_EXE) $(ALL2_EXE)

$(ALL_EXE): $(ALL_OBJ)
	$(CC) $(LD_FLGS) /o $@ $(ALL_OBJ) $(LIBS)

$(ALL2_EXE): $(ALL2_OBJ)
	$(CC) $(LD_FLGS) /o $@ $(ALL2_OBJ) $(LIBS)


clean: 
	$(RM) $(ALL_OBJ)
	$(RM) $(ALL_EXE)

test:
	$(ALL_EXE) ABCDEFG
