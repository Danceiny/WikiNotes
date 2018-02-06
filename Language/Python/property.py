from math import pi

class Circle(object):
    def __init__(self, radius):
        self.radius = radius

    def getRadius(self):
        return self.radius 

    def setRadius(self, value):
        if not isinstance(value, (int, long, float)):
            raise ValueError('wrong type.' )
        self.radius = float(value)

    def getArea(self):
        return self.radius ** 2 * pi

    R = property(getRadius, setRadius)

c = Circle(3.2)
print c.R
c.R = 'abcc'
print c.R

