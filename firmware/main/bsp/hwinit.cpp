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
//#include "LogSink.h"
#include <peripheral/FMC.h>
#include <peripheral/DWT.h>
#include <peripheral/ITM.h>
#include <peripheral/Power.h>
#include <ctype.h>
#include <embedded-utils/CoreSightRom.h>

void TrimSpaces(char* str);

/**
	@brief Mapping of link speed IDs to printable names
 */
static const char* g_linkSpeedNamesLong[] =
{
	"10 Mbps",
	"100 Mbps",
	"1000 Mbps",
	"10 Gbps"
};

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

//APB1 is 62.5 MHz but default is for timer clock to be 2x the bus clock (see table 53 of RM0468)
//Divide down to get 10 kHz ticks
Timer g_logTimer(&TIM2, Timer::FEATURE_GENERAL_PURPOSE, 12500);

///@brief Character device for logging
//LogSink<MAX_LOG_SINKS>* g_logSink = nullptr;

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
	@brief Global Ethernet interface

	Place it in TCM since we're not currently using DMA and TCM is faster for software memory copies
 */
__attribute__((section(".tcmbss"))) APBEthernetInterface g_ethIface(&FETHRX, &FETHTX);

///@brief Our MAC address
MACAddress g_macAddress;

///@brief Our IPv4 address
IPv4Config g_ipConfig;

///@brief Ethernet protocol stack
EthernetProtocol* g_ethProtocol = nullptr;

/**
	@brief MAC address I2C EEPROM
	Default kernel clock for I2C2 is pclk2 (68.75 MHz for our current config)
	Prescale by 16 to get 4.29 MHz
	Divide by 40 after that to get 107 kHz
*/
I2C g_macI2C(&I2C2, 16, 40);

///@brief BaseT link status
bool g_basetLinkUp = false;

//Ethernet link speed
uint8_t g_basetLinkSpeed = 0;

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

///@brief USERCODE of the FPGA (build timestamp)
uint32_t g_usercode = 0;

///@brief FPGA die serial number
uint8_t g_fpgaSerial[8] = {0};

///@brief IRQ line to the FPGA
APB_GPIOPin* g_ethIRQ = nullptr;

///@brief MDIO device for the PHY
MDIODevice* g_phyMdio = nullptr;

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

///@brief Version string for IBC board
char g_ibcHwVersion[20] = {0};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Globals

const IPv4Address g_defaultIP			= { .m_octets{ 10,   2,   6,  50} };
const IPv4Address g_defaultNetmask		= { .m_octets{255, 255, 255,   0} };
const IPv4Address g_defaultBroadcast	= { .m_octets{ 10,   2,   6, 255} };
const IPv4Address g_defaultGateway		= { .m_octets{ 10,   2,   6, 252} };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Do other initialization

void InitITM();

