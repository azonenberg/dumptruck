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

#ifndef SPIFlashDumper_h
#define SPIFlashDumper_h

#include "SocketedFlashDumper.h"
#include <etl/optional.h>

/**
	@brief Dumper class for x1 SPI flash
 */
class SPIFlashDumper : public SocketedFlashDumper
{
public:
	SPIFlashDumper(channelid_t chan)
		: SocketedFlashDumper(chan)
	{
		m_dmachannel = g_mdma.AllocateChannel();
		g_log("SPIFlashDumper: Allocate MDMA channel %d\n", m_dmachannel->GetIndex());
	}

	///@brief allow std::move from this object
	SPIFlashDumper(SPIFlashDumper&& rhs)
		: SocketedFlashDumper(rhs.m_channel)
	{
		m_dmachannel = rhs.m_dmachannel;
		rhs.m_dmachannel = nullptr;
	}

	~SPIFlashDumper()
	{
		if(m_dmachannel)
		{
			g_mdma.FreeChannel(m_dmachannel);

			//only power off if we're a live object, not one that's been moved from
			PowerOff();
		}
	}

	virtual bool Init() override
	{
		//Turn power on
		if(!PowerOn())
		{
			g_log(Logger::ERROR, "Failed to enable DUT power, shorted device or socket?\n");
			return false;
		}

		//Power is stable. Set mux config to use this flash
		SetMuxConfig(IOMuxConfig::X1_SPI);

		//Create our actual flash dumper
		m_flashInterface = APB_SpiFlashInterface(GetSPI(m_channel), 2);	//Fixed PCLK/2 divider for now
		return true;
	}

	virtual uint64_t GetCapacity() override
	{ return m_flashInterface->GetCapacity(); }

	virtual uint32_t ReadFile(uint64_t offset, uint8_t* data, uint32_t len) override
	{
		m_flashInterface->ReadData(offset, data, len, m_dmachannel);
		return len;
	}

	///@brief DMA channel for reads
	MDMAChannel* m_dmachannel;

	///@brief Flash controller
	etl::optional<APB_SpiFlashInterface> m_flashInterface;
};

#endif
