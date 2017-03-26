---
date: 2016-02-21 16:55
status: public
title: winsdk-lesson2
---

# 匈牙利标记法
前缀表示变量类型。

|前缀|含义|
|----|------|----|
|sz|以\0字符结束的字符串|
|h|handler类型|
|lp||
|n|short类型|
|cx|窗口x轴|
|cy|窗口y轴|
|DT_ |draw text |
|IDC_ |设备|
|MB_ |message box|
WM_ |windows message|
|WS_ |windows style|

# 新的数据类型（defined by MS）

|NEW|	OLD|
|-----|------|
|UINT |unsigned int|
|PSTR| char*|
|WNDCLASSEX| |
|MSG||
|RECT||
|TEXT||

-EX后缀的是扩展的。

# 文本输出
## GDI函数：图形设备接口函数
- DrawText()
- TextOut()


## DC：设备环境
## 获取设备环境句柄（HDC）
- 方法1
```c++:n
case:WM_PAINT:
	hdc = BeginPaint(hwnd,&ps);
	...
	EndPaint(hwnd,&ps);
```

- 方法2
```c++:n
hdc = GetDC(hwnd);
...
ReleaseDC(hwnd,hdc);
```

应用示例：
```c++:n
// demo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "demo.h"

#define MAX_LOADSTRING 100
#define LINEHEIGHT 15

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMO));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_DEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szBuffer[1024];

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...

		for(int i = 0; i < 100; ++i)
		{
			_sntprintf(szBuffer,1024,TEXT("Hello %d"),i+1);
			TextOut(hdc,0,i*LINEHEIGHT,szBuffer,lstrlen(szBuffer));
		}

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
```

# 系统字体
- GetSystemMetrics()
1920 * 1080
- GetTextMetrics()
- TEXTMETRICS结构
文本度量。

**ACCEL structure**
Defines an accelerator key used in an accelerator table.

```c++:n
typedef struct tagACCEL {
  BYTE fVirt;
  WORD key;
  WORD cmd;
} ACCEL, *LPACCEL;
```

代码示例：
```c++:n
// demo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "demo.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMO));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL; //MAKEINTRESOURCE(IDC_DEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cyChar; //字符高度，行高，行距
	static int cxChar;
	static int cxCaps;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TEXTMETRIC tm;
	TCHAR szBuffer[40];
	int iLength;

	static int cxScreen, cyScreen;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc,&tm);
		cxChar = tm.tmAveCharWidth;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		cxCaps = ((tm.tmPitchAndFamily & 1) ? 3 : 2) * cxChar / 2;//等宽字体检测得到2（等宽）或者3（不等宽）
		ReleaseDC(hWnd,hdc);

		cxScreen = GetSystemMetrics(SM_CXSCREEN);
		cyScreen = GetSystemMetrics(SM_CYSCREEN);

		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...

		TextOut(hdc,0,0,TEXT("SM_CXSCREEN"),lstrlen(TEXT("SM_CXSCREEN")));
		TextOut(hdc,22*cxCaps,0,TEXT("Screen width in pixels"),lstrlen(TEXT("Screen width in pixels")));

		SetTextAlign(hdc,TA_RIGHT|TA_TOP);

		iLength = wsprintf(szBuffer,TEXT("%d"),cxScreen);
		//Get 1920 buffer
		TextOut(hdc,22*cxCaps+40*cxChar,0,szBuffer,iLength);

		SetTextAlign(hdc,TA_LEFT|TA_TOP);

		TextOut(hdc,0,cyChar,TEXT("SM_CYSCREEN"),lstrlen(TEXT("SM_CYSCREEN")));
		TextOut(hdc,22*cxCaps,cyChar,TEXT("Screen height in pixels"),lstrlen(TEXT("Screen height in pixels")));

		SetTextAlign(hdc,TA_RIGHT|TA_TOP);

		iLength = wsprintf(szBuffer,TEXT("%d"),cyScreen);
		//Get 1080 buffer
		TextOut(hdc,22*cxCaps+40*cxChar,cyChar,szBuffer,iLength);

		SetTextAlign(hdc,TA_LEFT|TA_TOP);

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
```

# 滚动条
## 滚动条的范围和位置
- SetScrollRange()
- SetScrollPos()

