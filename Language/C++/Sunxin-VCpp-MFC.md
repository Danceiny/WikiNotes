---
date: 2016-02-21 16:56
status: public
title: Sunxin-VCpp-MFC
---

# win32 MFC wizard
# MDI Frame
Multi-Doc


# SDI Frame
Single-Doc

类。
- CMainFrame
- CAboutDlg
- C**App
在入口函数WinMain()加载之前就已执行了。

入口函数执行之前，全局对象就已分配内存空间。

`class C**App: public CWinApp`

`class CWinApp: public CWinThead`

- C**Doc

数据本身和数据显示分离的模式。

一个模板函数将CMainFrame,C**Doc,C**View组合起来。

- C**View

# WinMain
在所新建项目中找不到WinMain()函数。

编译时链接到WinMain()。

MFC源代码里面找。


## CWinThead::Run()
实现消息队列循环。



# 使用win32-windows应用程序生成向导创建HelloWin
项目文件结构
- HelloWin
	- 头文件
		- HelloWin.h
		- Resource.h
		- stdafx.h
		- targetver.h
	- 外部依赖项
	- 源文件
		- HelloWin.cpp
		- stdafx.cpp
	- 资源文件
		- HelloWin.ico
		- HelloWin.rc
		- small.ico
	- README.txt

**README.txt**

```
========================================================================
    WIN32 应用程序：HelloWin 项目概述
========================================================================

应用程序向导已为您创建了此 HelloWin 应用程序。

本文件概要介绍组成 HelloWin 应用程序的
的每个文件的内容。


HelloWin.vcproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件，
    其中包含生成该文件的 Visual C++ 的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

HelloWin.cpp
    这是主应用程序源文件。

/////////////////////////////////////////////////////////////////////////////
应用程序向导创建了下列资源：

HelloWin.rc
这是程序使用的所有 Microsoft Windows 资源的列表。它包括 RES 子目录中存储的图标、位图和光标。
此文件可以直接在 Microsoft Visual C++ 中进行编辑。

Resource.h
    这是标准头文件，可用于定义新的资源 ID。
    Microsoft Visual C++ 将读取并更新此文件。

HelloWin.ico
    这是一个图标文件，可用作应用程序的图标 (32x32)。
    此图标包括在主资源文件 HelloWin.rc 中。

small.ico
    这是一个图标文件，其中包含应用程序的图标的较小版本 (16x16)。此图标包括在主资源文件 HelloWin.rc 中。

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

StdAfx.h, StdAfx.cpp
    这些文件用于生成名为 HelloWin.pch 的预编译头 (PCH) 文件和名为 StdAfx.obj 的预编译类型文件。

/////////////////////////////////////////////////////////////////////////////
其他注释：

应用程序向导使用“TODO:”注释来指示应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////
```

**HelloWin.cpp**
```c++:n
// HelloWin.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "HelloWin.h"

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

//  WINAPI   WinMain
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
	LoadString(hInstance, IDC_HELLOWIN, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HELLOWIN));

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
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HELLOWIN));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_HELLOWIN);
	wcex.lpszMenuName = NULL;
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
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			// IDD_ABOUTBOX参数决定了关于框所显示的文字内容。
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
		DrawText(hdc,TEXT("Hello, Windows!"), -1, &rect,
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
	UNREFERENCED_PARAMETER(lParam);//注释这一句，运行发现似乎没什么影响。

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
	return (INT_PTR)FALSE;//注释掉这一句，关于的窗口显示变形，但文字内容没有变。由此可见，这个函数并不涉及关于框的内容。只是一个有关的‘消息’处理程序。
}
```

下面是纯手写的代码。
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
		//MessageBox(hwnd,TEXT("Mouse Clicked!"),TEXT("消息"),MB_OK);
		//PostQuitMessage(0);
		return 0;
	case WM_PAINT: //绘制消息
		hdc = BeginPaint(hwnd,&ps);
		GetClientRect(hwnd,&rect);
		//Ellipse(hdc,0,0,200,100);
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

# CDC 类
作图相关。

