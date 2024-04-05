/*
 * bootloader.h
 *
 *  Created on: 19 mrt. 2024
 *      Author: bart-
 */

#ifndef INC_BOOTLOADER_H_
#define INC_BOOTLOADER_H_

#include "stm32f4xx_hal.h"

#define USERAPPLICATION_BASEADDRESS	0x08010000		// Base address of the user application
#define BLRXLEN						500			// buffer size for the bootloader data

//This command is used to mass erase or sector erase of the user flash .
#define BL_FLASH_ERASE          0x56

//This command is used to write data in to different memories of the MCU
#define BL_MEM_WRITE			0x57


// External variables
extern uint8_t bl_rx_buffer[BLRXLEN];	// Buffer that holds bootloader received data
//extern uint64_t bl_rx_buffer[BLRXLEN];	// Buffer that holds bootloader received data

// Exit the bootloader and jump to the user application
void ExitBootloader();

// Bootloader commands
void bootloader_handle_flash_erase_cmd(uint8_t *pBuffer);
void bootloader_handle_mem_write_cmd(uint8_t *pBuffer);

// Helper function for debugging
uint8_t execute_flash_erase(uint8_t sector_number , uint8_t number_of_sector);
uint8_t execute_mem_write(uint8_t *pBuffer, uint32_t mem_address, uint32_t len);
void  bootloader_uart_read_data(void);


#endif /* INC_BOOTLOADER_H_ */
