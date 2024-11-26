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
#include <peripheral/I2C.h>
#include <peripheral/SPI.h>
#include <peripheral/RTC.h>
#include <peripheral/UART.h>

#include <APB_DeviceInfo_7series.h>
#include <APB_GPIO.h>
#include <APB_MDIO.h>
#include <APB_SPIHostInterface.h>
#include <APB_XADC.h>
#include <APB_EthernetRxBuffer.h>
#include <APB_EthernetTxBuffer_10G.h>
#include <embedded-utils/APB_SpiFlashInterface.h>

#include <microkvs/driver/STM32StorageBank.h>
#include <staticnet-config.h>

#include <staticnet/stack/staticnet.h>
#include <staticnet/drivers/apb/APBEthernetInterface.h>
#include <staticnet/drivers/stm32/STM32CryptoEngine.h>
//#include <staticnet/ssh/SSHTransportServer.h>
//#include "ManagementDHCPClient.h"
//#include "CrossbarSSHKeyManager.h"

#include <embedded-utils/LogSink.h>

#include <bootloader/BootloaderAPI.h>

#include <supervisor/SupervisorSPIRegisters.h>

void App_Init();
void InitFMC();
void InitRTC();
void InitFPGAFlash();
void InitI2C();
void InitEEPROM();
void InitSupervisor();
void InitIP();
void InitManagementPHY();
void PollPHYs();
void ConfigureIP();

void DoInitKVS();

//Interface to supervisor MCU
extern SPI<64, 64> g_superSPI;
extern GPIOPin* g_superSPICS;
uint16_t ReadSupervisorRegister(superregs_t regid);

//Common hardware interface stuff (mostly Ethernet related)
extern UART<32, 256> g_cliUART;
extern GPIOPin g_leds[4];
extern APBEthernetInterface g_ethIface;
extern MACAddress g_macAddress;
extern IPv4Config g_ipConfig;
extern IPv6Config g_ipv6Config;
extern MDIODevice* g_phyMdio;
extern const char* g_linkSpeedNamesLong[];
/*
extern bool g_usingDHCP;
extern ManagementDHCPClient* g_dhcpClient;
*/
extern EthernetProtocol* g_ethProtocol;
extern bool g_basetLinkUp;
extern uint8_t g_basetLinkSpeed;
//extern CrossbarSSHKeyManager g_keyMgr;

/*
extern const char* g_defaultSshUsername;
extern const char* g_usernameObjectID;
extern char g_sshUsername[CLI_USERNAME_MAX];
*/

//IP address configuration
extern const IPv4Address g_defaultIP;
extern const IPv4Address g_defaultNetmask;
extern const IPv4Address g_defaultBroadcast;
extern const IPv4Address g_defaultGateway;

void UART4_Handler();
/*
void OnEthernetLinkStateChanged();
bool CheckForFPGAEvents();
void TrimSpaces(char* str);
*/
void RegisterProtocolHandlers(IPv4Protocol& ipv4);
/*
//Flash controller for FPGA bitstream
extern APBSpiFlashInterface* g_fpgaFlash;
*/

extern volatile APB_XADC FXADC;
extern volatile APB_GPIO FPGA_GPIOA;

#define MAX_LOG_SINKS 4
extern LogSink<MAX_LOG_SINKS>* g_logSink;

#endif
