/***********************************************************************************************************************
*                                                                                                                      *
* DUMPTRUCK                                                                                                            *
*                                                                                                                      *
* Copyright (c) 2023-2025 Andrew D. Zonenberg and contributors                                                         *
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

#ifdef _DEBUG

#include "supervisor.h"
#include "ITMTask.h"
#include "SensorTask.h"

void ITMTask::OnTimer()
{
	m_stream.Printf("CSV-UNIT,V,A,V,A,V,A,V,A,V,A,V,A,°C,°C\n");
	m_stream.Printf("CSV-NAME,VBUS,IBUS,V3.3,I3.3,V2.5,I2.5,V1.8,I1.8,V1.2,I1.2,V1.0,I1.0,LTC_TEMP,MCU_TEMP\n");
	m_stream.Printf("CSV-DATA,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%d.%03d,%uhk,%uhk\n",
		g_vvbus / 1000, g_vvbus % 1000,
		g_ivbus / 1000, g_ivbus % 1000,
		g_v3v3 / 1000, g_v3v3 % 1000,
		g_i3v3 / 1000, g_i3v3 % 1000,
		g_v2v5 / 1000, g_v2v5 % 1000,
		g_i2v5 / 1000, g_i2v5 % 1000,
		g_v1v8 / 1000, g_v1v8 % 1000,
		g_i1v8 / 1000, g_i1v8 % 1000,
		g_v1v2 / 1000, g_v1v2 % 1000,
		g_i1v2 / 1000, g_i1v2 % 1000,
		g_v1v0 / 1000, g_v1v0 % 1000,
		g_i1v0 / 1000, g_i1v0 % 1000,
		g_ltcTemp,
		g_mcutemp
		);
}

#endif
