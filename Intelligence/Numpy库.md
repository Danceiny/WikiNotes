---
date: 2016-04-24 18:11
status: public
title: Numpy库
---

# ndarray数组
```python:n
import numpy as np
a = np.array([[1.,7.,0.],[-2.,1.,2.]])


```
ndarray数组对象属性：
* ndim 行数
* shape 维数，返回格式为(n,m)，表示n行m列
* size 数组元素的总数
* dtype 数组元素的类型
* itemsize 每个元素占据的字节大小
* data 数组元素的缓冲区

数组重塑为3x5矩阵。
`a = np.arange(15).reshape(3,5)`

![](~/18-49-29.jpg)
`d = array([[1,2],[3,4]],dtype=complex);`

Numpy的特殊数组主要有：
* zeros
* ones
* empty 空数组，元素全近似为0

![](~/19-17-10.jpg)

序列数组。
使用linspace函数创建等差序列数组。
例：从0到2*pi共100个数字。
![](~/19-22-53.jpg)

数组运算。
`+-*/`是元素级运算。

`numpy.dot(A,B)`才是矩阵内积~

数组的拷贝分为浅拷贝（引用）和深拷贝（复制）。

# 矩阵

![](~/19-39-49.jpg)

转置：
a.T
![](~/19-42-07.jpg)

求逆：
a.I

解线性方程组：
`solve(a,Y);`

# pylab、matplotlib绘图
绘制sin函数图像：
```python:n
import numpy as np
import matplotlib.pyplot as plt
x = np.arange(0,5,0.1);
y = np.sin(x)
plt.plot(x,y)
plt.show()
```

![](~/19-48-32.jpg)
```python:n
#!/user/bin/env python
import cv2
fn = "test1.jpg"
if __name__ == '__main__':
    print 'http://blog.csdn.net/myhaspl'
    print 'myhaspl@qq.com'
    print
    print 'loading %s...' % fn
    img = cv2.imread(fn)
    cv2.imshow('preview',img)
    cv2.waitKey()
    cv2.destroyAllWindows()
```

随机生成像素：
```python:n
import cv2
#随机生成像素点
import numpy as np
if __name__ == '__main__':
    sz1 = 200
    sz2 = 300
    print u'产生空图像矩阵(%d*%d)...' % (sz1,sz2)
    img = np.zeros((sz1,sz2,3),np.uint8)
    pos1 = np.random.randint(200,size=(2000,1))#行位置随机数组
    pos2 = np.random.randint(300,size=(2000,1))#列位置随机数组
    for i in range(2000):
        img[pos1[i],pos2[i],[0]]=np.random.randint(0,255)
        img[pos1[i],pos2[i],[1]]=np.random.randint(0,255)
        img[pos1[i],pos2[i],[2]]=np.random.randint(0,255)
    cv2.imshow('preview',img)
    cv2.waitKey()
    cv2.destroyAllWindows()
```

亮度变换：
```python:n
import numpy as np
fn = 'test1.jpg'
if __name__ == '__main__':
    print 'loading %s ...'%fn
    print u'processing...',
    img = cv2.imread(fn)
    w = img.shape[1]
    h = img.shape[0]
    ii = 0
    #将全部色彩变暗
    for xi in xrange(0,w):
        for xj in xrange(0,h):
            #将像素值整体减少，设为原值的20%
            img[xj,xi,0] = int(img[xj,xi,0]*0.2)
            img[xj,xi,1] = int(img[xj,xi,1]*0.2)
            img[xj,xi,2] = int(img[xj,xi,2]*0.2)
        #显示进度条
        if xi%10==0:print '.',
    cv2.namedWindow('img')
    cv2.imshow('img',img)
    cv2.waitKey()
    cv2.destroyAllWindows()


    print ''
    print 'processing',
    #变亮
    for xi in xrange(0,w):
        for xj in xrange(0,h):
            #将像素值整体减少，设为原值的20%
            img[xj,xi,0] = int(img[xj,xi,0]*10.2)
            img[xj,xi,1] = int(img[xj,xi,1]*10.2)
            img[xj,xi,2] = int(img[xj,xi,2]*10.2)
        #显示进度条
        if xi%10==0:print '.',
    cv2.namedWindow('img')
    cv2.imshow('img',img)
    cv2.waitKey()
    cv2.destroyAllWindows()
```

