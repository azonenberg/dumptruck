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

#ifndef SensorTask_h
#define SensorTask_h

/**
	@brief Task for polling external sensors
 */
class SensorTask : public Task
{
public:
	SensorTask()
	: m_index(0)
	, m_step(0)
	{}

	virtual void Iteration();

protected:
	int m_index;
	int m_step;

	bool ReadVoltageIteration(uint8_t i2cAddr, uint16_t& mv);
	bool ReadCurrentIteration(uint8_t i2cAddr, uint16_t& ma);

	void NextStep()
	{
		m_index ++;
		m_step = 0;
	}

	uint16_t m_codeInProgress;
};

extern uint16_t g_vvbus;
extern uint16_t g_v3v3;
extern uint16_t g_v2v5;
extern uint16_t g_v1v8;
extern uint16_t g_v1v2;
extern uint16_t g_v1v0;
extern uint16_t g_vdutvdd;
extern uint16_t g_vio33;
extern uint16_t g_vio25;
extern uint16_t g_vio18;
extern uint16_t g_vio12;

extern uint16_t g_i3v3_sb;
extern uint16_t g_ivbus;
extern uint16_t g_i3v3;
extern uint16_t g_i2v5;
extern uint16_t g_i1v8;
extern uint16_t g_i1v2;
extern uint16_t g_i1v0;
extern uint16_t g_idutvdd;
extern uint16_t g_iio33;
extern uint16_t g_iio25;
extern uint16_t g_iio18;
extern uint16_t g_iio12;

extern uint16_t g_ltcTemp;

#endif
