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

#ifndef dumptruck_h
#define dumptruck_h

#include <core/platform.h>
#include <hwinit.h>

#include <peripheral/SPI.h>

#include <common-embedded-platform/services/Iperf3Server.h>

#include <embedded-utils/StringBuffer.h>

enum channelid_t
{
	CHANNEL_1V2,
	CHANNEL_1V8,
	CHANNEL_2V5,
	CHANNEL_3V3,
	CHANNEL_NONE,
	CHANNEL_CONFLICT	//more than one socket inserted simultaneously
};

#include "DumptruckUDPProtocol.h"
#include "DumptruckTCPProtocol.h"

extern Iperf3Server* g_iperfServer;
extern DumptruckUDPProtocol* g_udp;
extern DumptruckTCPProtocol* g_tcp;

void InitLEDs();
void InitSensors();

//RGB LED color constants
#define RGB_OFF		0x000000
#define RGB_RED		0x200000
#define RGB_YELLOW	0x202000
#define RGB_GREEN	0x002000
#define RGB_BLUE	0x000020

class SocketDetectionTask;
extern SocketDetectionTask* g_detectionTask;

extern DumptruckSSHTransportServer* g_sshd;

#endif
