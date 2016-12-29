---
date: 2016-02-18 02:06
status: public
title: FlyCapture2-API-Reference
---

# Chapter1	Module Index
1. Global constants
2. Enumerations
3. GigE specific Enumerations
4. Structures
	GigE specific structures
	IIDC specific structures
	Image saving structures

# Chapter2	Namespace Index
1. FlyCap3CameraControl
2. FlyCapture2
3. MultiSyncLibrary


# Chapter3	Class Index
## Class Hierarchy 类等级
This inheritance list is sorted roughly, but not completely, alphabetically:
1. AVIOption
2. AVIRecorder
3. BMPOption
4. BusManager
5. CameraBase
	Camera
	GCCamera
	GigECamera
6. CameraControlDlg
7. CameraInfo
8. CameraSelectionDlg
9. CameraStats
10. ConfigROM
11. EmbeddedImageInfo
12. EmbeddedImageInfoProperty
13. Error
14. FC2Config
15. FC2Version
16. FlyCapture3ApiGuiWrapper
17. Format7ImageSettings
18. Format7Info
19. Format7PacketInfo
20. GigEConfig
21. GigEImageSettings
22. GigEImageSettingsInfo
23. GigEProperty
24. GigEStreamChannel
25. H264Option
26. Image
27. ImageMetadata
28. ImageStatistics
29. Internal
30. IPAddress
31. JPEGOption
32. JPG2Option
33. LUTData
34. MACAddress
35. MJPGOption
36. NodeMap
37. PGMOption
38. PGRGuid
39. PNGOption
40. PPMOption
41. Property
42. PropertyInfo
43. StrobeControl
44. StrobeInfo
45. SyncManager
46. SystemInfo
47. TIFFOption
48. TimeStamp
49. TopologyNode
50. TriggerMode
51. TriggerModeInfo
52. Utilities


# Chapter4	Class Index
## Class List
Here are the classes, structs, unions and interfaces with brief descriptions:
1. AVIOption
>Options for saving AVI files

2. AVIRecorder
>Functionality for the user to record images to an AVI file

3. BMPOption
>Options for saving Bitmap image

4. BusManager
>Functionality for the user to get an PGRGuid for desired camera or device easily

5. Camera
>The Camera object represents a physical camera that uses the IIDC register set

6. CameraBase
>Abstract base class that defines a general interface to a camera

7. CameraControlDlg
>The CameraControlDlg object represents a dialog that provides a graphical interface to a specified camera

8. CameraInfo
>Camera information

9. CameraSelectionDlg
>The CameraSelectionDlg object represents a dialog that provides a graphical interface that lists the number of camera available to the library.

10. CameraStats
>Camera diagnotic information

11. ConfigROM
>Camera configuration ROM

12. EmbeddedImageInfo
>Properties of the possible embedded image information

13. EmbeddedImageInfoProperty
>Properties of  a single embedded image info property

14. Error
>The Error object represents an error that is returned from the library

15. FC2Config
>Configuration for a camera

16. FC2Version
>The current version of the library

17. FlyCapture3ApiGuiWrapper

18. Format7ImageSettings
>Format 7 image settings

19. Format7Info
>Format 7 information for a single mode

20. Format7PacketInfo
>Format 7 packet information

21. GCCamera

22. GigECamera
>The GigECamera object represents a physical Gigabit Ethernet camera

23. GigEConfig
>Configuration for a GigE camera

24. GigEImageSettings
>Image settings for a GigE camera

25. GigEImageSettingsInfo
>Format 7 information for a single mode

26. GigEProperty
>A GigE property

27. GigEStreamChannel
>Information about a single GigE stream channel

28. H264Option
>Options for saving H264 files

29. Image
>Used to retrieve images from a camera, convert between multiple pixel formats and save images to disk

30. ImageMetadata
>Metadata related to an image

31. ImageStatistics
>The ImageStatistics object represents image statistics for an image

32. Internal
33. IPAddress
>IPv4 address

34. JPEGOption
>Options for saving JPEG image

35. LUTData
>Information about the camera's look up table

36. MACAddress
>MAC address

37.  MJPGOption
>Options for saving MJPG files

38. NodeMap
39. PGMOption
>Options for saving PGM images

40. PGRGuid
>A GUID to the camera

41. PNGOption
>Options for saving PNG images

42. PPMOption
>Options for saving PPM images

43. Property
>A specific camera property

44. PropertyInfo
>Information about a specific camera property

