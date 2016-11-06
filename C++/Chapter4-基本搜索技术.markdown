---
date: 2016-01-17 02:21
status: public
title: Chapter4-基本搜索技术
---

# Search
Blind Search 盲目搜索
Adversarial Search 对抗性搜索
# AND/OR TREE
甲乙对弈。
如果令甲胜的局面值为WIN,乙胜的局面值为LOST,和局为DRAW。轮到甲走时，甲会选择子节点值为WIN或DRAW(如果没有WIN)的走法；轮到乙时亦然。
对于中间节点的值有以下计算方法：
如果该节点所对应的局面轮到甲走棋，则该节点的值是其所有子节点中值最佳的一个值；如果对应轮到乙走棋，则是所有子节点中值最差的一个值。通过这种方法，从叶节点倒推到根部，就可以得到所有节点的值。
博弈树是与或树，AND/OR tree。一方做or的选择，并and对方的选择，进行博弈。
_大多数棋类游戏，都没有建立完全搜索树的可能。_
# Minimax Algorithm
Static Evaluation Function：静态估值函数。对走法的评价，win为+∞，lost为-∞，draw为0，其他情形依据双方剩余棋子的数量及位置评定具体分值。
这样建立一棵固定深度的搜索树，其叶子节点不必是终了状态，而只是固定深度的最深一层的节点。
**Heuristic Search[^function]**:利用具体的知识构成评估函数的搜索。
# Depth First Search
深度优先搜索极大极小树的过程，可以表示为一个递归形式。
深度优先搜索极大极小树的过程中，任何时候只要保存与其层数相同个数的节点。仅生成将要搜索的节点，搜索完成的节点可以立即删去以节省空间。
下面给出应用极大极小算法的类C伪代码：
```C:n
int MiniMax(position p, int d)
{
    int bestvalue, value;
    if(Game Over)//检查棋局是否结束
        return evaluation(p);//棋局结束，返回估值
    if(depth <= 0)//是否是叶节点
        return evaluation(p);//为叶子节点，返回估值
    if(p.color == RED)//是否轮到红方走棋
        bestvalue = -INFINITY;//是，令初始最大值为极小
    else
        bestvalue = INFINITY;//否
    for(each possibly move m)//对每一可能的走法m
    {
        MakeMove(m);//生成第i个局面（子节点）
        value = MiniMax(p,d-1);//递归调用MiniMax向下搜索子节点
        UnMakeMove(m);//恢复当前局面
        if(p.color == RED)
            bestvalue = max(value,bestvalue);//红方为己方，取极大值
        else
            bestvalue = min(value,bestvalue);
            }
    return bestvalue;//返回最大最小值
}
```
# Negamax Algorithm 负极大值算法
前面的极大极小值算法有点笨，每次都要判断是哪一方，然后做出是取极大值还是极小值的判断。
Knuth和Moore在1975年提出负极大值方法。
```C:n
int NegaMax(position p, int depth)
{
    int n,value,bestvalue=-INFINITY;//最大值初始为负无穷
    if(Game Over(p))return evaluation(p);//胜负已分，返回估值
    if(depth==0)//叶节点调用估值函数
        return evaluation(p);
    for(each possibly move m){//对每种可能走法
        MakeMove(m);//生成新节点
        value = -NegaMax(p,d-1);//递归搜索子节点
        unMakeMove(m);//撤销新节点
        if(value >= bestvalue)
            bestvalue = value;//取极大值
    }
    return bestvalue;
}    
```
两种算法的**关键不同**在于：
value = -NegaMax(p,d-1);
负极大值算法的核心在于，父节点的值是各子节点的值的负数的极大值。估值函数必须对谁走棋敏感，即对于一个红方走棋的局面返回正的估值的话，则对于黑方返回负的估值。
In a word，负极大值算法更优。