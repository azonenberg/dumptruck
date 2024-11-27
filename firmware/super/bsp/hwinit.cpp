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

/**
	@file
	@author	Andrew D. Zonenberg
	@brief	Boot-time hardware initialization
 */
#include <core/platform.h>
#include <supervisor/supervisor-common.h>
#include "hwinit.h"
#include <peripheral/Power.h>

void InitSPI();
void InitRailSensors();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common global hardware config used by both bootloader and application

//UART console
//USART1 is on APB1 (80 MHz), so we need a divisor of 694.44, round to 694
UART<16, 256> g_uart(&USART1, 694);

//SPI bus to the main MCU
SPI<64, 64> g_spi(&SPI1, true, 2, false);

//I2C1 defaults to running of APB clock (80 MHz)
//Prescale by 4 to get 20 MHz
//Divide by 200 after that to get 100 kHz
I2C g_i2c(&I2C1, 4, 200);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Low level init

void BSP_InitUART()
{
	//Initialize the UART for local console: 115.2 Kbps
	//TODO: nice interface for enabling UART interrupts
	GPIOPin uart_tx(&GPIOA, 9, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 7);
	GPIOPin uart_rx(&GPIOA, 10, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 7);

	g_logTimer.Sleep(10);	//wait for UART pins to be high long enough to remove any glitches during powerup

	//Enable the UART interrupt
	NVIC_EnableIRQ(37);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common features shared by both application and bootloader

void BSP_Init()
{
	//Bring up the IBC before powering up the rest of the system
	InitGPIOs();
	Super_Init();
	InitSPI();

	InitRailSensors();

	App_Init();
}

void InitSPI()
{
	/*
	g_log("Initializing management SPI bus\n");

	//Set up GPIOs for management bus
	static GPIOPin mgmt_sck(&GPIOA, 5, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_FAST, 5);
	static GPIOPin mgmt_mosi(&GPIOA, 7, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_FAST, 5);
	static GPIOPin mgmt_cs_n(&GPIOA, 4, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_FAST, 5);
	static GPIOPin mgmt_miso(&GPIOA, 6, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_FAST, 5);

	//Save the CS# pin
	g_spiCS = &mgmt_cs_n;

	//Set up IRQ6 for SPI CS# (PA4) change
	RCCHelper::EnableSyscfg();
	NVIC_EnableIRQ(10);
	EXTI::SetExtInterruptMux(4, EXTI::PORT_A);
	EXTI::EnableChannel(4);
	EXTI::EnableFallingEdgeTrigger(4);

	//Set up IRQ35 as SPI1 interrupt
	NVIC_EnableIRQ(35);
	g_spi.EnableRxInterrupt();
	*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GPIOs for all of the rail enables

void InitGPIOs()
{
	g_log("Initializing GPIOs\n");

	//turn off all LEDs
	g_pgoodLED = 0;
	g_faultLED = 0;
	g_sysokLED = 0;

	//Set up GPIOs for I2C bus
	static GPIOPin i2c_scl(&GPIOB, 6, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 4, true);
	static GPIOPin i2c_sda(&GPIOB, 7, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 4, true);
}

void InitRailSensors()
{
	g_log("Initializing rail sensors\n");
	LogIndenter li(g_log);

	float tltc = GetLTCTemp();
	g_log("LTC3374A: %d.%02d C\n",
		static_cast<int>(tltc),
		static_cast<int>(tltc * 100) % 100);

	int v_vbus = GetRailVoltageMillivolts(INA_VBUS);
	g_log("VBUS:      %d.%03d V\n", v_vbus / 1000, v_vbus % 1000);

	int v_3v3_sb = GetRailVoltageMillivolts(INA_3V3_SB);
	g_log("3V3_SB:    %d.%03d V\n", v_3v3_sb / 1000, v_3v3_sb % 1000);
}

float GetLTCTemp()
{
	//220 mV at 25C plus 7 mV/c
	float vtemp = g_adc->ReadChannelScaledAveraged(5, 16, 3.3);
	return ((vtemp - 0.22) / 0.007) + 25;
}

int GetRailVoltageMillivolts(uint8_t i2cAddr)
{
	g_i2c.NonblockingStart(1, i2cAddr, false);
	while(!g_i2c.IsStartDone())
	{}

	g_i2c.NonblockingWrite(0x02);	//bus voltage
	while(!g_i2c.IsWriteDone())
	{}

	g_i2c.NonblockingStart(2, i2cAddr, true);
	while(!g_i2c.IsStartDone())
	{}

	while(!g_i2c.IsReadReady())
	{}
	int code = (g_i2c.GetReadData()) << 8;

	while(!g_i2c.IsReadReady())
	{}
	code |= g_i2c.GetReadData();

	//1.25 mV / LSB
	return static_cast<int>(1.25 * code);
}

int GetRailCurrentMilliamps(uint8_t i2cAddr)
{
	g_i2c.NonblockingStart(1, i2cAddr, false);
	while(!g_i2c.IsStartDone())
	{}

	g_i2c.NonblockingWrite(0x01);	//shunt voltage
	while(!g_i2c.IsWriteDone())
	{}

	g_i2c.NonblockingStart(2, i2cAddr, true);
	while(!g_i2c.IsStartDone())
	{}

	while(!g_i2c.IsReadReady())
	{}
	int code = (g_i2c.GetReadData()) << 8;

	while(!g_i2c.IsReadReady())
	{}
	code |= g_i2c.GetReadData();

	//2.5 uV/LSB, 25 milliohm shunt
	float vshunt = code * 0.0000025;
	float ishunt = vshunt / 0.025;
	return static_cast<int>(ishunt * 1000);
}

void PrintRail(const char* name, uint8_t i2cAddr)
{
	int v = GetRailVoltageMillivolts(i2cAddr);
	int i = GetRailCurrentMilliamps(i2cAddr);
	int p = (v * i) / 1000;

	//Sanity check: if voltage is nonsense, ignore current
	if(v < 100)
	{
		i = 0;
		p = 0;
	}

	g_log("%7s:   %d.%03d V, %4d mA, %4d mW\n",
		name,
		v / 1000, v % 1000,
		i,
		p);
}

void PrintAllRails()
{
	g_log("Rail status:\n");
	LogIndenter li(g_log);

	PrintRail("VBUS", INA_VBUS);
	PrintRail("3V3_SB", INA_3V3_SB);
	PrintRail("3V3", INA_3V3);
	PrintRail("2V5", INA_2V5);
	PrintRail("1V8", INA_1V8);
	PrintRail("1V2", INA_1V2);
	PrintRail("1V0", INA_1V0);
	PrintRail("DUT_VDD", INA_DUT_VDD);
}
