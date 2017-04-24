# -*- coding:utf-8 -*-
class Solution:
    def NumberOf1(self, n):
        # write code here
        INT_LENGTH = 32
        if n<0:
        	binStr = bin(2**INT_LENGTH+n)[-INT_LENGTH:]	#print bin(2**$N+$X)[-$N:]，其实就是强行把bin返回值拉到8个长度以上
        else:
            binStr = bin(n)[2:]
        return binStr.count('1')
def main():
    sol = Solution()
    result = sol.NumberOf1(10)
    print result
if __name__ == '__main__':
    main()