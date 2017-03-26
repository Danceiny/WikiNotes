---
date: 2016-04-29 08:45
status: public
title: Chapter2-OpenCV入门
---


# 开始准备
>有些重要的头文件可以使工作变得更轻松。
> 在头文件`.../opencv/cxcore/include/cxtypes.h`和`cxmisc.h`中包含许多有用的宏定义。
> 使用这些宏可以仅用一行程序就完成结构体和数组的初始化，对链表进行排序等工作。
> 在编译时，有几个头文件是非常重要的：
1. 机器视觉中用到的`.../cv/include/cv.h`和`.../cxcore/include/cxcore.h`
2. IO操作中要用到的`.../otherlibs/highgui/highgui.h`;
3. 机器学习中要用到的`.../ml/include/ml.h`。

(此书成书于2009年，而本人所用opencv版本2.49与此目录结构、文件名略有不同)

# 显示图像
**用以从磁盘加载并在屏幕上显示一幅图像**：
```c++:n
# include "highgui.h"
int main(int argc, char** argv){
	IplImage* img = cvLoadImage(argv[1]);
	cvNamedWindow("Example", CV_WINDOW_AUTOSIZE);
	cvShowImage("Example", img);
	cvWaitKey(0);
	cvReleaseImage(&img);
	cvDestroyWindow("Example");
}
```

##  `IplImage* img = cvLoadImage( argv[1] );`
该行代码将图像文件加载至内存（此处省略异常处理）。cvLoadImage()函数是一个高层调用接口，通过文件名确定被加载文件的格式，并且自动分配图像数据结构所需的内存。

可读取的格式类型包括：BMP, DIB,JPE,JPEG,PNG,PBM,PGM,PPM,SR,RSR,TIFF...

函数执行完返回一个指针，指向一块为描述该图像文件的数据结构（IplImage）而分配的内存块。

IplImage结构体是最常用到的数据结构。
OpenCV使用IplImage结构体处理诸如单通道(single-channel)、多通道(multi-channel)、整型的(integer-valued)、浮点型的(floating-point-valued)等所有类型的图像文件。

**`cvNamedWindow( "Example1", CV_WINDOW_AUTOSIZE );`**

cvNamedWindow()函数也是一个高层调用接口，由HighGUI库提供。cvNamedWindow()函数用于在屏幕上创建一个窗口，将被显示的图像包含于该窗口中。
函数的第一个参数指定窗口标题title，如果要使用HighGUI库所提供的其他函数与该窗口进行交互时，通过该参数值引用这个窗口。

cvNamedWindow()函数的第二个参数定义了窗口的属性。默认值为0，此时窗口的大小不会因图像大小而改变，图像只能在窗口中根据窗口的大小进行拉伸或缩放；而设置为CV_WINDOW_AUTOSIZE时，窗口根据图像的实际大小自动进行拉伸或缩放。

**`cvShowImage( "Example1", img );`**

该函数的参数img是与某个图像文件相对应的IplImage*类型的指针，可以在一个已创建好的窗口（由cvNamedWindow()函数创建）显示该图像。该函数通过设置第一个参数确定在哪个已存在的窗口中显示图像。该函数调用时，该窗口将被重新绘制。

**`cvWaitKey(0)`**

该函数的功能是暂停程序，等待触发一个按键操作。参数若为正，则暂停多少个毫秒数，然后继续执行，即使没有按键。参数为0或负，一直等待按键触发。

**`cvReleaseImage( &img )`**

加载到内存的图像文件用完后即可释放所分配的内存。通过传递一个类型为IplImage*的指针参数调用该函数，用以释放内存。执行完毕后，img指针将被设置为NULL。

**`cvDestroyWindow( "Example1" )`**

关闭显示图像文件的窗口，并释放内存（包括窗口内部的图像内存缓存区，该缓冲区保存了与img指针相关的图像文件像素信息的一个副本）。对一些简单程序，不必调用cvDestroyWindow()或cvReleaseImage()函数显式释放资源。；

下面代码是用来检验环境是否搭建成功的，非本书给出：
```c++:n
# include<iostream>
# include <opencv2\core\core.hpp>
# include <opencv2\highgui\highgui.hpp>

using namespace cv;

int main()
{
	//读入一张图片
	Mat img = imread("pic.jpg");
	//创建一个窗口
	cvNamedWindow("弹簧振子");
	//在窗口中显示游戏原画
	imshow("弹簧振子", img);
	//等待5000ms后窗口自动关闭
	waitKey(5000);
}
```

