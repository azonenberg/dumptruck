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

/**
	@file
	@brief Declaration of DumptruckSFTPServer
 */
#ifndef DumptruckSFTPServer_h
#define DumptruckSFTPServer_h

//#include "FrontPanelFirmwareUpdater.h"

#include <fpga/FPGAFirmwareUpdater.h>

class DumptruckSFTPServer : public SFTPServer
{
public:
	DumptruckSFTPServer()
		: m_openFile(FILE_ID_NONE)
		, m_fpgaUpdater("7s100fgga484", 0x0000'0000, 0x0040'0000)
	{}

	virtual bool DoesFileExist(const char* path) override;
	virtual bool CanOpenFile(const char* path, uint32_t accessMask, uint32_t flags) override;
	virtual uint32_t OpenFile(const char* path, uint32_t accessMask, uint32_t flags) override;
	virtual void WriteFile(uint32_t handle, uint64_t offset, const uint8_t* data, uint32_t len) override;
	virtual bool CloseFile(uint32_t handle) override;

protected:
	enum FileID
	{
		FILE_ID_NONE,
		FILE_ID_SUPER_DFU,
		FILE_ID_FPGA_DFU
	} m_openFile;

	//Firmware updater drivers
	FPGAFirmwareUpdater m_fpgaUpdater;
};

#endif
