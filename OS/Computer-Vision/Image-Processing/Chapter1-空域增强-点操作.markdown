---
date: 2016-01-22 23:36
status: public
title: Chapter1-空域增强-点操作
---

空域，指像素位置所在的空间，也称图像空间。
对于仅定义在每个像素点的位置(x,y)上，或者说在操作中仅利用了单个像素的信息，则被看作一种点操作。
点操作还分为几何点操作（位置），和灰度点操作（幅度）。
点操作可视为映射操作，有可能不可逆。
几何点操作主要借助像素的坐标变换来变换像素的位置。
灰度点操作有较多的形式，如将像素值根据算术或逻辑运算结合成新的像素值，将像素值根据某种函数关系进行映射得到新的像素值，根据某种函数关系进行映射得到新的像素值，或根据一组像素值的统计结果进行映射得到一组新的像素值。
# 图像坐标变换
## 基本坐标变换
### 变换的表达式
对单个像素点：
v'=**A**v
v是变换前的齐次化坐标矩阵，[x,y,1]',v'是变换后的，[x',y' 1]'。
**A**是一个3x3的变换矩阵。

推广到对一组m个点的变换。
V'=**A**V
V,V'是一个3xm的矩阵。
### 平移变换
平移量[Tx,Ty]搬移(x,y)。
[x' y' 1]' = [1 0 Tx;0 1 Ty;0 0 1] [x y 1]'
### 放缩变换/尺度变换
放缩系数Sx,Sy的放缩变换矩阵：
**S**=[Sx 0 0;0 Sy 0;0 0 1]
Sx,Sy不为整数时，原图中某些像素点变换后的坐标值可能不为整数*（如果放缩系数是整数也有可能变换成非整数啊，原书此处似乎不妥）*，导致变换后图像中出现“孔”，此时需要进行**取整操作和插值操作**。
### 旋转变换
图像平面上的旋转可看作是绕一根垂直于平面的轴的旋转。如果这根旋转轴处在坐标原点，且设旋转角是按从旋转轴正向看原点而顺时针定义的，则这样将一个像素逆时针绕旋转轴γ角度的旋转变换可用下列旋转变换矩阵实现：
**R**=[cosγ sinγ 0;-sinγ cosγ 0;0 0 1]
如果旋转轴不在坐标原点，相应的旋转变换矩阵比较复杂。一般可考虑将这种情况先转换为旋转轴处在坐标原点的情况再来处理。
具体分为3步：
1. 将旋转轴平移到坐标系原点；
2. 进行绕原点旋转；
3. 将旋转轴平移回到其相对于坐标系原点的原始位置；
其变换矩阵为级联，
**A=T[^-1]RT**
## 坐标变换扩展
### 变换级联
几个变换矩阵接连作用，运算次序一般不可互换。
### 反变换
逆矩阵。
旋转矩阵的转置和其逆矩阵是相同的。
### 拉伸变换
在一个方向上放大，而在其正交方向上缩小。
放缩变换的一种特例。
设拉伸系数L,则水平方向放大，垂直方向缩小时，L>1。
**L** = [L 0 0;0 1/L 0;0 0 1]
### 剪切变换
只有水平坐标或垂直坐标一个发生平移的变换。分为水平剪切和垂直剪切。
水平：
**J**h=[1 Jx 0;0 1 0;0 0 1]
垂直：
**J**v=[1 0 0;Jy 1 0;0 0 1]
### 旋转变换的分解
旋转变换可以分解为一系列的一维变换。

图像旋转中使用尺度操作有两个问题：
1. 尺度操作需要额外的计算，因为图像不仅需要平移还要放缩。
2. 尽管一幅图像的尺寸在旋转中并不改变，先用放缩减小图像沿水平方向的尺寸会导致混叠效应。如果使用平滑操作来避免混叠，则会使分辨率减小。
但是这个问题的影响比较小，***因为任何旋转算法只需用于旋转角度小于45°的情况。对于更大的旋转角度，先用对应的互换以及镜像操作来获得90°整倍数的旋转，剩下的就小于45°了。***

避免尺度操作，将旋转变换分解为三个一维的剪切变换（级联加速）：
**R** = [cosθ sinθ 0;-sinθ cosθ 0;0 0 1] = 

**[1 tan(θ/2) 0;0 1 0;0 0 1] [1 0 0;-sinθ 1 0;0 0 1] [1 tan(θ/2) 0;0 1 0;0 0 1]**

