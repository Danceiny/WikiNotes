---
date: '2016-01-23 17:14'
status: public
title: Technical-Application
---

**_ The purpose of this Technical Application Note is to provide a brief introduction to stereo technology, a description of Point Grey stereo products, and some applicaition examples._**

# Applicable Products
- Bumblebee2
- Bumblebee XB3

# Overview
> Stereo vision is an imaging technique that can provide full field of view 3D measurements in an unstructured and dynamic environment. The foundation of stereo vision is similar to 3D perception in human vision and is based on triangulation of rays from multiple viewpoints. Each pixel in a digital camera image collects light that reaches the camera along a 3D ray. If a feature in the world can be identified as a pixel location in an image, we know that this feature lies on the 3D ray associated with that pixel. If we use multiple cameras, we can obtain multiple rays. The intersection of these rays is the 3D location of the feature.

立体视觉是一项在非结构化的动态的环境中提供全视角三维测量的图像技术。 和人体视觉的三维感知类似，立体视觉的建立基于多视点光线的三角测量。 数字图像的每个像素都收集了通过三维路径到达相机的光线。 如果使用多相机，可获得多光线，这些光线的交叉点就是所研究事物的特征的三维坐标。

 实际应用中，立体视觉的关键问题在于：
1. Identify which pixels in multiple images match the same world feature. This is known as the correspondence problem.

**对应一致性问题：在多视图中识别确定匹配实际特征的像素点。**
1. Identify for each pixel in the image the corresponding ray in 3D space. This is known as the calibration problem, and requires accurate calibration of the camera optical parameters and physical location.

**校准问题：识别三维空间中每个像素点所对应的光线。需要精确校准相机光学参数和物理位置。**

## Stage
- Stage 1: Rectification 纠正

  > After the images are transmitted  to the PC over the IEEE-1394 bus, they are corrected and aligned to remove lens distortion.

  > 图像通过IEEE-1394总线传输到PC后，被校正调整以去除镜头扭曲。

得到Rectified image.
- Stage 2:  Laplacian of Gaussian 高斯公式的拉普拉斯算子
  > The Laplacian of Gaussian filter is applied to create edge images that are not biased by image brightness.

> 高斯滤波器的拉普拉斯算子应用于创建无偏亮度的轮廓图像。

得到Edge image.
- Stage 3: Correlation Stereo  立体关联

  > For each pixel in the right image, a corresponding pixel in the left image is obtained via Correlation using the Sum of Absolute Differences criteria.

  > Depth image 的每个像素来说 ，Edge image中每个相关联的像素是通过使用绝对差值求和条件关联获得的。

得到 Depth image.

The correspondence problem is solved through image processing software. Depending on the application, the algorithm used may solve correspondences for only a sparse set of features in the image (feature-based algorithms), or attempt to find correspondences for every pixel in the image (dense stereo algorithms).

相关性问题可通过图像处理软件解决。 使用的算法可能仅解决图像中很少的特征的相关性（基于特征的算法），亦或试图为图像中每个像素搜索相关性（密集立体算法）

Camera calibration is done before using the stereo rig.

 相机校准在使用立体视觉平台前完成。

For dense stereo algorithms, typically the images from the cameras must be remapped to an image that fits a pin-hole camera model. This remapped image is called the rectified image.

 对密集立体算法，通常地，相机的图像必须重映射到一幅符合针孔相机模型的图像。重映射后的图像即为rectified image.

For lenses with barrel distortion, straight lines in the world will appear curved in the image. In the rectified image, however, barrel distortion is removed and straight lines will appear straight.

 对负畸变（桶形失真）的镜头，笔直光线会在图像中扭曲。但是在校准后的图像rrectified image中，桶形失真消失，光线重新变直。

![](~/14-09-09.jpg)

The accuracy of the 3D results of stereo matching depends upon many factors such as image texture, image resolution, lens focal length and the separation between cameras.

 立体图像匹配的三维结果的精度取决于许多因素，例如图像纹理，图像分辨率，镜头焦距，以及相机间距。

 Increase in camera separation or narrower field-of-view lenses improve the accuracy at long range. Higher image resolution increases the accuracy of the results but also may increase the processing time.

  相机间距的增加，或者镜头视野的缩小，都可提高远距离精度。更高的图像分辨率增加了结果精度，但是也增加了图像处理时间。

# Stereo Cameras By Point Grey Research
Point Grey Research has made stereo vision practical for a variety of application areas by providing hardware and software packages that include complete stereo processing support – from image correction and alignment to dense correlation-based stereo mapping.

Point Grey Bumblebee® stereo vision cameras are factory calibrated and come with a complete stereo vision software package called the Triclops™ SDK. The Bumblebee®2 uses two CCD image sensors to provide a balance between 3D data quality, processing speed, size and price. **It can output 640x480 images at 48 FPS, or 1024x768 images at 20 FPS, via its IEEE-1394 (FireWire) interface.**

