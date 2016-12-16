---
date: 2016-01-28 00:03
status: draft
title: Chpater3-亮度变换与空间滤波
---

# 背景知识
邻域变换。
计算实现要求务必注意数据类和取值范围。
# 亮度变换函数
变换的最简单形式是邻域1x1，即单个像素的变换。
亮度变换函数仅取决于亮度的值，与坐标无关。
## imadjust函数
对灰度图像进行亮度变换的基本IPT工具。
`g = imadjust(f,[low_in high_in],[low_out high_out],gamma)`

![](~/00-22-09.jpg)
### 明暗反转（负片）
`g = imadjust(f,[0 1],[1 0]);`
也可以：
`g = imcomplement(f);`

## 对数和对比度拉伸变换
对数变换：`g = c*log(1+double(f));`
对数变换的一项主要应用是压缩动态范围。
例如傅里叶频谱的范围为[0,10^6]或更高。当傅里叶频谱显示于已线性缩放至8比特的监视器上时，高值部分占优，从而导致频谱中低亮度值的可视细节丢失。而通过计算对数，高值降低，更便于处理。
**执行对数变换时通常期望将导致的压缩值还原为显示的全范围。**
对于8比特而言，最简方法是：
`gs = im2uint8(mat2gray(g));`
使用函数mat2gray将值限定[0,1]，使用im2uint8将值限定[0,255]。

![](~/12-26-55.jpg)
左上图，常由如下语句对整幅图像完成操作：
`g = 1./(1 + (m./(double(f) + eps)).^E)`

**可以使用对数变换减小动态范围。**
```
> g = im2uint8(mat2gray(log(1+double(f))));
> imshow(g);
```

# 亮度变换的一些实用M函数
## 处理可变数量的输入和/或输出
用函数nargin检测输入到M函数的参量数目：`n = nargin`
类似的有nargout函数。

函数nargchk可用于一个M函数体中，以检测传递的参数数目是否正确：
`msg = nargchk(low, high, number)`
该函数在number<low时，返回消息Not enough input parameters;
number>hight时，返回消息Too many...
在区间范围内时，返回一个空矩阵。
在下面的intrans函数中，注意函数的选项是如何在代码的帮助部分中格式化的，输入的变量数是如何处理的，错误检验是如何插入代码的，以及输入图像的类是如何与输出图像的类相匹配的。
**特别注意，varargin是一个单元数组，故其元素应使用大括号括起。**

```MATLAB:n
function g = instrans(f, varargin)
%INTRANS Performs intensity (gray-level) transformations.
% G = INTRANS(F, 'neg') computes the negative of input image F.
%
% G = INTRANS(F, 'log', C, CLASS) computes C*log(1+F) and  multiplies the result by (positive) constant C.
% If the last two parameters are omitted, C defaults to 1.
% Because the log is used frequentlly to display Fourist spectra, parameter CLASS offers the option to specify the class of the output as 'uint8' or 'uint16'.
% If parameter CLASS is omitted, the output is of the same class as the input.
%
% G = INTRANS(F, 'gamma', GAM) Performs a gamma transformation on the input image using parameter GAM (a required input).
%
% G = INTRANS(F, 'stretch', M, E) computes a constrast-stretching transformation using the expression 1./(1+(M./(F+eps)).^E). Parameter M must be in the range [0,1]. The default value for M is mean2(im2double(F)), and the default value for E is 4.
%
% For the 'neg', 'gamma', and 'stretch' transformations,  double input images whose maximum value is greater than 1 are scaled first using MAT2GRAY. Other images are converted to double first using IM2DOUBLE. For the 'log' transformation, double images are transformed without being scaled; other images are converted to double first using IM2DOUBLE.
%
% The output is of the same class as the input, except if a different class is specified for the 'log' option.
% Verify the correct number of inputs.
error(nargchk(2,4,nargin))
% Store the class of the input for use later.
classin = class(f);
% If the input is of class double, and it is out side the range [0,1], and the specified transformation is not 'log', convert the input to the range [0,1].
if strcmp(class(f), 'double') & max(f(:))>1 & ...
	~strcmp(varargin{1}, 'log')
	f = mat2gray(f);
else % Convert to double, regardless of class(f).
	f = im2double(f);
end
% Determine the type of transformation specified.
method = varargin{1};
% Perform the intensity transformation specified.
switch method
case 'neg'
	g = imcomplement(f);
case 'log'
	if length(varargin) == 1
		c = 1;
	elseif length(varargin) == 2
		c = varargin{2};
	elseif length(varargin) == 3
		c = varargin{2};
	else
	 	error('Incorrect number of inputs for the log option.')
	end
	g = c*(log(1+double(f)));
case 'gamma'
	if length(varargin) < 2
		error('Not enough inputs for the gamma option.')
	end
	gam = varargin{2};
	g = imadjust(f, [ ], [ ], gam);
case 'stretch'
 	if length(varargin) == 1
		% Use defaults.
		m = mean2(f);
		E = 4.0;
	elseif length(varargin) == 3
		m = varargin{2};
		E = varargin{3};
	else error('Incorrect number of inputs for the stretch option.')
	end
	 g = 1./(1+(m./(f+eps)).^E);
otherwise
	error('Unknown enhancement method.')
end
% Convert to the class of the input image.
%g = changeclass(classin, g);
```
对比度拉伸方法使用示例：
```
 >> g = instrans(f, 'stretch', mean29imagerydouble(f)), 0.9);
>> figure,imshow(g)
```

