/*
 * bootloader.c
 *
 *  Created on: 19 mrt. 2024
 *      Author: bart-
 */

#include "bootloader.h"
#include "main.h"
#include "inttypes.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

//uint8_t bl_rx_buffer[BLRXLEN];	// Buffer that holds bootloader received data
uint8_t bl_rx_buffer[BLRXLEN];	// Buffer that holds bootloader received data
extern UART_HandleTypeDef huart3;

void  bootloader_uart_read_data(void)
{
    uint8_t rcv_len=0;

	memset(bl_rx_buffer,0,500);

	HAL_UART_Receive(&huart3,bl_rx_buffer,1,500);
	rcv_len= bl_rx_buffer[0];
	HAL_UART_Receive(&huart3,&bl_rx_buffer[1],rcv_len,500);
	switch(bl_rx_buffer[1])
	{
		case BL_FLASH_ERASE:
//			bootloader_handle_flash_erase_cmd(bl_rx_buffer);
			execute_flash_erase(4 , 16);
			break;
		case BL_MEM_WRITE:
			//execute_flash_erase(4 , 16);
			bootloader_handle_mem_write_cmd(bl_rx_buffer);
			break;
		 default:
			break;
	}
}

void bootloader_handle_flash_erase_cmd(uint8_t *pBuffer)
{
	execute_flash_erase(pBuffer[2] , pBuffer[3]);
}

uint8_t execute_flash_erase(uint8_t sector_number , uint8_t number_of_sector)
{
	//we have totally 24 sectors in STM32F429ZI mcu
	//number_of_sector has to be in the range of 0 to 23
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);

	FLASH_EraseInitTypeDef flashErase_handle;
	uint32_t sectorError;
	HAL_StatusTypeDef status;

	if( number_of_sector > 23 )
	{
		return HAL_ERROR;
	}
	else
	{
		// See struct FLASH_EraseInitTypeDef for explanation of the parameters
		flashErase_handle.TypeErase = FLASH_TYPEERASE_SECTORS;
		flashErase_handle.Sector = sector_number; // this is the initial sector
		flashErase_handle.NbSectors = 24 - number_of_sector;
		flashErase_handle.Banks = FLASH_BANK_1;	// irrelevant, may be omitted (to test)
		flashErase_handle.VoltageRange = FLASH_VOLTAGE_RANGE_3;  // our mcu will work on this voltage range
		HAL_FLASH_Unlock();
		status = (uint8_t) HAL_FLASHEx_Erase(&flashErase_handle, &sectorError);
		HAL_FLASH_Lock();
	}
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	return status;
}

void bootloader_handle_mem_write_cmd(uint8_t *pBuffer)
{
	uint8_t payload_len = pBuffer[6];

	uint32_t mem_address = *((uint32_t *) ( &pBuffer[2]) );

	//glow the led to indicate bootloader is currently writing to memory
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);

	//execute mem write
	execute_mem_write(&pBuffer[7],mem_address, payload_len);

	//turn off the led to indicate memory write is over
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
}

uint8_t execute_mem_write(uint8_t *pBuffer, uint32_t mem_address, uint32_t len)
{
    uint8_t status=HAL_OK;

    //We have to unlock flash module to get control of registers
    HAL_FLASH_Unlock();

    for(uint32_t i = 0 ; i <len ; i++)
    {
        //Here we program the flash byte by byte
        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE,mem_address+i,pBuffer[i] );
    }

    HAL_FLASH_Lock();

    return status;
}

void ExitBootloader()
{
	// Function pointer to hold the address of the user application to jump to
	uint32_t reset_handler_add = *((volatile uint32_t *) (USERAPPLICATION_BASEADDRESS + 4));
	void (*Exit_Bootloader)(void) = (void*) reset_handler_add;

	// Disable RCC and set all peripherals to default
	HAL_RCC_DeInit();
	HAL_DeInit();

	// Disable systick timer
	SysTick->CTRL = 0x0;
	SysTick->LOAD=0;
	SysTick->VAL=0;

	// Main Stack Pointer value (location where the Interrupt Vector table starts). This sets
	// SCB->VTOR to the correct address
	uint32_t MspValue = *(volatile uint32_t*)USERAPPLICATION_BASEADDRESS;
	__set_MSP(MspValue);

	// Exit the bootloader and handoff control to the user application
	Exit_Bootloader();
}


