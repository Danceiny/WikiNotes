---
date: 2016-03-25 17:22
status: public
title: OpenCV的基本数据类型
---

# 头文件
.../OpenCV/cxcore/include/cxtypes.h
# 简单的
## CvPoint
int x,y 简单结构体。
有两个变体类型：CvPoint2D32f和CvPoint3D32f。前者的x,y为浮点型，后者也是浮点型，但是多了一个z。
## CvSize
和CvPoint结构体差不多，也有一个浮点版本的CvSize2D32f。
## CvRect
## CvScalar
包含4个整型成员。有一个单独的成员val，是一个指向4个双精度浮点数数组的指针。

所有这些数据类型具有以其名称来定义的构造函数，例如cvSize()（首字母不大写）。
这是C而不是c++，所以这些构造函数只是内联函数，首先提取参数列表，然后返回被赋予相关值的结构。

要在(5,10)和(20,30)之间画一个白色矩形，可以：
```c++:n
cvRectangle(
    myImg,
    cvPoint(5,10);
    cvPoint(20,30);
    cvScalar(255,255,255)
);
```

![](~/17-39-11.jpg)

# 矩阵和图像类型
CvArr -> CvMat -> IplImage

CvMat，OpenCV的矩阵结构。
1. 在opencv中没有vector结构，任何时候需要向量，都只需要一个列矩阵。
2. opencv矩阵的概念比线代中的更为抽象——矩阵的元素并非只能取简单的数值类型。例如：
`cvMat* cvCreateMat(int rows, int cols, int type);`
这里的type可以是任何**预定义类型**，预定义类型的结构如下：
`CV_<bit_depth>(S|U|F)C<number_of_channels>`
例如：CV_8UC3,CV_32FC1....

CvMat结构：矩阵头：
```c:n
typedef struct CvMat
{
    int type;
    int step;

    /* for internal use only */
    int* refcount;
    int hdr_refcount;

    union
    {
        uchar* ptr;
        short* s;
        int* i;
        float* fl;
        double* db;
    } data;

#ifdef __cplusplus
    union
    {
        int rows;
        int height;
    };

    union
    {
        int cols;
        int width;
    };
#else
    int rows;
    int cols;
#endif

}
CvMat;
```
虽然OpenCV完全由C语言实现，但是CvMat和IplImage的关系就如同C++中的继承关系。

在函数原型中，会经常看到CvArr（更准确地说，`CvArr*`），这时可以将`CvMat*` 或IplImage作为参数传递。

## 矩阵数据的存取
### 简单的方法
从矩阵中得到一个元素的最简单的方法是利用宏CV_MAT_ELEM()。这个宏传入
1. 矩阵
2. 待提取的元素的类型
3. 行数
4. 列数
返回提取出的元素的值。
```
CvMat* mat = cvCreateMat(5,5,CV_32FC1);
float element_3_2 = CV_MAT_ELEM(*mat,float,3,2);
```
更进一步，还有一个与此宏类似的宏，叫CV_MAT_ELEM_PTR()。
```c++:n
CvMat* mat = cvCreateMat(5,5,CV_32FC1);
float element_3_2 = 7.7;
*((float*)CV_MAT_ELEM_PTR(*mat,3,2)) = element_3_2;
```
这两个宏仅仅适用于访问1、2维的数组。
为了访问普通矩阵中的数据，可以利用`cvPtr*D`和`cvGet*D`..等函数族。

```c++:n
// 指针访问矩阵结构
uchar* cvPtr1D(
    const CvArr* arr,
    int idx0,
    int* type = NULL
    );
uchar* cvPtr2D(
    const CvArr* arr,
    int idx0,
    int idx1,
    int* type = NULL
    );
    ...    
uchar* cvPtrND(
    const CvArr* arr,
    int* idx,
    int* type = NULL,
    int create_node = 1,
    unsigned* precalc_hashval = NULL
    );
```

![](~/20-12-58.jpg)

**在多通道的矩阵中，通道是连续的，例如在一个3通道2维的表示红绿蓝矩阵中~**

