#include<Windows.h>
#include<stdint.h>

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

win32_window_dimension Win32GetWindowDimension(HWND Window)
{
	win32_window_dimension Result;

	RECT ClientRect;
	GetClientRect(Window, &ClientRect);
	Result.Height = ClientRect.bottom - ClientRect.top;
	Result.Width = ClientRect.right - ClientRect.left;

	return Result;
}

//TODO: This is golbal for now.
global_variable bool GlobalRunning;
global_variable win32_offscreen_buffer GlobalBackBuffer;

internal void RenderWeirdGradient(win32_offscreen_buffer Buffer, int BlueOffset, int GreenOffset)
{
	uint8* Row = (uint8*)Buffer.Memory;
	for (int Y = 0; Y < Buffer.Height; ++Y)
	{
		uint32* Pixel = (uint32*)Row;
		for (int X = 0; X < Buffer.Width; ++X)
		{
			uint8 Blue = (X + BlueOffset);
			uint8 Green = (Y + GreenOffset);

			*Pixel++ = ((Green << 8) | Blue);
		}
		Row += Buffer.Pitch;
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
	Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

	Buffer->Pitch = Width * Buffer->BytesPerPixel;
}

internal void Win32DsiplayBufferInWindow(HDC DeviceContext,
	int WindowWidth, int WindowHeight,
	win32_offscreen_buffer Buffer, int X, int Y, int Width, int Height)
{
	// How it gonna use our memory?
	StretchDIBits(DeviceContext,
				  0, 0, WindowWidth, WindowHeight,
				  0, 0, Buffer.Width, Buffer.Height,
				  Buffer.Memory,
				  &Buffer.Info,
			   	  DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK MainWindowCallback(
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
			OutputDebugStringA("WM_CREATE\n");
		} break;
		case WM_SIZE: 
		{
			
		} break;
		case WM_DESTROY: 
		{
			//TODO: Handle this as an error - recreate window?
			GlobalRunning = false;
			OutputDebugStringA("WM_DESTROY\n");
		} break;
		case WM_CLOSE: 
		{
			OutputDebugStringA("WM_CLOSE\n");
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
			OutputDebugStringA("WM_ACTIVATEAPP\n");
		} break;
		case WM_PAINT: 
		{   // When Stretch our window, progress will stuck in here.
			OutputDebugStringA("WM_PAINT\n");
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			int X = Paint.rcPaint.left;
			int Y = Paint.rcPaint.top;
			int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
			int Width = Paint.rcPaint.right - Paint.rcPaint.left;
			
			win32_window_dimension Dimension = Win32GetWindowDimension(Window);
			Win32DsiplayBufferInWindow(DeviceContext, Dimension.Width, Dimension.Height,
									   GlobalBackBuffer, X, Y, Width, Height);
			//PatBlt(DeviceContext, X, Y, Width, Height, WHITENESS);
			EndPaint(Window, &Paint);
		} break;
		default:
		{
			OutputDebugStringA("DEFAULT\n");
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
	WNDCLASS WindowClass = {};

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
			int XOffset = 0;
			int YOffset = 0;
			while(GlobalRunning)// most recommend 
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
				RenderWeirdGradient(GlobalBackBuffer, XOffset, YOffset);

				HDC DeviceContext = GetDC(Window);
				win32_window_dimension Dimension = Win32GetWindowDimension(Window);
				Win32DsiplayBufferInWindow(DeviceContext, Dimension.Width, Dimension.Height,
										   GlobalBackBuffer, 0, 0, Dimension.Width, Dimension.Height);
				//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea
				//by peekmessage to get rid of block.
				++XOffset;
				YOffset += 2;
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