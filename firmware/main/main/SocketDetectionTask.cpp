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
{
	m_3v3Present.SetPullMode(GPIOPin::PULL_DOWN);
	m_2v5Present.SetPullMode(GPIOPin::PULL_DOWN);
	m_1v8Present.SetPullMode(GPIOPin::PULL_DOWN);
	m_1v2Present.SetPullMode(GPIOPin::PULL_DOWN);
}

/**
	@brief Poll for socket insertion/removal
 */
void SocketDetectionTask::Iteration()
{
	//call base class to do timer stuff
	TimerTask::Iteration();

	switch(m_activeChannel)
	{
		//No socket? Wait until one is mated
		case CHANNEL_NONE:
			if(m_3v3Present)
				OnInsert(CHANNEL_3V3);
			else if(m_2v5Present)
				OnInsert(CHANNEL_2V5);
			else if(m_1v8Present)
				OnInsert(CHANNEL_1V8);
			else if(m_1v2Present)
				OnInsert(CHANNEL_1V2);
			break;

		//Detect removal of currently mated socket
		case CHANNEL_3V3:
			if(!m_3v3Present)
				OnRemove();

			//If any OTHER signal is present, we have a conflict

			break;

		case CHANNEL_2V5:
			if(!m_2v5Present)
				OnRemove();
			break;

		case CHANNEL_1V8:
			if(!m_1v8Present)
				OnRemove();
			break;

		case CHANNEL_1V2:
			if(!m_1v2Present)
				OnRemove();
			break;

		default:
			break;
	}
}

/**
	@brief Handle socket conflict
 */
void SocketDetectionTask::OnConflict(channelid_t chan1, channelid_t chan2)
{
	g_log(Logger::ERROR, "DUT socket detected on channel %s and %s simultaneously. Only one socket should be used at a time.\n",
		GetNameOfChannel(chan1), GetNameOfChannel(chan2) );

	//Turn all other LEDs off
	for(int i=0; i<4; i++)
		FRGBLED.framebuffer[2 + i] = 0x000000;

	//Set LEDs for the offending channels red
	FRGBLED.framebuffer[2 + (int)chan1] = RGB_RED;
	FRGBLED.framebuffer[2 + (int)chan2] = RGB_RED;
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
	//24C02 is 2 kB = 256 bytes, single byte address. Read the first 128 bytes
	uint8_t eepromData[128];
	g_macI2C.BlockingWrite8(g_socketIdEepromAddr, 0x00);
	if(!g_macI2C.BlockingRead(g_socketIdEepromAddr, &eepromData[0], sizeof(eepromData)))
	{
		g_log(Logger::ERROR, "Failed to read ID EEPROM (damaged or improperly mated socket adapter?)\n");
		return false;
	}

	//If the descriptor major version is 0xFF, eeprom is blank
	if(eepromData[0] == 0xff)
	{
		g_log(Logger::WARNING, "ID EEPROM is blank or corrupted, cannot detect socket. Use console to reprogram it.\n");
		return false;
	}

	//Read successful. Print it out
	for(int i=0; i<128; i += 16)
	{
		g_log("%02x: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",
			i,
			eepromData[i + 0], eepromData[i + 1], eepromData[i + 2], eepromData[i + 3],
			eepromData[i + 4], eepromData[i + 5], eepromData[i + 6], eepromData[i + 7],
			eepromData[i + 8], eepromData[i + 9], eepromData[i + 10], eepromData[i + 11],
			eepromData[i + 12], eepromData[i + 13], eepromData[i + 14], eepromData[i + 15]);
	}

	return false;
}
