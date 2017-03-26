---
date: 2016-04-14 13:43
status: public
title: 2016-04-14-An-Analysis-of-Hand-Gesture-Recognition
---


# An Analysis of Hand Gesture Recognition
Arpit Goyal Manikanta Reddy D Sourav Anand R Sundararajan

## ABSTRACT
In this era of where when we take a small step, technology takes a giant leap. Countless new innovation spawns each day which looks like something out of science fiction fantasy. Continuing on that trend, our current interface of communicating with computers have become quite old. Like the advents in technology, we need new technologies to interact with computers, technologies which can become more user friendly. Hence, we decided to make an interface through which we can easily communicate with the computer, a Gesture.
在这个人类进步一小步，技术进步一大步的时代，无数创新使得日新月异如科幻小说中的传奇。趋势延续，现今与计算机的交互方式已然老去。犹如基督降临，我们需要新的更加用户友好的计算机交互技术。因此，我们决定通过姿势这一简单方式来作为交互接口。

This analysis utilises opensource libraries of OpenCV available for C++, to implement Gaussian Mixture Models and Color based models for Background subtraction and feature extraction.
该分析使用了对C++可用的OpenCV的开源库，以实现高斯混合模型和基于颜色的背景差分模型和特征提取。


## WHAT IS GESTURE?
Loosely speaking, Gesture is a form of non-verbal communication which uses various parts of the body like arms and face to communicate. Gesture as a language is found in nature all around us, as animals, who lacks many distinct sounds to communicate effectively, rely mainly on gesture to do so. It was also our primitive language before we started weaving sounds together.
简单说来，姿势是一种使用身体不同部位，例如手臂和面孔来进行交流的非言语交流形式。姿势作为自然随处可见的语言，是缺乏直接有效的声音交流的动物们的主要沟通方式，也是我们开始一同组织声音语言之前的首选语言。

To interact with the computer in a more natural way, we will go down to our basics - communicate with gesture. Give the computer a signal with hand, and the command will be executed.
为了以一种更为自然的方式与计算机交互，我们回归初心——用我们的姿势。给计算机一个手势信号，命令即执行。

## DEVELOPMENT
### SOFTWARE USED
1.	Microsoft Visual Studio x64 2013 (Our IDE)
2.	OpenCV 2.4.9 (Libraries and function) The program was written in C++ language.


### THE THOUGHT PROCESS
Prior and during development, the basic idea of the project was framed, the essential steps to make our idea a reality. We had certain checkpoints. Our task was
1.	To capture the image as a video feed from the camera feed.
2.	To track the gesture from the camera feed.
3.	To recognize the gesture made.
4.	To check the gesture made with the database.
5.	Execute the function on the match.

在开发之前，构建了该项目的基本思想。这是实现我们的想法的必要步骤。我们有确定的检查点。我们的任务是：
1. 从相机通过视频流拍摄图像。
2. 从相机的视频流追踪姿势。
3. 识别姿势。
4. 与数据库对比检查姿势。
5. 执行匹配函数。

## THEORY
We focused mainly two main methods of gesture recognition, Gaussian model and color models.
我们主要关注两种姿势识别方法，高斯模型和颜色模型。

### Gaussian Mixture Model
Every image is made up of pixels and pixels are store of numbers that decode information about the image. These images can be modeled as a set of features. Let us delve a bit more into this. An image can have various components, say a straight line, a sloped line etc. These components can be approximated as a sum/mixture of Gaussians. This is a statistical model and when we say a mixture of Gaussians we imply that at every pixel the numbers it contains are approximately due to a linear combination of Gaussians. So it must be evident that if something is in background and it remains stationary for a long time then its mixture must be fairly constant over time. This is the whole crux of our method. Now we can apply a temporal filter which can subtract these background mixtures from every frame of the input feed and extract the feature as an alpha frame. The background should also be re-estimated at regular intervals so as to maintain a good approximation of the background pixels.

