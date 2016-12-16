---
date: 2016-01-19 19:52
status: public
title: Chapter13-Zobrist-Hash
---

# 如何快速产生哈希值
在[Chapter12-Hash-Table](/post/chess-game/chapter12-hash-table)中，对每一个节点，需要产生一个32位的索引来定位节点，还要产生一个64位的Checksum来验证表中记录的内容是否与当前局面相同。

## Zobrist
1. 在程序启动（或棋局开始）的时候，建立一个多维数组（在象棋中通常是三维）：

**Z[pieceType][boardWIdth][boardHeight]**

其中pieceType是棋子种类数，中国象棋中为14；
boardWidth为棋盘宽度，中国象棋中为9；
boardHeight为棋盘高度，中国象棋中为10。

2. 将此数组中填满随机数。若要求某一局面的哈希值，则将棋盘上所有棋子在数组Z中对应的随机数相加即得。
*示例：左上角有一个黑车，假定pieceType=5,则该棋子所对应的随机数就是Z[5][0][0]。将所有棋子对应的随机数相加即得某局面的哈希值。*
如果要32位哈希值，数组Z中元素应为32位。64位同理。

## 增量式计算
Zobrist方法无须每次都加总所有棋子。在程序根部，做一次加总操作求出根节点的哈希值，当搜索一个新节点时只要棋子在移动前将对应的随机数从哈希值中减去，再加上该棋子在移动后的随机数即可。
如果移动的棋子吃了别的棋子，还要减去被吃掉的棋子的随机数。
局面搜索完成后，再将搜索前减掉的值加上，搜索前加上的值减去，就恢复了当前节点的哈希值。
计算哈希值的加减过程可以用按位异或操作代替，效率更高。
增量式计算哈希值的方法在思想上同Alpha-Beta过程中的MakeMove/UnmakeMove是一致的。

## 推广应用
Zobrist哈希的方法不仅可以用于置换表，而且也可以开局库和残局库等，还可以建立一个小的哈希表用来检测是否发生了循环，如象棋中的长将。

## Transposition.h
为了能够将置换表方便地加入任何搜索引擎，将置换表单独写成一个类CTransposition。下为使用了置换表的Alpha-Beta搜索引擎。
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


// TranspositionTable.h: interface for the CTranspositionTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSPOSITIONTABLE_H__716F8220_CEEA_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_TRANSPOSITIONTABLE_H__716F8220_CEEA_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum ENTRY_TYPE{ exact , lower_bound, upper_bound}; 

typedef struct HASHITEM{
    LONGLONG checksum;	// or long long might be even better
	ENTRY_TYPE  entry_type;
    short depth;
    short eval;
}HashItem;

class CTranspositionTable  
{
public:
	CTranspositionTable();
	virtual ~CTranspositionTable();

	void CalculateInitHashKey(BYTE CurPosition[10][9]);
	void Hash_MakeMove(CHESSMOVE* move,BYTE CurPosition[10][9]);
	void Hash_UnMakeMove(CHESSMOVE* move,BYTE nChessID,BYTE CurPosition[10][9]);

	int LookUpHashTable(int alpha, int beta, int depth,int TableNo);
	void EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth,int TableNo);

	UINT m_nHashKey32[15][10][9];
	ULONGLONG m_ulHashKey64[15][10][9];
	HashItem *m_pTT[10];
	void InitializeHashKey();
	UINT m_HashKey32;
	LONGLONG m_HashKey64;
};

