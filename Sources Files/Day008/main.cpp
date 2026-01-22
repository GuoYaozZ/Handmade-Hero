#include<Windows.h>
#include<stdint.h>
#include<xinput.h>
#include<dsound.h>

#define internal static // this function is used only in this file.
#define local_persist static
#define global_variable static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int32 bool32;

struct win32_offscreen_buffer
{
	BITMAPINFO Info;
	void* Memory;
	int Width;
	int Height;
	int Pitch;
	int BytesPerPixel;
};

struct win32_window_dimension
{
	int Width;
	int Height;
};

//TODO: This is golbal for now.
global_variable bool GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackBuffer;
global_variable LPDIRECTSOUNDBUFFER GlobalSecondaryBuffer;

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuildDevice, LPDIRECTSOUND* ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

//you can use function type to clarify a function
//x_input_get_state xxx = DWORD WINAPI xxx(DWORD dwUserIndex, XINPUT_STATE* pState)
//or just typedef a function type(function pointer type) to a function
//global_variable x_input_get_state* xxx;
//https://www.reddit.com/r/C_Programming/comments/2pkwvf/whats_the_use_of_a_typedef_of_a_bare_function_not/

#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}
global_variable x_input_get_state* XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
	return ERROR_DEVICE_NOT_CONNECTED;
}
global_variable x_input_set_state* XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

//https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya
internal void Win32LoadXInput(void)
{
	HMODULE XInputLibrary = LoadLibraryA("xinput1_4.dll");
	if (!XInputLibrary)
	{
		HMODULE XInputLibrary = LoadLibraryA("xinput1_3.dll");
	}
	if (XInputLibrary)
	{
		XInputGetState = (x_input_get_state*)GetProcAddress(XInputLibrary, "XInputGetState");
		if (!XInputGetState) { XInputGetState = XInputGetStateStub; }

		XInputSetState = (x_input_set_state*)GetProcAddress(XInputLibrary, "XInputSetState");
		if (!XInputSetState) { XInputSetState = XInputSetStateStub; }
	}
	else
	{

	}
}

internal void Win32InitDSound(HWND Window, int32 SamplesPerSecond, int32 BufferSize)
{
	// Load the Library
	HMODULE DSoundLibrary = LoadLibraryA("dsound.dll");
	if (DSoundLibrary)
	{
		// Get a DirectSoundCreate object! from dll - cooperative
		// after you converted it into function pointer, you can use it like func
		direct_sound_create* DirectSoundCreate = (direct_sound_create*)
			GetProcAddress(DSoundLibrary, "DirectSoundCreate");

		// Create a primary buffer
		LPDIRECTSOUND DirectSound;
		if (DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
		{
			// describe PCM audio format
			WAVEFORMATEX WaveFormat = {};
			WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
			WaveFormat.nChannels = 2;
			WaveFormat.nSamplesPerSec = SamplesPerSecond;
			WaveFormat.wBitsPerSample = 16;
			WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8;
			WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;
			WaveFormat.cbSize = 0;

			// set the cooperative level, DSSCL_PRIORITY means have the privilige to set the buffer format
			if (SUCCEEDED(DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY)))
			{
				//https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416820(v=vs.85)
				DSBUFFERDESC BufferDescription = {};
				BufferDescription.dwSize = sizeof(BufferDescription);
				BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;

				LPDIRECTSOUNDBUFFER PrimaryBuffer;
				// create the Primary buffer
				// this is not to write audio data, this is more like a carrier
				//https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee418039(v=vs.85)
				if (SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &PrimaryBuffer, 0)))
				{

					/*
					HRESULT Error = PrimaryBuffer->SetFormat(&WaveFormat);
					if(SUCCEEDED(Error))
					...how to debug
					*/

					// Create the memory and write sample in it.
					if (SUCCEEDED(PrimaryBuffer->SetFormat(&WaveFormat)))
					{
						// finally set the format!
						OutputDebugStringA("Primary Buffer!\n");
					}
					else
					{

					}

				}
			}
			else
			{
				// Diagnostic
			}

			// 'Create' a secondary buffer
			// This is the buffer actually used to writing/playback
			DSBUFFERDESC BufferDescription = {};
			BufferDescription.dwSize = sizeof(BufferDescription);
			BufferDescription.dwFlags = 0;
			BufferDescription.dwBufferBytes = BufferSize;
			BufferDescription.lpwfxFormat = &WaveFormat;
			if (SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &GlobalSecondaryBuffer, 0)))
			{
				// Start it playing
				OutputDebugStringA("Secondary Buffer!\n");
			}

		}
		else
		{
			// DIAGNOSTIC
		}
	}
}

