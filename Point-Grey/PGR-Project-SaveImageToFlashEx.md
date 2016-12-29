---
date: 2016-03-05 00:29
status: public
title: PGR-Project-SaveImageToFlashEx
---

# SaveImageToFlashEx.cpp
```c++:n
//=============================================================================
// Copyright ?2008 Point Grey Research, Inc. All Rights Reserved.
//
// This software is the confidential and proprietary information of Point
// Grey Research, Inc. ("Confidential Information").  You shall not
// disclose such Confidential Information and shall use it only in
// accordance with the terms of the license agreement you entered into
// with PGR.
//
// PGR MAKES NO REPRESENTATIONS OR WARRANTIES ABOUT THE SUITABILITY OF THE
// SOFTWARE, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE, OR NON-INFRINGEMENT. PGR SHALL NOT BE LIABLE FOR ANY DAMAGES
// SUFFERED BY LICENSEE AS A RESULT OF USING, MODIFYING OR DISTRIBUTING
// THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================
//=============================================================================
// $Id: SaveImageToFlashEx.cpp,v 1.21 2009-12-08 18:58:36 soowei Exp $
//=============================================================================

#include "stdafx.h"

#include <string.h>
#include <math.h>

#include "FlyCapture2.h"

enum FlashMode
{
    // 两种模式，可以看出是有逻辑先后的
    CAPTURE,// 捕获
    RETRIEVE   // 接收
};

using namespace FlyCapture2;

void PrintUsage()
{
    printf( "\nUsage:\n\nSaveImageToFlashEx [-c | -r ]\n" );// 打印提示信息-通过命令参数选定模式
    printf( "\n\twhere \n\n\t-c  = Used to capture an image in on-camera FLASH\n" );
    printf( "\t-r = Used to retrieve an image from FLASH and save it to disk.\n\n" );
}

void PrintBuildInfo()
{
    FC2Version fc2Version;
    Utilities::GetLibraryVersion( &fc2Version );
    char version[128];
    sprintf( 
        version, 
        "FlyCapture2 library version: %d.%d.%d.%d\n", 
        fc2Version.major, fc2Version.minor, fc2Version.type, fc2Version.build );

    printf( "%s", version );

    char timeStamp[512];
    sprintf( timeStamp, "Application build date: %s %s\n\n", __DATE__, __TIME__ );

    printf( "%s", timeStamp );
}

void PrintCameraInfo( CameraInfo* pCamInfo )
{
    printf(
        "\n*** CAMERA INFORMATION ***\n"
        "Serial number - %u\n"
        "Camera model - %s\n"
        "Camera vendor - %s\n"
        "Sensor - %s\n"
        "Resolution - %s\n"
        "Firmware version - %s\n"
        "Firmware build time - %s\n\n",
        pCamInfo->serialNumber,
        pCamInfo->modelName,
        pCamInfo->vendorName,
        pCamInfo->sensorInfo,
        pCamInfo->sensorResolution,
        pCamInfo->firmwareVersion,
        pCamInfo->firmwareBuildTime );
}

void PrintError( Error error )
{
    error.PrintErrorTrace();
}

// 通过读寄存器的值，判断是否支持dataflash（数据流传输？类似于flv的视频格式？~）
bool IsDataFlashSupported( Camera* pCam )
{
    const unsigned int k_dataFlashReg = 0x1240;
    //寄存器地址？~
    Error error;
    unsigned int regVal;//寄存器的值？~
    error = pCam->ReadRegister( k_dataFlashReg, &regVal );// 相机类对象pCam有读寄存器值的方法
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return false;
    }

    return (regVal & 0x80000000) == 0 ? false : true;
}

unsigned int GetDataFlashSize( Camera* pCam )
{
    const unsigned int k_dataFlashReg = 0x1240;
    Error error;
    unsigned int regVal;
    error = pCam->ReadRegister( k_dataFlashReg, &regVal );
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return 0;
    }

    int exponent = 0;
    //这里的exponent是指数的意思，pow(2.0,exponent)显然应该是浮点数的size。至于为啥整个可用flash的size=pageSize*numPages，这就有待研究了。
    exponent = static_cast<int>((regVal & 0x00FFF000) >> 12);//抛弃k_dataFlashReg=0x1240的右12位（就是240不要了），左边的按位与（其实主要操作部分就是0x00FFF000的最后一个F)
    int pageSize = static_cast<int>(pow(2.0, exponent));

    exponent = static_cast<int>(regVal & 0x00000FFF);
    int numPages = static_cast<int>(pow(2.0, exponent));
    unsigned int availableFlashSize = pageSize * numPages;

    return availableFlashSize;
}

//这里的offset不知道翻译成啥比较准确啊，所以影响理解~有补偿、抵消的含义
unsigned int GetDataFlashOffset( Camera* pCam )
{
    const unsigned int k_uiDataFlashOffset = 0x1244;
    Error error;
    unsigned int regVal;
    error = pCam->ReadRegister( k_uiDataFlashOffset, &regVal );
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return 0;
    }

    return regVal;
}

int main(int argc, char** argv)
{
    PrintBuildInfo();

    // Make sure the number of arguments is correct
    if ( argc != 2 )
    {//不等于2就是没有外部传参了~~~所以就调用那个打印提醒信息的函数咯
        PrintUsage();
        return -1;
    }

    // Figure out what mode to be operating in
    FlashMode flashMode;
    if ( strcmp( argv[1], "-r" ) == 0 )
    {
        flashMode = RETRIEVE;
    }
    else if ( strcmp( argv[1], "-c" ) == 0 )
    {
        flashMode = CAPTURE;
    }
    else
    {
        PrintUsage();
        return -1;
    }

    Error error;

	if (flashMode == RETRIEVE)
	{
		// Since this application saves images in the current folder
		// we must ensure that we have permission to write to this folder.
		// If we do not have permission, fail right away.
		FILE* tempFile = fopen("test.txt", "w+");
		if (tempFile == NULL)
		{
			printf("Failed to create file in current folder.  Please check permissions.\n");
			return -1;
		}
		fclose(tempFile);
		remove("test.txt");
	}

    BusManager busMgr;
    unsigned int numCameras;
    error = busMgr.GetNumOfCameras(&numCameras);
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    printf( "Number of cameras detected: %u\n", numCameras );

    if ( numCameras < 1 )
    {
        printf( "Insufficient number of cameras... exiting\n" );
        return -1;
    }

    PGRGuid guid;
    error = busMgr.GetCameraFromIndex(0, &guid);
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    Camera cam;

    // Connect to a camera
    error = cam.Connect(&guid);
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    // Get the camera information
    CameraInfo camInfo;
    error = cam.GetCameraInfo(&camInfo);
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    PrintCameraInfo(&camInfo);  

    bool dataFlashSupported = IsDataFlashSupported( &cam );
    if ( dataFlashSupported != true )
    {
        printf( "This camera does not support the data flash feature... press Enter to exit.\n" );
        getchar();
        return -1;
    }

    error = cam.StartCapture();
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    Image image;
    error = cam.RetrieveBuffer( &image );
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    error = cam.StopCapture();
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }
    
    const unsigned int dataFlashSize = GetDataFlashSize( &cam );    
    
    // If the flash is not large enough to hold a full image, 
    // determine the maximum image with a 4:3 aspect ratio which can fit in 
    // the flash and use those dimensions. We will then start the camera
    // in Format7 using these settings. Otherwise, we will just save an
    // image with the current settings.
 
    if ( image.GetDataSize() > dataFlashSize )
    {
        const unsigned int bytesPerPixel = image.GetBitsPerPixel() / 8;
        unsigned int imageWidth = image.GetCols();
        unsigned int imageHeight = image.GetRows();    
   //为何实际抓取图像大小超过缓冲大小，就要做一个4：3的变换呢，这样变换的话好像实际大小并未改变~
        imageHeight = static_cast<unsigned int>(sqrt((dataFlashSize / bytesPerPixel) * 3.0 / 4.0));
        imageWidth = static_cast<unsigned int>(imageHeight * 4.0 / 3.0);

        // Query for available Format 7 modes
        Format7Info fmt7Info;
        bool supported;
        fmt7Info.mode = MODE_0;
        error = cam.GetFormat7Info( &fmt7Info, &supported ) ;

        imageHeight -= imageHeight % fmt7Info.imageVStepSize;
        imageWidth -= imageWidth % fmt7Info.imageHStepSize;

        Format7ImageSettings fmt7ImageSettings;
        fmt7ImageSettings.mode = MODE_0;
        fmt7ImageSettings.offsetX = 0;
        fmt7ImageSettings.offsetY = 0;
        fmt7ImageSettings.width = imageWidth;
        fmt7ImageSettings.height = imageHeight;
        fmt7ImageSettings.pixelFormat = image.GetPixelFormat();

        // Set the settings to the camera
        error = cam.SetFormat7Configuration(
            &fmt7ImageSettings,
            100.0f );
        if (error != PGRERROR_OK)
        {
            PrintError( error );
            return -1;
        }

        printf( "Full size image does not fit in memory.\n" );
        printf( "Using Format7 image of size %ux%u, %u bytes per pixel.\n", 
            imageWidth, imageHeight, bytesPerPixel );
    }

    // Start capture
    error = cam.StartCapture();
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    // Grab 10 images to stabilize the image, but only keep the last one
    //
    for ( int i=0; i < 10; i++ )
    {
        cam.RetrieveBuffer( &image );
        if (error != PGRERROR_OK)
        {
            PrintError( error );
            return -1;
        }
    }

    // Stop capturing images
    error = cam.StopCapture();
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }      
    //两种模式的真正动作差异性在这里体现~~
    if ( flashMode == CAPTURE )
    {
        printf( "Starting writing of image to flash\n" );

        const unsigned int dataFlashLocation = GetDataFlashOffset( &cam );
    //写寄存器区块很关键
        error = cam.WriteRegisterBlock(
            0xFFFF,
            0xF0000000 + (dataFlashLocation*4),
            (unsigned int*)image.GetData(),
            image.GetDataSize() / 4 );
        if (error != PGRERROR_OK)
        {
            PrintError( error );
            return -1;
        }

        printf( "Successfully wrote image to flash\n" );
    }
    else if ( flashMode == RETRIEVE )
    {
        printf( "Starting reading of image from flash\n" );

        unsigned int dataFlashLocation = GetDataFlashOffset( &cam );
        unsigned int imageSize = image.GetDataSize();
        unsigned char* pData = new unsigned char[imageSize];

        error = cam.ReadRegisterBlock(
            0xFFFF,
            0xF0000000 + (dataFlashLocation*4),
            (unsigned int*)pData,
            imageSize / 4 );

        Image retrievedImage(
            image.GetRows(),
            image.GetCols(),
            image.GetStride(),
            pData,
            image.GetStride() * image.GetRows(),
            image.GetPixelFormat(),
            image.GetBayerTileFormat() );

        char filename[64];
        sprintf( filename, "%u_from_flash.pgm", camInfo.serialNumber );//这里文件名为何要用device的 serialNumber来命名，这样岂不是极容易重复？

        retrievedImage.Save( filename );

        delete [] pData;

        printf( "Successfully retrieved image from camera to %s\n", filename );
    }    

    // Disconnect the camera
    error = cam.Disconnect();
    if (error != PGRERROR_OK)
    {
        PrintError( error );
        return -1;
    }

    printf( "Done! Press Enter to exit...\n" );
    getchar();

	return 0;
}
```

