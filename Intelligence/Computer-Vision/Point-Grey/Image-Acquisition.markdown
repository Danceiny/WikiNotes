---
date: 2016-01-22 15:31
status: public
title: Image-Acquisition
---

# Automatic Synchronization
Multiple Bumblebee2's on the same 1394 bus automatically sync.
多相机共总线，自动同步。
# Fast Frame Rates
Faster standard frame rates.
# Multiple Trigger Modes
Bulb-trigger mode, overlapped trigger/transfer.
# Color Conversion
On-camera control conversion to YUV411,YUV422 and RGB formats.
相机端控制格式转换。
## YUV
source: <http://www.cnblogs.com/azraelly/archive/2013/01/01/2841269.html>
YUV格式有两大类：planar和packed。
对于planar的YUV格式，先连续存储所有像素点的Y，紧接着存储所有像素点的U，随后是所有像素点的V。
对于packed的YUV格式，每个像素点的Y,U,V是连续交错存储的。
YUV，分为三个分量，“Y”表示明亮度（Luminance或Luma），也就是灰度值；而“U”和“V” 表示的则是色度（Chrominance或Chroma），作用是描述影像色彩及饱和度，用于指定像素的颜色。

与RGB类似，**YUV也是一种颜色编码方法**，主要用于电视系统以及模拟视频领域，它将亮度信息（Y）与色彩信息（UV）分离，没有UV信息一样可以显示完整的图像，只不过是黑白的，这样的设计很好地解决了彩色电视机与黑白电视的兼容问题。并且，YUV不像RGB那样要求三个独立的视频信号同时传输，所以用YUV方式传送占用极少的频宽。

YUV码流的存储格式其实与其采样的方式密切相关，主流的采样方式有三种，YUV4:4:4，YUV4:2:2，YUV4:2:0，这里强调的是**如何根据其采样格式来从码流中还原每个像素点的YUV值**，因为只有正确地还原了每个像素点的YUV值，才能通过YUV与RGB的转换公式提取出每个像素点的RGB值，然后显示出来。
如图，直观地表示采集的方式，以黑点表示采样该像素点的Y分量，以空心圆圈表示采用该像素点的UV分量。

![](~/15-50-01.jpg)
提取每个像素的YUV分量时需要用到：
1. YUV 4:4:4采样，每一个Y对应一组UV分量。
2. YUV 4:2:2采样，每两个Y共用一组UV分量。 
3. YUV 4:2:0采样，每四个Y共用一组UV分量。 
### YUV码流的存储方式及取样像素点数据的方法
Cb,Cr的含义等同于U、V。
#### YUVY格式（属于YUV422)

![](~/15-56-46.jpg)
YUYV为YUV422采样的存储格式中的一种，相邻的两个Y共用其相邻的两个Cb、Cr，分析，对于像素点Y'00、Y'01 而言，其Cb、Cr的值均为 Cb00、Cr00，其他的像素点的YUV取值依次类推。
#### UYVY格式（属于YUV422)

![](~/15-58-29.jpg)
#### YUV422P（属于YUV422)

![](~/16-07-31.jpg)
YUV422P也属于YUV422的一种，它是一种Planar模式，即平面模式，并不是将YUV数据交错存储，而是先存放所有的Y分量，然后存储所有的U（Cb）分量，最后存储所有的V（Cr）分量，如上图所示。其每一个像素点的YUV值提取方法也是遵循YUV422格式的最基本提取方法，即两个Y共用一个UV。比如，对于像素点Y'00、Y'01 而言，其Cb、Cr的值均为 Cb00、Cr00。
# Image Processing
On-camera control of sharpness,hue,saturation,gamma,LUT
相机端控制锐度，色调，饱和度，gamma（校正），显示查找表。
## LUT:  Look-Up-Table
LUT本质上是一个RAM，将数据事先写入RAM后，每当输入一个信号就等于输入一个地址进行查表，找出地址对应内容然后输出。
例如：LUT(Look-Up Table)可以应用到一张像素灰度值的映射表，它将实际采样到的像素灰度值经过一定的变换如阈值、反转、二值化、对比度调整、线性变换等，变成了另外一个与之对应的灰度值，这样可以起到突出图像的有用信息，增强图像的光对比度的作用。
# Embedded Image Info
Pixels contain frame-specific info (e.g. shutter,1394 cycle time)
含帧特性的像素。
