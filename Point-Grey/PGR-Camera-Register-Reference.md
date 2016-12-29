---
date: 2016-03-06 16:16
status: public
title: PGR-Camera-Register-Reference
---

# 1.Using Control and Status Registers
>The user can monitor or control each feature of the camera through the control and status registers (CSRs) programmed into the camera firmware. These registers conform to the IIDC v1.32 standard (except where noted). Format tables for each 32-bit register are presented to describe the purpose of each bit that comprises the register. Bit 0 is always the most significant bit of the register value. Register offsets and values are generally referred to in their hexadecimal forms, represented by either a ‘0x’ before the number or ‘h’ after the number, e.g. the decimal number 255 can be represented as 0xFF or FFh. The controllable fields of most registers are Mode and Value.
## 1.1 Modes
Each CSR has three bits for mode control, ON_OFF, One_Push and A_M_Mode (Auto/Manual mode). Each feature can have four states corresponding to the combination of mode control bits.

|One_Push|ON_OFF|A_M_Mode|State|
|---|---|---|---|
|N/A|0|N/A|**Off state.** Feature will be fixed value state and uncontrollable.|
|N/A|1|1|**Auto control state.** Camera controls feature by itself continuously.|
|0|1|0|**Manual control state.** User can control feature by writing value to the value field.|
|1(Self clear)|1|0|**One-Push action.** Camera controls feature by itself only once and returns to the Manual control state with adjusted value.|

## 1.2 Values
If the Presence_Inq bit of the register is one, the value field is valid and can be used for controlling the feature. The user can write control values to the value field only in the Manual control state. In the other states, the user can only read the value. The camera always has to show the real setting value at the value field if Presence_Inq is one.
## 1.3 Register Memory Map
The camera uses a 64-bit fixed addressing model. The upper 10 bits show the Bus ID, and the next six bits show the Node ID. The next 20 bits must be 1 (FFFF Fh).

|Address|Register Name|Description|
|---|---|---|
|FFFF F000 0000h|Base address||
|FFFF F000 0400h|Config ROM||
|FFFF F000 0400h|Base address for all camera control command registers||
||The following register addresses are offset from the base address, FFFF F0F0 0000h.||
|000h|INITIALIZE|Camera initialize register|
|100h|V_FORMAT_INQ|Inquiry register for video format|
|180h|V_MODEE_INQ_X|Inquiry register for video mode|
|200h|V_RATE_INQ_y_X|Inquiry register for video frame rate|
|300h|Reserved||
||BASIC_FUNC_INQ||
|400h|FEATURE_HI_INQ|Inquiry register for feature presence|
||FEATURE_LO_INQ||
|500h|*Feature_Name_INQ*|Inquiry register for feature elements|
|600h|CAM_STA_CTRL|Status and control register for camera|
|640h||Feature control error status register|
|700h|ABS_CSR_HI_INQ_x|Inquiry register for Absolute value CSR offset address|
|800h|Feature_Name|Status and control register for feature|

The FlyCapture API library has function calls to get and set camera register values. These function calls automatically take into account the base address. For example, to get the 32-bit value of the SHUTTER register at 0xFFFF F0F0 081C:
FlyCapture v2.x (assuming a camera object named cam):
cam.ReadRegister(0x81C, &regVal);
cam.WriteRegister(0x81C, regVal, broadcast=false);
Broadcast is only available for FlyCapture2 and FireWire cameras. FireWire has the ability to write to multiple cameras at the same time.
## 1.4 Config ROM
### 1.4.1 Root Directory
||Offset|Bit|Description|
|----|----|----|---|
|Bus Info Block|||
|Root Directory|||


### 1.4.2 Unit Directory
|Offset|Bit|Description|
|---|---|---|
|0000h|||
|0004h|||
|0008h|||
|000Ch|||
### 1.4.3 Unit Dependent Info
|Offset|Bit|Description|
|----|----|----|
|0000h|||
|0004h|||
......
## 1.5 Calculating Base Register Address using 32-bit Offsets
## 1.6 Absolute Value Registers
### 1.6.1 Setting Absolute Value Register Values
### 1.6.2 Absolute Value Offset Addresses
### 1.6.3 Units of Value for Absolute Value CSR Registers
### 1.6.4 Determining Absolute Value Register Values

# 2.Inquiry Registers
# 3.General Camera Operation
# 4.Input/Output Control
# 5.Video Format,Mode,and Frame Rate Settings
# 6.Image Acquisition
# 7.Camera Attributes
# 8.Troubleshoting