# -*- coding:utf-8 -*-
class Solution:
    stack1 = []
    stack2 = []
    def push(self, node):
        # write code here
        self.stack1.append(node)
    def pop(self):
        if len(self.stack1)==0 and len(self.stack2)==0:
            return
        if len(self.stack2)==0:
            while len(self.stack1)!=0:
                self.stack2.append(self.stack1.pop())
        return self.stack2.pop()
def main():
    sol = Solution()
    sol.push(3)
    print sol.pop()
if __name__ == '__main__':
    main()