internal win32_window_dimension Win32GetWindowDimension(HWND Window)
{
	win32_window_dimension Result;

	RECT ClientRect;
	GetClientRect(Window, &ClientRect);
	Result.Height = ClientRect.bottom - ClientRect.top;
	Result.Width = ClientRect.right - ClientRect.left;

	return Result;
}

internal void RenderWeirdGradient(win32_offscreen_buffer *Buffer, int BlueOffset, int GreenOffset)
{
	uint8* Row = (uint8*)Buffer->Memory;
	for (int Y = 0; Y < Buffer->Height; ++Y)
	{
		uint32* Pixel = (uint32*)Row;
		for (int X = 0; X < Buffer->Width; ++X)
		{
			uint8 Blue = (X + BlueOffset);
			uint8 Green = (Y + GreenOffset);

			*Pixel++ = ((Green << 8) | Blue);
		}
		Row += Buffer->Pitch;
	}
}

// DIB Device Independent Bitmap.
internal void Win32ResizeDIBection(win32_offscreen_buffer* Buffer, int Width, int Height)
{
	// TODO: Bulletproof this.
	// Maybe dont free first, free afterm then free first if that fails.

	if (Buffer->Memory)
	{
		VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
	}

	Buffer->Width = Width;
	Buffer->Height = Height;
	Buffer->BytesPerPixel = 4;

	/*
	* When the biHeight is negative, this is the clue to Windows to treat this bitmap as top-down. 
	*/
	Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
	Buffer->Info.bmiHeader.biWidth = Buffer->Width;
	Buffer->Info.bmiHeader.biHeight = -Buffer->Height;

	Buffer->Info.bmiHeader.biPlanes = 1;
	Buffer->Info.bmiHeader.biBitCount = 32;
	Buffer->Info.bmiHeader.biCompression = BI_RGB;

	int BitmapMemorySize = (Buffer->Width * Buffer->Width) * Buffer->BytesPerPixel;
	Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

	Buffer->Pitch = Width * Buffer->BytesPerPixel;
}

internal void Win32DsiplayBufferInWindow(HDC DeviceContext,
	int WindowWidth, int WindowHeight,
	win32_offscreen_buffer* Buffer)
{
	// How it gonna use our memory?
	StretchDIBits(DeviceContext,
				  0, 0, WindowWidth, WindowHeight,
				  0, 0, Buffer->Width, Buffer->Height,
				  Buffer->Memory,
				  &Buffer->Info,
			   	  DIB_RGB_COLORS, SRCCOPY);
}

