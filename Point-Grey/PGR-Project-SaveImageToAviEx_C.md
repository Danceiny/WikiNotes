---
date: 2016-03-04 14:58
status: public
title: PGR-Project-SaveImageToAviEx_C
---

# SavaImageToAviEx_C.c
```c:n
//=============================================================================
// Copyright ?2011 Point Grey Research, Inc. All Rights Reserved.
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

#if defined(WIN32) || defined(WIN64)
#define _CRT_SECURE_NO_WARNINGS		
#endif

#include "C/FlyCapture2_C.h"
#include <stdio.h>

typedef enum _AviType
{
    UNCOMPRESSED,
    MJPG,
    H264
} AviType;

void PrintBuildInfo()
{
    fc2Version version;
    char versionStr[512];
    char timeStamp[512];

    fc2GetLibraryVersion( &version );

    sprintf( 
        versionStr, 
        "FlyCapture2 library version: %d.%d.%d.%d\n", 
        version.major, version.minor, version.type, version.build );

    printf( "%s", versionStr );

    sprintf( timeStamp, "Application build date: %s %s\n\n", __DATE__, __TIME__ );

    printf( "%s", timeStamp );
}

void PrintCameraInfo( fc2Context context )
{
    fc2Error error;
    fc2CameraInfo camInfo;
    error = fc2GetCameraInfo( context, &camInfo );
    if ( error != FC2_ERROR_OK )
    {
        // Error
    }

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

int SaveAVIHelper(fc2Context context, AviType aviType, float frameRate)
{
    fc2Error error;
    const int k_numImagesToGrab = 100;    
    fc2Image rawImage;
    fc2AVIContext aviContext;
    fc2AVIOption aviOption;
    fc2H264Option h264Option;
    fc2MJPGOption mjpgOption;
    int i;

    error = fc2CreateAVI(&aviContext);
    if (error != FC2_ERROR_OK)
    {
        printf("Error in fc2CreateAVI: %d\n", error);
        return -1;
    }

    error = fc2CreateImage( &rawImage );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2CreateImage: %d\n", error );           
        fc2DestroyAVI(aviContext);
        return -1;
    }

    for (i=0; i < k_numImagesToGrab; i++)
    {
        // Retrieve the image
        error = fc2RetrieveBuffer(context, &rawImage);
        if (error != FC2_ERROR_OK)
        {
            printf("Error in retrieveBuffer: %d\n", error);
            continue;
        }

        // Perform some initialization for the first time
        if (i == 0)
        {
            switch (aviType)
            {
            case UNCOMPRESSED:
                aviOption.frameRate = frameRate;
                error = fc2AVIOpen(aviContext, "SaveImageToAviEx_C-Uncompressed", &aviOption);
                if (error != FC2_ERROR_OK)
                {
                    printf("Error opening AVI: %d\n", error);
                }
                break;

            case MJPG:
                mjpgOption.frameRate = frameRate;
                mjpgOption.quality = 75;
                error = fc2MJPGOpen(aviContext, "SaveImageToAviEx_C-MJPG", &mjpgOption);
                if (error != FC2_ERROR_OK)
                {
                    printf("Error opening AVI: %d\n", error);
                }
                break;

            case H264:     
                h264Option.frameRate = frameRate;
                h264Option.bitrate = 1000000;
                h264Option.width = rawImage.cols;
                h264Option.height = rawImage.rows;
                error = fc2H264Open(aviContext, "SaveImageToAviEx_C-H264", &h264Option);
                if (error != FC2_ERROR_OK)
                {
                    printf("Error opening AVI: %d\n", error);
                }
                break;            
            }
        }
        // 将抓取的原始帧图像添加到AVI上下文中
        error = fc2AVIAppend(aviContext, &rawImage);
        if (error != FC2_ERROR_OK)
        {
            printf("Error appending to AVI: %d\n", error);
        }
             
        printf("Appended image %d\n", i);        
    }  /** 结束for循环图像抓取 **/

    error = fc2DestroyImage(&rawImage);
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2CreateImaged: %d\n", error );           
        fc2DestroyAVI(aviContext);
        return -1;
    }

    error = fc2DestroyAVI(aviContext);
    if (error != FC2_ERROR_OK)
    {
        printf("Error in fc2DestroyAVI: %d\n", error);
        return -1;
    }

    return 0;
}

float GetFrameRate(fc2Context context)
{
    fc2Error error;
    fc2PropertyInfo propInfo;
    fc2Property prop;

    // Check if the camera supports the FRAME_RATE property
    printf( "Detecting frame rate from camera... \n" );
    propInfo.type = FC2_FRAME_RATE;
    error = fc2GetPropertyInfo(context, &propInfo);
    if (error != FC2_ERROR_OK)
    {
        return 0.0f;
    }
    
    if (propInfo.present)
    {
        // Get the frame rate
        prop.type = FC2_FRAME_RATE;
        error = fc2GetProperty(context, &prop);
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
        if (error != FC2_ERROR_OK)
        {
            return 0.0f;
        }

        // Set the frame rate.
        /*set value of prop, and error = fc2SetProperty(context,&prop)*/
        // Note that the actual recording frame rate may be slower,
        // depending on the bus speed and disk writing speed.
        return prop.absValue;        
    }

    return 0.0f;
}
// propertyinfo在前，property在后。(读写操作）

int RunCamera(fc2Context context, fc2PGRGuid guid)
{
    fc2Error error;
    float frameRate = 0.0f;

    error = fc2Connect( context, &guid );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2Connect: %d\n", error );
        return -1;
    }

    PrintCameraInfo( context );    

    error = fc2StartCapture( context );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2StartCapture: %d\n", error );
        return -1;
    }

    frameRate = GetFrameRate(context);
    if (frameRate == 0.0f)
    {
        printf("Invalid frame rate returned\n");
        return -1;
    }
    //对各种缓存的销毁都在SaveAVIHelper内部进行
    SaveAVIHelper(context, UNCOMPRESSED, frameRate);
    SaveAVIHelper(context, H264, frameRate);
    SaveAVIHelper(context, MJPG, frameRate);

    error = fc2StopCapture( context );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2StopCapture: %d\n", error );
        return -1;
    }

    return 0;
}

int main(int argc, char** argv)
{        
    fc2Error error;
    fc2Context context;
    fc2PGRGuid guid;
    unsigned int numCameras = 0;      

    PrintBuildInfo();

    error = fc2CreateContext( &context );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2CreateContext: %d\n", error );
        return 0;
    }        

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
        return -1;
    }

    // Get the 0th camera
    error = fc2GetCameraFromIndex( context, 0, &guid );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2GetCameraFromIndex: %d\n", error );
        return -1;
    }    

    if (RunCamera(context, guid) != 0)
    {
        printf("Error running camera\n");
        return -1;
    }

    error = fc2DestroyContext( context );
    if ( error != FC2_ERROR_OK )
    {
        printf( "Error in fc2DestroyContext: %d\n", error );
        return -1;
    }

    printf( "Done! Press Enter to exit...\n" );
    getchar();

    return 0;
}
```
