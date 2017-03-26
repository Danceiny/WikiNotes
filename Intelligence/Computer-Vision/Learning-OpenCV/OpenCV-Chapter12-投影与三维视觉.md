---
date: 2016-04-15 00:41
status: public
title: OpenCV-Chapter12-投影与三维视觉
---


沿用Chapter11中的一些原理：
* 摄像机内参数矩阵M
* 畸变参数
* 旋转矩阵R
* 平移向量T
* 单应性矩阵H

# 投影
标定完成后，即可将现实物理世界中的点无歧义地投影到图像上。
```
void cvProjectPoints2(

);
```
使用场景：欲投影的点位于某些刚体上。

# 仿射变换和透视变换函数
|函数名称|用途|
|---|---|
|cvTransform()|点序列的仿射变换|
|cvWarpAffine()|整个图像的仿射变换|
|cvGetAffineTransform()|得到仿射变换矩阵参数|
|cv2DRotationMatrix()|得到仿射变换矩阵参数|
|cvGetQuadrangleSubPix()|低开销的透视变换|
|cvPerspectiveTransform()|点序列的透视变换|
|cvWarpPerspective()|整个图像的透视变换|
|cvGetPerspectiveTransform()|得到透视变换矩阵参数|


# 鸟瞰图变换实例
具体算法流程：
1. 查找地平面上的已知物体，获得最少4个亚像素精度上的点；
2. 将找到的点输入到函数cvGetPerspectiveTransform()中，计算地平面视图的单应性矩阵H；
3. 使用函数cvWarpPerspective()，设置标志CV_INTER_LINEAR+CV_WARP_INVERSE_MAP+CV_WARP_FILL_OUTLIERS，获得地平面的前向平行视图（即鸟瞰图）。


# POSIT：3D姿态估计
估计已知物体三维位置的算法：POSIT算法。
Pose from Orthography and Scaling with Iteration

为计算这个姿态，必须找到物体表面的至少4个非共面点在相应二维图像上的位置。

```
CvPOSITObject* cvCreatePOSITObject(

);
void cvReleasePOSITObject(

);

void cvPOSIT(

);


```

多余的共面点只会降低效率，非共面点则有好处。

# 立体成像
两台摄像机的立体成像过程：
1. 消除畸变：数学方法消除径向和切向的镜头畸变。
2. 摄像机校正：调整摄像机间的角度和距离，输出行对准的校正图像。
3. 图像匹配：输出视差图。
4. 重投影：将视差图通过三角测量法转为距离，输出等深度图。

## 对极几何
基础矩阵：
```
int cvFindFundamentalMat(

);
```

计算极线：
```
void cvComputeCorrespondEpilines(

);
```

```
bool cvStereoCalibrate(

);
```

## 非标定立体校正：Hartley算法
Hartley算法旨在找到最小化两幅立体图像的计算视差时将极点映射到无穷远处的单应矩阵。

## 标定立体校正：Bouguet算法
void cvStereoRectify();

## 校正映射
cvInitUndistortRectifyMap()

## 立体匹配
cvFindStereoCorrespondenceBM()使用“绝对误差累计”的小窗口（SAD）来查找左右两幅立体校正图像之间的匹配点。

基于块的匹配参数和内部离散缓存保存在一个叫CvStereoBMState的结构体内。

# 从三维重投影获得深度映射
```
void cvPerspectiveTransform(

	);
void cvReprojectImageTo3D(

	);
```
# 二维和三维下的直线拟合
```
void cvFitLine(

	);
```
