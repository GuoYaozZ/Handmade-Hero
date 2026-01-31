#if !defined(HANDMADE_H)

// NOTE services that the platform layer provide to the game


// NOTE services that the game provide to the platform layer

// FourThing: timing, controller/keyboard input, bitmap buffer to use, sound buffer to use.
#define internal static // this function is used only in this file.
#define local_persist static
#define global_variable static

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

internal void GameUpdateAndRender(game_offscreen_buffer* Buffer, int BlueOffset, int GreenOffset,
								  game_sound_output_buffer* SoundBuffer);

#define HANDMADE_H
#endif