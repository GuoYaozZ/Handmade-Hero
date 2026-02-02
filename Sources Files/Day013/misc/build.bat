@echo off
call "F:\VisualStudio2022\VC\Auxiliary\Build\vcvarsall.bat" x64
set path="F:\MyVSProject\HandMadeHero\misc";%path%

IF NOT EXIST ..\..\bat_container\build mkdir ..\..\bat_container\build
pushd ..\..\bat_container\build
cl -DHANDMADE_WIN32=1 -Zi ..\..\HandMadeHero\win32_handmade.cpp user32.lib gdi32.lib
popd 
