---
date: 2016-01-18 01:00
status: public
title: Chapter8-Fail-soft-alpha-beta
---

# Fail-soft alpha-beta
前面在实现调用alphabeta(depth, alpha, beta)的时候，alpha,beta的初始值设为±20000。如果一开始就把alpha和beta限定得较小，那么整个的搜索过程就会减去更多的枝条。
但是有可能：
1. 要找的目标落在alpha、beta的范围之内，这是最理想的情形，很少时间内就可得到结果；
2. 要找的目标比alpha小；
3. 或者比beta大。

对于后两种情况，只能重新搜索。

但是在上面的alpha-beta过程中，一旦搜索失败，因为其返回值是alpha，所以无法返回任何比alpha更小的值，就无从得知任何与结果有关的信息。

改进算法，令其返回当前的score。这样的alpha-beta搜索即为Fail-soft alpha-beta。

其实现的伪代码如下：
```C:n
int FAlphaBeta(int depth, int alpha, int beta)
{
    int current = -INFINITY;//负无穷
    if(game over or depth <= 0)//游戏结束或叶节点
    return evaluation();
    for(each possible move m)
    {
        make move m;
        score = -FAlphaBeta(depth-1, -beta, -alpha)
        unmake move m;
        if(score > current)
        {
            current = score;//保留极大值
            if(score >= alpha)
                alpha = score;//修改alpha边界
            if(score >= beta)
                break;//beta剪枝
        }
    }
    return current;//返回最佳值或者边界
```
改进后的算法，根据返回值可以判断要找的结果是否在[alpha,beta]范围内。
这一改进并未提高效率，但却是其他一些改进算法的基础。

## FAlphaBetaEngine.h
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

// FAlphaBetaEngine.h: interface for the CFAlphaBetaEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FALPHABETAENGINE_H__6C3A4903_CDED_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_FALPHABETAENGINE_H__6C3A4903_CDED_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"

class CFAlphaBetaEngine : public CSearchEngine  
{
public:
	CFAlphaBetaEngine();
	virtual ~CFAlphaBetaEngine();

	virtual SearchAGoodMove(BYTE position[10][9]);

protected:
	int FAlphaBeta(int depth, int alpha, int beta);

};

#endif // !defined(AFX_FALPHABETAENGINE_H__6C3A4903_CDED_11D5_AEC7_5254AB2E22C7__INCLUDED_)
```
## FAlphaBetaEngine.cpp
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

// FAlphaBetaEngine.cpp: implementation of the CFAlphaBetaEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "FAlphaBetaEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFAlphaBetaEngine::CFAlphaBetaEngine()
{
}

CFAlphaBetaEngine::~CFAlphaBetaEngine()
{
}

CFAlphaBetaEngine::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);
	m_nMaxDepth = m_nSearchDepth;
	FAlphaBeta(m_nMaxDepth, -20000, 20000);
	MakeMove(&m_cmBestMove);
	memcpy(position, CurPosition, 90);
	return 0;
}


int CFAlphaBetaEngine::FAlphaBeta(int depth, int alpha, int beta)
{
	int current = -20000 ;
	int score;
	int Count,i;
	BYTE type;
	
	i = IsGameOver(CurPosition, depth);
	if (i != 0)
		return i;

	if (depth <= 0)	//叶子节点取估值
		return m_pEval->Eveluate(CurPosition, (m_nMaxDepth-depth)%2);
	
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, (m_nMaxDepth-depth)%2);

	for (i=0;i<Count;i++) 
	{
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);
		score = -FAlphaBeta(depth - 1, -beta, -alpha);
		UnMakeMove(&m_pMG->m_MoveList[depth][i],type); 
				
		if (score > current)
		{
			current = score;
			if(depth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
			if (score > alpha)
				alpha = score;
			if (score >= beta) //beta剪枝
				break;
		}

	}

	return current;
}
```

同样，将这个搜索引擎加入New Game的列表，并在chessDlg.cpp中加入相应的创建代码。