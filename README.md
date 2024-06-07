# STM32BootloaderProject

This repository contains two main projects:

## STM32BootloaderSolution

This solution contains a bootloader project and a user application, both on a NUCLEO-F429ZI

### Bootloader

The STM32 Bootloader Project is designed to facilitate firmware updates for STM32F429ZI microcontrollers. It operates by listening over UART and UDP for commands, particularly focusing on "erase" and "write" commands. The bootloader expects a packet containing the following information:

#### UART
To erase:
PACKET: LEN 0x56 STARTSECTOR QTY

To write:
PACKET: LEN 0x57 ADDR PAYLOADLENGTH PAYLOAD

#### UDP
To erase:
PACKET: 0x56 STARTSECTOR QTY

To write:
PACKET: 0x57 ADDR PAYLOADLENGTH PAYLOAD
### UserApp

Just a example application for testing.

## QTUartBinaryFlasher

The Qt Project with UI is a graphical user interface (GUI) application developed using the Qt framework. This application is designed to work in conjunction with the STM32F429ZI bootloader project to facilitate firmware updates. Key features of this project include:

- Selecting and reading a binary file for firmware update
- Selecting and opening/closing available COM port for communication
- Selecting IP address and binding a port of the host
- Selecting IP address and port of the target device
- Erasing firmware UART/UDP
- Send update firmware UART/UDP

### UDP example
![BootloaderUDP](https://github.com/GysenBart/STM32BootloaderProject/assets/46839017/8f3d6392-b692-4311-a47b-492b88dab540)

### UART example
![BootloaderUART](https://github.com/GysenBart/STM32BootloaderProject/assets/46839017/ddeba12b-ffc2-4d41-b53c-c26364e5c5fe)

