@echo off

IF NOT EXIST .\build mkdir .\build
pushd .\build
cl /nologo ..\code\persist.c /link Advapi32.lib
popd
