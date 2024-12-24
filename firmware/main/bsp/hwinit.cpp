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
#include <fpga/FPGAFirmwareUpdater.h>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Memory mapped SFRs on the FPGA

volatile APB_GPIO FPGA_GPIOA __attribute__((section(".fgpioa")));
volatile APB_DeviceInfo_7series FDEVINFO __attribute__((section(".fdevinfo")));
volatile APB_MDIO FMDIO __attribute__((section(".fmdio")));
volatile APB_SPIHostInterface FSPI1 __attribute__((section(".fspi1")));
volatile APB_XADC FXADC __attribute__((section(".fxadc")));
volatile APB_Curve25519 FCURVE25519 __attribute__((section(".fcurve25519")));
volatile APB_SerialLED FRGBLED __attribute__((section(".frgbled")));

volatile APB_EthernetTxBuffer_10G FETHTX __attribute__((section(".fethtx")));
volatile APB_EthernetRxBuffer FETHRX __attribute__((section(".fethrx")));

volatile APB_IOMuxConfig F1V2_MUXCFG __attribute__((section(".f1v2_muxcfg")));
volatile APB_GPIO F1V2_GPIO __attribute__((section(".f1v2_gpio")));
volatile APB_SPIHostInterface F1V2_SPI __attribute__((section(".f1v2_spi")));

volatile APB_IOMuxConfig F1V8_MUXCFG __attribute__((section(".f1v8_muxcfg")));
volatile APB_GPIO F1V8_GPIO __attribute__((section(".f1v8_gpio")));
volatile APB_SPIHostInterface F1V8_SPI  __attribute__((section(".f1v8_spi")));

volatile APB_IOMuxConfig F2V5_MUXCFG __attribute__((section(".f2v5_muxcfg")));
volatile APB_GPIO F2V5_GPIO __attribute__((section(".f2v5_gpio")));
volatile APB_SPIHostInterface F2V5_SPI  __attribute__((section(".f2v5_spi")));

volatile APB_IOMuxConfig F3V3_MUXCFG __attribute__((section(".f3v3_muxcfg")));
volatile APB_GPIO F3V3_GPIO __attribute__((section(".f3v3_gpio")));
volatile APB_SPIHostInterface F3V3_SPI  __attribute__((section(".f3v3_spi")));

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Common peripherals used by application and bootloader

/**
	@brief UART console

	Default after reset is for UART4 to be clocked by PCLK1 (APB1 clock) which is 62.5 MHz
	So we need a divisor of 542.53
 */
UART<32, 256> g_cliUART(&UART4, 543);

/**
	@brief MCU GPIO LEDs
 */
GPIOPin g_leds[4] =
{
	GPIOPin(&GPIOH, 8, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),
	GPIOPin(&GPIOF, 13, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),
	GPIOPin(&GPIOF, 15, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW),
	GPIOPin(&GPIOF, 14, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW)
};

/**
	@brief FPGA GPIO LEDs
 */
APB_GPIOPin g_fpgaLEDs[4] =
{
	APB_GPIOPin(&FPGA_GPIOA, 0, APB_GPIOPin::MODE_OUTPUT, APB_GPIOPin::INIT_DEFERRED),
	APB_GPIOPin(&FPGA_GPIOA, 1, APB_GPIOPin::MODE_OUTPUT, APB_GPIOPin::INIT_DEFERRED),
	APB_GPIOPin(&FPGA_GPIOA, 2, APB_GPIOPin::MODE_OUTPUT, APB_GPIOPin::INIT_DEFERRED),
	APB_GPIOPin(&FPGA_GPIOA, 3, APB_GPIOPin::MODE_OUTPUT, APB_GPIOPin::INIT_DEFERRED)
};

/**
	@brief MAC address I2C EEPROM
	Default kernel clock for I2C2 is pclk2 (68.75 MHz for our current config)
	Prescale by 16 to get 4.29 MHz
	Divide by 40 after that to get 107 kHz
*/
I2C g_macI2C(&I2C2, 16, 40);

///@brief The single supported SSH username
char g_sshUsername[CLI_USERNAME_MAX] = "";

///@brief KVS key for the SSH username
const char* g_usernameObjectID = "ssh.username";

///@brief Default SSH username if not configured
const char* g_defaultSshUsername = "admin";

///@brief Selects whether the DHCP client is active or not
//bool g_usingDHCP = false;

///@brief The DHCP client
//ManagementDHCPClient* g_dhcpClient = nullptr;

///@brief IRQ line to the FPGA
APB_GPIOPin* g_ethIRQ = nullptr;

///@brief The battery-backed RAM used to store state across power cycles
volatile BootloaderBBRAM* g_bbram = reinterpret_cast<volatile BootloaderBBRAM*>(&_RTC.BKP[0]);

