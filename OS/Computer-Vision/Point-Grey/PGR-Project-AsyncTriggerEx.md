---
date: 2016-03-10 22:49
status: public
title: PGR-Project-AsyncTriggerEx
---

# 项目文件结构
- AsyncTriggerEx
    - 引用
    - 外部依赖项
    - AsyncTriggerEx.cpp
    - AsyncTriggerEx.rc
    - resource.h
    - stdafx.cpp
    - stdafx.h
    
# AsyncTriggerEx.cpp
```python:n
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
// $Id: AsyncTriggerEx.cpp,v 1.21 2010-07-22 22:51:51 soowei Exp $
//=============================================================================

#include "stdafx.h"
#ifdef LINUX
#include <unistd.h>
#endif

#include "FlyCapture2.h"

//使用软件触发器，而不是外部硬件。是否相机上有可编程器件？软件触发和硬件触发的区别和联系究竟在哪里？
// Software trigger the camera instead of using an external hardware trigger
//
#define SOFTWARE_TRIGGER_CAMERA

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

void PrintError( Error error )
{
	error.PrintErrorTrace();
}

bool CheckSoftwareTriggerPresence( Camera* pCam )
{
	const unsigned int k_triggerInq = 0x530;

	Error error;
	unsigned int regVal = 0;

	error = pCam->ReadRegister( k_triggerInq, &regVal );

	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return false;
	}

	if( ( regVal & 0x10000 ) != 0x10000 )
	{
		return false;
	}

	return true;
}

bool PollForTriggerReady( Camera* pCam )//为准备触发轮询poll【计算机】顺序询问，轮询；探询
{
	const unsigned int k_softwareTrigger = 0x62C;
	Error error;
	unsigned int regVal = 0;

	do 
	{
		error = pCam->ReadRegister( k_softwareTrigger, &regVal );
		if (error != PGRERROR_OK)
		{
			PrintError( error );
			return false;
		}

	} while ( (regVal >> 31) != 0 );

	return true;
}

bool FireSoftwareTrigger( Camera* pCam )//火线传输软件触发？火线连的是电脑端~
{
	const unsigned int k_softwareTrigger = 0x62C;
	const unsigned int k_fireVal = 0x80000000;
	Error error;    

	error = pCam->WriteRegister( k_softwareTrigger, k_fireVal );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return false;
	}

	return true;
}

int main(int /*argc*/, char** /*argv*/)
{
	PrintBuildInfo();

	const int k_numImages = 10;

	Error error;

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

	// Power on the camera 
	//这里是给相机上电么，对于嵌入式设备和计算机有所区分？上电都要写寄存器，之前的代码程序里面并没有这一步。。。所以这一步的意义在于？
	const unsigned int k_cameraPower = 0x610;
	const unsigned int k_powerVal = 0x80000000;
	error  = cam.WriteRegister( k_cameraPower, k_powerVal );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}

	const unsigned int millisecondsToSleep = 100;
	unsigned int regVal = 0;
	unsigned int retries = 10;

	// Wait for camera to complete power-up
	do 
	{
#if defined(WIN32) || defined(WIN64)
		Sleep(millisecondsToSleep);    //
#else
		usleep(millisecondsToSleep * 1000);
#endif
		error = cam.ReadRegister(k_cameraPower, &regVal);
		if (error == PGRERROR_TIMEOUT)//在这个do循环里面忽略超时错误，到循环外再处理，这个是为什么？因为上电时相机可能不会响应寄存器读操作？
		{
			// ignore timeout errors, camera may not be responding to
			// register reads during power-up
		}
		else if (error != PGRERROR_OK)
		{
			PrintError( error );
			return -1;
		}

		retries--;
	} while ((regVal & k_powerVal) == 0 && retries > 0);//循环条件是重试次数以内，以及上电寄存器值不为全0。

	// Check for timeout errors after retrying 
	//重试完成之后，又检查是否超时。因为有可能最后一次重试读到的值为0吧。这里应该是对最后一次情况的检测
	if (error == PGRERROR_TIMEOUT)
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

#ifndef SOFTWARE_TRIGGER_CAMERA
	// Check for external trigger support检查外部触发器支持
	TriggerModeInfo triggerModeInfo;
	error = cam.GetTriggerModeInfo( &triggerModeInfo );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}

	if ( triggerModeInfo.present != true )
	{
		printf( "Camera does not support external trigger! Exiting...\n" );
		return -1;
	}
#endif
	
	// Get current trigger settings
	TriggerMode triggerMode;
	error = cam.GetTriggerMode( &triggerMode );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}

	// Set camera to trigger mode 0
	triggerMode.onOff = true;
	triggerMode.mode = 0;
	triggerMode.parameter = 0;

#ifdef SOFTWARE_TRIGGER_CAMERA
	// A source of 7 means software trigger
	triggerMode.source = 7;
#else
	// Triggering the camera externally using source 0.
	triggerMode.source = 0;
#endif
	
	error = cam.SetTriggerMode( &triggerMode );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}
	
	// Poll to ensure camera is ready 顺序询问各个触发模式对应的GPIO？
	bool retVal = PollForTriggerReady( &cam );
	if( !retVal )
	{
		printf("\nError polling for trigger ready!\n");
		return -1;
	}

	// Get the camera configuration
	FC2Config config;
	error = cam.GetConfiguration( &config );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	} 
	
	// Set the grab timeout to 5 seconds
	config.grabTimeout = 5000;

	// Set the camera configuration
	error = cam.SetConfiguration( &config );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	} 

	// Camera is ready, start capturing images
	error = cam.StartCapture();
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}   
	
#ifdef SOFTWARE_TRIGGER_CAMERA
	if (!CheckSoftwareTriggerPresence( &cam ))
	{
		printf( "SOFT_ASYNC_TRIGGER not implemented on this camera!  Stopping application\n");
		return -1;
	}
#else	
	printf( "Trigger the camera by sending a trigger pulse to GPIO%d.\n", 
	  triggerMode.source );
#endif

	Image image;
	for ( int imageCount=0; imageCount < k_numImages; imageCount++ )
	{

#ifdef SOFTWARE_TRIGGER_CAMERA
		// Check that the trigger is ready
		PollForTriggerReady( &cam);

		printf( "Press the Enter key to initiate a software trigger.\n" );
		getchar();

		// Fire software trigger
		bool retVal = FireSoftwareTrigger( &cam );
		if ( !retVal )
		{
			printf("\nError firing software trigger!\n");
			return -1;        
		}
#endif

		// Grab image        
		error = cam.RetrieveBuffer( &image );
		if (error != PGRERROR_OK)
		{
			PrintError( error );
			break;
		}
		printf(".\n");
	}

	// Turn trigger mode off.
	triggerMode.onOff = false;    
	error = cam.SetTriggerMode( &triggerMode );
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}
	printf( "\nFinished grabbing images\n" );

	// Stop capturing images
	error = cam.StopCapture();
	if (error != PGRERROR_OK)
	{
		PrintError( error );
		return -1;
	}      

	// Turn off trigger mode
	triggerMode.onOff = false;
	error = cam.SetTriggerMode( &triggerMode );
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
```