## 滚动条消息的通知码
- SB_LINEUP
- SB_PAGEUP
- SB_THUMBERTACK
- SB_TOP
- SB_ENDSCROLL

创建窗口时，选择滚动条；

过程处理函数中，处理滚动条消息。

有VSCROLL（纵vertical）和HSCROLL（横horizontal）。

滚动条位置移动需要自己写代码。

滚动条能动了，但是窗口显示内容不动。

又要自己写代码。

```c++:n
case WM_PAINT:
	hdc = BeginPaint(hWnd, &ps);
	// TODO: 在此添加任意绘图代码...

	TCHAR szBuffer[1024];

	for(int i=0; i<MAX_LINE; ++i)
	{
		x = cyChar * (i - iHscrollPos);//这里只是一个测试，实际显示效果很有趣，但不实际
		y = cyChar * (i - iVscrollPos);
		_sntprintf(szBuffer,1024,TEXT("hello %d"),i+1);
		TextOut(hdc,x,y,szBuffer,lstrlen(szBuffer));
	}
	EndPaint(hWnd, &ps);
	break;
```



示例代码：
```c++:n
// demo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "demo.h"

#define MAX_LOADSTRING 100
#define MAX_LINE 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMO));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_DEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	static int cyChar;
	TEXTMETRIC tm;
	int x,y;
	static int iVscrollPos;
	static int cyClient;

	switch (message)
	{
	case WM_SIZE:
		//调整窗口大小时，客户区大小发生变化。
		cyClient = HIWORD(lParam);
		break;
	case WM_CREATE:
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc,&tm);
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hWnd,hdc);

		iVscrollPos = 0;
		SetScrollRange(hWnd,SB_VERT,0,MAX_LINE-1,FALSE);
		SetScrollPos(hWnd,SB_VERT,iVscrollPos,TRUE);

		return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...

		TCHAR szBuffer[1024];

		for(int i=0; i<MAX_LINE; ++i)
		{
			x = 0;
			y = cyChar * (i - iVscrollPos);
			_sntprintf(szBuffer,1024,TEXT("hello %d"),i+1);
			TextOut(hdc,x,y,szBuffer,lstrlen(szBuffer));
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_VSCROLL:
		switch(LOWORD(wParam))
		{
		case SB_LINEUP:
			iVscrollPos -= 1;
			break;
		case SB_LINEDOWN:
			iVscrollPos += 1;
			break;
		case SB_PAGEDOWN:
			iVscrollPos += cyClient/cyChar;
			break;
		case SB_PAGEUP:
			iVscrollPos -= cyClient/cyChar;
			break;
		case SB_THUMBPOSITION:
			iVscrollPos = HIWORD(wParam);
			break;
		default:
			break;
		}

		iVscrollPos = max(0,min(iVscrollPos,MAX_LINE -1));

		if(iVscrollPos != GetScrollPos(hWnd,SB_VERT))
		{
			SetScrollPos(hWnd,SB_VERT,iVscrollPos,TRUE);
			InvalidateRect(hWnd,NULL,TRUE);//NULL重画,TRUE背景全部擦除
		}

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
```

我给他加上了水平滚动条。

注意diffrence添加的部分。

