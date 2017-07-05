# -*- coding:utf-8 -*-
class Solution:
    def jumpFloor(self, number):
        # write code here
        cnt = []
        for i in range(number/2+1):
            j = number-2*i
        
            # C(i+j,i)
        
            cnt.append(self.C(i+j,i))
        return sum(cnt)
    
    def C(self,m,n):
        return self.factorial(m) / self.factorial(m-n) / self.factorial(n)
        #
    
    def factorial(self,x):
        if x==0 or x==1: return 1
        return x*self.factorial(x-1)
def main():
    sol = Solution()
    result = sol.jumpFloor(10)
    print result
if __name__ == '__main__':
    main()