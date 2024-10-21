@echo off

if not exist ..\build (
    mkdir ..\build 
)

pushd ..\build
del ..\build\*.pdb
del ..\build\*.ilk
del ..\build\*.obj

cl /Zi ..\code\win32_platform.c /Fe:text-editor.exe /link user32.lib gdi32.lib
popd
