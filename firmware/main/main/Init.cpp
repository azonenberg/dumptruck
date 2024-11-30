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

#include "dumptruck.h"
#include <ctype.h>
//#include "../super/superregs.h"
#include "../bsp/FPGATask.h"
#include <tcpip/IPAgingTask.h>
#include <tcpip/PhyPollTask.h>
#include "LocalConsoleTask.h"
#include "SocketDetectionTask.h"

//#include "../super/superregs.h"

/**
	@brief Initialize global GPIO LEDs
 */
void InitLEDs()
{
	//Turn on the MCU GPIO LEDs
	g_leds[0] = 1;
	g_leds[1] = 1;
	g_leds[2] = 1;
	g_leds[3] = 1;

	//Turn on the FPGA GPIO LEDs
	g_fpgaLEDs[0] = 1;
	g_fpgaLEDs[1] = 1;
	g_fpgaLEDs[2] = 1;
	g_fpgaLEDs[3] = 1;

	//Turn off all of the RGB LEDs
	for(int i=0; i<6; i++)
		FRGBLED.framebuffer[i] = 0x000000;
}

/**
	@brief Initialize sensors and log starting values for each
 */
void InitSensors()
{
	g_log("Initializing sensors\n");
	LogIndenter li(g_log);

	//No fans on this board

	//Read FPGA temperature
	auto temp = FXADC.die_temp;
	g_log("FPGA die temperature:              %uhk C\n", temp);

	//Read FPGA voltage sensors
	int volt = FXADC.volt_core;
	g_log("FPGA VCCINT:                        %uhk V\n", volt);
	volt = FXADC.volt_ram;
	g_log("FPGA VCCBRAM:                       %uhk V\n", volt);
	volt = FXADC.volt_aux;
	g_log("FPGA VCCAUX:                        %uhk V\n", volt);
}

void App_Init()
{
	//Enable interrupts early on since we use them for e.g. debug logging during boot
	EnableInterrupts();

	//Basic hardware setup
	InitLEDs();
	InitDTS();
	InitSensors();

	static FPGATask fpgaTask;
	static PhyPollTask phyTask;
	static IPAgingTask ipAgingTask;
	static LocalConsoleTask localConsoleTask;
	static SocketDetectionTask socketDetectionTask;

	g_tasks.push_back(&fpgaTask);
	g_tasks.push_back(&phyTask);
	g_tasks.push_back(&ipAgingTask);
	g_tasks.push_back(&localConsoleTask);
	g_tasks.push_back(&socketDetectionTask);

	g_timerTasks.push_back(&phyTask);
	g_timerTasks.push_back(&ipAgingTask);
	g_timerTasks.push_back(&socketDetectionTask);

	g_detectionTask = &socketDetectionTask;
}

void RegisterProtocolHandlers(IPv4Protocol& ipv4)
{
	__attribute__((section(".tcmbss"))) static DumptruckUDPProtocol udp(&ipv4);
	//__attribute__((section(".tcmbss"))) static DemoTCPProtocol tcp(&ipv4, udp);
	ipv4.UseUDP(&udp);
	//ipv4.UseTCP(&tcp);

	g_udp = &udp;
	//g_dhcpClient = &udp.GetDHCP();
}
