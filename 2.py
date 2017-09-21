try:
    num = int(raw_input())
    seq = map(int, raw_input().split(' '))
except:
    raise Exception
    
sorted(seq,reverse=False)


qujians = list()
max_q = 0
max_qs = 0
for i in xrange(num):
    qujians.append([seq[i]])
    for j in xrange(1,num-i-1):
        qujians.append(seq[i:i+j])

for qujian in qujians:
    print qujian
    min_q = min(qujian)
    sum_q = sum(qujian)
    temp = min_q * sum_q
    if temp > max_qs:
        max_qs = temp
print max_qs