```c++:n
// demo.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "demo.h"

#define MAX_LOADSTRING 100
#define MAX_LINE 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMO));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_DEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   //水平滚动条
   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	static int cyChar;

	TEXTMETRIC tm;
	int x,y;
	static int iVscrollPos;
	static int iHscrollPos;		//水平滚动条
	static int cyClient;
	static int cxClient;		//水平滚动条

	switch (message)
	{
	case WM_SIZE:
		cyClient = HIWORD(lParam);

		//水平滚动条
		cxClient = HIWORD(wParam);

		break;
	case WM_CREATE:
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc,&tm);
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hWnd,hdc);

		iVscrollPos = 0;
		SetScrollRange(hWnd,SB_VERT,0,MAX_LINE-1,FALSE);
		SetScrollPos(hWnd,SB_VERT,iVscrollPos,TRUE);

		return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...

		TCHAR szBuffer[1024];

		for(int i=0; i<MAX_LINE; ++i)
		{
			x = 0;
			y = cyChar * (i - iVscrollPos);
			_sntprintf(szBuffer,1024,TEXT("hello %d"),i+1);
			TextOut(hdc,x,y,szBuffer,lstrlen(szBuffer));
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_VSCROLL:
		switch(LOWORD(wParam))
		{
		case SB_LINEUP:
			iVscrollPos -= 1;
			break;
		case SB_LINEDOWN:
			iVscrollPos += 1;
			break;
		case SB_PAGEDOWN:
			iVscrollPos += cyClient/cyChar;
			break;
		case SB_PAGEUP:
			iVscrollPos -= cyClient/cyChar;
			break;
		case SB_THUMBPOSITION:
			iVscrollPos = HIWORD(wParam);
			break;
		default:
			break;
		}
		iVscrollPos = max(0, min(iVscrollPos, MAX_LINE - 1));

		if (iVscrollPos != GetScrollPos(hWnd, SB_VERT))
		{
			SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

		//强行添加水平滚动条
	case WM_HSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINERIGHT:
			iHscrollPos -= 1;
			break;
		case SB_LINELEFT:
			iHscrollPos += 1;
			break;
		case SB_PAGELEFT:
			iHscrollPos += cxClient / cyChar;
			break;
		case SB_PAGERIGHT:
			iHscrollPos -= cxClient / cyChar;
			break;
		case SB_THUMBPOSITION:
			iHscrollPos = HIWORD(wParam);
			break;
		default:
			break;
		}
		iHscrollPos = max(0, min(iHscrollPos, MAX_LINE - 1));

		if (iHscrollPos != GetScrollPos(hWnd, SB_HORZ))
		{
			SetScrollPos(hWnd, SB_HORZ, iHscrollPos, TRUE);
			InvalidateRect(hWnd, NULL, TRUE);
		}

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
```

# SYSMETS
项目文件结构：
- SYSMETS
	- 头文件
		- data.h
		- Resource.h
		- stdafx.h
		- SYSMETS.h
		- targetver.h
	- 外部依赖项
	- 源文件
		- stdafx.cpp
		- SYSMETS.cpp
	- 资源文件
		- ReadMe.txt

ReadMe.txt
```
========================================================================
	WIN32 应用程序：SYSMETS 项目概述
========================================================================

应用程序向导已为您创建了此 SYSMETS 应用程序。

本文件概要介绍组成 SYSMETS 应用程序的
的每个文件的内容。


SYSMETS.vcproj
	这是使用应用程序向导生成的 VC++ 项目的主项目文件，
	其中包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

SYSMETS.cpp
	这是主应用程序源文件。

/////////////////////////////////////////////////////////////////////////////
应用程序向导创建了下列资源：

SYSMETS.rc
这是程序使用的所有 Microsoft Windows 资源的列表。它包括 RES 子目录中存储的图标、位图和光标。
此文件可以直接在 Microsoft Visual C++ 中进行编辑。

Resource.h
	这是标准头文件，可用于定义新的资源 ID。
	Microsoft Visual C++ 将读取并更新此文件。

SYSMETS.ico
	这是一个图标文件，可用作应用程序的图标 (32x32)。
	此图标包括在主资源文件 SYSMETS.rc 中。

small.ico
	这是一个图标文件，其中包含应用程序的图标的较小版本 (16x16)。此图标包括在主资源文件 SYSMETS.rc 中。

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

StdAfx.h, StdAfx.cpp
	这些文件用于生成名为 SYSMETS.pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

/////////////////////////////////////////////////////////////////////////////
其他注释：

应用程序向导使用“TODO:”注释来指示应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////
```

