# 思路
用两个指针，第一个指针先走k-1步，然后两个指针一起走。当第一个指针走到尾结点的时候，第二个指针指向的就是倒数第k个结点。

# 初级
```c
ListNode* FindKthToTail(ListNode* pListHead, unsigned int k)
{
    if(pListHead == NULL)
    {
        return NULL;
    }
    for(unsigned int i=0; i<k-1; ++i)
    {
        pAhead = pAhead->m_pNext;
    }
    pBehind = pListHead;
    while(pAhead->m_pNext != NULL)
    {
        pAhead = pAhead->m_pNext;
        bBehind = pBehind->m_pNext;
    }
    return pBehind;
}
```

# 问题
1. 当链表中的结点总数小于k的时候，程序还是会崩溃
