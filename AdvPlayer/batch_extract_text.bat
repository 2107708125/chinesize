@echo off

set "ArcName=d:\galgame\������ݳ֤��Τ����ͤ��� �������ͤ��뤤�򤺤äȡ�\Rio.arc"
set "ExeName=d:\galgame\������ݳ֤��Τ����ͤ��� �������ͤ��뤤�򤺤äȡ�\AdvHD.exe"

rd /q /s Rio
rd /q /s RioDec

extArc\extArc.exe "%ArcName%" Rio
decWs2\decWs2.exe Rio RioDec

rd /q /s txt
rd /q /s idx
mkdir txt
mkdir idx
for %%i in (RioDec\*.ws2) do extWs2\extWs2.exe -e "%ExeName%" -i %%i -o "txt\%%~ni.txt" -only-txt -name-prefix "%%LF" -index-file "idx\%%~ni.idx"

pause