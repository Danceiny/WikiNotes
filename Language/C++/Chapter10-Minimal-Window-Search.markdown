---
date: 2016-01-18 19:16
status: public
title: Chapter10-Minimal-Window-Search
---

# 极小窗口搜索
Alpha-Beta的一种变体。此算法基于如下假设：
**假定第一步是最佳的移动，其后继则次之，直到另一个节点被证明是最佳的。**
在一个中间节点，其第一个分支以一个完整窗口(a,b)搜索之并产生一个位于该窗口中的值v，后继的分支则以一个极小的窗口(v,v+1)搜索之。
极小窗口搜索的意图在于使用极小的窗口建立极小的搜索树，以此达成高效的搜索。
本质上讲，极小窗口搜索依赖于后继节点的值相对于前驱节点值的微小变化的猜测。
如果猜测不准确(fail high),随后就需以(v+1,b)为窗口重新搜索（如果fail low,说明这个节点不如已有的最佳节点，就不必再搜索了）。
极小窗口搜索算法也叫PVS(Principal Variation Search)算法。
下面给出伪代码的描述：
```C:n
int PrincipalVariation(depth, alpha, beta)
{
    if(game over or depth==0)return evaluetion(p);//胜负已分或者叶节点，返回估值
    make move m;//创建第一个子节点
    //用全窗口搜索第一个节点
    best = -PrincipalVariation(depth-1, -beta , -alpha);
    unmake move m;//撤销第一个节点
    for(each possible move m)//从第二个节点起，对每个节点
    {
        if(best < beta)
        {
            if(best > alpha)alpha = best;
            make move m;//创建子节点
            //空窗探测
            value = -PrincipalVariation(depth-1, -alpha-1, -alpha);
            //如果探测结果fail-high，重新搜索
            if(value > alpha && value < beta)
            best = -PrincipalVariation(depth-1, -beta, -value);
            else if(value > best)best = value;//命中
            unmake move m;//撤销子节点
        }
    return best;//返回最佳值
}
```
## PVS_Engine.h
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

// PVS_Engine.h: interface for the CPVS_Engine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PVS_ENGINE_H__8E7D8B20_DB95_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_PVS_ENGINE_H__8E7D8B20_DB95_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"

class CPVS_Engine : public CSearchEngine
{
public:
	CPVS_Engine();
	virtual ~CPVS_Engine();
	virtual SearchAGoodMove(BYTE position[10][9]);

protected:
	int PrincipalVariation(int depth, int alpha, int beta);


};

#endif // !defined(AFX_PVS_ENGINE_H__8E7D8B20_DB95_11D5_AEC7_5254AB2E22C7__INCLUDED_)
```
## PVS_Engine.cpp
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

// PVS_Engine.cpp: implementation of the CPVS_Engine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "PVS_Engine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPVS_Engine::CPVS_Engine()
{

}

CPVS_Engine::~CPVS_Engine()
{

}
CPVS_Engine::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);
	m_nMaxDepth = m_nSearchDepth;

//	for (m_nMaxDepth = 1; m_nMaxDepth <= m_nSearchDepth; m_nMaxDepth++)
	{
	   PrincipalVariation(m_nMaxDepth, -20000, 20000);
	}
	
	MakeMove(&m_cmBestMove);
	memcpy(position, CurPosition, 90);
}

int CPVS_Engine::PrincipalVariation(int depth, int alpha, int beta)
{
	int score;
	int Count,i;
	BYTE type;
	int best;

	i = IsGameOver(CurPosition, depth);
	if (i != 0)
		return i;

	if (depth <= 0)	//叶子节点取估值
		return m_pEval->Eveluate(CurPosition, (m_nMaxDepth-depth)%2);
	
	Count = m_pMG->CreatePossibleMove(CurPosition, depth, (m_nMaxDepth-depth)%2);
	

	type = MakeMove(&m_pMG->m_MoveList[depth][0]);
	best = -PrincipalVariation( depth-1, -beta, -alpha);
	UnMakeMove(&m_pMG->m_MoveList[depth][0],type); 
	if(depth == m_nMaxDepth)
		m_cmBestMove = m_pMG->m_MoveList[depth][0];

	for (i=1;i<Count;i++) 
	{
		
		if(best < beta) 
		{
			if (best > alpha) 
				alpha = best;
			type = MakeMove(&m_pMG->m_MoveList[depth][i]);
			score = -PrincipalVariation(depth-1, -alpha-1, -alpha);
			if (score > alpha && score < beta) 
			{
				best = -PrincipalVariation(depth-1, -beta, -score);
				if(depth == m_nMaxDepth)
					m_cmBestMove = m_pMG->m_MoveList[depth][i];
			}
			else if (score > best)
			{
				best = score;
				if(depth == m_nMaxDepth)
					m_cmBestMove = m_pMG->m_MoveList[depth][i];
			}
			UnMakeMove(&m_pMG->m_MoveList[depth][i],type); 
		}
	}

	return best;
}
```