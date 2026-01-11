#include<Windows.h>

#define internal static // this function is used only in this file.
#define local_persist static
#define global_variable static

//TODO: This is golbal for now.
global_variable bool Running;
global_variable BITMAPINFO BitmapInfo;
global_variable void* BitmapMemory;
global_variable HBITMAP BitmapHandle;
global_variable HDC BitmapDeviceContext;
// if use static to initialize a global variable, it would be 0;

// DIB Device Independent Bitmap.
internal void Win32ResizeDIBection(int Width, int Height)
{
	// TODO: Bulletproof this.
	// Maybe dont free first, free afterm then free first if that fails.

	if (BitmapHandle)
	{
		DeleteObject(BitmapHandle);
	}
	
	if (!BitmapDeviceContext)
	{
		// TODO : Should we recreate these under certain circumstances.
		BitmapDeviceContext = CreateCompatibleDC(0);
	}

	BitmapInfo.bmiHeader.biSize = sizeof(BitmapInfo.bmiHeader);
	BitmapInfo.bmiHeader.biWidth = Width;
	BitmapInfo.bmiHeader.biHeight = Height;
	BitmapInfo.bmiHeader.biPlanes = 1;
	BitmapInfo.bmiHeader.biBitCount = 32;
	BitmapInfo.bmiHeader.biCompression = BI_RGB;

	BitmapHandle = CreateDIBSection(
		BitmapDeviceContext, &BitmapInfo,
		DIB_RGB_COLORS,
		&BitmapMemory, // A pointer to a pointer! This is a trick to get mutiple return values?
		0, 0
	);
	/*
	* CreateDIBSection need to return BitmapHandle, It also need to return this BitmapMemory
	*/
}

