---
date: 2016-01-22 16:18
status: public
title: Camera-And-Device-Control
---

# Frame Rate Control
Fine-tune frame rates for video conversion(e.g.PAL@24FPS)
格式转换帧率调优。

<https://en.wikipedia.org/wiki/24p#Converting_24p_to_PAL>

# Strobe Output
Increased drive strength,configurable strobe pattern output.
强力驱动闪光灯/频闪模式输出。
# RS-232 Serial Port
Provides serial communication via GPIO TTL digital logic levels.
通过GPIO **生存期**(此处TTL是否意指time-to-live?或者指transistor-transistor-logic?)数字逻辑电平，提供
**序列化数据交互**（自译）。
## Transistor–transistor logic (TTL)
Transistor–transistor logic (TTL) is a class of digital circuits built from bipolar junction transistors (BJT) and resistors. It is called transistor–transistor logic because both the logic gating function (e.g., AND) and the amplifying function are performed by transistors (contrast with resistor–transistor logic (RTL) and diode–transistor logic (DTL)).

<https://en.wikipedia.org/wiki/Transistor%E2%80%93transistor_logic>
## Time to live (TTL)
Time to live (TTL) or hop limit is a mechanism that limits the lifespan or lifetime of data in a computer or network. TTL may be implemented as a counter or timestamp attached to or embedded in the data. Once the prescribed event count or timespan has elapsed, data is discarded. In computer networking, TTL prevents a data packet from circulating indefinitely. In computing applications, TTL is used to improve performance of caching or to improve privacy.

<https://en.wikipedia.org/wiki/Time_to_live>
## serial communication
In telecommunication and computer science, serial communication is the process of sending data one bit at a time, sequentially, over a communication channel or computer bus. This is in contrast to parallel communication, where several bits are sent as a whole, on a link with several parallel channels.

Serial communication is used for all long-haul communication and most computer networks, where the cost of cable and synchronization difficulties make parallel communication impractical. Serial computer buses are becoming more common even at shorter distances, as improved signal integrity and transmission speeds in newer serial technologies have begun to outweigh the parallel bus's advantage of simplicity (no need for serializer and deserializer, or SerDes) and to outstrip its disadvantages (clock skew, interconnect density). The migration from PCI to PCI Express is an example.

<https://en.wikipedia.org/wiki/Serial_communication>
## logic level
In digital circuits, a logic level is one of a finite number of states that a digital signal can have.

<https://en.wikipedia.org/wiki/Logic_level>
# Memory Channels
Non-volatile storage of camera default power-up settings
默认上电设置为非volatile存储。
在C语言中，volatile关键字可以用来提醒编译器它后面所定义的变量随时有可能改变，因此编译后的程序每次需要存储或读取这个变量的时候，都会直接从变量地址中读取数据。如果没有volatile关键字，则编译器可能优化读取和存储，可能暂时使用寄存器中的值，如果这个变量由别的程序更新了的话，将出现不一致的现象。
# Temprature Sensor
Reports the temperature near the imaging sensor.
# Camera Upgrades 
Firmware upgradeable in field via IEEE-1394 interface.
可升级固件。