data.h
```c++:n
#define NUMLINES ((int)(sizeof sysmetrics / sizeof sysmetrics[0]))

struct
{
	int		iIndex;
	TCHAR * szLabel;
	TCHAR * szDesc;
}
sysmetrics[] =
{
	 SM_CXSCREEN,             TEXT ("SM_CXSCREEN"),              
							  TEXT ("Screen width in pixels"),
	 SM_CYSCREEN,             TEXT ("SM_CYSCREEN"),              
							  TEXT ("Screen height in pixels"),
	 SM_CXVSCROLL,            TEXT ("SM_CXVSCROLL"),             
							  TEXT ("Vertical scroll width"),
	 SM_CYHSCROLL,            TEXT ("SM_CYHSCROLL"),             
							  TEXT ("Horizontal scroll height"),
	 SM_CYCAPTION,            TEXT ("SM_CYCAPTION"),             
							  TEXT ("Caption bar height"),
	 SM_CXBORDER,             TEXT ("SM_CXBORDER"),              
							  TEXT ("Window border width"),
	 SM_CYBORDER,             TEXT ("SM_CYBORDER"),              
							  TEXT ("Window border height"),
	 SM_CXFIXEDFRAME,         TEXT ("SM_CXFIXEDFRAME"),          
							  TEXT ("Dialog window frame width"),
	 SM_CYFIXEDFRAME,         TEXT ("SM_CYFIXEDFRAME"),          
							  TEXT ("Dialog window frame height"),
	 SM_CYVTHUMB,             TEXT ("SM_CYVTHUMB"),              
							  TEXT ("Vertical scroll thumb height"),
	 SM_CXHTHUMB,             TEXT ("SM_CXHTHUMB"),              
							  TEXT ("Horizontal scroll thumb width"),
	 SM_CXICON,               TEXT ("SM_CXICON"),                
							  TEXT ("Icon width"),
	 SM_CYICON,               TEXT ("SM_CYICON"),                
							  TEXT ("Icon height"),
	 SM_CXCURSOR,             TEXT ("SM_CXCURSOR"),              
							  TEXT ("Cursor width"),
	 SM_CYCURSOR,             TEXT ("SM_CYCURSOR"),              
							  TEXT ("Cursor height"),
	 SM_CYMENU,               TEXT ("SM_CYMENU"),                
							  TEXT ("Menu bar height"),
	 SM_CXFULLSCREEN,         TEXT ("SM_CXFULLSCREEN"),          
							  TEXT ("Full screen client area width"),
	 SM_CYFULLSCREEN,         TEXT ("SM_CYFULLSCREEN"),          
							  TEXT ("Full screen client area height"),
	 SM_CYKANJIWINDOW,        TEXT ("SM_CYKANJIWINDOW"),         
							  TEXT ("Kanji window height"),
	 SM_MOUSEPRESENT,         TEXT ("SM_MOUSEPRESENT"),          
							  TEXT ("Mouse present flag"),
	 SM_CYVSCROLL,            TEXT ("SM_CYVSCROLL"),             
							  TEXT ("Vertical scroll arrow height"),
	 SM_CXHSCROLL,            TEXT ("SM_CXHSCROLL"),             
							  TEXT ("Horizontal scroll arrow width"),
	 SM_DEBUG,                TEXT ("SM_DEBUG"),                 
							  TEXT ("Debug version flag"),
	 SM_SWAPBUTTON,           TEXT ("SM_SWAPBUTTON"),            
							  TEXT ("Mouse buttons swapped flag"),
	 SM_CXMIN,                TEXT ("SM_CXMIN"),                 
							  TEXT ("Minimum window width"),
	 SM_CYMIN,                TEXT ("SM_CYMIN"),                 
							  TEXT ("Minimum window height"),
	 SM_CXSIZE,               TEXT ("SM_CXSIZE"),                
							  TEXT ("Min/Max/Close button width"),
	 SM_CYSIZE,               TEXT ("SM_CYSIZE"),                
							  TEXT ("Min/Max/Close button height"),
	 SM_CXSIZEFRAME,          TEXT ("SM_CXSIZEFRAME"),           
							  TEXT ("Window sizing frame width"),
	 SM_CYSIZEFRAME,          TEXT ("SM_CYSIZEFRAME"),           
							  TEXT ("Window sizing frame height"),
	 SM_CXMINTRACK,           TEXT ("SM_CXMINTRACK"),            
							  TEXT ("Minimum window tracking width"),
	 SM_CYMINTRACK,           TEXT ("SM_CYMINTRACK"),            
							  TEXT ("Minimum window tracking height"),
	 SM_CXDOUBLECLK,          TEXT ("SM_CXDOUBLECLK"),           
							  TEXT ("Double click x tolerance"),
	 SM_CYDOUBLECLK,          TEXT ("SM_CYDOUBLECLK"),           
							  TEXT ("Double click y tolerance"),
	 SM_CXICONSPACING,        TEXT ("SM_CXICONSPACING"),         
							  TEXT ("Horizontal icon spacing"),
	 SM_CYICONSPACING,        TEXT ("SM_CYICONSPACING"),         
							  TEXT ("Vertical icon spacing"),
	 SM_MENUDROPALIGNMENT,    TEXT ("SM_MENUDROPALIGNMENT"),     
							  TEXT ("Left or right menu drop"),
	 SM_PENWINDOWS,           TEXT ("SM_PENWINDOWS"),            
							  TEXT ("Pen extensions installed"),
	 SM_DBCSENABLED,          TEXT ("SM_DBCSENABLED"),           
							  TEXT ("Double-Byte Char Set enabled"),
	 SM_CMOUSEBUTTONS,        TEXT ("SM_CMOUSEBUTTONS"),         
							  TEXT ("Number of mouse buttons"),
	 SM_SECURE,               TEXT ("SM_SECURE"),                
							  TEXT ("Security present flag"),
	 SM_CXEDGE,               TEXT ("SM_CXEDGE"),                
							  TEXT ("3-D border width"),
	 SM_CYEDGE,               TEXT ("SM_CYEDGE"),                
							  TEXT ("3-D border height"),
	 SM_CXMINSPACING,         TEXT ("SM_CXMINSPACING"),          
							  TEXT ("Minimized window spacing width"),
	 SM_CYMINSPACING,         TEXT ("SM_CYMINSPACING"),          
							  TEXT ("Minimized window spacing height"),
	 SM_CXSMICON,             TEXT ("SM_CXSMICON"),              
							  TEXT ("Small icon width"),
	 SM_CYSMICON,             TEXT ("SM_CYSMICON"),              
							  TEXT ("Small icon height"),
	 SM_CYSMCAPTION,          TEXT ("SM_CYSMCAPTION"),           
							  TEXT ("Small caption height"),
	 SM_CXSMSIZE,             TEXT ("SM_CXSMSIZE"),              
							  TEXT ("Small caption button width"),
	 SM_CYSMSIZE,             TEXT ("SM_CYSMSIZE"),              
							  TEXT ("Small caption button height"),
	 SM_CXMENUSIZE,           TEXT ("SM_CXMENUSIZE"),            
							  TEXT ("Menu bar button width"),
	 SM_CYMENUSIZE,           TEXT ("SM_CYMENUSIZE"),            
							  TEXT ("Menu bar button height"),
	 SM_ARRANGE,              TEXT ("SM_ARRANGE"),               
							  TEXT ("How minimized windows arranged"),
	 SM_CXMINIMIZED,          TEXT ("SM_CXMINIMIZED"),           
							  TEXT ("Minimized window width"),
	 SM_CYMINIMIZED,          TEXT ("SM_CYMINIMIZED"),           
							  TEXT ("Minimized window height"),
	 SM_CXMAXTRACK,           TEXT ("SM_CXMAXTRACK"),            
							  TEXT ("Maximum draggable width"),
	 SM_CYMAXTRACK,           TEXT ("SM_CYMAXTRACK"),            
							  TEXT ("Maximum draggable height"),
	 SM_CXMAXIMIZED,          TEXT ("SM_CXMAXIMIZED"),           
							  TEXT ("Width of maximized window"),
	 SM_CYMAXIMIZED,          TEXT ("SM_CYMAXIMIZED"),           
							  TEXT ("Height of maximized window"),
	 SM_NETWORK,              TEXT ("SM_NETWORK"),               
							  TEXT ("Network present flag"),
	 SM_CLEANBOOT,            TEXT ("SM_CLEANBOOT"),             
							  TEXT ("How system was booted"),
	 SM_CXDRAG,               TEXT ("SM_CXDRAG"),                
							  TEXT ("Avoid drag x tolerance"),
	 SM_CYDRAG,               TEXT ("SM_CYDRAG"),                
							  TEXT ("Avoid drag y tolerance"),
	 SM_SHOWSOUNDS,           TEXT ("SM_SHOWSOUNDS"),            
							  TEXT ("Present sounds visually"),
	 SM_CXMENUCHECK,          TEXT ("SM_CXMENUCHECK"),           
							  TEXT ("Menu check-mark width"),
	 SM_CYMENUCHECK,          TEXT ("SM_CYMENUCHECK"),           
							  TEXT ("Menu check-mark height"),
	 SM_SLOWMACHINE,          TEXT ("SM_SLOWMACHINE"),           
							  TEXT ("Slow processor flag"),
	 SM_MIDEASTENABLED,       TEXT ("SM_MIDEASTENABLED"),        
							  TEXT ("Hebrew and Arabic enabled flag"),
	 SM_MOUSEWHEELPRESENT,    TEXT ("SM_MOUSEWHEELPRESENT"),     
							  TEXT ("Mouse wheel present flag"),
	 SM_XVIRTUALSCREEN,       TEXT ("SM_XVIRTUALSCREEN"),        
							  TEXT ("Virtual screen x origin"),
	 SM_YVIRTUALSCREEN,       TEXT ("SM_YVIRTUALSCREEN"),        
							  TEXT ("Virtual screen y origin"),
	 SM_CXVIRTUALSCREEN,      TEXT ("SM_CXVIRTUALSCREEN"),       
							  TEXT ("Virtual screen width"),
	 SM_CYVIRTUALSCREEN,      TEXT ("SM_CYVIRTUALSCREEN"),       
							  TEXT ("Virtual screen height"),
	 SM_CMONITORS,            TEXT ("SM_CMONITORS"),             
							  TEXT ("Number of monitors"),
	 SM_SAMEDISPLAYFORMAT,    TEXT ("SM_SAMEDISPLAYFORMAT"),     
							  TEXT ("Same color format flag")
};
```
SYSMETS.cpp
```c++:n
// SYSMETS.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SYSMETS.h"
#include "data.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SYSMETS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SYSMETS));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SYSMETS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_SYSMETS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, TEXT("Get System Metrics No.1"), WS_OVERLAPPEDWINDOW,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxChar, cxCaps, cyChar;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szBuffer[10];
	TEXTMETRIC tm;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc,&tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hWnd,hdc);
		return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		for(int i=0; i<NUMLINES; ++i)
		{
			TextOut(hdc,0,i*cyChar,sysmetrics[i].szLabel,lstrlen(sysmetrics[i].szLabel));
			TextOut(hdc,22*cxCaps,i*cyChar,sysmetrics[i].szDesc,lstrlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc,TA_RIGHT | TA_TOP);
			TextOut(hdc,22*cxCaps + 40 * cxChar,i*cyChar,szBuffer,wsprintf(szBuffer,TEXT("%5d"),GetSystemMetrics(sysmetrics[i].iIndex)));

			SetTextAlign(hdc,TA_LEFT | TA_TOP);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
```

