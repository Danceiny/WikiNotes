---
date: 2016-04-29 08:48
status: public
title: 'Opencv-A simple transition'
---


# 一个简单的变换
很多基本的视觉任务包括对视频流的滤波。
修改【入门】中的程序，实现随着视频的播放而对其中每一帧进行一些简单的运算。

一个简单的变化就是对图像平滑处理。
通过对图像数据与高斯或者其他函数进行卷积，有效地减少图像信息内容。

```C++:n
# include "cv.h"
# include "highgui.h"

void example2_4( IplImage* image)
{
	// Create some windows to show the input and output images in.

	cvNamedWindow("Example4-in");
	cvNamedWindow("Example4-out");

	// Create a window to show our input image

	cvShowImage("Example4-in",image);

	// Create an image to hold the smoothed output
	IplImage* out = cvCreateImage(
		cvGetSize(image),
		IPL_DEPTH_8U,
		3
		);

	// Do the smoothing

	cvSmooth(image, out, CV_GAUSSIAN, 3,3);

	// Show the smoothed image in the output window

	cvShowImage("Example4-out",out);

	// Be tidy

	cvReleaseImage(&out);

	// Wait for the user to hit a key, then clean up the windows

	cvWaitKey(0);
	cvDestroyWindow("Example4-in");
	cvDestroyWindow("Example4-out");
}

```

【入门】中用cvCreateFileCapture()来为新的帧分配空间。事实上，这个函数只分配一帧图像的空间，每次调用时覆盖前面一次的数据（这样每次调用返回的指针是一样的）。

因此这里用cvCreateImage()函数代替之，以分配自己的图像结构空间用来存储平滑处理后的图像。

第一个参数是一个cvSize结构，由cvGetSize(image)获得。

第二个参数说明了各通道每个像素点的数据类型。

最后一个参数说明了通道的总数。

由程序可知，当前图像是3通道8位。

`cvSmooth(image, out, CV_GAUSSIAN, 3,3);`是一个库函数的调用。通过使用每个像素3*3区域进行高斯平滑处理。

# 一个复杂一点的变换


```c++:n
/**************************************************
 * 背景建模，运动物体检测
 *      
 **************************************************/

/***********************************************************************
 * OpenCV example
 * By Shiqi Yu 2006
 ***********************************************************************/


#include <stdio.h>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

int main( int argc, char** argv )
{
  //声明IplImage指针
  IplImage* pFrame = NULL;
  IplImage* pFrImg = NULL;
  IplImage* pBkImg = NULL;

  CvMat* pFrameMat = NULL;
  CvMat* pFrMat = NULL;
  CvMat* pBkMat = NULL;

  CvCapture* pCapture = NULL;

  int nFrmNum = 0;

  //创建窗口
  cvNamedWindow("video", 1);
  cvNamedWindow("background",1);
  cvNamedWindow("foreground",1);
  //使窗口有序排列
  cvMoveWindow("video", 30, 0);
  cvMoveWindow("background", 360, 0);
  cvMoveWindow("foreground", 690, 0);



  if( argc > 2 )
    {
      fprintf(stderr, "Usage: bkgrd [video_file_name]\n");
      return -1;
    }

  //打开摄像头
  if (argc ==1)
    if( !(pCapture = cvCaptureFromCAM(-1)))
      {
	fprintf(stderr, "Can not open camera.\n");
	return -2;
      }

  //打开视频文件
  if(argc == 2)
    if( !(pCapture = cvCaptureFromFile(argv[1])))
      {
	fprintf(stderr, "Can not open video file %s\n", argv[1]);
	return -2;
      }

  //逐帧读取视频
  while(pFrame = cvQueryFrame( pCapture ))
    {
      nFrmNum++;

      //如果是第一帧，需要申请内存，并初始化
      if(nFrmNum == 1)
	{
	  pBkImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);
	  pFrImg = cvCreateImage(cvSize(pFrame->width, pFrame->height),  IPL_DEPTH_8U,1);

	  pBkMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
	  pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
	  pFrameMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);

	  //转化成单通道图像再处理
	  cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
	  cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);

	  cvConvert(pFrImg, pFrameMat);
	  cvConvert(pFrImg, pFrMat);
	  cvConvert(pFrImg, pBkMat);
	}
      else
	{
	  cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
	  cvConvert(pFrImg, pFrameMat);
	  //高斯滤波先，以平滑图像
	  //cvSmooth(pFrameMat, pFrameMat, CV_GAUSSIAN, 3, 0, 0);

	  //当前帧跟背景图相减
	  cvAbsDiff(pFrameMat, pBkMat, pFrMat);

	  //二值化前景图
	  cvThreshold(pFrMat, pFrImg, 60, 255.0, CV_THRESH_BINARY);

	  //进行形态学滤波，去掉噪音  
	  //cvErode(pFrImg, pFrImg, 0, 1);
	  //cvDilate(pFrImg, pFrImg, 0, 1);

	  //更新背景
	  cvRunningAvg(pFrameMat, pBkMat, 0.003, 0);
	  //将背景转化为图像格式，用以显示
	  cvConvert(pBkMat, pBkImg);

	  //显示图像
	  cvShowImage("video", pFrame);
	  cvShowImage("background", pBkImg);
	  cvShowImage("foreground", pFrImg);

	  //如果有按键事件，则跳出循环
	  //此等待也为cvShowImage函数提供时间完成显示
	  //等待时间可以根据CPU速度调整
	  if( cvWaitKey(2) >= 0 )
	    break;
	}
    }

  //销毁窗口
  cvDestroyWindow("video");
  cvDestroyWindow("background");
  cvDestroyWindow("foreground");

  //释放图像和矩阵
  cvReleaseImage(&pFrImg);
  cvReleaseImage(&pBkImg);

  cvReleaseMat(&pFrameMat);
  cvReleaseMat(&pFrMat);
  cvReleaseMat(&pBkMat);

  cvReleaseCapture(&pCapture);

  return 0;
}
```