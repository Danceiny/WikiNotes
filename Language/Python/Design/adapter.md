```python
import pprint
from collections import Counter, OrderedDict
class OrderedCounter(Counter, OrderedDict):
    'Counter that remembers the order elements are first seen'
    def __repr__(self):
        return '%s(%r)' % (self.__class__.__name__,
        	OrderedDict(self))
    def __reduce__(self):
    	return self.__class__, (OrderedDict(self),)

oc = OrderedCounter('abracadabra')
pprint.pprint(oc)
```

