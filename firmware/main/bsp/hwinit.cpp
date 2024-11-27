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
#include "hwinit.h"
#include <peripheral/FMC.h>
#include <peripheral/DWT.h>
#include <peripheral/ITM.h>
#include <ctype.h>
#include <embedded-utils/CoreSightRom.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory mapped SFRs on the FPGA

volatile APB_GPIO FPGA_GPIOA __attribute__((section(".fgpioa")));
volatile APB_DeviceInfo_7series FDEVINFO __attribute__((section(".fdevinfo")));
volatile APB_MDIO FMDIO __attribute__((section(".fmdio")));
volatile APB_SPIHostInterface FSPI1 __attribute__((section(".fspi1")));
volatile APB_XADC FXADC __attribute__((section(".fxadc")));

volatile APB_EthernetTxBuffer_10G FETHTX __attribute__((section(".fethtx")));
volatile APB_EthernetRxBuffer FETHRX __attribute__((section(".fethrx")));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common peripherals used by application and bootloader

/**
	@brief UART console

	Default after reset is for UART4 to be clocked by PCLK1 (APB1 clock) which is 62.5 MHz
	So we need a divisor of 542.53
 */
UART<32, 256> g_cliUART(&UART4, 543);

/**
	@brief GPIO LEDs
 */
GPIOPin g_leds[4] =
{
	GPIOPin(&GPIOC, 2, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),
	GPIOPin(&GPIOF, 9, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),
	GPIOPin(&GPIOH, 12, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),
	GPIOPin(&GPIOH, 6, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW)
};

/**
	@brief MAC address I2C EEPROM
	Default kernel clock for I2C2 is pclk2 (68.75 MHz for our current config)
	Prescale by 16 to get 4.29 MHz
	Divide by 40 after that to get 107 kHz
*/
I2C g_macI2C(&I2C2, 16, 40);

///@brief Key manager
//CrossbarSSHKeyManager g_keyMgr;

///@brief The single supported SSH username
//char g_sshUsername[CLI_USERNAME_MAX] = "";

///@brief KVS key for the SSH username
//const char* g_usernameObjectID = "ssh.username";

///@brief Default SSH username if not configured
//const char* g_defaultSshUsername = "admin";

///@brief Selects whether the DHCP client is active or not
//bool g_usingDHCP = false;

///@brief The DHCP client
//ManagementDHCPClient* g_dhcpClient = nullptr;

///@brief IRQ line to the FPGA
APB_GPIOPin* g_ethIRQ = nullptr;

///@brief The battery-backed RAM used to store state across power cycles
volatile BootloaderBBRAM* g_bbram = reinterpret_cast<volatile BootloaderBBRAM*>(&_RTC.BKP[0]);

///@brief SPI bus to the supervisor MCU
GPIOPin g_superCS_n(&GPIOE, 4, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_MEDIUM);

/**
	@brief SPI bus to supervisor

	SPI4 runs on spi 4/5 kernel clock domain
	default after reset is APB2 clock which is 62.5 MHz, divide by 128 to get 488 kHz
 */
SPI<64, 64> g_superSPI(&SPI4, true, 64);

///@brief Version string for supervisor MCU
char g_superVersion[20] = {0};

///@brief SPI flash controller for FPGA
APB_SpiFlashInterface* g_fpgaFlash;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do other initialization

void InitITM();

