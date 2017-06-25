data = {'a':'a','b':'b'}

def foo(data):
    data['a'] = 'b'

print data
foo(data)
print data