#
#	マクロ定義(NMAKE)
#		$@	ターゲット（サフィックスあり）
#		$*	ターゲット（サフィックスなし）
#		$**	全ての依存ファイル
#		$?	新しい、全ての依存ファイル
#		$<	新しい依存ファイル（デフォルト規則ないのみ）
#
#usage:
#  nmake all "DBG=ON" "OPT=,NUMCHECK" TARGET=cobmain
#    "OPT=..." additional COBOL-COMPILE options
#    for example "OPT=,CHECK(NUMERIC)"
#
# 2015/04
#  COPY句ディレクトリの指定が無効となっていたので、修正.
#  cygwinでも動くように、コマンドを絶対パスで
#
# サフィックスを削除して、追加
# デフォルト(.c .o .h .a)定義を念のため、削除
.SUFFIXES:
.SUFFIXES: .obj .exe .cob .cbl .lib .dll .c

#	コマンドの定義
RM		= del
MV		= move
ECHO	= @echo
ECHO_OFF = echo off
CC		= "C:\Program Files (x86)\NetCOBOL\VC\Tools\MSVC\14.16.27023\bin\Hostx86\x86\cl"
COB		= "C:\Program Files (x86)\NetCOBOL\cobol32"
LINK	= "C:\Program Files (x86)\NetCOBOL\VC\Tools\MSVC\14.16.27023\bin\Hostx86\x86\link"

############################################################
#	Directories
############################################################
BASEDIR = .
HOME = $(BASEDIR)
CPY  = $(HOME)\cpy
SOC  = $(HOME)\soc
OBJ  = $(HOME)\obj
LST  = $(HOME)\lst
DLL  = $(HOME)\dll
EXE  = $(HOME)\exe

############################################################
#	コンパイルオプション
############################################################
## for C-lang
# C_FLGS	= -wall
C_FLGS		= /TC /c 
C_INC_DIR	=
C_LIB_DIR	=
C_LIB		=

## for COBOL
COBOPT=XREF,COPY$(OPT)
#
!IFDEF  DBG
!MESSAGE COBOL with TEST-MODE
COB_DLL_FLGS= -P -dp$(LST) -do$(OBJ) -WC,"SOURCE,MESSAGE,NODLOAD,TEST,$(COBOPT)"
COB_EXE_FLGS= -P -dp$(LST) -do$(OBJ) -WC,"SOURCE,MESSAGE,NODLOAD,TEST,MAIN(MAIN),$(COBOPT)"
!ELSE
!MESSAGE COBOL noTEST-MODE
COB_DLL_FLGS= -P -dp$(LST) -do$(OBJ) -WC,"SOURCE,MESSAGE,NODLOAD,$(COBOPT)"
COB_EXE_FLGS= -P -dp$(LST) -do$(OBJ) -WC,"SOURCE,MESSAGE,NODLOAD,MAIN(MAIN),$(COBOPT)"
!ENDIF

COB_COPY_DIR= -I$(CPY) 

############################################################
#	リンクオプション
############################################################
# COBOL-LIB-DIR
COB_LIB_DIR	= /LIBPATH:"C:\Program Files (x86)\NetCOBOL\VC\Tools\MSVC\14.16.27023\lib\x86" \
  /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\ucrt\x86" \
  /LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x86"

# VC-LIB-DIR
# VC_LIB_DIR	= /LIBPATH:"C:\Program Files (x86)\NetCOBOL\VC\Tools\MSVC\14.16.27023\lib\x86"
VC_LIB_DIR	= 
LIB_DIR		= $(COB_LIB_DIR) $(VC_LIB_DIR)

# COBOL-LIBRARY
COB_LIB_DLL	= "C:\Program Files (x86)\NetCOBOL\F3BICIMP.LIB" \
  "C:\Program Files (x86)\NetCOBOL\VC\Tools\MSVC\14.16.27023\lib\x86\MSVCRT.LIB" \
  "C:\Program Files (x86)\Windows Kits\10\lib\10.0.19041.0\um\x86\KERNEL32.LIB"
COB_LIB_EXE	= $(COB_LIB_DLL)
LINK_DLL_FLGS=
LINK_EXE_FLGS=

# LINKER DEBUG MODE
!IFDEF DBG
!MESSAGE LINK with DBG-MODE
LINK_DBG_FLGS= /DEBUG /DEBUGTYPE:COFF
!ELSE
!MESSAGE LINK noDBG-MODE
LINK_DBG_FLGS=
!ENDIF

############################################################
#	ターゲットと依存関係
############################################################
!IFDEF TARGET
TARGET = $(TARGET)
!ELSE
TARGET = Helloworld
!ENDIF


dummy: 
	$(ECHO) ###########################################################
	$(ECHO) ##  usage: nmake all TARGET=xxx
	$(ECHO) ##
	$(ECHO) ##  VisualC++ NetCOBOL COMPILE make for NMAKE
	$(ECHO) ##    BASEDIR =$(BASEDIR)
	$(ECHO) ##    TARGET  =$(TARGET)
	$(ECHO) ##    CPY     =$(CPY)
	$(ECHO) ##    SOC     =$(SOC)
	$(ECHO) ##    OBJ     =$(OBJ)
	$(ECHO) ##    LST     =$(LST)
	$(ECHO) ##    DLL     =$(DLL)
	$(ECHO) ##    EXE     =$(EXE)
	$(ECHO) ##    
	$(ECHO) ###########################################################

all : $(EXE)\$(TARGET).exe

clean: 
	$(RM) $(OBJ)\$(TARGET).obj
#
	$(RM) $(DLL)\$(TARGET).dll
	$(RM) $(DLL)\$(TARGET).lib
	$(RM) $(DLL)\$(TARGET).exp
#
	$(RM) $(EXE)\$(TARGET).exe
	$(RM) $(EXE)\$(TARGET).lib
	$(RM) $(EXE)\$(TARGET).exp
#
	$(RM) $(LST)\$(TARGET).lst
	$(RM) $(SOC)\$(TARGET).svd
	

$(EXE)\$(TARGET).exe : $(SOC)\$(TARGET).cob
	$(COB) $(COB_EXE_FLGS) $(COB_COPY_DIR) -NM $(SOC)\$(TARGET).cob 
	$(LINK) $(LINK_EXE_FLGS) $(LINK_DBG_FLGS) \
	$(COB_LIB_EXE) $(COB_LIB_DIR) $(VC_LIB_DIR) \
	/OUT:$(EXE)\$(TARGET).exe $(OBJ)\$(TARGET).obj

$(DLL)\$(TARGET).dll : $(SOC)\$(TARGET).cob
	$(COB) $(COB_DLL_FLGS) $(COB_COPY_DIR) -NM $(SOC)\$(TARGET).cob 
	$(LINK) $(LINK_DLL_FLGS) $(LINK_DBG_FLGS) \
	$(COB_LIB_DLL) $(COB_LIB_DIR) $(VC_LIB_DIR) \
#	/ENTRY:COBDMAIN /DLL \
	/DLL \
	/OUT:$(DLL)\$(TARGET).dll $(OBJ)\$(TARGET).obj

