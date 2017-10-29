try:
    n, m, p = map(int, raw_input().split(' '))
    ideas = []
    for i in xrange(n):
        ideas.append(map(int, raw_input().split(' ')))
except:
    raise Exception

for i in xrange(m):
    