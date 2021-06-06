@echo off
rem
rem SORT exec .
rem
echo temp is %TEMP%

set sortwk01=sortwk01

rem 05-10000
set vfilein=sortdata05-10000.dat
set vfileot=sortout05-10000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 06-10000
set vfilein=sortdata06-10000.dat
set vfileot=sortout06-10000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 07-10000
set vfilein=sortdata07-10000.dat
set vfileot=sortout07-10000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 08-10000
set vfilein=sortdata08-10000.dat
set vfileot=sortout08-10000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k
goto FIN


rem 40-1000
set vfilein=sortdata192M.dat
set vfileot=sortout192M.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 40-1000
set vfilein=sortdata40-1000.dat
set vfileot=sortout40-1000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 45-1000
set vfilein=sortdata45-1000.dat
set vfileot=sortout45-1000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 20-1000
set vfilein=sortdata20-1000.dat
set vfileot=sortout20-1000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 30-1000
set vfilein=sortdata30-1000.dat
set vfileot=sortout30-1000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 10-1000
set vfilein=sortdata10-1000.dat
set vfileot=sortout10-1000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 01-1000
set vfilein=sortdata01-1000.dat
set vfileot=sortout01-1000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 05-1000
set vfilein=sortdata05-1000.dat
set vfileot=sortout05-1000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 15-1000
set vfilein=sortdata15-1000.dat
set vfileot=sortout15-1000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k

rem 25-1000
set vfilein=sortdata25-1000.dat
set vfileot=sortout25-1000.dat
echo SORTIN %vfilein%
exe\sort003.exe "0" -CBL smsize10000k


:FIN
pause

rem --- comments ---
rem exe\sort002.exe -CBL smsize10000k
rem exe\sort003.exe "1" -CBL smsize10000k
