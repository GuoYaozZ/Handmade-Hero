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

struct projectile {

	//These are the members, fields, of this structure.
	char unsigned IsThisOnFire; // Note: 1 if it's on fire. 0 if it's not 
	int Damage; // This is how much damage it does on impact
	int ParticlePerSecond; // For special effects
	short HowManyCooks; // Too many cooks?
	// 1 + 4 + 4 + 2 = 11 Bytes, you can compress it in 11Bytes, default is 16Bytes. 
	/*
	* char 8bit
	* short 16bits
	* int 32bits
	* In memory monitor, each value is 8bit, if you choose short, it would take 2Bytes which means 2values
	*/
	/*unsigned short Test;
	Test = 500;*/
	/*
	* +		&Test	0x000000743e0ff624 {500}	unsigned short *
	* 0x000000743E0FF624  244   1   0   0 220 242 239  30 254  ?...???.?
	* The memory is in Little-Endian (Endianness)
	*/
};

int CALLBACK WinMain(
	HINSTANCE hInstance,      // This is the handle of "WinMain" itself
	HINSTANCE hPrevInstance,  // Historical issues, wont never used
	LPSTR     lpCmdLine,      // CMD arguments£¨not contain function name£©
	int       nShowCmd        // this defines how the function window shows
) {
	projectile Test;
	int SizeofChar = sizeof(char unsigned);
	int SizeofInt = sizeof(int);
	int SizeofPorjectile = sizeof(projectile);
	int size = sizeof(Test);

	Test.IsThisOnFire = 1;
	Test.Damage = 2;
	Test.ParticlePerSecond = 3;
	Test.HowManyCooks = 4;

	projectile* ProjectilePointer = &Test;
	short* MrPointerMan = (short*)&Test;

	/*
	* 4 * 32 = 8 * 16 = 16 * 8
	* -		MrPointerMan,8	0x0000007d42effb18 {-13311, -13108, 2, 0, 3, 0, 4, -13108}	short[8]
	* 		[0]	-13311	short
			[1]	-13108	short
			[2]	2	short
			[3]	0	short
			[4]	3	short
			[5]	0	short
			[6]	4	short
			[7]	-13108	short

			0x0000007D42EFFB18    1 204 204 204   2   0   0   0   3  .???.....
			0x0000007D42EFFB21    0   0   0   4   0 204 204 204 204  .....????

								  0000 0001 1100 1100 1100 1100 1100 1100 0000 0002 0000 0000 0000 0000 0000 0000 0000 0011
											   -13311              -13108                   2                   0
								  0000 0000 0000 0000 0000 0000 0000 0004 0000 0100 1100 1100 1100 1100
										  3                   0                   4              -13108
	Why did I got -13108?
	204(decimal) is 0xCC and 204 204 is 0xCCCC = 52428(Unsigned Short, because I turned the memory monitor into unsigned mode.)
	So the Signed Short is 0xCCCC - 0x10000 (0xCCCC has a top bit = 1, which is negative) = -13108.
	Because, for a signed number x, if x > 0(the highest bit is 0), x(unsigned) = x(signed), else, x => -x, which is calculated as (x - 2^n)

	Why every thing is 204? MS like it.

	*/

	projectile Projectiles[40];

	projectile Projectile0;
	projectile Projectile1;
	int WhichProjectile = 1;
	if (WhichProjectile == 1) {

	}
	else {

	};

	Projectiles[4];

	projectile* ProjectilesPointer = Projectiles;
	int SizeofProjectiles = sizeof(Projectiles); // look at these two things, Projectiles can be used as list and pointer at the same time.
	int SizeofProjectilesPointer = sizeof(ProjectilePointer);

	Projectiles[30].Damage = 60;
	(ProjectilesPointer + 30)->Damage = 90;
	((projectile*)((char*)ProjectilesPointer + 30 * sizeof(projectile)))->Damage = 120;

	char* BytePointer = (char*)ProjectilesPointer;
	BytePointer = BytePointer + 30 * sizeof(projectile);
	projectile* Thirty = (projectile*)BytePointer;
	Thirty -> Damage = 200;

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