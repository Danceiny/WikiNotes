---
date: 2016-02-29 18:11
status: public
title: PGR-Project-FlyCapture2Test_C
---

# FlyCapture2Test_C.c 分析理解

```c:n
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
// $Id: FlyCapture2Test_C.c,v 1.29 2010/04/13 21:35:02 hirokim Exp $
//=============================================================================

// 定义平台（但是VS10为何只能用x64才能build成功，而用win32则无法打开链接某lib而失败。
#if defined(WIN32) || defined(WIN64)
#define _CRT_SECURE_NO_WARNINGS		
#endif

#include "C/FlyCapture2_C.h"    
//这种包含文件真奇怪
//右键转到该文件一看，哎哟，叫FlyCapture2_C.h，里面有一段代码挺有意思：
// #ifdef __cplusplus extern "C"{#endif
// 我猜测这意思是如果用C++编译器的话，则用个C前缀打头吧。一贯的风格而已。这种实现方式真让人长见识。
#include <stdio.h>

// 打印解决方案生成的信息（SDK库版本号，build时间戳）
void PrintBuildInfo()   
{
	fc2Version version;
	/* 当前的flycapture2库版本
	typedef struct _fc2Version
{
	unsigned int major;
	unsigned int minor;
	unsigned int type;
	unsigned int build;
} fc2Version;
    */
	char versionStr[512];
	char timeStamp[512];

	fc2GetLibraryVersion( &version );

	sprintf( 
		versionStr, 
		"FlyCapture2 library version: %d.%d.%d.%d\n", 
		version.major, version.minor, version.type, version.build );
		// 输出版本号为2.6.3.2
	printf( "%s", versionStr );
    
	sprintf( timeStamp, "Application build date: %s %s\n\n", __DATE__, __TIME__ );
    // 输出build时间（日期和时分秒）
    // 注意是build的时间。解决方案生成后，时间就定了。除非重新生成。否则再执行这段代码仍然显示是build时间而非当前时间。
	printf( "%s", timeStamp );
}

// 打印相机信息
void PrintCameraInfo( fc2Context context )
{
    /**
    typedef enum _fc2Error
{
	FC2_ERROR_UNDEFINED = -1, /**< Undefined */
	FC2_ERROR_OK, /**< Function returned with no errors. */
	FC2_ERROR_FAILED, /**< General failure. */
	FC2_ERROR_NOT_IMPLEMENTED, /**< Function has not been implemented. */
	FC2_ERROR_FAILED_BUS_MASTER_CONNECTION, /**< Could not connect to Bus Master. */
	FC2_ERROR_NOT_CONNECTED, /**< Camera has not been connected. */
	FC2_ERROR_INIT_FAILED, /**< Initialization failed. */ 
	FC2_ERROR_NOT_INTITIALIZED, /**< Camera has not been initialized. */
	FC2_ERROR_INVALID_PARAMETER, /**< Invalid parameter passed to function. */
	FC2_ERROR_INVALID_SETTINGS, /**< Setting set to camera is invalid. */         
	FC2_ERROR_INVALID_BUS_MANAGER, /**< Invalid Bus Manager object. */
	FC2_ERROR_MEMORY_ALLOCATION_FAILED, /**< Could not allocate memory. */ 
	FC2_ERROR_LOW_LEVEL_FAILURE, /**< Low level error. */
	FC2_ERROR_NOT_FOUND, /**< Device not found. */
	FC2_ERROR_FAILED_GUID, /**< GUID failure. */
	FC2_ERROR_INVALID_PACKET_SIZE, /**< Packet size set to camera is invalid. */
	FC2_ERROR_INVALID_MODE, /**< Invalid mode has been passed to function. */
	FC2_ERROR_NOT_IN_FORMAT7, /**< Error due to not being in Format7. */
	FC2_ERROR_NOT_SUPPORTED, /**< This feature is unsupported. */
	FC2_ERROR_TIMEOUT, /**< Timeout error. */
	FC2_ERROR_BUS_MASTER_FAILED, /**< Bus Master Failure. */
	FC2_ERROR_INVALID_GENERATION, /**< Generation Count Mismatch. */
	FC2_ERROR_LUT_FAILED, /**< Look Up Table failure. */
	FC2_ERROR_IIDC_FAILED, /**< IIDC failure. */
	FC2_ERROR_STROBE_FAILED, /**< Strobe failure. */
	FC2_ERROR_TRIGGER_FAILED, /**< Trigger failure. */
	FC2_ERROR_PROPERTY_FAILED, /**< Property failure. */
	FC2_ERROR_PROPERTY_NOT_PRESENT, /**< Property is not present. */
	FC2_ERROR_REGISTER_FAILED, /**< Register access failed. */
	FC2_ERROR_READ_REGISTER_FAILED, /**< Register read failed. */
	FC2_ERROR_WRITE_REGISTER_FAILED, /**< Register write failed. */
	FC2_ERROR_ISOCH_FAILED, /**< Isochronous failure. */
	FC2_ERROR_ISOCH_ALREADY_STARTED, /**< Isochronous transfer has already been started. */
	FC2_ERROR_ISOCH_NOT_STARTED, /**< Isochronous transfer has not been started. */
	FC2_ERROR_ISOCH_START_FAILED, /**< Isochronous start failed. */
	FC2_ERROR_ISOCH_RETRIEVE_BUFFER_FAILED, /**< Isochronous retrieve buffer failed. */
	FC2_ERROR_ISOCH_STOP_FAILED, /**< Isochronous stop failed. */
	FC2_ERROR_ISOCH_SYNC_FAILED, /**< Isochronous image synchronization failed. */
	FC2_ERROR_ISOCH_BANDWIDTH_EXCEEDED, /**< Isochronous bandwidth exceeded. */
	FC2_ERROR_IMAGE_CONVERSION_FAILED, /**< Image conversion failed. */
	FC2_ERROR_IMAGE_LIBRARY_FAILURE, /**< Image library failure. */
	FC2_ERROR_BUFFER_TOO_SMALL, /**< Buffer is too small. */
	FC2_ERROR_IMAGE_CONSISTENCY_ERROR, /**< There is an image consistency error. */
	FC2_ERROR_FORCE_32BITS = FULL_32BIT_VALUE

} fc2Error;  
**/
	fc2Error error;// 错误代号好理解
    /*
    typedef struct _fc2CameraInfo
{      
	unsigned int serialNumber;
	fc2InterfaceType interfaceType;
	fc2DriverType driverType;
	BOOL isColorCamera;
	char modelName[ MAX_STRING_LENGTH];
	char vendorName[ MAX_STRING_LENGTH];
	char sensorInfo[ MAX_STRING_LENGTH];
	char sensorResolution[ MAX_STRING_LENGTH];
	char driverName[ MAX_STRING_LENGTH];
	char firmwareVersion[ MAX_STRING_LENGTH];
	char firmwareBuildTime[ MAX_STRING_LENGTH];
	fc2BusSpeed maximumBusSpeed;
	fc2PCIeBusSpeed pcieBusSpeed;
	fc2BayerTileFormat bayerTileFormat;
	unsigned short busNumber;
	unsigned short nodeNumber;

	// IIDC specific information
	unsigned int iidcVer;
	fc2ConfigROM configROM;

	// GigE specific information
	unsigned int gigEMajorVersion;
	unsigned int gigEMinorVersion;
	char userDefinedName[ MAX_STRING_LENGTH];
	char xmlURL1[ MAX_STRING_LENGTH];
	char xmlURL2[ MAX_STRING_LENGTH];
	fc2MACAddress macAddress;
	fc2IPAddress ipAddress;
	fc2IPAddress subnetMask;
	fc2IPAddress defaultGateway;

	/** Status/Content of CCP register */
	unsigned int ccpStatus;
	/** Local Application IP Address. */
	unsigned int applicationIPAddress;
	/** Local Application port. */
	unsigned int applicationPort;

	unsigned int reserved[16];

} fc2CameraInfo;
*/	
	fc2CameraInfo camInfo;
	error = fc2GetCameraInfo( context, &camInfo );
	if ( error != FC2_ERROR_OK )//这就是有错误咯，应该进行错误处理
	{
		// Error
	}
    
    // 相机正常连接并成功获得有关参数信息，打印
	printf(
		"\n*** CAMERA INFORMATION ***\n"
		"Serial number - %u\n"
		"Camera model - %s\n"
		"Camera vendor - %s\n"
		"Sensor - %s\n"
		"Resolution - %s\n"
		"Firmware version - %s\n"
		"Firmware build time - %s\n\n",
		camInfo.serialNumber,
		camInfo.modelName,
		camInfo.vendorName,
		camInfo.sensorInfo,
		camInfo.sensorResolution,
		camInfo.firmwareVersion,
		camInfo.firmwareBuildTime );
}

void SetTimeStamping( fc2Context context, BOOL enableTimeStamp )
{
	fc2Error error;
    // 嵌入式图像信息
    /*typedef struct _fc2EmbeddedImageInfo
{
	fc2EmbeddedImageInfoProperty timestamp;
	fc2EmbeddedImageInfoProperty gain;
	fc2EmbeddedImageInfoProperty shutter;
	fc2EmbeddedImageInfoProperty brightness;
	fc2EmbeddedImageInfoProperty exposure;
	fc2EmbeddedImageInfoProperty whiteBalance;
	fc2EmbeddedImageInfoProperty frameCounter;
	fc2EmbeddedImageInfoProperty strobePattern;
	fc2EmbeddedImageInfoProperty GPIOPinState;
	fc2EmbeddedImageInfoProperty ROIPosition;

} fc2EmbeddedImageInfo;*/

	fc2EmbeddedImageInfo embeddedInfo;

	error = fc2GetEmbeddedImageInfo( context, &embeddedInfo );
	if ( error != FC2_ERROR_OK )
	{
	// 连接嵌入式（硬件）图像信息出错
		printf( "Error in fc2GetEmbeddedImageInfo: %d\n", error );
	}

	if ( embeddedInfo.timestamp.available != 0 )
	{       
    // 时间戳不可用（相机上的）
		embeddedInfo.timestamp.onOff = enableTimeStamp;
	}    
    // 和上面那句代码长得好像。。。不过这个是Set，上面是Get。
	error = fc2SetEmbeddedImageInfo( context, &embeddedInfo );
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2SetEmbeddedImageInfo: %d\n", error );
	}
}

//终于到抓取图像了
// 这些个函数，上下文总是第一个参数。
// 既然抓取图像，就要指定一个图像的编号来抓
void GrabImages( fc2Context context, int numImagesToGrab )
{
	fc2Error error;
	/*typedef struct _fc2Image
{
	unsigned int rows;
	unsigned int cols;
	unsigned int stride;
	unsigned char* pData;
	unsigned int dataSize;
	unsigned int receivedDataSize;
	fc2PixelFormat format;
	fc2BayerTileFormat bayerFormat;

	fc2ImageImpl imageImpl;

} fc2Image;*/
	fc2Image rawImage; //抓来的首先肯定是原始图像
	fc2Image convertedImage;//对原始图像要做转换
	fc2TimeStamp prevTimestamp = {0};//这里是个大括号，惊呆了。原来是个结构体。
	/*
typedef struct _fc2TimeStamp
{
	long long seconds;
	unsigned int microSeconds;
	unsigned int cycleSeconds;
	unsigned int cycleCount;
	unsigned int cycleOffset;
	unsigned int reserved[8];

} fc2TimeStamp;*/
	int i;

	error = fc2CreateImage( &rawImage );
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2CreateImage: %d\n", error );
	}

	error = fc2CreateImage( &convertedImage );
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2CreateImage: %d\n", error );
	}

	// If externally allocated memory is to be used for the converted image,
	// simply assigning the pData member of the fc2Image structure is
	// insufficient. fc2SetImageData() should be called in order to populate
	// the fc2Image structure correctly. This can be done at this point,
	// assuming that the memory has already been allocated.

	for ( i=0; i < numImagesToGrab; i++ )
	{
		// 与硬件交流，错误处理尤其重要，不可马虎
		// Retrieve the image
		error = fc2RetrieveBuffer( context, &rawImage );
		if ( error != FC2_ERROR_OK )
		{
			printf( "Error in retrieveBuffer: %d\n", error);
		}
		else
		{
			// Get and print out the time stamp
			fc2TimeStamp ts = fc2GetImageTimeStamp( &rawImage);//不过fc2Image结构体的哪一个数据包含了时间戳啊。。。
			// diff应该可以指示抓取一张图像所耗费的时间？
			int diff = (ts.cycleSeconds - prevTimestamp.cycleSeconds) * 8000
						+ (ts.cycleCount - prevTimestamp.cycleCount);
			prevTimestamp = ts;
			printf( 
				"timestamp [%d %d] - %d\n", 
				ts.cycleSeconds, 
				ts.cycleCount, 
				diff );
		}       
	}

	if ( error == FC2_ERROR_OK )
	{
		// Convert the final image to RGB
		error = fc2ConvertImageTo(FC2_PIXEL_FORMAT_BGR, &rawImage, &convertedImage);/**像素格式 < 24 bit BGR. */
		if ( error != FC2_ERROR_OK )
		{
			printf( "Error in fc2ConvertImageTo: %d\n", error );
		}

		// Save it to PNG
		printf("Saving the last image to fc2TestImage.png \n");
		error = fc2SaveImage( &convertedImage, "fc2TestImage.png", FC2_PNG );//将转换后的图像保存为png图像文件格式
		if ( error != FC2_ERROR_OK )
		{
			printf( "Error in fc2SaveImage: %d\n", error );
			printf( "Please check write permissions.\n");
		}
	}
    //销毁原图像
	error = fc2DestroyImage( &rawImage );
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2DestroyImage: %d\n", error );
	}
    // 销毁转换后图像
	error = fc2DestroyImage( &convertedImage );
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2DestroyImage: %d\n", error );
	}
}
//特大喜讯，终于到主函数了。。。
int main(int argc, char** argv)
{        
	fc2Error error;
	fc2Context context;
	fc2PGRGuid guid;
	unsigned int numCameras = 0;    
	const int k_numImages = 100;

	PrintBuildInfo();

	error = fc2CreateContext( &context );//这个上下文概念应该挺重要。不过这个FLYCAPTURE2_C_API(用define定义，加载dll`#define FLYCAPTURE2_C_API __declspec( dllimport )`)全都给封装了，不知道细节
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2CreateContext: %d\n", error );
		return 0;
	}        
    //检测到有几个相机~
	error = fc2GetNumOfCameras( context, &numCameras );
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2GetNumOfCameras: %d\n", error );
		return 0;
	}

	if ( numCameras == 0 )
	{
		// No cameras detected
		printf( "No cameras detected.\n");
		return 0;
	}
    //相机序列~从0开始
	// Get the 0th camera
	
