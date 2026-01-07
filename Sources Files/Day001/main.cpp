/*
* 
* Date: 2026-01-07
*/
#include<Windows.h>
#include<stdio.h>

int CALLBACK WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
) {
	//GetModuleHandle(0);
	/*
	* Test if cl add kernel32 automaticly. 
	*/
	MessageBox(0, "Hello this is Handmadehero", "Hello!",
			   MB_OK|MB_ICONINFORMATION);
	/*
	* Actually MessageBox is a macro: MessageBoxA or MessageBoxW, that depends on UTF-8 of ANSI mode.
	*/
	/*
	* MB_OK: The message box contains one push button: OK. This is the default.
	* MB_ICONINFORMATION: An icon consisting of a lowercase letter i in a circle appears in the message box.
	* OR these together is to tell what MessageBox to do.
	*/
	return 0;
}
