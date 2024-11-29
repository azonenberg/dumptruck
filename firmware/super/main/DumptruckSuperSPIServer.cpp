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
#include "DumptruckSuperSPIServer.h"
#include "SensorTask.h"

void DumptruckSuperSPIServer::OnApplicationCommand(uint8_t b)
{
	switch(b)
	{
		case SUPER_REG_VVBUS:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_vvbus, sizeof(g_vvbus));
			break;

		case SUPER_REG_V3V3:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_v3v3, sizeof(g_v3v3));
			break;

		case SUPER_REG_V2V5:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_v2v5, sizeof(g_v2v5));
			break;

		case SUPER_REG_V1V8:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_v1v8, sizeof(g_v1v8));
			break;

		case SUPER_REG_V1V2:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_v1v2, sizeof(g_v1v2));
			break;

		case SUPER_REG_V1V0:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_v1v0, sizeof(g_v1v0));
			break;

		case SUPER_REG_VDUTVDD:
			m_spi.NonblockingWriteFifo((const uint8_t*)&g_vdutvdd, sizeof(g_vdutvdd));
			break;

		default:
			break;
	}
}