/**
 * Gets the PGRGuid for a camera on the PC. It uniquely identifies
 * the camera specified by the index and is used to identify the camera
 * during a fc2Connect() call.
 *
 * @param context The fc2Context to be used.
 * @param index Zero based index of camera.
 * @param pGuid Unique PGRGuid for the camera.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
	error = fc2GetCameraFromIndex( context, 0, &guid );
/** fc2PGRGuid struct
 * A GUID to the camera.  It is used to uniquely identify a camera.
 */ 
typedef struct _fc2PGRGuid
{
	unsigned int value[4];

} fc2PGRGuid;	**/
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2GetCameraFromIndex: %d\n", error );
		return 0;
	}    
    
    //连接相机
	error = fc2Connect( context, &guid );
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2Connect: %d\n", error );
		return 0;
	}

	PrintCameraInfo( context );    

	SetTimeStamping( context, TRUE );

    // 开始同步传输图像。使用现有视频模式或最近设置的模式。
	error = fc2StartCapture( context );
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2StartCapture: %d\n", error );
		return 0;
	}

	GrabImages( context, k_numImages );   

	error = fc2StopCapture( context );
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2StopCapture: %d\n", error );
		return 0;
	}

	error = fc2DestroyContext( context );
	if ( error != FC2_ERROR_OK )
	{
		printf( "Error in fc2DestroyContext: %d\n", error );
		return 0;
	}

	printf( "Done! Press Enter to exit...\n" );
	getchar();

	return 0;
}
```

执行这个程序（本地电脑，未接入相机），输出如下：

![](~/11-51-43.jpg)

不知道status那几行是哪个代码打印的。

单步执行发现，是fc2CreateContext()。
>/**
 * Create a FC2 context for IIDC camaera. 
 * This call must be made before any other calls that use a context
 * will succeed.
 *
 * @param pContext A pointer to the fc2Context to be created.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
 
# FlyCapture2_C.h
```c:n
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
// $Id: FlyCapture2_C.h,v 1.69 2010-12-13 23:58:00 mgara Exp $
//=============================================================================

