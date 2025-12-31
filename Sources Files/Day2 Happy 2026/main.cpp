#include<windows.h>

void foo(void) { // Function signature or prototype
	const char *Foo = "Hello World!\n";
	/*
	* Have to add const
	* Taught what is ASCII
	* '\r' Sent the print head back to the start of the line
	* '\n' Advanced the paper by one line
	*/
	/*OutputDebugString(
		"Line 0\nLine 1\r"
	);*/
	OutputDebugString(
		L"Line 0\nLine 1\r"
	);
	/*
	* in debugapi.h can see
	    #ifdef UNICODE
		#define OutputDebugString  OutputDebugStringW
		#else
		#define OutputDebugString  OutputDebugStringA
	  VS will auto insert macros when create this project and UNICODE is DEFAULT
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
	int a = 5;
	int b = 2;
	int c = a + b;
	/*
	Taught some points in Computer Architecture and C

	IMPORTANT that VS community can "Go To Disassembly" and "Register" window!

	Happy Happy 01/01/2026 00:00:00 and this would be a totally different year for me.
	*/
	foo();
}