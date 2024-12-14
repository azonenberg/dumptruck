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
#include "SocketedFlashDumper.h"

SocketedFlashDumper::SocketedFlashDumper(channelid_t chan)
	: m_channel(chan)
{
	m_muxcfg = GetMuxConfig(chan);
}

/**
	@brief Get the mux configuration for a given channel
 */
volatile APB_IOMuxConfig* SocketedFlashDumper::GetMuxConfig(channelid_t chan)
{
	switch(chan)
	{
		case CHANNEL_3V3:	return &F3V3_MUXCFG;
		case CHANNEL_2V5:	return &F2V5_MUXCFG;
		case CHANNEL_1V8:	return &F1V8_MUXCFG;
		case CHANNEL_1V2:	return &F1V2_MUXCFG;
		default:			return nullptr;
	}
}

/**
	@brief Get the SPI controller for a given channel
 */
volatile APB_SPIHostInterface* SocketedFlashDumper::GetSPI(channelid_t chan)
{
	switch(chan)
	{
		case CHANNEL_3V3:	return &F3V3_SPI;
		case CHANNEL_2V5:	return &F2V5_SPI;
		case CHANNEL_1V8:	return &F1V8_SPI;
		case CHANNEL_1V2:	return &F1V2_SPI;
		default:			return nullptr;
	}
}

/**
	@brief Gets the supervisor register for the VCCIO voltage reading on a given channel
 */
dsuperregs_t SocketedFlashDumper::GetVIORegister(channelid_t chan)
{
	switch(chan)
	{
		case CHANNEL_3V3:	return SUPER_REG_VIO_33;
		case CHANNEL_2V5:	return SUPER_REG_VIO_25;
		case CHANNEL_1V8:	return SUPER_REG_VIO_18;
		case CHANNEL_1V2:	return SUPER_REG_VIO_12;
		default:			return SUPER_REG_VIO_33;	//should never happen but we have to return something
	}
}

/**
	@brief Gets the supervisor register for the VCCIO current reading on a given channel
 */
dsuperregs_t SocketedFlashDumper::GetIIORegister(channelid_t chan)
{
	switch(chan)
	{
		case CHANNEL_3V3:	return SUPER_REG_IIO_33;
		case CHANNEL_2V5:	return SUPER_REG_IIO_25;
		case CHANNEL_1V8:	return SUPER_REG_IIO_18;
		case CHANNEL_1V2:	return SUPER_REG_IIO_12;
		default:			return SUPER_REG_IIO_33;	//should never happen but we have to return something
	}
}

/**
	@brief Gets the supervisor register for the VCCIO current reading on a given channel
 */
uint16_t SocketedFlashDumper::GetNominalVIOMillivolts(channelid_t chan)
{
	switch(chan)
	{
		case CHANNEL_3V3:	return 3300;
		case CHANNEL_2V5:	return 2500;
		case CHANNEL_1V8:	return 1800;
		case CHANNEL_1V2:	return 1200;
		default:			return 9999;				//return garbage value that will always fail checks
	}
}

bool SocketedFlashDumper::PowerOn()
{
	g_log("Power on request\n");
	LogIndenter li(g_log);

	//Get register config for VCCIO readback
	auto regv = GetVIORegister(m_channel);
	auto regi = GetIIORegister(m_channel);
	auto expectedVCCIO = GetNominalVIOMillivolts(m_channel);

	//Set the channel LED to blue
	FRGBLED.framebuffer[2 + (int)m_channel] = RGB_BLUE;

	//Turn on power, then wait 25 ms for it to stabilize and ADCs to sample it
	//(load switch has about a 15ms ramp)
	SendSupervisorCommand(SUPER_REG_VDD_ON);
	SendSupervisorCommand(SUPER_REG_VCCIO_ON);
	g_logTimer.Sleep(250);

	//Read voltages a couple of times
	auto vio = ReadSupervisorRegister(regv);
	auto iio = ReadSupervisorRegister(regi);
	auto vcore = ReadSupervisorRegister(SUPER_REG_VDUTVDD);
	auto icore = ReadSupervisorRegister(SUPER_REG_IDUTVDD);
	g_log("VCCIO:  %d.%03dV, %d.%03dA \n", vio / 1000, vio % 1000, iio / 1000, iio % 1000);
	g_log("VDD:    %d.%03dV, %d.%03dA \n", vcore / 1000, vcore % 1000, icore / 1000, icore % 1000);

	//TODO: Validate VDD if we can?

	//TODO: check for overcurrent on VDD

	//Validate VCCIO is within range
	int vio_delta = vio - expectedVCCIO;
	if(abs(vio_delta) < 75)
		g_log("VCCIO delta: %d mV (OK)\n", vio_delta);
	else
	{
		g_log(Logger::ERROR, "VCCIO delta: %d mV (FAULT)\n", vio_delta);
		SendSupervisorCommand(SUPER_REG_VDD_OFF);
		SendSupervisorCommand(SUPER_REG_VCCIO_OFF);

		//Set the LED to red
		FRGBLED.framebuffer[2 + (int)m_channel] = RGB_RED;
		return false;
	}

	//TODO: check for overcurrent on VCCIO

	//Try to talk to the flash
	//APB_SpiFlashInterface flash(spi, 10);	//100 MHz PCLK = 10 MHz SCK

	//If we get here, all good
	return true;
}

void SocketedFlashDumper::PowerOff()
{
	g_log("Turning off DUT power\n");

	//Tristate the pins when we're done
	m_muxcfg->muxsel = (uint32_t)IOMuxConfig::Inactive;

	//Remove power from both rails
	SendSupervisorCommand(SUPER_REG_VDD_OFF);
	SendSupervisorCommand(SUPER_REG_VCCIO_OFF);

	//Set the LED to green
	FRGBLED.framebuffer[2 + (int)m_channel] = RGB_GREEN;
}