# 播放AVI视频
播放视频时所需要处理的新问题是如何循环地顺序读取视频中的每一帧，以及从读取中退出该循环操作。
```c++:n
# include "highgui.h"

int main(int argc, char** argv){
	cvNamedWindow("Example2", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateFileCapture(argv[1]);
	IplImage* frame;
	while (1){
		frame = cvQueryFrame(capture);
		if (!frame)	break;
		cvShowImage("Example2", frame);
		char c = cvWaitKey(33);
		if (c == 27)	break;
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow("Example");
}
```
**疑问：对这个argv参数，VS一直试图读取位置0x00000000，失败，无法正常通过编译。**

`CvCapture* capture = cvCreateFileCapture( argv[1] );`

函数cvCreateFileCapture()通过参数设置确定要读取的AVI文件，返回一个指向CvCapture结构的指针。该结构包括了所有关于所要读取AVI文件的信息，包括状态信息。
调用这个函数后，返回指针所指向的CvCapture结构被初始化到所对应的AVI文件的开头。

`frame = cvQueryFrame( capture );`

进入while(1)循环后，开始读入avi文件。cvQueryFrame()的参数为CvCapture结构的指针，将下一帧视频文件载入内存（实际是填充或更新到CvCapture结构中）。返回一个对应当前帧的指针。
与cvLoadImage()不同的是，cvLoadImage()为图像分配内存空间，而cvQueryFrame()使用已分配好的内存。如此， **就没有必要通过cvReleaseImage()对这个返回的图像指针进行释放。**

`c = cvWaitKey(33);if(c==27)break;`

当前帧显示后，等待33ms。如果触发一个按键，c 被设置为该按键的ASCII码。否则会被设置为-1。触发ESC键（ASCII27），退出循环。

*在这个例程中，未使用任何准确的方法来控制视频帧率。*
只是简单地通过cvWaitKey来以固定时间间隔载入帧图像。

**通过从CvCapture结构体（由cvCaptureFromCamera()返回）中读取实际帧率是一个更好的选择。**

`cvReleaseCapture( &capture );`

退出循环体，释放为CvCapture结构体分配的内存空间，关闭所有打开的avi文件相关的句柄。

# 视频播放控制
任务：通过加入一个滚动条，实现在视频播放时进行快速拖动。

调用cvCreateTrackbar()，创建一个滚动条，并且通过设置参数确定滚动条所属于的窗口。
```c++:n
# include "cv.h"
# include "highgui.h"

int g_slider_position = 0;
CvCapture* g_capture = NULL;

void onTrackbarSlide(int pos){
	cvSetCaptureProperty(
		g_capture,
		CV_CAP_PROP_POS_FRAMES,
		pos
		);
}

int main(int argc, char** argv){
	if (NULL == argv[1]){
		printf("there is no file to read!");
		return -1;
	}
	// it always gets no  parameter, and no chance for me to put in a parameter, so i just shut it down with return value -1.
	// Or maybe I can set up a file as default.

	cvNamedWindow("Example3", CV_WINDOW_AUTOSIZE);
	g_capture = cvCreateFileCapture(argv[1]);

	// If it doesn't run at cmd, g_capture will be a null pointer, so let's give a check.
	// but it seemly doesn't work!!!why~
	// I assume that argv[1], which is NULL actually, passed into cvCreateFileCapture() as a parameter, at the same time, the program trys to read the address 0x000000.
	// So, it aborted.
	if (!g_capture){
		printf("loading video failed!\n");
		return -1;
	}

	int frames = (int)cvGetCaptureProperty(
		g_capture,
		CV_CAP_PROP_FRAME_COUNT
		);
	if (frames != 0){
		cvCreateTrackbar(
			"Position",
			"Example3",
			&g_slider_position,
			frames,
			onTrackbarSlide
			);
	}
	IplImage* frame;
	//While loop (as in Example2) capture & show video
	while (1){
		frame = cvQueryFrame(g_capture);
		if (!frame)break;
		cvShowImage("Example3", frame);
		char c = cvWaitKey(33);
		if (c == 27)break;
	}

	//Release memory and destroy window
	cvReleaseCapture(&g_capture);
	cvDestroyAllWindows();
	return 0;

}
```

本质上，该方法通过添加一个全局变量（position，frames）来表示滚动条位置并且添加回调函数更新变量以及重新设置视频读入位置。

`int g_slider_position = 0;	CvCapture* g_capture = NULL;`

由于回调函数需要使用CvCapture对象，因此定义为全局变量。

