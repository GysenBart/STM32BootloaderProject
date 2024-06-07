/*
 * bootloader.c
 *
 *  Created on: 19 mrt. 2024
 *      Author: bart-
 */

#include "bootloader.h"
#include "main.h"
#include "inttypes.h"
#include "crc.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

//uint8_t bl_rx_buffer[BLRXLEN];	// Buffer that holds bootloader received data
uint8_t bl_rx_buffer[BLRXLEN];	// Buffer that holds bootloader received data
extern UART_HandleTypeDef huart3;
extern CRC_HandleTypeDef hcrc;

void  bootloader_uart_read_data(void)
{
    uint8_t rcv_len=0;

	memset(bl_rx_buffer,0,100);

	HAL_UART_Receive(&huart3,bl_rx_buffer,1,500);
	rcv_len= bl_rx_buffer[0];
	HAL_UART_Receive(&huart3,&bl_rx_buffer[1],rcv_len,500);
	switch(bl_rx_buffer[1])
	{
		case BL_FLASH_ERASE:
			bootloader_handle_flash_erase_cmd(bl_rx_buffer, BL_UART);
			break;
		case BL_MEM_WRITE:
			//execute_flash_erase(4 , 16);
			crc_response(bl_rx_buffer, bl_rx_buffer[0]);
			bootloader_handle_mem_write_cmd(bl_rx_buffer, BL_UART);
			break;
		case BL_EOT_EXIT:
			if(bootloader_check_eot(bl_rx_buffer, sizeof(bl_rx_buffer), BL_UART))
			{
				ExitBootloader();
			}
			break;
		 default:
			break;
	}
}

void crc_response(uint8_t *pBuffer, uint8_t size)
{
	uint8_t crcArray[3];
	unsigned short crcResponse;
	crcResponse = CRC16(pBuffer, size);
	crcArray[0] = (uint8_t)(crcResponse & 0x00FF);
	crcArray[1] = (uint8_t)((crcResponse & 0xFF00) >> 8);
	crcArray[2] = 0;
	//uint16_t crcResponse = HAL_CRC_Calculate(&hcrc, (uint32_t *)pBuffer, pBuffer[0]);
	HAL_UART_Transmit(&huart3, crcArray, sizeof(crcArray), 500);
}

uint8_t bootloader_handle_flash_erase_cmd(uint8_t *pBuffer, uint8_t protocol)
{
	HAL_StatusTypeDef status;

	switch(protocol)
	{
	case BL_UART:
		status = execute_flash_erase(pBuffer[2] , pBuffer[3]);
		break;
	case BL_UDP:
		status = execute_flash_erase(pBuffer[1] , pBuffer[2]);
		break;
	}
	return status;
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

uint8_t bootloader_handle_mem_write_cmd(uint8_t *pBuffer, uint8_t protocol)
{
	uint8_t status;
	uint8_t payload_len;
	uint32_t mem_address;

	//glow the led to indicate bootloader is currently writing to memory
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);

	switch(protocol)
	{
	case BL_UART:
		payload_len = pBuffer[6];
		mem_address = *((uint32_t *) ( &pBuffer[2]) );
		//execute mem write
		status = execute_mem_write(&pBuffer[7],mem_address, payload_len);
		break;
	case BL_UDP:
		payload_len = pBuffer[5];
		mem_address = *((uint32_t *) ( &pBuffer[1]) );
		//execute mem write
		status = execute_mem_write(&pBuffer[6],mem_address, payload_len);
		break;
	}

	//turn off the led to indicate memory write is over
	HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);

	return status;
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

uint8_t bootloader_check_eot(uint8_t *pBuffer, uint16_t size, uint8_t protocol)
{
	switch(protocol)
	{
		case BL_UART:
			const uint8_t expectedSequenceUART[] = {0x06, 0x45, 0x4F, 0x54, 0x0A, 0x0D};
			const uint8_t expectedLengthUART = sizeof(expectedSequenceUART);

			//	if(size != expectedLength)
			//	{
			//		return 0;
			//	}

			for(uint8_t i = 0; i < expectedLengthUART; i++)
			{
				if(pBuffer[i] != expectedSequenceUART[i])
				{
					return 0;
				}
			}
			break;

		case BL_UDP:
			const uint8_t expectedSequenceUDP[] = {0x45, 0x4F, 0x54, 0x0A, 0x0D};
			const uint8_t expectedLengthUDP = sizeof(expectedSequenceUDP);

			//	if(size != expectedLength)
			//	{
			//		return 0;
			//	}

			for(uint8_t i = 0; i < expectedLengthUDP; i++)
			{
				if(pBuffer[i] != expectedSequenceUDP[i])
				{
					return 0;
				}
			}
			break;
	}

	return 1;
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


