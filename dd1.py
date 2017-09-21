# coding = utf-8
import sys
if __name__ == "__main__":
    try:
        arr = map(int, sys.stdin.readline().strip().split(' '))
    except Exception:
        print 0
    if arr == None or len(arr)==0:
        print 0

    def findMaxSumConsistent(nums, n):
        this_sum = 0
        max_sum = 0
        for j in range(n):
            this_sum += nums[j]
            if this_sum > max_sum:
                max_sum = this_sum
            elif this_sum < 0:
                this_sum = 0
        return max_sum

    print findMaxSumConsistent(arr,len(arr))



    def max_sub_sum(nums,left,right):
        max_left_sum, max_right_sum = 0, 0
        max_left_border_sum, max_right_border_sum = 0, 0