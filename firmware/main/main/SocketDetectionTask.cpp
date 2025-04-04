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
#include "SocketDetectionTask.h"

const uint8_t g_socketIdEepromAddr = 0xa2;

SocketDetectionTask::SocketDetectionTask()
	: TimerTask(0, 10 * 1000)	//1 Hz tick rate
	, m_activeChannel(CHANNEL_NONE)
	, m_3v3Present(&GPIOD, 2, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW)
	, m_2v5Present(&GPIOC, 12, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW)
	, m_1v8Present(&GPIOC, 10, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW)
	, m_1v2Present(&GPIOH, 13, GPIOPin::MODE_INPUT, GPIOPin::SLEW_SLOW)
	, m_detectPending(false)
	, m_cacheKey(0)
{
	m_3v3Present.SetPullMode(GPIOPin::PULL_DOWN);
	m_2v5Present.SetPullMode(GPIOPin::PULL_DOWN);
	m_1v8Present.SetPullMode(GPIOPin::PULL_DOWN);
	m_1v2Present.SetPullMode(GPIOPin::PULL_DOWN);

	memset(&m_descriptor, 0xff, sizeof(m_descriptor));
}

/**
	@brief Poll for socket insertion/removal
 */
void SocketDetectionTask::Iteration()
{
	//call base class to do timer stuff
	TimerTask::Iteration();

	//Figure out what channels are present
	etl::vector<channelid_t, 4> channelsPresent;
	if(m_3v3Present)
		channelsPresent.push_back(CHANNEL_3V3);
	if(m_2v5Present)
		channelsPresent.push_back(CHANNEL_2V5);
	if(m_1v8Present)
		channelsPresent.push_back(CHANNEL_1V8);
	if(m_1v2Present)
		channelsPresent.push_back(CHANNEL_1V2);

	//Detect and report conflicts
	if( (channelsPresent.size() > 1) && (m_activeChannel != CHANNEL_CONFLICT) )
	{
		OnConflict(channelsPresent[0], channelsPresent[1]);
		return;
	}

	//If we do not have an active channel, and one was added, handle that
	if( (m_activeChannel == CHANNEL_NONE) && !channelsPresent.empty())
		OnInsert(channelsPresent[0]);

	//If we had an active channel, and it was removed, handle that
	else if( (m_activeChannel != CHANNEL_NONE) && channelsPresent.empty())
		OnRemove();

	//If we were conflicted, but now only have one channel, handle that
	else if( (m_activeChannel == CHANNEL_CONFLICT) && (channelsPresent.size() == 1) )
		Redetect();
}

/**
	@brief Handle socket conflict
 */
void SocketDetectionTask::OnConflict(channelid_t chan1, channelid_t chan2)
{
	g_log(Logger::ERROR, "DUT socket detected on channel %s and %s simultaneously. Only one socket should be used at a time.\n",
		GetNameOfChannel(chan1), GetNameOfChannel(chan2) );

	m_activeChannel = CHANNEL_CONFLICT;

	//Turn all other LEDs off
	for(int i=0; i<4; i++)
		FRGBLED.framebuffer[2 + i] = 0x000000;

	//Set LEDs for the offending channels red
	FRGBLED.framebuffer[2 + (int)chan1] = RGB_RED;
	FRGBLED.framebuffer[2 + (int)chan2] = RGB_RED;

	m_cacheKey ++;
}

/**
	@brief Handle socket insertion
 */
void SocketDetectionTask::OnInsert(channelid_t chan)
{
	g_log("DUT socket detected on channel %s\n", GetNameOfChannel(chan) );
	m_activeChannel = chan;

	//Turn all other LEDs off
	for(int i=0; i<4; i++)
		FRGBLED.framebuffer[2 + i] = 0x000000;

	//Set the LED for this channel to yellow (detecting)
	FRGBLED.framebuffer[2 + (int)chan] = RGB_YELLOW;

	//Wait 1 sec before detecting to make sure that the I2C bus has made contact too
	m_detectPending = true;
	Restart();

	m_cacheKey ++;
}

/**
	@brief Handle socket removal
 */
void SocketDetectionTask::OnRemove()
{
	g_log("DUT socket removed\n");

	//No channel active
	m_activeChannel = CHANNEL_NONE;
	m_detectPending = false;

	//Turn all channel LEDs off
	for(int i=0; i<4; i++)
		FRGBLED.framebuffer[2 + i] = RGB_OFF;

	m_cacheKey ++;
}

/**
	@brief Return name of a channel
 */
const char* SocketDetectionTask::GetNameOfChannel(channelid_t chan)
{
	switch(chan)
	{
		case CHANNEL_3V3:	return "3V3";
		case CHANNEL_2V5:	return "2V5";
		case CHANNEL_1V8:	return "1V8";
		case CHANNEL_1V2:	return "1V2";
		default:			return "None";
	}
}

/**
	@brief Timer handler
 */
void SocketDetectionTask::OnTimer()
{
	if(m_detectPending)
	{
		g_log("Identifying socket on channel %s\n", GetNameOfChannel(m_activeChannel));
		LogIndenter li(g_log);

		bool socketOK = ReadEEPROM();

		//Once detected, turn red if bad/blank EEPROM or green if IDed
		m_detectPending = false;
		if(socketOK)
			FRGBLED.framebuffer[2 + m_activeChannel] = RGB_GREEN;
		else
			FRGBLED.framebuffer[2 + m_activeChannel] = RGB_RED;
	}
}

/**
	@brief Read the EEPROM on the socket and return true if valid, false if not
 */
bool SocketDetectionTask::ReadEEPROM()
{
	//24C02 is 2 kB = 256 bytes, single byte address. Read the descriptor from it
	g_macI2C.BlockingWrite8(g_socketIdEepromAddr, 0x00);
	if(!g_macI2C.BlockingRead(g_socketIdEepromAddr, (uint8_t*)&m_descriptor, sizeof(m_descriptor)))
	{
		g_log(Logger::ERROR, "Failed to read ID EEPROM (damaged or improperly mated socket adapter?)\n");
		return false;
	}

	//If the descriptor major version is 0xFF, eeprom is blank
	if(m_descriptor.fmtMajor == 0xff)
	{
		g_log(Logger::WARNING, "ID EEPROM is blank or corrupted, cannot detect socket. Use console to reprogram it.\n");
		return false;
	}

	//Validate version info
	if(m_descriptor.fmtMajor != 1)
	{
		g_log(Logger::WARNING, "ID EEPROM has header version %d, expected 1\n", m_descriptor.fmtMajor);
		return false;
	}

	//If we get here it's valid
	g_log("EEPROM version %d.%d, s/n %02x%02x%02x%02x%02x%02x%02x%02x, type %s\n",
		m_descriptor.fmtMajor, m_descriptor.fmtMinor,
		m_descriptor.serial[0], m_descriptor.serial[1], m_descriptor.serial[2], m_descriptor.serial[3],
		m_descriptor.serial[4], m_descriptor.serial[5], m_descriptor.serial[6], m_descriptor.serial[7],
		GetNameOfType(m_descriptor.socketType));
	return true;
}

const char* SocketDetectionTask::GetNameOfType(DutSocketType type)
{
	switch(type)
	{
		case DutSocketType::Dip8Qspi:			return "QSPI NOR flash, DIP-8 or DIP-8 DFN socket";
		case DutSocketType::ParallelNorCSP56:	return "Async NOR flash, CSPBGA-56";

		default:
			return "unknown or unprogrammed";
	}
}
