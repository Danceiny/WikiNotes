---
date: 2016-01-17 03:57
status: public
title: Chapter6-单搜索引擎实例
---

# 关于程序
**源代码基于VC,我计划在此基础上使用VS二次开发。以熟悉VS环境下的C++开发。**
利用Visual C++的Wizard建立一个使用MFC基于Dialog的EXE工程，取名Chess。VC生成的代码包括3个类CChessApp,CChessDlg,CAboutDlg。

# 程序结构 
[文件说明.txt](/人机博弈-游戏编程/_attachment/Chapter5 实例研究/文件说明.txt)
1. 将棋子的定义写成宏定义在一个头文件define.h中，并包含进chess.h中。

## define.h
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

//define.h // data structure and macros
#ifndef define_h_
#define define_h_

#define NOCHESS 0    //没有棋子

#define B_KING       1  //黑帅
#define B_CAR        2  //黑车
#define B_HORSE      3  //黑马
#define B_CANON      4  //黑炮
#define B_BISHOP     5  //黑士
#define B_ELEPHANT   6  //黑象
#define B_PAWN       7  //黑卒
#define B_BEGIN      B_KING
#define B_END        B_PAWN

#define R_KING       8   //红帅
#define R_CAR        9   //红车
#define R_HORSE      10  //红马
#define R_CANON      11  //红炮
#define R_BISHOP     12  //红士
#define R_ELEPHANT   13  //红象
#define R_PAWN       14  //红兵
#define R_BEGIN      R_KING
#define R_END        R_PAWN

#define IsBlack(x)  (x>=B_BEGIN&&x<=B_END)
#define IsRed(x)  (x>=R_BEGIN&&x<=R_END)
#define IsSameSide(x,y)  ((IsBlack(x)&&IsBlack(y))||(IsRed(x)&&IsRed(y)))

typedef struct _chessmanposition
{
	BYTE		x;
	BYTE		y;
}CHESSMANPOS;
//记录走法的结构
typedef struct _chessmove
{
	short	  	    ChessID;	//
	CHESSMANPOS	From;
	CHESSMANPOS	To;			//
	int			    Score;		// 
}CHESSMOVE;

#endif //define_h_
```
在这个头文件中还定义了一个结构，用以记录走法。
2. 接下来使用wizard向工程中增加4个新类（没有基类的普通类）：

* CMoveGenerator,走法生成器；
* CSearchEngine,搜索引擎的基类，定义接口及公用函数；
* CNegaMaxEngine,搜索引擎，使用负极大值方法，由CSearchEngine派生而来；
* CEveluation,估值核心

# CMoveGenerator 走法生成器
## CMoveGenerator.h
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

// MoveGenerator.h: interface for the CMoveGenerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVEGENERATOR_H__54A88FC2_CAFC_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_MOVEGENERATOR_H__54A88FC2_CAFC_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMoveGenerator  
{
public:
	CMoveGenerator();
	virtual ~CMoveGenerator();
	static BOOL IsValidMove(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY);
	int CreatePossibleMove(BYTE position[10][9], int nPly,int nSide);

	CHESSMOVE m_MoveList[10][80];
protected:

	void Gen_KingMove(BYTE position[10][9], int i, int j, int nPly);
	void Gen_RBishopMove(BYTE position[10][9], int i, int j, int nPly);
	void Gen_BBishopMove(BYTE position[10][9], int i, int j, int nPly);
	void Gen_ElephantMove(BYTE position[10][9], int i, int j, int nPly);
	void Gen_HorseMove(BYTE position[10][9],  int i, int j, int nPly);
	void Gen_CarMove(BYTE position[10][9],  int i, int j, int nPly);
	void Gen_RPawnMove(BYTE position[10][9],  int i, int j, int nPly);
	void Gen_BPawnMove(BYTE position[10][9], int i, int j, int nPly);
	void Gen_CanonMove(BYTE position[10][9], int i, int j, int nPly);

	int AddMove(int nFromX,int nToX, int nFromY, int nToY,int nPly);
	int m_nMoveCount;
};

#endif // !defined(AFX_MOVEGENERATOR_H__54A88FC2_CAFC_11D5_AEC7_5254AB2E22C7__INCLUDED_)

```
上面的这个头文件定义了走法产生器CMoveGenerator类，其包含两个重要成员函数：IsValidMove和CreatePossibleMove。前者未用到其他成员，被定义为静态的。
## MoveGenerator.cpp
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

// MoveGenerator.cpp: implementation of the CMoveGenerator class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "MoveGenerator.h"

#ifdef _DEBUG
#undef THIS_FILE
static BYTE THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMoveGenerator::CMoveGenerator()
{
	
}

CMoveGenerator::~CMoveGenerator()
{
	
}

int CMoveGenerator::AddMove(int nFromX, int nFromY, int nToX, int nToY,int nPly)
{
	m_MoveList[nPly][m_nMoveCount].From.x = nFromX;
	m_MoveList[nPly][m_nMoveCount].From.y = nFromY;
	m_MoveList[nPly][m_nMoveCount].To.x = nToX;
	m_MoveList[nPly][m_nMoveCount].To.y = nToY;
	m_nMoveCount++;
	return m_nMoveCount;
}

int CMoveGenerator::CreatePossibleMove(BYTE position[10][9], int nPly, int nSide)
{
	int     nChessID;
	int		i,j;
	
	m_nMoveCount = 0;
	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			if (position[i][j] != NOCHESS)
			{
				nChessID = position[i][j];
				if (!nSide && IsRed(nChessID))
					continue;
				if (nSide && IsBlack(nChessID))
					continue;
				switch(nChessID)
				{
				case R_KING:
				case B_KING:
					Gen_KingMove(position, i, j, nPly);
					break;
					
				case R_BISHOP:
					Gen_RBishopMove(position, i, j, nPly);
					break;
					
				case B_BISHOP:
					Gen_BBishopMove(position, i, j, nPly);
					break;
					
				case R_ELEPHANT:
				case B_ELEPHANT:
					Gen_ElephantMove(position, i, j, nPly);
					break;
					
				case R_HORSE:		
				case B_HORSE:		
					Gen_HorseMove(position, i, j, nPly);
					break;
					
				case R_CAR:
				case B_CAR:
					Gen_CarMove(position, i, j, nPly);
					break;
					
				case R_PAWN:
					Gen_RPawnMove(position, i, j, nPly);
					break;
					
				case B_PAWN:
					Gen_BPawnMove(position, i, j, nPly);
					break;
					
				case B_CANON:
				case R_CANON:
					Gen_CanonMove(position, i, j, nPly);
					break;
					
				default:
					break;
					
				}
			}
		}
		
		return m_nMoveCount;
}



void CMoveGenerator::Gen_KingMove(BYTE position[10][9], int i, int j, int nPly)
{
	int x,  y;
	for (y = 0; y < 3; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y))
				AddMove(j, i, x, y, nPly);

	for (y = 7; y < 10; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y))
				AddMove(j, i, x, y, nPly);
}

void CMoveGenerator::Gen_RBishopMove(BYTE position[10][9], int i, int j, int nPly)
{
	int x,  y;
	for (y = 7; y < 10; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y))
				AddMove(j, i, x, y, nPly);
}

void CMoveGenerator::Gen_BBishopMove(BYTE position[10][9], int i, int j, int nPly)
{
	int x,  y;
	for (y = 0; y < 3; y++)
		for (x = 3; x < 6; x++)
			if (IsValidMove(position, j, i, x, y))
				AddMove(j, i, x, y, nPly);
}

