/***********************************************************************************************************************
*                                                                                                                      *
* fpga-stm32-iface                                                                                                     *
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

#ifndef dumptruck_h
#define dumptruck_h

#include <core/platform.h>
#include <hwinit.h>

#include <peripheral/DTS.h>
//#include <peripheral/SPI.h>

#include <common-embedded-platform/services/Iperf3Server.h>

#include <embedded-utils/StringBuffer.h>
//#include "DemoUDPProtocol.h"
//#include "DemoTCPProtocol.h"

//extern ManagementNTPClient* g_ntpClient;

extern DigitalTempSensor g_dts;
extern Iperf3Server* g_iperfServer;

void InitLEDs();
void InitDTS();
//void InitSupervisor();
void InitSensors();
/*
uint16_t SupervisorRegRead(uint8_t regid);

extern SPI<64, 64> g_superSPI;
extern GPIOPin* g_superSPICS;

extern char g_superVersion[20];

extern ManagementSSHTransportServer* g_sshd;
*/

#endif