///@brief SPI bus to the supervisor MCU
GPIOPin g_superCS_n(&GPIOF, 6, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_MEDIUM);

/**
	@brief SPI bus to supervisor

	SPI5 runs on spi 4/5 kernel clock domain
	default after reset is APB2 clock which is 62.5 MHz, divide by 128 to get 488 kHz
 */
SPI<64, 64> g_superSPI(&SPI5, true, 64);

///@brief Version string for supervisor MCU
char g_superVersion[20] = {0};

///@brief Database of authorized SSH keys
SSHKeyManager g_keyMgr;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do other initialization

void InitITM();

void BSP_Init()
{
	//Set up PLL2 to run the external memory bus
	//We have some freedom with how fast we clock this!
	//Doesn't have to be a multiple of 500 since separate VCO from the main system
	RCCHelper::InitializePLL(
		2,		//PLL2
		25,		//input is 25 MHz from the HSE
		2,		//25/2 = 12.5 MHz at the PFD
		24,		//12.5 * 24 = 300 MHz at the VCO
		32,		//div P (not used for now)
		32,		//div Q (not used for now)
		1,		//div R (300 MHz FMC kernel clock = 150 MHz FMC clock)
		RCCHelper::CLOCK_SOURCE_HSE
	);

	InitRTCFromHSE();
	InitSupervisor();
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
	#ifdef _DEBUG
		ITM::Enable();
		DWT::EnablePCSampling(DWT::PC_SAMPLE_SLOW);
		ITM::EnableDwtForwarding();
	#endif
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

	//Initialize the LEDs
	for(auto& led : g_fpgaLEDs)
		led.DeferredInit();
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

	static APB_SpiFlashInterface flash(&FSPI1, 2);	//100 MHz PCLK = 25 MHz SCK
													//(even dividers required, /2 fails for reasons TBD)
	g_fpgaFlash = &flash;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SFR access

/**
	@brief Initialize the SPI bus to the supervisor
 */
void InitSupervisor()
{
	g_log("Initializing supervisor\n");
	LogIndenter li(g_log);

	//Deselect the supervisor
	g_superCS_n = 1;
	g_logTimer.Sleep(1);

	//Initialize the rest of our IOs
	static GPIOPin spi_sck(&GPIOF, 7, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_MEDIUM, 5);
	static GPIOPin spi_miso(&GPIOF, 8, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_MEDIUM, 5);
	static GPIOPin spi_mosi(&GPIOF, 9, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_MEDIUM, 5);

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

	auto mcutemp = ReadSupervisorRegister(SUPER_REG_MCUTEMP);
	g_log("    Super:  %uhk C\n", mcutemp);

	auto vvbus = ReadSupervisorRegister(SUPER_REG_VVBUS);
	g_log("    VBUS:   %2d.%03d V\n", vvbus / 1000, vvbus % 1000);

	auto v3v3_sb = ReadSupervisorRegister(SUPER_REG_3V3);
	g_log("    3V3_SB: %2d.%03d V\n", v3v3_sb / 1000, v3v3_sb % 1000);

	auto v3v3 = ReadSupervisorRegister(SUPER_REG_V3V3);
	g_log("    3V3:    %2d.%03d V\n", v3v3 / 1000, v3v3 % 1000);

	auto v2v5 = ReadSupervisorRegister(SUPER_REG_V2V5);
	g_log("    2V5:    %2d.%03d V\n", v2v5 / 1000, v2v5 % 1000);

	auto v1v8 = ReadSupervisorRegister(SUPER_REG_V1V8);
	g_log("    1V8:    %2d.%03d V\n", v1v8 / 1000, v1v8 % 1000);

	auto v1v2 = ReadSupervisorRegister(SUPER_REG_V1V2);
	g_log("    1V2:    %2d.%03d V\n", v1v2 / 1000, v1v2 % 1000);

	auto v1v0 = ReadSupervisorRegister(SUPER_REG_V1V0);
	g_log("    1V0:    %2d.%03d V\n", v1v0 / 1000, v1v0 % 1000);

	auto vdutvdd = ReadSupervisorRegister(SUPER_REG_VDUTVDD);
	g_log("DUT_VDD:    %2d.%03d V\n", vdutvdd / 1000, vdutvdd % 1000);
}

uint16_t ReadSupervisorRegister(dsuperregs_t regid)
{
	return ReadSupervisorRegister(static_cast<superregs_t>(regid));
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
}

void SendSupervisorCommand(dsuperregs_t regid)
{
	g_superCS_n = 0;

	//Send the register
	g_superSPI.BlockingWrite(regid);
	g_superSPI.WaitForWrites();
	g_superSPI.DiscardRxData();

	g_superSPI.BlockingRead();	//discard dummy byte sent in response to the command

	g_superCS_n = 1;

	//Wait a short time between commands
	g_logTimer.Sleep(2);
}