/*
* CALLBACK means a function we want the Windows API to call.
* Primarily, CALLBACK explains how the function is called.
*/
internal void Win32UpdateWindow(HDC DeviceContext, int X, int Y, int Width, int Height)
{
	StretchDIBits(DeviceContext,
		X, Y, Width, Height,
		X, Y, Width, Height,
		DIB_RGB_COLORS,
		&BitmapInfo, 
		DIB_RGB_COLORS, SRCCOPY
	);
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
			OutputDebugStringA("WM_SIZE\n");
			//We have to know what the size of window after WM_SIZE by GetClientrect.
			RECT ClientRect;
			GetClientRect(Window, &ClientRect);
			int Height = ClientRect.bottom - ClientRect.top;
			int Width = ClientRect.right - ClientRect.left;
			Win32ResizeDIBection(Width, Height);
			OutputDebugStringA("WM_SIZE\n");
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
			Win32UpdateWindow(DeviceContext, X, Y, Width, Height);
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
		HWND WindowHandle = CreateWindowExA(
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
		if (WindowHandle)
		{
			Running = true;
			while(Running)// most recommend 
			{
				MSG Message;
				BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
				/*
				* Windows reserves the right to call us back at that MainWindowCallback Anytime it wants without DispatchMessage.
				* So anytime we calling to a Windows function, we have to be ready for it protencially to calls back to There(MainWindowCallback).
				* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessage
				* If the function retrieves a message other than WM_QUIT, the return value is nonzero.
				* If the function retrieves the WM_QUIT message, the return value is zero.
				* If there is an error, the return value is -1. For example, the function fails if hWnd is an invalid window handle or lpMsg is an invalid pointer. To get extended error information, call GetLastError.
				* 
				* You need start to thinking: Creation, Destruction, Acquisation and Releasing In Waves.
				*/
				if (MessageResult > 0)
				{
					TranslateMessage(&Message);
					DispatchMessage(&Message);
				}	
				else
				{
					break;
				}
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

/*
* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclientrect
* We have to know what the size of window after WM_SIZE by GetClientrect.
* BOOL GetClientRect(
	  [in]  HWND   hWnd,
	  [out] LPRECT lpRect // long pointer RECT
	);
*
* https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createdibsection
* HBITMAP CreateDIBSection(
	  [in]  HDC              hdc,
	  [in]  const BITMAPINFO *pbmi,
	  [in]  UINT             usage,
	  [out] VOID             **ppvBits,
	  [in]  HANDLE           hSection,
	  [in]  DWORD            offset
	);

* https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchdibits
* int StretchDIBits(
	  [in] HDC              hdc,
	  [in] int              xDest,
	  [in] int              yDest,
	  [in] int              DestWidth,
	  [in] int              DestHeight,
	  [in] int              xSrc,
	  [in] int              ySrc,
	  [in] int              SrcWidth,
	  [in] int              SrcHeight,
	  [in] const VOID       *lpBits,
	  [in] const BITMAPINFO *lpbmi,
	  [in] UINT             iUsage,
	  [in] DWORD            rop
	);
*
* https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfo
* typedef struct tagBITMAPINFO {
	  BITMAPINFOHEADER bmiHeader;
	  RGBQUAD          bmiColors[1];
	} BITMAPINFO, *LPBITMAPINFO, *PBITMAPINFO;
*/

///*
//* Last Day
//*/
//#include<Windows.h>
////typedef is a way to specify a new name for some existing type that can be specified.
////tagWND CLASS
////typedef struct tagWNDCLASSEXA {
////	UINT      cbSize;
////	UINT      style;
////	WNDPROC   lpfnWndProc;
////	int       cbClsExtra;
////  if we want to store extra bytes along with the window class
////	int       cbWndExtra;
////	HINSTANCE hInstance;
////	HICON     hIcon;
////	HCURSOR   hCursor;
////	HBRUSH    hbrBackground;
////	LPCSTR    lpszMenuName;
////	LPCSTR    lpszClassName;
////	HICON     hIconSm;
////} WNDCLASSEXA, * PWNDCLASSEXA, * NPWNDCLASSEXA, * LPWNDCLASSEXA;
//
//LRESULT CALLBACK MainWindowCallback(
//	//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-callwindowproca
//	//WNDPROC lpPrevWndFunc, //?
//	HWND    Window,//HWND    hWnd,
//	// only need WM, no others.
//	UINT    Message,//UINT    Msg,
//	WPARAM  wParam,//WPARAM  wParam,
//	LPARAM  lParam//LPARAM  lParam
//)
//{
//	LRESULT Result = 0;
//	//https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types
//	switch (Message)
//	{
//	case WM_CREATE:
//	{
//		OutputDebugStringA("WM_CREATE\n");
//	} break;
//	case WM_SIZE:
//	{
//		OutputDebugStringA("WM_SIZE\n");
//	} break;
//	case WM_DESTROY:
//	{
//		OutputDebugStringA("WM_DESTROY\n");
//	} break;
//	case WM_CLOSE:
//	{
//		OutputDebugStringA("WM_CLOSE\n");
//	} break;
//	case WM_ACTIVATEAPP:
//	{
//		OutputDebugStringA("WM_ACTIVATEAPP\n");
//	} break;
//	case WM_PAINT:
//	{
//		//https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-paintstruct
//		/*
//		* typedef struct tagPAINTSTRUCT {
//			  HDC  hdc;
//			  BOOL fErase;
//			  RECT rcPaint;
//			  BOOL fRestore;
//			  BOOL fIncUpdate;
//			  BYTE rgbReserved[32];
//			} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;
//		*/
//		// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-beginpaint
//		/*
//		* HDC BeginPaint(
//			  [in]  HWND          hWnd,
//			  [out] LPPAINTSTRUCT lpPaint
//			);
//		*/
//		// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-endpaint
//		/*
//		* BOOL EndPaint(
//			  [in] HWND              hWnd,
//			  [in] const PAINTSTRUCT *lpPaint
//			);
//		*/
//		// https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-patblt
//		/*
//		* BOOL PatBlt(
//			  [in] HDC   hdc,
//			  [in] int   x,
//			  [in] int   y,
//			  [in] int   w,
//			  [in] int   h,
//			  [in] DWORD rop
//			);
//		*/
//		PAINTSTRUCT Paint;
//		HDC DeviceContext = BeginPaint(Window, &Paint);
//		int x = Paint.rcPaint.left;
//		int y = Paint.rcPaint.top;
//		int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
//		int Width = Paint.rcPaint.right - Paint.rcPaint.left;
//		//static DWORD Operation = WHITENESS; //
//		PatBlt(DeviceContext, x, y, Width, Height, WHITENESS);
//		EndPaint(Window, &Paint);
//	} break;
//	default:
//	{
//		//OutputDebugStringA("DEFAULT");
//		Result = DefWindowProc(Window, Message, wParam, lParam);
//	} break;
//	}
//
//	return Result;
//}
//
//int CALLBACK WinMain(
//	HINSTANCE Instance, //HINSTANCE hInstance,
//	HINSTANCE PrevInstance,//HINSTANCE hPrevInstance,
//	LPSTR     CommandLine,//LPSTR     lpCmdLine,
//	int       ShowCode//int       nShowCmd
//)
//{
//	WNDCLASS WindowClass = {};
//
//	WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
//	/*
//	* CS_CLASSDC Allocates one device context to be shared by all windows in the class.
//	* CS_CLASSDC and CS_OWNDC is almost same cause we only have one window.
//	* https://learn.microsoft.com/en-us/windows/win32/winmsg/window-class-styles
//	*/
//	WindowClass.lpfnWndProc = MainWindowCallback;
//	// Pointer to a function we will define how our window responce to events 
//	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-callwindowproca 
//
//	WindowClass.hInstance = Instance; // can call GetModuleHandle();
//	// we have to know which instance is setting window
//
//	WindowClass.lpszClassName = "HademadeHeroWindowClass";
//
//	/*
//	ATOM RegisterClassA(
//		[in] const WNDCLASSA * lpWndClass
//	);
//	https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa
//	*/
//	if (RegisterClass(&WindowClass))
//		// can get a ATOM if you want to register a named property that can associated with windows or other thing
//		// we just want to register a class.
//		// once the register is done, we can create something like windowclass
//	{
//		HWND WindowHandle = CreateWindowExA(
//			0,
//			WindowClass.lpszClassName,
//			"Handmade Hero",
//			WS_OVERLAPPEDWINDOW | WS_VISIBLE,
//			CW_USEDEFAULT,
//			CW_USEDEFAULT,
//			CW_USEDEFAULT,
//			CW_USEDEFAULT,
//			0,
//			0,
//			Instance,
//			0
//		);
//		//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
//		/*
//		HWND CreateWindowExA(
//		  [in]           DWORD     dwExStyle,
//		  [in, optional] LPCSTR    lpClassName,
//		  [in, optional] LPCSTR    lpWindowName,
//		  [in]           DWORD     dwStyle,
//		  [in]           int       X,
//		  [in]           int       Y,
//		  [in]           int       nWidth,
//		  [in]           int       nHeight,
//		  [in, optional] HWND      hWndParent,
//		  [in, optional] HMENU     hMenu,
//		  [in, optional] HINSTANCE hInstance,
//		  [in, optional] LPVOID    lpParam
//		);
//		*/
//		if (WindowHandle)
//		{
//			/*
//			* need to start message loop
//			* Windows wont send message to your window
//			* unless you start pulling them off from the queue.
//			* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessage
//			* BOOL GetMessage(
//				  [out]          LPMSG lpMsg,
//				  [in, optional] HWND  hWnd,
//				  [in]           UINT  wMsgFilterMin,
//				  [in]           UINT  wMsgFilterMax
//				);
//			*/
//			MSG Message;
//			for (;;)
//			{
//				BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
//				if (MessageResult > 0)
//				{
//					// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage
//					TranslateMessage(&Message);
//					// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessage
//					DispatchMessage(&Message);
//				}
//				else
//				{
//					break;
//				}
//			}
//
//			// If the function retrieves a message other than WM_QUIT, the return value is nonzero.
//		}
//		else
//		{
//
//		}
//	}
//	else
//	{
//
//	}
//
//	return 0;
//}
///*
//* debug:
//* 		sizeof(LRESULT)	8	unsigned __int64
//*/
