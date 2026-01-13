#include<Windows.h>
#include<stdint.h>

#define internal static // this function is used only in this file.
#define local_persist static
#define global_variable static

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

//TODO: This is golbal for now.
global_variable bool Running;

global_variable BITMAPINFO BitmapInfo;
global_variable void* BitmapMemory;
global_variable int BytesPerPixel = 4;
global_variable int BitmapWidth;
global_variable int BitmapHeight;

internal void RenderWeirdGradient(int XOffset, int YOffset)
{
	/*
	* using 8bits because anytime you move a pointer,
	* C will move the pointer * size() in memory.
	*/
	int Width = BitmapWidth;
	int Height = BitmapHeight;
	int Pitch = Width * BytesPerPixel;
	uint8* Row = (uint8*)BitmapMemory;
	for (int Y = 0; Y < BitmapHeight; ++Y)
	{
		uint8* Pixel = (uint8*)Row;
		for (int X = 0; X < BitmapWidth; ++X)
		{
			/*                  0  1  2  3
			* Pixel in memory: 00 00 00 00
			*                  RR GG BB XX
			* Little Endian Architecture.
			*                   2  3  0  1
			* Pixel in memory: 00 00 00 00
			*                  XX BB GG RR
			*/
			*Pixel = (uint8)(X + XOffset);
			++Pixel;

			*Pixel = (uint8)(Y + XOffset);
			++Pixel;

			*Pixel = 0;
			++Pixel;

			*Pixel = 0;
			++Pixel;
		}
		Row += Pitch;
	}
}

// DIB Device Independent Bitmap.
internal void Win32ResizeDIBection(int Width, int Height)
{
	// TODO: Bulletproof this.
	// Maybe dont free first, free afterm then free first if that fails.

	if (BitmapMemory)
	{
		VirtualFree(BitmapMemory, 0, MEM_RELEASE);
	}

	BitmapWidth = Width;
	BitmapHeight = Height;

	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biWidth = BitmapWidth;
	BitmapInfo.bmiHeader.biHeight = -BitmapHeight;
	/*
	* StretchDIBits creates a top-down image if the sign of the biHeight member of the BITMAPINFOHEADER structure for the DIB is negative. For a code example, see Sizing a JPEG or PNG Image.
	*/
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;

	int BitmapMemorySize = (BitmapWidth * BitmapWidth) * BytesPerPixel;
	BitmapMemory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);
	/*
	* Memory allocate has two types: Virtual-alloc and Heap-alloc
	* https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
	* Allocte by Pages.
	*/
}
/*
* CALLBACK means a function we want the Windows API to call.
* Primarily, CALLBACK explains how the function is called.
*/
internal void Win32UpdateWindow(HDC DeviceContext, RECT* WindowRect, int X, int Y, int Width, int Height)
{
	int WindowWidth = WindowRect->right - WindowRect->left;
	int WindowHeight = WindowRect->bottom - WindowRect->top;

	// How it gonna use our memory?
	StretchDIBits(DeviceContext,
				  0, 0, BitmapWidth, BitmapHeight,
				  0, 0, WindowWidth, WindowHeight,
				  BitmapMemory,
				  &BitmapInfo, 
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
			//We have to know what the size of window after WM_SIZE by GetClientrect.
			RECT ClientRect;
			GetClientRect(Window, &ClientRect);
			int Height = ClientRect.bottom - ClientRect.top;
			int Width = ClientRect.right - ClientRect.left;
			Win32ResizeDIBection(Width, Height);
		} break;
		case WM_DESTROY: 
		{
			//TODO: Handle this as an error - recreate window?
			Running = false;
			OutputDebugStringA("WM_DESTROY\n");
		} break;
		case WM_CLOSE: 
		{
			OutputDebugStringA("WM_CLOSE\n");
			//TODO: Handle this with a message to the user?
			Running = false;
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
		{
			OutputDebugStringA("WM_PAINT\n");
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			int X = Paint.rcPaint.left;
			int Y = Paint.rcPaint.top;
			int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
			int Width = Paint.rcPaint.right - Paint.rcPaint.left;
			RECT ClientRect;
			GetClientRect(Window, &ClientRect);
			Win32UpdateWindow(DeviceContext, &ClientRect, X, Y, Width, Height);
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

	WindowClass.style = CS_OWNDC|CS_HREDRAW|CS_VREDRAW;
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
			Running = true;
			int XOffset = 0;
			int YOffset = 0;
			while(Running)// most recommend 
			{
				MSG Message;
				if (PeekMessage(&Message, 0, 0, 0, PM_REMOVE))
				{
					if (Message.message == WM_QUIT)
					{
						Running = false;
					}

					TranslateMessage(&Message);
					DispatchMessage(&Message);
				}
				RenderWeirdGradient(XOffset, YOffset);

				HDC DeviceContext = GetDC(Window);
				RECT ClientRect;
				GetClientRect(Window, &ClientRect);
				int WindowWidth = ClientRect.right - ClientRect.left;
				int WindowHeight = ClientRect.bottom - ClientRect.top;
				Win32UpdateWindow(DeviceContext, &ClientRect, 0, 0, WindowWidth, WindowHeight);
				//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea
				//by peekmessage to get rid of block.
				++XOffset;
				++YOffset;
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