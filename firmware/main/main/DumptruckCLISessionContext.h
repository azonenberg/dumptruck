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
	@brief Declaration of DumptruckCLISessionContext
 */
#ifndef DumptruckCLISessionContext_h
#define DumptruckCLISessionContext_h

#include <embedded-cli/CLIOutputStream.h>
#include <embedded-cli/CLISessionContext.h>
#include <staticnet/cli/SSHOutputStream.h>

#include "SocketDetectionTask.h"

class DumptruckCLISessionContext : public CLISessionContext
{
public:
	DumptruckCLISessionContext();

	void Initialize(int sessid, TCPTableEntry* socket, SSHTransportServer* server, const char* username)
	{
		m_sshstream.Initialize(sessid, socket, server);
		Initialize(&m_sshstream, username);
	}

	//Generic init for non-SSH streams
	void Initialize(CLIOutputStream* stream, const char* username)
	{
		m_stream = stream;
		LoadHostname();
		CLISessionContext::Initialize(m_stream, username);
	}

	SSHOutputStream* GetSSHStream()
	{ return &m_sshstream; }

	virtual void PrintPrompt();

protected:

	void LoadHostname();

	virtual void OnExecute();
	void OnExecuteRoot();

	void OnCommit();
	//void OnDFU();
	void OnEepromCommand();
	void OnEepromProgram(DutSocketType stype);

	void OnIPCommand();
	void OnIPAddress(const char* addr);
	void OnIPGateway(const char* gw);

	void OnNoCommand();
	void OnNoSSHCommand();

	void OnNtpServer(const char* addr);

	void OnReload();
	void OnRollback();
	void OnShowCommand();
	void OnShowFlash();
	void OnShowFlashDetail();
	void OnShowHardware();

	void OnShowVersion();
	void OnSSHCommand();

	SSHOutputStream m_sshstream;
	CLIOutputStream* m_stream;

	///@brief Hostname (only used for display)
	char m_hostname[33];

	void PrintPowerRail(const char* name, dsuperregs_t vreg, dsuperregs_t ireg);

	void PrintPowerRail(const char* name, superregs_t vreg, dsuperregs_t ireg)
	{ PrintPowerRail(name, static_cast<dsuperregs_t>(vreg), ireg); }
};

#endif
