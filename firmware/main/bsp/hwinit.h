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

#include <cli/UARTOutputStream.h>

#include <peripheral/CRC.h>
#include <peripheral/Flash.h>
#include <peripheral/GPIO.h>
#include <peripheral/SPI.h>
#include <peripheral/UART.h>

#include <APB_Curve25519.h>
#include <APB_DeviceInfo_7series.h>
#include <APB_GPIO.h>
#include <APB_SerialLED.h>
#include <APB_SPIHostInterface.h>
#include <APB_XADC.h>
#include <APB_EthernetRxBuffer.h>
#include <APB_EthernetTxBuffer_10G.h>

#include <tcpip/CommonTCPIP.h>
#include <fpga/Ethernet.h>
#include <staticnet/drivers/stm32/STM32CryptoEngine.h>
#include <staticnet/ssh/SSHTransportServer.h>
//#include "ManagementDHCPClient.h"
#include <tcpip/SSHKeyManager.h>

#include <embedded-utils/LogSink.h>

#include <bootloader/BootloaderAPI.h>

#include <supervisor/SupervisorSPIRegisters.h>
#include "../../super/main/DumptruckSuperSPIRegisters.h"

#include <boilerplate/h735/StandardBSP.h>
#include <fpga/FMCUtils.h>

void App_Init();
void InitFMC();
void InitFPGAFlash();
void InitI2C();
void InitSupervisor();

//Interface to supervisor MCU
extern SPI<64, 64> g_superSPI;
extern GPIOPin* g_superSPICS;
uint16_t ReadSupervisorRegister(superregs_t regid);
uint16_t ReadSupervisorRegister(dsuperregs_t regid);
void SendSupervisorCommand(dsuperregs_t regid);

//Common hardware interface stuff (mostly Ethernet related)
extern GPIOPin g_leds[4];
extern APB_GPIOPin g_fpgaLEDs[4];

//extern bool g_usingDHCP;
//extern ManagementDHCPClient* g_dhcpClient;

extern SSHKeyManager g_keyMgr;

extern const char* g_defaultSshUsername;
extern const char* g_usernameObjectID;
extern char g_sshUsername[CLI_USERNAME_MAX];

void UART4_Handler();
/*
void OnEthernetLinkStateChanged();
bool CheckForFPGAEvents();
*/
void RegisterProtocolHandlers(IPv4Protocol& ipv4);

enum class IOMuxConfig
{
	Inactive	= 0,
	X4_SPI		= 1
};

struct APB_IOMuxConfig
{
	uint32_t	muxsel;
};

extern volatile APB_XADC FXADC;
extern volatile APB_GPIO FPGA_GPIOA;
extern volatile APB_Curve25519 FCURVE25519;
extern volatile APB_SerialLED FRGBLED;
extern volatile APB_DeviceInfo_7series FDEVINFO;

//1V2 flash
extern volatile APB_IOMuxConfig	F1V2_MUXCFG;
extern volatile APB_GPIO F1V2_GPIO;
extern volatile APB_SPIHostInterface F1V2_SPI;

//1V8 flash
extern volatile APB_IOMuxConfig	F1V8_MUXCFG;
extern volatile APB_GPIO F1V8_GPIO;
extern volatile APB_SPIHostInterface F1V8_SPI;

//2V5 flash
extern volatile APB_IOMuxConfig	F2V5_MUXCFG;
extern volatile APB_GPIO F2V5_GPIO;
extern volatile APB_SPIHostInterface F2V5_SPI;

//3V3 flash
extern volatile APB_IOMuxConfig	F3V3_MUXCFG;
extern volatile APB_GPIO F3V3_GPIO;
extern volatile APB_SPIHostInterface F3V3_SPI;

#endif
