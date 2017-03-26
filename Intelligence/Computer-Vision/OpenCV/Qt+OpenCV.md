---
date: 2016-05-19 13:04
status: public
title: Qt+OpenCV
---

# .pro文件示例
```
QT += core
QT -= gui

CONFIG += c++11

TARGET = opencv_qt
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += D:/opencv/build/include/opencv \
                D:/opencv/build/include/opencv2 \
                D:/opencv/build/include



LIBS += -LD:\opencv\build\x86\vc12\lib    \
-lopencv_calib3d249d \
-lopencv_contrib249d  \
-lopencv_core249d \
-lopencv_features2d249d   \
-lopencv_flann249d    \
-lopencv_gpu249d  \
-lopencv_highgui249d  \
-lopencv_imgproc249d \
-lopencv_legacy249d \
-lopencv_ml249d \
-lopencv_objdetect249d \
-lopencv_ts249d \
-lopencv_video249d  \
-lopencv_ocl249d    \
-lopencv_nonfree249d   \
-lopencv_photo249d  \
-lopencv_stitching249d \
-lopencv_superres249d  \
-lopencv_videostab249d  \
```

# 援引
<https://www.librehat.com/qt-5-2-vs2012-opencv-2-4-7-dev-environment-setup-tutorial/>