#endif // !defined(AFX_TRANSPOSITIONTABLE_H__716F8220_CEEA_11D5_AEC7_5254AB2E22C7__INCLUDED_)
```
## Transposition.cpp
本文件的头部有两个生成32位随机数和64位随机数的函数。这是因为**C语言中提供的伪随机函数rand()仅能提供15位的随机数**。
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

// TranspositionTable.cpp: implementation of the CTranspositionTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "TranspositionTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
LONGLONG rand64(void)
{

    return rand() ^ ((LONGLONG)rand() << 15) ^ ((LONGLONG)rand() << 30) ^

        ((LONGLONG)rand() << 45) ^ ((LONGLONG)rand() << 60);

}
LONG rand32(void)
{

    return rand() ^ ((LONG)rand() << 15) ^ ((LONG)rand() << 30);

}

CTranspositionTable::CTranspositionTable()
{
	InitializeHashKey();
}

CTranspositionTable::~CTranspositionTable()
{
	delete m_pTT[0];
	delete m_pTT[1];
}

void CTranspositionTable::InitializeHashKey()
{
	int i,j,k;
	
	srand( (unsigned)time( NULL ));
	
	for (i = 0; i < 15; i++)
		for (j = 0; j < 10; j++)
			for (k = 0; k < 9; k++)
			{
				m_nHashKey32[i][j][k] = rand32();
				m_ulHashKey64[i][j][k] = rand64();
			}

	m_pTT[0] = new HashItem[1024*1024];
	m_pTT[1] = new HashItem[1024*1024];
}
void CTranspositionTable::CalculateInitHashKey(BYTE CurPosition[10][9])
{
	int j,k,nChessType;
	m_HashKey32 = 0;
	m_HashKey32 = 0;
	for (j = 0; j < 10; j++)
		for (k = 0; k < 9; k++)
		{
			nChessType = CurPosition[j][k];
			if (nChessType != NOCHESS)
			{
				m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nChessType][j][k]; 
				m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nChessType][j][k]; 
			}
		}
}

void CTranspositionTable::Hash_MakeMove(CHESSMOVE* move, BYTE CurPosition[10][9])
{
	BYTE nToID, nFromID;
	nFromID = CurPosition[move->From.y][move->From.x];
	nToID = CurPosition[move->To.y][move->To.x];

	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nFromID][move->From.y][move->From.x]; 
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nFromID][move->From.y][move->From.x]; 

	if (nToID != NOCHESS)
	{
		m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nToID][move->To.y][move->To.x]; 
		m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nToID][move->To.y][move->To.x]; 
	}
	
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nFromID][move->To.y][move->To.x]; 
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nFromID][move->To.y][move->To.x]; 
}

void CTranspositionTable::Hash_UnMakeMove(CHESSMOVE* move, BYTE nChessID, BYTE CurPosition[10][9])
{
	BYTE nToID;
	nToID = CurPosition[move->To.y][move->To.x];
	
	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nToID][move->From.y][move->From.x]; 
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nToID][move->From.y][move->From.x]; 

	m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nToID][move->To.y][move->To.x]; 
	m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nToID][move->To.y][move->To.x]; 
	
	if (nChessID)
	{
		m_HashKey32 = m_HashKey32 ^ m_nHashKey32[nChessID][move->To.y][move->To.x]; 
		m_HashKey64 = m_HashKey64 ^ m_ulHashKey64[nChessID][move->To.y][move->To.x]; 
	}
}

int CTranspositionTable::LookUpHashTable(int alpha, int beta, int depth,int TableNo)
{
	int x;
	HashItem * pht;

	x = m_HashKey32 & 0xFFFFF;
	pht = &m_pTT[TableNo][x];

    if (pht->depth >= depth && pht->checksum == m_HashKey64)
	{
		switch (pht->entry_type) 
		{
		case exact: 
			return pht->eval;
		case lower_bound:
			if (pht->eval >= beta)
				return (pht->eval);
			else 
				break;
		case upper_bound:
			if (pht->eval <= alpha)
				return (pht->eval);
			else 
				break;
        }
	}

	return 66666;
}
void CTranspositionTable::EnterHashTable(ENTRY_TYPE entry_type, short eval, short depth,int TableNo)
{
	int x;
	HashItem * pht;

	x = m_HashKey32 & 0xFFFFF;//二十位哈希地址
	pht = &m_pTT[TableNo][x];

	pht->checksum = m_HashKey64;
	pht->entry_type = entry_type;
	pht->eval = eval;
	pht->depth = depth;

}
```