```c++:n
void CDrawView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	// 下面画出的线条是黑色的。

	//下面是第一种方法
	HDC hdc;
	hdc = ::GetDC(m_hWnd);//m_hWnd是从基类派生而来的一个保存窗口对象信息的变量
	MoveToEx(hdc, m_ptOrigin.x, m_ptOrigin.y, NULL);
	LineTo(hdc, point.x, point.y);
	::ReleaseDC(m_hWnd,hdc);

	//下面是第二种方法
	/*
	CDC *pDC = GetDC();
	pDC->MoveTo(m_ptOrigin);
	pDC->LineTo(point);
	ReleaseDC(pDC);
	CView::OnLButtonUp(nFlags, point);
	*/

	//下面是第三种方法
	/*
	CClientDC dc(this);
	//或者CClientDC dc(GetParent());这个可以画到工具栏上去，因为父窗口嘛
	dc.MoveTo(m_ptOrigin);
	dc.LineTo(point);
	CView::OnLButtonUp(nFlags, point);
	*/

	//下面是第四种方法
	// both client and nonclient areas（标题栏和工具栏）.
	/*CWindowDC 类
	Derived from CDC.*/
	/*
	CWindowDC dc(GetParent());	// CWindowDC dc(this);
	dc.MoveTo(m_ptOrigin);
	dc.LineTo(point);
	CView::OnLButtonUp(nFlags, point);
	*/

	//还可以画到桌面上去~截屏软件么么哒
	//只需要GetParent()改成GetDesktopWindow()。

	//线条也要多姿多彩~
	/*CPen
	BOOL CreatePen(
   int nPenStyle,
   int nWidth,
   COLORREF crColor  
);
BOOL CreatePen(
   int nPenStyle,
   int nWidth,
   const LOGBRUSH* pLogBrush,
   int nStyleCount = 0,
   const DWORD* lpStyle = NULL  
   */

   /*
   CPen pen(PS_SOLID,1,RGB(255,0,0));//线形，线宽，线色
   CClientDC dc(this);
   // 将笔选到设备描述表中

   /*CDC::SelectObject return a pointer to the object being replaced.
   CPen* SelectObject(
   CPen* pPen
);
CBrush* SelectObject(
   CBrush* pBrush
);
virtual CFont* SelectObject(
   CFont* pFont
);
CBitmap* SelectObject(
   CBitmap* pBitmap
);
int SelectObject(
   CRgn* pRgn
);
CGdiObject* SelectObject(
   CGdiObject* pObject
);*/

	CPen *pOldPen = dc.SelectObject(&pen);
	dc.MoveTo(m_ptOrigin);
	dc.LineTo(point);
	dc.SelectObject(pOldPen);
    CView::OnLButtonUp(nFlags, point);
);
	*/

	//下面来玩画刷
	/*
	CBrush brush(RGB(255,0,0));
	CClientDC dc(this);
	//此处不需要将画刷选到设备描述表中
	dc.FillRect(CRect(m_ptOrigin, point), &brush);
	CView::OnLButtonUp(nFlags, point);
	*/

	//下面来玩位图
	/*
	/*CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP1);
	CBrush brush(&bitmap);*/
	CClientDC dc(this);
	//dc.FillRect(CRect(m_ptOrigin, point),&brush);//上面这里提供了&brush指针，所以不用SelectObject()。
	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush *pOldBrush = dc.SelectObject(pBrush);
	dc.Rectangle(CRect(m_ptOrigin, point));//这里没有提供&brush指针，所以要SelectObject()
	CView::OnLButtonUp(nFlags, point);

}
```


下面来画连续的线条。

```c++:n
void CDrawView::OnMouseMove(UINT nFlags CPoint point)
{
	CClientDC dc(this);
	CPen pen(PS_SOLID,1,RGB(255,0,0));
	CPen *pOldPen = dc.SelectObject(&pen);

	if(m_bDraw == TRUE)//m_bDraw是类成员变量，初始化为FALSE，鼠标左键按下开始作图
	{
		dc.MoveTo(m_ptOrigin);
		dc.LineTo(point);
		m_ptOrigin = point;
	}
	dc.SelectObject(pOldPen);
	CView::OnMouseMove(nFlags, point);
}
```

下面的画扇形。
```c++:n
void CDrawView::OnMouseMove(UINT nFlags CPoint point)
{
	CClientDC dc(this);
	CPen pen(PS_SOLID,1,RGB(255,0,0));
	CPen *pOldPen = dc.SelectObject(&pen);

	if(m_bDraw == TRUE)//m_bDraw是类成员变量，初始化为FALSE，鼠标左键按下开始作图
	{
		dc.MoveTo(m_ptOrigin);
		dc.LineTo(m_ptOld);
		dc.MoveTo(m_ptOrigin);//改成dc.MoveTo(m_ptOld)则扇形带有边线
		dc.LineTo(point);
	}
	dc.SelectObject(pOldPen);
	CView::OnMouseMove(nFlags, point);
}
```
# CWnd 类
窗口相关。