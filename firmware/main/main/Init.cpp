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

//#include "DemoCLISessionContext.h"
#include <peripheral/ITMStream.h>
//#include "../super/superregs.h"

///@brief Output stream for local serial console
//UARTOutputStream g_localConsoleOutputStream;

///@brief Context data structure for local serial console
//DemoCLISessionContext g_localConsoleSessionContext;

//extern Iperf3Server* g_iperfServer;

///@brief ITM serial trace data stream
ITMStream g_itmStream(4);

/**
	@brief Initialize global GPIO LEDs
 */
void InitLEDs()
{
	g_leds[0] = 1;
	g_leds[1] = 1;
	g_leds[2] = 1;
	g_leds[3] = 1;
}

/**
	@brief Initialize sensors and log starting values for each
 */
/*
void InitSensors()
{
	g_log("Initializing sensors\n");
	LogIndenter li(g_log);

	//Wait 50ms to get accurate readings
	//g_logTimer.Sleep(500);

	//No fans on this board, skip those

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
*/

/**
	@brief Initialize the digital temperature sensor
 */
/*
void InitDTS()
{
	auto tempval = g_dts.GetTemperature();
	g_log("MCU die temperature:                   %d.%02d C\n",
		(tempval >> 8),
		static_cast<int>(((tempval & 0xff) / 256.0) * 100));
}

void RegisterProtocolHandlers(IPv4Protocol& ipv4)
{
	__attribute__((section(".tcmbss"))) static DemoUDPProtocol udp(&ipv4);
	__attribute__((section(".tcmbss"))) static DemoTCPProtocol tcp(&ipv4, udp);
	ipv4.UseUDP(&udp);
	ipv4.UseTCP(&tcp);
	//g_dhcpClient = &udp.GetDHCP();
}
*/

void App_Init()
{
	/*
	//Enable interrupts early on since we use them for e.g. debug logging during boot
	EnableInterrupts();

	//Basic hardware setup
	InitLEDs();
	InitDTS();
	InitSensors();
	*/
}
