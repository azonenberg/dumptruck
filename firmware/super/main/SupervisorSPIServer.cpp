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

#include "supervisor.h"
#include "SupervisorSPIServer.h"

SupervisorSPIServer::SupervisorSPIServer(SPI<64, 64>& spi)
	: SPIServer(spi)
{
}

/**
	@brief Called when a SPI command arrives
 */
void SupervisorSPIServer::OnCommand(uint8_t b)
{
	/*
	m_command = b;

	//Always send two dummy bytes
	static uint8_t dummy[1] = { 0x00 };
	m_spi.NonblockingWriteFifo(dummy, 1);

	switch(m_command)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Stats readout commands

		case SUPER_REG_VERSION:
			m_spi.NonblockingWriteFifo((const uint8_t*)g_version, sizeof(g_version));
			break;

		case SUPER_REG_IBCVERSION:
			m_spi.NonblockingWriteFifo((const uint8_t*)g_ibcSwVersion, sizeof(g_ibcSwVersion));
			break;

		case SUPER_REG_IBCHWVERSION:
			m_spi.NonblockingWriteFifo((const uint8_t*)g_ibcHwVersion, sizeof(g_ibcHwVersion));
			break;

		case SUPER_REG_IBCVIN:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_vin48, sizeof(g_vin48));
			break;

		case SUPER_REG_IBCIIN:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_iin, sizeof(g_iin));
			break;

		case SUPER_REG_IBCTEMP:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_ibcTemp, sizeof(g_ibcTemp));
			break;

		case SUPER_REG_IBCVOUT:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_vout12, sizeof(g_vout12));
			break;

		case SUPER_REG_IBCIOUT:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_iout, sizeof(g_iout));
			break;

		case SUPER_REG_IBCVSENSE:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_voutsense, sizeof(g_voutsense));
			break;

		case SUPER_REG_IBCMCUTEMP:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_ibcMcuTemp, sizeof(g_ibcMcuTemp));
			break;

		case SUPER_REG_IBC3V3:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_ibc3v3, sizeof(g_ibc3v3));
			break;

		case SUPER_REG_MCUTEMP:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_mcutemp, sizeof(g_mcutemp));
			break;

		case SUPER_REG_3V3:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_3v3Voltage, sizeof(g_3v3Voltage));
			break;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Firmware update commands etc TODO

		default:
			break;
	}
	*/
}
