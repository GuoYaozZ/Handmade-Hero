@echo off
call "F:\VisualStudio2022\VC\Auxiliary\Build\vcvarsall.bat" x64
set path="F:\MyVSProject\HandMadeHero\misc";%path%

IF NOT EXIST ..\..\bat_container\build mkdir ..\..\bat_container\build
pushd ..\..\bat_container\build
cl -nologo -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_WIN32=1 -Z7 ..\..\HandMadeHero\win32_handmade.cpp user32.lib gdi32.lib winmm.lib
popd 
