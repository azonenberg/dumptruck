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

const char* g_zeroPath = "/dev/zero";
const char* g_fpgaDfuPath = "/dfu/fpga";
const char* g_3v3Path = "/socket/3v3";
const char* g_2v5Path = "/socket/2v5";
const char* g_1v8Path = "/socket/1v8";
const char* g_1v2Path = "/socket/1v2";

const uint32_t g_fpgaImageSize = 0x0040'0000;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction / destruction

DumptruckSFTPServer::DumptruckSFTPServer()
	: m_openFile(FILE_ID_NONE)
	, m_fpgaUpdater("7s100fgga484", 0x0000'0000, g_fpgaImageSize)
	, m_dumper(nullptr)
	, m_fileSize(0)
	, m_dumpCacheKey(0)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filesystem wrapper APIs

bool DumptruckSFTPServer::CreateDumper(const char* path, bool opening)
{
	//g_log("CreateDumper %s\n", path);

	//If we already have a dumper, use it until we close our file and open a new one
	//TODO: is this going to cause problems if a client drops??
	if( (m_dumper != nullptr) && (m_dumpCacheKey == g_detectionTask->GetCacheKey()) )
	{
		//Set the open file if needed
		if(opening)
		{
			if(!strcmp(path, g_fpgaDfuPath))
				m_openFile = FILE_ID_FPGA_READBACK;
			else
				m_openFile = FILE_ID_SOCKET;
		}

		return true;
	}

	//For now, just recreate the dumper every time we do a get-size request without an open handle.
	//This is wasteful, but will get the job done
	if(!strcmp(path, g_fpgaDfuPath))
	{
		g_log("Opening FPGA flash for readback\n");

		if(opening)
			m_openFile = FILE_ID_FPGA_READBACK;

		m_vdumper = std::move(FPGAFlashDumper());
		m_dumper = &etl::get<FPGAFlashDumper>(m_vdumper);
		m_fileSize = m_dumper->GetCapacity();
		return true;
	}
	else if(!strcmp(path, g_zeroPath))
	{
		if(opening)
			m_openFile = FILE_ID_ZERO;

		m_vdumper = std::move(ZeroFlashDumper());
		m_dumper = &etl::get<ZeroFlashDumper>(m_vdumper);
		m_fileSize = m_dumper->GetCapacity();
		return true;
	}
	else if(!strcmp(path, g_3v3Path))
		return CreateDumperForSocket(CHANNEL_3V3, opening);
	else if(!strcmp(path, g_2v5Path))
		return CreateDumperForSocket(CHANNEL_2V5, opening);
	else if(!strcmp(path, g_1v8Path))
		return CreateDumperForSocket(CHANNEL_1V8, opening);
	else if(!strcmp(path, g_1v2Path))
		return CreateDumperForSocket(CHANNEL_1V2, opening);
	else
		return false;
}

bool DumptruckSFTPServer::CreateDumperForSocket(channelid_t id, bool opening)
{
	if(opening)
		m_openFile = FILE_ID_SOCKET;

	m_dumpCacheKey = g_detectionTask->GetCacheKey();

	//Figure out which socket is selected and choose the appropriate dump configuration based on that
	auto socktype = g_detectionTask->GetSocketType();
	switch(socktype)
	{
		case DutSocketType::Dip8Qspi:
			//TODO: see if x1 or x4 mode etc
			return CreateDumperForSPI(id);

		default:
			g_log(Logger::ERROR, "Unknown socket type, can't dump\n");
			return false;
	}
	return true;
}

/**
	@brief Create x1 SPI flash dumper
 */
bool DumptruckSFTPServer::CreateDumperForSPI(channelid_t id)
{
	//Create the dumper
	m_vdumper = std::move(SPIFlashDumper(id));
	auto dumper = &etl::get<SPIFlashDumper>(m_vdumper);
	m_dumper = dumper;

	//Initialize it
	if(!dumper->Init())
	{
		g_log(Logger::ERROR, "Failed to enable DUT power, shorted device or socket?\n");
		m_dumper = nullptr;
		return false;
	}

	m_fileSize = m_dumper->GetCapacity();
	return true;
}

bool DumptruckSFTPServer::DoesFileExist(const char* path)
{
	//Check against all known file names
	if(	!strcmp(path, g_fpgaDfuPath) ||
		!strcmp(path, g_zeroPath) ||
		!strcmp(path, g_3v3Path) ||
		!strcmp(path, g_2v5Path) ||
		!strcmp(path, g_1v8Path) ||
		!strcmp(path, g_1v2Path) )
	{
		return true;
	}

	//no other files to match
	return false;
}

uint64_t DumptruckSFTPServer::GetFileSize(const char* path)
{
	//Make the dumper. If it fails, file is empty
	if(!CreateDumper(path, false))
		return 0;

	//Return the size calculated by the dumper
	return m_fileSize;
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

	//TODO: Sanity check that we're trying to dump a channel that actually has something there
	//or change the SFTP paths to just be /dev/socket maybe?
	//auto chan = g_detectionTask->GetActiveChannel();

	//Check if this is a socket path
	bool isSocket = false;
	bool isZero = false;
	if(!strcmp(path, g_3v3Path))
		isSocket = true;
	else if(!strcmp(path, g_2v5Path))
		isSocket = true;
	else if(!strcmp(path, g_1v8Path))
		isSocket = true;
	else if(!strcmp(path, g_1v2Path))
		isSocket = true;

	else if(!strcmp(path, g_zeroPath))
		isZero = true;

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

	//Socket files must be opened in read mode
	else if(isSocket)
	{
		//TODO: check not in write mode etc?
		if(accessMask & SFTPPacket::ACE4_READ_DATA)
			return true;
	}

	// /dev/zero must be opened in read mode
	else if(isZero)
	{
		if(accessMask & SFTPPacket::ACE4_READ_DATA)
			return true;
	}

	//If we get here, no go
	return false;
}

uint32_t DumptruckSFTPServer::OpenFile(
	const char* path,
	uint32_t accessMask,
	[[maybe_unused]] uint32_t flags)
{
	//g_log("OpenFile(%s, access=%x, flags=%x)\n", path, accessMask, flags);

	//Handle firmware updates separately
	if(accessMask & SFTPPacket::ACE4_WRITE_DATA)
	{
		if(!strcmp(path, g_fpgaDfuPath))
		{
			m_openFile = FILE_ID_FPGA_DFU;
			m_fpgaUpdater.OnDeviceOpened();
		}
	}

	//DUT sockets, for now, must be read only (we don't support writing to sockets yet)
	else if(accessMask & SFTPPacket::ACE4_READ_DATA)
		CreateDumper(path, true);

	//Return the constant handle zero for all open requests
	return 0;
}

#ifdef HAVE_ITCM
__attribute__((section(".tcmtext")))
#endif
uint32_t DumptruckSFTPServer::ReadFile(
	[[maybe_unused]] uint32_t handle,
	uint64_t offset,
	uint8_t* data,
	uint32_t len)
{
	//If start position is at or after end, we have nothing to read
	if(offset > m_fileSize)
		return 0;

	//If EOF is inside the requested block, truncate the return buffer
	if( (offset + len) >= m_fileSize)
		len = m_fileSize - offset;

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
