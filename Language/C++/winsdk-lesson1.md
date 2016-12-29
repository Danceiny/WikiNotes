---
date: 2016-04-28 14:33
status: public
title: winsdk-lesson1
---

# wchar_t
宽字符数据类型，Unicode，2个字节。

`wchar_t c =L'国'`

要写L，代表是Unicode，不加就是Ascii。

不能用printf()函数。要用wprintf()。

`wprintf(L"%lc\n",c)`


|ACSII|	Unicode|
|-------|-------|
|char|	wchar_t|
|CHAR|	WCHAR|
|printf|	wprintf|
|strlen|	wcslen|
|MessageBoxA|	MessageBoxW|

通用：
- `_tcslen`
- `_tprintf`
- MessageBox
- TCHAR
- 宏 TEXT
- 宏 `_TEXT`
- 宏 `__T`
- 宏 `_T`

```c:n
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
int main()
{
	//不要用
	//char c;
	//wchar_t c;
	//CHAR c;
	//WCHAR c;
	TCHAR str[] = _T("中国");

	//setlocale(LC_ALL,"chs");
	_tprintf(_T("%s\n"),str);
	return 0;
}
```

# Windows的字符串函数
|通用|ASCII|
|-----|-----|
|lstrlen|strlen|
|lstrcpy|strcpy|
|lstrcpyn|strcpyn|
|lstrcat|strcat|
|lstrcmp|strcmp|
|lstrcmpi|strcmpi|


# sprintf
```c++:n
int sprintf_s(
   char *buffer,
   size_t sizeOfBuffer,
   const char *format [,
      argument] ...
);
int _sprintf_s_l(
   char *buffer,
   size_t sizeOfBuffer,
   const char *format,
   locale_t locale [,
      argument] ...
);
int swprintf_s(
   wchar_t *buffer,
   size_t sizeOfBuffer,
   const wchar_t *format [,
      argument]...
);
int _swprintf_s_l(
   wchar_t *buffer,
   size_t sizeOfBuffer,
   const wchar_t *format,
   locale_t locale [,
      argument]…
);
template <size_t size>
int sprintf_s(
   char (&buffer)[size],
   const char *format [,
      argument] ...
); // C++ only
template <size_t size>
int swprintf_s(
   wchar_t (&buffer)[size],
   const wchar_t *format [,
      argument]...
); // C++ only
```
下面的程序显示屏幕分辨率。
```c++:n
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <stdarg.h>
int CDECL MessageBoxPrintf(TCHAR * szCaption, TCHAR * szFormat, ...)
{
	TCHAR szBuffer[1024];
	va_list pArgList;
	va_start(pArgList, szFormat);
	_vsntprintf(szBuffer, sizeof(szBuffer) / sizeof(TCHAR), szFormat, pArgList);
	va_end(pArgList);

	return MessageBox(NULL, szBuffer, szCaption, MB_OK | MB_ICONINFORMATION);
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstane,
	PSTR szCmdLine,
	int iCmdShow)
{
	int x = 100, y = 200, z = 300;
	int cxScreen, cyScreen;

	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

	MessageBoxPrintf(TEXT("我的标题"), TEXT("Hello %d, %d, %d"), x, y, z);
	MessageBoxPrintf(TEXT("屏幕大小"), TEXT("我的屏幕宽度是: %d, 高度是: %d"), cxScreen, cyScreen);

	return 0;
}
```


下面展示了一个最简单windows窗口程序的标准流程。
```c++:n
#include <windows.h>

//窗口过程回调函数       H是Handler的简写，句柄
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

//相当于 int main()
//                 实例句柄
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpszCmdLine,
				   int nCmdShow)
{
	HWND hwnd;//窗口句柄
	MSG msg;//消息
	WNDCLASS wc;//窗口类

	//1.设计一个窗口类
	wc.style = 0;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = TEXT("MyWndClass");

	//2.注册窗口类
	RegisterClass(&wc);

	//3.创建窗口
	hwnd = CreateWindow(
		TEXT("MyWndClass"),
		TEXT("Hello SDK Application"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,//父窗口句柄
		NULL,//窗口菜单句柄
		hInstance,
		NULL);

	//4.显示和更新窗口
	ShowWindow(hwnd,nCmdShow);
	UpdateWindow(hwnd);
	//5.消息循环
	while(GetMessage(&msg,NULL,0,0)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);//转发到窗口过程
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,
						 UINT message,
						 WPARAM wParam,
						 LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;//DC句柄
	RECT rect;

	//对各种消息进行处理
	switch(message)
	{
	case WM_SIZE:
		//重画 Paint
		return 0;
	case WM_LBUTTONDOWN:
		MessageBox(hwnd,TEXT("Mouse Clicked!"),TEXT("消息"),MB_OK);
		PostQuitMessage(0);
		return 0;
	case WM_PAINT: //绘制消息
		hdc = BeginPaint(hwnd,&ps);
		GetClientRect(hwnd,&rect);
		//Ellipse(hdc,0,0,200,100);	//	绘制椭圆
		DrawText(hdc,TEXT("Hello, Windows!"),-1,&rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd,&ps);
		return 0;
	case WM_DESTROY://销毁窗口消息（关闭）
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd,message,wParam,lParam);
}
```


下面是一个可以播放wav格式声音文件的程序源代码主文件。
**HelloWin32.cpp**
```c++:n
// HellWin32.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "HellWin32.h"

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
	LoadString(hInstance, IDC_HELLWIN32, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HELLWIN32));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELLWIN32));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HELLWIN32);
	//wcex.lpszMenuName = NULL;
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
	RECT rect;

	switch (message)
	{
	case WM_CREATE:
		PlaySound(TEXT("LoopyMusic.wav"),NULL,SND_FILENAME | SND_ASYNC);
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
		GetClientRect(hWnd,&rect);
		DrawText(hdc,TEXT("Hello Windows!"),-1,&rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
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


该代码所播放的两个wav文件我用Potplayer和网易云音乐都播放不了，这是什么鬼？