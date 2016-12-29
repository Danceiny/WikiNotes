---
date: 2016-02-21 16:52
status: public
title: Sunxin-VCpp-Window-Msg
---

# 消息 MSG structure
windows编程的 **消息** 概念非常重要。

- 标准消息
以WM_开头，从CWnd派生的类，都可以接收到。

CWnd是从CCmdTarget派生而来的。

- 命令消息
来自菜单、加速键或工具栏按钮的消息。以WM_COMMAND呈现。

**在MFC中，通过菜单项的标识（ID）来区分不同的命令消息；在SDK中，通过消息的wParam参数识别。**

从CCmdTarget派生的类，都可以接收到。

- 通告消息
由控件产生的消息，例如：按钮的单击，列表框的选择等。为的是向其父窗口（通常是对话框）通知事件的发生。这类消息也是以WM_COMMAND形式呈现。

从CCmdTarget派生的类，都可以接收到。


![](~/12-37-16.jpg)


消息队列。

事件驱动。
# WinMain entry point(Windows)
**Parameters**

- hInstance [in]
Type: HINSTANCE

>A handle to the current instance of the application.

- hPrevInstance [in]
Type: HINSTANCE

>A handle to the previous instance of the application. This parameter is always NULL. If you need to detect whether another instance already exists, create a uniquely named mutex using the CreateMutex function. CreateMutex will succeed even if the mutex already exists, but the function will return ERROR_ALREADY_EXISTS. This indicates that another instance of your application exists, because it created the mutex first. However, a malicious user can create this mutex before you do and prevent your application from starting. To prevent this situation, create a randomly named mutex and store the name so that it can only be obtained by an authorized user. Alternatively, you can use a file for this purpose. To limit your application to one instance per user, create a locked file in the user's profile directory.

- lpCmdLine [in]
Type: LPSTR

>The command line for the application, excluding the program name. To retrieve the entire command line, use the GetCommandLine function.

- nCmdShow [in]
Type: int

>Controls how the window is to be shown. This parameter can be one of the following values.

```c++:n
int WINAPI Window(
	HINSTANCE hInstance,	//	handle to current instance
	HINSTANCE hPrevInstance,	//	handle to previous instance
	LPSTR lpCmdLine,	//	command line
	int nCmdShow	//	show sw_shownoactivate
	)
```

# Windows 窗口程序
## 设计窗口类 WNDCLASS
```c++:n
typedef struct tagWNDCLASS {
  UINT      style;
  WNDPROC   lpfnWndProc;
  int       cbClsExtra;
  int       cbWndExtra;
  HINSTANCE hInstance;
  HICON     hIcon;
  HCURSOR   hCursor;
  HBRUSH    hbrBackground;
  LPCTSTR   lpszMenuName;
  LPCTSTR   lpszClassName;
} WNDCLASS, *PWNDCLASS;
```
**Members**
- style
Type: UINT

>The class style(s). This member can be any combination of the Class Styles.

- lpfnWndProc
Type: WNDPROC

>A pointer to the window procedure. You must use the CallWindowProc function to call the window procedure. For more information, see WindowProc.

- cbClsExtra
Type: int

>The number of extra bytes to allocate following the window-class structure. The system initializes the bytes to zero.

- cbWndExtra
Type: int

>The number of extra bytes to allocate following the window instance. The system initializes the bytes to zero. If an application uses WNDCLASS to register a dialog box created by using the CLASS directive in the resource file, it must set this member to DLGWINDOWEXTRA.

- hInstance
Type: HINSTANCE

>A handle to the instance that contains the window procedure for the class.

- hIcon
Type: HICON

>A handle to the class icon. This member must be a handle to an icon resource. If this member is NULL, the system provides a default icon.

- hCursor
Type: HCURSOR

>A handle to the class cursor. This member must be a handle to a cursor resource. If this member is NULL, an application must explicitly set the cursor shape whenever the mouse moves into the application's window.

- hbrBackground
Type: HBRUSH