# 关于flash的碎碎念
最开始读代码的时候并不知道flash是什么，还以为是flash视频，或者Adobe的flash那种。

不过读到后面我有一种感觉，特别是main()中后面对两种模式的处理。在没有搜索相关资料的情况下，我粗略判断flash是属于硬件的某种术语，或者称之为闪存比较好吧。因为在学习stm32的时候，有一点印象是stm32和flash有一定的关系。当然因为学习时间太短，并未有所了解。

在CAPTURE模式下,
>Starting writing of image to flash
然后一个uint类型的dataFlashLocation，其值由GetDataFlashOffset(&cam)得到（返回的是一个寄存器值）。这个应该是一个flash的硬件内存地址。
然后写寄存器。
然后就
>Successfully wrote image to flash。
显然是写到相机硬件中，而尚未将图像数据包传送到计算机或者说主处理器这边来。


在RETRIVE模式下，
>Starting reading of image from flash
然后也是dataFlashLocation，和上面同样的操作。
然后就是定义两个图像数据有关的变量imageSize,*pData。
然后读寄存器。
然后始化一个Image类，这里用到了最开始接收的一个image实例的一些"边缘"信息（比如行列步长格式），以及从上一步读寄存器操作中所获得的真正的数据pData。
然后就
>Successfully retrieved image from camera to FILENAME