internal LRESULT CALLBACK MainWindowCallback(
	HWND    Window,//HWND    hWnd,
	UINT    Message,//UINT    Msg,
	WPARAM  wParam,//WPARAM  wParam,
	LPARAM  lParam//LPARAM  lParam
)
{
	LRESULT Result = 0;
	switch (Message) 
	{
		case WM_CREATE:
		{
		} break;
		case WM_SIZE: 
		{
			
		} break;
		case WM_DESTROY: 
		{
			//TODO: Handle this as an error - recreate window?
			GlobalRunning = false;
		} break;
		case WM_CLOSE: 
		{
			//TODO: Handle this with a message to the user?
			GlobalRunning = false;
			//PostQuitMessage(0);
			/*
			* post the quit message into message queue
			* and after a while we will see the a False MessageResult in our infinite loop.
			*/
		} break;
		case WM_ACTIVATEAPP:
		{
		} break;
		/*
		* https://learn.microsoft.com/en-us/windows/win32/learnwin32/keyboard-input
		* https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		*/
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			uint32 VKCode = wParam;
			bool WasDown = ((lParam & (1 << 30)) != 0);
			bool IsDown = ((lParam & (1 << 31)) == 0);
			if (WasDown != IsDown)
			{
				if (VKCode == 'W')
				{
					OutputDebugStringA("W: ");
					if (WasDown)
					{
						OutputDebugStringA("WasDown ");
					}
					if (IsDown)
					{
						OutputDebugStringA("IsDown ");
					}
					OutputDebugStringA("\n");
				}
				else if (VKCode == 'A')
				{

				}
				else if (VKCode == 'S')
				{

				}
				else if (VKCode == 'D')
				{

				}
				else if (VKCode == 'Q')
				{

				}
				else if (VKCode == 'E')
				{

				}
				else if (VKCode == VK_UP)
				{

				}
				else if (VKCode == VK_DOWN)
				{

				}
				else if (VKCode == VK_LEFT)
				{

				}
				else if (VKCode == VK_RIGHT)
				{

				}
				else if (VKCode == VK_ESCAPE)
				{
					OutputDebugStringA("ESCAPE: ");
					if (WasDown)
					{
						OutputDebugStringA("WasDown ");
					}
					if (IsDown)
					{
						OutputDebugStringA("IsDown ");
					}
					OutputDebugStringA("\n");
				}
				else if (VKCode == VK_SPACE)
				{

				}
			}


			// Alt+F4 could be auto handled in windows by DefWindowProc.
			bool32 AltKeyWasDown = lParam & (1 << 29);
			if ((VKCode == VK_F4) && AltKeyWasDown)
			{
				GlobalRunning = false;
			}
		} break;
		case WM_PAINT: 
		{   // When Stretch our window, progress will stuck in here.
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			
			win32_window_dimension Dimension = Win32GetWindowDimension(Window);
			Win32DsiplayBufferInWindow(DeviceContext, Dimension.Width, Dimension.Height,
									   &GlobalBackBuffer);
			//PatBlt(DeviceContext, X, Y, Width, Height, WHITENESS);
			EndPaint(Window, &Paint);
		} break;
		default:
		{
			Result = DefWindowProc(Window, Message, wParam, lParam);
		} break;
	}

	return Result;
}

