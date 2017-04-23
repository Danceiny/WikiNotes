# -*- coding:utf-8 -*-
class Solution:
    def minNumberInRotateArray(self, rotateArray):
        # write code here
        length = len(rotateArray)
        print length
        if  length == 0:
            return 0
        if length == 1:
            return rotateArray[0]
        i = 0
        step = 1
        small = rotateArray[0]
        while i<length-1:     		
            print i," "
            for j in xrange(i+1,length):
                if rotateArray[j]<small:
                    small = rotateArray[j]
                    step = j-i
                    print "small update",small,"step",step
                    break
            i = i + step
        return small 
def main():
    sol = Solution()
    input = [3,4,5,1,2]
    input2 = [6501,6828,6963,7036,7422,7674,8146,8468,8704,8717,9170,9359,9719,9895,9896,9913,9962,154,293,334,492,1323,1479,1539,1727,1870,1943,2383,2392,2996,3282,3812,3903,4465,4605,4665,4772,4828,5142,5437,5448,5668,5706,5725,6300,6335]
    result = sol.minNumberInRotateArray(input2)
    print "result",result
if __name__ == '__main__':
    main()