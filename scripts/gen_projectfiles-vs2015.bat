@Echo off

SET rootPath=%~dp0
SET rootPath=%rootPath%..\
SET testDir=%rootPath%test

REM Generate for library
cd %rootPath%
qmake.exe -spec win32-msvc2015 -tp vc QMetaOrm.pro

REM Generate for tests
cd %testDir%
for /D %%i in (*) do cd "%%i" & qmake.exe -spec win32-msvc2015 -tp vc & cd ..

cd %rootPath%