#ifndef PGR_FC2_FLYCAPTURE2_C_H
#define PGR_FC2_FLYCAPTURE2_C_H

//=============================================================================
// Global C header file for FlyCapture2. 
//
// This file defines the C API for FlyCapture2
//=============================================================================

#include "FlyCapture2Platform_C.h"
#include "FlyCapture2Defs_C.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Create a FC2 context for IIDC camaera. 
 * This call must be made before any other calls that use a context
 * will succeed.
 *
 * @param pContext A pointer to the fc2Context to be created.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2CreateContext(
	fc2Context* pContext );

/**
 * Create a FC2 context for a GigE Vision camera. 
 * This call must be made before any other calls that use a context
 * will succeed.
 *
 * @param pContext A pointer to the fc2Context to be created.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2CreateGigEContext(
	fc2Context* pContext );

/**
 * Destroy the FC2 context. This must be called when the user is finished
 * with the context in order to prevent memory leaks.
 *
 * @param context The context to be destroyed.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2DestroyContext(
	fc2Context context );

/** 
 * Fire a bus reset. The actual bus reset is only fired for the 
 * specified 1394 bus, but it will effectively cause a global bus
 * reset for the library.
 *
 * @param context The fc2Context to be used.
 * @param pGuid PGRGuid of the camera or the device to cause bus reset.
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2FireBusReset( 
	fc2Context context,
	fc2PGRGuid* pGuid);

/**
 * Gets the number of cameras attached to the PC.
 *
 * @param context The fc2Context to be used.
 * @param pNumCameras Number of cameras detected.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetNumOfCameras(
	fc2Context context,
	unsigned int* pNumCameras );

/**
 * Query whether a GigE camera is controlable.
 *
 * @param context The fc2Context to be used.
 * @param pGuid Unique PGRGuid for the camera.
 * @param pControlable True indicates camera is controllable
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2IsCameraControlable(
	fc2Context context,
	fc2PGRGuid* pGuid,
	BOOL* pControlable );

/**
 * Gets the PGRGuid for a camera on the PC. It uniquely identifies
 * the camera specified by the index and is used to identify the camera
 * during a fc2Connect() call.
 *
 * @param context The fc2Context to be used.
 * @param index Zero based index of camera.
 * @param pGuid Unique PGRGuid for the camera.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetCameraFromIndex(
	fc2Context context,
	unsigned int index,
	fc2PGRGuid* pGuid );

/**
 * Gets the PGRGuid for a camera on the PC. It uniquely identifies
 * the camera specified by the serial number and is used to identify the camera
 * during a fc2Connect() call.
 *
 * @param context The fc2Context to be used.
 * @param serialNumber Serial number of camera.
 * @param pGuid Unique PGRGuid for the camera.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetCameraFromSerialNumber(
	fc2Context context,
	unsigned int serialNumber,
	fc2PGRGuid* pGuid );

/**
 * Gets the serial number of the camera with the specified index.
 *
 * @param context The fc2Context to be used.
 * @param index Zero based index of desired camera.
 * @param pSerialNumber Serial number of camera.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetCameraSerialNumberFromIndex(
	fc2Context context,
	unsigned int index,
	unsigned int* pSerialNumber );

/**
 * Gets the interface type associated with a PGRGuid. This is useful
 * in situations where there is a need to enumerate all cameras
 * for a particular interface.
 *
 * @param context The fc2Context to be used.
 * @param pGuid The PGRGuid to get the interface for.
 * @param pInterfaceType The interface type of the PGRGuid.
 *
 * @return
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetInterfaceTypeFromGuid(
	fc2Context context,
	fc2PGRGuid *pGuid,
	fc2InterfaceType* pInterfaceType );

/**
 * Gets the number of devices. This may include hubs, host controllers
 * and other hardware devices (including cameras).
 *
 * @param context The fc2Context to be used.
 * @param pNumDevices The number of devices found.
 *
 * @return An Error indicating the success or failure of the function.
 */ 
FLYCAPTURE2_C_API fc2Error 
fc2GetNumOfDevices( 
	fc2Context context,
	unsigned int* pNumDevices );

/**
 * Gets the PGRGuid for a device. It uniquely identifies the device
 * specified by the index.
 *
 * @param context The fc2Context to be used.
 * @param index Zero based index of device.
 * @param pGuid Unique PGRGuid for the device.
 *
 * @see fc2GetNumOfDevices()
 *
 * @return An Error indicating the success or failure of the function.
 */ 
FLYCAPTURE2_C_API fc2Error 
fc2GetDeviceFromIndex( 
	fc2Context context,
	unsigned int index, 
	fc2PGRGuid*     pGuid );

/**
 * Register a callback function that will be called when the
 * specified callback event occurs.
 *
 * @param context The fc2Context to be used.
 * @param enumCallback Pointer to function that will receive the callback.
 * @param callbackType Type of callback to register for.
 * @param pParameter Callback parameter to be passed to callback.
 * @param pCallbackHandle Unique callback handle used for unregistering 
 *                        callback.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2RegisterCallback(
	fc2Context context,
	fc2BusEventCallback enumCallback,
	fc2BusCallbackType callbackType,
	void* pParameter,
	fc2CallbackHandle* pCallbackHandle );

/**
 * Unregister a callback function.
 *
 * @param context The fc2Context to be used.
 * @param callbackHandle Unique callback handle.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2UnregisterCallback(
	fc2Context context,
	fc2CallbackHandle callbackHandle );

/**
 * Force a rescan of the buses. This does not trigger a bus reset.
 * However, any current connections to a Camera object will be
 * invalidated.
 *
 * @return An Error indicating the success or failure of the function.
 */ 
FLYCAPTURE2_C_API fc2Error 
fc2RescanBus( fc2Context context);

/**
 * Force the camera with the specific MAC address to the specified
 * IP address, subnet mask and default gateway. This is useful in
 * situations where a GigE Vision camera is using Persistent IP and the
 * application's subnet is different from the device subnet.
 *
 * @param context The fc2Context to be used.
 * @param macAddress MAC address of the camera.
 * @param ipAddress IP address to set on the camera.
 * @param subnetMask Subnet mask to set on the camera.
 * @param defaultGateway Default gateway to set on the camera.
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2ForceIPAddressToCamera( 
	fc2Context context,
	fc2MACAddress macAddress,
	fc2IPAddress ipAddress,
	fc2IPAddress subnetMask,
	fc2IPAddress defaultGateway );

/**
* Force all cameras on the network to be assigned sequential IP addresses
* on the same subnet as the netowrk adapters that they are connected to. 
* This is useful in situations where a GigE Vision cameras are using 
* Persistent IP addresses and the application's subnet is different from 
* the devices.
*
* @return An Error indicating the success or failure of the function.
*/
FLYCAPTURE2_C_API fc2Error
fc2ForceAllIPAddressesAutomatically();

/**
* Force a cameras on the network to be assigned sequential IP addresses
* on the same subnet as the netowrk adapters that it is connected to. 
* This is useful in situations where a GigE Vision cameras is using 
* Persistent IP addresses and the application's subnet is different from 
* the device.
*
* @return An Error indicating the success or failure of the function.
*/
FLYCAPTURE2_C_API fc2Error
fc2ForceIPAddressAutomatically(unsigned int serialNumber);

