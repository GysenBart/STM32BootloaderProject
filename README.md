# STM32BootloaderProject

This repository contains two main projects:

## STM32BootloaderSolution

This solution contains a bootloader project and a user application, both on a NUCLEO-F429ZI

### Bootloader

The STM32 Bootloader Project is designed to facilitate firmware updates for STM32 microcontrollers. It operates by listening over UART for commands, particularly focusing on "erase" and "write" commands. The bootloader expects a packet containing the following information:

To erase:
PACKET: LEN 0x56 STARTSECTOR QTY

To write:
PACKET: LEN 0x57 ADDR PAYLOADLENGTH PAYLOAD

### UserApp

Just a example application for testing.

## QTUartBinaryFlasher

The Qt Project with UI is a graphical user interface (GUI) application developed using the Qt framework. This application is designed to work in conjunction with the STM32 bootloader project to facilitate firmware updates. Key features of this project include:

- Selecting a binary file for firmware update
- Reading the selected binary file
- Selecting an available COM port for communication
- Opening/closing the selected COM port
- Sending the necessary packet (containing firmware update information) to the STM32 bootloader via UART
