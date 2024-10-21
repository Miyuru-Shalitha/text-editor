@echo off

if not exist ..\build mkdir ..\build

pushd ..\build
cl /Zi ..\code\win32_platform.c /Fe:text-editor.exe /link user32.lib
popd