/**
 * Discover all cameras connected to the network even if they reside
 * on a different subnet. This is useful in situations where a GigE
 * camera is using Persistent IP and the application's subnet is
 * different from the device subnet. After discovering the camera,  
 * it is easy to use ForceIPAddressToCamera() to set a different IP 
 * configuration.
 *
 * @param context The fc2Context to be used.
 * @param gigECameras Pointer to an array of CameraInfo structures.
 * @param arraySize Size of the array. Number of discovered cameras
 *                  is returned in the same value.
 *
 * @return An Error indicating the success or failure of the function.
 *         If the error is PGRERROR_BUFFER_TOO_SMALL then arraySize will
 *         contain the minimum size needed for gigECameras array.
 */
FLYCAPTURE2_C_API fc2Error 
fc2DiscoverGigECameras( 
	fc2Context context,
	fc2CameraInfo* gigECameras,
	unsigned int* arraySize  );

/**
 * Write to the specified register on the camera.
 *
 * @param context The fc2Context to be used.
 * @param address DCAM address to be written to.
 * @param value The value to be written.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2WriteRegister(
	fc2Context context,
	unsigned int address,
	unsigned int value);

/**
 * Write to the specified register on the camera with broadcast.
 *
 * @param context The fc2Context to be used.
 * @param address DCAM address to be written to.
 * @param value The value to be written.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2WriteRegisterBroadcast(
	fc2Context context,
	unsigned int address,
	unsigned int value);

/**
 * Read the specified register from the camera.
 *
 * @param context The fc2Context to be used.
 * @param address DCAM address to be read from.
 * @param pValue The value that is read.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2ReadRegister(
	fc2Context context,
	unsigned int address,
	unsigned int* pValue );

/**
 * Write to the specified register block on the camera.
 *
 * @param context The fc2Context to be used.
 * @param addressHigh Top 16 bits of the 48 bit absolute address to write to.
 * @param addressLow Bottom 32 bits of the 48 bits absolute address to write to.              
 * @param pBuffer Array containing data to be written.
 * @param length Size of array, in quadlets.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2WriteRegisterBlock(
	fc2Context context,
	unsigned short addressHigh,
	unsigned int addressLow,
	const unsigned int* pBuffer,
	unsigned int length );

/**
 * Write to the specified register block on the camera.
 *
 * @param context The fc2Context to be used.
 * @param addressHigh Top 16 bits of the 48 bit absolute address to read from.
 * @param addressLow Bottom 32 bits of the 48 bits absolute address to read from.                
 * @param pBuffer Array to store read data.
 * @param length Size of array, in quadlets.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2ReadRegisterBlock(
	fc2Context context,
	unsigned short addressHigh,
	unsigned int addressLow,
	unsigned int* pBuffer,
	unsigned int length );     

/**
 * Connects the camera object to the camera specified by the GUID.
 *
 * @param context The fc2Context to be used.
 * @param guid The unique identifier for a specific camera on the PC.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2Connect( 
	fc2Context context,
	fc2PGRGuid* guid );

/**
 * Disconnects the fc2Context from the camera.
 *
 * @param context The fc2Context to be used.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2Disconnect( 
	fc2Context context );

/**
 * Sets the callback data to be used on completion of image transfer.
 * To clear the current stored callback data, pass in NULL for both
 * callback arguments.
 *
 * @param context The fc2Context to be used.
 * @param pCallbackFn A function to be called when a new image is received.
 * @param pCallbackData A pointer to data that can be passed to the
 *                      callback function.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error
fc2SetCallback(
	fc2Context context,
	fc2ImageEventCallback pCallbackFn,
	void* pCallbackData);

/**
 * Starts isochronous image capture. It will use either the current
 * video mode or the most recently set video mode of the camera.
 *
 * @param context The fc2Context to be used.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2StartCapture( 
	fc2Context context );

/**
 * Starts isochronous image capture. It will use either the current
 * video mode or the most recently set video mode of the camera. The callback
 * function is called when a new image is received from the camera.
 *
 * @param context The fc2Context to be used.
 * @param pCallbackFn A function to be called when a new image is received.        
 * @param pCallbackData A pointer to data that can be passed to the
 *                      callback function. A NULL pointer is acceptable.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2StartCaptureCallback(
	fc2Context context,
	fc2ImageEventCallback pCallbackFn,
	void* pCallbackData);

/**
 * Starts synchronized isochronous image capture on multiple cameras.
 *
 * @param numCameras Number of fc2Contexts in the ppCameras array.
 * @param pContexts Array of fc2Contexts.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2StartSyncCapture( 
	unsigned int numCameras, 
	fc2Context *pContexts );

/**
 * Starts synchronized isochronous image capture on multiple cameras.
 *
 * @param numCameras Number of fc2Contexts in the ppCameras array.
 * @param pContexts Array of fc2Contexts.
 * @param pCallbackFns Array of callback functions for each camera.
 * @param pCallbackDataArray Array of callback data pointers. 
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2StartSyncCaptureCallback( 
	unsigned int numCameras, 
	fc2Context *pContexts,
	fc2ImageEventCallback* pCallbackFns,
	void** pCallbackDataArray);

/**
 * Retrieves the the next image object containing the next image.
 *
 * @param context The fc2Context to be used.
 * @param pImage Pointer to fc2Image to store image data.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2RetrieveBuffer( 
	fc2Context context,
	fc2Image* pImage );

/**
 * Stops isochronous image transfer and cleans up all associated
 * resources.
 *
 * @param context The fc2Context to be used.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2StopCapture( 
	fc2Context context );

/**
 * Specify user allocated buffers to use as image data buffers.
 *
 * @param context The fc2Context to be used.
 * @param ppMemBuffers Pointer to memory buffers to be written to. The
 *                     size of the data should be equal to 
 *                     (size * numBuffers) or larger.
 * @param size The size of each buffer (in bytes).
 * @param nNumBuffers Number of buffers in the array.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetUserBuffers(
	fc2Context context,
	unsigned char* const ppMemBuffers,
	int size,
	int nNumBuffers );

/**
 * Get the configuration associated with the camera.
 *
 * @param context The fc2Context to be used.
 * @param config Pointer to the configuration structure to be filled.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetConfiguration( 
	fc2Context context,
	fc2Config* config );

/**
 * Set the configuration associated with the camera.
 *
 * @param context The fc2Context to be used.
 * @param config Pointer to the configuration structure to be used.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetConfiguration( 
	fc2Context context,
	fc2Config* config );

/**
 * Retrieves information from the camera such as serial number, model
 * name and other camera information.
 *
 * @param context The fc2Context to be used.
 * @param pCameraInfo Pointer to the camera information structure
 *                    to be filled.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetCameraInfo( 
	fc2Context context,
	fc2CameraInfo* pCameraInfo );

/**
 * Retrieves information about the specified camera property. The 
 * property type must be specified in the fc2PropertyInfo structure 
 * passed into the function in order for the function to succeed.
 *
 * @param context The fc2Context to be used.
 * @param propInfo Pointer to the PropertyInfo structure to be filled.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetPropertyInfo( 
	fc2Context context,
	fc2PropertyInfo* propInfo );

/**
 * Reads the settings for the specified property from the camera. The 
 * property type must be specified in the fc2Property structure passed
 * into the function in order for the function to succeed. If auto
 * is on, the integer and abs values returned may not be consistent
 * with each other.
 *
 * @param context The fc2Context to be used.
 * @param prop Pointer to the Property structure to be filled.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetProperty( 
	fc2Context context,
	fc2Property* prop );

/**
 * Writes the settings for the specified property to the camera. The 
 * property type must be specified in the Property structure passed
 * into the function in order for the function to succeed.
 * The absControl flag controls whether the absolute or integer value
 * is written to the camera.
 *
 * @param context The fc2Context to be used.
 * @param prop Pointer to the Property structure to be used.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetProperty( 
	fc2Context context,
	fc2Property* prop );

/**
 * Writes the settings for the specified property to the camera. The 
 * property type must be specified in the Property structure passed
 * into the function in order for the function to succeed.
 * The absControl flag controls whether the absolute or integer value
 * is written to the camera.
 *
 * @param context The fc2Context to be used.
 * @param prop Pointer to the Property structure to be used.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetPropertyBroadcast( 
	fc2Context context,
	fc2Property* prop );

/**
 * Get the GPIO pin direction for the specified pin. This is not a
 * required call when using the trigger or strobe functions as
 * the pin direction is set automatically internally.
 *
 * @param context The fc2Context to be used.
 * @param pin Pin to get the direction for.
 * @param pDirection Direction of the pin. 0 for input, 1 for output.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetGPIOPinDirection( 
	fc2Context context,
	unsigned int pin, 
	unsigned int* pDirection );

/**
 * Set the GPIO pin direction for the specified pin. This is useful if
 * there is a need to set the pin into an input pin (i.e. to read the
 * voltage) off the pin without setting it as a trigger source. This 
 * is not a required call when using the trigger or strobe functions as
 * the pin direction is set automatically internally.
 *
 * @param context The fc2Context to be used.
 * @param pin Pin to get the direction for.
 * @param direction Direction of the pin. 0 for input, 1 for output.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetGPIOPinDirection( 
	fc2Context context,
	unsigned int pin, 
	unsigned int direction);

/**
 * Set the GPIO pin direction for the specified pin. This is useful if
 * there is a need to set the pin into an input pin (i.e. to read the
 * voltage) off the pin without setting it as a trigger source. This 
 * is not a required call when using the trigger or strobe functions as
 * the pin direction is set automatically internally.
 *
 * @param context The fc2Context to be used.
 * @param pin Pin to get the direction for.
 * @param direction Direction of the pin. 0 for input, 1 for output.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetGPIOPinDirectionBroadcast( 
	fc2Context context,
	unsigned int pin, 
	unsigned int direction);

/**
 * Retrieve trigger information from the camera.
 *
 * @param context The fc2Context to be used.
 * @param triggerModeInfo Structure to receive trigger information.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetTriggerModeInfo( 
	fc2Context context,
	fc2TriggerModeInfo* triggerModeInfo );

/**
 * Retrieve current trigger settings from the camera.
 *
 * @param context The fc2Context to be used.
 * @param triggerMode Structure to receive trigger mode settings.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetTriggerMode( 
	fc2Context context,
	fc2TriggerMode* triggerMode );

/**
 * Set the specified trigger settings to the camera.
 *
 * @param context The fc2Context to be used.
 * @param triggerMode Structure providing trigger mode settings.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetTriggerMode( 
	fc2Context context,
	fc2TriggerMode* triggerMode );

/**
 * Set the specified trigger settings to the camera.
 *
 * @param context The fc2Context to be used.
 * @param triggerMode Structure providing trigger mode settings.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetTriggerModeBroadcast( 
	fc2Context context,
	fc2TriggerMode* triggerMode );

/**
 * 
 *
 * @param context The fc2Context to be used.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2FireSoftwareTrigger(
	fc2Context context );

/**
 * Fire the software trigger according to the DCAM specifications.
 *
 * @param context The fc2Context to be used.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2FireSoftwareTriggerBroadcast(
	fc2Context context );

/**
 * Retrieve trigger delay information from the camera.
 *
 * @param context The fc2Context to be used.
 * @param triggerDelayInfo Structure to receive trigger delay information.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetTriggerDelayInfo( 
	fc2Context context,
	fc2TriggerDelayInfo* triggerDelayInfo );

/**
 * Retrieve current trigger delay settings from the camera.
 *
 * @param context The fc2Context to be used.
 * @param triggerDelay Structure to receive trigger delay settings.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetTriggerDelay( 
	fc2Context context,
	fc2TriggerDelay* triggerDelay );

/**
 * Set the specified trigger delay settings to the camera.
 *
 * @param context The fc2Context to be used.
 * @param triggerDelay Structure providing trigger delay settings.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetTriggerDelay( 
	fc2Context context,
	fc2TriggerDelay* triggerDelay );

/**
 * Set the specified trigger delay settings to the camera.
 *
 * @param context The fc2Context to be used.
 * @param triggerDelay Structure providing trigger delay settings.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetTriggerDelayBroadcast( 
	fc2Context context,
	fc2TriggerDelay* triggerDelay );

/**
 * Retrieve strobe information from the camera.
 *
 * @param context The fc2Context to be used.
 * @param strobeInfo Structure to receive strobe information.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetStrobeInfo( 
	fc2Context context,
	fc2StrobeInfo* strobeInfo );

/**
 * Retrieve current strobe settings from the camera. The strobe pin
 * must be specified in the structure before being passed in to
 * the function.
 *
 * @param context The fc2Context to be used.
 * @param strobeControl Structure to receive strobe settings.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetStrobe( 
	fc2Context context,
	fc2StrobeControl* strobeControl );

/**
 * Set current strobe settings to the camera. The strobe pin
 * must be specified in the structure before being passed in to
 * the function.
 *
 * @param context The fc2Context to be used.
 * @param strobeControl Structure providing strobe settings.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetStrobe( 
	fc2Context context,
	fc2StrobeControl* strobeControl );

/**
 * Set current strobe settings to the camera. The strobe pin
 * must be specified in the structure before being passed in to
 * the function.
 *
 * @param context The fc2Context to be used.
 * @param strobeControl Structure providing strobe settings.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetStrobeBroadcast( 
	fc2Context context,
	fc2StrobeControl* strobeControl );

/**
 * Query the camera to determine if the specified video mode and 
 * frame rate is supported.
 *
 * @param context The fc2Context to be used.
 * @param videoMode Video mode to check.
 * @param frameRate Frame rate to check.
 * @param pSupported Whether the video mode and frame rate is supported.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetVideoModeAndFrameRateInfo(
	fc2Context context,
	fc2VideoMode videoMode,
	fc2FrameRate frameRate,
	BOOL* pSupported);

/**
 * Get the current video mode and frame rate from the camera. If
 * the camera is in Format7, the video mode will be VIDEOMODE_FORMAT7
 * and the frame rate will be FRAMERATE_FORMAT7.
 *
 * @param context The fc2Context to be used.
 * @param videoMode Current video mode.
 * @param frameRate Current frame rate.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetVideoModeAndFrameRate(
	fc2Context context,
	fc2VideoMode* videoMode,
	fc2FrameRate* frameRate );

/**
 * Set the specified video mode and frame rate to the camera. It is
 * not possible to set the camera to VIDEOMODE_FORMAT7 or 
 * FRAMERATE_FORMAT7. Use the Format7 functions to set the camera
 * into Format7.
 *
 * @param context The fc2Context to be used.
 * @param videoMode Video mode to set to camera.
 * @param frameRate Frame rate to set to camera.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetVideoModeAndFrameRate(
	fc2Context context,
	fc2VideoMode videoMode,
	fc2FrameRate frameRate );

/**
 * Retrieve the availability of Format7 custom image mode and the
 * camera capabilities for the specified Format7 mode. The mode must
 * be specified in the Format7Info structure in order for the
 * function to succeed.
 *
 * @param context The fc2Context to be used.
 * @param info Structure to be filled with the capabilities of the specified
 *             mode and the current state in the specified mode.
 * @param pSupported Whether the specified mode is supported.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetFormat7Info( 
	fc2Context context,
	fc2Format7Info* info,
	BOOL* pSupported );

/**
 * Validates Format7ImageSettings structure and returns valid packet
 * size information if the image settings are valid. The current
 * image settings are cached while validation is taking place. The
 * cached settings are restored when validation is complete.
 *
 * @param context The fc2Context to be used.
 * @param imageSettings Structure containing the image settings.
 * @param settingsAreValid Whether the settings are valid.
 * @param packetInfo Packet size information that can be used to
 *                   determine a valid packet size.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2ValidateFormat7Settings( 
	fc2Context context,
	fc2Format7ImageSettings* imageSettings,
	BOOL* settingsAreValid,
	fc2Format7PacketInfo* packetInfo );

/**
 * Get the current Format7 configuration from the camera. This call
 * will only succeed if the camera is already in Format7.
 *
 * @param context The fc2Context to be used.
 * @param imageSettings Current image settings.
 * @param packetSize Current packet size.
 * @param percentage Current packet size as a percentage.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetFormat7Configuration( 
	fc2Context context,
	fc2Format7ImageSettings* imageSettings,
	unsigned int* packetSize,
	float* percentage );

/**
 * Set the current Format7 configuration to the camera.
 *
 * @param context The fc2Context to be used.
 * @param imageSettings Image settings to be written to the camera.
 * @param packetSize Packet size to be written to the camera.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetFormat7ConfigurationPacket( 
	fc2Context context,
	fc2Format7ImageSettings* imageSettings,
	unsigned int packetSize );

/**
* Set the current Format7 configuration to the camera.
*
* @param context The fc2Context to be used.
* @param imageSettings Image settings to be written to the camera.
* @param percentSpeed Packet size as a percentage to be written to the camera.
*
* @return A fc2Error indicating the success or failure of the function.
*/
FLYCAPTURE2_C_API fc2Error 
fc2SetFormat7Configuration(
	fc2Context context,
	fc2Format7ImageSettings* imageSettings,
	float percentSpeed );