### 矩阵数组的step元素
是矩阵中行的长度，单位字节。

为了方便可以使用cvmSet()和cvmGet(),用于处理浮点型单通道矩阵。
~cvmSet(mat,2,2,0.5000);` == `cvSetReal2D(mat,2,2,0.5000);`
**出于效率的考虑，举着或图像的内存分配都是4字节的整数倍。**
>计算机视觉是一种运算密集型的任务，因而需要尽量利用最有效的方法做事，而这些函数接口是不可能做到十分高效的。所以应该定义自己的指针计算，并且在矩阵中利用自己的方法。如果打算对数组中的每一个元素执行一些操作，使用自己的指针是尤为重要的。
```c++:n
//累加一个三通道矩阵中的所有元素
float sum(const CvMat* mat){
    float s = 0.0f;
    for(int row=0; row < mat->rows; row++){
        const float* ptr = (const float*)(mat->data.ptr + row * mat->step);
        for(col=0; col < mat->cols; col++){
            s += *ptr++;
        }
    }
    return s;
}
```            

# 点的数组
**包含多维对象的多维数组（或矩阵）和包含一维对象的高维数组之间的不同。**
例如，假设有n个三维的点，想要传递到参数类型CvMat*的一些OpenCV函数中。有以下四中方式（不一定等价）：
1. 用一个二维数组，数组的类型是CV32FC1，有n行，3列（n * 3)。
2. 用一个3 * n的二维数组。
3. 用一个n * 1的数组，数组的类型是CV32FC3。
4. 用一个1 * n的数组，数组的类型是CV32FC3。

![](~/13-44-47.jpg)

![](~/13-46-07.jpg)

# IplImage数据结构

![](~/13-48-17.jpg)
里面有几个变量很重要。
width,height,depth,nchannels,origin,dataOrder.
depth变量取值于ipl.h中定义的一组数据，但与在矩阵中看到的对应变量不同。因为在图像中往往将深度和通道数分开处理，但是在矩阵中往往同时表示。

![](~/16-58-01.jpg)
origin可取为IPL_ORIGIN_TL or IPL_ORIGIN_BL,分别设置坐标原点的位置在左上角或者左下角。
dataOrder的取值可以是IPL_DATA_ORDER_PIXEL或IPL_DATA_ORDER_PLANE（实际上不受支持），前者指明数据是将像素点不同通道的值交错排在一起，后者是把所有像素同通道值排在一起，形成通道平面，再把平面排列起来。
widthStep是相邻行的同列点之间的字节数，和step类似。
imageData包含一个指向第一行图像数据的指针。
最后还有一个实用的重要参数——感兴趣的区域ROI，实际上是结构IplROI的实例。IplROI结构包含xOffset,yOffset,height,width,coi成员变量，其中coi是感兴趣的通道。

# 访问图像数据 
```c++:n
//仅最大化HSV图像的S和V部分
void saturate_sv(IplImage* img){
    for(int y=0; y<img->height; y++)}
        uchar* ptr = (uchar*)(img->imageData + y*img->widthStep);
        for(int x=0; x<img->width; x++){
            ptr[3*x+1] = 255;//三通道图像
            ptr[3*x+2] = 255;
        }
    }
}
```

![](~/21-12-09.jpg)
# ROI
如果想设置、取消ROI，可以使用函数cvSetImageROI(),
`void cvSetImageROI(IplImage* image,CvRect rect);`
`void cvResetImageROI(IplImage* image);`
```c++:n
//用ImageROI来增加某范围的像素
int roi_add(const char* filename, int x, int y, int width, int height, int add)
{
	IplImage* src;
	if (src = cvLoadImage(filename, 1))
	{
		cvSetImageROI(src, cvRect(x, y, width, height));
		cvAddS(src, cvScalar(add), src);
		cvResetImageROI(src);
		cvNamedWindow("ROI_Add", 1);
		cvShowImage("ROI_Add", src);
		cvWaitKey();
	}
	return 0;
}
```
