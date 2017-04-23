# -*- coding:utf-8 -*-
class Solution:
    def NumberOf1(self, n):
        # write code here
        if n<0:
            n = -n	
            n = ~n
            n = n + 1            

        binStr = bin(n)[2:]	#'0b0101'
        cnt = 0
        for i in binStr:
            if i=='1':
                cnt += 1
        return cnt
def main():
    sol = Solution()
    result = sol.NumberOf1(10)
    print result
if __name__ == '__main__':
    main()