图像日落效果：蓝色值和绿色值设为原值的70%，红色值不变。
```python:n
import numpy as np
fn = 'test1.jpg'
if __name__ == '__main__':
    print 'loading %s...'%fn
    print 'processing...',
    img = cv2.imread(fn)
    w = img.shape[1]
    h = img.shape[0]
    ii = 0
    #生成日落效果
    for xi in xrange(0,w):
        for xj in xrange(0,h):
            img[xj,xi,0] = int(img[xj,xi,0]*0.7)
            img[xj,xi,1] = int(img[xj,xi,1]*0.7)
            #显示进度条
        if xi%10==0:print '.',
    cv2.namedWindow('preview')
    cv2.imshow('preview',img)
    cv2.waitKey()
    cv2.destroyAllWindows()
```

负片与水印效果。
负片原理是将像素的三色值设为(255-原值)。需要用到cv2.split函数
```python:n
import numpy as np
fn = "test1.jpg"
if __name__ == '__main__':
    print 'loading %s ...' % fn
    print 'processing',
    img = cv2.imread(fn)
    w = img.shape[1]
    h = img.shape[0]

    ii =0

    #生成负片
    b,g,r = cv2.split(img)
    b = 255-b
    g = 255-g
    r = 255-r
    #直接通过索引改变色彩分量
    img[:,:,0]=b
    img[:,:,1]=g
    img[:,:,2]=r
    #加上水印
    cv2.putText(img,"machine learning",thickness=2)
    cv2.putText(img,"Support Vector MachInes (SVMs) is an algorithim of machine learning.",20,100,(0,0,0),thickness=2)
    cv2.namedWindow('img')
    cv2.imshow('img',img)
    cv2.waitKey()
    cv2.destroyAllWindows()
```

图像平铺：
>原理：首先计算平铺后的图像大小，生成同样大小的空白图像，然后逐像素复制图像（逐行）。
```python:n
import numpy as np
fn = 'test1.jpg'
if __name__ == '__main__':
    print 'loading %s'% fn
    img = cv2.imread(fn)
    w = img.shape[1]
    h = img.shape[0]

    #横向平铺5个图像
    sz1 = w*5
    #纵向平铺2个图像
    sz0 = h*2

    #创建空白图像，然后将图片排列
    myimg1 = np.zeros((sz0,sz1,3),np.uint8)

    img_x = 0
    img_y = 0
    for now_y in xrange(0,sz0):
        for now_x in xrange(0,sz1):
            myimg1[now_y,now_x,0]=img[img_y,img_x,0]
            myimg1[now_y,now_x,1]=img[img_y,img_x,1]
            myimg1[now_y,now_x,2]=img[img_y,img_x,2]

            img_x += 1
            if img_x >= w:
                img_x = 0
        img_y += 1
        if img_y >= h:
            img_y = 0
        print '.',
    cv2.namedWindow('img1')
    cv2.imshow('img1',myimg1)
    cv2.waitKey()
    cv2.destroyAllWindows()
```

事实上，原图大小是很重要的影响因子。如果原图过大，平铺数量过多，屏幕无以承载，则不会复制……


旋转并平铺图像。
关键代码：
```
for now_y in xrange(0,sz0):
    for now_x in xrange(0,sz1):
        myimg1[now_x,now_y,:]=img[img_y,img_x,:]
```

完整代码如下：
```python:n
import numpy as np
fn = '22.jpg'
if __name__ == '__main__':
    print 'loading %s'% fn
    img = cv2.imread(fn)
    w = img.shape[1]
    h = img.shape[0]

    #横向平铺3个图像
    sz1 = w*3
    #纵向平铺2个图像
    sz0 = h*2

    #创建空白图像，然后将图片排列(行列互换）
    myimg1 = np.zeros((sz1,sz0,3),np.uint8)

    img_x = 0
    img_y = 0
    for now_y in xrange(0,sz0):
        for now_x in xrange(0,sz1):

            myimg1[now_x,now_y,:]=img[img_y,img_x,:]

            img_x += 1
            if img_x >= w:
                img_x = 0


        img_y += 1
        if img_y >= h:
            img_y = 0
        print '.',
    cv2.namedWindow('img1')
    cv2.imshow('img1',myimg1)
    cv2.waitKey()
    cv2.destroyAllWindows()
```
