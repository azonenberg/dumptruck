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

#ifndef DumptruckSuperSPIRegisters_h
#define DumptruckSuperSPIRegisters_h

enum dsuperregs_t
{
	//Rail voltages starting 0x80 (in mV)
	SUPER_REG_VVBUS		= 0x80,
	SUPER_REG_V3V3		= 0x81,
	SUPER_REG_V2V5		= 0x82,
	SUPER_REG_V1V8		= 0x83,
	SUPER_REG_V1V2		= 0x84,
	SUPER_REG_V1V0		= 0x85,
	SUPER_REG_VDUTVDD	= 0x86,

	//Rail currents starting 0x90 (in mA)
	SUPER_REG_IVBUS		= 0x90,
	SUPER_REG_I3V3		= 0x91,
	SUPER_REG_I2V5		= 0x92,
	SUPER_REG_I1V8		= 0x93,
	SUPER_REG_I1V2		= 0x94,
	SUPER_REG_I1V0		= 0x95,
	SUPER_REG_IDUTVDD	= 0x96,
	SUPER_REG_I3V3_SB	= 0x97,

	//Temperatures starting 0xa0 (in 8.8 degC
	SUPER_REG_LTC_TEMP	= 0xa0
};

#endif
