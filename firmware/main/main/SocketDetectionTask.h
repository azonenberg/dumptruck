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

#ifndef SocketDetectionTask_h
#define SocketDetectionTask_h

#include <core/TimerTask.h>

extern const uint8_t g_socketIdEepromAddr;

enum class DutSocketType : uint16_t
{
	/**
		@brief DIP8 breakout using standard QSPI flash pinout

		https://github.com/azonenberg/dumptruck/tree/main/dut-breakouts/dip8-qspi
	 */
	Dip8Qspi			= 0x0000,

	/**
		@brief CSP-56 parallel NOR flash

		https://github.com/azonenberg/dumptruck/tree/main/dut-breakouts/parallel-nor-csp56
	 */
	ParallelNorCSP56	= 0x1000,

	///@brief Blank EEPROM
	Unprogrammed		= 0xffff
};

/**
	@brief Identifying information for the type of socket
 */
struct __attribute__((packed)) DutSocketDescriptor
{
	///@brief Descriptor format version, always 1
	uint8_t			fmtMajor;

	///@brief Descriptor format version, always 0
	uint8_t			fmtMinor;

	///@brief Socket type (so we know which types of memories are compatible)
	DutSocketType	socketType;

	/**
		@brief Serial number (eight random bytes)

		This gives 2^32 probability of birthday collision which is plenty given how many we intend to actually build
		(a few dozen max)
	 */
	uint8_t			serial[8];

	//TODO: checksum?
};

/**
	@brief Task for detecting which socket is installed
 */
class SocketDetectionTask : public TimerTask
{
public:
	SocketDetectionTask();

	static const char* GetNameOfChannel(channelid_t chan);
	static const char* GetNameOfType(DutSocketType type);

	void Redetect()
	{ OnRemove(); }

	channelid_t GetActiveChannel()
	{ return m_activeChannel; }

	DutSocketType GetSocketType()
	{ return m_descriptor.socketType; }

	/**
		@brief Return a cache key indicating that we are dumping the same DUT (socket has not been removed)

		If the key is unchanged, it is safe to cache memory size, etc.
	 */
	uint64_t GetCacheKey()
	{ return m_cacheKey; }

protected:
	virtual void OnTimer() override;
	virtual void Iteration() override;

	void OnInsert(channelid_t chan);
	void OnConflict(channelid_t chan1, channelid_t chan2);
	void OnRemove();

	channelid_t	m_activeChannel;

	GPIOPin m_3v3Present;
	GPIOPin m_2v5Present;
	GPIOPin m_1v8Present;
	GPIOPin m_1v2Present;

	bool m_detectPending;

	bool ReadEEPROM();

	DutSocketDescriptor m_descriptor;

	uint64_t m_cacheKey;
};

#endif
