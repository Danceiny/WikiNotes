---
date: 2016-03-28 00:27
status: public
title: opencv绘图
---

# 直线
## cvLine()
```
void cvLine(
	CvArr* array,// 一般为一个IplImage*
	CvPoint pt1,
	CvPoint pt2,
	CvScalar color,//typedef struct{double val[4];}CvScalar
	//q前三个代表RGB通道，第四个只在适当的时候用于alpha通道
	//一个常用的便捷宏指令是CV_RGB(r,g,b),接收三个参数然后封装到CvScalar.
	int thickness=1,
	int connectivity=8	//防走样模式，默认值为8连通，这样较为平滑
	//如果设置为4连通，斜线会产生重叠以致于粗重，但是画起来速度较快
	);
```
## cvRectangle()
只比cvLine()少了connectivity参数。其他都一样。


# 圆和椭圆

## 圆
```c:n
void cvCircle(
	CvArr* array,
	CvPoint center,
	int radius,
	CvScalar color,
	int thickness=1,//对圆形等很多封闭图形，此参数可以设置为CV_FILL，其值为-1，表示使用与边界同样的颜色填充封闭图形内部
	int connectivity=8
	);
```
## 椭圆
```
void cvEllipse(
	CvArr* img,
	CvPoint center,
	CvSize axes,//长轴短轴的半轴长
	double angle,//偏离主轴(X)的角度，逆时针正向
	double start_angle,
	double end_angle,
	CvScalar color,
	int thickness=1,
	int line_type=8
	);
```

使用外接矩形是描述椭圆绘制的另一种方法：
```
void cvEllipseBox(
	CvArr* img,
	CvBox2D box,
	CvScalar color,
	int thickness=1,
	int line_type=8,
	int shift=0
	);
```
这里用到一个结构CvBox2D，
```
typedef struct{
	CvPoint2D32f center;
	CvSize2D32f size;
	float angle;
	}CvBox2D;
```

# 多边形
```
void cvFillPoly(//一次可以画多个多边形，但是效率慢
	CvArr* img,
	CvPoint** pts,
	int* npts,
	int contours,
	CvScalar color,
	int line_type=8
	);
void cvFillConvexPoly(//只能画凸多边形(monotone单调多边形)，一次一个
	CvArr* img,
	CvPoint pts,
	int npts,
	CvScalar color,
	int line_type=8
	);
void cvPolyLine(
	CvArr* img,
	CvPoint** pts,
	int* npts,
	int contours,
	int is_closed,
	CvScalar color,
	int thickness=1,
	int line_type=8
	);
```

# 字体和文字
```
void cvPutText(
	CvArr* img,
	const char* text,
	CvPoint origin,
	const CvFont* font,
	CvScalar color
	);
```
获取CvFont*指针的方式就是调用cvInitFont()。
```
void cvInitFont(
	CvFont* font,
	int font_face,	//字体
	double hscale,	//只能设为1.0或者0.5
	double vscale,
	double shear=0,	//1.0时为斜体字，倾斜约45°
	int thickness=1,
	int line_type=8
	);
```