```C++:n
void onTrackbarSlide(int pos){
	cvSetCaptureProperty(
		g_capture,
		CV_CAP_PROP_POS_FRAMES,
		pos
		);
	}
```

该回调函数在滚动条被拖动时调用。滚动条的位置作为一个32位整数以参数形式传入。

cvSetCaptureProperty()，cvGetCaptureProperty()这两个函数允许设置或查询CvCapture对象的各种属性。
在本程序中，设置参数CV_CAP_PROP_POS_FRAMES(表示以帧数设置读入位置)
如果想通过视频长度比例来设置读入位置，可以用AVI_RATIO代替frames（type:int)来实现。
```c++:n
int frames = (int)cvGetCaptureProperty(
		g_capture,
		CV_CAP_PROP_FRAME_COUNT
		);
```

本程序中，希望获得视频文件的总帧数以对滚动条进行设置。

```c++:n
if (frames != 0){
	cvCreateTrackbar(
		"Position",
		"Example3",
		&g_slider_position,
		frames,
		onTrackbarSlide
		);
}
```

调用函数cvCreateTrackbar()，设置滚动条的名称并确定滚动条的所属窗口。将一个变量绑定到这个滚动条来表示滚动条的最大值和一个回调函数（不需要回掉函数时置为空，当滚动条被拖动时触发）。
当cvGetCaptureProperty()返回的帧数为0时，滚动条不会被创建。这是因为对于有些编码方式，总帧数无法获取。这种情况下看不到滚动条。

本程序没有实现滚动条随着视频播放移动的功能。



# BUG
滚动条程序生成错误：
>还原 NuGet 程序包时出错: 未找到与约束
	ContractName	NuGet.ISettings
	RequiredTypeIdentity	NuGet.ISettings 匹配的导出

并且较大几率偶然性不正常退出。

F10单步执行追踪发现，cvCreateFileCapture()函数返回指针指向0地址，这点应该没问题（有成功播放视频画面的，也有在while(1)循环体内直接break退出的，此时通过调用【获取帧特性的g_capture指针作为参数】的cvQueryFrame()函数所返回的frame是‘假’值。） **猜测该方法对视频编码、格式等要求甚严格？**

# 常用结构、枚举、函数定义
## cvQueryFrame()

`CVAPI(IplImage*) cvQueryFrame( CvCapture* capture );`
## cvCreateTrackbar()
```c++:n
/* create trackbar and display it on top of given window, set callback */
CVAPI(int) cvCreateTrackbar( const char* trackbar_name, const char* window_name,
                             int* value, int count, CvTrackbarCallback on_change CV_DEFAULT(NULL));
```
## IplImage结构体
```c++:n
typedef struct _IplImage
{
    int  nSize;             /* sizeof(IplImage) */
    int  ID;                /* version (=0)*/
    int  nChannels;         /* Most of OpenCV functions support 1,2,3 or 4 channels */
    int  alphaChannel;      /* Ignored by OpenCV */
    int  depth;             /* Pixel depth in bits: IPL_DEPTH_8U, IPL_DEPTH_8S, IPL_DEPTH_16S,
                               IPL_DEPTH_32S, IPL_DEPTH_32F and IPL_DEPTH_64F are supported.  */
    char colorModel[4];     /* Ignored by OpenCV */
    char channelSeq[4];     /* ditto */
    int  dataOrder;         /* 0 - interleaved color channels, 1 - separate color channels.
                               cvCreateImage can only create interleaved images */
    int  origin;            /* 0 - top-left origin,
                               1 - bottom-left origin (Windows bitmaps style).  */
    int  align;             /* Alignment of image rows (4 or 8).
                               OpenCV ignores it and uses widthStep instead.    */
    int  width;             /* Image width in pixels.                           */
    int  height;            /* Image height in pixels.                          */
    struct _IplROI *roi;    /* Image ROI. If NULL, the whole image is selected. */
    struct _IplImage *maskROI;      /* Must be NULL. */
    void  *imageId;                 /* "           " */
    struct _IplTileInfo *tileInfo;  /* "           " */
    int  imageSize;         /* Image data size in bytes
                               (==image->height*image->widthStep
                               in case of interleaved data)*/
    char *imageData;        /* Pointer to aligned image data.         */
    int  widthStep;         /* Size of aligned image row in bytes.    */
    int  BorderMode[4];     /* Ignored by OpenCV.                     */
    int  BorderConst[4];    /* Ditto.                                 */
    char *imageDataOrigin;  /* Pointer to very origin of image data
                               (not necessarily aligned) -
                                needed for correct deallocation */
}
IplImage;
```