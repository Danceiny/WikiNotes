# -*- coding:utf-8 -*-

class Hanoi:
    def chkStep(self, arr, n):
        # write code here
        self.cur_status = [1]*n#初始状态，所有的圆盘都在左柱上
        self.moves = []#记录所有的cur_status
        self.moves.append(self.cur_status[:])
        self.solve(n,1,2,3)
        i = 0
        for status in self.moves:
            if arr==status:
                return i
            i += 1
        return -1
        
    def move(self,n,s1,s2):
        self.cur_status[n-1] = s2
        print(self.cur_status)
        self.moves.append(self.cur_status[:])

    def solve(self,n,s1,s2,s3):
        if n==1:
            self.move(n,s1,s3)
        else:
            self.solve(n-1,s1,s3,s2)#拆
            self.move(n,s1,s3)#
            self.solve(n-1,s2,s1,s3)#组
def main():
    sol = Hanoi()
    input = [3,3]
    result = sol.chkStep(input,2)
    print(result)

    input2 = [1]
    result2 = sol.chkStep(input2,1)
    print(result2)
if __name__ == '__main__':
    main()