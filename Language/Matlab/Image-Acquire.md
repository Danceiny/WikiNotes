# Image Acquire Add-on
Add-on Explorer 不可用。直接下载osgenericvideointerface.mlpkginstall， 在matlab的文件浏览器中双击打开进行下载安装。

```matlab
% MATLAB控制摄像头操作
% 获取本机中已经安装的硬件资源列表
imaqhwinfo
% 建立videoinput对象
obj = videoinput('winvideo');
% 设置属性
set(obj, 'FramesPerTrigger', 1);
set(obj, 'TriggerRepeat', Inf);
% preview(obj);
% 建立界面
hf = figure('Units', 'Normalized', 'Menubar', 'None', ...
    'NumberTitle', 'off', 'Name', 'danceiny:Show&Take photo');
ha = axes('Parent', hf, 'Units', 'Normalized', ...
    'Position', [.05 .2 .85 .7]);
axis off
hb1 = uicontrol('Parent', hf, 'Units', 'Normalized', ...
    'Position', [.25 .05 .2 .1], 'String', 'danceiny:preview', ...
    'Callback', ...
    ['objRes = get(obj, ''VideoResolution'');' ...
     'nBands = get(obj, ''NumberOfBands'');' ...
     'hImage = image(zeros(objRes(2), objRes(1), nBands));' ...
     'preview(obj, hImage);']);
hb2 = uicontrol('Parent', hf, 'Units', 'Normalized', ...
    'Position', [.55 .05 .2 .1], 'String', 'danceiny:take', ...
    'Callback', 'imwrite(getsnapshot(obj), ''im.jpg'')');
```
定时拍照/监控的实现：
（videoinput对象有start,stop函数，和TimerFcn,TimerPeriod属性）
1. 首先设置obj的定时周期和定时回调函数。
2. start obj
3. 定时周期到，执行回调函数
4. stop obj退出系统