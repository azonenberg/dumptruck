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
	@brief Implementation of DumptruckSFTPServer
 */
#include "dumptruck.h"
#include "DumptruckSFTPServer.h"
#include <staticnet/sftp/SFTPOpenPacket.h>

const char* g_fpgaDfuPath = "/dfu/fpga";

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction / destruction

DumptruckSFTPServer::DumptruckSFTPServer()
	: m_openFile(FILE_ID_NONE)
	, m_fpgaUpdater("7s100fgga484", 0x0000'0000, 0x0040'0000)
	, m_dumper(nullptr)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filesystem wrapper APIs

bool DumptruckSFTPServer::DoesFileExist(const char* path)
{
	if(!strcmp(path, g_fpgaDfuPath))
		return true;

	//no other files to match
	return false;
}

uint64_t DumptruckSFTPServer::GetFileSize(const char* path)
{
	//Return size of the file we're trying to read
	if(!strcmp(path, g_fpgaDfuPath))
		return 0x0040'0000;

	//default to empty
	return 0;
}

bool DumptruckSFTPServer::CanOpenFile(const char* path, uint32_t accessMask, uint32_t flags)
{
	//If we already have an open file, abort
	//(we don't support concurrent file operations)
	if(m_openFile != FILE_ID_NONE)
		return false;

	//Check if this is a DFU file path
	bool isDFU = false;
	if(!strcmp(path, g_fpgaDfuPath))
		isDFU = true;

	//DFU files must be opened in overwrite/truncate mode
	if(isDFU)
	{
		switch(flags & SFTPOpenPacket::SSH_FXF_ACCESS_DISPOSITION)
		{
			//valid modes
			case SFTPOpenPacket::SSH_FXF_CREATE_NEW:
			case SFTPOpenPacket::SSH_FXF_CREATE_TRUNCATE:
			case SFTPOpenPacket::SSH_FXF_TRUNCATE_EXISTING:
				break;

			//Allow readback for debug
			case SFTPOpenPacket::SSH_FXF_OPEN_EXISTING:
				break;

			//anything else isn't allowed
			default:
				return false;
		}

		//Special case for FPGA readback
		if( (!strcmp(path, g_fpgaDfuPath)) &&
			(accessMask & SFTPPacket::ACE4_READ_DATA) &&
			( (accessMask & SFTPPacket::ACE4_WRITE_DATA) == 0) )
		{
			return true;
		}

		//We have to request write access to do a DFU
		if( (accessMask & SFTPPacket::ACE4_WRITE_DATA) == 0)
			return false;

		//no readback allowed for the ELF binaries
		//(since they're not actually stored as ELF and we don't want to synthesize one on the fly!)
		//TODO: allow readback of bitstream binaries?
		if( (accessMask & SFTPPacket::ACE4_READ_DATA) != 0)
			return false;

		//otherwise we're good
		return true;
	}

	//If we get here, no go
	return false;
}

uint32_t DumptruckSFTPServer::OpenFile(
	const char* path,
	[[maybe_unused]] uint32_t accessMask,
	[[maybe_unused]] uint32_t flags)
{
	//g_log("OpenFile(%s, access=%x, flags=%x)\n", path, accessMask, flags);

	//For now, all of our files are stored in a single handle
	//See which one to use
	if(!strcmp(path, g_fpgaDfuPath))
	{
		if(accessMask & SFTPPacket::ACE4_WRITE_DATA)
		{
			g_log("Opening FPGA flash for DFU\n");

			m_openFile = FILE_ID_FPGA_DFU;
			m_fpgaUpdater.OnDeviceOpened();
		}
		else
		{
			g_log("Opening FPGA flash for readback\n");

			m_openFile = FILE_ID_FPGA_READBACK;
			m_vdumper = std::move(FPGAFlashDumper());
			m_dumper = &etl::get<FPGAFlashDumper>(m_vdumper);
		}
	}

	//Return the constant handle zero for all open requests
	return 0;
}

uint32_t DumptruckSFTPServer::ReadFile(
	[[maybe_unused]] uint32_t handle,
	uint64_t offset,
	uint8_t* data,
	uint32_t len)
{
	//Bound the actual read operation
	uint32_t fileSize = 0x0040'0000;

	//If start position is at or after end, we have nothing to read
	if(offset > fileSize)
		return 0;

	//If EOF is inside the requested block, truncate the return buffer
	if( (offset + len) >= fileSize)
		len = fileSize - offset;

	//Actually dump!
	if(m_dumper)
		m_dumper->ReadFile(offset, data, len);

	//bail if we have no dumper to read
	else
		return 0;

	//Return actual number of bytes read after truncation if needed
	return len;
}

void DumptruckSFTPServer::WriteFile(
	[[maybe_unused]] uint32_t handle,
	[[maybe_unused]] uint64_t offset,
	const uint8_t* data,
	uint32_t len)
{
	//Ignore handle since we only support one right now
	switch(m_openFile)
	{
		case FILE_ID_FPGA_DFU:
			m_fpgaUpdater.OnRxData(data, len);
			break;

		default:
			break;
	}
}

bool DumptruckSFTPServer::CloseFile([[maybe_unused]] uint32_t handle)
{
	switch(m_openFile)
	{
		case FILE_ID_FPGA_DFU:
			m_fpgaUpdater.OnDeviceClosed();
			break;

		default:
			break;
	}

	//always allowed, we no longer have an open file
	m_openFile = FILE_ID_NONE;
	m_vdumper = NullFlashDumper();
	m_dumper = nullptr;
	return true;
}