每张图像都是由像素组成的，像素被存储为编码了图像信息的数字集。这些图像可通过建模视为特征集。深究来说，一张图像可能有许多部分，比如直线、斜线等。这些部分可由高斯混合/和拟合近似。这是一个基于假设的统计模型，我们假设在每个像素点上包含的数字是可以通过高斯分布的线性组合近似的。显而易见，如果背景有物体保持长时间静止，那么其高斯分布混合必然是时间不变的。这是算法的关键。现在我们可以应用一个临时滤波器，来从每一帧输入流减去背景混合，并提取特征为alpha帧。背景也应定期重估计，以保持背景像素的良好近似。**(alpha为更新率）**

#### IMPLEMENTATION
OpenCV provides us with elegant mixture model based functions to estimate matrices of Gaussian’s of the background, which can then be subtracted. For good tracking of features tweaks must be made to number of n-mixtures used in the mixture model and at n=6 we found an optimal balance between computational complexity and tracking precision any higher the computation is complex enough to slow do the program and an smaller gives us a very rough estimation which is prone to misinterpretation. 

OpenCV提供了很好的基于函数的混合模型。该函数估计背景的高斯分布矩阵。为了良好的特征匹配，必须给在混合模型中使用的n个混合设置间隔。在n=6时我们发现交互复杂度和追踪精度的一个最佳平衡。n取更大值时，交互足够复杂以致于程序速度过慢，而n取更小值时估计精度不够。

### Color based tracking
The color based model relies on a much simpler mode of feature extraction which is computationally lighter. Every color image lies in the Red-Green-Blue (R,G,B) color space. So every pixel has a coordinate information and then a color information encoded as (r,g,b). So if we are able to recognize the color of the tracking object fairly closely, we can track it to a very good approximation, by estimating its centroid.

基于颜色的模型依赖于一个简单得多的特征提取模式，计算量较少。每个颜色图像都位于RGB颜色空间，因此每个像素有一个坐标信息，且颜色信息被编码为(r,g,b)。因此，如果我们能够相当近地识别所追踪物体的颜色，就能通过它的质心达到一个非常好的追踪近似度。 

It is much simpler to work in Hue-Saturation-Value (HSV) in order to extract bright colors. HSV is an other color space, into which RGB can be easily converted. A HSV space is better to use because unlike RGB, HSV seperates luma, or the imageintensity, from chroma or the color information.

为提取亮度点，在HSV下更为简单。HSV是另一种颜色空间，RGB可以简单地变换得到。HSV空间更便于使用，因为HSV从图像色度或颜色信息中分离了亮度，或图像强度，这点与RGB不同。

This is very much useful as we want to seperate color components from intensity for various reasons, such as robustness to lighting changes, removing shadows,etc .However HSV is one of many color spaces that seperate chroma from luma, like LAB,YCbCr, etc.. HSV is being used because the code for converting from RGB to HSV is easily available and can be easily implemented.

由于不同的原因，例如光照变化的鲁棒性，移除阴影等，我们想要从图像强度中分离颜色成分，因此HSV是非常有用的。不过，HSV只是许多颜色空间中的一种，它像LAB，YCBCr等一样区分色度和亮度。因为从RGB转换为HSV的代码非常简单可实现，所以HSV常被使用。

Now by thresholding the HSV values by a low and high bars we can extract the desired color.

现在通过设定HSV的阈值，我们可以提取想要的颜色。

#### IMPLEMENTATION
In our project, we used colour markers as trackers. That means our software will track the colour movement and record it. To recognize the colour, we converted our webcam feed’s RGB colour space into easy-to-manipulate HSV colour space by using cvtColor() function. Then by using inRange(), we assigned the range of desired colour between two adjustable H, S and V co-ordinates. This results in the formation of threshold matrix which can be interpretted as the alpha image of all points which have HSV value in the range of our two defined co-ordinates. Now as we have an alpha feed, we can start estimating the median in every frame and track it. This is indeed a very good approximation of the Gesture made by the hand. 

在我们的项目中，我们使用颜色标记作为追踪器，这意味着我们的软件将会追踪并记录颜色的移动。为识别颜色，通过cvtColor()函数我们将摄像头流的RGB颜色空间转换为易操作的HSV颜色空间。然后通过使用inRange()函数给目标颜色在可调的H，S以及V坐标间设定范围。这使得可理解为alpha图像的阈值矩阵格式化了，alpha图像的所有点的HSV值均处于两个定义的坐标范围内。现在我们有了alpha流，则可以开始估计每一帧的中值并追踪。这实际上是手势的一个很好的近似。

## RECOGNISING THE GESTURE
Now that we are done tracking and recording a gesture, we must get along with recognising the gesture. The current program can recognise only a few gestures. The gestures we are using are fundamentally quite easy to recognize, for all gestures in this projects are just permutations of 4 directional reading taken up to 3 times.

既然追踪并记录姿势已经完成，则必须进行识别了。当前的程序只可以识别几种姿势。我们使用的手势基本上是相当容易识别的，因为这些手势的排列仅仅是需时3次的4个方向读取。

That gives us more than enough, i.e. 64 gestures for this project.
这足够了，有64(=4^3)个姿势呢。


To recognize the direction, we based it on the difference in the co-ordinates of initial point and the current point in the threshold matrix. The recognized direction is registered only when the previous direction registered is not same as the current direction awaiting registry. This way, we can also record the change in direction and string out a gesture.

为识别方向，我们基于阈值矩阵中的初始点和当前点的坐标差分。仅当与已有的被注册方向不同时，已识别的方向才会被注册。这样，我们还可以记录方向的改变，将姿势连成列。

To terminate the gesture mid-way, we just have to zero out the threshold matrix, i.e. remove the colour marker out of the webcam range.

中止姿势时，只需要将阈值矩阵置零，从摄像头范围内移除颜色标记。

## FINDING A MATCH
The directions registered are labelled as ‘l’, V, ‘u’ and ‘d’ (quiet naturally they mean left, right, up, down). They are threaded together in a form of an array of characters for example {urd}. They are then compared to pre-defined strings with strcmp() function to find a match. On finding the match, the if condition is initialized and a command is executed.
注册的方向被标记为'l','V','u'和'd'（表示左右上下）。以一组字母的形式（例如{urd})串联。然后将这些方向与预定义的字符串比较，以寻找匹配。找到匹配后，初始化if条件，执行一个命令。

## EXECUTING A COMMAND
We also decide to for a recognized gesture to trigger an event. Upon recognition a gesture can perform a certain operation by utilizing keystrokes api provided by various Operating Systems.
