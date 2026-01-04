#include<windows.h>

void bar() {
	int BarVariable;
	BarVariable = 100;
}

void foo(void) { // Function signature or prototype
	/*
	* Have to add const
	* Taught what is ASCII
	* '\r' Sent the print head back to the start of the line
	* '\n' Advanced the paper by one line
	*/
	/*OutputDebugString(
		"Line 0\nLine 1\r"
	);*/
	/*OutputDebugString(
		L"Line 0\nLine 1\r"
	);*/
	/*
	* in debugapi.h can see
	    #ifdef UNICODE
		#define OutputDebugString  OutputDebugStringW
		#else
		#define OutputDebugString  OutputDebugStringA
	  VS will auto insert macros when create this project and UNICODE is DEFAULT
	*/
	int FooVariable;
	FooVariable = 200;
	bar();
	/*
	* Test = 255;
	* -		TestPointer	0x00000018c478fba4 ""	unsigned char *
	* 0x00000018C478FBA4  255 204 204 204 204 204 204 204 204  .????????
	* 
	* Line 56
	* Test = Test + 1;
	* -		TestPointer	0x00000018c478fba4 ""	unsigned char *
	* 0x00000018C478FBA4    0 204 204 204 204 204 204 204 204  .????????
	* 
	* foo();
	* int FooVariable;FooVariable = 200;
	* -		&FooVariable	0x00000018c478fa84 {6553710}	int *
	* 
	* FooVariable = 200;
	* -		&FooVariable	0x00000018c478fa84 {200}	int *
	* 0x00000018C478FA84  200   0   0   0  77   0  97   0 100  ?...M.a.d
	* 
	* BarVariable = 100;
	* +		&BarVariable	0x00000018c478f964 {100}	int *
	* 0x00000018C478F964  100   0   0   0 199  56 240 115 254  d...?8?s?
	* 
	* from 0x00000018c478fba4 to
	*      0x00000018c478fa84 to
	*      0x00000018c478f964 to
	*      ...
	* This is how Stack in Function call works.
	* And -- Don't Ctrl+S while looking in memory.
	* 
	* Due to the Address Space Layout Randomization(for safety), the memory address changed. 
	* 
	* * How the C++ and OS determine how much memory to allocate
	*/
}

int CALLBACK WinMain(
	HINSTANCE hInstance,      // This is the handle of "WinMain" itself
	HINSTANCE hPrevInstance,  // Historical issues, wont never used
	LPSTR     lpCmdLine,      // CMD arguments£¨not contain function name£©
	int       nShowCmd        // this defines how the function window shows
) {
	// CALLBACK HINSTANCE LPSTR _In_: Macro Definition by Windows OS
	// _In_: Tell the compiler this is an input parameter
	// To use WinMain: Project->Attributes->Linker->System->Subsystem->Window
	// Or: Just use main()
	char unsigned Test;
	char unsigned* TestPointer = &Test;
	Test = 255;
	Test = Test + 1;
	// The Debugger can reach the memory and change the value
	foo();
}
/*
* Test = 255 + 1
* TestPointer = 0x000000ec1aaff8b4a
* 0x000000EC1AAFF8B4  00 cc cc cc cc cc cc cc cc cc cc  .??????????
* 
* Test = 16
* TestPointer = 0x000000ec1aaff8b4a
* 0x000000EC1AAFF8B4  10 cc cc cc cc cc cc cc cc cc cc  .??????????
* 0x000000EC1AAFF8B4   16 204 204 204 204 204 204 204 204  .???????? (in unsigned mode)
* the value in memory would change after you change the value in debugger 
* 
* in watch window:
* TestPointer + 1 = 0x000000ec1aaff8b5
* value to be 12
* 0x000000EC1AAFF8B5   12 204 204 204 204 204 204 204 204  .????????
* 
* Can't touch the memory you don't own yet. 
*/

/*
* The memory getting is embed in C language, and get the memory from OS automaticly
* The function is call and ret, the memory of function in memory is alocated by Stack.
* 1 -> 2
*	   2 -> 3
*           3()
*      2 <- 3
* 1 <- 2
* 
*/