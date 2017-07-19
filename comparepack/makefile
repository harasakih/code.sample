#
#	マクロ定義
#		$@	ターゲット（サフィックスあり）
#		$*	ターゲット（サフィックスなし）
#		$<	依存ファイル
#
# 新たに追加するサフィックス(.c .o .h .a)以外
.SUFFIXES: .obj .exe .cob .CBL

#	コマンドの定義
CC		= cl
RM		= rm
ECHO	= echo
COB		= cobol32
LINK	= link

#	コンパイルオプション
# C_FLGS	= -wall
INC_DIR =
LIB_DIR =
C_FLGS	= /TC 
COB_OPT = SOURCE,MAP,MESSAGE
COB_FLGS= -P -WC,"$(COB_OPT)"
LD_FLGS	=
COB_LIB	= F3BICIMP.LIB LIBC.LIB
C_LIB	=
LIBS	= $(COB_LIB) $(C_LIB)

#
#	デフォルト生成規則
#
.c.obj:
	$(CC) /c $(C_FLGS) $<

.cob.obj:
	$(COB) $(COB_FLGS) $<

#.cob.exe:
#	$(ECHO) ### implicit rules ####
#	$(COB)  -M -P -WC"$(COB_OPT)" $*.cob
#	$(LINK) /OUT:$@ $(LD_FLGS) $*.obj $(COB_LIB)

#.obj.exe:
#	$(CC) $(LD_FLGS) /o $@ $< $(LIBS)

# .c.exe:
# 	$(CC) $(LD_FLGS) /o $@ $< $(LIBS)

#	このファイルのターゲット＆ソース
ALL_EXE = comporpack.exe

dummy: 
	$(ECHO) ###########################################################
	$(ECHO) ##                                                       ##
	$(ECHO) ##  NetCOBOL COMPILE make                                ##
	$(ECHO) ##                                                       ##
	$(ECHO) ###########################################################

comporpack.exe: cmain0.obj comporpack.obj
	$(LINK) /OUT:$@ $(LD_FLGS) $^ $(COB_LIB)

cmain0.obj: cmain0.c
	$(CC) /c $(C_FLGS) $<

all: $(ALL_EXE)


clean: 
	$(RM) *.exe
	$(RM) *.obj
	$(RM) *.EXP
	$(RM) *.LIB