void CMoveGenerator::Gen_ElephantMove(BYTE position[10][9], int i, int j, int nPly)
{
	int x,  y;

	x=j+2;
	y=i+2;
	if(x < 9 && y < 10  && IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	
	x=j+2;
	y=i-2;
	if(x < 9 && y>=0  &&  IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	
	x=j-2;
	y=i+2;
	if(x>=0 && y < 10  && IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	
	x=j-2;
	y=i-2;
	if(x>=0 && y>=0  && IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	
}

void CMoveGenerator::Gen_HorseMove(BYTE position[10][9], int i, int j, int nPly)
{
	int x,  y;

	x=j+2;
	y=i+1;
	if((x < 9 && y < 10) &&IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	
	x=j+2;
	y=i-1;
	if((x < 9 && y >= 0) &&IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	
	x=j-2;
	y=i+1;
	if((x >= 0 && y < 10) &&IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	
	x=j-2;
	y=i-1;
	if((x >= 0 && y >= 0) &&IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	
	x=j+1;
	y=i+2;
	if((x < 9 && y < 10) &&IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	x=j-1;
	y=i+2;
	if((x >= 0 && y < 10) &&IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	x=j+1;
	y=i-2;
	if((x < 9 && y >= 0) &&IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	x=j-1;
	y=i-2;
	if((x >= 0 && y >= 0) &&IsValidMove(position, j, i, x, y))
		AddMove(j, i, x, y, nPly);
	
}

void CMoveGenerator::Gen_RPawnMove(BYTE position[10][9], int i, int j, int nPly)
{
	int x, y;
	int nChessID;

	nChessID = position[i][j];

	y = i - 1;
	x = j;
	
	if(y > 0 && !IsSameSide(nChessID, position[y][x]))
		AddMove(j, i, x, y, nPly);
	
	if(i < 5)
	{
		y=i;
		x=j+1;
		if(x < 9 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly);
		x=j-1;
		if(x >= 0 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly);
	}
}


void CMoveGenerator::Gen_BPawnMove(BYTE position[10][9], int i, int j, int nPly)
{
	int x, y;
	int nChessID;

	nChessID = position[i][j];

	y = i + 1;
	x = j;
	
	if(y < 10 && !IsSameSide(nChessID, position[y][x]))
		AddMove(j, i, x, y, nPly);
	
	if(i > 4)
	{
		y=i;
		x=j+1;
		if(x < 9 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly);
		x=j-1;
		if(x >= 0 && !IsSameSide(nChessID, position[y][x]))
			AddMove(j, i, x, y, nPly);
	}
	
}

void CMoveGenerator::Gen_CarMove(BYTE position[10][9], int i, int j, int nPly)
{
	int x,  y;
	int nChessID;

	nChessID = position[i][j];
	
	x=j+1;
	y=i;
	while(x < 9)
	{
		if( NOCHESS == position[y][x] )
			AddMove(j, i, x, y, nPly);
		else
		{
			if(!IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly);
			break;
		}
		x++;
	}
	
	x = j-1;
	y = i;
	while(x >= 0)
	{
		if( NOCHESS == position[y][x] )
			AddMove(j, i, x, y, nPly);
		else
		{
			if(!IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly);
			break;
		}
		x--;
	}
	
	x=j;
	y=i+1;//
	while(y < 10)
	{
		if( NOCHESS == position[y][x])
			AddMove(j, i, x, y, nPly);
		else
		{
			if(!IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly);
			break;
		}
		y++;
	}
	
	x = j;
	y = i-1;//
	while(y>=0)
	{
		if( NOCHESS == position[y][x])
			AddMove(j, i, x, y, nPly);
		else
		{
			if(!IsSameSide(nChessID, position[y][x]))
				AddMove(j, i, x, y, nPly);
			break;
		}
		y--;
	}
}

void CMoveGenerator::Gen_CanonMove(BYTE position[10][9], int i, int j, int nPly)
{
	int x, y;
	BOOL	flag;
	int nChessID;

	nChessID = position[i][j];
	
	x=j+1;		//
	y=i;
	flag=FALSE;
	while(x < 9)		
	{
		if( NOCHESS == position[y][x] )
		{
			if(!flag)
				AddMove(j, i, x, y, nPly);
		}
		else
		{
			if(!flag)
				flag=TRUE;
			else 
			{
				if(!IsSameSide(nChessID, position[y][x]))
					AddMove(j, i, x, y, nPly);
				break;
			}
		}
		x++;
	}
	
	x=j-1;
	flag=FALSE;	
	while(x>=0)
	{
		if( NOCHESS == position[y][x] )
		{
			if(!flag)
				AddMove(j, i, x, y, nPly);
		}
		else
		{
			if(!flag)
				flag=TRUE;
			else 
			{
				if(!IsSameSide(nChessID, position[y][x]))
					AddMove(j, i, x, y, nPly);
				break;
			}
		}
		x--;
	}
	x=j;	
	y=i+1;
	flag=FALSE;
	while(y < 10)
	{
		if( NOCHESS == position[y][x] )
		{
			if(!flag)
				AddMove(j, i, x, y, nPly);
		}
		else
		{
			if(!flag)
				flag=TRUE;
			else 
			{
				if(!IsSameSide(nChessID, position[y][x]))
					AddMove(j, i, x, y, nPly);
				break;
			}
		}
		y++;
	}
	
	y=i-1;	//
	flag=FALSE;	
	while(y>=0)
	{
		if( NOCHESS == position[y][x] )
		{
			if(!flag)
				AddMove(j, i, x, y, nPly);
		}
		else
		{
			if(!flag)
				flag=TRUE;
			else 
			{
				if(!IsSameSide(nChessID, position[y][x]))
					AddMove(j, i, x, y, nPly);
				break;
			}
		}
		y--;
	}
	
}

BOOL CMoveGenerator::IsValidMove(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY)
{
	int i, j;
	int nMoveChessID, nTargetID;
	
	if (nFromY ==  nToY && nFromX == nToX)
		return FALSE;//目的与源相同
	
	nMoveChessID = position[nFromY][nFromX];
	nTargetID = position[nToY][nToX];
	
	if (IsSameSide(nMoveChessID, nTargetID))
		return FALSE;//不能吃自己的棋
	
	switch(nMoveChessID)
	{
	case B_KING:     
		if (nTargetID == R_KING)//老将见面?
		{
			if (nFromX != nToX)
				return FALSE;
			for (i = nFromY + 1; i < nToY; i++)
				if (position[i][nFromX] != NOCHESS)
					return FALSE;
		}
		else
		{
			if (nToY > 2 || nToX > 5 || nToX < 3)
				return FALSE;//目标点在九宫之外
			if(abs(nFromY - nToY) + abs(nToX - nFromX) > 1) 
				return FALSE;//将帅只走一步直线:
		}
		break;
	case R_BISHOP:   
		
		if (nToY < 7 || nToX > 5 || nToX < 3)
			return FALSE;//士出九宫	
		
		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//士走斜线
		
		break;
		
	case B_BISHOP:   //黑士
		
		if (nToY > 2 || nToX > 5 || nToX < 3)
			return FALSE;//士出九宫	
		
		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//士走斜线
		
		break;
		
	case R_ELEPHANT://红象
		
		if(nToY < 5)
			return FALSE;//相不能过河
		
		if(abs(nFromX-nToX) != 2 || abs(nFromY-nToY) != 2)
			return FALSE;//相走田字
		
		if(position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//相眼被塞住了
		
		break;
		
	case B_ELEPHANT://黑象 
		
		if(nToY > 4)
			return FALSE;//相不能过河
		
		if(abs(nFromX-nToX) != 2 || abs(nFromY-nToY) != 2)
			return FALSE;//相走田字
		
		if(position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//相眼被塞住了
		
		break;
		
	case B_PAWN:     //黑兵
		
		if(nToY < nFromY)
			return FALSE;//兵不回头
		
		if( nFromY < 5 && nFromY == nToY)
			return FALSE;//兵过河前只能直走
		
		if(nToY - nFromY + abs(nToX - nFromX) > 1)
			return FALSE;//兵只走一步直线:
		
		break;
		
	case R_PAWN:    //红兵
		
		if(nToY > nFromY)
			return FALSE;//兵不回头
		
		if( nFromY > 4 && nFromY == nToY)
			return FALSE;//兵过河前只能直走
		
		if(nFromY - nToY + abs(nToX - nFromX) > 1)
			return FALSE;//兵只走一步直线:
		
		break;
		
	case R_KING:     
		if (nTargetID == B_KING)//老将见面?
		{
			if (nFromX != nToX)
				return FALSE;//两个将不在同一列
			for (i = nFromY - 1; i > nToY; i--)
				if (position[i][nFromX] != NOCHESS)
					return FALSE;//中间有别的子
		}
		else
		{
			if (nToY < 7 || nToX > 5 || nToX < 3)
				return FALSE;//目标点在九宫之外
			if(abs(nFromY - nToY) + abs(nToX - nFromX) > 1) 
				return FALSE;//将帅只走一步直线:
		}
		break;
		
	case B_CAR:      
	case R_CAR:      
		
		if(nFromY != nToY && nFromX != nToX)
			return FALSE;	//车走直线:
		
		if(nFromY == nToY)
		{
			if(nFromX < nToX)
			{
				for(i = nFromX + 1; i < nToX; i++)
					if(position[nFromY][i] != NOCHESS)
						return FALSE;
			}
			else
			{
				for(i = nToX + 1; i < nFromX; i++)
					if(position[nFromY][i] != NOCHESS)
						return FALSE;
			}
		}
		else
		{
			if(nFromY < nToY)
			{
				for(j = nFromY + 1; j < nToY; j++)
					if(position[j][nFromX] != NOCHESS)
						return FALSE;
			}
			else
			{
				for(j= nToY + 1; j < nFromY; j++)
					if(position[j][nFromX] != NOCHESS)
						return FALSE;
			}
		}
		
		break;
		
	case B_HORSE:    
	case R_HORSE:    
		
		if(!((abs(nToX-nFromX)==1 && abs(nToY-nFromY)==2)
			||(abs(nToX-nFromX)==2&&abs(nToY-nFromY)==1)))
			return FALSE;//马走日字
		
		if	(nToX-nFromX==2)
		{
			i=nFromX+1;
			j=nFromY;
		}
		else if	(nFromX-nToX==2)
		{
			i=nFromX-1;
			j=nFromY;
		}
		else if	(nToY-nFromY==2)
		{
			i=nFromX;
			j=nFromY+1;
		}
		else if	(nFromY-nToY==2)
		{
			i=nFromX;
			j=nFromY-1;
		}
		
		if(position[j][i] != NOCHESS)
			return FALSE;//绊马腿
		
		break;
	
	case B_CANON:    
	case R_CANON:    
		
		if(nFromY!=nToY && nFromX!=nToX)
			return FALSE;	//炮走直线:
		
		//炮不吃子时经过的路线中不能有棋子
		
		if(position[nToY][nToX] == NOCHESS)
		{
			if(nFromY == nToY)
			{
				if(nFromX < nToX)
				{
					for(i = nFromX + 1; i < nToX; i++)
						if(position[nFromY][i] != NOCHESS)
							return FALSE;
				}
				else
				{
					for(i = nToX + 1; i < nFromX; i++)
						if(position[nFromY][i]!=NOCHESS)
							return FALSE;
				}
			}
			else
			{
				if(nFromY < nToY)
				{
					for(j = nFromY + 1; j < nToY; j++)
						if(position[j][nFromX] != NOCHESS)
							return FALSE;
				}
				else
				{
					for(j = nToY + 1; j < nFromY; j++)
						if(position[j][nFromX] != NOCHESS)
							return FALSE;
				}
			}
		}
		//炮吃子时
		else	
		{
			int count=0;
			if(nFromY == nToY)
			{
				if(nFromX < nToX)
				{
					for(i=nFromX+1;i<nToX;i++)
						if(position[nFromY][i]!=NOCHESS)
							count++;
						if(count != 1)
							return FALSE;
				}
				else
				{
					for(i=nToX+1;i<nFromX;i++)
						if(position[nFromY][i] != NOCHESS)
							count++;
						if(count!=1)
							return FALSE;
				}
			}
			else
			{
				if(nFromY<nToY)
				{
					for(j=nFromY+1;j<nToY;j++)
						if(position[j][nFromX]!=NOCHESS)
							count++;
						if(count!=1)
							return FALSE;
				}
				else
				{
					for(j=nToY+1;j<nFromY;j++)
						if(position[j][nFromX] != NOCHESS)
							count++;
						if(count!=1)
							return FALSE;
				}
			}
		}
		break;
	default:
		return FALSE;
	}
	
	return TRUE;
}

```
# 搜索引擎
## SearchEngine.h
搜索引擎的接口类CSearchEngine定义了搜索引擎的接口和多种搜索引擎公用的函数。
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

// SearchEngine.h: interface for the CSearchEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SEARCHENGINE_H__2AF7A220_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_SEARCHENGINE_H__2AF7A220_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MoveGenerator.h"
#include "Eveluation.h"

class CSearchEngine  
{
public:
	CSearchEngine();
	virtual ~CSearchEngine();
	
	virtual SearchAGoodMove(BYTE position[10][9])=0;
	
	virtual SetSearchDepth(int nDepth){m_nSearchDepth = nDepth;};
	
	virtual SetEveluator(CEveluation *pEval){m_pEval = pEval;};

	virtual SetMoveGenerator(CMoveGenerator *pMG){m_pMG = pMG;};

protected:
	virtual BYTE MakeMove(CHESSMOVE* move);
	virtual void UnMakeMove(CHESSMOVE* move,BYTE type);
	virtual int IsGameOver(BYTE position[10][9], int nDepth);
	BYTE CurPosition[10][9];
	CHESSMOVE m_cmBestMove;
	CMoveGenerator *m_pMG;
	CEveluation *m_pEval;
	int m_nSearchDepth;
	int m_nMaxDepth;
};

#endif // !defined(AFX_SEARCHENGINE_H__2AF7A220_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_)
```
CSearchEngine类实现了3个成员函数：MakeMove和UnMakeMove这两个函数与走法生成器配合，以生成或撤销子节点。IsGameOver判断当前局面是否已分胜负。
## NegamaxEngine.h
负极大值算法的搜索核心类CNegamaxEngine类，由接口类派生。
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

// NegamaxEngine.h: interface for the CNegamaxEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEGAMAXENGINE_H__6C3A4902_CDED_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_NEGAMAXENGINE_H__6C3A4902_CDED_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SearchEngine.h"

class CNegamaxEngine : public CSearchEngine  
{
public:
	CNegamaxEngine();
	virtual ~CNegamaxEngine();

	virtual SearchAGoodMove(BYTE position[10][9]);

protected:
	int NegaMax(int depth);

};

#endif // !defined(AFX_NEGAMAXENGINE_H__6C3A4902_CDED_11D5_AEC7_5254AB2E22C7__INCLUDED_)

```
它继承了CSearchEngine的接口，并实现了函数SearchAGoodMove()，有一个包含负极大值算法的搜索函数NegaMax，SearchAGoodMove通过调用该算法实现搜索。
## NegamaxEngine.cpp
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

// NegamaxEngine.cpp: implementation of the CNegamaxEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "NegamaxEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNegamaxEngine::CNegamaxEngine()
{

}

CNegamaxEngine::~CNegamaxEngine()
{

}
CNegamaxEngine::SearchAGoodMove(BYTE position[10][9])
{
	m_nMaxDepth = m_nSearchDepth;

	memcpy(CurPosition, position, 90);
	
	NegaMax(m_nMaxDepth);
	
	MakeMove(&m_cmBestMove);
	memcpy(position, CurPosition, 90);
}
extern int count;

int CNegamaxEngine::NegaMax(int depth)
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
		score = -NegaMax(depth - 1);
		UnMakeMove(&m_pMG->m_MoveList[depth][i],type); 
		
		if (score > current)
		{
			current = score;
			if(depth == m_nMaxDepth)
			{
				m_cmBestMove = m_pMG->m_MoveList[depth][i];
			}
		}

	}

	return current;
}

```

## Evaluation.h
**原书作者对估值的英文全作eveluation，实乃谬误。特此更正。其他处若有此错误未正，定当注意。**
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

// Eveluation.h: interface for the CEveluation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVELUATION_H__2AF7A221_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_EVELUATION_H__2AF7A221_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define BASEVALUE_PAWN 100
#define BASEVALUE_BISHOP 250
#define BASEVALUE_ELEPHANT 250
#define BASEVALUE_CAR 500
#define BASEVALUE_HORSE 350
#define BASEVALUE_CANON 350
#define BASEVALUE_KING 10000

#define FLEXIBILITY_PAWN 15
#define FLEXIBILITY_BISHOP 1
#define FLEXIBILITY_ELEPHANT 1
#define FLEXIBILITY_CAR 6
#define FLEXIBILITY_HORSE 12
#define FLEXIBILITY_CANON 6
#define FLEXIBILITY_KING 0

class CEveluation  
{
public:
	CEveluation();
	virtual ~CEveluation();
	
	virtual int Eveluate(BYTE position[10][9], BOOL bIsRedTurn);

protected:
	int GetRelatePiece(BYTE position[10][9],int j, int i);
	BOOL CanTouch(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY);	
	AddPoint(int x, int y);
	int GetBingValue(int x, int y, BYTE CurSituation[][9]);
	int m_BaseValue[15];
	int m_FlexValue[15];
	short m_AttackPos[10][9];
	BYTE m_GuardPos[10][9];
	BYTE m_FlexibilityPos[10][9];
	int m_chessValue[10][9];
	int nPosCount;
	POINT RelatePos[20];

};

#endif // !defined(AFX_EVELUATION_H__2AF7A221_CB28_11D5_AEC7_5254AB2E22C7__INCLUDED_)
```
估值核心的定义包括了估值函数，是评价一个棋局优劣的核心代码。
## Evaluation.cpp
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

// Eveluation.cpp: implementation of the CEveluation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "chess.h"
#include "Eveluation.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int BA0[10][9]=
{
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
	{90,90,110,120,120,120,110,90,90},
	{90,90,110,120,120,120,110,90,90},
	{70,90,110,110,110,110,110,90,70},
	{70,70,70, 70, 70,  70, 70,70,70},
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
};

const int BA1[10][9]=
{
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
	{0,  0,  0,  0,  0,  0,  0,  0,  0},
	{70,70,70, 70, 70,70, 70,70, 70},
	{70,90,110,110,110,110,110,90,70},
	{90,90,110,120,120,120,110,90,90},
	{90,90,110,120,120,120,110,90,90},
	{0,  0, 0,  0,  0,  0,  0,  0,  0},
};

int CEveluation:: GetBingValue(int x, int y, BYTE CurSituation[][9])
{
	if (CurSituation[y][x] == R_PAWN)
		return BA0[y][x];
	
	if (CurSituation[y][x] == B_PAWN)
		return BA1[y][x];

	return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEveluation::CEveluation()
{
	m_BaseValue[B_KING] = BASEVALUE_KING; 
	m_BaseValue[B_CAR] = BASEVALUE_CAR; 
	m_BaseValue[B_HORSE] = BASEVALUE_HORSE; 
	m_BaseValue[B_BISHOP] = BASEVALUE_BISHOP; 
	m_BaseValue[B_ELEPHANT] = BASEVALUE_ELEPHANT; 
	m_BaseValue[B_CANON] = BASEVALUE_CANON; 
	m_BaseValue[B_PAWN] = BASEVALUE_PAWN; 

	m_BaseValue[R_KING] = BASEVALUE_KING; 
	m_BaseValue[R_CAR] = BASEVALUE_CAR; 
	m_BaseValue[R_HORSE] = BASEVALUE_HORSE; 
	m_BaseValue[R_BISHOP] = BASEVALUE_BISHOP; 
	m_BaseValue[R_ELEPHANT] = BASEVALUE_ELEPHANT; 
	m_BaseValue[R_CANON] = BASEVALUE_CANON; 
	m_BaseValue[R_PAWN] = BASEVALUE_PAWN; 

	m_FlexValue[B_KING] = FLEXIBILITY_KING; 
	m_FlexValue[B_CAR] = FLEXIBILITY_CAR; 
	m_FlexValue[B_HORSE] = FLEXIBILITY_HORSE; 
	m_FlexValue[B_BISHOP] = FLEXIBILITY_BISHOP; 
	m_FlexValue[B_ELEPHANT] = FLEXIBILITY_ELEPHANT; 
	m_FlexValue[B_CANON] = FLEXIBILITY_CANON; 
	m_FlexValue[B_PAWN] = FLEXIBILITY_PAWN; 

	m_FlexValue[R_KING] = FLEXIBILITY_KING; 
	m_FlexValue[R_CAR] = FLEXIBILITY_CAR; 
	m_FlexValue[R_HORSE] = FLEXIBILITY_HORSE; 
	m_FlexValue[R_BISHOP] = FLEXIBILITY_BISHOP; 
	m_FlexValue[R_ELEPHANT] = FLEXIBILITY_ELEPHANT; 
	m_FlexValue[R_CANON] = FLEXIBILITY_CANON; 
	m_FlexValue[R_PAWN] = FLEXIBILITY_PAWN; 

}

CEveluation::~CEveluation()
{
}
int count=0;
int CEveluation::Eveluate(BYTE position[10][9], BOOL bIsRedTurn)
{
	int i, j, k;
	int nChessType, nTargetType;
	count++;

	memset(m_chessValue,0, 360);
	memset(m_AttackPos,0, 180); 
	memset(m_GuardPos,0, 90); 
	memset(m_FlexibilityPos, 0, 90);

	for(i = 0; i < 10; i++)
		for(j = 0; j < 9; j++)
		{
			if(position[i][j] != NOCHESS)
			{
				nChessType = position[i][j];
				GetRelatePiece(position, j, i);
				for (k = 0; k < nPosCount; k++)
				{
					nTargetType = position[RelatePos[k].y][RelatePos[k].x];
					if (nTargetType == NOCHESS)
					{
						m_FlexibilityPos[i][j]++;	
					}
					else
					{
						if (IsSameSide(nChessType, nTargetType))
						{
							m_GuardPos[RelatePos[k].y][RelatePos[k].x]++;
						}else
						{
							m_AttackPos[RelatePos[k].y][RelatePos[k].x]++;
							m_FlexibilityPos[i][j]++;	
							switch (nTargetType)
							{
							case R_KING:
								if (!bIsRedTurn)
									return 18888;
								break;
							case B_KING:
								if (bIsRedTurn)
									return 18888;
								break;
							default:
								m_AttackPos[RelatePos[k].y][RelatePos[k].x] += (30 + (m_BaseValue[nTargetType] - m_BaseValue[nChessType])/10)/10;
								break;
							}
						}
					}
				}
			}
		}

	for(i = 0; i < 10; i++)
		for(j = 0; j < 9; j++)
		{
			if(position[i][j] != NOCHESS)
			{
				nChessType = position[i][j];
				m_chessValue[i][j]++;
				m_chessValue[i][j] += m_FlexValue[nChessType] * m_FlexibilityPos[i][j];
				m_chessValue[i][j] += GetBingValue(j, i, position);
			}
		}
	int nHalfvalue;
	for(i = 0; i < 10; i++)
		for(j = 0; j < 9; j++)
		{
			if(position[i][j] != NOCHESS)
			{
				nChessType = position[i][j];
				nHalfvalue = m_BaseValue[nChessType]/16;
				m_chessValue[i][j] += m_BaseValue[nChessType];
				
				if (IsRed(nChessType))
				{
					if (m_AttackPos[i][j])
					{
						if (bIsRedTurn)
						{
							if (nChessType == R_KING)
							{
								m_chessValue[i][j]-= 20;
							}
							else
							{
								m_chessValue[i][j] -= nHalfvalue * 2;
								if (m_GuardPos[i][j])
									m_chessValue[i][j] += nHalfvalue;
							}
						}
						else
						{
							if (nChessType == R_KING)
								return 18888;
							m_chessValue[i][j] -= nHalfvalue*10;
							if (m_GuardPos[i][j])
								m_chessValue[i][j] += nHalfvalue*9;
						}
						m_chessValue[i][j] -= m_AttackPos[i][j];
					}
					else
					{
						if (m_GuardPos[i][j])
							m_chessValue[i][j] += 5;
					}
				}
				else
				{
					if (m_AttackPos[i][j])
					{
						if (!bIsRedTurn)
						{
							if (nChessType == B_KING)									
							{
								m_chessValue[i][j]-= 20;
							}
							else
							{
								m_chessValue[i][j] -= nHalfvalue * 2;
								if (m_GuardPos[i][j])
									m_chessValue[i][j] += nHalfvalue;
							}
						}
						else
						{
							if (nChessType == B_KING)
								return 18888;
							m_chessValue[i][j] -= nHalfvalue*10;
							if (m_GuardPos[i][j])
								m_chessValue[i][j] += nHalfvalue*9;
						}
						m_chessValue[i][j] -= m_AttackPos[i][j];
					}
					else
					{
						if (m_GuardPos[i][j])
							m_chessValue[i][j] += 5;
					}
				}
			}
		}

	int nRedValue = 0; 
	int	nBlackValue = 0;

	for(i = 0; i < 10; i++)
		for(j = 0; j < 9; j++)
		{
			nChessType = position[i][j];
//			if (nChessType == R_KING || nChessType == B_KING)
//				m_chessValue[i][j] = 10000;	
			if (nChessType != NOCHESS)
			{
				if (IsRed(nChessType))
				{
					nRedValue += m_chessValue[i][j];	
				}
				else
				{
					nBlackValue += m_chessValue[i][j];	
				}
			}
		}
		if (bIsRedTurn)
		{
			return nRedValue - nBlackValue;
		}
		else
		{
			return  nBlackValue-nRedValue ;
		}
}





CEveluation::AddPoint(int  x, int y)
{
	RelatePos[nPosCount].x = x;
	RelatePos[nPosCount].y = y;
	nPosCount++;
}

int CEveluation::GetRelatePiece(BYTE position[10][9], int j, int i)
{
	nPosCount = 0;
	BYTE nChessID;
	BYTE flag;
	int x,y;
	
	nChessID = position[i][j];
	switch(nChessID)
	{
	case R_KING:
	case B_KING:
		
		for (y = 0; y < 3; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))
					AddPoint(x, y);
		for (y = 7; y < 10; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))
					AddPoint(x, y);
		break;
						
	case R_BISHOP:
		
		for (y = 7; y < 10; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))
					AddPoint(x, y);
		break;
				
	case B_BISHOP:
		
		for (y = 0; y < 3; y++)
			for (x = 3; x < 6; x++)
				if (CanTouch(position, j, i, x, y))
					AddPoint(x, y);
		break;
				
	case R_ELEPHANT:
	case B_ELEPHANT:
		
		x=j+2;
		y=i+2;
		if(x < 9 && y < 10  && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		
		x=j+2;
		y=i-2;
		if(x < 9 && y>=0  &&  CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		
		x=j-2;
		y=i+2;
		if(x>=0 && y < 10  && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		
		x=j-2;
		y=i-2;
		if(x>=0 && y>=0  && CanTouch(position, j, i, x, y))
			AddPoint(x, y);
		break;
		
		case R_HORSE:		
		case B_HORSE:		
			x=j+2;
			y=i+1;
			if((x < 9 && y < 10) &&CanTouch(position, j, i, x, y))
				AddPoint(x, y);
					
					x=j+2;
					y=i-1;
					if((x < 9 && y >= 0) &&CanTouch(position, j, i, x, y))
						AddPoint(x, y);
					
					x=j-2;
					y=i+1;
					if((x >= 0 && y < 10) &&CanTouch(position, j, i, x, y))
						AddPoint(x, y);
					
					x=j-2;
					y=i-1;
					if((x >= 0 && y >= 0) &&CanTouch(position, j, i, x, y))
						AddPoint(x, y);
					
					x=j+1;
					y=i+2;
					if((x < 9 && y < 10) &&CanTouch(position, j, i, x, y))
						AddPoint(x, y);
					x=j-1;
					y=i+2;
					if((x >= 0 && y < 10) &&CanTouch(position, j, i, x, y))
						AddPoint(x, y);
					x=j+1;
					y=i-2;
					if((x < 9 && y >= 0) &&CanTouch(position, j, i, x, y))
						AddPoint(x, y);
					x=j-1;
					y=i-2;
					if((x >= 0 && y >= 0) &&CanTouch(position, j, i, x, y))
						AddPoint(x, y);
					break;
					
				case R_CAR:
				case B_CAR:
					x=j+1;
					y=i;
					while(x < 9)
					{
						if( NOCHESS == position[y][x] )
							AddPoint(x, y);
						else
						{
							AddPoint(x, y);
							break;
						}
						x++;
					}
					
					x = j-1;
					y = i;
					while(x >= 0)
					{
						if( NOCHESS == position[y][x] )
							AddPoint(x, y);
						else
						{
							AddPoint(x, y);
							break;
						}
						x--;
					}
					
					x=j;
					y=i+1;//
					while(y < 10)
					{
						if( NOCHESS == position[y][x])
							AddPoint(x, y);
						else
						{
							AddPoint(x, y);
							break;
						}
						y++;
					}
					
					x = j;
					y = i-1;//よ
					while(y>=0)
					{
						if( NOCHESS == position[y][x])
							AddPoint(x, y);
						else
						{
							AddPoint(x, y);
							break;
						}
						y--;
					}
					break;
					
				case R_PAWN:
					y = i - 1;
					x = j;
					
					if(y >= 0)
						AddPoint(x, y);
					
					if(i < 5)
					{
						y=i;
						x=j+1;
						if(x < 9 )
							AddPoint(x, y);
						x=j-1;
						if(x >= 0 )
							AddPoint(x, y);
					}
					break;
					
				case B_PAWN:
					y = i + 1;
					x = j;
					
					if(y < 10 )
						AddPoint(x, y);
					
					if(i > 4)
					{
						y=i;
						x=j+1;
						if(x < 9)
							AddPoint(x, y);
						x=j-1;
						if(x >= 0)
							AddPoint(x, y);
					}
					break;
					
				case B_CANON:
				case R_CANON:
					
					x=j+1;		//
					y=i;
					flag=FALSE;
					while(x < 9)		
					{
						if( NOCHESS == position[y][x] )
						{
							if(!flag)
								AddPoint(x, y);
						}
						else
						{
							if(!flag)
								flag=TRUE;
							else 
							{
								AddPoint(x, y);
								break;
							}
						}
						x++;
					}
					
					x=j-1;
					flag=FALSE;	
					while(x>=0)
					{
						if( NOCHESS == position[y][x] )
						{
							if(!flag)
								AddPoint(x, y);
						}
						else
						{
							if(!flag)
								flag=TRUE;
							else 
							{
								AddPoint(x, y);
								break;
							}
						}
						x--;
					}
					x=j;	
					y=i+1;
					flag=FALSE;
					while(y < 10)
					{
						if( NOCHESS == position[y][x] )
						{
							if(!flag)
								AddPoint(x, y);
						}
						else
						{
							if(!flag)
								flag=TRUE;
							else 
							{
								AddPoint(x, y);
								break;
							}
						}
						y++;
					}
					
					y=i-1;	//
					flag=FALSE;	
					while(y>=0)
					{
						if( NOCHESS == position[y][x] )
						{
							if(!flag)
								AddPoint(x, y);
						}
						else
						{
							if(!flag)
								flag=TRUE;
							else 
							{
								AddPoint(x, y);
								break;
							}
						}
						y--;
					}
					break;
					
				default:
					break;
					
				}
				return nPosCount ;				
}

BOOL CEveluation::CanTouch(BYTE position[10][9], int nFromX, int nFromY, int nToX, int nToY)
{
	int i, j;
	int nMoveChessID, nTargetID;
	
	if (nFromY ==  nToY && nFromX == nToX)
		return FALSE;//目的与源相同
	
	nMoveChessID = position[nFromY][nFromX];
	nTargetID = position[nToY][nToX];
	
	switch(nMoveChessID)
	{
	case B_KING:     
		if (nTargetID == R_KING)//老将见面?
		{
			if (nFromX != nToX)
				return FALSE;
			for (i = nFromY + 1; i < nToY; i++)
				if (position[i][nFromX] != NOCHESS)
					return FALSE;
		}
		else
		{
			if (nToY > 2 || nToX > 5 || nToX < 3)
				return FALSE;//目标点在九宫之外
			if(abs(nFromY - nToY) + abs(nToX - nFromX) > 1) 
				return FALSE;//将帅只走一步直线:
		}
		break;
	case R_BISHOP:   
		
		if (nToY < 7 || nToX > 5 || nToX < 3)
			return FALSE;//士出九宫	
		
		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//士走斜线
		
		break;
		
	case B_BISHOP:   //黑士
		
		if (nToY > 2 || nToX > 5 || nToX < 3)
			return FALSE;//士出九宫	
		
		if (abs(nFromY - nToY) != 1 || abs(nToX - nFromX) != 1)
			return FALSE;	//士走斜线
		
		break;
		
	case R_ELEPHANT://红象
		
		if(nToY < 5)
			return FALSE;//相不能过河
		
		if(abs(nFromX-nToX) != 2 || abs(nFromY-nToY) != 2)
			return FALSE;//相走田字
		
		if(position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//相眼被塞住了
		
		break;
		
	case B_ELEPHANT://黑象 
		
		if(nToY > 4)
			return FALSE;//相不能过河
		
		if(abs(nFromX-nToX) != 2 || abs(nFromY-nToY) != 2)
			return FALSE;//相走田字
		
		if(position[(nFromY + nToY) / 2][(nFromX + nToX) / 2] != NOCHESS)
			return FALSE;//相眼被塞住了
		
		break;
		
	case B_PAWN:     //黑兵
		
		if(nToY < nFromY)
			return FALSE;//兵不回头
		
		if( nFromY < 5 && nFromY == nToY)
			return FALSE;//兵过河前只能直走
		
		if(nToY - nFromY + abs(nToX - nFromX) > 1)
			return FALSE;//兵只走一步直线:
		
		break;
		
	case R_PAWN:    //红兵
		
		if(nToY > nFromY)
			return FALSE;//兵不回头
		
		if( nFromY > 4 && nFromY == nToY)
			return FALSE;//兵过河前只能直走
		
		if(nFromY - nToY + abs(nToX - nFromX) > 1)
			return FALSE;//兵只走一步直线:
		
		break;
		
	case R_KING:     
		if (nTargetID == B_KING)//老将见面?
		{
			if (nFromX != nToX)
				return FALSE;//两个将不在同一列
			for (i = nFromY - 1; i > nToY; i--)
				if (position[i][nFromX] != NOCHESS)
					return FALSE;//中间有别的子
		}
		else
		{
			if (nToY < 7 || nToX > 5 || nToX < 3)
				return FALSE;//目标点在九宫之外
			if(abs(nFromY - nToY) + abs(nToX - nFromX) > 1) 
				return FALSE;//将帅只走一步直线:
		}
		break;
		
	case B_CAR:      
	case R_CAR:      
		
		if(nFromY != nToY && nFromX != nToX)
			return FALSE;	//车走直线:
		
		if(nFromY == nToY)
		{
			if(nFromX < nToX)
			{
				for(i = nFromX + 1; i < nToX; i++)
					if(position[nFromY][i] != NOCHESS)
						return FALSE;
			}
			else
			{
				for(i = nToX + 1; i < nFromX; i++)
					if(position[nFromY][i] != NOCHESS)
						return FALSE;
			}
		}
		else
		{
			if(nFromY < nToY)
			{
				for(j = nFromY + 1; j < nToY; j++)
					if(position[j][nFromX] != NOCHESS)
						return FALSE;
			}
			else
			{
				for(j= nToY + 1; j < nFromY; j++)
					if(position[j][nFromX] != NOCHESS)
						return FALSE;
			}
		}
		
		break;
		
	case B_HORSE:    
	case R_HORSE:    
		
		if(!((abs(nToX-nFromX)==1 && abs(nToY-nFromY)==2)
			||(abs(nToX-nFromX)==2&&abs(nToY-nFromY)==1)))
			return FALSE;//马走日字
		
		if	(nToX-nFromX==2)
		{
			i=nFromX+1;
			j=nFromY;
		}
		else if	(nFromX-nToX==2)
		{
			i=nFromX-1;
			j=nFromY;
		}
		else if	(nToY-nFromY==2)
		{
			i=nFromX;
			j=nFromY+1;
		}
		else if	(nFromY-nToY==2)
		{
			i=nFromX;
			j=nFromY-1;
		}
		
		if(position[j][i] != (BYTE)NOCHESS)
			return FALSE;//绊马腿
		
		break;
	case B_CANON:    
	case R_CANON:    
		
		if(nFromY!=nToY && nFromX!=nToX)
			return FALSE;	//炮走直线:
		
		//炮不吃子时经过的路线中不能有棋子:------------------
		
		if(position[nToY][nToX] == NOCHESS)
		{
			if(nFromY == nToY)
			{
				if(nFromX < nToX)
				{
					for(i = nFromX + 1; i < nToX; i++)
						if(position[nFromY][i] != NOCHESS)
							return FALSE;
				}
				else
				{
					for(i = nToX + 1; i < nFromX; i++)
						if(position[nFromY][i]!=NOCHESS)
							return FALSE;
				}
			}
			else
			{
				if(nFromY < nToY)
				{
					for(j = nFromY + 1; j < nToY; j++)
						if(position[j][nFromX] != NOCHESS)
							return FALSE;
				}
				else
				{
					for(j = nToY + 1; j < nFromY; j++)
						if(position[j][nFromX] != NOCHESS)
							return FALSE;
				}
			}
		}
		//以上是炮不吃子-------------------------------------
		//吃子时:=======================================
		else	
		{
			int nCount=0;
			if(nFromY == nToY)
			{
				if(nFromX < nToX)
				{
					for(i=nFromX+1;i<nToX;i++)
						if(position[nFromY][i]!=NOCHESS)
							nCount++;
						if(nCount != 1)
							return FALSE;
				}
				else
				{
					for(i=nToX+1;i<nFromX;i++)
						if(position[nFromY][i] != NOCHESS)
							nCount++;
						if(nCount!=1)
							return FALSE;
				}
			}
			else
			{
				if(nFromY<nToY)
				{
					for(j=nFromY+1;j<nToY;j++)
						if(position[j][nFromX]!=NOCHESS)
							nCount++;
						if(nCount!=1)
							return FALSE;
				}
				else
				{
					for(j=nToY+1;j<nFromY;j++)
						if(position[j][nFromX] != NOCHESS)
							nCount++;
						if(nCount!=1)
							return FALSE;
				}
			}
		}
		//以上是炮吃子时================================
		break;
	default:
		return FALSE;
	}
	
	return TRUE;
}

```
# 操作界面
原程序默认电脑执黑棋，不能更换。按下New Game按钮后选择搜索引擎和搜索深度。
在工程中加入一个新的对话框（Dialog）资源，在对话框中加入：
1. 一个listbox用以列出供选择的搜索引擎；
2. 一个edit控件，再加入一个spinbutton控件来选择搜索层数。
对话框的ID设为IDD_NEWGAME。
使用wizard建立基于此模板的对话框类CNewGame。

## NewGame.h
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

#if !defined(AFX_NEWGAME_H__B0962A80_D557_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_NEWGAME_H__B0962A80_D557_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewGame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewGame dialog

class CNewGame : public CDialog
{
// Construction
public:

	CNewGame(CWnd* pParent = NULL);   // standard constructor
	int GetSelectedEngine(){return m_nSelectedEngine;};
	int GetSelectedPly(){return m_nSelectedPly;};

// Dialog Data
	//{{AFX_DATA(CNewGame)
	enum { IDD = IDD_NEWGAME };
	CSpinButtonCtrl	m_SetPly;
	CListBox	m_SearchEngineList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewGame)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nSelectedEngine;
	int m_nSelectedPly;
	// Generated message map functions
	//{{AFX_MSG(CNewGame)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWGAME_H__B0962A80_D557_11D5_AEC7_5254AB2E22C7__INCLUDED_)
```
New Game对话框类的实现部分NewGame.cpp同头文件一样，大部分代码由wizard自动生成。
## NewGame.cpp
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

// NewGame.cpp : implementation file
//

#include "stdafx.h"
#include "chess.h"
#include "NewGame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewGame dialog


CNewGame::CNewGame(CWnd* pParent /*=NULL*/)
	: CDialog(CNewGame::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewGame)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNewGame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewGame)
	DDX_Control(pDX, IDC_PLY, m_SetPly);
	DDX_Control(pDX, IDC_LISTENGINE, m_SearchEngineList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewGame, CDialog)
	//{{AFX_MSG_MAP(CNewGame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewGame message handlers

void CNewGame::OnOK() 
{
	// TODO: Add extra validation here
	m_nSelectedEngine = m_SearchEngineList.GetCurSel();
	m_nSelectedPly = m_SetPly.GetPos();
	 
	CDialog::OnOK();
}

BOOL CNewGame::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_SearchEngineList.AddString("Negamax Search Engine");
	m_SearchEngineList.AddString("AlphaBeta Search Engine");
	m_SearchEngineList.AddString("Fail-Soft Alpha-Beta Search Engine");
	m_SearchEngineList.AddString("Aspiration Search Engine");
	m_SearchEngineList.AddString("PVS Search Engine");
	m_SearchEngineList.AddString("Iterative Deepening Alphabeta Search");
	m_SearchEngineList.AddString("AlphaBeta + TT Search Engine");
	m_SearchEngineList.AddString("Alphabeta + History Heuristic Search");
	m_SearchEngineList.AddString("Mdt(f) Search Engine");
	m_SearchEngineList.AddString("NegaScout + TT + HH");
	
	m_SearchEngineList.SetCurSel(0);
	m_SetPly.SetRange(1, 15);
	m_SetPly.SetPos(3);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

```
## ChessDlg
其他界面的处理放在CChessDlg这个类当中。在这个类中加入一个棋盘定义，将棋盘定义为一个9x10字节的二维数组，作为CChessDlg类的私有成员变量。接下来加入一幅新的BITMAP资源。绘制一个每个格子为38x38像素的简单棋盘，将这个BITMAP资源的ID设为IDB_CHESSARD。再向工程中加入一幅新的BITMAP图，在棋盘上绘制14种棋子，每个大小为36x36像素。边缘外填充绿色，RGB为(0,255,0),将这个BITMAP资源的ID设为IDB_CHESSMAN。

![](~/bitmap1.bmp)

![](~/bmp00001.bmp)
下面的chessDlg,chessDlg类是由VC在创建工程时自动生成的。然后使用wizard加入对WM_MOUSE;WM_LBUTTONDOWN;WM_LBUTTONUP;以及WM_INITDIALOG消息的响应函数，和对按钮New Game的响应函数。这个类中包含了绝大部分有关界面的内容。
## chessDlg.h
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

// chessDlg.h : header file
//

#if !defined(AFX_CHESSDLG_H__2B09B234_CA39_11D5_AEC7_5254AB2E22C7__INCLUDED_)
#define AFX_CHESSDLG_H__2B09B234_CA39_11D5_AEC7_5254AB2E22C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "define.h"
#include "SearchEngine.h"
#include "NegamaxEngine.h"
#include "AlphaBetaEngine.h"
#include "AlphaBetaAndTT.h"
#include "FAlphaBetaEngine.h"
#include "PVS_Engine.h"
#include "NegaScout.h"
#include "IDAlphabeta.h"
#include "AspirationSearch.h"
#include "Alphabeta_HH.h"
#include "mtd_f.h"
#include "NegaScout_TT_HH.h"

typedef struct _movechess
{
	BYTE nChessID;
	POINT ptMovePoint;
}MOVECHESS;
/////////////////////////////////////////////////////////////////////////////
// CChessDlg dialog
class CChessDlg : public CDialog
{
// Construction
public:

	CChessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CChessDlg)
	enum { IDD = IDD_CHESS_DIALOG };
	CStatic	m_OutputInfo;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CChessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNewgame();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bGameOver;
	BYTE m_ChessBoard[10][9];
	BYTE m_BackupChessBoard[10][9];
	MOVECHESS m_MoveChess;
	POINT m_ptMoveChess;
	CBitmap m_BoardBmp;
	CImageList m_Chessman;
	int m_nBoardWidth; 
	int m_nBoardHeight;
	CSearchEngine *m_pSE;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHESSDLG_H__2B09B234_CA39_11D5_AEC7_5254AB2E22C7__INCLUDED_)
```

## chessDlg.cpp
CChessDlg的实现部分。除了实现对界面的绘制和对鼠标消息的响应函数外，还定义了一组宏给定棋盘边框的宽度，以及一个常量数组，包含了初始的棋盘状态。
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

// chessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chess.h"
#include "chessDlg.h"
#include "newgame.h"
#include "MoveGenerator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

#define BORDERWIDTH 15 //棋盘(左右)边缘的宽度
#define BORDERHEIGHT 14 //棋盘(上下)边缘的高度
#define GRILLEWIDTH 39  //棋盘上每个格子的高度
#define GRILLEHEIGHT 39 //棋盘上每个格子的宽度
const BYTE InitChessBoard[10][9]=
{
	{B_CAR,   B_HORSE, B_ELEPHANT, B_BISHOP, B_KING,  B_BISHOP, B_ELEPHANT, B_HORSE, B_CAR},
	{NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS},
	{NOCHESS, B_CANON, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    B_CANON, NOCHESS},
	{B_PAWN,  NOCHESS, B_PAWN,     NOCHESS,  B_PAWN,  NOCHESS,  B_PAWN,     NOCHESS, B_PAWN},
	{NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS},
	
	{NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS},
	{R_PAWN,  NOCHESS, R_PAWN,     NOCHESS,  R_PAWN,  NOCHESS,  R_PAWN,     NOCHESS, R_PAWN},
	{NOCHESS, R_CANON, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    R_CANON, NOCHESS},
	{NOCHESS, NOCHESS, NOCHESS,    NOCHESS,  NOCHESS, NOCHESS,  NOCHESS,    NOCHESS, NOCHESS},
	{R_CAR,   R_HORSE, R_ELEPHANT, R_BISHOP, R_KING,  R_BISHOP, R_ELEPHANT, R_HORSE, R_CAR}
};

/////////////////////////////////////////////////////////////////////////////
// CChessDlg dialog


////added by Spring Wang this code does not contents in books
int IsGameOver(BYTE position[10][9])
{
	int i,j;
	BOOL RedLive = FALSE, BlackLive=FALSE; 
	for (i = 7; i < 10; i++)
		for (j = 3; j < 6; j++)
		{
			if (position[i][j] == B_KING)
				BlackLive = TRUE;
			if (position[i][j] == R_KING)
				RedLive  = TRUE;
		}

	for (i = 0; i < 3; i++)
		for (j = 3; j < 6; j++)
		{
			if (position[i][j] == B_KING)
				BlackLive = TRUE;
			if (position[i][j] == R_KING)
				RedLive  = TRUE;
		}
	if (RedLive && BlackLive)
		return FALSE;
	else
		return TRUE;
}
////added by Spring Wang this code does not contents in books



CChessDlg::CChessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChessDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChessDlg)
	DDX_Control(pDX, IDC_NODECOUNT, m_OutputInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChessDlg, CDialog)
	//{{AFX_MSG_MAP(CChessDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_NEWGAME, OnNewgame)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChessDlg message handlers

BOOL CChessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_Chessman.Create(IDB_CHESSMAN, 36, 14, RGB(0,255,0)); 

	BITMAP BitMap;
	m_BoardBmp.LoadBitmap(IDB_CHESSBOARD);
	m_BoardBmp.GetBitmap(&BitMap);
	m_nBoardWidth =  BitMap.bmWidth; 
	m_nBoardHeight = BitMap.bmHeight;
	m_BoardBmp.DeleteObject();


	memcpy(m_ChessBoard, InitChessBoard, 90);//初始化棋盘
	
	CMoveGenerator *pMG;
	CEveluation *pEvel;

	m_pSE = new CNegamaxEngine;
	pMG = new CMoveGenerator;
	pEvel = new CEveluation;
	
	m_pSE->SetSearchDepth(3);
	m_pSE->SetMoveGenerator(pMG);
	m_pSE->SetEveluator(pEvel);
	m_MoveChess.nChessID = NOCHESS;
	m_bGameOver = FALSE;//this code does not contents in books.

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChessDlg::OnPaint() 
{
	CPaintDC dc(this);
	CDC MemDC;
	int i, j;
	POINT pt;
	CBitmap *pOldBmp;
	
	MemDC.CreateCompatibleDC( &dc );
	m_BoardBmp.LoadBitmap(IDB_CHESSBOARD);
	pOldBmp = MemDC.SelectObject(&m_BoardBmp);
	for (i = 0; i < 10; i++)
		for (j = 0; j < 9; j++)
		{
			if (m_ChessBoard[i][j] == NOCHESS)
				continue;
			pt.x = j*GRILLEHEIGHT+14;
			pt.y = i*GRILLEWIDTH+15;
			m_Chessman.Draw(&MemDC, m_ChessBoard[i][j]-1, pt, ILD_TRANSPARENT);		
		}
		if (m_MoveChess.nChessID != NOCHESS)
			m_Chessman.Draw(&MemDC, m_MoveChess.nChessID - 1, m_MoveChess.ptMovePoint, ILD_TRANSPARENT);		
		
		dc.BitBlt(0, 0, m_nBoardWidth, m_nBoardHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(&pOldBmp);
		MemDC.DeleteDC();
		m_BoardBmp.DeleteObject();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CChessDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bGameOver)
		return;
	memcpy(m_BackupChessBoard, m_ChessBoard, 90);
	if (point.x > 0 && point.y > 0 &&point.x< m_nBoardWidth && point.y< m_nBoardHeight && 
		IsRed(m_ChessBoard[(point.y-14)/GRILLEHEIGHT][(point.x-15)/GRILLEWIDTH]))
	{
		memcpy(m_BackupChessBoard, m_ChessBoard, 90);
		m_ptMoveChess.x = (point.x-15)/GRILLEWIDTH;
		m_ptMoveChess.y = (point.y-14)/GRILLEHEIGHT;
		m_MoveChess.nChessID = m_ChessBoard[m_ptMoveChess.y][m_ptMoveChess.x];
		m_ChessBoard[m_ptMoveChess.y][m_ptMoveChess.x] = NOCHESS;
		point.x -= 18;
		point.y -= 18;
		m_MoveChess.ptMovePoint = point;
		InvalidateRect(NULL,FALSE);
		UpdateWindow();
		SetCapture();
	}
	CDialog::OnLButtonDown(nFlags, point);
}
extern int count;

void CChessDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	BOOL bTurnSide = FALSE; 

	if (m_bGameOver)
		return;

	int timecount;
	
	if (m_MoveChess.nChessID &&
		CMoveGenerator::IsValidMove(m_BackupChessBoard, m_ptMoveChess.x, m_ptMoveChess.y,
		(point.x-15)/GRILLEWIDTH,(point.y-14)/GRILLEHEIGHT))
	{
		m_ChessBoard[(point.y-14)/GRILLEHEIGHT][(point.x-15)/GRILLEWIDTH] = m_MoveChess.nChessID;
		bTurnSide = TRUE;
	}
	else
		memcpy(m_ChessBoard, m_BackupChessBoard, 90);
	
	m_MoveChess.nChessID = NOCHESS;
	InvalidateRect(NULL,FALSE);
	UpdateWindow();
	
	ReleaseCapture();
	if (bTurnSide == TRUE)
	{
		timecount = GetTickCount();
		m_pSE->SearchAGoodMove(m_ChessBoard);	
		
		CString sNodeCount;

		sNodeCount.Format(" Cost %d ms. %d Nodes were eveluated.", GetTickCount() - timecount,count);
		m_OutputInfo.SetWindowText(sNodeCount);
	}
	
	if (IsGameOver(m_ChessBoard))
	{
		m_bGameOver = TRUE;
		MessageBox("Game Over! Press New Game to replay...");
	}
	
	count = 0;
	InvalidateRect(NULL, FALSE);
	UpdateWindow();
	CDialog::OnLButtonUp(nFlags, point);
}

void CChessDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_MoveChess.nChessID)
	{
		//防止将棋子拖出棋盘
		if( point.x < 15)
			point.x = 15;
		if( point.y < 15)
			point.y = 15;
		if( point.x > m_nBoardWidth - 15)
			point.x = m_nBoardWidth - 15;
		if( point.y > m_nBoardHeight - 15)
			point.y = m_nBoardHeight - 15;

		point.x -= 18;
		point.y -= 18;
		m_MoveChess.ptMovePoint = point;
		InvalidateRect(NULL,FALSE);//刷新窗口
		UpdateWindow();
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CChessDlg::OnNewgame() 
{
	// TODO: Add your control notification handler code here
	CMoveGenerator * pMG;
	CEveluation *pEvel;

	CNewGame newGameDlg;
	if (  newGameDlg.DoModal() == IDOK)
	{
		if (m_pSE)
			delete m_pSE;
		switch(newGameDlg.GetSelectedEngine())
		{
		case 0:
			m_pSE = new CNegamaxEngine;
			break;

		case 1:
			m_pSE = new CAlphaBetaEngine;
			break;

		case 2:
			m_pSE = new CFAlphaBetaEngine;
			break;

		case 3:
			m_pSE = new CAspirationSearch;
			break;

		case 4:
			m_pSE = new CPVS_Engine;
			break;

		case 5:
			m_pSE = new CIDAlphabeta;
			break;

		case 6:
			m_pSE = new CAlphaBetaAndTT;
			break;

		case 7:
			m_pSE = new CAlphabeta_HH;
			break;

		case 8:
			m_pSE = new CMTD_f;
			break;

		case 9:
			m_pSE = new CNegaScout_TT_HH;
			break;
		}
		m_pSE->SetSearchDepth(newGameDlg.GetSelectedPly());
		pEvel = new CEveluation;
	}
	else 
		return;

	memcpy(m_ChessBoard, InitChessBoard, 90);//初始化棋盘
	m_MoveChess.nChessID = NOCHESS;

	pMG = new CMoveGenerator;
	m_pSE->SetMoveGenerator(pMG);
	m_pSE->SetEveluator(pEvel);
	
	m_bGameOver = FALSE;//this code does not contents in books.

	InvalidateRect(NULL,FALSE);
	UpdateWindow();
}
```
# 初步试用
以上已经建立了一个能初步运行的象棋程序。但性能十分不理想。