>A handle to the class background brush. This member can be a handle to the physical brush to be used for painting the background, or it can be a color value. A color value must be one of the following standard system colors (the value 1 must be added to the chosen color). If a color value is given, you must convert it to one of the following HBRUSH types:
COLOR_ACTIVEBORDER
COLOR_ACTIVECAPTION
COLOR_APPWORKSPACE
COLOR_BACKGROUND
COLOR_BTNFACE
COLOR_BTNSHADOW
COLOR_BTNTEXT
COLOR_CAPTIONTEXT
COLOR_GRAYTEXT
COLOR_HIGHLIGHT
COLOR_HIGHLIGHTTEXT
COLOR_INACTIVEBORDER
COLOR_INACTIVECAPTION
COLOR_MENU
COLOR_MENUTEXT
COLOR_SCROLLBAR
COLOR_WINDOW
COLOR_WINDOWFRAME
COLOR_WINDOWTEXT

>The system automatically deletes class background brushes when the class is unregistered by using UnregisterClass. An application should not delete these brushes.

>When this member is NULL, an application must paint its own background whenever it is requested to paint in its client area. To determine whether the background must be painted, an application can either process the WM_ERASEBKGND message or test the fErase member of the PAINTSTRUCT structure filled by the BeginPaint function.

- lpszMenuName
Type: LPCTSTR

>The resource name of the class menu, as the name appears in the resource file. If you use an integer to identify the menu, use the MAKEINTRESOURCE macro. If this member is NULL, windows belonging to this class have no default menu.

- lpszClassName
Type: LPCTSTR

>A pointer to a null-terminated string or is an atom. If this parameter is an atom, it must be a class atom created by a previous call to the RegisterClass or RegisterClassEx function. The atom must be in the low-order word of lpszClassName; the high-order word must be zero.

>If lpszClassName is a string, it specifies the window class name. The class name can be any name registered with RegisterClass or RegisterClassEx, or any of the predefined control-class names.

>The maximum length for lpszClassName is 256. If lpszClassName is greater than the maximum length, the RegisterClass function will fail.



## 注册窗口类
RegisterClass
```c++:n
ATOM WINAPI RegisterClass(
  _In_ const WNDCLASS *lpWndClass
);
```
向操作系统注册窗口。
## 创建窗口
CreateWindow
```c++:n
HWND WINAPI CreateWindow(
  _In_opt_ LPCTSTR   lpClassName,	//	registered class name
  _In_opt_ LPCTSTR   lpWindowName,	//	window name
  _In_     DWORD     dwStyle,	//	window style
  _In_     int       x,	//	horizontal position of window
  _In_     int       y,	//	vertical position of window
  _In_     int       nWidth,	//	window width
  _In_     int       nHeight,	//	window height
  _In_opt_ HWND      hWndParent,	//	handle to parent or owner window
  _In_opt_ HMENU     hMenu,	//	menu handle or child identifier
  _In_opt_ HINSTANCE hInstance,	//	handle to application instance
  _In_opt_ LPVOID    lpParam	//	window-creation data
);
```
**Parameters**
- lpClassName [in, optional]
Type: LPCTSTR

>A null-terminated string or a class atom created by a previous call to the RegisterClass or RegisterClassEx function. The atom must be in the low-order word of lpClassName; the high-order word must be zero. If lpClassName is a string, it specifies the window class name. The class name can be any name registered with RegisterClass or RegisterClassEx, provided that the module that registers the class is also the module that creates the window. The class name can also be any of the predefined system class names. For a list of system class names, see the Remarks section.

- lpWindowName [in, optional]
Type: LPCTSTR

>The window name. If the window style specifies a title bar, the window title pointed to by lpWindowName is displayed in the title bar. When using CreateWindow to create controls, such as buttons, check boxes, and static controls, use lpWindowName to specify the text of the control. When creating a static control with the SS_ICON style, use lpWindowName to specify the icon name or identifier. To specify an identifier, use the syntax "# num".

- dwStyle [in]
Type: DWORD

>The style of the window being created. This parameter can be a combination of the window style values, plus the control styles indicated in the Remarks section.

- x [in]
Type: int

>The initial horizontal position of the window. For an overlapped or pop-up window, the x parameter is the initial x-coordinate of the window's upper-left corner, in screen coordinates. For a child window, x is the x-coordinate of the upper-left corner of the window relative to the upper-left corner of the parent window's client area. If this parameter is set to CW_USEDEFAULT, the system selects the default position for the window's upper-left corner and ignores the y parameter. CW_USEDEFAULT is valid only for overlapped windows; if it is specified for a pop-up or child window, the x and y parameters are set to zero.

