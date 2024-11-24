/***********************************************************************************************************************
*                                                                                                                      *
* DUMPTRUCK                                                                                                       *
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

#include "supervisor.h"
#include "SupervisorSPIServer.h"
#include "LEDTask.h"
#include "ButtonTask.h"

//TODO: fix this path somehow?
#include "../../../../common-ibc/firmware/main/regids.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// System status indicator LEDs

GPIOPin g_pgoodLED(&GPIOB, 9, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
GPIOPin g_faultLED(&GPIOB, 8, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
GPIOPin g_sysokLED(&GPIOH, 0, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Power rail descriptors

//TODO: ADC-feedback rail descriptor type
GPIOPin g_1v0_en(&GPIOH, 1, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
RailDescriptorWithEnable g_1v0("1V0", g_1v0_en, g_logTimer, 50);

/*
GPIOPin g_1v0_en(&GPIOB, 15, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
GPIOPin g_1v0_pgood(&GPIOA, 8, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
RailDescriptorWithEnableAndPGood g_1v0("1V0", g_1v0_en, g_1v0_pgood, g_logTimer, 75);

GPIOPin g_1v2_en(&GPIOA, 2, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
GPIOPin g_1v2_pgood(&GPIOA, 1, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
RailDescriptorWithEnableAndPGood g_1v2("1V2", g_1v2_en, g_1v2_pgood, g_logTimer, 75);

GPIOPin g_1v8_en(&GPIOA, 11, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
GPIOPin g_1v8_pgood(&GPIOB, 14, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
RailDescriptorWithEnableAndPGood g_1v8("1V8", g_1v8_en, g_1v8_pgood, g_logTimer, 75);

GPIOPin g_3v3_en(&GPIOB, 12, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
GPIOPin g_3v3_pgood(&GPIOB, 13, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
RailDescriptorWithEnableAndPGood g_3v3("3V3", g_3v3_en, g_3v3_pgood, g_logTimer, 75);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Power rail sequence
*/
etl::vector g_powerSequence
{
	//VCCINT - VCCAUX - VCCO for the FPGA
	(RailDescriptor*)&g_1v0/*,
	&g_1v8,
	&g_3v3,

	//1V2 rail for the PHY should come up after 3.3V rail (note 1 on page 62)
	&g_1v2*/
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Reset descriptors

//Active low edge triggered reset
GPIOPin g_fpgaResetN(&GPIOA, 8, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
ActiveLowResetDescriptor g_fpgaResetDescriptor(g_fpgaResetN, "FPGA PROG");
/*
//Active low level triggered delay-boot flag
//Use this as the "FPGA is done booting" indicator
GPIOPin g_fpgaInitN(&GPIOA, 3, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW, 0, true);
GPIOPin g_fpgaDone(&GPIOB, 2, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW);
ActiveLowResetDescriptorWithActiveHighDone g_fpgaInitDescriptor(g_fpgaInitN, g_fpgaDone, "FPGA INIT");

//MCU reset comes at the end
GPIOPin g_mcuResetN(&GPIOA, 3, GPIOPin::MODE_OUTPUT, GPIOPin::SLEW_SLOW);
ActiveLowResetDescriptor g_mcuResetDescriptor(g_mcuResetN, "MCU");
*/
etl::vector g_resetSequence
{
	//First boot the FPGA
	(ResetDescriptor*)&g_fpgaResetDescriptor/*,	//need to cast at least one entry to base class
												//for proper template deduction
	&g_fpgaInitDescriptor,

	//then release the MCU
	&g_mcuResetDescriptor,
	*/
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Task tables

etl::vector<Task*, MAX_TASKS>  g_tasks;
etl::vector<TimerTask*, MAX_TIMER_TASKS>  g_timerTasks;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The top level supervisor controller

DumptruckPowerResetSupervisor g_super(g_powerSequence, g_resetSequence);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Peripheral initialization

void App_Init()
{
	RCCHelper::Enable(&_RTC);

	//Format version string
	StringBuffer buf(g_version, sizeof(g_version));
	static const char* buildtime = __TIME__;
	buf.Printf("%s %c%c%c%c%c%c",
		__DATE__, buildtime[0], buildtime[1], buildtime[3], buildtime[4], buildtime[6], buildtime[7]);
	g_log("Firmware version %s\n", g_version);

	static LEDTask ledTask;
	static ButtonTask buttonTask;
	//static SupervisorSPIServer spiserver(g_spi);

	g_tasks.push_back(&ledTask);
	g_tasks.push_back(&buttonTask);
	g_tasks.push_back(&g_super);
	//g_tasks.push_back(&spiserver);

	//No external pull on this pin
	//g_mcuReady.SetPullMode(GPIOPin::PULL_DOWN);

	//wait 2 sec in case of problems
	g_logTimer.Sleep(2 * 1000 * 10);

	g_super.PowerOn();

	//Read ADC voltage for 1V0 rail
	//TODO: scale by using INA230 on 3v3_SB?
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main loop
