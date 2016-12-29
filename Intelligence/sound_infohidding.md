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
