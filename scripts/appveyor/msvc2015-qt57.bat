set QTDIR=C:\Qt\5.7\msvc2015
set PATH=%PATH%;%QTDIR%\bin
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
mkdir build
cd build
cmake -G "Visual Studio 14 Win64" -DCMAKE_PREFIX_PATH=$QTDIR\lib\cmake\ ..
ls -l
msbuild
ctest -V
