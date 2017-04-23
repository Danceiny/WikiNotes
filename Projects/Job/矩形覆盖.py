# -*- coding:utf-8 -*-
class Solution:
    def rectCover(self, number):
        # write code here
        if number < 0: return -1
        if number == 0: return 0
        if number == 1: return 1
        if number == 2: return 2
        return self.rectCover(number-1)+self.rectCover(number-2)#数字一大就不行了，最大递归次数限制

def main():
    sol = Solution()
    result = sol.rectCover(1000)
    print result
if __name__ == '__main__':
    main()