int CALLBACK WinMain(
	HINSTANCE Instance, //HINSTANCE hInstance,
	HINSTANCE PrevInstance,//HINSTANCE hPrevInstance,
	LPSTR     CommandLine,//LPSTR     lpCmdLine,
	int       ShowCode//int       nShowCmd
)
{
	Win32LoadXInput();

	WNDCLASSA WindowClass = {};

	Win32ResizeDIBection(&GlobalBackBuffer, 1280, 720);

	// CS_OWNDC not need.
	WindowClass.style = CS_HREDRAW|CS_VREDRAW;
	WindowClass.lpfnWndProc = MainWindowCallback;
	WindowClass.hInstance = Instance; // can call GetModuleHandle();
	WindowClass.lpszClassName = "HademadeHeroWindowClass";
	if (RegisterClass(&WindowClass))
	{
		HWND Window= CreateWindowExA(
			0,
			WindowClass.lpszClassName,
			"Handmade Hero",
			WS_OVERLAPPEDWINDOW|WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			0,
			0,
			Instance,
			0
		);
		if (Window)
		{
			GlobalRunning = true;
			HDC DeviceContext = GetDC(Window);

			// Graphic test
			int XOffset = 0;
			int YOffset = 0;

			// Sound Test
			// if SquareWaveCounter happens tobe 0;
			int SamplePerSecond = 48000;
			int ToneHz = 256;
			int ToneVolume = 3000;
			uint32 RunningSampleIndex = 0;
			// Remember how many sound we output
			int SquareWavePeriod = SamplePerSecond / ToneHz;
			int HalfSquareWavePeriod = SquareWavePeriod / 2;
			int BytesPerSample = sizeof(int16) * 2;
			int SecondaryBufferSize = SamplePerSecond * BytesPerSample;
			
			Win32InitDSound(Window, SamplePerSecond, SecondaryBufferSize);
			GlobalSecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

			while (GlobalRunning)// most recommend 
			{
				MSG Message;
				if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
				{
					if (Message.message == WM_QUIT)
					{
						GlobalRunning = false;
					}

					TranslateMessage(&Message); // Translate Key Codes
					DispatchMessage(&Message); // Will call our window class as necessary.
				}

				// Should we pool this more frequently?
				/*
				* XInput
				* https://learn.microsoft.com/en-us/windows/win32/xinput/xinput-game-controller-apis-portal
				* XINPUT_STATE structure
				* https://learn.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_state
				* typedef struct _XINPUT_STATE {
					  DWORD          dwPacketNumber;
					  XINPUT_GAMEPAD Gamepad;
					} XINPUT_STATE, *PXINPUT_STATE;
				*/
				for (DWORD ControllerIndex = 0;
					ControllerIndex < XUSER_MAX_COUNT;
					++ControllerIndex)
				{
					XINPUT_STATE ControllerState;
					if (XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
					{
						// This controller is plugged in.
						XINPUT_GAMEPAD* Pad = &ControllerState.Gamepad;

						bool Up = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_UP);
						bool Down = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
						bool Left = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
						bool Right = (Pad->wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);
						bool Start = (Pad->wButtons & XINPUT_GAMEPAD_START);
						bool Back = (Pad->wButtons & XINPUT_GAMEPAD_BACK);
						bool LeftShoulder = (Pad->wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
						bool RightShoulder = (Pad->wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);
						bool AButton = (Pad->wButtons & XINPUT_GAMEPAD_A);
						bool BButton = (Pad->wButtons & XINPUT_GAMEPAD_B);
						bool XButton = (Pad->wButtons & XINPUT_GAMEPAD_X);
						bool YButton = (Pad->wButtons & XINPUT_GAMEPAD_Y);

						int16 StickX = Pad->sThumbLX;
						int16 StickY = Pad->sThumbLY;

						XOffset += StickX >> 12;
						YOffset += StickY >> 12;
					}
					else
					{
						// This controller is not available.
					}
				}

				// if you want to use an API, always remember to look up the MSDN handbook.
				XINPUT_VIBRATION Vibration;
				Vibration.wLeftMotorSpeed = 60000;
				Vibration.wRightMotorSpeed = 60000;
				XInputSetState(0, &Vibration);

				RenderWeirdGradient(&GlobalBackBuffer, XOffset, YOffset);

				//Right here! After our rendering, we try to lock out audio buffer.
				// Direct Sound output test
				// See in DSBUFFERDESC page : Buffer Basics
				// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee418073(v=vs.85)
				DWORD PlayCursor;
				DWORD WriteCursor;
				if (SUCCEEDED(GlobalSecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor)))
				{
					DWORD BytesToLock = RunningSampleIndex * BytesPerSample % SecondaryBufferSize;
					DWORD BytesToWrite;
					if (BytesToLock > PlayCursor)
					{
						BytesToWrite = (SecondaryBufferSize - BytesToLock);
						BytesToWrite += PlayCursor;
					}
					else
					{
						BytesToWrite = PlayCursor - BytesToLock;
					}
					// caculate WritePointer/BytesTowrite by GetCurrentPositionMethod.
					// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee418062(v=vs.85)
					VOID* Region1;
					DWORD Region1Size;
					VOID* Region2;
					DWORD Region2Size;
					if (SUCCEEDED(GlobalSecondaryBuffer->Lock(
						BytesToLock, BytesToWrite,
						&Region1, &Region1Size,
						&Region2, &Region2Size,
						0)))
					{
						// TODO Assert that Region1Size/Region2Size is valid
						int16* SampleOut = (int16*)Region1;
						DWORD Region1SampleCount = Region1Size / BytesPerSample;
						DWORD Region2SampleCount = Region2Size / BytesPerSample;
						for (DWORD SampleIndex = 0; SampleIndex < Region1SampleCount; ++SampleIndex)
						{
							int16 SampleValue = ((RunningSampleIndex / HalfSquareWavePeriod) % 2) ? ToneVolume : -ToneVolume;
							*SampleOut++ = SampleValue;
							*SampleOut++ = SampleValue;
							++RunningSampleIndex;
						}
						SampleOut = (int16*)Region2;
						for (DWORD SampleIndex = 0; SampleIndex < Region2SampleCount; ++SampleIndex)
						{
							int16 SampleValue = ((RunningSampleIndex / HalfSquareWavePeriod) % 2) ? ToneVolume : -ToneVolume;
							*SampleOut++ = SampleValue;
							*SampleOut++ = SampleValue;
							++RunningSampleIndex;
						}

						GlobalSecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size)
					}
				}

				win32_window_dimension Dimension = Win32GetWindowDimension(Window);
				Win32DsiplayBufferInWindow(DeviceContext, Dimension.Width, Dimension.Height,
										   &GlobalBackBuffer);
			}
		}
		else
		{

		}
	}
	else
	{

	}

	return 0;
}