- y [in]
Type: int

>The initial vertical position of the window. For an overlapped or pop-up window, the y parameter is the initial y-coordinate of the window's upper-left corner, in screen coordinates. For a child window, y is the initial y-coordinate of the upper-left corner of the child window relative to the upper-left corner of the parent window's client area. For a list box, y is the initial y-coordinate of the upper-left corner of the list box's client area relative to the upper-left corner of the parent window's client area.

>If an overlapped window is created with the WS_VISIBLE style bit set and the x parameter is set to CW_USEDEFAULT, then the y parameter determines how the window is shown. If the y parameter is CW_USEDEFAULT, then the window manager calls ShowWindow with the SW_SHOW flag after the window has been created. If the y parameter is some other value, then the window manager calls ShowWindow with that value as the nCmdShow parameter.

- nWidth [in]
Type: int

>The width, in device units, of the window. For overlapped windows, nWidth is either the window's width, in screen coordinates, or CW_USEDEFAULT. If nWidth is CW_USEDEFAULT, the system selects a default width and height for the window; the default width extends from the initial x-coordinate to the right edge of the screen, and the default height extends from the initial y-coordinate to the top of the icon area. CW_USEDEFAULT is valid only for overlapped windows; if CW_USEDEFAULT is specified for a pop-up or child window, nWidth and nHeight are set to zero.

- nHeight [in]
Type: int

>The height, in device units, of the window. For overlapped windows, nHeight is the window's height, in screen coordinates. If nWidth is set to CW_USEDEFAULT, the system ignores nHeight.

- hWndParent [in, optional]
Type: HWND

>A handle to the parent or owner window of the window being created. To create a child window or an owned window, supply a valid window handle. This parameter is optional for pop-up windows.

>To create a message-only window, supply HWND_MESSAGE or a handle to an existing message-only window.

- hMenu [in, optional]
Type: HMENU

>A handle to a menu, or specifies a child-window identifier depending on the window style. For an overlapped or pop-up window, hMenu identifies the menu to be used with the window; it can be NULL if the class menu is to be used. For a child window, hMenu specifies the child-window identifier, an integer value used by a dialog box control to notify its parent about events. The application determines the child-window identifier; it must be unique for all child windows with the same parent window.

- hInstance [in, optional]
Type: HINSTANCE

>A handle to the instance of the module to be associated with the window.

- lpParam [in, optional]
Type: LPVOID

>A pointer to a value to be passed to the window through the CREATESTRUCT structure ( lpCreateParams member) pointed to by the lParam param of the WM_CREATE message. This message is sent to the created window by this function before it returns.

>If an application calls CreateWindow to create a MDI client window, lpParam should point to a CLIENTCREATESTRUCT structure. If an MDI client window calls CreateWindow to create an MDI child window, lpParam should point to a MDICREATESTRUCT structure. lpParam may be NULL if no additional data is needed.

**Return value**

Type:

*Type: HWND*


If the function succeeds, the return value is a handle to the new window.

If the function fails, the return value is NULL. To get extended error information, call GetLastError.

This function typically fails for one of the following reasons:

- an invalid parameter value
- the system class was registered by a different module
- The WH_CBT hook is installed and returns a failure code
- if one of the controls in the dialog template is not registered, or its window
- window procedure fails WM_CREATE or WM_NCCREATE

## 显示窗口
ShowWindow
```c++:n
BOOL WINAPI ShowWindow(
  _In_ HWND hWnd,
  _In_ int  nCmdShow
);
```
## 更新窗口
UpdateWindow
可有可无
## 消息循环
GetMessage
```c++:n
BOOL WINAPI GetMessage(
  _Out_    LPMSG lpMsg,	//	message information
  _In_opt_ HWND  hWnd,	//	handle to window
  _In_     UINT  wMsgFilterMin,	//	first message
  _In_     UINT  wMsgFilterMax	//	last message
);
```

```c++:n
MSG msg;
while(GetMessage(&msg,NULL,0,0))
{
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}
```

