class House:
    def __init__(self,house):
        self.house = house
        self.cur_pos = [0,0]
    def move(self,x):
        pass
        return
    def search(self,x,y,record):
        self.square = 1
        record[x,y] = 1 #visited
        # up
        if record[x][y-1] and self.move(0):
            self.square += self.search(x,y-1,record)
            self.move(2)
        # right
        if record[x+1][y] and self.move(3):
            self.square += self.search(x+1,y,record)
            self.move(1)
        # down
        if record[x][y+1]  and self.move(2):
            self.square += self.search(x,y+1,record)
            self.move(0)
        # left
        if record[x-1][y] and self.move(1):
            self.square += self.search(x-1,y,record)
            self.move(3)
        return self.square
if __name__ == '__main__':
    # house = 'x\nxxx x\nxxxxx\n'
    # house_list = house.strip('\n').split('\n')
    house = [[0],[1,1,1,0,1],[1,1,1,1,1]]
    h_ist = House(house)
    print h_ist.search(0,0,self.house)

# #-*- coding:utf-8
# import sys
# if __name__ == '__main__':
# l1 = [1,3,5]
# l2 = [4,8]
# l3 = [2,5]
# #转化后的字典表示
# finalDict = {1:[1],2:[3],3:[1],4:[2],5:[1,3],8:[2]}
# #区间包含结果
# result = {1:[1,3],2:[4],3:[2]}
# begin = min(l1[0],l2[0],l3[0])
# end = max(l1[0],l2[0],l3[0])
# min = end - begin
# while True:
# if len(result[1]) > 1 and result[1][0] < result[2][0] and
# result[0] < result [3][0]:
# begin = result[1][1]
# del result[1][0