set QTDIR=C:\Qt\5.7\msvc2015
set PATH=%PATH%;%QTDIR%\bin
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
qmake
nmake
