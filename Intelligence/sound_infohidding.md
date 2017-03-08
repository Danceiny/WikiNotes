 # 隐藏策略
产生一段正弦波的噪声,在这段噪声中隐藏一段文本文件的内容.

# 实例
1. 读取需隐藏的文本文件,提取文字信息
```python
fo = wave.open(r"pltest.wav","wb")
file_object = open("4-1.py")

try:
    all_the_text = file_object.read()
finally:
    file_object.close()
```
2. 文本转化为编码
wdata = map(ord,all_the_text)
wdata = np.array(wdata)

3. 设置噪声载体文件的波形参数
```python
# 设置波形参数
# 采样率
framerate = 44100
# 声道数
nchannels = 2
# 每位宽度、长度、振幅
sampwidth = 2
nframes = framerate*4
base_amplitude = 200
max_amplitude = 128*base_amplitude
```

4. 计算每个字符的间隔
```python
interval = (nframes-10) / lwdata
```

5. 生成空波形数据
```python
wave_data = np.zeros((nframes),dtype=np.short)
```

6. 生成噪声数据
这一步是算法的核心。如果采样时间处于第四步计算的间隔处，则表示此处为加密字符区(每个字符区只能存放一个字符)。
```python
myrand = np.random.rand(nframes)
for curpos in xrange(0,nframes):
    if curpos % interval == 0 and count<lwdata:
        #将隐藏文字的的字符通过一定的变化写入噪声数据中
        possamp = (wdata[count]-64)*base_amplitude
        count += 1
    elif curpos % 60 == 0:
        #生成随机噪声数据
        possamp = int(myrand[curpos]*max_amplitude - max_amplitude/2)
    else:
        possamp = 0
    wave_data[curpos] = possamp

```

7. 写波形数据
```python
#写波形数据参数
print "Save new wav files..."
str_data = wave_data.tostring()
fo.setnchannels(nchannels)
fo.setframerate(framerate)
fo.setsampwidth(sampwidth)
fo.setnframes(nframes)
fo.writeframes(str_data)
```


# 解码

# 源码
功能：读取名为xx.py的python源程序文件，将该文本隐藏在声音文件中，然后打开载体声音文件，将文本还原为python程序文件。
```python
#!/bin/bash/env python
#-*- coding: utf-8 -*-
import wave
import pylab as pl
import numpy as np
#编码
print u'正在将文件编码进声音'
print "generate wav data..."
# open document
fo = wave.open(r"pltest.wav","wb")
file_object = open('xx.py')
try:
    all_the_text = file_object.read()
finally:
    file_object.close()
wdata = map(ord,all_the_text)
wdata = np.array(wdata)
lwdata = len(wdata)
# 设置波形参数
# 采样率
framerate = 44100
# 声道数
nchannels = 2
# 每位宽度、长度、振幅
sampwidth = 2
nframes = framerate*4
base_amplitude = 200
max_amplitude = 128*base_amplitude

interval = (nframes-10) / lwdata
wave_data = np.zeros((nframes),dtype=np.short)

count = 0

myrand = np.random.rand(nframes)
for curpos in xrange(0,nframes):
    if curpos % interval == 0 and count<lwdata:
        #将隐藏文字的的字符通过一定的变化写入噪声数据中
        possamp = (wdata[count]-64)*base_amplitude
        count += 1
    elif curpos % 60 == 0:
        #生成随机噪声数据
        possamp = int(myrand[curpos]*max_amplitude - max_amplitude/2)
    else:
        possamp = 0
    wave_data[curpos] = possamp

print "Save new wav files..."
str_data = wave_data.tostring()
fo.setnchannels(nchannels)
fo.setframerate(framerate)
fo.setsampwidth(sampwidth)
fo.setnframes(nframes)
fo.writeframes(str_data)
fo.close()

#绘制波形
wave_data.shape = -1,2
wave_data = wave_data.T
time = np.arange(0,nframes/2)
pl.subplot(211)
pl.plot(time,wave_data[0],c="r")
pl.subplot(212)
pl.plot(time,wave_data[1],c="g")
pl.xlabel("time (seconds)")

#解码
new_wdata = []
print u"正在从声音解码文件"
fi = wave.open(r"pltest.wav","rb")
fi_params =  fi.getparams()
fi_nframes = fi_params[3]
fi_str_data = fi.readframes(fi_nframes)
fi_wave_data = np.fromstring(fi_str_data,dtype=np.short)
count = 0
for curpos in xrange(0,nframes):
    if curpos % interval == 0 and count < lwdata:
        possamp = (fi_wave_data[curpos] + 64 * base_amplitude) / base_amplitude
        new_wdata.append(possamp)
        count += 1
my_the_test = "".join(map(chr,new_wdata))
file_object = open("mytext.txt","w")
file_object.write(my_the_test)
file_object.close()
pl.show()                 
```