# 加滚动条
```c++:n
// SYSMETS.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SYSMETS.h"
#include "data.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SYSMETS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SYSMETS));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SYSMETS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_SYSMETS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, TEXT("Get System Metrics No.2"), WS_OVERLAPPEDWINDOW|WS_VSCROLL,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxChar, cxCaps, cyChar, iVScrollPos, cyClient;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szBuffer[10];
	TEXTMETRIC tm;
	int y;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc,&tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hWnd,hdc);

		iVScrollPos = 0;

		SetScrollRange(hWnd,SB_VERT,0,NUMLINES-1,FALSE);
		SetScrollPos(hWnd,SB_VERT,iVScrollPos,TRUE);

		return 0;
	case WM_SIZE:
		cyClient = HIWORD(lParam);
		return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		for(int i=0; i<NUMLINES; ++i)
		{
			y = cyChar * (i - iVScrollPos);
			TextOut(hdc,0,y,sysmetrics[i].szLabel,lstrlen(sysmetrics[i].szLabel));
			TextOut(hdc,22*cxCaps,y,sysmetrics[i].szDesc,lstrlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc,TA_RIGHT | TA_TOP);
			TextOut(hdc,22*cxCaps + 40 * cxChar,y,szBuffer,wsprintf(szBuffer,TEXT("%5d"),GetSystemMetrics(sysmetrics[i].iIndex)));

			SetTextAlign(hdc,TA_LEFT | TA_TOP);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_VSCROLL:
		switch(LOWORD(wParam))
		{
		case SB_LINEUP:
			iVScrollPos -= 1;
			break;
		case SB_LINEDOWN:
			iVScrollPos += 1;
			break;
		case SB_PAGEDOWN:
			iVScrollPos += cyClient/cyChar;
			break;
		case SB_PAGEUP:
			iVScrollPos -= cyClient/cyChar;
			break;
		case SB_THUMBPOSITION:
			iVScrollPos = HIWORD(wParam);
			break;
		default:
			break;
		}

		iVScrollPos = max(0,min(iVScrollPos,NUMLINES-1));

		if(iVScrollPos != GetScrollPos(hWnd,SB_VERT))
		{
			SetScrollPos(hWnd,SB_VERT,iVScrollPos,TRUE);
			InvalidateRect(hWnd,NULL,TRUE);
			UpdateWindow(hWnd);
		}

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
```
# 滚动条优化
```c++:n
// SYSMETS.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SYSMETS.h"
#include "data.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SYSMETS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SYSMETS));

	// 主消息循环:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SYSMETS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;//MAKEINTRESOURCE(IDC_SYSMETS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, TEXT("Get System Metrics No.2"), WS_OVERLAPPEDWINDOW|WS_VSCROLL,
	  CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
	  return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int cxChar, cxCaps, cyChar, cyClient;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szBuffer[10];
	TEXTMETRIC tm;
	int y;
	SCROLLINFO si;
	int iVertPos;
	int iPaintBeg,iPaintEnd;

	switch (message)
	{
	case WM_CREATE:
		hdc = GetDC(hWnd);
		GetTextMetrics(hdc,&tm);
		cxChar = tm.tmAveCharWidth;
		cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
		cyChar = tm.tmHeight + tm.tmExternalLeading;
		ReleaseDC(hWnd,hdc);

		return 0;
	case WM_SIZE:
		cyClient = HIWORD(lParam);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE;
		si.nPage = cyClient/cyChar;
		si.nMin = 0;
		si.nMax = NUMLINES - 1;
		SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
		return 0;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		// scroll information is stored by 'si'
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo(hWnd,SB_VERT,&si);
		iVertPos = si.nPos;

		iPaintBeg = max(0,iVertPos + ps.rcPaint.top / cyChar);
		iPaintEnd = min(NUMLINES-1,iVertPos + ps.rcPaint.bottom / cyChar);

		for(int i=iPaintBeg; i<=iPaintEnd; ++i)
		{
			y = cyChar * (i - iVertPos);
			TextOut(hdc,0,y,sysmetrics[i].szLabel,lstrlen(sysmetrics[i].szLabel));
			TextOut(hdc,22*cxCaps,y,sysmetrics[i].szDesc,lstrlen(sysmetrics[i].szDesc));

			SetTextAlign(hdc,TA_RIGHT | TA_TOP);
			TextOut(hdc,22*cxCaps + 40 * cxChar,y,szBuffer,wsprintf(szBuffer,TEXT("%5d"),GetSystemMetrics(sysmetrics[i].iIndex)));

			SetTextAlign(hdc,TA_LEFT | TA_TOP);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_VSCROLL:

		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		GetScrollInfo(hWnd,SB_VERT,&si);

		iVertPos = si.nPos;

		switch(LOWORD(wParam))
		{
		case SB_LINEUP:
			si.nPos -= 1;
			break;
		case SB_LINEDOWN:
			si.nPos += 1;
			break;
		case SB_PAGEDOWN:
			si.nPos += si.nPage;
			break;
		case SB_PAGEUP:
			si.nPos -= si.nPage;
			break;
		case SB_THUMBTRACK:
			si.nPos = si.nTrackPos;
			break;
		default:
			break;
		}

		si.fMask = SIF_POS;
		SetScrollInfo(hWnd,SB_VERT,&si,TRUE);
		GetScrollInfo(hWnd,SB_VERT,&si);

		if(si.nPos != iVertPos)
		{
			ScrollWindow(hWnd,0,cyChar*(iVertPos-si.nPos),NULL,NULL);
			UpdateWindow(hWnd);
		}

		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
```
