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

uint16_t g_i3v3_sb = 0;
uint16_t g_ivbus = 0;
uint16_t g_i3v3 = 0;
uint16_t g_i2v5 = 0;
uint16_t g_i1v8 = 0;
uint16_t g_i1v2 = 0;
uint16_t g_i1v0 = 0;
uint16_t g_idutvdd = 0;

uint16_t g_ltcTemp = 0;

void SensorTask::Iteration()
{
	switch(m_index)
	{
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Temperatures

		//Update MCU temperature
		case 0:
			if(g_adc->GetTemperatureNonblocking(g_mcutemp))
				NextStep();
			break;

		case 1:
			g_ltcTemp = GetLTCTemp() * 256;
			NextStep();
			break;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Voltages

		//Read MCU 3.3V rail
		//Don't use the internal ADC, use the INA230 since it's way more accurate
		case 2:
			if(ReadVoltageIteration(INA_3V3_SB, g_3v3Voltage))
				NextStep();
			break;

		//Read VBUS
		case 3:
			if(ReadVoltageIteration(INA_VBUS, g_vvbus))
				NextStep();
			break;

		//Read 3V3
		case 4:
			if(ReadVoltageIteration(INA_3V3, g_v3v3))
				NextStep();
			break;

		//Read 2V5
		case 5:
			if(ReadVoltageIteration(INA_2V5, g_v2v5))
				NextStep();
			break;

		//Read 1V8
		case 6:
			if(ReadVoltageIteration(INA_1V8, g_v1v8))
				NextStep();
			break;

		//Read 1V2
		case 7:
			if(ReadVoltageIteration(INA_1V2, g_v1v2))
				NextStep();
			break;

		//Read 1V0
		case 8:
			if(ReadVoltageIteration(INA_1V0, g_v1v0))
				NextStep();
			break;

		//Read DUT_VDD
		case 9:
			if(ReadVoltageIteration(INA_DUT_VDD, g_vdutvdd))
				NextStep();
			break;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Currents

		case 10:
			if(ReadCurrentIteration(INA_3V3_SB, g_i3v3_sb))
				NextStep();
			break;

		//Read VBUS
		case 11:
			if(ReadCurrentIteration(INA_VBUS, g_ivbus))
				NextStep();
			break;

		//Read 3V3
		case 12:
			if(ReadCurrentIteration(INA_3V3, g_i3v3))
				NextStep();
			break;

		//Read 2V5
		case 13:
			if(ReadCurrentIteration(INA_2V5, g_i2v5))
				NextStep();
			break;

		//Read 1V8
		case 14:
			if(ReadCurrentIteration(INA_1V8, g_i1v8))
				NextStep();
			break;

		//Read 1V2
		case 15:
			if(ReadCurrentIteration(INA_1V2, g_i1v2))
				NextStep();
			break;

		//Read 1V0
		case 16:
			if(ReadCurrentIteration(INA_1V0, g_i1v0))
				NextStep();
			break;

		//Read DUT_VDD
		case 17:
			if(ReadCurrentIteration(INA_DUT_VDD, g_idutvdd))
				NextStep();
			break;

		//return to start once we roll off the end
		default:
			m_index = 0;
			m_step = 0;
	}
};

/**
	@brief Single step of reading a voltage from an INA230
 */
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

/**
	@brief Single step of reading a current from an INA230
 */
bool SensorTask::ReadCurrentIteration(uint8_t i2cAddr, uint16_t& ma)
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
			g_i2c.NonblockingWrite(0x01);	//shunt voltage
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

		//2.5 uV/LSB, 25 milliohm shunt
		case 8:
			{
				m_codeInProgress |= g_i2c.GetReadData();
				float vshunt = m_codeInProgress * 0.0000025;
				float ishunt = vshunt / 0.025;
				ma = ishunt * 1000;
			}
			return true;
	}

	return false;
}
