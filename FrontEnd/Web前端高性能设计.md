# 减少HTTP请求
1. 使用图片地图(或者CSS sprites)，单张图片带多个超链接，减少HTTP请求
2. HTTP头的Connection: Keep-alive
3. HTTP头的Expire 跟一个时间（字符串？）