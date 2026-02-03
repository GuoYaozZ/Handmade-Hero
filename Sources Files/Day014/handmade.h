#if !defined(HANDMADE_H)

/*
* HANDMADE_INTERNAL:
*  0 - Build for public realease
*  1 - Build for developer only
* 
* HANDMADE_SLOW:
*  0 - Not slow code allowed!
*  1 - Slow code welcome.
* 
* But I dont know how to trans some parameters in VS2022 while debugging. 
* So these parameters is now recommend to used reversed. 0 = 1 1 = 0.
*/

#if HANDMADE_SLOW
#define Assert(Expression) if(!(Expression)) {*(int*)0 = 0;}
#else
#define Assert(Expression)
#endif

// NOTE services that the platform layer provide to the game


// NOTE services that the game provide to the platform layer

// FourThing: timing, controller/keyboard input, bitmap buffer to use, sound buffer to use.
#define internal static // this function is used only in this file.
#define local_persist static
#define global_variable static

#define ArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))
#define Kilobytes(Value) ((Value)*1024LL)
#define Megabytes(Value) (Kilobytes(Value)*1024LL)
#define Gigabytes(Value) (Megabytes(Value)*1024LL)
#define Terabytes(Value) (Gigabytes(Value)*1024LL)

#define Pi32 3.141592653591f

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int32 bool32;
typedef float real32;
typedef double real64;

struct game_offscreen_buffer
{
	//BITMAPINFO Info;
	void* Memory;
	int Width;
	int Height;
	int Pitch;
	int BytesPerPixel;
};

struct game_sound_output_buffer
{
	int SamplesPerSecond; //tells the buffer itself. 
	int SampleCount; // 
	int16* Samples;
	int BytesPerSample;
};

struct game_button_state
{
	int HalfTransitionCount;
	bool32 EndeDown;
};

struct game_controller_input
{
	bool32 IsAnalog;

	real32 StartX;
	real32 StartY;
	real32 MinX;
	real32 MinY;
	real32 MaxX;
	real32 MaxY;
	real32 EndX;
	real32 EndY;

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
};

struct game_input
{
	//TODO insert clock here;
	real32 GameClock;
	game_controller_input Controllers[4];
};

struct game_memory
{
	bool32 IsInitialized;
	uint64 PermanentStorageSize;
	void* PermanentStorage; // Required to be cleared to zero at startup.

	uint64 TransientStorageSize;
	void* TransientStorage;
};

internal void GameUpdateAndRender(game_memory *Memory,
								  game_input* Input,
								  game_offscreen_buffer* Buffer,
								  game_sound_output_buffer* SoundBuffer);

struct game_state
{
	int ToneHz;
	int GreenOffset;
	int BlueOffset;
};

#define HANDMADE_H
#endif