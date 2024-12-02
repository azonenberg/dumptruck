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

#include "dumptruck.h"
#include <algorithm>
#include "DumptruckCLISessionContext.h"
#include <ctype.h>
#include <bootloader/BootloaderAPI.h>
#include <cli/CommonCommands.h>
#include <fpga/AcceleratedCryptoEngine.h>

static const char* hostname_objid = "hostname";

//List of all valid commands
enum cmdid_t
{
	//CMD_ADDRESS,
	CMD_ARP,
	CMD_AUTHORIZED,
	CMD_CACHE,
	//CMD_CLEAR,
	CMD_COMMIT,
	CMD_COMPACT,
	CMD_DETAIL,
	//CMD_DFU,
	//CMD_DHCP,
	CMD_DIP8_QSPI,
	CMD_DUMP,
	CMD_EEPROM,
	CMD_EXIT,
	CMD_FINGERPRINT,
	CMD_FLASH,
	//CMD_GATEWAY,
	CMD_HARDWARE,
	CMD_HOSTNAME,
	//CMD_IP,
	CMD_KEY,
	CMD_KEYS,
	CMD_NO,
	CMD_NTP,
	CMD_PROGRAM,
	CMD_RESCAN,
	CMD_RELOAD,
	CMD_ROLLBACK,
	//CMD_ROUTE,
	CMD_SERVER,
	CMD_SHOW,
	CMD_SSH,
	CMD_SSH_ED25519,
	//CMD_STATUS,
	CMD_USERNAME,
	//CMD_VERSION,
	CMD_ZEROIZE
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "eeprom"

static const clikeyword_t g_eepromProgramCommands[] =
{
	{"dip8qspi",	CMD_DIP8_QSPI,		nullptr,		"Socket is a dip8-qspi adapter"},
	{nullptr,		INVALID_COMMAND,	nullptr,		nullptr}
};


static const clikeyword_t g_eepromCommands[] =
{
	{"rescan",		CMD_RESCAN,			nullptr,					"Force re-read of EEPROM"},
	{"program",		CMD_PROGRAM,		g_eepromProgramCommands,	"Program socket EEPROM"},
	{nullptr,		INVALID_COMMAND,	nullptr,					nullptr}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "flash"

static const clikeyword_t g_flashCommands[] =
{
	{"compact",		CMD_COMPACT,		nullptr,	"Force a compact operation on the key-value store, even if not full"},
	{nullptr,		INVALID_COMMAND,	nullptr,	nullptr}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "hostname"

static const clikeyword_t g_hostnameCommands[] =
{
	{"<string>",	FREEFORM_TOKEN,		nullptr,	"New host name"},
	{nullptr,		INVALID_COMMAND,	nullptr,	nullptr}
};
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "ip"

static const clikeyword_t g_ipAddressCommands[] =
{
	{"dhcp",		CMD_DHCP,			nullptr,				"Use DHCP for IP configuration"},
	{"<address>",	FREEFORM_TOKEN,		nullptr,				"New IPv4 address and subnet mask in x.x.x.x/yy format"},
	{nullptr,		INVALID_COMMAND,	nullptr,				nullptr}
};

static const clikeyword_t g_ipGatewayCommands[] =
{
	{"<address>",	FREEFORM_TOKEN,		nullptr,				"New IPv4 default gateway"},
	{nullptr,		INVALID_COMMAND,	nullptr,				nullptr}
};

static const clikeyword_t g_ipCommands[] =
{
	{"address",		CMD_ADDRESS,		g_ipAddressCommands,	"Set the IPv4 address of the device"},
	{"gateway",		CMD_GATEWAY,		g_ipGatewayCommands,	"Set the IPv4 default gateway of the device"},

	{nullptr,		INVALID_COMMAND,	nullptr,				nullptr}
};
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "no"

static const clikeyword_t g_noSshKeyCommands[] =
{
	{"<slot>",			FREEFORM_TOKEN,			nullptr,					"Slot number of the authorized SSH key to delete"},
	{nullptr,			INVALID_COMMAND,		nullptr,					nullptr}
};

static const clikeyword_t g_noSshCommands[] =
{
	{"key",				CMD_KEY,				g_noSshKeyCommands,			"Remove authorized SSH keys"},

	{nullptr,			INVALID_COMMAND,		nullptr,					nullptr}
};

static const clikeyword_t g_noFlashCommands[] =
{
	{"<key>",			FREEFORM_TOKEN,			nullptr,					"Key of the flash object to delete"},
	{nullptr,			INVALID_COMMAND,		nullptr,					nullptr}
};

static const clikeyword_t g_noCommands[] =
{
	{"flash",			CMD_FLASH,				g_noFlashCommands,			"Deletes objects from flash"},
	{"ntp",				CMD_NTP,				nullptr,					"Disables the NTP client"},
	{"ssh",				CMD_SSH,				g_noSshCommands,			"Remove authorized SSH keys"},

	{nullptr,			INVALID_COMMAND,		nullptr,					nullptr}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "show"

static const clikeyword_t g_showArpCommands[] =
{
	{"cache",			CMD_CACHE,				nullptr,				"Show contents of the ARP cache"},
	{nullptr,			INVALID_COMMAND,		nullptr,				nullptr}
};
/*
static const clikeyword_t g_showIpCommands[] =
{
	{"address",			CMD_ADDRESS,			nullptr,				"Show the IPv4 address and subnet mask"},
	{"route",			CMD_ROUTE,				nullptr,				"Show the IPv4 routing table"},
	{nullptr,			INVALID_COMMAND,		nullptr,				nullptr}
};
*/
static const clikeyword_t g_showSshAuthorized[] =
{
	{"keys",			CMD_KEYS,				nullptr,				"Show authorized keys"},
	{nullptr,			INVALID_COMMAND,		nullptr,				nullptr}
};

static const clikeyword_t g_showSshCommands[] =
{
	{"authorized",		CMD_AUTHORIZED,			g_showSshAuthorized,	"Show authorized keys"},
	{"fingerprint",		CMD_FINGERPRINT,		nullptr,				"Show the SSH host key fingerprint (in OpenSSH base64 SHA256 format)"},
	{nullptr,			INVALID_COMMAND,		nullptr,				nullptr}
};

static const clikeyword_t g_showFlashDetailCommands[] =
{
	{"<objname>",		FREEFORM_TOKEN,		nullptr,					"Name of the flash object to display"},
	{nullptr,			INVALID_COMMAND,	nullptr,					nullptr}
};

static const clikeyword_t g_showFlashCommands[] =
{
	{"<cr>",			OPTIONAL_TOKEN,		nullptr,					""},
	{"detail",			CMD_DETAIL,			g_showFlashDetailCommands,	"Show detailed flash object contents"},
	{nullptr,			INVALID_COMMAND,	nullptr,					nullptr}
};

static const clikeyword_t g_showCommands[] =
{
	{"arp",				CMD_ARP,			g_showArpCommands,			"Print ARP information"},
	{"flash",			CMD_FLASH,			g_showFlashCommands,		"Display flash usage and log data"},
	{"hardware",		CMD_HARDWARE,		nullptr,					"Print hardware information"},
	//{"ip",				CMD_IP,				g_showIpCommands,			"Print IPv4 information"},
	//{"ntp",				CMD_NTP,			nullptr,					"Print NTP information"},
	{"ssh",				CMD_SSH,			g_showSshCommands,			"Print SSH information"},
	//{"version",			CMD_VERSION,		nullptr,					"Show firmware / FPGA version"},
	{nullptr,			INVALID_COMMAND,	nullptr,					nullptr}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "ssh"

static const clikeyword_t g_sshCommandsDescription[] =
{
	{"<description>",	FREEFORM_TOKEN,			nullptr,					"Description of key (typically user@host)"},
	{nullptr,			INVALID_COMMAND,		nullptr,					nullptr}
};

static const clikeyword_t g_sshCommandsBlob[] =
{
	{"<blob>",			FREEFORM_TOKEN,			g_sshCommandsDescription,	"Base64 encoded public key blob"},
	{nullptr,			INVALID_COMMAND,		nullptr,					nullptr}
};

static const clikeyword_t g_sshCommandsType[] =
{
	{"ssh-ed25519",		CMD_SSH_ED25519,		g_sshCommandsBlob,			"Ed25519 public key"},
	{nullptr,			INVALID_COMMAND,		nullptr,					nullptr}
};

static const clikeyword_t g_sshCommandsUsername[] =
{
	{"<username>",		FREEFORM_TOKEN,			nullptr,					"SSH username"},
	{nullptr,			INVALID_COMMAND,		nullptr,					nullptr}
};

static const clikeyword_t g_sshCommands[] =
{
	{"key",				CMD_KEY,				g_sshCommandsType,			"Authorize a new SSH public key"},
	{"username",		CMD_USERNAME,			g_sshCommandsUsername,		"Sets the SSH username"},
	{nullptr,			INVALID_COMMAND,		nullptr,					nullptr}
};
/*
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "ntp"

static const clikeyword_t g_ntpServerCommands[] =
{
	{"<ip>",			FREEFORM_TOKEN,			nullptr,					"IP address of NTP server to use"},
	{nullptr,			INVALID_COMMAND,		nullptr,					nullptr}
};

static const clikeyword_t g_ntpCommands[] =
{
	{"server",			CMD_SERVER,				g_ntpServerCommands,		"Sets the NTP server to use"},
	{nullptr,			INVALID_COMMAND,		nullptr,					nullptr}
};
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// zeroize

static const clikeyword_t g_zeroizeCommands[] =
{
	{"all",				FREEFORM_TOKEN,			nullptr,				"Confirm erasing all flash data and return to default state"},
	{nullptr,			INVALID_COMMAND,		nullptr,				nullptr}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Top level command lists

//Top level commands in root mode
static const clikeyword_t g_rootCommands[] =
{
	//{"clear",		CMD_CLEAR,			g_clearCommands,		"Clear performance counters and other debugging state"},
	{"commit",		CMD_COMMIT,			nullptr,				"Commit volatile config changes to flash memory"},
	//{"dfu",			CMD_DFU,			nullptr,				"Reboot in DFU mode for firmware updating the main CPU"},
	{"dump",		CMD_DUMP,			nullptr,				"Dump flash DEV ONLY TEST"},
	{"eeprom",		CMD_EEPROM,			g_eepromCommands,		"Program or dump socket EEPROM"},
	{"exit",		CMD_EXIT,			nullptr,				"Log out"},
	{"flash",		CMD_FLASH,			g_flashCommands,		"Maintenance operations on flash"},
	{"hostname",	CMD_HOSTNAME,		g_hostnameCommands,		"Change the host name"},
	//{"ip",			CMD_IP,				g_ipCommands,			"Configure IP addresses"},
	{"no",			CMD_NO,				g_noCommands,			"Remove or disable features"},
	//{"ntp",			CMD_NTP,			g_ntpCommands,			"Configure NTP client"},
	{"reload",		CMD_RELOAD,			nullptr,				"Restart the system"},
	{"rollback",	CMD_ROLLBACK,		nullptr,				"Revert changes made since last commit"},
	{"show",		CMD_SHOW,			g_showCommands,			"Print information"},
	{"ssh",			CMD_SSH,			g_sshCommands,			"Configure SSH protocol"},
	{"zeroize",		CMD_ZEROIZE,		g_zeroizeCommands,		"Erase all configuration data and reload"},
	{nullptr,		INVALID_COMMAND,	nullptr,				nullptr}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main CLI code

DumptruckCLISessionContext::DumptruckCLISessionContext()
	: CLISessionContext(g_rootCommands)
{
	//cannot LoadHostname or do anything else touching the KVS here because the local console session is a global,
	//and is initialized before App_Init() calls InitKVS()
}

void DumptruckCLISessionContext::PrintPrompt()
{
	if(m_rootCommands == g_rootCommands)
		m_stream->Printf("%s@%s# ", m_username, m_hostname);
	m_stream->Flush();
}

void DumptruckCLISessionContext::LoadHostname()
{
	memset(m_hostname, 0, sizeof(m_hostname));

	//Read hostname, set to default value if not found
	auto hlog = g_kvs->FindObject(hostname_objid);
	if(hlog)
		strncpy(m_hostname, (const char*)g_kvs->MapObject(hlog), std::min((size_t)hlog->m_len, sizeof(m_hostname)-1));
	else
		strncpy(m_hostname, "dumptruck", sizeof(m_hostname)-1);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Top level command dispatcher

void DumptruckCLISessionContext::OnExecute()
{
	if(m_rootCommands == g_rootCommands)
		OnExecuteRoot();

	//TODO other modes

	m_stream->Flush();
}

/**
	@brief Execute a command in config mode
 */
void DumptruckCLISessionContext::OnExecuteRoot()
{
	switch(m_command[0].m_commandID)
	{
		case CMD_COMMIT:
			OnCommit();
			break;
		/*
		case CMD_DFU:
			{
				//TODO: require confirmation or something
				RTC::Unlock();
				g_bbram->m_state = STATE_DFU;
				asm("dmb st");
				RTC::Lock();
				Reset();
			}
			break;
		*/

		case CMD_DUMP:
			OnDump();
			break;

		case CMD_EEPROM:
			OnEepromCommand();
			break;

		case CMD_EXIT:
			m_stream->Flush();

			//SSH session? Close the socket
			if(m_stream == &m_sshstream)
				m_sshstream.GetServer()->GracefulDisconnect(m_sshstream.GetSessionID(), m_sshstream.GetSocket());

			//Local console? Nothing needed on real hardware (TODO logout)
			else
			{
			}
			break;

		case CMD_FLASH:
			if(m_command[1].m_commandID == CMD_COMPACT)
			{
				if(!g_kvs->Compact())
					g_log(Logger::ERROR, "Compaction failed\n");
			}
			break;

		case CMD_HOSTNAME:
			memset(m_hostname, 0, sizeof(m_hostname));
			strncpy(m_hostname, m_command[1].m_text, sizeof(m_hostname)-1);
			break;
		/*
		case CMD_IP:
			OnIPCommand();
			break;
		*/
		case CMD_NO:
			OnNoCommand();
			break;
		/*
		case CMD_NTP:
			if(m_command[1].m_commandID == CMD_SERVER)
				OnNtpServer(m_command[2].m_text);
			break;
		*/
		case CMD_RELOAD:
			OnReload();
			break;
		/*
		case CMD_ROLLBACK:
			OnRollback();
			break;
		*/
		case CMD_SHOW:
			OnShowCommand();
			break;

		case CMD_SSH:
			OnSSHCommand();
			break;

		case CMD_ZEROIZE:
			if(!strcmp(m_command[1].m_text, "all"))
			{
				g_kvs->WipeAll();
				OnReload();
			}
			break;

		default:
			m_stream->Printf("Unrecognized command\n");
			break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "commit"

void DumptruckCLISessionContext::OnCommit()
{
	//Save hostname and SSH username
	if(!g_kvs->StoreStringObjectIfNecessary(hostname_objid, m_hostname, "dumptruck"))
		m_stream->Printf("KVS write error\n");

	if(!g_kvs->StoreStringObjectIfNecessary(g_usernameObjectID, g_sshUsername, g_defaultSshUsername))
		m_stream->Printf("KVS write error\n");

	//Save SSH authorized key list
	g_keyMgr.CommitToKVS();

	//Save service configuration
	//g_dhcpClient->SaveConfigToKVS();
	//g_ntpClient->SaveConfigToKVS();

	//Save IP configuration
	if(!g_kvs->StoreObjectIfNecessary<IPv4Address>(g_ipConfig.m_address, g_defaultIP, "ip.address"))
		m_stream->Printf("KVS write error\n");
	if(!g_kvs->StoreObjectIfNecessary<IPv4Address>(g_ipConfig.m_netmask, g_defaultNetmask, "ip.netmask"))
		m_stream->Printf("KVS write error\n");
	if(!g_kvs->StoreObjectIfNecessary<IPv4Address>(g_ipConfig.m_broadcast, g_defaultBroadcast, "ip.broadcast"))
		m_stream->Printf("KVS write error\n");
	if(!g_kvs->StoreObjectIfNecessary<IPv4Address>(g_ipConfig.m_gateway, g_defaultGateway, "ip.gateway"))
		m_stream->Printf("KVS write error\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "eeprom"

void DumptruckCLISessionContext::OnEepromCommand()
{
	switch(m_command[1].m_commandID)
	{
		//Program the EEPROM
		case CMD_PROGRAM:
			switch(m_command[2].m_commandID)
			{
				case CMD_DIP8_QSPI:
					OnEepromProgram(DutSocketType::Dip8Qspi);
					break;

				default:
					break;
			}
			break;

		//Re-read the EEPROM
		case CMD_RESCAN:
			g_detectionTask->Redetect();
			break;

		default:
			break;
	}
}

void DumptruckCLISessionContext::OnEepromProgram(DutSocketType stype)
{
	//Fill out the descriptor
	DutSocketDescriptor desc;
	desc.fmtMajor = 1;
	desc.fmtMinor = 0;
	desc.socketType = stype;

	//Generate a serial number
	AcceleratedCryptoEngine engine;
	engine.GenerateRandom(desc.serial, sizeof(desc.serial));

	//Write it to the EEPROM
	//For now, write in a single block (up to 16 bytes can be done per block)
	uint8_t writeData[sizeof(desc) + 1];
	writeData[0] = 0x00;	//address
	memcpy(&writeData[1], &desc, sizeof(desc));
	g_macI2C.BlockingWrite(g_socketIdEepromAddr, writeData, sizeof(writeData));

	//Ping until the write completes
	auto now = g_logTimer.GetCount();
	while(g_macI2C.BlockingPing(g_socketIdEepromAddr) == 0)
	{}
	auto dt = g_logTimer.GetCount() - now;
	g_log("Write complete (took %d.%d ms)\n", dt/10, dt % 10);

	//Re-scan so we know what's mated
	g_detectionTask->Redetect();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "ip"
/*
bool DumptruckCLISessionContext::ParseIPAddress(const char* addr, IPv4Address& ip)
{
	int len = strlen(addr);

	int nfield = 0;
	unsigned int fields[4] = {0};

	//Parse
	for(int i=0; i<len; i++)
	{
		//Dot = move to next field
		if( (addr[i] == '.') && (nfield < 3) )
			nfield ++;

		//Digit = update current field
		else if(isdigit(addr[i]))
			fields[nfield] = (fields[nfield] * 10) + (addr[i] - '0');

		else
			return false;
	}

	//Validate
	if(nfield != 3)
		return false;
	for(int i=0; i<4; i++)
	{
		if(fields[i] > 255)
			return false;
	}

	//Set the IP
	for(int i=0; i<4; i++)
		ip.m_octets[i] = fields[i];
	return true;
}

bool DumptruckCLISessionContext::ParseIPAddressWithSubnet(const char* addr, IPv4Address& ip, uint32_t& mask)
{
	int len = strlen(addr);

	int nfield = 0;	//0-3 = IP, 4 = netmask
	unsigned int fields[5] = {0};

	//Parse
	for(int i=0; i<len; i++)
	{
		//Dot = move to next field
		if( (addr[i] == '.') && (nfield < 3) )
			nfield ++;

		//Slash = move to netmask
		else if( (addr[i] == '/') && (nfield == 3) )
			nfield ++;

		//Digit = update current field
		else if(isdigit(addr[i]))
			fields[nfield] = (fields[nfield] * 10) + (addr[i] - '0');

		else
			return false;
	}

	//Validate
	if(nfield != 4)
		return false;
	for(int i=0; i<4; i++)
	{
		if(fields[i] > 255)
			return false;
	}
	if( (fields[4] > 32) || (fields[4] == 0) )
		return false;

	//Set the IP
	for(int i=0; i<4; i++)
		ip.m_octets[i] = fields[i];

	mask = 0xffffffff << (32 - fields[4]);
	return true;
}

void DumptruckCLISessionContext::OnIPAddress(const char* addr)
{
	//Parse the base IP address
	uint32_t mask = 0;
	if(!ParseIPAddressWithSubnet(addr, g_ipConfig.m_address, mask))
	{
		m_stream->Printf("Usage: ip address x.x.x.x/yy\n");
		return;
	}

	//Calculate the netmask
	g_ipConfig.m_netmask.m_octets[0] = (mask >> 24) & 0xff;
	g_ipConfig.m_netmask.m_octets[1] = (mask >> 16) & 0xff;
	g_ipConfig.m_netmask.m_octets[2] = (mask >> 8) & 0xff;
	g_ipConfig.m_netmask.m_octets[3] = (mask >> 0) & 0xff;

	//Calculate the broadcast address
	for(int i=0; i<4; i++)
		g_ipConfig.m_broadcast.m_octets[i] = g_ipConfig.m_address.m_octets[i] | ~g_ipConfig.m_netmask.m_octets[i];
}

void DumptruckCLISessionContext::OnIPGateway(const char* gw)
{
	if(!ParseIPAddress(gw, g_ipConfig.m_gateway))
		m_stream->Printf("Usage: ip gateway x.x.x.x\n");
}

void DumptruckCLISessionContext::OnIPCommand()
{
	switch(m_command[1].m_commandID)
	{
		case CMD_ADDRESS:
			if(m_command[2].m_commandID == CMD_DHCP)
			{
				g_usingDHCP = true;
				g_dhcpClient->Enable();
			}
			else
			{
				g_usingDHCP = false;
				g_dhcpClient->Disable();
				OnIPAddress(m_command[2].m_text);
			}
			break;

		case CMD_GATEWAY:
			OnIPGateway(m_command[2].m_text);
			break;

		default:
			m_stream->Printf("Unrecognized command\n");
			break;
	}
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "ntp"
/*
void DumptruckCLISessionContext::OnNtpServer(const char* addr)
{
	//Parse the base IP address
	IPv4Address iaddr;
	if(!ParseIPAddress(addr, iaddr))
	{
		m_stream->Printf("Usage: ntp server x.x.x.x\n");
		return;
	}

	g_ntpClient->Enable();
	g_ntpClient->SetServerAddress(iaddr);
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "no"

void DumptruckCLISessionContext::OnNoSSHCommand()
{
	switch(m_command[2].m_commandID)
	{
		case CMD_KEY:
			g_keyMgr.RemovePublicKey(atoi(m_command[3].m_text));
			break;

		default:
			break;
	}
}

void DumptruckCLISessionContext::OnNoCommand()
{
	switch(m_command[1].m_commandID)
	{
		case CMD_FLASH:
			RemoveFlashKey(m_stream, m_command[2].m_text);
			break;

		//case CMD_NTP:
		//	g_ntpClient->Disable();
		//	break;

		case CMD_SSH:
			OnNoSSHCommand();
			break;

		default:
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "reload"

void DumptruckCLISessionContext::OnReload()
{
	//TODO: do this through the supervisor to do a whole-system reset instead of just rebooting the MCU

	g_log("Reload requested\n");
	Reset();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "rollback"

/**
	@brief Load all of our configuration from the KVS, discarding any recent changes made in the CLI
 */
void DumptruckCLISessionContext::OnRollback()
{
	g_keyMgr.LoadFromKVS(false);

	//g_dhcpClient->LoadConfigFromKVS();
	//g_ntpClient->LoadConfigFromKVS();
	ConfigureIP();
	LoadHostname();
	//g_sshd->LoadUsername();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "show"

void DumptruckCLISessionContext::OnShowCommand()
{
	switch(m_command[1].m_commandID)
	{
		case CMD_ARP:
			switch(m_command[2].m_commandID)
			{
				case CMD_CACHE:
					PrintARPCache(m_stream, g_ethProtocol);
					break;

				default:
					break;
			}
			break;

		case CMD_FLASH:
			OnShowFlash();
			break;

		case CMD_HARDWARE:
			OnShowHardware();
			break;

		/*
		case CMD_IP:
			switch(m_command[2].m_commandID)
			{
				case CMD_ADDRESS:
					OnShowIPAddress();
					break;

				case CMD_ROUTE:
					OnShowIPRoute();
					break;

				default:
					break;
			}
			break;

		case CMD_NTP:
			OnShowNtp();
			break;
		*/
		case CMD_SSH:
			switch(m_command[2].m_commandID)
			{
				case CMD_AUTHORIZED:
					OnShowSSHKeys();
					break;

				case CMD_FINGERPRINT:
					PrintSSHHostKey(m_stream);
					break;

				default:
					break;
			}
			break;
		/*
		case CMD_VERSION:
			OnShowVersion();
			break;
		*/
		default:
			m_stream->Printf("Unrecognized command\n");
			break;
	}
}

void DumptruckCLISessionContext::OnShowFlash()
{
	//No details requested? Show root dir listing
	if(m_command[2].m_commandID == OPTIONAL_TOKEN)
		PrintFlashSummary(m_stream);

	//Showing details of a single object
	else
		PrintFlashDetails(m_stream, m_command[3].m_text);
}

void DumptruckCLISessionContext::OnShowHardware()
{
	PrintProcessorInfo(m_stream);

	//m_stream->Printf("\n");
	//m_stream->Printf("Supervisor:");
	//m_stream->Printf("    %s\n", g_superVersion);

	m_stream->Printf("\n");
	m_stream->Printf("Ethernet interface mgmt0:\n");
	m_stream->Printf("    MAC address: %02x:%02x:%02x:%02x:%02x:%02x\n",
		g_macAddress[0], g_macAddress[1], g_macAddress[2], g_macAddress[3], g_macAddress[4], g_macAddress[5]);

	//TODO: refactor into common
	m_stream->Printf("\n");
	m_stream->Printf("FPGA:\n");

	uint32_t idcode = FDEVINFO.idcode;
	memcpy(g_fpgaSerial, (const void*)FDEVINFO.serial, 8);

	//Print status
	m_stream->Printf("    IDCODE:   %08x (%s rev %d)\n", idcode, GetNameOfFPGA(idcode), idcode >> 28);
	m_stream->Printf("    Serial:   %02x%02x%02x%02x%02x%02x%02x%02x\n",
		g_fpgaSerial[7], g_fpgaSerial[6], g_fpgaSerial[5], g_fpgaSerial[4],
		g_fpgaSerial[3], g_fpgaSerial[2], g_fpgaSerial[1], g_fpgaSerial[0]);

	//Read USERCODE
	g_usercode = FDEVINFO.usercode;
	m_stream->Printf("    Usercode: %08x\n", g_usercode);
	{
		LogIndenter li2(g_log);

		//Format per XAPP1232:
		//31:27 day
		//26:23 month
		//22:17 year
		//16:12 hr
		//11:6 min
		//5:0 sec
		int day = g_usercode >> 27;
		int mon = (g_usercode >> 23) & 0xf;
		int yr = 2000 + ((g_usercode >> 17) & 0x3f);
		int hr = (g_usercode >> 12) & 0x1f;
		int min = (g_usercode >> 6) & 0x3f;
		int sec = g_usercode & 0x3f;
		m_stream->Printf("    Version:  %04d-%02d-%02d %02d:%02d:%02d\n",
			yr, mon, day, hr, min, sec);
	}

	m_stream->Printf("\n");
	m_stream->Printf("Temperatures:\n");
	m_stream->Printf("    Supervisor: %uhk C\n", ReadSupervisorRegister(SUPER_REG_MCUTEMP));
	m_stream->Printf("    MCU:        %uhk C\n",  g_dts.GetTemperature());
	m_stream->Printf("    FPGA:       %uhk C\n",  FXADC.die_temp);
	m_stream->Printf("    LTC3374A:   %uhk C\n", ReadSupervisorRegister(SUPER_REG_LTC_TEMP));

	//Print sensor values
	m_stream->Printf("\n");
	m_stream->Printf("Power rails:\n");
	m_stream->Printf("    +------------+---------+---------+--------+\n");
	m_stream->Printf("    | Rail       | Voltage | Current |  Power |\n");
	m_stream->Printf("    +------------+---------+---------+--------+\n");

	PrintPowerRail("VBUS", SUPER_REG_VVBUS, SUPER_REG_IVBUS);
	PrintPowerRail("3V3_SB", SUPER_REG_3V3, SUPER_REG_I3V3_SB);
	PrintPowerRail("3V3", SUPER_REG_V3V3, SUPER_REG_I3V3);
	PrintPowerRail("2V5", SUPER_REG_V2V5, SUPER_REG_I2V5);
	PrintPowerRail("1V8", SUPER_REG_V1V8, SUPER_REG_I1V8);
	m_stream->Printf("    |     VCCAUX |   %uhk |         |        |\n", FXADC.volt_aux);
	PrintPowerRail("1V2", SUPER_REG_V1V2, SUPER_REG_I1V2);
	PrintPowerRail("1V0", SUPER_REG_V1V0, SUPER_REG_I1V0);
	m_stream->Printf("    |     VCCINT |   %uhk |         |        |\n", FXADC.volt_core);
	m_stream->Printf("    |    VCCBRAM |   %uhk |         |        |\n", FXADC.volt_ram);
	PrintPowerRail("DUT_VDD", SUPER_REG_VDUTVDD, SUPER_REG_IDUTVDD);
	PrintPowerRail("VCCIO33", SUPER_REG_VIO_33, SUPER_REG_IIO_33);
	PrintPowerRail("VCCIO25", SUPER_REG_VIO_25, SUPER_REG_IIO_25);
	PrintPowerRail("VCCIO18", SUPER_REG_VIO_18, SUPER_REG_IIO_18);
	PrintPowerRail("VCCIO12", SUPER_REG_VIO_12, SUPER_REG_IIO_12);

	m_stream->Printf("    +------------+---------+---------+-------+\n");
}

void DumptruckCLISessionContext::PrintPowerRail(const char* name, dsuperregs_t vreg, dsuperregs_t ireg)
{
	auto v = ReadSupervisorRegister(vreg);
	auto i = ReadSupervisorRegister(ireg);

	auto p = i * v / 1000;
	m_stream->Printf("    | %10s |   %d.%03d |   %d.%03d |  %d.%03d |\n",
		name, v / 1000, v % 1000, i / 1000, i % 1000, p / 1000, p % 1000);
}

/*
void DumptruckCLISessionContext::OnShowIPAddress()
{
	m_stream->Printf("IPv4 address: %d.%d.%d.%d\n",
		g_ipConfig.m_address.m_octets[0],
		g_ipConfig.m_address.m_octets[1],
		g_ipConfig.m_address.m_octets[2],
		g_ipConfig.m_address.m_octets[3]
	);

	m_stream->Printf("Subnet mask:  %d.%d.%d.%d\n",
		g_ipConfig.m_netmask.m_octets[0],
		g_ipConfig.m_netmask.m_octets[1],
		g_ipConfig.m_netmask.m_octets[2],
		g_ipConfig.m_netmask.m_octets[3]
	);

	m_stream->Printf("Broadcast:    %d.%d.%d.%d\n",
		g_ipConfig.m_broadcast.m_octets[0],
		g_ipConfig.m_broadcast.m_octets[1],
		g_ipConfig.m_broadcast.m_octets[2],
		g_ipConfig.m_broadcast.m_octets[3]
	);
}

void DumptruckCLISessionContext::OnShowIPRoute()
{
	m_stream->Printf("IPv4 routing table\n");
	m_stream->Printf("Destination     Gateway\n");
	m_stream->Printf("0.0.0.0         %d.%d.%d.%d\n",
		g_ipConfig.m_gateway.m_octets[0],
		g_ipConfig.m_gateway.m_octets[1],
		g_ipConfig.m_gateway.m_octets[2],
		g_ipConfig.m_gateway.m_octets[3]);
}

void DumptruckCLISessionContext::OnShowNtp()
{
	if(g_ntpClient->IsEnabled())
	{
		m_stream->Printf("NTP client enabled\n");
		auto ip = g_ntpClient->GetServerAddress();

		if(g_ntpClient->IsSynchronized())
		{
			tm synctime;
			uint16_t syncsub;
			g_ntpClient->GetLastSync(synctime, syncsub);

			m_stream->Printf("Last synchronized to server %d.%d.%d.%d at %04d-%02d-%02dT%02d:%02d:%02d.%04d\n",
				ip.m_octets[0], ip.m_octets[1], ip.m_octets[2], ip.m_octets[3],
				synctime.tm_year + 1900,
				synctime.tm_mon+1,
				synctime.tm_mday,
				synctime.tm_hour,
				synctime.tm_min,
				synctime.tm_sec,
				syncsub);
		}
		else
		{
			m_stream->Printf("Using server %d.%d.%d.%d (not currently synchronized)\n",
				ip.m_octets[0], ip.m_octets[1], ip.m_octets[2], ip.m_octets[3] );
		}

	}
	else
		m_stream->Printf("NTP client disabled\n");
}
*/
void DumptruckCLISessionContext::OnShowSSHKeys()
{
	m_stream->Printf("Authorized keys:\n");
	m_stream->Printf("Slot  Nickname                        Fingerprint\n");

	AcceleratedCryptoEngine tmp;
	char fingerprint[64];

	for(int i=0; i<MAX_SSH_KEYS; i++)
	{
		if(g_keyMgr.m_authorizedKeys[i].m_nickname[0] != '\0')
		{
			tmp.GetKeyFingerprint(fingerprint, sizeof(fingerprint), g_keyMgr.m_authorizedKeys[i].m_pubkey);
			m_stream->Printf("%2d    %-30s  SHA256:%s\n",
				i,
				g_keyMgr.m_authorizedKeys[i].m_nickname,
				fingerprint);
		}
	}
}

/*
void DumptruckCLISessionContext::OnShowVersion()
{
	m_stream->Printf("Trigger crossbar v0.1\n");
	m_stream->Printf("by Andrew D. Zonenberg\n");
	m_stream->Printf("\n");
	m_stream->Printf("This system is open hardware! Board design files and firmware/gateware source code are at:\n");
	m_stream->Printf("https://github.com/azonenberg/triggercrossbar\n");
	m_stream->Printf("\n");
	m_stream->Printf("Firmware compiled at %s on %s\n", __TIME__, __DATE__);
	#ifdef __GNUC__
	m_stream->Printf("Compiler: g++ %s\n", __VERSION__);
	m_stream->Printf("CLI source code last modified: %s\n", __TIMESTAMP__);
	#endif
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "ssh"

void DumptruckCLISessionContext::OnSSHCommand()
{
	switch(m_command[1].m_commandID)
	{
		case CMD_KEY:
			g_keyMgr.AddPublicKey(m_command[2].m_text, m_command[3].m_text, m_command[4].m_text);
			break;

		case CMD_USERNAME:
			//yes this can truncate, we accept that
			#pragma GCC diagnostic push
			#pragma GCC diagnostic ignored "-Wstringop-truncation"
			strncpy(g_sshUsername, m_command[2].m_text, sizeof(g_sshUsername)-1);
			#pragma GCC diagnostic pop
			break;

		default:
			m_stream->Printf("Unrecognized command\n");
			break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// "dump"

void DumptruckCLISessionContext::OnDump()
{
	m_stream->Printf("Dump test\n");

	volatile APB_IOMuxConfig* muxcfg = nullptr;
	volatile APB_SPIHostInterface* spi = nullptr;

	//Figure out which channel is selected and choose the appropriate peripherals based on that
	auto chan = g_detectionTask->GetActiveChannel();
	switch(chan)
	{
		case CHANNEL_3V3:
			muxcfg = &F3V3_MUXCFG;
			spi = &F3V3_SPI;
			break;

		case CHANNEL_2V5:
			muxcfg = &F2V5_MUXCFG;
			spi = &F2V5_SPI;
			break;

		case CHANNEL_1V8:
			muxcfg = &F1V8_MUXCFG;
			spi = &F1V8_SPI;
			break;

		case CHANNEL_1V2:
			muxcfg = &F1V2_MUXCFG;
			spi = &F1V2_SPI;
			break;

		case CHANNEL_NONE:
		default:
			m_stream->Printf("No socket present, can't dump\n");
			return;
	}

	//Figure out which socket is selected and choose the appropriate dump configuration based on that
	auto socktype = g_detectionTask->GetSocketType();
	switch(socktype)
	{
		case DutSocketType::Dip8Qspi:
			OnDumpQspi(muxcfg, spi);
			break;

		default:
			m_stream->Printf("Unknown socket type, can't dump\n");
			break;
	}
}

void DumptruckCLISessionContext::OnDumpQspi(volatile APB_IOMuxConfig* muxcfg, volatile APB_SPIHostInterface* spi)
{
	auto chan = g_detectionTask->GetActiveChannel();

	g_log("Dumping QSPI flash on channel %s\n",
		SocketDetectionTask::GetNameOfChannel(chan));
	LogIndenter li(g_log);

	//Figure out power registers for the target channel
	dsuperregs_t regv = SUPER_REG_VIO_33;
	dsuperregs_t regi = SUPER_REG_IIO_33;
	uint16_t expectedVCCIO = 3300;
	switch(chan)
	{
		case CHANNEL_3V3:
			regv = SUPER_REG_VIO_33;
			regi = SUPER_REG_IIO_33;
			expectedVCCIO = 3300;
			break;

		case CHANNEL_2V5:
			regv = SUPER_REG_VIO_25;
			regi = SUPER_REG_IIO_25;
			expectedVCCIO = 2500;
			break;

		case CHANNEL_1V8:
			regv = SUPER_REG_VIO_18;
			regi = SUPER_REG_IIO_18;
			expectedVCCIO = 1800;
			break;

		case CHANNEL_1V2:
			regv = SUPER_REG_VIO_12;
			regi = SUPER_REG_IIO_12;
			expectedVCCIO = 1200;
			break;

		default:
			break;
	}

	//Set the LED to blue
	FRGBLED.framebuffer[2 + (int)chan] = RGB_BLUE;

	//Turn on power, then wait 50ms for it to stabilize and ADCs to sample it
	//(load switch has about a 15ms ramp)
	g_log("Turning on DUT power\n");
	SendSupervisorCommand(SUPER_REG_VDD_ON);
	SendSupervisorCommand(SUPER_REG_VCCIO_ON);
	g_logTimer.Sleep(500);

	//For some reason, we need to read and throw away one reading
	//This needs investigation...
	ReadSupervisorRegister(regv);
	ReadSupervisorRegister(regi);

	//Read and use second readings
	auto vio = ReadSupervisorRegister(regv);
	auto iio = ReadSupervisorRegister(regi);
	auto vcore = ReadSupervisorRegister(SUPER_REG_VDUTVDD);
	auto icore = ReadSupervisorRegister(SUPER_REG_IDUTVDD);
	g_log("VCCIO:  %d.%03dV, %d.%03dA \n", vio / 1000, vio % 1000, iio / 1000, iio % 1000);
	g_log("VDD:    %d.%03dV, %d.%03dA \n", vcore / 1000, vcore % 1000, icore / 1000, icore % 1000);

	//TODO: Validate VDD if we can?

	//TODO: check for overcurrent on VDD

	//Validate VCCIO is within range
	int vio_delta = vio - expectedVCCIO;
	if(abs(vio_delta) < 75)
		g_log("VCCIO delta: %d mV (OK)\n", vio_delta);
	else
	{
		g_log("VCCIO delta: %d mV (FAULT)\n", vio_delta);
		SendSupervisorCommand(SUPER_REG_VDD_OFF);
		SendSupervisorCommand(SUPER_REG_VCCIO_OFF);

		//Set the LED to red
		FRGBLED.framebuffer[2 + (int)chan] = RGB_RED;
		return;
	}

	//TODO: check for overcurrent on VCCIO

	//Power is stable. Set mux config to use this flash
	muxcfg->muxsel = (uint32_t)IOMuxConfig::X1_SPI;

	//Wait 50ms after enabling IOs before trying to talk to the flash
	g_logTimer.Sleep(50);

	//Try to talk to the flash
	APB_SpiFlashInterface flash(spi, 10);	//100 MHz PCLK = 10 MHz SCK

	//Tristate the pins when we're done
	muxcfg->muxsel = (uint32_t)IOMuxConfig::Inactive;

	//Turn off power
	SendSupervisorCommand(SUPER_REG_VDD_OFF);
	SendSupervisorCommand(SUPER_REG_VCCIO_OFF);

	//Set the LED to green
	FRGBLED.framebuffer[2 + (int)chan] = RGB_GREEN;
}
