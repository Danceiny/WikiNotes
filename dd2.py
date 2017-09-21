# coding = utf-8
import sys
if __name__ == "__main__":
    try:
        arr = map(int, sys.stdin.readline().strip().split(' '))
        k = int(sys.stdin.readline().strip())
    except Exception:
        print 0
    if arr == None or len(arr)==0:
        print 0

    index0 = 0
    index2 = k
    arr = sorted(arr,reverse=True)
    print arr[k-1]
    # for i in range(len(arr):
