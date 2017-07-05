# -*- coding:utf-8 -*-
class ListNode:
    def __init__(self, x):
        self.val = x
        self.next = None

def printListNode(head):
    while head!=None:
        print head.val
        head = head.next

class Solution3:
    # 返回ListNode
    def ReverseList(self, pHead):
        if pHead == None or pHead.next == None:
            return pHead
        newNode = pHead.next
        while newNode.next != None:
            tmp = newNode.next
            newNode.next = tmp.next
            #print "newNode.next",newNode.next.val
            tmp.next = pHead.next
            #print "tmp.next",tmp.next.val
            pHead.next = tmp
            print 'pHead'
            printListNode(pHead)
            # print 'newNode'
            # printListNode(newNode)

        newNode.next = pHead
        #print '闭环',newNode.val

        pHead = newNode.next.next
        #print '新头变成原头的next',pHead.val

        newNode.next.next = None 
        #print '解环',newNode.next.val
        
        printListNode(pHead)
        return pHead
class Solution2:
    # 返回ListNode
    def ReverseList(self, pHead):
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
class Solution4:
    def ReverseList(self, pHead):
        if pHead == None:
            return None
        if pHead.next == None:
            self.m_pHead = pHead
            return pHead    #????
        new_tail = self.ReverseList(pHead.next);
        new_tail.next = pHead
        pHead.next = None
        return pHead   #最后一次，pHead是尾指针
        
    def ReverseList2(self,pHead,new_head):
        if pHead == None:
            return None
        if pHead.next == None:
            new_head = pHead
            return pHead
        new_tail = self.ReverseList2(pHead.next,new_head)
        new_tail.next = pHead
        pHead.next = None
        return pHead
def main():
    listNode1 = ListNode(10)
    listNode2 = ListNode(8)
    listNode3 = ListNode(5)
    listNode4 = ListNode(2)
    listNode1.next = listNode2
    listNode2.next = listNode3
    listNode3.next = listNode4

    sol3 = Solution3()
    # result3 = sol3.ReverseList(listNode1)
    # #print result.val
    # while result3 != None:
    #     print result3.val
    #     result3 = result3.next
    sol4 = Solution4()
    _ = sol4.ReverseList(listNode1)
    result3 = sol4.m_pHead
    #print result.val
    while result3 != None:
        print result3.val
        result3 = result3.next


    sol4 = Solution4()
    _ = sol4.ReverseList2(listNode1)
    result3 = sol4.m_pHead
    #print result.val
    while result3 != None:
        print result3.val
        result3 = result3.next
        

if __name__ == '__main__':
    main()