#
# $(Id) : $
#	マクロ定義
#		$@	ターゲット（サフィックスあり）
#		$*	ターゲット（サフィックスなし）
#		$<	依存ファイル
#
# 新たに追加するサフィックス(.c .o .h .a)以外
ifeq (Windows_NT,$(filter Windows_NT, $(OS)))
  MY_OS = Windows
  SUF_C   = .c
  SUF_OBJ = .obj
  SUF_EXE = .exe
else
  MY_OS = linux
  SUF_C   = .c
  SUF_OBJ = .o
  SUF_EXE =
endif

.SUFFIXES: $(SUF_OBJ) $(SUF_EXE)

#	コマンドの定義
CC		= gcc
RM		= rm
ECHO	= "/bin/echo -n"

#	コンパイルオプション
C_FLGS		= -Wall		; gcc warn-all
# C_FLGS	= /Wall		; vc++ warn-all

ifeq (Windows,$(MY_OS))
  C_FLGS_D	= --input-charset=CP932 --exec-charset=CP932 -mno-cygwin -ggdb
  C_FLGS_R	= --input-charset=CP932 --exec-charset=CP932 -mno-cygwin
  LD_FLGS	=
else
#  C_FLGS_D	= --input-charset=CP932 --exec-charset=CP932 -ggdb
  C_FLGS_D	= -ggdb
#  C_FLGS_R	= --input-charset=CP932 --exec-charset=CP932 
  C_FLGS_R	= 
endif


#	デフォルト生成規則
#
$(SUF_C)$(SUF_OBJ):
	$(ECHO) ### implicit rules .c -> .o ###
	$(CC) -c $(C_FLGS_D) $<

$(SUF_OBJ)$(SUF_EXE):
	$(ECHO) ### implicit rules .o -> .exe ###
	$(CC) $(LD_FLGS) -o $@ $< $(LIBS)

$(SUF_C)$(SUF_EXE):
	$(CC) $(C_FLGS_D) $(LD_FLGS) -o $@ $< $(LIBS)

#	このファイルのターゲット＆ソース
ALL_EXE	= btdtcnv$(SUF_EXE)
ALL_OBJ = btdtcnv$(SUF_OBJ)

dummy: 
	$(ECHO) ###########################################################
	$(ECHO) ##                                                       ##
	$(ECHO) ##     makefile for gcc $(MY_OS) version                 ##
	$(ECHO) ##                                                       ##
	$(ECHO) ###########################################################

all: $(ALL_EXE)

clean: 
	$(RM) $(ALL_EXE)

rebuild:  clean all

