from functools import wraps

import time
import logging

def warn(timeout):
    timeout = [timeout]
    def decorator(func):
        def wrapper(*args, **kargs):
            start = time.time()
            res = func(*args, **kargs)
            used = time.time() - start
            if used > timeout[0]:
                msg = '"%s": %s > %s' % (func.__name__, used, timeout[0])
                logging.warn(msg)
            return res
        def setTimeout(k):
            #nonlocal timeout
            timeout[0] = k
        wrapper.setTimeout = setTimeout
        return wrapper
    return decorator

from random import randint
@warn(1.5)
def test():
    print('In test')
    while randint(0, 1):
        time.sleep(0.5)

for _ in range(30):
    test()

test.setTimeout(1)
for _ in range(30):
    test()







