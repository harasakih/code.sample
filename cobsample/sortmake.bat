@echo off
rem
rem Make sort test data.
rem

echo temp is %TEMP%

set vfile=sortdata05-10000.dat
exe\sort001.exe  "0005,00010000,0020,0002"

set vfile=sortdata06-10000.dat
exe\sort001.exe  "0006,00010000,0020,0002"

set vfile=sortdata07-10000.dat
exe\sort001.exe  "0007,00010000,0020,0002"

set vfile=sortdata08-10000.dat
exe\sort001.exe  "0008,00010000,0020,0002"
goto FIN

set vfile=sortdata40-1000.dat
exe\sort001.exe  "0040,00001000,0020,0002"

set vfile=sortdata45-1000.dat
exe\sort001.exe  "0045,00001000,0020,0002"

set vfile=sortdata20-1000.dat
exe\sort001.exe  "0020,00001000,0020,0002"

set vfile=sortdata30-1000.dat
exe\sort001.exe  "0030,00001000,0020,0002"

set vfile=sortdata10-1000.dat
exe\sort001.exe  "0010,00001000,0020,0002"

set vfile=sortdata01-1000.dat
exe\sort001.exe  "0001,00001000,0020,0002"

set vfile=sortdata05-1000.dat
exe\sort001.exe  "0005,00001000,0020,0002"

set vfile=sortdata15-1000.dat
exe\sort001.exe  "0015,00001000,0020,0002"

set vfile=sortdata25-1000.dat
exe\sort001.exe  "0025,00001000,0020,0002"

:FIN
pause


rem 192M ok
rem exe\sort001.exe  "0004,00010000,0020,0002"
rem 239M og
rem exe\sort001.exe  "0005,00010000,0020,0002"
rem 491M ng
rem exe\sort001.exe  "0010,00010000,0020,0002"
rem exe\sort001.exe  "0005,00000100,0020,0002"
