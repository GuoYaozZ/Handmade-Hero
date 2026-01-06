#include<windows.h>

int CALLBACK WinMain(
	HINSTANCE hInstance,      
	HINSTANCE hPrevInstance,  
	LPSTR     lpCmdLine,      
	int       nShowCmd
) {
	int y = 5;
	int* ptr = 0;
	int** ptrptr = 0;
	
	if (y == 5) {
		int x = 6;
		int y = 4; //its a different y, legal shadowing with new scopes.
		ptr = &x;
		ptrptr = &ptr;
	}
	
	y = (*ptr);
	/*
	* Dereference turns it into a memory location
	* And by this way we can refer to x out of the block.
	* Save the address by '&', and dereference the address by '*'.
	*/
	y = (**ptrptr);
	/*
	* Pointer to Pointer
	* And Arrow in Struct is like:Dereference the pointer and do the dot. (*ptr)->
	*/
}