/**
 * Write a GVCP register.
 *
 * @param context The fc2Context to be used.
 * @param address GVCP address to be written to.
 * @param value The value to be written.
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2WriteGVCPRegister( 
	fc2Context context,
	unsigned int address, 
	unsigned int value);     

/**
 * Write a GVCP register with broadcast
 *
 * @param context The fc2Context to be used.
 * @param address GVCP address to be written to.
 * @param value The value to be written.
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2WriteGVCPRegisterBroadcast( 
	fc2Context context,
	unsigned int address, 
	unsigned int value);     

/**
 * Read a GVCP register.
 *
 * @param context The fc2Context to be used.
 * @param address GVCP address to be read from.
 * @param pValue The value that is read.
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2ReadGVCPRegister( 
	fc2Context context,
	unsigned int address, 
	unsigned int* pValue );       

/**
 * Write a GVCP register block.
 *
 * @param context The fc2Context to be used.
 * @param address GVCP address to be write to.
 * @param pBuffer Array containing data to be written.
 * @param length Size of array, in quadlets.
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2WriteGVCPRegisterBlock(
	fc2Context context,
	unsigned int address,
	const unsigned int* pBuffer,
	unsigned int length );    

/**
 * Read a GVCP register block.
 *
 * @param context The fc2Context to be used.
 * @param address GVCP address to be read from.
 * @param pBuffer Array containing data to be written.
 * @param length Size of array, in quadlets.
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2ReadGVCPRegisterBlock(
	fc2Context context,
	unsigned int address,
	unsigned int* pBuffer,
	unsigned int length ); 

/**
 * Write a GVCP memory block.
 *
 * @param context The fc2Context to be used.
 * @param address GVCP address to be write to.
 * @param pBuffer Array containing data to be written.
 * @param length Size of array, in quadlets.
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2WriteGVCPMemory(
	fc2Context context,
	unsigned int address,
	const unsigned char* pBuffer,
	unsigned int length );    

/**
 * Read a GVCP memory block.
 *
 * @param context The fc2Context to be used.
 * @param address GVCP address to be read from.
 * @param pBuffer Array containing data to be written.
 * @param length Size of array, in quadlets.
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2ReadGVCPMemory(
	fc2Context context,
	unsigned int address,
	unsigned char* pBuffer,
	unsigned int length ); 

/**
 * Get the specified GigEProperty. The GigEPropertyType field must
 * be set in order for this function to succeed.
 *
 * @param context The fc2Context to be used.
 * @param pGigEProp The GigE property to get.
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetGigEProperty( 
		fc2Context context,
		fc2GigEProperty* pGigEProp );

/**
 * Set the specified GigEProperty. The GigEPropertyType field must
 * be set in order for this function to succeed.
 *
 * @param context The fc2Context to be used.
 * @param pGigEProp The GigE property to set.
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetGigEProperty( 
	fc2Context context,
	const fc2GigEProperty* pGigEProp );

FLYCAPTURE2_C_API fc2Error 
fc2QueryGigEImagingMode( 
	fc2Context context,
	fc2Mode mode, 
	BOOL* isSupported );

FLYCAPTURE2_C_API fc2Error 
fc2GetGigEImagingMode(
	fc2Context context,
	fc2Mode* mode );

FLYCAPTURE2_C_API fc2Error 
fc2SetGigEImagingMode( 
	fc2Context context,
	fc2Mode mode );        

FLYCAPTURE2_C_API fc2Error 
fc2GetGigEImageSettingsInfo( 
	fc2Context context,
	fc2GigEImageSettingsInfo* pInfo );

FLYCAPTURE2_C_API fc2Error 
fc2GetGigEImageSettings(
	fc2Context context,
	fc2GigEImageSettings* pImageSettings );

FLYCAPTURE2_C_API fc2Error 
fc2SetGigEImageSettings( 
	fc2Context context,
	const fc2GigEImageSettings* pImageSettings );

FLYCAPTURE2_C_API fc2Error 
fc2GetGigEConfig(
	fc2Context context,
	fc2GigEConfig* pConfig );

FLYCAPTURE2_C_API fc2Error 
fc2SetGigEConfig( 
	fc2Context context,
	const fc2GigEConfig* pConfig );

FLYCAPTURE2_C_API fc2Error 
fc2GetGigEImageBinningSettings( 
	fc2Context context, 
	unsigned int* horzBinnningValue, 
	unsigned int* vertBinnningValue );

FLYCAPTURE2_C_API fc2Error 
fc2SetGigEImageBinningSettings( 
	fc2Context context, 
	unsigned int horzBinnningValue, 
	unsigned int vertBinnningValue );

FLYCAPTURE2_C_API fc2Error 
fc2GetNumStreamChannels(
	fc2Context context,
	unsigned int* numChannels );

FLYCAPTURE2_C_API fc2Error 
fc2GetGigEStreamChannelInfo(
	fc2Context context,
	unsigned int channel, 
	fc2GigEStreamChannel* pChannel );

FLYCAPTURE2_C_API fc2Error 
fc2SetGigEStreamChannelInfo( 
	fc2Context context,
	unsigned int channel, 
	fc2GigEStreamChannel* pChannel );

/**
 * Query if LUT support is available on the camera. Note that some cameras 
 * may report support for the LUT and return an inputBitDepth of 0. In these 
 * cases use log2(numEntries) for the inputBitDepth.
 *
 * @param context The fc2Context to be used.
 * @param pData The LUT structure to be filled.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetLUTInfo(
	fc2Context context,
	fc2LUTData* pData );

/**
 * Query the read/write status of a single LUT bank.
 *
 * @param context The fc2Context to be used.
 * @param bank The bank to query.
 * @param pReadSupported Whether reading from the bank is supported.
 * @param pWriteSupported Whether writing to the bank is supported.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetLUTBankInfo(
	fc2Context context,
	unsigned int bank,
	BOOL* pReadSupported,
	BOOL* pWriteSupported );

/**
 * Get the LUT bank that is currently being used. For cameras with
 * PGR LUT, the active bank is always 0.
 *
 * @param context The fc2Context to be used.
 * @param pActiveBank The currently active bank.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetActiveLUTBank(
	fc2Context context,
	unsigned int* pActiveBank );

/**
 * Set the LUT bank that will be used.
 *
 * @param context The fc2Context to be used.
 * @param activeBank The bank to be set as active.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetActiveLUTBank(
	fc2Context context,
	unsigned int activeBank );

/**
 * Enable or disable LUT functionality on the camera.
 *
 * @param context The fc2Context to be used.
 * @param on Whether to enable or disable LUT.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2EnableLUT(
	fc2Context context,
	BOOL on );

/**
 * Get the LUT channel settings from the camera.
 *
 * @param context The fc2Context to be used.
 * @param bank Bank to retrieve.
 * @param channel Channel to retrieve.
 * @param sizeEntries Number of entries in LUT table to read.
 * @param pEntries Array to store LUT entries.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetLUTChannel(
	fc2Context context,
	unsigned int bank, 
	unsigned int channel,
	unsigned int sizeEntries,
	unsigned int* pEntries );

/**
 * Set the LUT channel settings to the camera.
 *
 * @param context The fc2Context to be used.
 * @param bank Bank to set.
 * @param channel Channel to set.
 * @param sizeEntries Number of entries in LUT table to write. This must be the 
 *					  same size as numEntries returned by GetLutInfo().
 * @param pEntries Array containing LUT entries to write.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetLUTChannel(
	fc2Context context,
	unsigned int bank, 
	unsigned int channel,
	unsigned int sizeEntries,
	unsigned int* pEntries );

/**
 * Retrieve the current memory channel from the camera.
 *
 * @param context The fc2Context to be used.
 * @param pCurrentChannel Current memory channel.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetMemoryChannel( 
	fc2Context context,
	unsigned int* pCurrentChannel );

/**
 * Save the current settings to the specfied current memory channel.
 *
 * @param context The fc2Context to be used.
 * @param channel Memory channel to save to.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SaveToMemoryChannel( 
	fc2Context context,
	unsigned int channel );

/**
 * Restore the specfied current memory channel.
 *
 * @param context The fc2Context to be used.
 * @param channel Memory channel to restore from.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2RestoreFromMemoryChannel( 
	fc2Context context,
	unsigned int channel );

/**
 * Query the camera for memory channel support. If the number of 
 * channels is 0, then memory channel support is not available.
 *
 * @param context The fc2Context to be used.
 * @param pNumChannels Number of memory channels supported.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetMemoryChannelInfo( 
	fc2Context context,
	unsigned int* pNumChannels );

/**
 * Get the current status of the embedded image information register,
 * as well as the availability of each embedded property.
 *
 * @param context The fc2Context to be used.
 * @param pInfo Structure to be filled.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetEmbeddedImageInfo( 
	fc2Context context,
	fc2EmbeddedImageInfo* pInfo );

/**
 * Sets the on/off values of the embedded image information structure
 * to the camera.
 *
 * @param context The fc2Context to be used.
 * @param pInfo Structure to be used.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetEmbeddedImageInfo( 
	fc2Context context,
	fc2EmbeddedImageInfo* pInfo );

/**
 * Returns a text representation of the register value.
 *
 * @param registerVal The register value to query.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API const char* 
fc2GetRegisterString( 
	unsigned int registerVal);

/**
 * Create a fc2Image. If externally allocated memory is to be used for the 
 * converted image, simply assigning the pData member of the fc2Image structure 
 * is insufficient. fc2SetImageData() should be called in order to populate
 * the fc2Image structure correctly.
 *
 * @param pImage Pointer to image to be created.
 *
 * @see fc2SetImageData()
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2CreateImage( 
	fc2Image* pImage );

/**
 * Destroy the fc2Image.
 *
 * @param image Pointer to image to be destroyed.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2DestroyImage( 
	fc2Image* image );

/**
 * Set the default color processing algorithm.  This method will be 
 * used for any image with the DEFAULT algorithm set. The method used 
 * is determined at the time of the Convert() call, therefore the most 
 * recent execution of this function will take precedence. The default 
 * setting is shared within the current process.
 *
 * @param defaultMethod The color processing algorithm to set.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetDefaultColorProcessing( 
	fc2ColorProcessingAlgorithm defaultMethod );

/**
 * Get the default color processing algorithm.
 *
 * @param pDefaultMethod The default color processing algorithm.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetDefaultColorProcessing( 
	fc2ColorProcessingAlgorithm* pDefaultMethod );

/**
 * Set the default output pixel format. This format will be used for any 
 * call to Convert() that does not specify an output format. The format 
 * used will be determined at the time of the Convert() call, therefore 
 * the most recent execution of this function will take precedence. 
 * The default is shared within the current process.
 *
 * @param format The output pixel format to set.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetDefaultOutputFormat( 
	fc2PixelFormat format );

/**
 * Get the default output pixel format.
 *
 * @param pFormat The default pixel format.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetDefaultOutputFormat( 
	fc2PixelFormat* pFormat );

/**
 * Calculate the bits per pixel for the specified pixel format.
 *
 * @param format The pixel format.
 * @param pBitsPerPixel The bits per pixel.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2DetermineBitsPerPixel( 
	fc2PixelFormat format,
	unsigned int* pBitsPerPixel );

/**
 * Save the image to the specified file name with the file format
 * specified.
 *
 * @param pImage The fc2Image to be used.
 * @param pFilename Filename to save image with.
 * @param format File format to save in.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SaveImage(
	fc2Image* pImage,
	const char* pFilename,
	fc2ImageFileFormat format );

/**
 * Save the image to the specified file name with the file format
 * specified.
 *
 * @param pImage The fc2Image to be used.
 * @param pFilename Filename to save image with.
 * @param format File format to save in.
 * @param pOption Options for saving image.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SaveImageWithOption( 
	fc2Image* pImage, 
	const char* pFilename, 
	fc2ImageFileFormat format, 
	void* pOption );

/**
 * 
 *
 * @param pImageIn
 * @param pImageOut
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2ConvertImage(
	fc2Image* pImageIn,
	fc2Image* pImageOut );

/**
 * Converts the current image buffer to the specified output format and
 * stores the result in the specified image. The destination image 
 * does not need to be configured in any way before the call is made.
 *
 * @param format Output format of the converted image.
 * @param pImageIn Input image.
 * @param pImageOut Output image.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2ConvertImageTo(
	fc2PixelFormat format,
	fc2Image* pImageIn,
	fc2Image* pImageOut );

/**
 * Get a pointer to the data associated with the image. This function
 * is considered unsafe. The pointer returned could be invalidated if
 * the buffer is resized or released. The pointer may also be
 * invalidated if the Image object is passed to fc2RetrieveBuffer().
 *
 * @param pImage The fc2Image to be used.
 * @param ppData A pointer to the image data.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetImageData(
	fc2Image* pImage,
	unsigned char** ppData);

/**
 * Set the data of the Image object.
 * Ownership of the image buffer is not transferred to the Image object.
 * It is the user's responsibility to delete the buffer when it is
 * no longer in use.
 *
 * @param pImage The fc2Image to be used.
 * @param pData Pointer to the image buffer.
 * @param dataSize Size of the image buffer.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetImageData( 
	fc2Image* pImage,
	const unsigned char* pData,
	unsigned int dataSize);

/**
 * Sets the dimensions of the image object.
 *
 * @param pImage The fc2Image to be used.
 * @param rows Number of rows to set.
 * @param cols Number of cols to set.
 * @param stride Stride to set.
 * @param pixelFormat Pixel format to set.
 * @param bayerFormat Bayer tile format to set.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2SetImageDimensions( 
	fc2Image* pImage,
	unsigned int rows,
	unsigned int cols,
	unsigned int stride,
	fc2PixelFormat pixelFormat,
	fc2BayerTileFormat bayerFormat);

/**
 * Get the timestamp data associated with the image. 
 *
 * @param pImage The fc2Image to be used.
 *
 * @return Timestamp data associated with the image.
 */