void BSP_Init()
{
	InitRTCFromHSE();
	//InitSupervisor();
	InitFMC();
	InitFPGA();
	InitFPGAFlash();
	DoInitKVS();
	InitI2C();
	InitMacEEPROM();
	InitManagementPHY();
	InitIP();
	InitITM();

	App_Init();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Enable trace

/**
	@brief Initialize tracing
 */
void InitITM()
{
	g_log("Initializing ITM\n");

	//Enable ITM, enable PC sampling, and turn on forwarding to the TPIU
	ITM::Enable();
	DWT::EnablePCSampling(DWT::PC_SAMPLE_FAST);
	ITM::EnableDwtForwarding();

	//Turn on ITM stimulus channel 4 for serial logging
	ITM::EnableChannel(4);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// BSP overrides for low level init

void BSP_InitUART()
{
	//Initialize the UART for local console: 115.2 Kbps using PA12 for UART4 transmit and PA11 for UART4 receive
	//TODO: nice interface for enabling UART interrupts
	static GPIOPin uart_tx(&GPIOA, 12, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 6);
	static GPIOPin uart_rx(&GPIOA, 11, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 6);

	//Enable the UART interrupt
	NVIC_EnableIRQ(52);

	g_logTimer.Sleep(10);	//wait for UART pins to be high long enough to remove any glitches during powerup
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Higher level initialization we used for a lot of stuff

void InitFMC()
{
	g_log("Initializing FMC...\n");
	LogIndenter li(g_log);

	static GPIOPin fmc_ad0(&GPIOD, 14, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad1(&GPIOD, 15, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad2(&GPIOD, 0, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad3(&GPIOD, 1, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad4(&GPIOE, 7, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad5(&GPIOE, 8, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad6(&GPIOE, 9, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad7(&GPIOE, 10, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad8(&GPIOE, 11, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad9(&GPIOA, 5, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad10(&GPIOB, 14, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad11(&GPIOE, 14, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad12(&GPIOE, 15, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad13(&GPIOD, 8, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad14(&GPIOD, 9, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_ad15(&GPIOD, 10, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);

	static GPIOPin fmc_a16(&GPIOD, 11, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_a17(&GPIOD, 12, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_a18(&GPIOD, 13, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);

	static GPIOPin fmc_a19(&GPIOE, 3, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_a20(&GPIOE, 4, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_a21(&GPIOE, 5, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_a22(&GPIOE, 6, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_a23(&GPIOE, 2, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);

	static GPIOPin fmc_nl_nadv(&GPIOB, 7, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_nwait(&GPIOC, 6, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 9);
	static GPIOPin fmc_ne1(&GPIOC, 7, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 9);
	static GPIOPin fmc_ne3(&GPIOG, 6, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_clk(&GPIOD, 3, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_noe(&GPIOD, 4, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_nwe(&GPIOD, 5, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_nbl0(&GPIOE, 0, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);
	static GPIOPin fmc_nbl1(&GPIOE, 1, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_VERYFAST, 12);

	//Add a pullup on NWAIT
	fmc_nwait.SetPullMode(GPIOPin::PULL_UP);

	InitFMCForFPGA();
}

void InitI2C()
{
	g_log("Initializing I2C interfaces\n");
	static GPIOPin mac_i2c_scl(&GPIOH, 4, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 4, true);
	static GPIOPin mac_i2c_sda(&GPIOH, 5, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 4, true);
}

void InitFPGAFlash()
{
	g_log("Initializing FPGA flash\n");
	LogIndenter li(g_log);

	static APB_SpiFlashInterface flash(&FSPI1, 10);	//100 MHz PCLK = 10 MHz SCK
	g_fpgaFlash = &flash;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SFR access

/**
	@brief Initialize the SPI bus to the supervisor
 */
/*
void InitSupervisor()
{
	g_log("Initializing supervisor\n");
	LogIndenter li(g_log);

	//Deselect the supervisor
	g_superCS_n = 1;
	g_logTimer.Sleep(1);

	//Initialize the rest of our IOs
	static GPIOPin spi_sck(&GPIOE, 12, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_MEDIUM, 5);
	static GPIOPin spi_miso(&GPIOE, 5, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_MEDIUM, 5);
	static GPIOPin spi_mosi(&GPIOE, 6, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_MEDIUM, 5);

	//Get the supervisor firmware version
	g_superCS_n = 0;
	g_superSPI.BlockingWrite(SUPER_REG_VERSION);
	g_superSPI.WaitForWrites();
	g_superSPI.DiscardRxData();
	g_logTimer.Sleep(2);
	g_superSPI.BlockingRead();	//discard dummy byte
	for(size_t i=0; i<sizeof(g_superVersion); i++)
		g_superVersion[i] = g_superSPI.BlockingRead();
	g_superVersion[sizeof(g_superVersion)-1] = '\0';
	g_superCS_n = 1;
	g_log("Firmware version:     %s\n", g_superVersion);

	//Get IBC firmware version
	g_superCS_n = 0;
	g_superSPI.BlockingWrite(SUPER_REG_IBCVERSION);
	g_superSPI.WaitForWrites();
	g_superSPI.DiscardRxData();
	g_logTimer.Sleep(2);
	g_superSPI.BlockingRead();	//discard dummy byte
	for(size_t i=0; i<sizeof(g_ibcVersion); i++)
		g_ibcVersion[i] = g_superSPI.BlockingRead();
	g_ibcVersion[sizeof(g_ibcVersion)-1] = '\0';
	g_superCS_n = 1;
	g_log("IBC firmware version: %s\n", g_ibcVersion);

	//Get IBC hardware version
	g_superCS_n = 0;
	g_superSPI.BlockingWrite(SUPER_REG_IBCHWVERSION);
	g_superSPI.WaitForWrites();
	g_superSPI.DiscardRxData();
	g_logTimer.Sleep(2);
	g_superSPI.BlockingRead();	//discard dummy byte
	for(size_t i=0; i<sizeof(g_ibcHwVersion); i++)
		g_ibcVersion[i] = g_superSPI.BlockingRead();
	g_ibcHwVersion[sizeof(g_ibcHwVersion)-1] = '\0';
	g_superCS_n = 1;
	g_log("IBC hardware version: %s\n", g_ibcHwVersion);

	//Read sensors
	auto vin = ReadSupervisorRegister(SUPER_REG_IBCVIN);
	g_log("IBC input:  %2d.%03d V\n", vin / 1000, vin % 1000);

	auto iin = ReadSupervisorRegister(SUPER_REG_IBCIIN);
	g_log("            %2d.%03d A\n", iin / 1000, iin % 1000);

	auto pin = vin * iin / 1000;
	g_log("            %2d.%03d W\n", pin / 1000, pin % 1000);

	auto vout = ReadSupervisorRegister(SUPER_REG_IBCVOUT);
	g_log("    output: %2d.%03d V\n", vout / 1000, vout % 1000);

	auto iout = ReadSupervisorRegister(SUPER_REG_IBCIOUT);
	g_log("            %2d.%03d A\n", iout / 1000, iout % 1000);

	auto pout = vout * iout / 1000;
	g_log("            %2d.%03d W\n", pout / 1000, pout % 1000);

	auto vsense = ReadSupervisorRegister(SUPER_REG_IBCVSENSE);
	g_log("    sense:  %2d.%03d V\n", vsense / 1000, vsense % 1000);

	auto ibctemp = ReadSupervisorRegister(SUPER_REG_IBCTEMP);
	g_log("    DC-DC:  %uhk C\n", ibctemp);

	auto smcutemp = ReadSupervisorRegister(SUPER_REG_IBCMCUTEMP);
	g_log("    MCU:    %uhk C\n", smcutemp);

	auto ibc3v3 = ReadSupervisorRegister(SUPER_REG_IBC3V3);
	g_log("            %2d.%03d V\n", ibc3v3 / 1000, ibc3v3 % 1000);

	auto mcutemp = ReadSupervisorRegister(SUPER_REG_MCUTEMP);
	g_log("    Super:  %uhk C\n", mcutemp);

	auto super3v3 = ReadSupervisorRegister(SUPER_REG_3V3);
	g_log("            %2d.%03d V\n", super3v3 / 1000, super3v3 % 1000);
}

uint16_t ReadSupervisorRegister(superregs_t regid)
{
	g_superCS_n = 0;

	//Send the register
	g_superSPI.BlockingWrite(regid);
	g_superSPI.WaitForWrites();
	g_superSPI.DiscardRxData();

	//Wait a little while since the firmware on the supervisor is slow
	//TODO: can we optimize this? or send several dummy cycles or something?
	g_logTimer.Sleep(2);

	g_superSPI.BlockingRead();	//discard dummy byte

	uint16_t ret = g_superSPI.BlockingRead();
	ret |= g_superSPI.BlockingRead() << 8;
	g_superCS_n = 1;

	return ret;
}*/
