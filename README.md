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