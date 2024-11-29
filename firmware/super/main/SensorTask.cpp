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
#include "SensorTask.h"

uint16_t g_vvbus = 0;
uint16_t g_v3v3 = 0;
uint16_t g_v2v5 = 0;
uint16_t g_v1v8 = 0;
uint16_t g_v1v2 = 0;
uint16_t g_v1v0 = 0;
uint16_t g_vdutvdd = 0;

void SensorTask::Iteration()
{
	switch(m_index)
	{
		//Update MCU temperature
		case 0:
			if(g_adc->GetTemperatureNonblocking(g_mcutemp))
				NextStep();
			break;

		//Read MCU 3.3V rail
		//Don't use the internal ADC, use the INA230 since it's way more accurate
		case 1:
			if(ReadVoltageIteration(INA_3V3_SB, g_3v3Voltage))
				NextStep();
			break;

		//Read VBUS
		case 2:
			if(ReadVoltageIteration(INA_VBUS, g_vvbus))
				NextStep();
			break;

		//Read 3V3
		case 3:
			if(ReadVoltageIteration(INA_3V3, g_v3v3))
				NextStep();
			break;

		//Read 2V5
		case 4:
			if(ReadVoltageIteration(INA_2V5, g_v2v5))
				NextStep();
			break;

		//Read 1V8
		case 5:
			if(ReadVoltageIteration(INA_1V8, g_v1v8))
				NextStep();
			break;

		//Read 1V2
		case 6:
			if(ReadVoltageIteration(INA_1V2, g_v1v2))
				NextStep();
			break;

		//Read 1V0
		case 7:
			if(ReadVoltageIteration(INA_1V0, g_v1v0))
				NextStep();
			break;

		//Rad DUT_VDD
		case 8:
			if(ReadVoltageIteration(INA_DUT_VDD, g_vdutvdd))
				NextStep();
			break;

		//return to start once we roll off the end
		default:
			m_index = 0;
			m_step = 0;
	}
};

bool SensorTask::ReadVoltageIteration(uint8_t i2cAddr, uint16_t& mv)
{
	switch(m_step)
	{
		case 0:
			g_i2c.NonblockingStart(1, i2cAddr, false);
			m_step ++;
			break;

		case 1:
			if(g_i2c.IsStartDone())
				m_step ++;
			break;

		case 2:
			g_i2c.NonblockingWrite(0x02);	//bus voltage
			m_step ++;
			break;

		case 3:
			if(g_i2c.IsWriteDone())
				m_step ++;
			break;

		case 4:
			g_i2c.NonblockingStart(2, i2cAddr, true);
			m_step ++;
			break;

		case 5:
			if(g_i2c.IsReadReady())
				m_step ++;
			break;

		case 6:
			m_codeInProgress = (g_i2c.GetReadData()) << 8;
			m_step ++;
			break;

		case 7:
			if(g_i2c.IsReadReady())
				m_step ++;
			break;

		//1.25 mV / LSB
		case 8:
			m_codeInProgress |= g_i2c.GetReadData();
			mv = static_cast<int>(1.25 * m_codeInProgress);
			return true;
	}

	return false;
}
