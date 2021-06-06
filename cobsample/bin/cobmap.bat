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
set CPY=..\CPY
set COBOPT=SOURCE,MESSAGE,COPY,MAP,NOOBJECT

%COBCC% -P -I%CPY% -dp%LST% -do%OBJ% -WC,"%COBOPT%" %1
