---
date: 2016-01-23 10:14
status: public
title: Camera-Interface
---

# IEEE-1394 Connector
The Bumblebee®2 has a standard 6-pin IEEE-1394 connector that is used for data transmission, camera control and powering the camera. The maximum 1394 cable length between any 1394 node (e.g. camera to PCI card, card to hub, etc.) is 4.5m, as specified by the IEEE-1394 standard. Use standard, shielded twisted pair copper cables.
标准6针，数据传输/供电。指定标准下，任意1394节点间最大线长4.5m（相机到PCI卡，卡到hub总线）。使用屏蔽的双绞铜线。
# General Purpose Input/Output (GPIO)
The Bumblebee2 has a 12-pin Hirose HR10 (Mfg P/N: HR10A-10R-12SB) female circular connector on the back of the case. 
母接头。对应公接头在PCI采集卡端。
Camera KIT contents include a pre-wired male connector; refer to the diagram below for wire colorcoding. Additional male counterparts (Mfg P/N: HR10A-10P-12P) can be purchased from Digi-Key (P/N: HR112-ND).

![](~/10-23-03.jpg)
The Bumblebee2 GPIO pins are TTL 3.3V pins. Inputs can be configured to accept external trigger signals. When configured as inputs, the pins are internally pulled high using *weak pull-up resistors(弱上拉电阻）* to allow easy triggering of the camera by simply shorting the pin to ground (GND). The inputs are protected from both over and under voltage.
**过、低电压下虽有输入保护，但仍然建议输入标准电压信号。**
It is recommended, however, that they only be connected to 5V or 3.3V digital logic signals. Outputs can be configured to send an output signal or *strobe pulse（闸门脉冲）*. When configured as outputs, each line can sink 10mA of current.
**确认输出后，电流下降10mA。**