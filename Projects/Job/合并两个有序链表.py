# -*- coding:utf-8 -*-
class ListNode:
    def __init__(self, x):
        self.val = x
        self.next = None

def printListNode(head):
    while head!=None:
        print head.val
        head = head.next
class Solution_norecur:
    # 返回合并后列表
    def Merge(self, pHead1, pHead2):
        # write code here
        if pHead1 == None:
            return pHead2
        if pHead2 == None:
            return pHead1
        
        mergeHead = None
        cur = None
        while pHead1!=None and pHead2!=None:
            if pHead1.val <= pHead2.val:
                if mergeHead == None:
                    mergeHead = cur = pHead1
                else:
                    cur.next = pHead1
                    cur = cur.next
                pHead1 = pHead1.next
            else:
                if mergeHead == None:
                    mergeHead = cur = pHead2
                else:
                    cur.next = pHead2
                    cur = cur.next
                pHead2 = pHead2.next
        if pHead1 == None:
            cur.next = pHead2
        else:
            cur.next = pHead1
        return mergeHead

class Solution:
    # 返回合并后列表
    def Merge(self, pHead1, pHead2):
        # write code here
        pHead2 = self.ReverseList(pHead1)
        while pHead1 != None:
            while pHead2 != None:
                tmp2 = pHead2
                tmp2next = tmp2.next
                print type(tmp2),type(tmp2next),type(pHead1)
                if tmp2next == None:
                    if tmp2.val<pHead1.val:
                        tmp2.next = pHead1
                    break

                elif tmp2next.val<pHead1.val:
                    tmp2head = self.ReverseList(tmp2next)
                    tmp2.next = None
                    pHead2.next = pHead1
                
                pHead2 = pHead2.next
                print 'pHaed2'
                printListNode(pHead2)
                pHead1 = pHead1.next
            print 'pHead1'
            printListNode(pHead1)
        return pHead1
                    
def ReverseList(pHead):
    if pHead == None or pHead.next == None:
        return pHead
    p = pHead
    r = pHead.next	#指向待搬运的结点（从第二个开始到最后一个）
    m = None	# 搬运工
    tail = pHead.next
    while r!= None:
        m = r
        r = r.next
        m.next = p.next
        p.next = m
        pHead = p.next
    tail.next = p
    p.next = None
    tail = p
    return pHead
def main():
    listNode1 = ListNode(1)
    listNode2 = ListNode(8)
    listNode3 = ListNode(32)
    listNode4 = ListNode(244)
    listNode1.next = listNode2
    listNode2.next = listNode3
    listNode3.next = listNode4


    listNode11 = ListNode(10)
    listNode12 = ListNode(32)
    listNode13 = ListNode(43)
    listNode14 = ListNode(222)
    listNode11.next = listNode12
    listNode12.next = listNode13
    listNode13.next = listNode14

    sol = Solution_norecur()
    result = sol.Merge(listNode1, listNode11)

    while result != None:
        print result.val
        result = result.next

if __name__ == '__main__':
    main()