---
date: 2016-01-22 12:39
status: public
title: Bumblebee2-Specifications
---

# Imaging Sensor 图像传感器
Sony[`R] 1/3'' progressive scan CCD
ICX204(1032x776 max pixels)
4.65um square pixels
# Baseline 基线
12cm
# Lens Focal Length 镜头焦距
2.5mm with 97° HFOV or 3.8mm 66° HFOV or 6mm with 43° HFOV
**HFOV:** Horizontal Field of View 水平视角
指相机的镜头在水平方向上能够达到的最大可拍摄角度。
# A/D Converter
12-bit analog-to-digital converter
12位AD转换器
# Video Data Output
8,16 and 24-bit digital data(see Supported Data Formats below)
8，16和24位三种位宽数据。
# Frame Rates 帧率
18，15，7.5，3.75，1.875 FPS。
# Interfaces
6-pin IEEE-1394a for camera control and video data transmission
6针IEEE-1394a接口，用于摄像头控制，视频数据传输。
## IEEE-1394
俗称火线接口firewire，可为外设供电，支持同步数据传输。分为Backplane和Cable两种传输方式。本相机是后者。在**通信协议层**方面具有事务层、物理层、链路数据层。事务层支持持异步传输，同步传输由链路层支持。

![](~/13-03-24.jpg)
IEEE-1394a 6-pin 与4-pin
![](~/13-41-26.jpg)

![](~/13-41-36.jpg)

![](~/13-42-29.jpg)

![](~/13-43-28.jpg)
## "1394-ready" what does it mean and what should you know?【from Imaging Insight】
### The standard 
The IEEE-1394 standard - established in 1995 -specifies a digital serial interface and interconnect for consumer electronic devices and computer peripherals(外设）.Currently supporting transfer rates growing number of protocols. For different 1394 device "classes" there are different communication protocols, such as IEC 61883
(professional digital audio/video), SBP-2 (storage, scanners, printers, etc.), HAVi (consumer digitalaudio/video) and IICP (electronic instrumentation and industrial control devices). The 1394 protocol we currently support is DCAM, the specification for industrial/scientific digital video cameras. Why do we support it? The 1394standard is currently leading the industry movement from proprietary digital interfaces to standard high-1394 hardware (Matrox 4Sight imaging platform and Meteor-II/1394 frame grabber). This allows customers to work with 1394 DCAM devices using the same API thatthey use with analog cameras and Matrox Imaging frame grabbers.
Currently, we only support 1394 under Windows NT 4.0, not Windows 98 or 2000. It is also important to note that within the DCAM protocol, there are different formats and modes. Of these, we support certain VGA and SVGA, as well as scalable image formats (Format_7) of Sony XCD-X700 (Mode_2) and XCD-SX900 (Mode_3). Also of interest is that because 1394 has standardized communication between cameras and PCs, customers no longer need a .DCF (digitizer configuration format)file required for particular cameras connected to our frame grabbers.
### Multiple cameras
Many customers require the ability to have multiple cameras running at one time in order to get the "full picture."With 1394, it is possible to link many cameras together simultaneously. In our case, although the driver only supports one Meteor-II/1394 per PC, it does support a 1394 camera network of up to 63 cameras. There are three ports on the 4Sight and Meteor-II/1394 which share the maximum 400 Mbps bandwidth budget. Users must keep in mind that their cameras' raw image output
(Horizontal Resolution x Vertical Resolution x
Pixel Depth x1394 sends a large amount of data across channels in packets, not in one lump sum. Each packet transfer has its own bandwidth requirement, albeit a small one. Therefore, although the raw image bandwidth of the camera/s may well be within the 400 Mbps bandwidth budget, users must add the overhead bandwidth caused by the packet transfer.
### Dedicated channels
Another quirk in the 1394 design that users should be aware of is 1394 sends signals along a reserved isochronous（同步） channel. This mode of communication guarantees a certain amount of bus time for dedicated channels every 125µs (8,000 isochronous cycles per second). Only one device may senddata on a particular channel and, although there can be more than one channel, rapid switching cannot take place between them. Once established, these channels are always reserved, whether they're being used or not, and this can be limiting for certain applications. 1394 will address this limitation by providing morebandwidth in the near future.
Stay tuned to Imaging Insight for new IEEE-1394 developments. If you have any technical questions you would like to pose to Tech Talk, please email us at: kdavis@matrox.com or call 1-800-804-6243 ext. 7970.
# Voltage Requirements 电压要求
8-30V via IEEE-1394 interface or GPIO connector
8~30V的供电，通过IEEE-1394接口，或者GPIO口。
# Power Consumption
2.5W at 12V
# Gain 
Automatic/Manual
影像传感器不可更换，为得到不同的感光度，需要在信号的AD转换时变换信号**增益Gain**。如果光线暗就需要提高感光度即加大增益，这会降低信噪比。
# Shutter 快门
Automatic/Manual, 0.01ms to 66.63ms at 15FPS
帧速率15FPS(Frame Per Second)下快门时间在0.01毫秒到66.63毫秒之间。
# Gamma
0.50 to 4.00

显示器Gamma:显示器的输出图像对输入信号失真的度量参数。对显示器来说是不可更改的物理属性。

系统Gamma:针对输入的图像电压信号进行校正。（对windows系统存在于显卡中，可调节可校正）由于显示器gamma是常量，因而这种校正的幅度也是相对固定的。

文件Gamma:从宏观上讲，被照相机拍摄的物体的亮度是连续变化的，如果将亮度连续变化的被摄物体的图像转换成数字文件时，需要面临用离散的数值去近似表示连续的物理量的问题。
从人的视觉原理上说，人眼感觉到的亮度从暗到亮分为a,b,c,d,e,f,g七段的话，那么其对应的光强不是1，2，3，4，5，6，7，而是1，2，4，8，16，32，64。
以黑白图像为例，每个像素以一个8bit二进制编码表示，有256个量级，从0到255，最亮的为255，则比最亮暗一级的像素的编码值为128。

**这种编码方法存在一个问题：在最亮的分区中，表达的亮度细节非常丰富，超过人眼的识别能力（人眼在亮处可以识别1%的亮度变化），但是在较暗的分区中，表达的亮度细节就非常之少。（会出现马赛克……）**

所以需要对感光元件的输出的电压值在AD转换时做一种变换，使得较暗的分区占有的二进制编码量级多一些。文件gamma是表示这种变换的度量参数。

在具体的上下文环境中，gamma有不同含义。
此处的0.50 to 4.00范围值，文件gamma?
# Trigger Modes 触发模式
DCAM v1.31 Trigger Modes 0,1,3,and 14

![](~/13-31-58.jpg)
DCAM是一个描述与IEEE-1394相机之间交换数据的协议，由IIDC制定，故又称IIDC协议。
DCAM定义的是*未经压缩*的视频数据流在IEEE-1394总线*异步信道*中的传输方法，**不支持音频信号的传输**。
DCAM定义了图像数据流的结构以及相机参数化（例如亮度、快门、白平衡等）。

source：[External trigger modes supported by Poin Grey cameras](https://www.ptgrey.com/KB/10250)

## Standard External Trigger(Mode 0)    标准外部触发模式
In Standard External mode, the camera starts integration（集成） of the incoming light from external trigger input falling/rising edge（外部触发器下降/上升沿输入）. 
**The Exposure（曝光） (Shutter) Time describes integration time.**

No parameter is required. The camera can be triggered in this mode by using the GPIO pins as external trigger or by using a software trigger.
**可用GPIO口触发或者软件触发。**

It is not possible to trigger the camera at full frame rate using Trigger Mode 0; however, this is possible using Overlapped（重叠的） Exposure Readout Trigger (Mode 14).
如图时间轴，Trigger触发器，检测曝光的上升沿触发；Sensor exposure传感器曝光，外部触发器输入（external trigger input），曝光时间等于快门寄存器的值；最后完成Sensor readout,传感器读出（数据）???

![](~/14-15-10.jpg)


同步外部触发信号：[Synchronizing to an external trigger signal using DCAM 1.31 Trigger Mode_0](https://www.ptgrey.com/tan/10591)

## Bulb Shutter Trigger (Mode 1)    灯泡快门触发模式
In Bulb Shutter mode, the camera starts integration of the incoming light from external trigger input. Integration time is equal to low state time of the external trigger input.

如图时间轴，Trigger触发器，Sensor exposure传感器曝光，采光集成时间integration time等于传感器曝光时间；最后完成Sensor readout,传感器读出（数据）。
***trigger width是指？触发器触发所需时间？是一定的吗？或者说是通过寄存器等人为设定的值？***

![](~/14-19-58.jpg)
## Skip Frames Trigger (Mode 3)     跳帧触发
Skip Frames mode allows the user to put the camera into a mode where the camera **only transmits one out of N specified images**.  
This is an internal trigger mode that requires no external interaction（内部触发模式不需外部作用）. Where N is the parameter set in the Trigger Mode, **the camera will issue a trigger internally at a cycle time that is N times greater than the current frame rate**.
（以N倍当时帧率周期触发，*N为时间单位，则N倍帧率为具体某帧，在该帧触发？*）
As with Standard External Trigger, the Exposure Time (Shutter) value describes integration time. Note that this is different from the IIDC specification that states the cycle time will be N times greater than the fastest frame rate.

![](~/14-20-26.jpg)
## Overlapped Exposure Readout Trigger (Mode 14) 重叠曝光读出触发模式
Overlapped Exposure Readout mode is a vendor-unique（供应/唯一） trigger mode that is very similar to Standard External mode, but allows for triggering at faster frame rates.
**与标准外部触发模式非常类似，但是允许最大帧率触发。**
This mode works well for users who want to drive exposure start with an external event. However, users who need a precise exposure start should use the Standard External Trigger.
**该模式适合以外部事件驱动曝光开始，但是如果需要准确的曝光开始，应该使用标准外部触发模式。**
In the figure below, the trigger may be overlapped with the readout of the image, similar to *continuous shot (free-running) mode*. If the trigger arrives after readout is complete, it starts as quickly as the *imaging area（成像区）* can be cleared. 
If the trigger arrives before the end of shutter integration (that is, before the trigger is armed), it is dropped.
**如果触发器在快门集成结束之前触发，即在下一个触发器冷却之前触发，则该触发丢失。**
If the trigger arrives while the image is still being read out of the sensor, the start of exposure is delayed until the next opportunity to clear the imaging area without injecting noise into the output image.
**如果触发器在图像读出传感器阶段触发，则曝光开始被延迟到下一次‘输出图像无噪声插入的’清空成像区。此时触发被视为噪声？**
The end of exposure cannot occur before the end of the previous image readout. Therefore, exposure start may be delayed to ensure this, which means priority is given to maintaining the proper exposure time instead of to the trigger start.
**在上一张图像读出之前，曝光不能结束。因此，曝光开始为此可能需要延迟。这意味着应该优先考虑维持合适的曝光时间，而非触发开始。**
![](~/14-20-49.jpg)
# Signals To Noise Ratio 信噪比
Greater than 60dB at 0dB gain.
# Dimensions 三维尺寸
157mm x 36mm x 47.4mm
# Mass 质量
342grams
# Camera Specification 摄像头主要功能
IIDC 1394-based Digital Camera Specificaition v1.31
# Emissions Compliance 排放规则
Complies with CE rules and Part 15 Class A of FCC Rules
服从<http://www.metlabs.com/blog/tag/fcc-part-15/>
(1) This device may not cause harmful interference, and 
(2) this device must accept any interference received, including interference that may cause undesirable operation.
# Operating Temperature  操作温度
Commercial grade electronics rated from 0℃ to 45℃。
商业级电子设备。
# Storage Temperature  存储温度
-30℃ to 60℃



***以上条目来源于Bumblebee2-Getting-Started-Manual.pdf，以下条目来自官网搜索该型号产品的信息页的Model Specificaitons条目。***


# Resolution 像素
1032 x 776
# Frame rate
20FPS
# Megapixels 百万像素
0.8MP
80万~=1032*776
# Chroma 色度
Color
# Sensor Name
Sony ICX204
# Sensor Type
CCD
# Readout Method
Global shutter
# Sensor Format
1/3''

<https://en.wikipedia.org/wiki/Image_sensor_format>

In digital photography, the image sensor format is the shape and size of the image sensor.

The image sensor format of a digital camera determines the angle of view of a particular lens when used with a particular camera. In particular, image sensors in digital SLR cameras tend to be smaller than the 24 mm × 36 mm image area of full-frame 35 mm cameras, and therefore lead to a narrower angle of view.
![](~/11-05-38.jpg)

# Pixel Size
4.64μm
# Lens Mount 镜头底座
2 x M12 microlens
# Focal Length
3.8mm,65-deg HFOV
# Aperture 光圈
f/2.0
# ADC
12-bit
ADC模数转换器的主要特性是转换速度（转换频率）和量化精度（编码位数）。转换速度指转换中每秒可达到的采样次数。量化精度指每次采样可达到的离散的电平等级。量化精度决定了在ADC模数转换过程中的数据失真，一般的数码相机中的ADC模数转换的量化精度为8位、16位、24位或36位。编码位数越长，数据的失真越小，还原出来的图像画质越好。
# Exposure Range
0.03ms to 66.63ms
# Trigger Modes
Standard,bulb,skip frames,overlapped
# Flash Memory 
512KB non-volatile memory
# Non-isolated I/O Ports
4bi-directional
非独立I/O口，4个，双向
# Serial Port
1（over dedicated专用 pins)
# Auxiliary Output 辅助输出
3.3V,150mA maximum
# Interface
FireWire 1394a
# Power Requirements
12V
# Power Consumption(Maximum)
2.5W at 12V
# Dimensions
157mm x 36mm x 47.4mm
# Mass
342grams
# Machine Vision Standard
IIDC v1.31
# Compliance
CE,FCC,KCC,RoHS
# Temperature (Operating)
0℃ to 45℃。
# Temperature (Storage)
-30℃ to 60℃
# Humidty
20 to 80%(no condensation冷凝)
# Humidty (Storage)
20 to 95%(no condensation)
# Warranty
2 years
# System Requirement
* Windows or Linux
* 3.1 GHz or equival
* 2 GB RAM or more
* 200 MB hard drive
* FireWire IEEE-1394a
 