void BSP_Init()
{
	InitRTC();
	//InitSupervisor();
	//InitFMC();
	//InitFPGA();
	DoInitKVS();
	/*
	InitI2C();
	InitEEPROM();
	InitManagementPHY();
	InitIP();
	*/
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

void BSP_InitPower()
{
	//Initialize power (must be the very first thing done after reset)
	Power::ConfigureSMPSToLDOCascade(Power::VOLTAGE_1V8, RANGE_VOS0);
}

void BSP_InitClocks()
{
	//With CPU_FREQ_BOOST not set, max frequency is 520 MHz

	//Configure the flash with wait states and prefetching before making any changes to the clock setup.
	//A bit of extra latency is fine, the CPU being faster than flash is not.
	Flash::SetConfiguration(513, RANGE_VOS0);

	//By default out of reset, we're clocked by the HSI clock at 64 MHz
	//Initialize the external clock source at 25 MHz
	RCCHelper::EnableHighSpeedExternalClock();

	//Set up PLL1 to run off the external oscillator
	RCCHelper::InitializePLL(
		1,		//PLL1
		25,		//input is 25 MHz from the HSE
		2,		//25/2 = 12.5 MHz at the PFD
		40,		//12.5 * 40 = 500 MHz at the VCO
		1,		//div P (primary output 500 MHz)
		10,		//div Q (50 MHz kernel clock)
		5,		//div R (100 MHz SWO Manchester bit clock, 50 Mbps data rate)
		RCCHelper::CLOCK_SOURCE_HSE
	);

	//Set up PLL2 to run the external memory bus
	//We have some freedom with how fast we clock this!
	//Doesn't have to be a multiple of 500 since separate VCO from the main system
	RCCHelper::InitializePLL(
		2,		//PLL2
		25,		//input is 25 MHz from the HSE
		2,		//25/2 = 12.5 MHz at the PFD
		22,		//12.5 * 22 = 275 MHz at the VCO
		32,		//div P (not used for now)
		32,		//div Q (not used for now)
		1,		//div R (275 MHz FMC kernel clock = 137.5 MHz FMC clock)
		RCCHelper::CLOCK_SOURCE_HSE
	);

	//Set up main system clock tree
	RCCHelper::InitializeSystemClocks(
		1,		//sysclk = 500 MHz
		2,		//AHB = 250 MHz
		4,		//APB1 = 62.5 MHz
		4,		//APB2 = 62.5 MHz
		4,		//APB3 = 62.5 MHz
		4		//APB4 = 62.5 MHz
	);

	//RNG clock should be >= HCLK/32
	//AHB2 HCLK is 250 MHz so min 7.8125 MHz
	//Select PLL1 Q clock (50 MHz)
	RCC.D2CCIP2R = (RCC.D2CCIP2R & ~0x300) | (0x100);

	//Select PLL1 as system clock source
	RCCHelper::SelectSystemClockFromPLL1();
}

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

void BSP_InitLog()
{
	//static LogSink<MAX_LOG_SINKS> sink(&g_cliUART);
	//g_logSink = &sink;

	//g_log.Initialize(g_logSink, &g_logTimer);
	g_log.Initialize(&g_cliUART, &g_logTimer);
	g_log("DUMPTRUCK by Andrew D. Zonenberg\n");
	g_log("Firmware compiled at %s on %s\n", __TIME__, __DATE__);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Higher level initialization we used for a lot of stuff
/*
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

	//Enable the FMC and select PLL2 R as the clock source
	RCCHelper::Enable(&_FMC);
	RCCHelper::SetFMCKernelClock(RCCHelper::FMC_CLOCK_PLL2_R);

	//Use free-running clock output (so FPGA can clock APB off it)
	//Configured as 16-bit multiplexed synchronous PSRAM
	static FMCBank fmc(&_FMC, 0);
	fmc.EnableFreeRunningClock();
	fmc.EnableWrites();
	fmc.SetSynchronous();
	fmc.SetBusWidth(FMC_BCR_WIDTH_16);
	fmc.SetMemoryType(FMC_BCR_TYPE_PSRAM);
	fmc.SetAddressDataMultiplex();

	//Enable wait states wiath NWAIT active during the wait
	fmc.EnableSynchronousWaitStates();
	fmc.SetEarlyWaitState(false);

	//Map the PSRAM bank in slot 1 (0xc0000000) as strongly ordered / device memory
	fmc.SetPsramBankAs1();
}
*/
void InitRTC()
{
	g_log("Initializing RTC...\n");
	LogIndenter li(g_log);
	g_log("Using external clock divided by 50 (500 kHz)\n");

	//Turn on the RTC APB clock so we can configure it, then set the clock source for it in the RCC
	RCCHelper::Enable(&_RTC);
	RTC::SetClockFromHSE(50);
}

void DoInitKVS()
{
	/*
		Use sectors 6 and 7 of main flash (in single bank mode) for a 128 kB microkvs

		Each log entry is 64 bytes, and we want to allocate ~50% of storage to the log since our objects are pretty
		small (SSH keys, IP addresses, etc). A 1024-entry log is a nice round number, and comes out to 64 kB or 50%,
		leaving the remaining 64 kB or 50% for data.
	 */
	static STM32StorageBank left(reinterpret_cast<uint8_t*>(0x080c0000), 0x20000);
	static STM32StorageBank right(reinterpret_cast<uint8_t*>(0x080e0000), 0x20000);
	InitKVS(&left, &right, 1024);
}
/*
void InitI2C()
{
	g_log("Initializing I2C interfaces\n");
	static GPIOPin mac_i2c_scl(&GPIOH, 4, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 4, true);
	static GPIOPin mac_i2c_sda(&GPIOH, 5, GPIOPin::MODE_PERIPHERAL, GPIOPin::SLEW_SLOW, 4, true);
}

void InitEEPROM()
{
	g_log("Initializing MAC address EEPROM\n");

	//Extended memory block for MAC address data isn't in the normal 0xa* memory address space
	//uint8_t main_addr = 0xa0;
	uint8_t ext_addr = 0xb0;

	//Pointers within extended memory block
	uint8_t serial_offset = 0x80;
	uint8_t mac_offset = 0x9a;

	//Read MAC address
	g_macI2C.BlockingWrite8(ext_addr, mac_offset);
	g_macI2C.BlockingRead(ext_addr, &g_macAddress[0], sizeof(g_macAddress));

	//Read serial number
	const int serial_len = 16;
	uint8_t serial[serial_len] = {0};
	g_macI2C.BlockingWrite8(ext_addr, serial_offset);
	g_macI2C.BlockingRead(ext_addr, serial, serial_len);

	{
		LogIndenter li(g_log);
		g_log("MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n",
			g_macAddress[0], g_macAddress[1], g_macAddress[2], g_macAddress[3], g_macAddress[4], g_macAddress[5]);

		g_log("EEPROM serial number: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\n",
			serial[0], serial[1], serial[2], serial[3], serial[4], serial[5], serial[6], serial[7],
			serial[8], serial[9], serial[10], serial[11], serial[12], serial[13], serial[14], serial[15]);
	}
}
*/

/**
	@brief Bring up the control interface to the FPGA
 */
/*
void InitFPGA()
{
	g_log("Initializing FPGA\n");
	LogIndenter li(g_log);

	//Wait for the DONE signal to go high
	g_log("Waiting for FPGA boot\n");
	static GPIOPin fpgaDone(&GPIOA, 0, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
	while(!fpgaDone)
	{}

	//Verify reliable functionality by poking the scratchpad register (TODO: proper timing-control link training?)
	g_log("FMC loopback test...\n");
	{
		LogIndenter li2(g_log);
		uint32_t tmp = 0xbaadc0de;
		uint32_t count = 100000;
		uint32_t errs = 0;
		for(uint32_t i=0; i<count; i++)
		//for(uint32_t i=0; true; i++)
		{
			FDEVINFO.scratch = tmp;
			uint32_t readback = FDEVINFO.scratch;
			if(readback != tmp)
			{
				//if(errs == 0)
					g_log(Logger::ERROR, "Iteration %u: wrote 0x%08x, read 0x%08x\n", i, tmp, readback);
				errs ++;
			}
			tmp ++;
		}
		g_log("%u iterations complete, %u errors\n", count, errs);
	}

	//Read the FPGA IDCODE and serial number
	while(FDEVINFO.status != 3)
	{}

	uint32_t idcode = FDEVINFO.idcode;
	memcpy(g_fpgaSerial, (const void*)FDEVINFO.serial, 8);

	//Print status
	switch(idcode & 0x0fffffff)
	{
		case 0x3647093:
			g_log("IDCODE: %08x (XC7K70T rev %d)\n", idcode, idcode >> 28);
			break;

		case 0x364c093:
			g_log("IDCODE: %08x (XC7K160T rev %d)\n", idcode, idcode >> 28);
			break;

		case 0x37c4093:
			g_log("IDCODE: %08x (XC7S25 rev %d)\n", idcode, idcode >> 28);
			break;

		case 0x4a63093:
			g_log("IDCODE: %08x (XCKU3P rev %d)\n", idcode, idcode >> 28);
			break;

		default:
			g_log("IDCODE: %08x (unknown device, rev %d)\n", idcode, idcode >> 28);
			break;
	}
	g_log("Serial: %02x%02x%02x%02x%02x%02x%02x%02x\n",
		g_fpgaSerial[7], g_fpgaSerial[6], g_fpgaSerial[5], g_fpgaSerial[4],
		g_fpgaSerial[3], g_fpgaSerial[2], g_fpgaSerial[1], g_fpgaSerial[0]);

	//Read USERCODE
	g_usercode = FDEVINFO.usercode;
	g_log("Usercode: %08x\n", g_usercode);
	{
		LogIndenter li2(g_log);

		//Format per XAPP1232:
		//31:27 day
		//26:23 month
		//22:17 year
		//16:12 hr
		//11:6 min
		//5:0 sec
		int day = g_usercode >> 27;
		int mon = (g_usercode >> 23) & 0xf;
		int yr = 2000 + ((g_usercode >> 17) & 0x3f);
		int hr = (g_usercode >> 12) & 0x1f;
		int min = (g_usercode >> 6) & 0x3f;
		int sec = g_usercode & 0x3f;
		g_log("Bitstream timestamp: %04d-%02d-%02d %02d:%02d:%02d\n",
			yr, mon, day, hr, min, sec);
	}

	InitFPGAFlash();

	//Set up FPGA LEDs
	FPGA_GPIOA.tris = 0xfffffff0;
	FPGA_GPIOA.out = 0x5;
}
*/

/**
	@brief Initializes the management PHY
 */
 /*
void InitManagementPHY()
{
	g_log("Initializing management PHY\n");
	LogIndenter li(g_log);

	//Reset the PHY
	static APB_GPIOPin phy_rst_n(&FPGA_GPIOA, 4, APB_GPIOPin::MODE_OUTPUT);
	phy_rst_n = 0;
	g_logTimer.Sleep(10);
	phy_rst_n = 1;

	//Wait 100us (datasheet page 62 note 2) before starting to program the PHY
	g_logTimer.Sleep(10);

	//Read the PHY ID
	static MDIODevice phydev(&FMDIO, 0);
	g_phyMdio = &phydev;
	auto phyid1 = phydev.ReadRegister(REG_PHY_ID_1);
	auto phyid2 = phydev.ReadRegister(REG_PHY_ID_2);

	if( (phyid1 == 0x22) && ( (phyid2 >> 4) == 0x162))
	{
		g_log("PHY ID   = %04x %04x (KSZ9031RNX rev %d)\n", phyid1, phyid2, phyid2 & 0xf);

		//Adjust pad skew for RX_CLK register to improve timing FPGA side
		//ManagementPHYExtendedWrite(2, REG_KSZ9031_MMD2_CLKSKEW, 0x01ef);
	}
	else
		g_log("PHY ID   = %04x %04x (unknown)\n", phyid1, phyid2);
}

void InitFPGAFlash()
{
	g_log("Initializing FPGA flash\n");
	LogIndenter li(g_log);

	static APB_SpiFlashInterface flash(&FSPI1, 10);	//125 MHz PCLK = 12.5 MHz SCK
	//g_fpgaFlash = &flash;
}
*/
/**
	@brief Remove spaces from trailing edge of a string
 */
void TrimSpaces(char* str)
{
	char* p = str + strlen(str) - 1;

	while(p >= str)
	{
		if(isspace(*p))
			*p = '\0';
		else
			break;

		p --;
	}
}

/**
	@brief Set our IP address and initialize the IP stack
 */
/*
void InitIP()
{
	g_log("Initializing management IPv4 interface\n");
	LogIndenter li(g_log);

	g_ethIface.Init();

	static APB_GPIOPin irq(&FPGA_GPIOA, 6, APB_GPIOPin::MODE_INPUT);
	g_ethIRQ = &irq;

	ConfigureIP();

	g_log("Our IP address is %d.%d.%d.%d\n",
		g_ipConfig.m_address.m_octets[0],
		g_ipConfig.m_address.m_octets[1],
		g_ipConfig.m_address.m_octets[2],
		g_ipConfig.m_address.m_octets[3]);

	//ARP cache (shared by all interfaces)
	static ARPCache cache;

	//Per-interface protocol stacks
	static EthernetProtocol eth(g_ethIface, g_macAddress);
	g_ethProtocol = &eth;
	static ARPProtocol arp(eth, g_ipConfig.m_address, cache);

	//Global protocol stacks
	static IPv4Protocol ipv4(eth, g_ipConfig, cache);
	static ICMPv4Protocol icmpv4(ipv4);

	//Register protocol handlers with the lower layer
	eth.UseARP(&arp);
	eth.UseIPv4(&ipv4);
	ipv4.UseICMPv4(&icmpv4);
	RegisterProtocolHandlers(ipv4);
}
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// SFR access

/**
	@brief Load our IP configuration from the KVS
 */
 /*
void ConfigureIP()
{
	g_ipConfig.m_address = g_kvs->ReadObject<IPv4Address>(g_defaultIP, "ip.address");
	g_ipConfig.m_netmask = g_kvs->ReadObject<IPv4Address>(g_defaultNetmask, "ip.netmask");
	g_ipConfig.m_broadcast = g_kvs->ReadObject<IPv4Address>(g_defaultBroadcast, "ip.broadcast");
	g_ipConfig.m_gateway = g_kvs->ReadObject<IPv4Address>(g_defaultGateway, "ip.gateway");
}*/

/**
	@brief Check PHYs for updates
 */
/*
void PollPHYs()
{
	//Get the baseT link state
	uint16_t bctl = g_phyMdio->ReadRegister(REG_BASIC_CONTROL);
	uint16_t bstat = g_phyMdio->ReadRegister(REG_BASIC_STATUS);
	bool bup = (bstat & 4) == 4;
	if(bup && !g_basetLinkUp)
	{
		g_basetLinkSpeed = 0;
		if( (bctl & 0x40) == 0x40)
			g_basetLinkSpeed |= 2;
		if( (bctl & 0x2000) == 0x2000)
			g_basetLinkSpeed |= 1;
		g_log("Interface mgmt0: link is up at %s\n", g_linkSpeedNamesLong[g_basetLinkSpeed]);
		//OnEthernetLinkStateChanged();
		g_ethProtocol->OnLinkUp();
	}
	else if(!bup && g_basetLinkUp)
	{
		g_log("Interface mgmt0: link is down\n");
		g_basetLinkSpeed = 0xff;
		//OnEthernetLinkStateChanged();
		g_ethProtocol->OnLinkDown();
	}
	g_basetLinkUp = bup;
}
*/

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
