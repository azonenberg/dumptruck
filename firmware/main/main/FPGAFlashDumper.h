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

#ifndef FPGAFlashDumper_h
#define FPGAFlashDumper_h

/**
	@brief Debug helper (mostly) for dumping the FPGA boot flash
 */
class FPGAFlashDumper : public FlashDumper
{
public:
	FPGAFlashDumper()
	{
		m_dmachannel = g_mdma.AllocateChannel();
		g_log("FPGAFlashDumper: Allocate MDMA channel %d\n", m_dmachannel->GetIndex());
	}

	FPGAFlashDumper(FPGAFlashDumper&& rhs)
	{
		m_dmachannel = rhs.m_dmachannel;
		rhs.m_dmachannel = nullptr;
	}

	~FPGAFlashDumper()
	{
		if(m_dmachannel)
		{
			g_log("FPGAFlashDumper: free MDMA channel %d (%08x), this = %08x\n", m_dmachannel->GetIndex(), m_dmachannel, this);
			g_mdma.FreeChannel(m_dmachannel);
		}
	}

	virtual uint64_t GetCapacity() override
	{ return g_fpgaFlash->GetCapacity(); }

	virtual uint32_t ReadFile(uint64_t offset, uint8_t* data, uint32_t len) override
	{
		g_fpgaFlash->ReadData(offset, data, len, m_dmachannel);
		return len;
	}

	MDMAChannel* m_dmachannel;
};

#endif
