/***********************************************************************************************************************
*                                                                                                                      *
* DUMPTRUCK                                                                                                            *
*                                                                                                                      *
* Copyright (c) 2023-2024 Andrew D. Zonenberg and contributors                                                         *
* All rights reserved.                                                                                                 *
*                                                                                                                      *
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the     *
* following conditions are met:                                                                                        *
*                                                                                                                      *
*    * Redistributions of source code must retain the above copyright notice, this list of conditions, and the         *
*      following disclaimer.                                                                                           *
*                                                                                                                      *
*    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the       *
*      following disclaimer in the documentation and/or other materials provided with the distribution.                *
*                                                                                                                      *
*    * Neither the name of the author nor the names of any contributors may be used to endorse or promote products     *
*      derived from this software without specific prior written permission.                                           *
*                                                                                                                      *
* THIS SOFTWARE IS PROVIDED BY THE AUTHORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED   *
* TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL *
* THE AUTHORS BE HELD LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES        *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR       *
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
* POSSIBILITY OF SUCH DAMAGE.                                                                                          *
*                                                                                                                      *
***********************************************************************************************************************/

#ifndef hwinit_h
#define hwinit_h

#include <peripheral/ADC.h>
#include <peripheral/EXTI.h>
#include <peripheral/Flash.h>
#include <peripheral/GPIO.h>
#include <peripheral/I2C.h>
#include <peripheral/SPI.h>
#include <peripheral/UART.h>

#include <supervisor/supervisor-common.h>

///@brief Initialize application-specific hardware stuff
extern void App_Init();

extern uint32_t g_spiRxFifoOverflows;

//Common ISRs used by application and bootloader
void SPI_CSHandler();
void SPI1_Handler();
void USART2_Handler();

//GPIOs
extern GPIOPin g_pgoodLED;
extern GPIOPin g_faultLED;
extern GPIOPin g_sysokLED;

extern GPIOPin g_dutVddEn;
extern GPIOPin g_dutVccioEn;

void InitGPIOs();

enum Ina230I2CAddress
{
	INA_VBUS	= 0x80,
	INA_3V3_SB	= 0x82,
	INA_3V3		= 0x84,
	INA_2V5		= 0x86,
	INA_1V8		= 0x88,
	INA_1V2		= 0x8a,
	INA_1V0		= 0x8c,
	INA_DUT_VDD = 0x8e,
	INA_VIO33	= 0x90,
	INA_VIO25	= 0x92,
	INA_VIO18	= 0x94,
	INA_VIO12	= 0x96
};

float GetLTCTemp();
int GetRailVoltageMillivolts(uint8_t i2cAddr);
int GetRailCurrentMilliamps(uint8_t i2cAddr);
void PrintAllRails();
void PrintRail(const char* name, uint8_t i2cAddr);

#endif
