# -*- coding:utf-8 -*-
class Solution:
    def Fibonacci(self, n):
        # write code here
        if n==0 or n==1:
            return n
        
        f_history = []
        #for i in range(n):
        # 递推公式法：时间复杂度过大
        #return self.Fibonacci(n-1)+self.Fibonacci(n-2)   #
        
        # 方法二：O(n)复杂度
        firstItem = 0
        secondItem = 1
        cnt = 1; fib = 0
        while cnt < n:
            fib = firstItem + secondItem
            firstItem = secondItem
            secondItem = fib
            cnt += 1
        return fib

def main():
    sol = Solution()
    n = 1000
    result = sol.Fibonacci(n)
    print result
if __name__ == '__main__':
    main()