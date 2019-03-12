---
date: 2016-04-29 08:44
status: public
title: Install-OpenCV
---


# 配置环境变量
`opencv/build/x64/vc?/bin` or `opencv/build/x86/vc?bin`添加到PATH中。
# 工程包含(include)目录的配置
1.  在VS中新建一个控制台应用程序（win32控制台），附加选项中勾选空项目。
2. 在解决方案资源管理器的【源文件】处右击添加新建项，新建一个cpp文件。
3. 配置属性管理器。【视图】->【属性管理器】，在新出现的属性管理器工作区，点击项目，Debug|Win32，双击即打开属性页面。
4.
在通用属性·VC++目录·包含目录中添加以下目录：
	1. `opencv/build/include`
	2. `opencv/build/include/opencv`
	3. `opencv/build/include/opencv2`
# 工程库(lib)目录的配置
同上打开属性页面，在通用属性·VC++目录·库目录中，添加`opencv/build/x86/vc?/lib`。
**这里需要选x86，因为不管是32bit还是64bit操作系统，最终取决的是使用的是win32编译器还是x64编译器。而一般情况下选用win32的x86编译器。**
# 链接库的配置
同上打开属性页面，通用属性·链接器·输入·附加的依赖项
>针对debug配置添加以下库：
opencv_calib3d249d.lib
opencv_contrib249d.lib
opencv_core249d.lib
opencv_features2d249d.lib
opencv_flann249d.lib
opencv_gpu249d.lib
opencv_highgui249d.lib
opencv_imgproc249d.lib
opencv_legacy249d.lib
opencv_ml249d.lib
opencv_objdetect249d.lib
opencv_ts249d.lib
opencv_video249d.lib
opencv_ocl249d.lib
opencv_nonfree249d.lib
opencv_photo249d.lib
opencv_stitching249d.lib
opencv_superres249d.lib
opencv_videostab249d.lib

>如果是release配置则添加：
opencv_calib3d249.lib
opencv_contrib249.lib
opencv_core249.lib
opencv_features2d249.lib
opencv_flann249.lib
opencv_gpu249.lib
opencv_highgui249.lib
opencv_imgproc249.lib
opencv_legacy249.lib
opencv_ml249.lib
opencv_objdetect249.lib
opencv_ts249.lib
opencv_video249.lib
opencv_ocl249.lib
opencv_nonfree249.lib
opencv_photo249.lib
opencv_stitching249.lib
opencv_superres249.lib
opencv_videostab249.lib
# 在windows文件夹下加入OpenCV动态链接库

![](~/21-21-00.jpg)
缺少的dll放在`opencv/build/x86/vc?/bin`目录下。拷贝到`C:/Windows`即可。

## 我的问题
按照安装文件内的txt帮助文件，添加的lib链接库较pdf帮助文件少了几个lib，结果中断报错，各种找不到pdb。

然后我把dll复制到C://Windows目录下，又添加了“缺少”的lib。再编译执行，可以实现预计效果。但是：：：：：：**强迫症不能忍啊**
>“start-opencv.exe”(Win32):  已加载“H:\Documents\Visual Studio 2013\Projects\start-opencv\Release\start-opencv.exe”。已加载符号。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\ntdll.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\kernel32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\KernelBase.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\opencv_core249.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\msvcp120.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\msvcr120.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\opencv_highgui249.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\user32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\gdi32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\lpk.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\usp10.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\msvcrt.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\advapi32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\sechost.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\rpcrt4.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\sspicli.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\cryptbase.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\ole32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\oleaut32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\winsxs\x86_microsoft.windows.common-controls_6595b64144ccf1df_5.82.7601.18837_none_ec86b8d6858ec0bc\comctl32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\msvfw32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\winmm.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\shell32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\shlwapi.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\avifil32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\msacm32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\avicap32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\version.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\imm32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\msctf.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\uxtheme.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Program Files (x86)\Common Files\microsoft shared\ink\tiptsf.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\apphelp.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\kunlun.ime”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\dbghelp.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\winsxs\x86_microsoft.windows.gdiplus_6595b64144ccf1df_1.1.7601.19061_none_72d6d48d86649709\GdiPlus.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\wininet.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\api-ms-win-downlevel-user32-l1-1-0.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\api-ms-win-downlevel-shlwapi-l1-1-0.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\api-ms-win-downlevel-version-l1-1-0.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\api-ms-win-downlevel-normaliz-l1-1-0.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\normaliz.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\iertutil.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\api-ms-win-downlevel-advapi32-l1-1-0.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\userenv.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\profapi.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\ws2_32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\nsi.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\dwmapi.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\ole32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已卸载“C:\Windows\SysWOW64\ole32.dll”
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\ntmarta.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\Wldap32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\clbcatq.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\msctf.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已卸载“C:\Windows\SysWOW64\msctf.dll”
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\ole32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已卸载“C:\Windows\SysWOW64\ole32.dll”
线程 0x3e00 已退出，返回值为 0 (0x0)。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\WindowsCodecs.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\msctf.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已卸载“C:\Windows\SysWOW64\msctf.dll”
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\oleacc.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已加载“C:\Windows\SysWOW64\shell32.dll”。无法查找或打开 PDB 文件。
“start-opencv.exe”(Win32):  已卸载“C:\Windows\SysWOW64\shell32.dll”
线程 0x12d0 已退出，返回值为 0 (0x0)。
线程 0x5f8 已退出，返回值为 0 (0x0)。
程序“[13276] start-opencv.exe”已退出，返回值为 0 (0x0)。