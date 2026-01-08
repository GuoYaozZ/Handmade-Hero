/*
* 
* Date: 2026-01-07
*/
#include<Windows.h>
//typedef is a way to specify a new name for some existing type that can be specified.
//tagWND CLASS
//typedef struct tagWNDCLASSEXA {
//	UINT      cbSize;
//	UINT      style;
//	WNDPROC   lpfnWndProc;
//	int       cbClsExtra;
//  if we want to store extra bytes along with the window class
//	int       cbWndExtra;
//	HINSTANCE hInstance;
//	HICON     hIcon;
//	HCURSOR   hCursor;
//	HBRUSH    hbrBackground;
//	LPCSTR    lpszMenuName;
//	LPCSTR    lpszClassName;
//	HICON     hIconSm;
//} WNDCLASSEXA, * PWNDCLASSEXA, * NPWNDCLASSEXA, * LPWNDCLASSEXA;

LRESULT CALLBACK MainWindowCallback(
	//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-callwindowproca
	//WNDPROC lpPrevWndFunc, //?
	HWND    Window,//HWND    hWnd,
	// only need WM, no others.
	UINT    Message,//UINT    Msg,
	WPARAM  wParam,//WPARAM  wParam,
	LPARAM  lParam//LPARAM  lParam
)
{
	LRESULT Result = 0;
    //https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types
	switch (Message) 
	{
		case WM_CREATE:
		{
			OutputDebugStringA("WM_CREATE\n");
		} break;
		case WM_SIZE: 
		{
			OutputDebugStringA("WM_SIZE\n");
		} break;
		case WM_DESTROY: 
		{
			OutputDebugStringA("WM_DESTROY\n");
		} break;
		case WM_CLOSE: 
		{
			OutputDebugStringA("WM_CLOSE\n");
		} break;
		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA("WM_ACTIVATEAPP\n");
		} break;
		case WM_PAINT: 
		{
			//https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-paintstruct
			/*
			* typedef struct tagPAINTSTRUCT {
				  HDC  hdc;
				  BOOL fErase;
				  RECT rcPaint;
				  BOOL fRestore;
				  BOOL fIncUpdate;
				  BYTE rgbReserved[32];
				} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;
			*/
			// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-beginpaint
			/*
			* HDC BeginPaint(
				  [in]  HWND          hWnd,
				  [out] LPPAINTSTRUCT lpPaint
				);
			*/
			// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-endpaint
			/*
			* BOOL EndPaint(
				  [in] HWND              hWnd,
				  [in] const PAINTSTRUCT *lpPaint
				);
			*/
			// https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-patblt
			/*
			* BOOL PatBlt(
				  [in] HDC   hdc,
				  [in] int   x,
				  [in] int   y,
				  [in] int   w,
				  [in] int   h,
				  [in] DWORD rop
				);
			*/
			PAINTSTRUCT Paint;
			HDC DeviceContext = BeginPaint(Window, &Paint);
			int x = Paint.rcPaint.left;
			int y = Paint.rcPaint.top;
			int Height = Paint.rcPaint.bottom - Paint.rcPaint.top;
			int Width = Paint.rcPaint.right - Paint.rcPaint.left;
			//static DWORD Operation = WHITENESS; //
			PatBlt(DeviceContext, x, y, Width, Height, WHITENESS);
			EndPaint(Window, &Paint);
		} break;
		default:
		{
			//OutputDebugStringA("DEFAULT");
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
	/*
	* CS_CLASSDC Allocates one device context to be shared by all windows in the class.
	* CS_CLASSDC and CS_OWNDC is almost same cause we only have one window.
	* https://learn.microsoft.com/en-us/windows/win32/winmsg/window-class-styles
	*/
	WindowClass.lpfnWndProc = MainWindowCallback;
	// Pointer to a function we will define how our window responce to events 
	// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-callwindowproca 

	WindowClass.hInstance = Instance; // can call GetModuleHandle();
	// we have to know which instance is setting window

	WindowClass.lpszClassName = "HademadeHeroWindowClass";

	/*
	ATOM RegisterClassA(
		[in] const WNDCLASSA * lpWndClass
	);
	https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa
	*/
	if (RegisterClass(&WindowClass))
		// can get a ATOM if you want to register a named property that can associated with windows or other thing
		// we just want to register a class.
		// once the register is done, we can create something like windowclass
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
		//https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
		/*
		HWND CreateWindowExA(
		  [in]           DWORD     dwExStyle,
		  [in, optional] LPCSTR    lpClassName,
		  [in, optional] LPCSTR    lpWindowName,
		  [in]           DWORD     dwStyle,
		  [in]           int       X,
		  [in]           int       Y,
		  [in]           int       nWidth,
		  [in]           int       nHeight,
		  [in, optional] HWND      hWndParent,
		  [in, optional] HMENU     hMenu,
		  [in, optional] HINSTANCE hInstance,
		  [in, optional] LPVOID    lpParam
		);
		*/
		if (WindowHandle)
		{
			/*
			* need to start message loop
			* Windows wont send message to your window 
			* unless you start pulling them off from the queue.
			* https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessage
			* BOOL GetMessage(
				  [out]          LPMSG lpMsg,
				  [in, optional] HWND  hWnd,
				  [in]           UINT  wMsgFilterMin,
				  [in]           UINT  wMsgFilterMax
				);
			*/
			MSG Message;
			for (;;)
			{
				BOOL MessageResult = GetMessage(&Message, 0, 0, 0);
				if (MessageResult > 0)
				{
					// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage
					TranslateMessage(&Message);
					// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessage
					DispatchMessage(&Message);
				}	
				else
				{
					break;
				}
			}
				
			// If the function retrieves a message other than WM_QUIT, the return value is nonzero.
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
* debug:
* 		sizeof(LRESULT)	8	unsigned __int64
*/
