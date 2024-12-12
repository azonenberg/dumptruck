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

#ifndef SocketedFlashDumper_h
#define SocketedFlashDumper_h

/**
	@brief Abstract base class for all flash dumpers which interface with the four DUT sockets
 */
class SocketedFlashDumper : public FlashDumper
{
public:
	SocketedFlashDumper(channelid_t chan);

	~SocketedFlashDumper()
	{}

	void SetMuxConfig(IOMuxConfig cfg)
	{
		//Set mux selector
		m_muxcfg->muxsel = (uint32_t)cfg;

		//Wait 5ms after enabling IOs before trying to talk to the flash
		g_logTimer.Sleep(50);
	}

	bool PowerOn();
	void PowerOff();

protected:

	volatile APB_IOMuxConfig* GetMuxConfig(channelid_t chan);
	volatile APB_SPIHostInterface* GetSPI(channelid_t chan);
	dsuperregs_t GetVIORegister(channelid_t chan);
	dsuperregs_t GetIIORegister(channelid_t chan);
	uint16_t GetNominalVIOMillivolts(channelid_t chan);

	///@brief Which channel the DUT is on
	channelid_t m_channel;

	///@brief Mux config for the I/O bank
	volatile APB_IOMuxConfig* m_muxcfg;
};

#endif
