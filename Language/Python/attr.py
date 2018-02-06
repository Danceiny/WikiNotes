
class Attr(object):
    def __init__(self, name, type_):
        self.name = name
        self.type_ = type_

    def __get__(self, instance, cls):
        return instance.__dict__[self.name]

    def __set__(self, instance, value):
        if not isinstance(value, self.type_):
            raise TypeError('expected an %s' % self.type_)
        instance.__dict__[self.name] = value

    def __delete__(self, instance):
        raise AttributeError("can't delete this attr")

class Person(object):
    name    = Attr('name', str)
    age     = Attr('age', int) 
    height  = Attr('height', float)
    weight  = Attr('weight', float)

s = Person()
s.name = 'Bob'
s.age = 17
s.height = 1.82
s.weight = 52.5