45. StrobeControl
>A camera strobe

46. StrobeInfo
>A camera strobe property

47. SyncManager
48. SystemInfo
>descriptions of the system

49. TIFFOption
>Options for saving TIFF images

50. TimeStamp
>TimeStamp information

51. TopologyNode
>Topology information that can be used to generate a tree structure of all cameras and devices connected to a computer

52. TriggerMode
>A camera trigger

53. TriggerModeInfo
>Information about a camera trigger property

54. Utilities
>The Utility class is generally used to query for general system ifnormation such as operating system, available memory etc


# Chapter5	File Index
## File List
Here is a list of all files with brief descriptions
1. AVIRecorder.h
2. BusManager.h
3. Camera.h
4. CameraBase.h
5. Error.h
6. FlyCapture2.h
7. FlyCapture2Defs.h
8. FlyCaptureGUI.h
9. FlyCapture2Platform.h
10. NodeMap.h
11. TopologyNode.h
12. Utilities.h


# Chapter6	Module Documentation
## Global constants
***Variables***
* static const unsigned int sk_maxStringLength = 512
>*The maximum length that is allocated for a string.*

* static const unsigned int sk_maxNumPorts = 32
>*The maximum number of ports one device can have.*

### Variable Documentation
> const unsigned int sk_maxNumPorts = 32 [static]
const unsigned int sk_maxStringLength = 512 [static]

## Enumerations
* enum ErrorType
>*The error types returned by functions.*

* enum BusCallbackType
>*The type of bus callback to register a callback function for.*

* enum GrabMode
>*The grab strategy employed during image transfer.*

* enum GrabTimeout
>*Timeout options for grabbing images.*

* enum BandwidthAllocation
>*Bandwidth allocation options for 1394 devices.*

* enum InterfaceType
>*Interfaces that a camera may use to communicate with a host.*

* enum PropertyType
>*Camera Properties.*

* enum FrameRate
>*Frame rates in frames per second.*

* enum VideoMode
>*DCAM video modes.*

* enum Mode
>*Camera modes for DCAM formats as well as Format7.*

* enum PixelFormat
>*Pixel formats available for Format7 modes.*

* enum BusSpeed
>*Bus speeds.*

* enum PCIeBusSpeed

* enum DriverType
>*Types of low level drivers that flycapture uses.*

* enum ColorProcessingAlgorithm
>*Color processing algorithms.*

* enum BayerTileFormat
>*Bayer tile formats.*

* enum ImageFileFormat
>*File formats to be used for saving images to disk.*

### Enumeration Type Documentation
Detailed Description
各枚举量详解。略。

## Structures
Collaboration diagram for Structures:
![](~/02-07-05.jpg)
## Classes
## Modules
* GigE specific structures
>*These structures are specific to GigE camera operation only.*

* IIDC specific structures
>*These structures are specific to IIDC camera operation only.*

* Image saving structures
>*These structures define various parameters used for saving images.*

## Typedefs
* typedef PropertyInfo TriggerDelayInfo
>*The TriggerDelayInfo structure is identical to PropertyInfo.*

* typedef Property TriggerDelay
>*The TriggerDelay structure is identical to Property.*

- GigE specific structures
- IIDC specific structures
- Image saving structures

# Chapter7	Namespace Documentation
## FlyCap3CameraControl Namespace Reference
Classes
* class FlyCapture3ApiGuiWrapper
## FlyCapture2 Namespace Reference
Classes
* class AVIRecorder
>*The AVIRecorder class provides the functionality for the record images to an AVI file.*

* class BusManager
>*The BusManager class provides the functionality for the user to get an PGRGuid for a desired camera or device easily.*

* class Camera
>*The Camera object represents a physical camera that uses the IIDC register set.*

* class CameraBase
>*The CameraBase class is an abstract base class that defines a general interface to a camera.*

* class Error
>*The Error object represents an error that is returned from the library.*

* struct FC2Version
>*The current version of the library.*

* class PGRGuid
>*A GUID to the camera.*

* struct IPAddress
>*IPv4 address.*

* struct MACAddress
>*MAC address.*

* struct GigEProperty
>*A GigE property.*

* struct GigEStreamChannel
>*Information about a single GigE stream channel.*

* struct GigEConfig
>*Configuration for a GigE camera.*

* struct GigEImageSettingsInfo
>*Format 7 information for a single mode.*

* struct GigEImageSettings
>*Image settings for a GigE camera.*

