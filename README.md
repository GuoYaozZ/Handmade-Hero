# HandMade-Hero notes

### Day1 - Day5
Notes, they are in cpp files.  

### Day001
**Day001 learns: debug with VS2022, compile with build.bat, search function in [MSDN](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-messagebox)**  

build.bat: Learn how to use build.bat to set the environment and build the project.
emacs.bat: Editor.  
devenv.exe: Awake the VS2022 in Windows.
```
cl xxx.cpp //compile
cl /Zi xxx.cpp //generate debug detail
```
I use VS2022, 'Build.bat' can be used too in cmd.  

### Day002
**Day002 learns: open the window and paint on it**  
If we met import error(__imp_...) not defined, we can go to the MSDN website, find the function/property, grab the library which is need(shown on the site), and sticky it on build.bat.

### Day003
**Day003: Try to build a buffer and paint by buffer.**  
'HWND' is a window handle. 'HDC' (Handle to Device Context) is the device context handle.

### Day004
**Day004: Learn how to Animating the Buffer.**  
One way to prevent "use after free" is to protect the memory used.  
MEM_COMMIT - MEM_DECOMMIT, MEM_RESERVE - MEM_RELEASE, they are using in different circumstance.  

### Day005
**Day005: Recap: step-through of program behavior.**  
Optimization 1: Passing a struct is better than passing a pointer(which may cause pointer aliasing) when function and things is relativly small, beacuse compiler doesnt know enough information to do optimization.  
Optimization 2: Dealing with global variables more cleanly by bundling them up into structures.  
Stack: The stack supported by x86 hardware "grow downwards", the 'push' instruction causes the stack pointer (sp) to decrement by one slot, and the pop instruction causes the stack pointer to increment by one slot. Other hardware architectures may have different behaviors.

### Day006
**Day006: Keyboard Input and Controller Input**  

### Day007
**Day007: Initializing DirectSound**  
Learn how to request a cache and cache structure for sound.  

### Day008
**Day008: Writing a Square Wave to DirectSound**  

### Day009
**Day009: Variable-Pitch Sine Wave Output**  

### Day010
**Day010: QueryPerformanceCounter and RDTSC**  
QueryPerformanceCounter: function in the Windows API used to obtain high-precision timer values. It is often used in scenarios that require high-resolution time measurement, such as precise performance analysis, timers, frame rate control, etc.  
RDTSC: This instruction returns the number of clock cycles since the CPU started up.(Notice: Contains clock cycles used for task switches)  

### Day011
**Day011: The Basics of Platform API Design**  
Its straightforward, you just wrap up the types in your own open handle, and in the platform non-specific part you just don't show what it is.  
  
Case:  
```
linux_handmade.cpp -> handmade.cpp -> handmade.h
win32_handmade.cpp -> handmade.cpp -> handmade.h
specific part      -> non-specific part
```  
Notice: Specific part code used to compile some specific platform code and call it in non-specific part code.  
Notice: IF you want try different platform, there is a **[Handmade Penguin](https://davidgow.net/handmadepenguin/)** in community.

Avoid complicate logics, and treat GAME as a service of OS  

### Day012
**Day012: Platform independent Sound Output**  
To do Platform-independent work, I have to make sure that I understand all the work before. I guess I need a  time to do some review work?  
Updated the ./misc/build.bat to fit with the new name of project.  

### Day013
**Day013: Platform-independent User Input**  
```
union
{
	game_button_state Buttons[6];
	struct
	{
		game_button_state Up;
		game_button_state Down;
		game_button_state Left;
		game_button_state Right;
		game_button_state LeftShoulder;
		game_button_state RightShoulder;
	};
};
// a union can choose Up, Down... or loop Buttons[0], Buttons[1]...
```

### Day014
**Day014: Platform-independent Game Memory**  
VirtualAlloc: Reserves or commits a region of pages in the virtual address space of the calling process. Memory allocated by this function is automatically initialized to zero, unless MEM_RESET is specified.  
Q: How to trans some parameters in VS2022 while debugging?  


### Day015:
**Day015: Platform-independent Debug File I/O**  
Q: How to trans some parameters in VS2022 while debugging?  
A: sln(right click)->Properties->C/C++->cmd: -DHANDMADE_INTERNAL -DHANDMADE_SLOW  
```
CreateFileA(Filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
```
This Function wont create folders auto. so you have to create the folder manually and then the file will be created.  


### Day016:
**Day016: VisualStudio Compiler Switches**  
1. Some points about build.bat: the parameters meaning and how to use. About how to build based on Win64 of Win32  
2. By default the linker wont create a Windows XP compatible binary.  
```
cl -MT -nologo -Gm- -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -DHANDMADE_INTERNAL=1 -DHANDMADE_SLOW=1 -DHANDMADE_WIN32=1 -Z7 -Fmwin32_handmade.map ..\..\HandMadeHero\win32_handmade.cpp /link -opt:ref -subsystem:window,5.1 user32.lib gdi32.lib
// -MD: use the dll. 
// -MT: use static library
// -GM-: Turn off any sorts of incremental build stuff, and mimium rebuild.
// -Fm: Tells the linker loaction to stick a map file. Then there is a ".map" file which means ----- where all the functions are in your actual executable.
// -opt:ref: Dont put anything into the executable.(like some win32 runtime function)
```

### Day017
**Day 017: Unified Keyboard and Gamepad Input**
1. Adding 32-bit and 64-bit compiler line options to the build.bat.  
2. 
```
...
if (VKCode == 'W')
{
				Win32ProcessKeyboardMessage(&KeyboardController->MoveUp, IsDown);
}// I have to avoid VK_PROCESSKEY, why it would appeal before the normal key. and My program wont work on Assert(NewState->EndedDown != IsDown);
else if (VKCode == VK_PROCESSKEY)
{
				OutputDebugStringA("VK_PROCESSKEY, SKIP");
}
else if (VKCode == 'A')
{
				Win32ProcessKeyboardMessage(&KeyboardController->MoveLeft, IsDown);
}
...
```
3. Finally I found Out: Because I used a Chinese input method...  