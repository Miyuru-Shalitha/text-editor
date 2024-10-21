@echo off

if not exist ..\build mkdir ..\build

pushd ..\build
cl /Zi ..\code\win32_platform.c /Fe:editor.exe
popd