The Bumblebee® XB3 is a three-CCD multi-baseline stereo camera designed for improved flexibility and accuracy. ** It uses 1.3MP sensors with two baselines--12cm and 24cm--and can output 1280x960 images at 16 FPS via its 800Mb/s IEEE-1394b interface.**

The Triclops Stereo SDK included with the Bumblebee2 and Bumbleebee XB3 supplies **an assemblyoptimized（优化装配的） fast-correlation（快速校准） stereo core that performs fast Sum of Absolute Differences (SAD) stereo correlation. This method is known for its speed, simplicity and robustness, and generates dense disparity images. *** The Triclops SDK provides flexible access to all image stages in the stereo processing pipeline, making it ideal for custom stereo processing approaches.

# Applications
Stereo vision technology is used in a variety of applications, including people tracking, mobile robotics navigation, and mining.  应用于人像追踪，移动机器人导航，和矿业（挖掘）。

It is also used in **industrial automation and 3D machine vision** applications to perform tasks such as **bin picking, volume measurement, automotive part measurement and 3D object location and identification.**

 Point Grey pioneered the use of stereo vision for people tracking with the development of its stereo-based Censys3D® software, which accurately detects and tracks the 3D positions of all people in the camera's fieldof-view.

 This provides not only detection and counting, but also detailed trajectory information. One of the key advantages stereo vision has over traditional single camera tracking systems is its robustness in the presence of lighting changes or shadows.

 This has **made Censys3D the tracking approach of choice for interactive displays ***  such as NetWorld at the Chicago Museum for Science and Technology or the French Pavilion at Expo2005 – where **visitors can chase, or be chased, by computer guided floor projections.**

  This robustness to lighting variation has also **allowed Censys3D tracking to make inroads in the fields of retail and security**.

  People counting and trajectory（轨迹） information can be used in **retail applications to improve store layout and evaluate display effectiveness, and can also record queue length and average wait times for improved customer service.**

  Stereo vision is also becoming the technology of choice for **range sensing in mobile robotics navigation. ***

## 复杂城市环境中无人车避障应用
   The Bumblebee2 was selected as the primary range sensing method for the Princeton vehicle, "Prowler", in the DARPA Urban Challenge, an autonomous vehicle research and development program that has robotic vehicles competing in a race in a complex urban environment.

   While other teams relied on a multitude of often extremely expensive sensors to guide their robotic cars through the course, Prowler used only three stereo video cameras to see and avoid obstacles. "We chose the Bumblebee cameras from Point Grey because they provide a lot of quality data for low cost," explains Gordon Franken, team spokesman for the Princeton team. "The cameras can see out to 60 meters and we were able to write just one obstacle detection algorithm and apply it to all the cameras." Franken adds "We also like the cameras because they are passive sensors and don't rely on beaming out laser or radar waves. In addition, the cameras wouldn't be affected by stray laser or radar beams coming from a competing vehicle." Random bin picking (RBP) is another application that benefits from the use of 3D vision. RBP requires a robot arm to locate and pick out specific items from a mixture of parts haphazardly piled in a container. Automation of this task is fraught with difficulties that are primarily sensing challenges. Parts need to be identified from any viewing angle, with overlap, occlusions, lighting variability and shadows making the problem more difficult. Where stereo vision provides the key advantage, however, is in further distinguishing the 3D environment of a part and determining whether a part is free to be grasped. This portion of the task is commonly solved with expensive laser scanners, to which stereo is a cost effective alternative.

## Additional Downloads and Support
   Access more Technical Application Notes on the web at [http://www.ptgrey.com/support/downloads](http://www.ptgrey.com/support/downloads).    Point Grey Research Inc. endeavors to provide the highest level of technical support possible to our customers. Most support resources can be accessed through the Product Support section of our website: [http://www.ptgrey.com/support](http://www.ptgrey.com/support)

   Creating a Customer Login Account    The first step in accessing our technical support resources is to obtain a Customer Login Account. This requires a valid name, e-mail address, and camera serial number. To apply for a Customer Login Account go to [http://www.ptgrey.com/support/downloads/](http://www.ptgrey.com/support/downloads/). Knowledge Base Our on-line knowledge base at[http://www.ptgrey.com/support/kb/](http://www.ptgrey.com/support/kb/)    contains answers to some of the most common support questions. It is constantly updated, expanded, and refined to ensure that our customers have access to the latest information.    Product Downloads    Customers with a Customer Login Account can access the latest software and firmware for their cameras from our downloads site at [http://www.ptgrey.com/support/downloads](http://www.ptgrey.com/support/downloads). We encourage our customers to keep their software and firmware up-to-date by downloading and installing the latest versions.    Contacting Technical Support    Before contacting Technical Support, have you:
1. Read the product documentation and user manual?
2. Searched the Knowledge Base?
3. Downloaded and installed the latest version of software and/or firmware? If you have done all the above and still can't find an answer to your question, contact our
4. Technical Support team at [http://www.ptgrey.com/support/contact/](http://www.ptgrey.com/support/contact/).
