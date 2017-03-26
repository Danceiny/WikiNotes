---
date: 2016-01-18 18:44
status: public
title: Chapter9-Aspiration-Search
---

# Aspriation Search
基于Fail-soft alpha-beta的搜索算法。
猜测搜索的范围[alpha,beta]。
假定我们猜测搜索的结果在x附近，并设window是要搜索范围的大小的一半。
那我们可以令
**alpha=x-window**,
**beta=x+window**。
调用Value=FAlphaBeta(depth,x-window,x+window)来搜索结果。
可能得到的结果有3种：
1. 返回的值value在(x-window,x+window)区间内；
2. 返回的值value>=x+window，这种情形被称为fail high，需要重新给定范围。在重新搜索时令根节点处的alpha=value,beta=INFINITY,也就是调用FAlphaBeta(depth,value,INFINITY)；
3. 返回的值value<=x-window。这种情形被称为fail low。重新搜索时令根节点处的alpha=-INFINITY，beta=INGINITY，也就是调用FAlphaBeta(depth,-INFINITY,value)。

以上方法即为渴望搜索。以伪代码描述如下：
```C++:n
int alpha = X-WINDOW;
int beta = X+WINDOW;
for(;;){
    score = FAlphaBeta(depth,alpha,beta)
    if(score <= alpha)alpha = -WIN;
    else if(score >= beta)beta =WIN;
    else break;
}
```
如何猜得更准是渴望搜索提高效率的一个重要问题。
比较普遍的做法是记录上一次搜索得到的值，作为当前这一次的X,因为两次搜索在相当多的时候结果是相近的。
也可以先进行一次深度为depth-1的搜索，将返回值作为深度为depth的搜索的X。一般情况下深度为depth-1的搜索花费的时间仅为深度为depth的搜索的1/15到1/5。
## AspirationSearch.h
```C++:n
// COPYRIGHT NOTES
// ---------------
// This source code is a part of chess which is an example of <Game Programing guide>.
// You may use, compile or redistribute it as part of your application 
// for free. 
// You cannot redistribute sources without the official agreement of the author. 
// If distribution of you application which contents code below was occured, place 
// e-mail <hidebug@hotmail.com> on it is to be appreciated.
// This code can be used WITHOUT ANY WARRANTIES on your own risk.
// 
// Spring Wang <hidebug@hotmail.com>

// ---------------
// 版权声明
// ---------------
// 本文件所含之代码是《人机博弈程序设计指南》的范例程序中国象棋的一部分
// 您可以免费的使用, 编译 或者作为您应用程序的一部分。 
// 但，您不能在未经作者书面许可的情况下分发此源代码。 
// 如果您的应用程序使用了这些代码，在您的应用程序界面上 
// 放入 e-mail <hidebug@hotmail.com> 是令人欣赏的做法。
// 此代码并不含有任何保证，使用者当自承风险。
// 
// 王小春 <hidebug@hotmail.com>

// AspirationSearch.h: interface for the CAspirationSearch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASPIRATIONSEARCH_H__C033F4E0_E335_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_ASPIRATIONSEARCH_H__C033F4E0_E335_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FAlphaBetaEngine.h"

class CAspirationSearch : public CFAlphaBetaEngine  
{
public:
	CAspirationSearch();
	virtual ~CAspirationSearch();

	virtual SearchAGoodMove(BYTE position[10][9]);
};

#endif // !defined(AFX_ASPIRATIONSEARCH_H__C033F4E0_E335_11D5_AEC7_5254AB2E22C7__INCLUDED_)
```
## AspirationSearch.cpp
```C++:n
// COPYRIGHT NOTES
// ---------------
// This source code is a part of chess which is an example of <Game Programing guide>.
// You may use, compile or redistribute it as part of your application 
// for free. 
// You cannot redistribute sources without the official agreement of the author. 
// If distribution of you application which contents code below was occured, place 
// e-mail <hidebug@hotmail.com> on it is to be appreciated.
// This code can be used WITHOUT ANY WARRANTIES on your own risk.
// 
// Spring Wang <hidebug@hotmail.com>

// ---------------
// 版权声明
// ---------------
// 本文件所含之代码是《人机博弈程序设计指南》的范例程序中国象棋的一部分
// 您可以免费的使用, 编译 或者作为您应用程序的一部分。 
// 但，您不能在未经作者书面许可的情况下分发此源代码。 
// 如果您的应用程序使用了这些代码，在您的应用程序界面上 
// 放入 e-mail <hidebug@hotmail.com> 是令人欣赏的做法。
// 此代码并不含有任何保证，使用者当自承风险。
// 
// 王小春 <hidebug@hotmail.com>

// AspirationSearch.cpp: implementation of the CAspirationSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "AspirationSearch.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAspirationSearch::CAspirationSearch()
{
}

CAspirationSearch::~CAspirationSearch()
{
}

CAspirationSearch::SearchAGoodMove(BYTE position[10][9])
{
	int x,y;
	memcpy(CurPosition, position, 90);
	m_nMaxDepth = m_nSearchDepth-1;
	x= FAlphaBeta(m_nMaxDepth, -20000, 20000);
	m_nMaxDepth = m_nSearchDepth;
	y = FAlphaBeta(m_nMaxDepth, x-50, x+50);
	if (y < x-50)
		FAlphaBeta(m_nMaxDepth, -20000, y);
	if (y > x+50)
		FAlphaBeta(m_nMaxDepth, y, 20000);
	MakeMove(&m_cmBestMove);
	memcpy(position, CurPosition, 90);
	return 0;
}
```