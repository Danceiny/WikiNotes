# 下文所用到的图像隐藏原理
1. 从源图中提取文字图像信息,并记录该文字图像信息像素点在图像矩阵中的位置;
2. 对载体文件进行预处理,将蓝色像素值全部设为偶数;
3. 将记录的文字信息像素点在载体文件对应位置的蓝色像素值设为奇数.

```python
#!/usr/bin/env python
#-*- coding: utf-8 -*-
import cv2
import numpy as np

fn1 = "withtext.jpg"
fn2 = "withcarrier.jpg"
fn3 = "withsecret.jpg"

redcolor = (0,0,255)

if __name__ == '__main__':
    print u'Processing...'
    img1 = cv2.imread(fn1)
    img2 = cv2.imread(fn2)
    w = img1.shape[1]
    h = img1.shape[0]
    # add the information which to be hidden
    cv2.putText(img1,"hello, world!",(20,300),redcolor,ss=2)
    cv2.putText(img1,"code by danceiny",redcolor,thickness=2)
    cv2.putText(img1,"installing python is ",2,redcolor,thickness=1)

    cv2.namedWindow('img1')
    cv2.imshow('img1',img1)
    cv2.namedWindow('img2-1')
    cv.imshow('img2-1',img2)

    # process carrier img
    for j in xrange(0,h):
        for i in xrange(0,w):
            if (img2[j,i,0]%2) == 1:
                img[j,i,0] = img2[j,i,0] - 1
        print '.'
        mirror_w = w/2

    # read source img, write the info into goal img, set the blue value to odd which carry info.
    for j in xrange(0,h):
        for i in xrange(0,w):
            if(img1[j,i,0],img1[j,i,1],img1[j,i,2]) == redcolor:
                img2[j,i,0] = img2[j,i,0] + 1
        print '.',
    # save modified img and show it
    cv2.namedWindow('img2-2')
    cv2.imshow('img2-2',img2)
    cv2.imwrite(fn3,img2)
    cv2.waitKey()
    cv2.destroyAllWindows()



```


# 解密
```python
import cv2
import numpy as np

fn = "withsecret.png"
if __name__ == '__main__':
    print 'loading...'
    print 'processing...'
    img = cv2.imread(fn)
    w = img.shape[1]
    h = img.shape[0]
    imginfo = np.zeros((h,w,3), np.uint8)
    for j in xrange(0,h):
        for i in xrange(0,w):
            if (img[j,i,0] % 2) == 1:
                imginfo[j,i,1] = 255
        print '.',
    cv2.imshow('info',imginfo)
    cv2.imwrite(fn,imginfo)
    cv2.waitKey()
    cv2.destroyAllWindows()
```
