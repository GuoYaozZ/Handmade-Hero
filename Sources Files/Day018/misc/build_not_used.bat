@echo off
call "F:\VisualStudio2022\VC\Auxiliary\Build\vcvarsall.bat" x64
set path="F:\MyVSProject\HandMadeHero\misc";%path%

set CommonCompilerFlags = -MT -nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_WIN32=1 -Z7 -Fmwin32_handmade.map
set CommonLinkerFlags = -opt:ref user32.lib gdi32.lib

IF NOT EXIST ..\..\bat_container\build mkdir ..\..\bat_container\build
pushd ..\..\bat_container\build

REM 32-bit build
REM cl %CommonCompilerFlags% ..\..\HandMadeHero\win32_handmade.cpp /link -subsystem:window,5.1 %CommonLinkerFlags%

REM 64-bit build
cl %CommonCompilerFlags% ..\..\HandMadeHero\win32_handmade.cpp /link %CommonLinkerFlags%
popd 