## 亮度标度的M函数
处理图像时，存在像素值域由负到正的现象。gscale函数可把图像标度在全尺度，即最大范围[0,255]或[0.65535]。此外该函数将输出映射到一个特定的范围。
`g = gscale(f, method, low, high)`
其中f是将被标度的图像，method的有效值为'full8'(default)和'full16'，前者将输出标度为全范围[0,255]，后者[0,65535]。使用这两个method则可省略low，high。method还可以是'minmax'，此时low,high须给出。

# 直方图处理与函数绘图
## 生成并绘制图像的直方图
一幅数字图像在范围[0,G]内总共有L个灰度级，其直方图定义为离散函数`h(r) = n`。其中r是区间[0,G]内的第k级亮度，n是灰度级为r的图像中的像素数。
uint8类图像的G=255;uint16的G=65535;double类的G=1.0。
注意matlab中的索引不为0，所以r(1)相当于灰度级0。
uint8\uint16的图像有`G=L-1`。

通常用到归一化直方图，即使用所有元素h(r)除以图像中的像素总数n所得到的图形。从数量到频率。

在处理图像直方图的工具箱中，核心函数是imhist。`h = imhist(f, b)`
其中，f 为输入图像，h 为其直方图，b 是用于形成直方图的灰度级的个数。b的默认值为256。
若要处理一幅uint8类图像并令b=2，则亮度标度范围被分为2部分（两个灰度级）：0~127，128~255。

表达式`p = imhist(f, b)/numel(f)`可简单地获得归一化直方图。其中numel(f)给出数组f中的元素个数（即图像f的像素数）。
## 计算并绘制图像直方图
我读取一幅彩色图像，并imhist(f)。
>错误使用 imhist
需要的 第 1 个输入, I or X, 应为 二维。
出错 imhist>parse_inputs (line 278)
validateattributes(a,
{'double','uint8','int8','logical','uint16','int16','single','uint32',
'int32'}, ...
出错 imhist (line 60)
[a, n, isScaled, top, map] = parse_inputs(varargin{:});

需要是【黑白的二维图像】。

直方图常利用条形图来绘制。
`bar(horz, v, width)`
其中v是一个行向量，包含将被绘制的点；hoaz是一个与v维数相同的向量，包含水平标度值的增量；width是一个[0,1]的数。
若省略horz，则水平轴从0至length(v)等分为若干个单位。
若`width=1`，整条较明显；若`width=0`，整条是简单的垂直线。其默认值为0.8。

坐标轴的取值范围和刻度线：`ylim('auto')` 	`xlim('auto')`。
这两个函数也可人工设定：`ylim([ymin ymax])`。

键入`hold on`将保留当前的图形及某些轴属性。

## 直方图均衡化
假设灰度级为归一化至[0,1]内的连续量，对输入灰度级执行如下变换，得到处理后的输出灰度级s：
![](~/16-44-34.jpg)
灰度级均衡化处理的最终结果是一幅扩展了动态范围的图像，具有较高的对比度。

直方图均衡化由`hesteq`实现。`g = histeq(f, nlev)`

## 直方图匹配（规定化）
**直方图均衡化通过扩展输入图像的灰度级到较宽亮度尺度的范围来实现图像增强。**
能够指定想要的处理后的图像的直方图形状在某些应用中是非常有用的。生成具有指定直方图的图像的方法称为直方图匹配或直方图规定化。

`g = histeq(f, hspec)`
其中f为输入图像，hspec为指定的直方图（一个由指定值构成的行向量），g为输出图像，其直方图近似于指定的直方图hspec。向量中包含对应于等分空间bin的整数值。histeq的一个特性是在length(hespec)远小于图像f中的灰度级数时，图像g的直方图通常会较好地匹配hspec。