FLYCAPTURE2_C_API fc2TimeStamp 
fc2GetImageTimeStamp( 
	fc2Image* pImage);

/**
 * Calculate statistics associated with the image. In order to collect
 * statistics for a particular channel, the enabled flag for the
 * channel must be set to true. Statistics can only be collected for
 * images in Mono8, Mono16, RGB, RGBU, BGR and BGRU.
 *
 * @param pImage The fc2Image to be used.
 * @param pImageStatisticsContext The fc2ImageStatisticsContext to hold the 
 *                                statistics.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2CalculateImageStatistics(
	fc2Image* pImage,
	fc2ImageStatisticsContext* pImageStatisticsContext );

/**
 * Create a statistics context.
 *
 * @param pImageStatisticsContext A statistics context.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2CreateImageStatistics(
	fc2ImageStatisticsContext* pImageStatisticsContext );

/**
 * Destroy a statistics context.
 *
 * @param imageStatisticsContext A statistics context.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2DestroyImageStatistics(
	fc2ImageStatisticsContext imageStatisticsContext );

/**
 * Get the status of a statistics channel.
 *
 * @param imageStatisticsContext A statistics context.
 * @param channel The statistics channel.
 * @param pEnabled Whether the channel is enabled.
 *
 * @see SetChannelStatus()
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API const fc2Error
fc2GetChannelStatus(
	fc2ImageStatisticsContext imageStatisticsContext,
	fc2StatisticsChannel channel,
	BOOL* pEnabled );

/**
 * Set the status of a statistics channel.
 *
 * @param imageStatisticsContext A statistics context.
 * @param channel The statistics channel.
 * @param enabled Whether the channel should be enabled.
 *
 * @see GetChannelStatus()
 *
 * @return An Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error
fc2SetChannelStatus( 
	fc2ImageStatisticsContext imageStatisticsContext,
	fc2StatisticsChannel channel,
	BOOL enabled );

/**
 * Get all statistics for the image.
 *
 * @param imageStatisticsContext The statistics context.
 * @param channel The statistics channel.
 * @param pRangeMin The minimum possible value.
 * @param pRangeMax The maximum possible value.
 * @param pPixelValueMin The minimum pixel value.
 * @param pPixelValueMax The maximum pixel value.
 * @param pNumPixelValues The number of unique pixel values.
 * @param pPixelValueMean The mean of the image.
 * @param ppHistogram Pointer to an array containing the histogram.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetImageStatistics(
	fc2ImageStatisticsContext imageStatisticsContext,
	fc2StatisticsChannel channel,
	unsigned int* pRangeMin,
	unsigned int* pRangeMax,
	unsigned int* pPixelValueMin,
	unsigned int* pPixelValueMax,
	unsigned int* pNumPixelValues,
	float* pPixelValueMean,
	int** ppHistogram );

/**
 * Create a AVI context.
 *
 * @param pAVIContext A AVI context.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2CreateAVI(
	fc2AVIContext* pAVIContext );

/**
 * Open an AVI file in preparation for writing Images to disk.
 * The size of AVI files is limited to 2GB. The filenames are
 * automatically generated using the filename specified.
 *
 * @param AVIContext The AVI context to use.
 * @param pFileName The filename of the AVI file.
 * @param pOption Options to apply to the AVI file.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2AVIOpen( 
	fc2AVIContext AVIContext,
	const char*  pFileName, 
	fc2AVIOption* pOption );

/**
 * Open an MJPEG file in preparation for writing Images to disk.
 * The size of AVI files is limited to 2GB. The filenames are
 * automatically generated using the filename specified.
 *
 * @param AVIContext The AVI context to use.
 * @param pFileName The filename of the AVI file.
 * @param pOption Options to apply to the AVI file.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2MJPGOpen( 
	fc2AVIContext AVIContext,
	const char*  pFileName, 
	fc2MJPGOption* pOption );

/**
 * Open an H.264 file in preparation for writing Images to disk.
 * The size of AVI files is limited to 2GB. The filenames are
 * automatically generated using the filename specified.
 *
 * @param AVIContext The AVI context to use.
 * @param pFileName The filename of the AVI file.
 * @param pOption Options to apply to the AVI file.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2H264Open( 
	fc2AVIContext AVIContext,
	const char*  pFileName, 
	fc2H264Option* pOption );

/**
 * Append an image to the AVI file.
 *
 * @param AVIContext The AVI context to use.
 * @param pImage The image to append.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2AVIAppend( 
	fc2AVIContext AVIContext,
	fc2Image* pImage );

/**
 * Close the AVI file.
 *
 * @param AVIContext The AVI context to use.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2AVIClose(
	fc2AVIContext AVIContext );

/**
 * Destroy a AVI context.
 *
 * @param AVIContext A AVI context.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2DestroyAVI(
	fc2AVIContext AVIContext );

/**
 * Get system information.
 *
 * @param pSystemInfo Structure to receive system information.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetSystemInfo( fc2SystemInfo* pSystemInfo);

/**
 * Get library version.
 *
 * @param pVersion Structure to receive the library version.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetLibraryVersion( fc2Version* pVersion);

/**
 * Launch a URL in the system default browser.
 *
 * @param pAddress URL to open in browser.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2LaunchBrowser( const char* pAddress);

/**
 * Open a CHM file in the system default CHM viewer.
 *
 * @param pFileName Filename of CHM file to open.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2LaunchHelp( const char* pFileName);

/**
 * Execute a command in the terminal. This is a blocking call that 
 * will return when the command completes.
 *
 * @param pCommand Command to execute.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2LaunchCommand( const char* pCommand);

/**
 * Execute a command in the terminal. This is a non-blocking call that 
 * will return immediately. The return value of the command can be
 * retrieved in the callback.
 *
 * @param pCommand Command to execute.
 * @param pCallback Callback to fire when command is complete.
 * @param pUserData Data pointer to pass to callback.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2LaunchCommandAsync( 
	const char* pCommand,
	fc2AsyncCommandCallback pCallback,
	void* pUserData );

/**
 * Get a string representation of an error.
 *
 * @param error Error to be parsed.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API const char* 
fc2ErrorToDescription( 
	fc2Error error);

/**
 * Get cycle time from camera
 *
 * @param Timestamp struct.
 *
 * @return A fc2Error indicating the success or failure of the function.
 */