* struct Format7Info
>*Format 7 information for a single mode.*

* struct Format7PacketInfo
>*Format 7 packet information.*

* struct FC2Config
>*Configuration for a camera.*

* struct PropertyInfo
>*Information about a specific camera property.*

* struct Property
>*A specific camera property.*

* struct TriggerModeInfo
>*Information about a camera trigger property.*

* struct TriggerMode
>*A camera trigger.*

* struct StrobeInfo
>*A camera strobe property.*

* struct StrobeControl
>*A camera strobe.*

* struct TimeStamp
>*Timestamp information.*

* struct ConfigROM
>*Camera configuration ROM.*

* struct CameraInfo
>*Camera information.*

* struct EmbeddedImageInfoProperty
>*Properties of a single embedded image info property.*

* struct EmbeddedImageInfo
>*Properties of the possible embedded image information.*

* struct ImageMetadata
>*Metadata related to an image.*

* struct LUTData
>*Information about the camera's look up table.*

* struct CameraStats
>*Camera diagnotic information.*

* struct PNGOption
>*Options for saving PNG images.*

* struct PPMOption
>*Options for saving PPM images.*

* struct PGMOption
>*Options for saving PGM images.*

* struct TIFFOption
>*Options for saving TIFF images.*

* struct JPEGOption
>*Options for saving JPEG image.*

* struct JPG2Option
>*Options for saving JPEG2000 image.*

* struct BMPOption
>*Options for saving BItmap image.*

* struct MJPGOption
>*Options for saving MJPG files.*

* struct H264Option
>*Options for saving H264 files.*

* struct AVIOption
>*Options for saving AVI files.*

* class CameraControlDlg
>*The CameraControlDlg object represents a dialog that provides a graphical interface to a specified camera.*

* class CameraSelectionDlg
>*The CameraSelectionDlg object represents a dialog that provides a graphical interface that lists the number of cameras available to the library.*

* class GCCamera

* class GigECamera
>*The GigECamera object represents a physical Gigabit Ethernet camera.*

* class Image
>*The image class is used to retrive images from a camera, convert between multiple pixel formats and save images to disk.*

* class ImageStatistics
>*The ImageStatistics object represents image statistics for an image.*

* class Internal

* class NodeMap

* class TopologyNode
>*The TopologyNode class contains topology information that can be used to generate a tree structure of all cameras and devieces connected to a computer.*

* struct SystemInfo
>*Description of the system.*

* class Utilities
>*The Utility class is generally used to query for general system information such as operating system, available memory etc.*

### Typedefs
1. `typedef void ( *BusEventCallback)(void *  pParameter, unsigned int serial-Number)`
>*Bus event callback function prototype.*
> Defines the syntax of the callback function that is passed into RegisterCallback() and UnregisterCallback(). It is recommended that minimal handing be performed in this callback as it will block internal processing of bus events until it returns.

2. `typedef void * CallbackHandle`
>*Handle that is returned when registering a callback.*
> It is required when unregistering the callback.

3. `typedef void (* ImageEventCallback)(class Image *pImage, const void *pCallbackData)`
>*Image event callback function prototype.*
> Defines the syntax of the image callback function that is passed into StartCapture(). It is possible for this function to be called simultaneously. Therefore, users must make sure that code in the callback is thread safe.

4. `typedef PropertyInfo TriggerDelayInfo`

>*The TriggerDelayInfo structure is identical to PropertyInfo.*

5. `typedef Property TriggerDelay`
>*The TriggerDelay structure is identical to Property.*

6. `typedef void (* AsyncCommandCallback)(class Error retError, void *pUserData)`
>*Async command callback function prototype.*
> Defines the syntax of the async command function that is passed into LauchCommandAsync().


### Enumerations Type Documentation
- enum ByteOrder
- enum OSType

## MultiSyncLibrary Namespace Reference
### Classes
- class SyncManager

### Enumerations Type Documentation
- enum PGRSyncError
- enum PGRSyncMessage

# Chapter8	Class Documentation
## 8.1 AVIOption Struct Reference
**Public Member Functions**

- AVIOption()

**Public Attributes**

- float frameRate
>Frame rate of the stream.

- unsigned int reserved [256]
>Reserved for future use.

### 8.1.1 Detailed Description
### 8.1.2 Constructor & Destructor Documentation
AVIOption( ) [inline]
### Member Data Documentation
float frameRate
>Frame rate of the stream.
