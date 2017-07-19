#
#	マクロ定義
#		$@	ターゲット（サフィックスあり）
#		$*	ターゲット（サフィックスなし）
#		$<	最初の依存ファイルA
#		$?	ターベッとより新しい、全ての依存ファイル
#		$^	全ての依存ファイル
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

.SUFFIXES: $(SUF_OBJ) $(SUF_EXE) $(SUF_C)

# デバック有無
IS_DBG = DEBUG

#	コマンドの定義
CC		= gcc
RM		= rm
#ECHO	= echo

#	コンパイルオプション (デバック）
ifeq (DEBUG,$(IS_DBG))
  C_FLGS_D	= -Wall -ggdb
  # link math lib
  LD_FLGS_D = -lm
else
  C_FLGS_D	= -Wall 
  # link math lib
  LD_FLGS_D = -lm
endif

#	コンパイルオプション (ＯＳ依存）
ifeq (Windows,$(MY_OS))
  C_FLGS_OS	= --input-charset=CP932 --exec-charset=CP932 -mno-cygwin
else
  C_FLGS_OS	= 
endif


#	デフォルト生成規則
#
$(SUF_C)$(SUF_OBJ):
	@echo "### implicit rules .c -> .o ###"
	@echo "Target(with suf)    is " $@
	@echo "Target(without suf) is " $*
	@echo "Depend is " $< 
	$(CC) -c $(C_FLGS_D) $(C_FLGS_OS) $<

$(SUF_OBJ)$(SUF_EXE):
	@echo "### implicit rules .o -> .exe ###"
	$(CC) $(LD_FLGS_D) -o $@ $< $(LIBS)

$(SUF_C)$(SUF_EXE):
	@echo "### implicit rules .c -> .exe ###"
	$(CC) $(C_FLGS_D) $(C_FLGS_OS) $(LD_FLGS_D) -o $@ $< $(LIBS)

#	このファイルのターゲット＆ソース
ALL_EXE	= 
ALL_OBJ = 

dummy: 
	@echo "###########################################################"
	@echo "##                                                       ##"
	@echo "## makefile for gcc $(MY_OS) version"
	@echo "##   IS_DBG             : $(IS_DBG)"
	@echo "##   C_FLGS_D  for debug: $(C_FLGS_D)"
	@echo "##   C_FLGS_OS for os   : $(C_FLGS_OS)"
	@echo "##   LD_FLGS_D          : $(LD_FLGS_D)"
	@echo "##                                                       ##"
	@echo "###########################################################"

all: $(ALL_EXE)

clean: 
	$(RM) $(ALL_EXE)

rebuild:  clean all

linktest_static: linktest_main.c linktest_sub.c
	gcc -o $@ $^

liblinktest_sub.so: linktest_sub.c
	gcc -shared -o $@ $<

liblinktest_sub2.so: linktest_sub2.c
	gcc -shared -o $@ $<

linktest_shared: linktest_main.c 
	gcc -L./ -o $@ $< -llinktest_sub2

linktest:
	echo "### static ###"
	./linktest_static
	echo "### shared ###"
	./linktest_shared