FLYCAPTURE2_C_API fc2Error 
fc2GetCycleTime( fc2Context context, fc2TimeStamp* pTimeStamp );

#ifdef __cplusplus
};
#endif

#endif // PGR_FC2_FLYCAPTURE2_C_H
```

# 关于typedef void*
看到这种定义，没接触过，很陌生。Examples里面出现了蛮多次的，比如fc2Context就是void*。

source:<http://stackoverflow.com/questions/9412262/why-is-typedef-void-complex-used-in-this-interface>

摘要一个答案的重点：
>A pointer type normally indicates what kind of data the pointer points to. void * is an exception: it's a way to have a pointer without saying what the type of the value it points to is. You can freely assign any kind of pointer to a void * pointer and back.

void * pointers are normally used in generic library functions that must work with data of any type. 

所以fc2Context的理解应该是关于SDK上下文的一个任意类型的数据信息？

以下是该答案全文：
>
typedef defines a name for a type. So
typedef void *COMPLEX;
COMPLEX z;
is equivalent to
void * z;
A pointer type normally indicates what kind of data the pointer points to. void * is an exception: it's a way to have a pointer without saying what the type of the value it points to is. You can freely assign any kind of pointer to a void * pointer and back.
void * pointers are normally used in generic library functions that must work with data of any type. For example, consider the standard library function memcpy:
void *memcpy(void *dest, const void *src, size_t n);
You pass that function a pointer to an object of any type src, a pointer to another object (which is usually, but not always, of the same type) dest, and a number of bytes to copy. The function copies the bytes, it doesn't care what the bytes mean, so it's enough to pass two pointers-to-an-unspecified-type.
The use of void * here is not good or common programming practice. A complex number is represented as its real part and its imaginary part:
struct COMPLEX_IMPL { double a; double b; };
A typical complex number library would make this the COMPLEX type.
The code you posted hides the implementation of the COMPLEX type. The fact that complex numbers are implemented as a structure containing two double members is only apparent in complex.c. Users of the library only see that a COMPLEX is a pointer to something. This is a form of data abstraction: hiding the representation details of a data type. But it's poorly done: with this definition, any pointer to anything can be assigned to a COMPLEX. The normal way is to use an incomplete structure, which is declared and visibly a structure but whose members are not specified. In complex.h, you would write:
struct COMPLEX_IMPL;
typedef struct COMPLEX_IMPL *COMPLEX;
That way, the only way to legally create a COMPLEX_IMPL is through the functions provided by complex.h, but a variable of type COMPLEX is visibly a pointer to a representation of a complexe number as defined in complex.c.
Oh, and this is an ordinary variable name.