---
date: 2016-02-29 18:06
status: public
title: Chapter5-图像处理工具箱的标准空间滤波器
---

# 线性空间滤波器
用来生成滤波掩模w的函数fspecial：

`w = fspecial('type', parameters)`

其中，'type'表示滤波器类型，'parameters'进一步定义了指定的滤波器。
fspecial支持的空间滤波器如下：

|		类型		|		函数和参数		|
|----------------|-------------------------|
|	'average'	|	fspecial('average', [r c])。大小为r x c的一个矩形平均滤波器。默认3x3。若用一个数代替，则为方形滤波器。|
|	'disk'		|	fspecial('disk', r)。一个圆形平均滤波器（包含在2r+1大小的正方形内），半径r默认为5。|
|'gaussian' |	fspecial('gaussian', [r c], sig)。一个大小r x c的高斯低通滤波器，标准偏差为sig（正）。默认值为3 x 3和0.5。若由一个数代替，则为方形滤波器。|
|'laplacian'|	fspecial('laplacian',35..alpha)。一个3x3的拉普拉斯滤波器，其形状由alpha指定，alpha是[0,1]内的一个数，默认0.5。|
|'log'|fspecial('log', [r c], sig)。一个大小r x c的高斯-拉普拉斯滤波器，标准偏差sig（正），默认5x5和0.5。若用一个数代替[r c]，方形|
|'motion'|	fspecial('motion', len, theta)。围绕一幅len个像素的图像线性运动时，输出一个滤波器。运动方向theta，单位为°，即为从水平方向逆时针转动的角度。默认9,0，表示沿水平方向9个像素的运动。|                                                                                                                                                                                                          

***例：手工指定滤波器和增强技术的比较***

本例目的是用手工方法实现一个拉普拉斯滤波器，并与工具箱所提供的拉普拉斯滤波器作比较。
```MATLAB:n
> f = imread('moon.tif');
> w4 = fspecial('laplacian', 0);
> w8 = [1 1 1; 1 -8 1; 1 1 1 ];
> f = im2double(f);
> g4 = f - imfilter(f, w4, 'replicate');
> g8 = f - imfilter(f, w8, 'replicate')
> imshow(f)
> figure, imshow(g4)
> figure, imshow(g8)
```

# 非线性空间滤波器
函数ordiflt2常用来生成非线性空间滤波，可以生成统计排序(order statistics)滤波器（也成为排序滤波器rank filter）。其响应基于对图像邻域中所包含的像素进行排序，然后使用排序结果确定的值来替代邻域中的中心像素的值。

`g = ordfilt2(f, order, domain)`

该函数生成输出图像g的方式：

>使用邻域的一组排序元素中的第order个元素来替代f中的每个元素，而该邻域则由domain中的非零元素指定。domain是一个由0和1组成的m x n的矩阵，该矩阵指定了将在计算中使用的邻域中的像素位置。计算中不使用对应于矩阵domain中的0的邻域中的像素。

例如，要实现m x n的最小滤波器，

`g = ordfilt2(f, 1, ones(m,n))`

其中1表示mn个样本中的第一个样本。元素值全为1，表明邻域内的所有样本都将用于计算。


统计学术语中，最小滤波器（一组排序元素中的第一个样本值）称为第0个百分位。
相应的最大滤波器由

`g = ordfilt2(f, m*n, ones(m,n))`	实现。

数字图像处理中最著名的统计排序滤波器是 **中值滤波器**，对应第50个百分位。
语句

`g = ordfilt2(f, median(1:m*n), ones(m,n))`
创建一个中值滤波器。

其中`median(1:m*n)`计算序列1,2,..,mn的中值。其通用语法

`v = median(A, dim)`。

**二维中值滤波函数** ：

`g = medfilt2(f, [m n], padopt)`。

数组[m n]定义一个邻域，用于计算中值。padopt指定边界填充选项：

| 'zeros'| 默认值|
|------|------|
|'symmetric'|镜像反射对称沿其边界扩展|
|'indexed'|对double图像以1填充，否则0填充|

***例：使用函数medfilt2进行中值滤波***

>中值滤波是降低图像椒盐噪声的有效工具。
原图像f, 用下面函数生成带有椒盐噪声的图像：

`fn = imnoise(f, 'salt & pepper', 0.21)`

进行中值滤波处理：

`gm = medfilt2(fn)`

**medfilt2默认使用一个3x3的邻域计算中值，并以0填充所输入图像的边界。**

存在边界黑点问题，可用'symmetric'选项减弱。