## TranslateMessage
>Translates virtual-key messages into character messages. The character messages are posted to the calling thread's message queue, to be read the next time the thread calls the GetMessage or PeekMessage function.
```c++:n
BOOL WINAPI TranslateMessage(
  _In_ const MSG *lpMsg
);
```
## DispatchMessage
>Dispatches a message to a window procedure. It is typically used to dispatch a message retrieved by the GetMessage function.
```c++:n
LRESULT WINAPI DispatchMessage(
  _In_ const MSG *lpmsg
);
```
## WindowProc
>An application-defined function that processes messages sent to a window. The WNDPROC type defines a pointer to this callback function.

>WindowProc is a placeholder for the application-defined function name.

```c++:n
LRESULT CALLBACK WindowProc(
  _In_ HWND   hwnd, // handle to window
  _In_ UINT   uMsg,	// message identifier
  _In_ WPARAM wParam,	//	first message parameter
  _In_ LPARAM lParam	//	second message parameter
);
```

## 响应WM_PAINT
- BeginPaint
- EndPaint

## DC
- GetDC
- ReleaseDC

## TextOut
Writes a character string at the specified location using the currently selected font.
```c++:n
virtual BOOL TextOut(
   int x,
   int y,
   LPCTSTR lpszString,
   int nCount  
);
BOOL TextOut(
   int x,
   int y,
   const CString& str
);
```
## cdecl

Microsoft Specific

>`__cdecl` is the default calling convention for C and C++ programs. Because the stack is cleaned up by the caller, it can do vararg functions. The `__cdecl` calling convention creates larger executables than `__stdcall`, because it requires each function call to include stack cleanup code. The following list shows the implementation of this calling convention.

## stdcall
Microsoft Specific

The `__stdcall` calling convention is used to call Win32 API functions. The callee cleans the stack, so the compiler makes vararg functions `__cdecl`. Functions that use this calling convention require a function prototype.


# Steps
1. win32 application
2. emtpy project

下面仿照视频中所写。
对于代码中双引号内的字符串，修改前比较凌乱（无特意对应），运行后没有看到窗口，也没有任何显示。任务管理器可见。
修改如下后，正常。
```c++:n
# include <windows.h>
# include <stdio.h>

LRESULT CALLBACK WinSunProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	WNDCLASS wndcls;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndcls.hCursor = LoadCursor(NULL, IDC_CROSS);
	wndcls.hIcon = LoadIcon(NULL, ERROR);
	wndcls.hInstance = hInstance;
	wndcls.lpfnWndProc = WinSunProc;
	wndcls.lpszClassName = "danceiny";
	wndcls.lpszMenuName = NULL;
	wndcls.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wndcls);

	HWND hwnd;
	hwnd = CreateWindow("danceiny", "diane", WS_OVERLAPPEDWINDOW, 0, 0, 500, 400, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
LRESULT CALLBACK WinSunProc(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
	)
{
	switch (uMsg)
	{
	case WM_CHAR:
		char szChar[20];
		sprintf(szChar, "char is %d", wParam);
		MessageBox(hwnd, szChar, "danceiny", 0);
		break;
	case WM_LBUTTONDOWN:
		MessageBox(hwnd, "mouse clicked", "danceiny", 0);
		HDC hdc;
		hdc = GetDC(hwnd);
		TextOut(hdc, 0, 40, "danceiny", strlen("danceiny"));
		ReleaseDC(hwnd, hdc);
		break;
	case WM_PAINT:
		HDC hDC;
		PAINTSTRUCT ps;
		hDC = BeginPaint(hwnd, &ps);
		TextOut(hDC, 0, 0, "diane", strlen("diane"));
		EndPaint(hwnd, &ps);
		break;
	case WM_CLOSE:
		if (IDYES == MessageBox(hwnd, "end it?", "danceiny", MB_YESNO)){
			DestroyWindow(hwnd);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
```

下面的是winsdk中的demo1的hello_world.c。正常编译通过输出。
```c:n
////标准的C语言
//
////main()
////int main()
//int main(int argc, char *argv[])
//{
//	printf("hello,world\n");
//	return 0;
//}

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   PSTR szCmdLine,int iCmdShow)
{
	MessageBox(NULL,TEXT("Hello, Windows 程序设计!"),
		TEXT("HelloMsg - 这是标题"),
		MB_OK | MB_ICONINFORMATION);

	return 0;
}
``