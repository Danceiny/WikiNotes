try:
    num = int(raw_input())
except:
    raise Exception
x_dot_list = [0 for i in range(num)]
y_dot_list = [0 for i in range(num)]
for i in range(num):    
    x_dot_list[i], y_dot_list[i] = map(int, raw_input().split(' '))
x_max = max(x_dot_list)
y_max = max(y_dot_list)
x_max_index = x_dot_list.index(x_max)
y_max_index = y_dot_list.index(y_max)
x_min_index = x_dot_list.index(min(x_dot_list))
y_min_index = y_dot_list.index(min(y_dot_list))
result = []
for i in xrange(0, num):
    flag = True
    cache = []
    for j in xrange(0, num):
        if i==j:
            continue
        if x_dot_list[j] > x_dot_list[i] and y_dot_list[j] > y_dot_list[i]:
            flag = False
            break
    if flag:
        result.append({'x': x_dot_list[i],'y':y_dot_list[i]})
            
sorted(result, key=lambda x:x['x'])
for p in result:
    print p['x'],' ',p['y'] 




try:
    num = int(raw_input())
except:
    raise Exception
x_dot_list = [0 for i in range(num)]
y_dot_list = [0 for i in range(num)]
for i in range(num):    
    x_dot_list[i], y_dot_list[i] = map(int, raw_input().split(' '))


result = []
for i in xrange(0, num):
    for
    flag = True
    cache = []
    for j in xrange(0, num):
        if i==j:
            continue
        if x_dot_list[j] > x_dot_list[i] and y_dot_list[j] > y_dot_list[i]:
            flag = False
            break
    if flag:
        result.append({'x': x_dot_list[i],'y':y_dot_list[i]})
            
sorted(result, key=lambda x:x['x'])
for p in result:
    print p['x'],' ',p['y'] 