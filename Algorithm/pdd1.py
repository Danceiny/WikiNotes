#-*- coding:utf-8
def findMostOne(arr, m, n):
    most_list = []
    for row in range(m):
        for col in range(n):
        #记住每一行最小的 Index 即可,相同最大情况
            if arr[row][col] == 1 and col == n-1:
                most_list.append([row+1,m-col])
                #更新最大情况
            elif arr[row][col] == 1 and col != n-1:
                cols = col
                most_list = []
                most_list.append([row+1,n-col])
                break
    print most_list

def findMostOneBySum(arr, m, n):
    most_row_sum = -1
    most_row_index = []
    for row in range(m):
        tmp = sum(arr[row])
        if tmp > most_row_sum:
            most_row_sum = tmp
            print 'update most to',most_row_sum
            most_row_index = []
            most_row_index.append(row)
        elif tmp == most_row_sum:
            print 'dup'
            most_row_index.append(row)

    return [[index+1,most_row_sum] for index in most_row_index]
if __name__ == '__main__':
    arr =[
    [0,0,0,0,0,0,0,1,1,1,1,1],
    [0,0,0,0,1,1,1,1,1,1,1,1],
    [0,0,0,0,0,0,1,1,1,1,1,1],
    [0,0,0,0,0,0,0,0,0,1,1,1],
    [0,0,0,0,0,0,0,1,1,1,1,1],
    [0,0,0,0,1,1,1,1,1,1,1,1]
    ]
    m = 6
    n = 12
    print findMostOneBySum(arr,m,n)

    