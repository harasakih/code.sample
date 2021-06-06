REM

@echo off
set NETCOBOL="C:\Program Files (x86)\NetCOBOL"
set VCBASE="C:\Program Files (x86)\NetCOBOL\VC\Tools\MSVC\14.16.27023\bin\Hostx86\x86"

set COBCC=%NETCOBOL%\cobol32
set COBLINK=%VCBASE%\link

set F3BICIMP="C:\Program Files (x86)\NetCOBOL\F3BICIMP.LIB"
set   MSVCRT="C:\Program Files (x86)\NetCOBOL\VC\Tools\MSVC\14.16.27023\lib\x86\MSVCRT.LIB"
set KERNEL32="C:\Program Files (x86)\Windows Kits\10\lib\10.0.19041.0\um\x86\KERNEL32.LIB"

set LIBPATH1="C:\Program Files (x86)\NetCOBOL\VC\Tools\MSVC\14.16.27023\lib\x86"
set LIBPATH2="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\ucrt\x86"
set LIBPATH3="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x86"

set LST=.
set OBJ=.
set TMPCOB=tmp.cob

echo 000001 @OPTIONS SOURCE,MESSAGE,COPY,MAP,NOOBJECT,NOLIST > %TMPCOB%
echo 000010 IDENTIFICATION   DIVISION.      >> %TMPCOB%
echo 000020 PROGRAM-ID.      COBMAP.        >> %TMPCOB%
echo 000030 ENVIRONMENT      DIVISION.      >> %TMPCOB%
echo 000040 DATA             DIVISION.      >> %TMPCOB%
echo 000050 WORKING-STORAGE  SECTION.       >> %TMPCOB%
echo 000051 01  _COPYMAP_.                  >> %TMPCOB%
echo 000060     COPY  %1 .                  >> %TMPCOB%
echo 000070 PROCEDURE        DIVISION.      >> %TMPCOB%
echo 000080     CONTINUE                   >> %TMPCOB%

%COBCC% -P -dp%LST% -do%OBJ% %TMPCOB%
move tmp.lst %1.copy.lst
del  %TMPCOB%
