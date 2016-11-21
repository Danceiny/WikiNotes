---
date: 2016-01-18 19:51
status: public
title: Chapter11-Transposition-Table
---

在极大极小搜索的过程中，改进搜索算法的目标在于将不必搜索的冗余分支从搜索的过程中尽量剔除，以达到搜索尽量少的分支来降低运算量的目的。

有个问题在于，在极大极小树的不同分支上，存在着完全相同的节点。

如果要利用已经搜索过的节点的结果，就要用一张表把搜索过的节点记录下来。然后在后续的搜索中，察看记录在表中的这些结果。如果已有记录，则直接利用。
这种方法即为置换表。简称TT。
用伪代码描述如下：
```C:n
int alphabeta(depth, &alpha, &beta)
{
    value = LookupTT(depth,Index);//查询置换表
    if(value is valid)//查看此节点是否已在TT中
    return value;//已有则直接返回表中值
    //没查到，进行alpha-beta搜索
    Search with alphabeta...
    //将搜索过的值记录到TT当中
    StoreToTT(depth,value,Index);
    return value;
}
```
这个置换表的实现见[Chapter12-Hash-Table](/post/chess-game/chapter12-hash-table)