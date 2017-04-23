# 题目

输入一个链表，输出该链表中倒数第k个结点

# 思路
将输入链表赋给两个链表；第一个链表走k步；然后和另外一个链表一起走到链表尾，另外一个链表即在倒数第k个结点。

```python
# -*- coding:utf-8 -*-
# class ListNode:
#     def __init__(self, x):
#         self.val = x
#         self.next = None

class Solution:
    def FindKthToTail(self, head, k):
        # write code here
        if head == None:return None
        if k < 1: return None
        nodeA = head; nodeB = head
        for i in range(k):
            nodeA = nodeA.next
            
            if nodeA==None:
                if i<k-1:
                	return None	#该链表不足k个结点
               	else:
                	return head	#该链表恰好k个结点
                
            
        while nodeA.next != None:
            nodeB = nodeB.next
            nodeA = nodeA.next
        return nodeB.next
```        