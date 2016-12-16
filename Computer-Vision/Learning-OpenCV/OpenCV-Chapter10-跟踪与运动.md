---
date: 2016-04-11 17:51
status: public
title: OpenCV-Chapter10-跟踪与运动
---

# 跟踪基础
理解物体的运动主要包含两个部分：识别和建模。

# 角点
```
void cvGoodFeaturesToTrack(
    const CvArr* image,
    CvArr* eigImage,
    CvArr* tempImage,
    CvPoint2D32f* corners,
    int* corner_count,
    double quality_level,
    double min_distance,
    const CvArr* mask=NULL,
    int block_size = 3,
    int use_harris = 0,
    double k = 0.4
);
```

## 亚像素点级角点
```
void cvFindCornerSubPix();
```
## 不变特征
SIFT(scale-invariant feature transform),缩放不变，在一点处检测主要梯度方向，根据这个方向记录局部梯度直方图结果，所以也是旋转不变的。
所以SIFT特征在小的仿射变换中表现不错。


## Lucas-Kanade算法
用于求稠密（稀疏）光流。

LK算法基本原理：
1. 亮度恒定。图像场景中目标像素在帧间运动时外观保持不变。对于灰度图像，需要假设像素被逐帧跟踪时其亮度不变。
2. 时间连续或者运动是“小运动”。
3. 空间一致。一个场景中同一表面上邻近的点具有相似的运动，在图像平面上的投影也在邻近区域。

```
void cvCalcOpticalFlowLK(

);
```

**金字塔Lucas-Kanande代码**：
```
void cvCalcOpticalFlowPyrLK(
    const CvArr* imgA,  //初始图像，8位单通道
    const CvArr* imgB,  //最终图像，8位单通道
    CvArr* pyrA,    //金字塔缓存，大小至少为(img.width+8)*img.height/3个字节，为空时自动分配但是效率较低
    CvArr* pyrB,    //金字塔缓存，大小至少为(img.width+8)*img.height/3个字节，为空时自动分配但是效率较低
    CvPoint2D32f* featuresA,    //用于寻找运动的点
    CvPoint2D32f* featuresB,    //存放featuresA中点的新位置
    int count,      //featuresA中点的数目
    CvSize winSize,     //定义计算局部连续运动的窗口尺寸
    int level,      //设置构建的图像金字塔的栈的层数
    char* status,   //该数组的长度为count
    float* track_error,     //删除局部外变化剧烈的点
    CvTermCriteria criteria,    //迭代终止条件
    int flags   //对函数内部bookkeeping进行细微控制
);
```

flags可以设置为（单个或or）：
* CV_LKFLOW_PYR_A_READY
在调用和存储到pyrA之前，先计算第一帧的金字塔
* CV_LKFLOW_PYR_B_READY
在调用和存储到pyrA之前，先计算第二帧的金字塔
* CV_LKFLOW_PYR_INITIAL_GUESSES
在函数调用之前，数组B已包含特征点的初始坐标值


## 稠密跟踪方法
### Horn-Schunck方法
使用亮度恒定假设。

通过迭代法解亮度恒定方程。
```
void cvCalcOpticalFlowHS(
);
```
## mean-shift和camshift跟踪

###　mean-shift
mean-shift算法是一种在一组数据的密度分布中寻找局部极值的稳定方法。（稳定是指统计意义上的稳定，因为其忽略了数据中远离峰值的点）

若分布是连续的，处理过程较为容易；若是离散的，则比较麻烦。

mean-shift算法的步骤如下：
1. 选择搜索窗口。
    * 窗口的初始位置；
    * 窗口的类型（均匀、多项式、指数或者高斯）
    * 窗口的形状（对称、歪斜、旋转、圆形、矩形等）
    * 窗口的大小（超出窗口大小则截去）
2. 计算窗口（可能带权重）的重心。
3. 将窗口的中心设置在重心。
4. 返回第2步，直到窗口的位置不再变化。

`int cvMeanShift();`
### camshift
会自动调整尺寸~
`int cvCamShift();`

## 运动模板
（1） 最简单的获取物体轮廓的办法是用静止摄像机，再使用帧间差得到物体的运动边缘。
（2） 利用颜色信息。例如已知背景颜色，那么可以简单地将不是背景颜色的物体看作前景。
（3） 学习一个背景模型，从此背景中可以将新的前景物体以轮廓的形式分割出来。
（4） 使用主动轮廓技术。例如创建一个近红外光的墙，将能感应近红外线的摄像机对着墙，那么任何介于墙与摄像机之间的物体都会以轮廓的形式出现。
（5） 使用热感图像。任何温度高的物体（如人脸）可视为前景。
（6） 使用分割技术（例如金字塔分割或mean-shift分割）来生成轮廓。

运动模板构建：
```
void cvUpdateMotionHistory(

);
```

```
void cvCalcMotionGradient(

);
```

cvCalGlobalOrientation()计算有效梯度方向矢量来获得全局运动方向。
```
double cvCalcGlobalOrientation(

);
```

分割和计算局部运动：
```
CvSeq* cvSeqmentMotion(

);
```


# 预估器
Kalman滤波器，condensation算法。

```
cvCreateKalman(
);
typedef struct CvKalman{
}CvKalman;

cvKalmanPredict(
);
cvKalmanCorrect(
);
```

Kalman滤波器是基于单个假设的建模，故不能同时表达多个假设。此时需要condensation算法来解决多态分布。

```
CvConDensation* cvCreateConDensation(

);
void cvReleaseConDensation(

);

typedef struct CvConDensation(

)CvConDensation;
void cvConDensInitSampleSet(

);
void cvConDensUpdateByTime(

);
```