## AlphaBetaAndTT.h
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

// AlphaBetaAndTT.h: interface for the CAlphaBetaAndTT class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALPHABETAANDTT_H__8E8EE2C0_CEBC_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_ALPHABETAANDTT_H__8E8EE2C0_CEBC_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"
#include "TranspositionTable.h"

class CAlphaBetaAndTT : public CSearchEngine, public CTranspositionTable 
{
public:
	CAlphaBetaAndTT();
	virtual ~CAlphaBetaAndTT();

	virtual SearchAGoodMove(BYTE position[10][9]);
protected:
	int alphabeta(int depth, int alpha, int beta);
};

#endif // !defined(AFX_ALPHABETAANDTT_H__8E8EE2C0_CEBC_11D5_AEC7_5254AB2E22C7__INCLUDED_)
```
## AlphaBetaAndTT.cpp
这里的alpha-beta搜索实际上是Fail-soft alpha-beta形式的。
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

// AlphaBetaAndTT.cpp: implementation of the CAlphaBetaAndTT class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "AlphaBetaAndTT.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAlphaBetaAndTT::CAlphaBetaAndTT()
{
}

CAlphaBetaAndTT::~CAlphaBetaAndTT()
{
}


CAlphaBetaAndTT::SearchAGoodMove(BYTE position[10][9])
{
	memcpy(CurPosition, position, 90);
	CalculateInitHashKey(CurPosition);
	m_nMaxDepth = m_nSearchDepth;
	alphabeta(m_nMaxDepth, -20000, 20000);
	MakeMove(&m_cmBestMove);
	memcpy(position, CurPosition, 90);
}


int CAlphaBetaAndTT::alphabeta(int depth, int alpha, int beta)
{
	int score;
	int Count,i;
	BYTE type;
	int side;

	i = IsGameOver(CurPosition, depth);
	if (i != 0)
		return i;

	side = (m_nMaxDepth-depth)%2;

	score = LookUpHashTable(alpha, beta, depth, side); 
	if (score != 66666) 
		return score;

	if (depth <= 0)	//叶子节点取估值
	{
		score = m_pEval->Eveluate(CurPosition, side );
		EnterHashTable(exact, score, depth, side );
		return score;
	}



	Count = m_pMG->CreatePossibleMove(CurPosition, depth, side);
	
    int eval_is_exact = 0;

	for (i=0;i<Count;i++) 
	{
		Hash_MakeMove(&m_pMG->m_MoveList[depth][i], CurPosition);
		type = MakeMove(&m_pMG->m_MoveList[depth][i]);
		
		score = -alphabeta(depth - 1, -beta, -alpha);

		Hash_UnMakeMove(&m_pMG->m_MoveList[depth][i],type, CurPosition); 
		UnMakeMove(&m_pMG->m_MoveList[depth][i],type); 
        if (score >= beta) 
		{
			EnterHashTable(lower_bound, score, depth,side);
            return score;
        }

		if (score > alpha)
		{
			alpha = score;
			eval_is_exact = 1;
			if(depth == m_nMaxDepth)
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
		}

	}

    if (eval_is_exact) 
		EnterHashTable(exact, alpha, depth,side);
    else 
		EnterHashTable(upper_bound, alpha, depth,side);
	return alpha;
}
```

## 性能评估
第三层开始，置换表搜索评估的叶节点数目要少于alpha-beta搜索。并且随着搜索的最大深度的增加，置换表命中率提高，则搜索的叶节点数下降。到5层深度时，置换表的速度是alpha-beta的两倍。
在浅层深度的搜索当中，由于命中率较低，虽然置换表评估的节点数比alpha-beta要少，但是花费的时间仍然更多。