---
date: 2016-01-18 00:07
status: public
title: Chapter7-AlphaBeta-Search
---

# Alpha-Beta Search
在极大极小搜索的过程中存在着一定程度的数据冗余。
## Alpha cutoff
如下左图，A=max(B,C),B=18,C=min(D,E,F),D=16。则显然C<=16,从而A=18。EF为冗余，可省去。这样可以减小运算量。
![](~/alpha-beta-cutoff.PNG)
## Beta cutoff
如上右图，A=min(B,C),B=8,C=max(D,E,F),D=18。则显然C>=18,从而A=8。
综上可知，节点以特定顺序排列时，就可以进行alpha和beta剪枝。
## Alpha-Beta搜索算法
将Alpha剪枝和Beta剪枝加入minimax搜索，就得到Alpha-Beta搜索算法。
我的理解：**某级在[alpha,beta]范围内(同级对照)**，
1. *某节点如果是向下取**极大值**，则将下一层的每个值**与beta比较大小**，比beta大的话，表示该节点已经比同级的其他节点大，其下一层的其他值做beta剪枝；*
2. *某节点如果是向下取**极小值**，则将下一层的每个值**与alpha比较大小**，比alpha小的话，表示该节点已经比同级的其他节点小，其下一层的其他值做alpha剪枝。*

其实现的类C伪代码如下：
```C:n
int AlphaBeta(nPly,nAlpha,nBeta)
{
    if(game over)return evaluation();//胜负已分
    if(nPly==0)return evalution();//叶子节点
    if(Is Min Node)//判断当前节点是哪种节点
    {//取极小值的节点
        for(each possible move m){
        make more m;//生成新节点
        score = AlphaBeta(nPly-1,nAlpha,nBeta)//递归搜索子节点
        unmake move m;
        if(score < nBeta){
            nBeta = score;//取极小值(下一层搜索的对照值）
            if(nAlpha >= nBeta)return nAlpha;//比对照最小值还小，则剪枝Alpha Cutoff，抛弃后继节点
        }
    }
    return nBeta;//返回最小值
    }
    else{//取极大值的节点
        for(each possible move m){
        make more m;//生成新节点
        score = AlphaBeta(nPly-1,nAlpha,nBeta)//递归搜索子节点
        unmake move m;
        if(score > nBeta){
            nBeta = score;//取极小值
            if(nAlpha >= nBeta)return nBeta;//Beta Cutoff，抛弃后继节点
        }
    }
    return nAlpha;//返回极大值    
    }
}
```

同极大极小搜索算法一样，以上代码有点繁琐，不仅要在奇数层进行alpha剪枝，还要在偶数层进行beta剪枝。如果套用负极大值的形式，可改进之。
注意此时只有beta剪枝。

```C++:n
int alphabeta(int nPly, int alpha, int beta)
{
    if(game over)return evaluation();
    if(nPly <= 0)return evaluation();
    for(each possible move m){
        make move m;
        score = -alphabeta(nPly-1,-beta,-alpha)
        unmake move m;
        if(score >= alpha)alpha = score;//保存最大值
        if(alpha >= beta)break;//beta剪枝
    }
    return alpha;//返回极大值    
```

alpha,beta初始值分别为-∞,+∞,递归调用中不断改变。越往后其范围越小，即落在其范围之外的内容越来越多，剪枝效率越来越高。
## Alpha-Beta搜索公式
在节点排列最理想的情形之下，使用Alpha-Beta搜索建立的博弈树的节点个数为：
___pow(W,(d+1)/2)+pow(W,d/2)+1___
其中W是博弈树的分支因子，d是最大搜索深度。
这个数字大约是极大极小搜索建立的节点数的平方根的2倍。
称这棵理想的博弈树为Minimal Tree。
在最坏的情况下，就是全部可能的节点。
由于Alpha-Beta剪枝与节点的排列顺序高度相关，则使用其他手段将节点排列调整为剪枝效率更高的顺序就显得尤为重要。
# AlphaBetaEngine
应用Alpha-Beta剪枝算法。
在工程中加入一个新类CAlphaBetaEngine，派生自CSearchEngine，同已有的CNegamax一样。如下，其暴露在外的函数只有基类定义的接口函数SearchAGoodMove。
## AlphaBetaEngine.h

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

// Alphabeta_HH.h: interface for the CAlphabeta_HH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Alphabeta_HH_H__5870AB21_E3F1_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_Alphabeta_HH_H__5870AB21_E3F1_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"
#include "HistoryHeuristic.h"

class CAlphabeta_HH : public CSearchEngine , public CHistoryHeuristic
{
public:
	CAlphabeta_HH();
	virtual ~CAlphabeta_HH();

	virtual SearchAGoodMove(BYTE position[10][9]);

protected:
	int alphabeta(int depth, int alpha, int beta);

};

#endif // !defined(AFX_Alphabeta_HH_H__5870AB21_E3F1_11D5_AEC7_5254AB2E22C7__INCLUDED_)

```
该头文件中重新定义了SearchAGoodMove，并且定义了一个alphabeta函数，供接口函数SearchAGoodMove调用。
## AlphaBetaEngine.cpp
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

// Alphabeta_HH.cpp: implementation of the CAlphabeta_HH class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "Alphabeta_HH.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAlphabeta_HH::CAlphabeta_HH()
{

}

CAlphabeta_HH::~CAlphabeta_HH()
{

}

CAlphabeta_HH::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);
	m_nMaxDepth = m_nSearchDepth;
	ResetHistoryTable();
	alphabeta(m_nMaxDepth, -20000, 20000);
	MakeMove(&m_cmBestMove);
	memcpy(position, CurPosition, 90);
}

int CAlphabeta_HH::alphabeta(int depth, int alpha, int beta)
{
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
		m_pMG->m_MoveList[depth][i].Score = 
			           GetHistoryScore(&m_pMG->m_MoveList[depth][i]);
	}
	MergeSort(m_pMG->m_MoveList[depth], Count, 0);
	int bestmove = -1;
	for (i=0;i<Count;i++) 
	{

		type = MakeMove(&m_pMG->m_MoveList[depth][i]);
		score = -alphabeta(depth - 1, -beta, -alpha);
		UnMakeMove(&m_pMG->m_MoveList[depth][i],type); 

		if (score > alpha)
		{
			alpha = score;
			if(depth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
			bestmove = i;
		}
        if (alpha >= beta) 
		{
			bestmove = i;
			break;
		}
				
	}
	if (bestmove != -1)
	EnterHistoryScore(&m_pMG->m_MoveList[depth][bestmove], depth);
	return alpha;
}
```
为了能够使用新的搜素引擎，需要在CNewGame和CChessDlg类中加点东西。
* 在newGame.cpp的函数OnInitDialog中；
* 在chessdlg.h中；
* 在chessdlg.cpp中的函数OnNewgame中；

代码见上章。[Chapter6-单搜索引擎实例.markdown](/人机博弈-游戏编程/Chapter6-单搜索引擎实例.markdown)