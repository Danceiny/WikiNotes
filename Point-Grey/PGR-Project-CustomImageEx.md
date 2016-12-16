---
date: 2016-03-05 16:46
status: public
title: PGR-Project-CustomImageEx
---

# 碎碎
问题关键在于**对Format7的格式不理解。**

- **convert the raw image as PIXEL_FORMAT_BGRU。**

- 该程序保存的文件格式是bmp位图。

- 有点特别的是，抓取到rawImage后，有一个GetDimensions操作。获得的数据好像又没有应用，好生奇怪。【252行】

其中比较复杂的一个代码块。大致的流程是：
- GetFormatInfo()
- PrintFormat7Capabilities()
- 判断pixel format是否受支持（两个比较值是常量PIXEL_FORMAT_MONO8（像素格式枚举）和第一步获取的info）
- 为Format7ImageSettings类的实例赋值
- 初始化一个Format7PacketInfo实例(下一步判断应该会给这个实例的属性赋值，然后用于最后的Configuration）
- ValidateFormat7Settings()，to "Validate the (Format7)settings to make suere that they are valid"
- SetFormat7Configuration() 

还有一点，开始拍照后，程序尝试获取帧率。然后才开始队列抓取保存。

# CustomImageEx.cpp
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
// $Id: CustomImageEx.cpp,v 1.20 2010-02-26 23:24:47 soowei Exp $
//=============================================================================

#include "stdafx.h"
#include "stdio.h"

#include "FlyCapture2.h"

using namespace FlyCapture2;

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

// 打印该格式的容量
void PrintFormat7Capabilities( Format7Info fmt7Info )
{
	printf(
		"Max image pixels: (%u, %u)\n"
		"Image Unit size: (%u, %u)\n"
		"Offset Unit size: (%u, %u)\n"
		"Pixel format bitfield: 0x%08x\n",
		fmt7Info.maxWidth,
		fmt7Info.maxHeight,
		fmt7Info.imageHStepSize,
		fmt7Info.imageVStepSize,
		fmt7Info.offsetHStepSize,
		fmt7Info.offsetVStepSize,
		fmt7Info.pixelFormatBitField );
}

void PrintError( Error error )
{
	error.PrintErrorTrace();
}

int main(int /*argc*/, char** /*argv*/)
{
	PrintBuildInfo();

	const Mode k_fmt7Mode = MODE_0;
	const PixelFormat k_fmt7PixFmt = PIXEL_FORMAT_MONO8;
	const int k_numImages = 10;

	Error error;

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

	// Query for available Format 7 modes
	Format7Info fmt7Info;
	bool supported;
	fmt7Info.mode = k_fmt7Mode;
	error = cam.GetFormat7Info( &fmt7Info, &supported );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}

	PrintFormat7Capabilities( fmt7Info );

	if ( (k_fmt7PixFmt & fmt7Info.pixelFormatBitField) == 0 )
	{
		// Pixel format not supported!
		printf("Pixel format is not supported\n");
		return -1;
	}
	
	Format7ImageSettings fmt7ImageSettings;
	fmt7ImageSettings.mode = k_fmt7Mode;
	fmt7ImageSettings.offsetX = 0;
	fmt7ImageSettings.offsetY = 0;
	fmt7ImageSettings.width = fmt7Info.maxWidth;
	fmt7ImageSettings.height = fmt7Info.maxHeight;
	fmt7ImageSettings.pixelFormat = k_fmt7PixFmt;

	bool valid;
	Format7PacketInfo fmt7PacketInfo;

	// Validate the settings to make sure that they are valid
	error = cam.ValidateFormat7Settings(
		&fmt7ImageSettings,
		&valid,
		&fmt7PacketInfo );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}

	if ( !valid )
	{
		// Settings are not valid
		printf("Format7 settings are not valid\n");
		return -1;
	}

	// Set the settings to the camera
	error = cam.SetFormat7Configuration(
		&fmt7ImageSettings,
		fmt7PacketInfo.recommendedBytesPerPacket );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}

	// Start capturing images
	error = cam.StartCapture();
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}

	// Retrieve frame rate property
	Property frmRate;
	frmRate.type = FRAME_RATE;
	error = cam.GetProperty( &frmRate );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}

	printf( "Frame rate is %3.2f fps\n", frmRate.absValue );

	printf( "Grabbing %d images\n", k_numImages );

	Image rawImage;    
	for ( int imageCount=0; imageCount < k_numImages; imageCount++ )
	{
		// Retrieve an image
		error = cam.RetrieveBuffer( &rawImage );
		if (error != PGRERROR_OK)
		{
			PrintError( error );
			continue;
		}

		printf( "." );

		// Get the raw image dimensions
		PixelFormat pixFormat;
		unsigned int rows, cols, stride;
		rawImage.GetDimensions( &rows, &cols, &stride, &pixFormat );

		// Create a converted image
		Image convertedImage;

		// Convert the raw image
		error = rawImage.Convert( PIXEL_FORMAT_BGRU, &convertedImage );
		if (error != PGRERROR_OK)
		{
			PrintError( error );
			return -1;
		}  

		// Create a unique filename
		char filename[512];
		sprintf( filename, "%u-%d.bmp", camInfo.serialNumber, imageCount );

		// Save the image. If a file format is not passed in, then the file
		// extension is parsed to attempt to determine the file format.
		error = convertedImage.Save( filename );
		if (error != PGRERROR_OK)
		{
			PrintError( error );
			return -1;
		}  
	}

	printf( "\nFinished grabbing images\n" );

	// Stop capturing images
	error = cam.StopCapture();
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
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
```a
