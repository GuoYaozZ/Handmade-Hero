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