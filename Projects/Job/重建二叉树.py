# -*- coding:utf-8 -*-
class TreeNode:
    def __init__(self, x):
        self.val = x
        self.left = None
        self.right = None
class Solution:
    # 返回构造的TreeNode根节点
    def reConstructBinaryTree(self, pre, tin):
        # write code here
        pre = list(pre); tin = list(tin)
        if(pre==None or tin==None or len(pre)!=len(tin) or len(pre)<1):
            return
        preStart = 0;tinStart = 0
        preEnd = len(pre)-1; tinEnd = len(tin)-1
        
        # 取前序遍历的第一个数字，就是当前的根节点
        value = pre[preStart]
        index = tinStart
        
        # 在中序遍历的数组中找根节点的位置
        while index <= tinEnd and tin[index]!=value:
            index += 1
        # 如果在整个中序遍历的数组中没有找到，说明输入参数不合法
        if index>tinEnd:
            return
       	# 创建当前根节点，并且为结点赋值
        node = TreeNode(value)
        
        # 递归构建当前根节点的左子树，左子树的元素个数： index-tinStart+1个
        # 左子树对应的前序遍历的位置在[preStart+1,preStart+index-tinStart]
        # 左子树对应的中序遍历的位置在[tinStart,index-1]
        # python list indice的原因，end位置用于切片时+1
        node.left = self.reConstructBinaryTree(pre[preStart+1:preStart+index-tinStart+1],\
                                          		tin[tinStart:index-1+1])
        
        node.right = self.reConstructBinaryTree(pre[preStart+index-tinStart+1:tinEnd+1],\
                                                tin[index+1:tinEnd+1])
        return node
    
        
    def printTreeInOrder(self,rootNode):
        if rootNode != None:
            self.printTreeInOrder(rootNode.left)
            print str(rootNode.val) + " "
            self.printTreeInOrder(rootNode.right)
        return
    def printTreePreOrder(self,rootNode):
        if rootNode != None:
            print str(rootNode.val) + " "
            self.printTreePreOrder(rootNode.left)
            self.printTreePreOrder(rootNode.right)
        return
    def printTreePostOrder(self,rootNode):
        if rootNode != None:
            self.printTreePreOrder(rootNode.left)
            self.printTreePreOrder(rootNode.right)
            print str(rootNode.val) + " "
        return
def main():
    sol = Solution()
    preOrder = {1,2,3,4,5,6,7}
    inOrder = {3,2,4,1,6,5,7}
    #{1,2,4,7,3,5,6,8},{4,7,2,1,5,3,8,6}
    result = sol.reConstructBinaryTree(preOrder,inOrder)
    sol.printTreeInOrder(result)
    print 
    sol.printTreePreOrder(result)

    print 
    sol.printTreePostOrder(result)
if __name__ == '__main__':
    main()
        