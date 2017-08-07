// emod_connect.cpp
// Network class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eraw.h"
#include "eloader.h"
#include "estdc.h"
#include "ereg.h"
#include "emath.h"
#include "ememory.h"
#include "efile.h"
#include "eini.h"
#include "evtimer.h"
#include "edisplay.h"
#include "esound.h"
#include "einput.h"
#include "econnect.h"
#include "edevice.h"
#include "econsole.h"
#include "edgeframe.h"

#include "extapi/netapi.h"   //Extended network API's
#include "core/edgecore.h"
#include "extra/verinfo.h"

//Edgelib bluetooth service name
#define BT_SERVICENAME "EDGELIB Serial Port"

//Prototype
unsigned long EdgeGetUIDNr(void);

/////////////////////////////////////////////////////////////////////
// ClassEConnect::Private                                          //
/////////////////////////////////////////////////////////////////////

//Split an url into a host and page to load
void ClassEConnect::SplitURL(const char *url, char *host, char *page, unsigned long urlend)
{
	bool headerfound = false;
	unsigned long ctr, hstart = 0, ulen = ClassEStd::StrLen(url);
	if (ulen > urlend)
		ulen = urlend;
	for (ctr = 0; ctr < ulen; ctr++)
	{
		if (ctr == 0 && ulen >= 7)
			if (url[0] == 'h' || url[0] == 'H')
				if (url[1] == 't' || url[1] == 'T')
					if (url[2] == 't' || url[2] == 'T')
						if (url[3] == 'p' || url[3] == 'P')
							if (url[4] == ':' && url[5] == '/' && url[6] == '/')
							{
								headerfound = true;
								ctr += 7;
							}
		if (url[ctr] == '/')
		{
			hstart = ctr;
			break;
		}
	}
	if (hstart == 0)
	{
		ClassEStd::StrCpy(page, "/");
		if (headerfound)
		{
			ClassEMemory::Copy(host, &url[7], ulen - 7);
			host[ulen - 7] = 0;
		}
		else
		{
			ClassEMemory::Copy(host, url, ulen);
			host[ulen] = 0;
		}
	}
	else
	{
		ClassEMemory::Copy(page, &url[hstart], ulen - hstart);
		page[ulen - hstart] = 0;
		if (headerfound)
		{
			ClassEMemory::Copy(host, &url[7], hstart - 7);
			host[hstart - 7] = 0;
		}
		else
		{
			ClassEMemory::Copy(host, url, hstart);
			host[hstart] = 0;
		}
	}
}

//Returns if the current character is base64
bool ClassEConnect::IsBase64(unsigned char c)
{
	return((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '+') || (c == '/'));
}

//Event handler for asynchronous events
void ClassEConnect::EventHandler(unsigned short eventtype, void *eventdata, void *userdata)
{
	ClassEConnect *net = (ClassEConnect *)userdata;
	if (eventtype == ECEV_NET_HOSTCONNECT)
	{
		char *hostaddr = (char *)eventdata;
		net->CallNetMessage(0, ENET_SERVERFOUND, ClassEStd::StrLen(hostaddr), (unsigned char *)hostaddr);
		if (net->httpcommand != NULL)
		{
			net->CallNetMessage(0, ENET_DEBUG, net->httpcmdsize, (unsigned char *)net->httpcommand);
			net->SendRaw((unsigned char *)net->httpcommand, net->httpcmdsize, 0);
			ClassEMemory::DeAlloc(net->httpcommand);
			net->httpcommand = NULL;
		}
	}
}

//Expand a string (for http commands)
void ClassEConnect::ExpandString(char *str, unsigned long &pos, const char *str2)
{
	ClassEStd::StrCpy(&str[pos], str2);
	pos += ClassEStd::StrLen(str2);
}

//Generate a boundary for multipart post data
void ClassEConnect::GenerateBoundary(char *boundary, unsigned char bsize)
{
	char vstr[20];
	unsigned char ctr;
	for (ctr = 0; ctr < bsize; ctr++)
		boundary[ctr] = '-';
	boundary[bsize] = 0;
	ClassEStd::IntToStr(vstr, ClassERTimer::Count(), 16);
	ClassEStd::StrCpy(&boundary[bsize - ClassEStd::StrLen(vstr)], vstr);
}

//Send data while checking for control characters (add an escape sequence), return it's final size
void ClassEConnect::EdgeSend(unsigned char id, const unsigned char *buffer, long buflen, long &totallen, bool simulate)
{
	if (usedprotocol == ECONN_SOCKET || usedprotocol == ECONN_BLUETOOTH)
	{
		unsigned long crc1 = 0, crc2 = 0, sendstart = 0, toskip = 0, datawritten = 0, ctr, dctr;
		unsigned char pkgbuf[6];
		if (simulate)
			totallen = buflen;
		pkgbuf[0] = 255;
		pkgbuf[1] = 250;
		pkgbuf[2] = (unsigned char)(totallen % 256);
		pkgbuf[3] = (unsigned char)(totallen / 256);
		if (!simulate)
			SendRaw(pkgbuf, 4, id);
		else
			totallen += 4;
		pkgbuf[0] = 255;
		pkgbuf[1] = 255;
		for (ctr = 0; (long)ctr < buflen; ctr++)
		{
			if (buffer[ctr] == 255 && toskip == 0)
			{
				if (ctr - sendstart > 0 && !simulate)
				{
					for (dctr = 0; dctr < ctr - sendstart; dctr++)
					{
						crc1 += buffer[sendstart + dctr] * (datawritten + 1);
						crc2 += buffer[sendstart + dctr] & 127;
						datawritten++;
					}
					SendRaw(&buffer[sendstart], ctr - sendstart, id);
					sendstart += (ctr - sendstart);
				}
				toskip = 255;
				if (buflen - ctr < 255)
					toskip = buflen - ctr;
				pkgbuf[2] = (unsigned char)toskip;
				if (!simulate)
				{
					for (dctr = 0; dctr < 3; dctr++)
					{
						crc1 += pkgbuf[dctr] * (datawritten + 1);
						crc2 += pkgbuf[dctr] & 127;
						datawritten++;
					}
					SendRaw(pkgbuf, 3, id);
				}
				else
					totallen += 3;
			}
			if (toskip > 0)
				toskip--;
		}
		if (!simulate)
		{
			for (dctr = 0; dctr < buflen - sendstart; dctr++)
			{
				crc1 += buffer[sendstart + dctr] * (datawritten + 1);
				crc2 += buffer[sendstart + dctr] & 127;
				datawritten++;
			}
			SendRaw(&buffer[sendstart], buflen - sendstart, id);
			pkgbuf[0] = (unsigned char)crc1;
			pkgbuf[1] = 127 - (unsigned char)(crc2 & 127);
			pkgbuf[2] = 255;
			pkgbuf[3] = 251;
			SendRaw(pkgbuf, 4, id);
		}
		else
			totallen += 4;
	}
}

//Send the actual ping data
void ClassEConnect::SendPingData(unsigned char id, bool request)
{
	if (usedprotocol == ECONN_SOCKET || usedprotocol == ECONN_BLUETOOTH)
	{
		unsigned char pkgbuf[10];
		unsigned char buflen = 2 + 8;
		pkgbuf[0] = 255;
		pkgbuf[1] = 250;
		pkgbuf[2] = buflen % 256;
		pkgbuf[3] = buflen / 256;
		pkgbuf[4] = 255;
		pkgbuf[5] = 252;
		pkgbuf[8] = 255;
		pkgbuf[9] = 251;
		if (request)
			pkgbuf[5] = 252;
		else
			pkgbuf[5] = 253;
		pkgbuf[6] = (unsigned char)(pkgbuf[4] + pkgbuf[5] * 2);
		pkgbuf[7] = (unsigned char)(127 - (((pkgbuf[4] & 127) + (pkgbuf[5] & 127)) & 127));
		SendRaw(pkgbuf, buflen, id);
		pingstate = EPING_WAITREPLY;
	}
}

//Log message to console
void ClassEConnect::LogStatus(const char *msg)
{
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		if (usedprotocol != ECONN_NONE)
			if (edge)
			{
				WCHAR sendmsg[256];
				ClassEStd::StrCpy(sendmsg, "net(");
				ClassEStd::StrCat(sendmsg, connectid);
				ClassEStd::StrCat(sendmsg, "): ");
				ClassEStd::StrCat(sendmsg, msg);
				edge->ecd.console->ShowMsg(sendmsg);
			}
	#endif
}

//Notify attached classes of network messages
void ClassEConnect::CallNetMessage(long id, long msg, long bufsize, unsigned char *buffer)
{
	if (capturedapi == NULL || msg == ENET_DEBUG)
	{
		edge->OnNetMessage(connectid, id, msg, bufsize, buffer);
	}
	else
		capturedapi->OnNetMessage(id, msg, bufsize, buffer);
}

//Check for pending sockets
void ClassEConnect::CheckSockets()
{
	char msgstr[128];
	if (usedprotocol == ECONN_SOCKET || usedprotocol == ECONN_BLUETOOTH)
	{
		if (opensocket)
		{
			unsigned char socketopt;
			if (isserver)
			{
				unsigned char ctr;
				socketopt = ecNetSockState(opensocket);
				if (socketopt & 1)
					for (ctr = 0; ctr < MAX_ECONCLIENTS; ctr++)
						if (!clientsocket[ctr])
						{
							char clientaddr[64];
							clientsocket[ctr] = ecNetSockAccept(opensocket, clientaddr);
							if (clientsocket[ctr])
							{
								if (edge->ecd.maincfg->netlogging)
								{
									ClassEStd::StrCpy(msgstr, "Client added: ");
									ClassEStd::StrCat(msgstr, clientaddr);
									ClassEStd::StrCat(msgstr, " (");
									ClassEStd::StrCat(msgstr, ctr);
									ClassEStd::StrCat(msgstr, ")");
									LogStatus(msgstr);
								}
								CallNetMessage(ctr + 1, ENET_ADDCLIENT, ClassEStd::StrLen(clientaddr), (unsigned char *)clientaddr);
								break;
							}
						}
				if (socketopt & 4)
					Disconnect();
				else for (ctr = 0; ctr < MAX_ECONCLIENTS; ctr++)
					if (clientsocket[ctr])
					{
						unsigned long rctr = 0;
						bool receiving;
						do
						{
							receiving = false;
							socketopt = ecNetSockState(clientsocket[ctr]);
							if (socketopt & 1)
							{
								unsigned long buflen = MAX_ECONREADBUF;
								if (!ecNetSockRecv(clientsocket[ctr], readbuf, buflen))
									socketopt |= 4;
								if (buflen > 0)
								{
									receiving = true;
									CallNetMessage(ctr + 1, ENET_RECEIVEDATA, buflen, (unsigned char *)readbuf);
									if (usedprotocol != ECONN_SOCKET && usedprotocol != ECONN_BLUETOOTH) // socket was closed
										return;
									if (edge->ecd.maincfg->netlogging)
									{
										ClassEStd::StrCpy(msgstr, "Receive data (");
										ClassEStd::StrCat(msgstr, buflen);
										ClassEStd::StrCat(msgstr, " bytes)");
										ClassEStd::StrCat(msgstr, " (");
										ClassEStd::StrCat(msgstr, ctr);
										ClassEStd::StrCat(msgstr, ")");
										LogStatus(msgstr);
									}
								}
							}
							if (socketopt & 4)
							{
								Disconnect(ctr + 1);
								break;
							}
							rctr++;
						}
						while(receiving && rctr < 64);
					}
			}
			else
			{
				unsigned long rctr = 0;
				bool receiving;
				do
				{
					receiving = false;
					socketopt = ecNetSockState(opensocket);
					if (socketopt & 1)
					{
						unsigned long buflen = MAX_ECONREADBUF;
						if (!ecNetSockRecv(opensocket, readbuf, buflen))
							socketopt |= 4;
						if (buflen > 0)
						{
							receiving = true;	
							CallNetMessage(0, ENET_RECEIVEDATA, buflen, (unsigned char *)readbuf);
							if (usedprotocol != ECONN_SOCKET && usedprotocol != ECONN_BLUETOOTH) // socket was closed
							    return;
							
							if (edge->ecd.maincfg->netlogging)
							{
								ClassEStd::StrCpy(msgstr, "Receive data (");
								ClassEStd::StrCat(msgstr, buflen);
								ClassEStd::StrCat(msgstr, " bytes)");
								LogStatus(msgstr);
							}
						}
					}
					if (socketopt & 4)
					{
						Disconnect();
						break;
					}
					rctr++;
				}
				while(receiving && rctr < 64);
			}
		}
	}
}

//Check for pending bluetooth devices
void ClassEConnect::CheckBluetoothDevices(void)
{
	if (usedprotocol == ECONN_BLUETOOTH)
	{
		unsigned char *dat;
		unsigned char result, type;
		result = (unsigned char)ecNetFetchBluetoothID(netobj, dat, type);
		if (result == 0)
		{
			unsigned char port = dat[1 + dat[0] + 1];
			EBLUETOOTH_DEVICEID deviceid;
			EBLUETOOTH_SERVICEID serviceid;
			if (port)
			{
				bool edgeservfound = false;
				char edgeservice[64];
				unsigned long startidx = 0;
				serviceid.port = port;
				ClassEStd::StrCpy(serviceid.address, (char *)&dat[startidx + 1]);
				startidx += dat[startidx] + 1;
				startidx += dat[startidx] + 1;
				ClassEStd::StrCpy(serviceid.devicename, (char *)&dat[startidx + 1]);
				startidx += dat[startidx] + 1;
				ClassEStd::StrCpy(serviceid.servicename, (char *)&dat[startidx + 1]);
				CallNetMessage(0, ENET_SERVICEFOUND, sizeof(EBLUETOOTH_SERVICEID), (unsigned char *)&serviceid);
				ClassEStd::StrCpy(edgeservice, BT_SERVICENAME);
				ClassEStd::StrCat(edgeservice, " [");
				ClassEStd::StrCat(edgeservice, EdgeGetUIDNr() & 0xFFFFFFF, 16, false);
				ClassEStd::StrCat(edgeservice, "]");
				if (!ClassEStd::StrEqual(serviceid.servicename, edgeservice, false))
				{
					ClassEStd::StrCpy(edgeservice, "Edge Serial Port");
					ClassEStd::StrCat(edgeservice, " [");
					ClassEStd::StrCat(edgeservice, EdgeGetUIDNr() & 0xFFFFFFF, 16, false);
					ClassEStd::StrCat(edgeservice, "]");
					if (ClassEStd::StrEqual(serviceid.servicename, edgeservice, false))
						edgeservfound = true;
				}
				else
					edgeservfound = true;
				if (edgeservfound)
				{
					deviceid.port = port;
					ClassEStd::StrCpy(deviceid.address, serviceid.address);
					ClassEStd::StrCpy(deviceid.name, serviceid.devicename);
					CallNetMessage(0, ENET_DEVICEFOUND, sizeof(EBLUETOOTH_DEVICEID), (unsigned char *)&deviceid);
					cursearchdevice++;
					if (cursearchdevice >= maxsearchdevice)
					{
						ecNetStopBluetoothSearch(netobj);
						CallNetMessage(0, ENET_ENDSEARCH, 0, NULL);
					}
				}
			}
			else if (!issearchingservice)
			{
				if (cursearchdevice >= maxsearchdevice)
				{
					ecNetStopBluetoothSearch(netobj);
					CallNetMessage(0, ENET_ENDSEARCH, 0, NULL);
					return;
				}
				deviceid.port = port;
				ClassEStd::StrCpy(deviceid.address, (char *)&dat[1]);
				ClassEStd::StrCpy(deviceid.name, (char *)&dat[1 + dat[0] + 1 + dat[dat[0] + 1]] + 1);
				CallNetMessage(0, ENET_DEVICEFOUND, sizeof(EBLUETOOTH_DEVICEID), (unsigned char *)&deviceid);
				cursearchdevice++;
			}
		}
		else if (result == 2)
		{
			ecNetStopBluetoothSearch(netobj);
			CallNetMessage(0, ENET_ENDSEARCH, 0, NULL);
		}
	}
}

/////////////////////////////////////////////////////////////////////
// ClassEConnect::Public                                           //
/////////////////////////////////////////////////////////////////////

//ClassEConnect: constructor
ClassEConnect::ClassEConnect(void)
{
	connectopen = false;
	edge = NULL;
	capturedapi = NULL;
	connectid = 0;
	isserver = false;
	isadvertising = false;
	tmpbton = false;
	httpcommand = NULL;
	usedprotocol = ECONN_NONE;
}

//ClassEConnect: destructor
ClassEConnect::~ClassEConnect(void)
{
	#if defined(DEVICE_GIZMONDO)
		if (tmpbton)
			ecNetBluetoothSetState(netobj, 1);
	#endif
	if (usedprotocol != ECONN_NONE)
		Cleanup();
}

//Set the connection ID nr.
void ClassEConnect::SetConnectID(unsigned char id)
{
	if (id > 15)
		id = 15;
	connectid = id;
}

//Initialize the connection protocol
ERESULT ClassEConnect::Init(unsigned short protocol, ClassEdge *pedge)
{
	#if (EDITION_SDK < 500)
		if (pedge->ecd.maincfg->netlogging)
			pedge->ecd.console->ShowMsg("! Feature not implemented in this SDK edition");
		return(E_NOFEATURE);
	#else
		if (usedprotocol != ECONN_NONE)
			Cleanup();
		if (protocol == ECONN_SOCKET || protocol == ECONN_BLUETOOTH)
		{
			int ctr;
			if (pedge != NULL)
				edge = pedge;
			else
				return(E_INVALIDPARAM);
			
			netobj = ecNetCreate(EdgeGetUIDNr() & 0xFFFFFFF);
			if (netobj == NULL)
				return(E_ERROR);
			ecNetRegisterHandler(netobj, ClassEConnect::EventHandler, this);
			pingstate = EPING_NONE;
			usedprotocol = protocol;
			for (ctr = 0; ctr < MAX_ECONCLIENTS; ctr++)
				clientsocket[ctr] = NULL;
			opensocket = NULL;
			#if defined(DEVICE_GIZMONDO)
				if (protocol == ECONN_BLUETOOTH && !tmpbton)
				{
					ecNetBluetoothSetState(netobj, 3);
					tmpbton = true;
				}
			#endif
			if (edge->ecd.maincfg->netlogging)
			{
				char msg[256];
				ClassEStd::StrCpy(msg, "Initialize (");
				if (usedprotocol == ECONN_SOCKET)
					ClassEStd::StrCat(msg, "TCP/IP");
				else
					ClassEStd::StrCat(msg, "Bluetooth");
				ClassEStd::StrCat(msg, ")");
				LogStatus(msg);
			}
			return(E_OK);
		}
		return(E_UNSUPPORTED);
	#endif
}

//Cleanup connection resources
void ClassEConnect::Cleanup(void)
{
	if (usedprotocol != ECONN_NONE)
	{
		Disconnect();
		ReleaseConnection();
		ecNetDestroy(netobj);
		netobj = NULL;
		if (httpcommand != NULL)
		{
			ClassEMemory::DeAlloc(httpcommand);
			httpcommand = NULL;
		}
		if (edge->ecd.maincfg->netlogging)
			LogStatus("Cleanup");
		edge = NULL;
		usedprotocol = ECONN_NONE;
	}
}

//Attach a netcapture API
void ClassEConnect::AttachNetCapture(ClassENetCapture *netcapture)
{
	DetachNetCapture();
	capturedapi = netcapture;
	if (capturedapi != NULL)
		capturedapi->net = this;
}

//Detach a netcapture API
void ClassEConnect::DetachNetCapture(void)
{
	if (capturedapi != NULL)
	{
		capturedapi->net = NULL;
		capturedapi = NULL;
	}
}

//Open a TCP/IP connection point
ERESULT ClassEConnect::EstablishConnection(void)
{
	if (usedprotocol == ECONN_SOCKET)
	{
		if (!connectopen)
		{
			if (!ecNetOpenInet(netobj))
				return(E_ERROR);
			connectopen = true;
		}
		return(E_OK);
	}
	return(E_TYPEMISMATCH);
}

//Close a TCP/IP connection point
void ClassEConnect::ReleaseConnection(void)
{
	if (usedprotocol == ECONN_SOCKET)
	{
		if (connectopen)
		{
			ecNetCloseInet(netobj);
			connectopen = false;
		}
	}
}

//Search for wireless devices in the area
unsigned long ClassEConnect::SearchWireless(unsigned long numdevices, bool searchservices)
{
	if (usedprotocol == ECONN_BLUETOOTH && numdevices > 0)
	{
		cursearchdevice = 0;
		maxsearchdevice = numdevices;
		issearchingservice = searchservices;
		if (ecNetStartBluetoothSearch(netobj, searchservices) == 0)
			return(1);
		else
			return(0);
	}
	else
		return(0);
}

//Stop searching for wireless devices
void ClassEConnect::StopWirelessSearch(void)
{
	if (usedprotocol == ECONN_BLUETOOTH)
		ecNetStopBluetoothSearch(netobj);
}

//Search for a host
bool ClassEConnect::SearchHost(const char *address, int port)
{
	if (usedprotocol != ECONN_SOCKET && usedprotocol != ECONN_BLUETOOTH)
		return(false);
	Disconnect();
	pingstate = EPING_NONE;
	if (usedprotocol == ECONN_SOCKET)
	{
		if (EstablishConnection() != E_OK)
			return(false);
	}
	char hostaddr[64];
	if (address == NULL)
	{
		isserver = true;
		if (usedprotocol == ECONN_SOCKET)
			opensocket = ecNetOpenSocket(netobj, 0);
		else
			opensocket = ecNetOpenSocket(netobj, 1);
		if (!opensocket)
		{
			Disconnect();
			return(false);
		}
		if (usedprotocol == ECONN_BLUETOOTH && port == 0)
			port = ecNetSockAvailablePort(opensocket);
		if (!ecNetSockBind(opensocket, hostaddr, port))
		{
			Disconnect();
			return(false);
		}
		if (!ecNetSockListen(opensocket, MAX_ECONCLIENTS))
		{
			Disconnect();
			return(false);
		}
		if (usedprotocol == ECONN_BLUETOOTH)
		{
			char edgeservice[64];
			ClassEStd::StrCpy(edgeservice, BT_SERVICENAME);
			ClassEStd::StrCat(edgeservice, " [");
			ClassEStd::StrCat(edgeservice, EdgeGetUIDNr() & 0xFFFFFFF, 16, false);
			ClassEStd::StrCat(edgeservice, "]");
			if (ecNetStartBluetoothService(netobj, opensocket, edgeservice, port) != 0)
			{
				Disconnect();
				return(false);
			}
			isadvertising = true;
		}
		ClassEStd::StrCat(hostaddr, "/");
		ClassEStd::StrCat(hostaddr, port);
		CallNetMessage(0, ENET_RUNNINGSERVER, ClassEStd::StrLen(hostaddr), (unsigned char *)hostaddr);
	}
	else
	{
		isserver = false;
		if (usedprotocol == ECONN_SOCKET)
			opensocket = ecNetOpenSocket(netobj, 0);
		else
			opensocket = ecNetOpenSocket(netobj, 1);
		if (!opensocket)
		{
			Disconnect();
			return(false);
		}
		if (!ecNetSockConnect(opensocket, address, port, hostaddr))
		{
			Disconnect();
			return(false);
		}
	}
	return(true);
}

//Disconnect a specific socket
void ClassEConnect::Disconnect(unsigned char connection)
{
	if (usedprotocol != ECONN_SOCKET && usedprotocol != ECONN_BLUETOOTH)
		return;
	char msgstr[128];
	if (connection == 0 && opensocket)
	{
		if (isserver)
			for (unsigned char ctr = 0; ctr < MAX_ECONCLIENTS; ctr++)
				Disconnect(ctr + 1);
		ecNetSockShutdown(opensocket);
		ecNetDestroySocket(netobj, opensocket);
		opensocket = NULL;
		if (isadvertising)
		{
			ecNetStopBluetoothService(netobj);
			isadvertising = false;
		}
		if (httpcommand != NULL)
		{
			ClassEMemory::DeAlloc(httpcommand);
			httpcommand = NULL;
		}
		CallNetMessage(0, ENET_CLOSEDSOCKET, 0, NULL);
		if (edge->ecd.maincfg->netlogging)
			LogStatus("Connection closed");
		return;
	}
	if (!isserver)
		return;
	if (connection > 0 && connection <= MAX_ECONCLIENTS)
		if (clientsocket[connection - 1])
		{
			ecNetSockShutdown(clientsocket[connection - 1]);
			ecNetDestroySocket(netobj, clientsocket[connection - 1]);
			clientsocket[connection - 1] = NULL;
			CallNetMessage(connection, ENET_CLOSEDSOCKET, 0, NULL);
			if (edge->ecd.maincfg->netlogging)
			{
				ClassEStd::StrCpy(msgstr, "Client disconnected (");
				ClassEStd::StrCat(msgstr, connection - 1);
				ClassEStd::StrCat(msgstr, ")");
				LogStatus(msgstr);
			}
		}
}

//Send an edge data package to a client socket
void ClassEConnect::Send(const unsigned char *buffer, long buflen, unsigned char connection)
{
	if (usedprotocol != ECONN_SOCKET && usedprotocol != ECONN_BLUETOOTH)
		return;
	if (!isserver)
		connection = 0;
	bool tosend = false;
	if (opensocket)
	{
		if (connection > 0)
		{
			if (connection <= MAX_ECONCLIENTS)
				if (clientsocket[connection - 1])
					tosend = true;
		}
		else
			tosend = true;
	}
	if (tosend)
	{
		long totalsize;
		EdgeSend(connection, buffer, buflen, totalsize, true);
		EdgeSend(connection, buffer, buflen, totalsize, false);
	}
}

//Send raw data to a client socket
void ClassEConnect::SendRaw(const unsigned char *buffer, long buflen, unsigned char connection)
{
	if (usedprotocol != ECONN_SOCKET && usedprotocol != ECONN_BLUETOOTH)
		return;
	if (!isserver)
		connection = 0;
	if (opensocket)
	{
		if (connection > 0)
		{
			if (connection <= MAX_ECONCLIENTS)
				if (clientsocket[connection - 1])
					ecNetSockSend(clientsocket[connection - 1], buffer, buflen);
		}
		else
		{
			if (isserver)
			{
				for (connection = 1; connection <= MAX_ECONCLIENTS; connection++)
					SendRaw(buffer, buflen, connection);
			}
			else
				ecNetSockSend(opensocket, buffer, buflen);
		}
	}
}

//Send an HTTP get command
bool ClassEConnect::SendHTTPGet(const char *url, bool autoencode)
{
	EHTTPRequest request(url);
	return(SendHTTPGet(&request));
}

//Send an HTTP get command
bool ClassEConnect::SendHTTPGet(EHTTPRequest *request)
{
	unsigned long ctr;
	Disconnect();
	if (httpcommand != NULL)
	{
		ClassEMemory::DeAlloc(httpcommand);
		httpcommand = NULL;
	}
	httpcommand = (char *)ClassEMemory::Alloc(8192);
	if (httpcommand == NULL)
		return(false);
	ClassEStd::StrCpy(httpcommand, "GET ");
	ClassEStd::StrCat(httpcommand, request->info.path);
	if (request->paramcount)
		ClassEStd::StrCat(httpcommand, "?");
	for (ctr = 0; ctr < request->paramcount; ctr++)
		if (request->mimetype[ctr] == NULL)
		{
			ClassEStd::StrCat(httpcommand, request->paramname[ctr]);
			ClassEStd::StrCat(httpcommand, "=");
			UrlEncode(&httpcommand[ClassEStd::StrLen(httpcommand)], (char *)request->paramvalue[ctr]);
			if (ctr < request->paramcount - 1)
				ClassEStd::StrCat(httpcommand, "&");
		}
	ClassEStd::StrCat(httpcommand, " HTTP/1.0\r\nHost: ");
	ClassEStd::StrCat(httpcommand, request->info.host);
	if (request->info.connection == EHTTP_CONNECTION_CLOSE)
		ClassEStd::StrCat(httpcommand, "\r\nConnection: close");
	else if (request->info.connection == EHTTP_CONNECTION_KEEPALIVE)
		ClassEStd::StrCat(httpcommand, "\r\nConnection: Keep-Alive");
	ClassEStd::StrCat(httpcommand, "\r\nAccept: */*\r\nUser-Agent: ");
	ClassEStd::StrCat(httpcommand, request->info.useragent);
	if (request->info.referer[0])
	{
		ClassEStd::StrCat(httpcommand, "\r\nReferer: ");
		ClassEStd::StrCat(httpcommand, request->info.referer);
	}
	if (request->info.username[0])
	{
		char strpass[1024];
		ClassEStd::StrCpy(strpass, request->info.username);
		ClassEStd::StrCat(strpass, ":");
		ClassEStd::StrCat(strpass, request->info.password);
		ClassEStd::StrCat(httpcommand, "\r\nAuthorization: Basic ");
		Base64Encode(&httpcommand[ClassEStd::StrLen(httpcommand)], strpass);
	}
	if (request->info.acceptencoding_gzip)
		ClassEStd::StrCat(httpcommand, "\r\nAccept-Encoding: gzip");
	if (request->customheader)
	{
		ClassEStd::StrCat(httpcommand, "\r\n");
		ClassEStd::StrCat(httpcommand, request->customheader);
	}
	ClassEStd::StrCat(httpcommand, "\r\n\r\n");
	httpcmdsize = ClassEStd::StrLen(httpcommand);
	return(SearchHost(request->info.host, request->info.port));
}

//Send an HTTP post command
bool ClassEConnect::SendHTTPPost(const char *url, bool autoencode)
{
	EHTTPRequest request(url);
	return(SendHTTPPost(&request));
}

//Send an HTTP post command
bool ClassEConnect::SendHTTPPost(EHTTPRequest *request)
{
	char encparams[4096];
	unsigned long ctr;
	Disconnect();
	if (httpcommand != NULL)
	{
		ClassEMemory::DeAlloc(httpcommand);
		httpcommand = NULL;
	}
	httpcommand = (char *)ClassEMemory::Alloc(8192);
	if (httpcommand == NULL)
		return(false);
	ClassEStd::StrCpy(encparams, "");
	for (ctr = 0; ctr < request->paramcount; ctr++)
		if (request->mimetype[ctr] == NULL)
		{
			ClassEStd::StrCat(encparams, request->paramname[ctr]);
			ClassEStd::StrCat(encparams, "=");
			UrlEncode(&encparams[ClassEStd::StrLen(encparams)], (char *)request->paramvalue[ctr]);
			if (ctr < request->paramcount - 1)
				ClassEStd::StrCat(encparams, "&");
		}
	ClassEStd::StrCpy(httpcommand, "POST ");
	ClassEStd::StrCat(httpcommand, request->info.path);
	ClassEStd::StrCat(httpcommand, " HTTP/1.0\r\nHost: ");
	ClassEStd::StrCat(httpcommand, request->info.host);
	ClassEStd::StrCat(httpcommand, "\r\nContent-Type: application/x-www-form-urlencoded\r\n");
	ClassEStd::StrCat(httpcommand, "Content-Length: ");
	ClassEStd::IntToStr(&httpcommand[ClassEStd::StrLen(httpcommand)], ClassEStd::StrLen(encparams), 10);
	if (request->info.connection == EHTTP_CONNECTION_CLOSE)
		ClassEStd::StrCat(httpcommand, "\r\nConnection: close");
	else if (request->info.connection == EHTTP_CONNECTION_KEEPALIVE)
		ClassEStd::StrCat(httpcommand, "\r\nConnection: Keep-Alive");
	ClassEStd::StrCat(httpcommand, "\r\nAccept: */*\r\nUser-Agent: ");
	ClassEStd::StrCat(httpcommand, request->info.useragent);
	if (request->info.referer[0])
	{
		ClassEStd::StrCat(httpcommand, "\r\nReferer: ");
		ClassEStd::StrCat(httpcommand, request->info.referer);
	}
	if (request->info.username[0])
	{
		char strpass[1024];
		ClassEStd::StrCpy(strpass, request->info.username);
		ClassEStd::StrCat(strpass, ":");
		ClassEStd::StrCat(strpass, request->info.password);
		ClassEStd::StrCat(httpcommand, "\r\nAuthorization: Basic ");
		Base64Encode(&httpcommand[ClassEStd::StrLen(httpcommand)], strpass);
	}
	if (request->info.acceptencoding_gzip)
		ClassEStd::StrCat(httpcommand, "\r\nAccept-Encoding: gzip");
	if (request->customheader)
	{
		ClassEStd::StrCat(httpcommand, "\r\n");
		ClassEStd::StrCat(httpcommand, request->customheader);
	}
	ClassEStd::StrCat(httpcommand, "\r\n\r\n");
	if (request->paramcount)
		ClassEStd::StrCat(httpcommand, encparams);
	httpcmdsize = ClassEStd::StrLen(httpcommand);
	return(SearchHost(request->info.host, request->info.port));
}

//Send an HTTP post command with multipart data
bool ClassEConnect::SendHTTPPostMultipart(EHTTPRequest *request)
{
	char boundary[256];
	unsigned long ctr, csize, contentlength = 0;
	unsigned char bsize = 40;
	Disconnect();
	if (httpcommand != NULL)
	{
		ClassEMemory::DeAlloc(httpcommand);
		httpcommand = NULL;
	}
	GenerateBoundary(boundary, bsize);
	for (ctr = 0; ctr < request->paramcount; ctr++)
	{
		contentlength += bsize + 2;
		if (request->mimetype[ctr] == NULL)
		{
			contentlength += ClassEStd::StrLen("\r\nContent-Disposition: form-data; name=  \r\n\r\n\r\n");
			contentlength += ClassEStd::StrLen(request->paramname[ctr]);
			contentlength += request->paramvaluesize[ctr];
		}
		else
		{
			contentlength += ClassEStd::StrLen("\r\nContent-Disposition: form-data; name=  ; filename=  \r\nContent-Type: \r\n\r\n\r\n");
			contentlength += ClassEStd::StrLen(request->paramname[ctr]);
			contentlength += ClassEStd::StrLen(request->paramfilename[ctr]);
			contentlength += ClassEStd::StrLen(request->mimetype[ctr]);
			contentlength += request->paramvaluesize[ctr];
		}
	}
	contentlength += bsize + 6;
	httpcommand = (char *)ClassEMemory::Alloc(4096 + contentlength);
	if (httpcommand == NULL)
		return(false);
	ClassEStd::StrCpy(httpcommand, "POST ");
	ClassEStd::StrCat(httpcommand, request->info.path);
	ClassEStd::StrCat(httpcommand, " HTTP/1.0\r\nHost: ");
	ClassEStd::StrCat(httpcommand, request->info.host);
	ClassEStd::StrCat(httpcommand, "\r\nContent-Type: multipart/form-data; boundary=");
	ClassEStd::StrCat(httpcommand, boundary);
	ClassEStd::StrCat(httpcommand, "\r\nContent-Length: ");
	ClassEStd::IntToStr(&httpcommand[ClassEStd::StrLen(httpcommand)], contentlength);
	if (request->info.connection == EHTTP_CONNECTION_CLOSE)
		ClassEStd::StrCat(httpcommand, "\r\nConnection: close");
	else if (request->info.connection == EHTTP_CONNECTION_KEEPALIVE)
		ClassEStd::StrCat(httpcommand, "\r\nConnection: Keep-Alive");
	ClassEStd::StrCat(httpcommand, "\r\nAccept: */*\r\nUser-Agent: ");
	ClassEStd::StrCat(httpcommand, request->info.useragent);
	if (request->info.referer[0])
	{
		ClassEStd::StrCat(httpcommand, "\r\nReferer: ");
		ClassEStd::StrCat(httpcommand, request->info.referer);
	}
	if (request->info.username[0])
	{
		char strpass[1024];
		ClassEStd::StrCpy(strpass, request->info.username);
		ClassEStd::StrCat(strpass, ":");
		ClassEStd::StrCat(strpass, request->info.password);
		ClassEStd::StrCat(httpcommand, "\r\nAuthorization: Basic ");
		Base64Encode(&httpcommand[ClassEStd::StrLen(httpcommand)], strpass);
	}
	if (request->info.acceptencoding_gzip)
		ClassEStd::StrCat(httpcommand, "\r\nAccept-Encoding: gzip");
	if (request->customheader)
	{
		ClassEStd::StrCat(httpcommand, "\r\n");
		ClassEStd::StrCat(httpcommand, request->customheader);
	}
	ClassEStd::StrCat(httpcommand, "\r\n\r\n");
	csize = ClassEStd::StrLen(httpcommand);
	if (request->paramcount)
	{
		for (ctr = 0; ctr < request->paramcount; ctr++)
		{
			ExpandString(httpcommand, csize, "--");
			ExpandString(httpcommand, csize, boundary);
			ExpandString(httpcommand, csize, "\r\nContent-Disposition: form-data; name=\"");
			ExpandString(httpcommand, csize, request->paramname[ctr]);
			ExpandString(httpcommand, csize, "\"");
			if (request->mimetype[ctr] != NULL)
			{
				ExpandString(httpcommand, csize, "; filename=\"");
				ExpandString(httpcommand, csize, request->paramfilename[ctr]);
				ExpandString(httpcommand, csize, "\"\r\nContent-Type: ");
				ExpandString(httpcommand, csize, request->mimetype[ctr]);
				ExpandString(httpcommand, csize, "\r\n\r\n");
			}
			else
				ExpandString(httpcommand, csize, "\r\n\r\n");
			ClassEMemory::Copy(&httpcommand[csize], request->paramvalue[ctr], request->paramvaluesize[ctr]);
			csize += request->paramvaluesize[ctr];
			ExpandString(httpcommand, csize, "\r\n");
		}
		ExpandString(httpcommand, csize, "--");
		ExpandString(httpcommand, csize, boundary);
		ExpandString(httpcommand, csize, "--\r\n");
	}
	httpcmdsize = csize;
	return(SearchHost(request->info.host, request->info.port));
}

//Request a ping
void ClassEConnect::RequestPing(unsigned char connection)
{
	if (usedprotocol != ECONN_SOCKET && usedprotocol != ECONN_BLUETOOTH)
		return;
	pingtimestamp = ClassERTimer::Count();
	SendPingData(connection, true);
}

//Respond to a ping request
void ClassEConnect::RespondPing(unsigned char connection)
{
	if (usedprotocol != ECONN_SOCKET && usedprotocol != ECONN_BLUETOOTH)
		return;
	pingtimestamp = ClassERTimer::Count();
	SendPingData(connection, false);
}

//Get the difference in time between a ping request and a ping response
unsigned long ClassEConnect::GetPingTime(void)
{
	if (usedprotocol != ECONN_SOCKET && usedprotocol != ECONN_BLUETOOTH)
		return(0);
	if (pingstate == EPING_WAITREPLY)
	{
		unsigned long currenttime = ClassERTimer::Count();
		pingstate = EPING_NONE;
		if (currenttime < pingtimestamp)
			return((0xFFFFFFFF - (pingtimestamp - currenttime) + 1) / 2);
		else
			return((currenttime - pingtimestamp) / 2);
	}
	return(0);
}

//Encode url parameters
//safe characters: -_.!~*'() , 0-9, A-Z, a-z
//http://www.faqs.org/rfcs/rfc2396.html
void ClassEConnect::UrlEncode(char *dst, const char *src)
{
	unsigned long ctr, dpos = 0, slen = ClassEStd::StrLen(src);
	for (ctr = 0; ctr < slen; ctr++)
	{
		if (src[ctr] == ' ')
			dst[dpos] = '+';
		else if ((src[ctr] >= 'a' && src[ctr] <= 'z') || (src[ctr] >= 'A' && src[ctr] <= 'Z') || (src[ctr] >= '0' && src[ctr] <= '9') || src[ctr] == '-' || src[ctr] == '_' || src[ctr] == '.' || src[ctr] == '!' || src[ctr] == '~' || src[ctr] == '*' || src[ctr] == '\'' || src[ctr] == '(' || src[ctr] == ')')
			dst[dpos] = src[ctr];
		else
		{
			dst[dpos] = '%';
			dst[dpos + 1] = '0' + src[ctr] / 16;
			dst[dpos + 2] = '0' + src[ctr] % 16;
			if (dst[dpos + 1] >= '0' + 10)
				dst[dpos + 1] = 'A' + dst[dpos + 1] - '0' - 10;
			if (dst[dpos + 2] >= '0' + 10)
				dst[dpos + 2] = 'A' + dst[dpos + 2] - '0' - 10;
			dpos += 2;
		}
		dpos++;
	}
	dst[dpos] = 0;
}

//Decode url parameters
void ClassEConnect::UrlDecode(char *dst, const char *src)
{
	unsigned long ctr, dpos = 0, slen = ClassEStd::StrLen(src);
	for (ctr = 0; ctr < slen; ctr++)
	{
		if (src[ctr] == '%')
		{
			dst[dpos] = 0;
			if (src[ctr + 1] >= 'a')
				dst[dpos] += (src[ctr + 1] - 'a' + 10) * 16;
			else if (src[ctr + 1] >= 'A')
				dst[dpos] += (src[ctr + 1] - 'A' + 10) * 16;
			else
				dst[dpos] += (src[ctr + 1] - '0') * 16;
			if (src[ctr + 2] >= 'a')
				dst[dpos] += (src[ctr + 2] - 'a' + 10);
			else if (src[ctr + 2] >= 'A')
				dst[dpos] += (src[ctr + 2] - 'A' + 10);
			else
				dst[dpos] += (src[ctr + 2] - '0');
			ctr += 2;
		}
		else if (src[ctr] == '+')
			dst[dpos] = ' ';
		else
			dst[dpos] = src[ctr];
		dpos++;
	}
	dst[dpos] = 0;
}

//Base 64 encode a string
void ClassEConnect::Base64Encode(char *dst, const char *src)
{
	char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int in_len = ClassEStd::StrLen(src);
	int i = 0;
	int j = 0;
	unsigned long dpos = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];
	while (in_len--)
	{
		char_array_3[i++] = *(src++);
		if (i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			for(i = 0; (i < 4) ; i++)
			{
				dst[dpos] = base64_chars[char_array_4[i]];
				dpos++;
			}
			i = 0;
		}
	}
	if (i)
	{
		for(j = i; j < 3; j++)
			char_array_3[j] = '\0';
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
		for (j = 0; (j < i + 1); j++)
		{
			dst[dpos] = base64_chars[char_array_4[j]];
			dpos++;
		}
		while((i++ < 3))
		{
			dst[dpos] = '=';
			dpos++;
		}
	}
	dst[dpos] = 0;
}

//Base 64 decode a string
void ClassEConnect::Base64Decode(char *dst, const char *src)
{
	char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int in_len = ClassEStd::StrLen(src);
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned long dpos = 0;
	unsigned char char_array_4[4], char_array_3[3];
	while (in_len-- && (src[in_] != '=') && IsBase64(src[in_]))
	{
		char_array_4[i++] = src[in_]; in_++;
		if (i == 4)
		{
			for (i = 0; i < 4; i++)
				char_array_4[i] = (unsigned char)ClassEStd::StrFind(base64_chars, char_array_4[i]);//base64_chars.find(char_array_4[i]);
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
			for (i = 0; (i < 3); i++)
			{
				dst[dpos] = char_array_3[i];
				dpos++;
			}
			i = 0;
		}
	}
	if (i)
	{
		for (j = i; j < 4; j++)
			char_array_4[j] = 0;
		for (j = 0; j < 4; j++)
			char_array_4[j] = (unsigned char)ClassEStd::StrFind(base64_chars, char_array_4[j]);//base64_chars.find(char_array_4[j]);
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
		for (j = 0; (j < i - 1); j++)
		{
			dst[dpos] = char_array_3[j];
			dpos++;
		}
	}
	dst[dpos] = 0;
}

/////////////////////////////////////////////////////////////////////
// ENetStack::Private                                              //
/////////////////////////////////////////////////////////////////////

//Scan the stack for edge packages, return package index + length (size 0 = no data)
void ENetStack::ScanEdge(unsigned long &pkgstart, unsigned long &pkgsize)
{
	if (rcvstack == NULL)
	{
		pkgsize = 0;
		return;
	}
	bool pkgerror, inpkg = false;
	unsigned long crc1 = 0, crc2 = 0, ctr, dctr, datacount = 0, realdatacount = 0;
	unsigned char toskip;
	do
	{
		pkgerror = false;
		for (ctr = 0; ctr < curstacksize; ctr++)
		{
			if (inpkg)
			{
				if (ctr < curstacksize - 2 && rcvstack[(stackindex + ctr) % totalstacksize] == 255 && rcvstack[(stackindex + ctr + 1) % totalstacksize] == 255 && rcvstack[(stackindex + ctr + 2) % totalstacksize] > 0)
				{
					toskip = rcvstack[(stackindex + ctr + 2) % totalstacksize];
					datacount += 2 + toskip;
					ctr += 2 + toskip;
					realdatacount += toskip - 1;
				}
				else if (ctr < curstacksize - 1 && rcvstack[(stackindex + ctr) % totalstacksize] == 255 && rcvstack[(stackindex + ctr + 1) % totalstacksize] == 251)
				{
					datacount += 2;
					realdatacount -= 2;
					if (datacount == pkgsize)
					{
						for (dctr = pkgstart + 4; dctr < stackindex + ctr - 2; dctr++)
						{
							crc1 += rcvstack[dctr % totalstacksize] * (dctr - pkgstart - 3);
							crc2 += rcvstack[dctr % totalstacksize] & 127;
						}
						crc2 = 127 - (unsigned char)(crc2 & 127);
					}
					if (datacount != pkgsize || rcvstack[(stackindex + ctr - 2) % totalstacksize] != (unsigned char)crc1 || rcvstack[(stackindex + ctr - 1) % totalstacksize] != (unsigned char)crc2)
					{
						inpkg = false;
						rcvstack[(pkgstart) % totalstacksize] = 0;
						rcvstack[(pkgstart + 1) % totalstacksize] = 0;
						rcvstack[(pkgstart + 2) % totalstacksize] = 0;
						rcvstack[(pkgstart + 3) % totalstacksize] = 0;
						pkgerror = true;
						break;
					}
					else
					{
						pkgsize = realdatacount;
						return;
					}
				}
				datacount++;
				realdatacount++;
			}
			else if (ctr < curstacksize - 3)
				if (rcvstack[(stackindex + ctr) % totalstacksize] == 255 && rcvstack[(stackindex + ctr + 1) % totalstacksize] == 250)
				{
					pkgstart = stackindex + ctr;
					pkgsize  = rcvstack[(stackindex + ctr + 2) % totalstacksize] + rcvstack[(stackindex + ctr + 3) % totalstacksize] * 256;
					if (pkgsize <= totalstacksize && pkgsize > 7)
					{
						if (ctr + pkgsize > curstacksize)
						{
							pkgsize = 0;
							return;
						}
						ctr += 3;
						datacount = 4;
						realdatacount = 0;
						inpkg = true;
					}
				}
		}
	}while(pkgerror);
	pkgsize = 0;
}

/////////////////////////////////////////////////////////////////////
// ENetStack::Public                                               //
/////////////////////////////////////////////////////////////////////

//ENetStack: Constructor
ENetStack::ENetStack(void)
{
	rcvstack = NULL;
}

//ENetStack: Destructor
ENetStack::~ENetStack(void)
{
	DeAlloc();
}

//Allocate a new stack
bool ENetStack::Alloc(unsigned long size)
{
	DeAlloc();
	rcvstack = new unsigned char[size];
	if (rcvstack == NULL)
		return(false);
	totalstacksize = size;
	curstacksize   = 0;
	stackindex     = 0;
	peekcache      = false;
	return(true);
}

//De-allocate stack memory
void ENetStack::DeAlloc(void)
{
	if (rcvstack != NULL)
		delete rcvstack;
	rcvstack = NULL;
}

//Push data onto the stack
bool ENetStack::Push(const void *buffer, unsigned long bufsize)
{
	if (rcvstack == NULL)
		return(false);
	if (curstacksize + bufsize > totalstacksize)
		return(false);
	unsigned long exbufsize = 0;
	const unsigned char *cbuffer = (const unsigned char *)buffer;
	if ((stackindex + curstacksize) % totalstacksize + bufsize > totalstacksize)
		exbufsize = (stackindex + curstacksize) % totalstacksize + bufsize - totalstacksize;
	ClassEMemory::Copy(&rcvstack[(stackindex + curstacksize) % totalstacksize], cbuffer, bufsize - exbufsize);
	if (exbufsize)
		ClassEMemory::Copy(rcvstack, &cbuffer[bufsize - exbufsize], exbufsize);
	curstacksize += bufsize;
	if (lastpeeksize == 0)
		peekcache = false;
	return(true);
}

//Check if there is data to be read from the stack
unsigned long ENetStack::Peek(void)
{
	if (rcvstack == NULL)
		return(0);
	if (peekcache)
		return(lastpeeksize);
	ScanEdge(lastpeekindex, lastpeeksize);
	peekcache = true;
	return(lastpeeksize);
}

//Pop data from the stack
unsigned char ENetStack::Pop(void *buffer, unsigned long &bufsize)
{
	if (rcvstack == NULL)
		return(E_ERROR);
	if (!peekcache)
		ScanEdge(lastpeekindex, lastpeeksize);
	if (bufsize >= lastpeeksize)
	{
		unsigned char *cbuffer = (unsigned char *)buffer;
		unsigned long ctr;
		unsigned char skipcount = 0;
		lastpeekindex += 4;
		for (ctr = 0; ctr < lastpeeksize; ctr++)
		{
			cbuffer[ctr] = rcvstack[lastpeekindex % totalstacksize];
			lastpeekindex++;
			if (cbuffer[ctr] == 255 && skipcount == 0)
			{
				if (rcvstack[lastpeekindex % totalstacksize] == 255)
				{
					skipcount = rcvstack[(lastpeekindex + 1) % totalstacksize];
					if (skipcount > 0)
					{
						cbuffer[ctr] = rcvstack[(lastpeekindex + 2) % totalstacksize];
						lastpeekindex += 3;
					}
				}
			}
			else if (skipcount > 0)
				skipcount--;
		}
		curstacksize = (stackindex + curstacksize - lastpeekindex);
		stackindex = lastpeekindex % totalstacksize;
		bufsize = lastpeeksize;
		peekcache = false;
		if (bufsize == 2)
		{
			if (cbuffer[0] == 255 && cbuffer[1] == 252)
				return(EPOP_REQUESTPING);
			if (cbuffer[0] == 255 && cbuffer[1] == 253)
				return(EPOP_RESPONDPING);
		}
		return(EPOP_RECEIVEDATA);
	}
	return(EPOP_NODATA);
}

/////////////////////////////////////////////////////////////////////
// EHTTPStack::Private                                             //
/////////////////////////////////////////////////////////////////////

//Parse the HTTP header from buffered data
void EHTTPStack::ParseHeader(void)
{
	char vstr[16];
	long spos, ctr;
	char *cbuffer = (char *)rcvstack;
	for (ctr = 0; ctr < (long)stackindex; ctr++)
		if (cbuffer[ctr] == ' ')
		{
			ClassEStd::StrCpy(vstr, &cbuffer[ctr + 1], 16);
			for (ctr = 0; ctr < 16; ctr++)
				if (vstr[ctr] < '0' || vstr[ctr] > '9' || ctr == 15)
					vstr[ctr] = 0;
			httpheader.status = ClassEStd::StrToInt(vstr);
			break;
		}
	spos = ClassEStd::StrFind(cbuffer, "content-length:", false);
	if (spos >= 0)
	{
		ClassEStd::StrCpy(vstr, &cbuffer[spos + ClassEStd::StrLen("content-length: ")], 16);
		for (ctr = 0; ctr < 16; ctr++)
			if (vstr[ctr] < '0' || vstr[ctr] > '9' || ctr == 15)
				vstr[ctr] = 0;
		httpheader.contentlength = ClassEStd::StrToInt(vstr);
	}
	spos = ClassEStd::StrFind(cbuffer, "date:", false);
	if (spos >= 0)
	{
		ClassEStd::StrCpy(httpheader.date, &cbuffer[spos + ClassEStd::StrLen("date: ")], 48);
		for (ctr = 0; ctr < 48; ctr++)
			if (httpheader.date[ctr] == '\r' || httpheader.date[ctr] == '\n')
			{
				httpheader.date[ctr] = 0;
				break;
			}
	}
	spos = ClassEStd::StrFind(cbuffer, "server:", false);
	if (spos >= 0)
	{
		ClassEStd::StrCpy(httpheader.server, &cbuffer[spos + ClassEStd::StrLen("server: ")], 256);
		for (ctr = 0; ctr < 256; ctr++)
			if (httpheader.server[ctr] == '\r' || httpheader.server[ctr] == '\n')
			{
				httpheader.server[ctr] = 0;
				break;
			}
	}
	spos = ClassEStd::StrFind(cbuffer, "content-type:", false);
	if (spos >= 0)
	{
		ClassEStd::StrCpy(httpheader.contenttype, &cbuffer[spos + ClassEStd::StrLen("content-type: ")], 128);
		for (ctr = 0; ctr < 128; ctr++)
			if (httpheader.contenttype[ctr] == '\r' || httpheader.contenttype[ctr] == '\n')
			{
				httpheader.contenttype[ctr] = 0;
				break;
			}
	}
	spos = ClassEStd::StrFind(cbuffer, "content-encoding:", false);
	if (spos >= 0)
	{
		ClassEStd::StrCpy(httpheader.contentencoding, &cbuffer[spos + ClassEStd::StrLen("content-encoding: ")], 16);
		for (ctr = 0; ctr < 16; ctr++)
			if (httpheader.contentencoding[ctr] == '\r' || httpheader.contentencoding[ctr] == '\n')
			{
				httpheader.contentencoding[ctr] = 0;
				break;
			}
	}
	spos = ClassEStd::StrFind(cbuffer, "location:", false);
	if (spos >= 0)
	{
		ClassEStd::StrCpy(httpheader.location, &cbuffer[spos + ClassEStd::StrLen("location: ")], 16);
		for (ctr = 0; ctr < 1024; ctr++)
			if (httpheader.location[ctr] == '\r' || httpheader.location[ctr] == '\n')
			{
				httpheader.location[ctr] = 0;
				break;
			}
	}
	if (ClassEStd::StrEqual(httpheader.contentencoding, "gzip") || ClassEStd::StrEqual(httpheader.contentencoding, "x-gzip"))
		compressedbody = true;
}

/////////////////////////////////////////////////////////////////////
// EHTTPStack::Public                                              //
/////////////////////////////////////////////////////////////////////

//EHTTPStack: Constructor
EHTTPStack::EHTTPStack(void)
{
	rcvstack = NULL;
}

//ENetStack: Destructor
EHTTPStack::~EHTTPStack(void)
{
	DeAlloc();
}

//Clear the HTTP stack
void EHTTPStack::Clear(void)
{
	compressedbody = false;
	lncnt = 0;
	lrcnt = 0;
	stackindex = 0;
	curstacksize = 0;
	ClassEStd::StrCpy(httpheader.contentencoding, "");
	ClassEStd::StrCpy(httpheader.server, "");
	ClassEStd::StrCpy(httpheader.date, "");
	httpheader.status = 0;
	httpheader.contentlength = 0;
}

//Allocate a new stack
bool EHTTPStack::Alloc(unsigned long size)
{
	DeAlloc();
	rcvstack = new unsigned char[size];
	if (rcvstack == NULL)
		return(false);
	totalstacksize = size;
	Clear();
	return(true);
}

//De-allocate stack memory
void EHTTPStack::DeAlloc(void)
{
	if (rcvstack != NULL)
		delete rcvstack;
	rcvstack = NULL;
}

//Push data onto the stack
bool EHTTPStack::Push(void *buffer, unsigned long bufsize)
{
	if (rcvstack == NULL)
		return(false);
	if (curstacksize + bufsize > totalstacksize)
		return(false);
	unsigned long ctr;
	unsigned char *cbuffer = (unsigned char *)buffer;
	ClassEMemory::Copy(&rcvstack[curstacksize], cbuffer, bufsize);
	curstacksize += bufsize;
	for (ctr = 0; ctr < bufsize; ctr++)
	{
		if (lrcnt < 2 || lncnt < 2)
		{
			if (lrcnt == lncnt && cbuffer[ctr] == '\r')
				lrcnt++;
			else if (lncnt == lrcnt - 1)
				lncnt++;
			else
			{
				lrcnt = 0;
				lncnt = 0;
			}
			if (lrcnt == 2 && lncnt == 2)
			{
				stackindex = ctr + 1;
				cbuffer[stackindex - 1] = 0;
				cbuffer[stackindex - 2] = 0;
				ParseHeader();
			}
		}
	}
	return(true);
}

//Return the size of the current HTTP body in the stack
unsigned long EHTTPStack::BodySize(void)
{
	if (lrcnt < 2 || lncnt < 2)
		return(0);
	return(curstacksize - stackindex);
}

//Return the body of the HTTP information that was sent to the stack
bool EHTTPStack::GetBody(void *buffer, unsigned long &bufsize)
{
	if (lrcnt < 2 || lncnt < 2)
		return(false);
	bufsize = curstacksize - stackindex;
	ClassEMemory::Copy(buffer, &rcvstack[stackindex], bufsize);
	return(true);
}

//Get the uncompressed body, last 4 bytes must be the original data size
/*bool EHTTPStack::GetBodyUncompressed(void *buffer, unsigned long &bufsize)
{
	unsigned char tmpbuffer[1024], srcbuffer[1024];
	unsigned long srcsize, dstsize;
	strcpy((char *)srcbuffer, "De kat krabt de krullen van de trap");
	srcsize = strlen((char *)srcbuffer);
	ClassEMemory::CompressZLib(tmpbuffer, dstsize, srcbuffer, srcsize);
	ClassEConsole::MsgWindow(dstsize);

	if (compressedbody)
	{
		unsigned long upos = curstacksize - 4;
		bufsize = rcvstack[upos] + (rcvstack[upos + 1] << 8) + (rcvstack[upos + 2] << 16) + (rcvstack[upos + 3] << 24);
		ClassEConsole::MsgWindow(bufsize);
		//return(ClassEMemory::UnCompressZLib(buffer, bufsize, &rcvstack[stackindex + 10], BodySize() - 30));
		ClassEConsole::MsgWindow(BodySize());
		rcvstack[stackindex + 8] = 0x78;
		rcvstack[stackindex + 9] = 0x9c;
		unsigned long a32 = ClassEMemory::Adler32(&rcvstack[stackindex + 8], BodySize() - 16) % 0xFFFFFF;
		char vstr[16];
		ClassEStd::IntToStr(vstr, a32, 16);
		ClassEConsole::MsgWindow(vstr);
		//rcvstack[upos - 4] = 0xDA;
		//rcvstack[upos - 3] = 0xCD;
		//rcvstack[upos - 2] = 0x0C;
		//rcvstack[upos - 1] = 0x69;
		rcvstack[upos - 4] = a32 >> 24;
		rcvstack[upos - 3] = a32 >> 16;
		rcvstack[upos - 2] = a32 >> 8;
		rcvstack[upos - 1] = a32;
		return(ClassEMemory::UnCompressZLib(buffer, bufsize, &rcvstack[stackindex + 8], BodySize() - 12));
	}
	return(false);
}*/

//Retrieve the stored http header, parsed from stack data
bool EHTTPStack::GetResponseInfo(EHTTPRESPONSE_INFO *info)
{
	if (lrcnt < 2 || lncnt < 2)
		return(false);
	ClassEMemory::Copy(info, &httpheader, sizeof(EHTTPRESPONSE_INFO));
	return(true);
}

//Retrieve the raw http header
bool EHTTPStack::GetResponseInfo(char *rawinfo)
{
	if (lrcnt < 2 || lncnt < 2)
		return(false);
	ClassEMemory::Copy(rawinfo, rcvstack, stackindex);
	return(true);
}

/////////////////////////////////////////////////////////////////////
// EHTTPRequest::Private                                           //
/////////////////////////////////////////////////////////////////////

//Set internal data to default
void EHTTPRequest::DefaultData(void)
{
	char tmpstr[64];
	#if defined(DEVICE_DESKTOP)
		ClassEDevice::GetOSName(tmpstr);
	#else
		ClassEDevice::GetModelName(tmpstr);
	#endif
	ClassEStd::StrCpy(info.host, "");
	ClassEStd::StrCpy(info.referer, "");
	ClassEStd::StrCpy(info.useragent, E_ENGINENAME);
	ClassEStd::StrCat(info.useragent, "/");
	ClassEStd::StrCat(info.useragent, E_VERSION);
	ClassEStd::StrCat(info.useragent, " (");
	ClassEStd::StrCat(info.useragent, tmpstr);
	ClassEStd::StrCat(info.useragent, ")");
	ClassEStd::StrCpy(info.username, "");
	ClassEStd::StrCpy(info.password, "");
	info.connection = EHTTP_CONNECTION_NONE;
	info.acceptencoding_gzip = false;
	info.port = 80;
}

//Expand the double pointer variable arrays
bool EHTTPRequest::ExpandList(unsigned char numblocks)
{
	unsigned long *newparamvaluesize = NULL;
	unsigned char **newparamvalue = NULL;
	char **newparamfilename = NULL, **newparamname = NULL, **newmimetype = NULL;
	unsigned long ctr;
	newparamvaluesize = (unsigned long *)ClassEMemory::Alloc(numblocks * sizeof(unsigned long) * 32);
	if (newparamvaluesize == NULL)
		return(false);
	newparamvalue = (unsigned char **)ClassEMemory::Alloc(numblocks * sizeof(unsigned char *) * 32);
	if (newparamvalue == NULL)
	{
		ClassEMemory::DeAlloc(newparamvaluesize);
		return(false);
	}
	newparamfilename = (char **)ClassEMemory::Alloc(numblocks * sizeof(char *) * 32);
	if (newparamfilename == NULL)
	{
		ClassEMemory::DeAlloc(newparamvaluesize);
		ClassEMemory::DeAlloc(newparamvalue);
		return(false);
	}
	newparamname = (char **)ClassEMemory::Alloc(numblocks * sizeof(char *) * 32);
	if (newparamname == NULL)
	{
		ClassEMemory::DeAlloc(newparamvaluesize);
		ClassEMemory::DeAlloc(newparamvalue);
		ClassEMemory::DeAlloc(newparamfilename);
		return(false);
	}
	newmimetype = (char **)ClassEMemory::Alloc(numblocks * sizeof(char *) * 32);
	if (newmimetype == NULL)
	{
		ClassEMemory::DeAlloc(newparamvaluesize);
		ClassEMemory::DeAlloc(newparamvalue);
		ClassEMemory::DeAlloc(newparamfilename);
		ClassEMemory::DeAlloc(newparamname);
		return(false);
	}
	for (ctr = 0; ctr < paramcount; ctr++)
	{
		newparamvaluesize[ctr] = paramvaluesize[ctr];
		newparamvalue[ctr] = paramvalue[ctr];
		newparamfilename[ctr] = paramfilename[ctr];
		newparamname[ctr] = paramname[ctr];
		newmimetype[ctr] = mimetype[ctr];
	}
	for (ctr = paramcount; ctr < (unsigned long)numblocks * 32; ctr++)
	{
		newparamvaluesize[ctr] = 0;
		newparamvalue[ctr] = NULL;
		newparamfilename[ctr] = NULL;
		newparamname[ctr] = NULL;
		newmimetype[ctr] = NULL;
	}
	ClassEMemory::DeAlloc(paramvaluesize);
	ClassEMemory::DeAlloc(paramvalue);
	ClassEMemory::DeAlloc(paramfilename);
	ClassEMemory::DeAlloc(paramname);
	ClassEMemory::DeAlloc(mimetype);
	paramvaluesize = newparamvaluesize;
	paramvalue = newparamvalue;
	paramfilename = newparamfilename;
	paramname = newparamname;
	mimetype = newmimetype;
	return(true);
}

//Add a mime type to the array
void EHTTPRequest::AddMimeArray(unsigned long &index, char extmime[MAX_MIMETYPES][8], char namemime[MAX_MIMETYPES][32], const char *ext, const char *name)
{
	ClassEStd::StrCpy(extmime[index], ext);
	ClassEStd::StrCpy(namemime[index], name);
	index++;
}

//Receive a mimetype based on the filename extension
void EHTTPRequest::GetMimeType(char *mime, const char *filename)
{
	if (filename == NULL)
	{
		ClassEStd::StrCpy(mime, "application/octet-stream");
		return;
	}
	unsigned long ctr, ictr, flen, mlen, index = 0;
	char extmime[MAX_MIMETYPES][8], namemime[MAX_MIMETYPES][32];
	for (ctr = 0; ctr < MAX_MIMETYPES; ctr++)
	{
		ClassEStd::StrCpy(extmime[ctr], "");
		ClassEStd::StrCpy(namemime[ctr], "");
	}
	AddMimeArray(index, extmime, namemime, ".avi",    "video/x-msvideo");
	AddMimeArray(index, extmime, namemime, ".c",      "text/plain");
	AddMimeArray(index, extmime, namemime, ".cc",     "text/plain");
	AddMimeArray(index, extmime, namemime, ".cpp",    "text/plain");
	AddMimeArray(index, extmime, namemime, ".css",    "text/css");
	AddMimeArray(index, extmime, namemime, ".doc",    "application/msword");
	AddMimeArray(index, extmime, namemime, ".gif",    "image/gif");
	AddMimeArray(index, extmime, namemime, ".gz",     "application/x-gzip");
	AddMimeArray(index, extmime, namemime, ".h",      "text/plain");
	AddMimeArray(index, extmime, namemime, ".htm",    "text/html");
	AddMimeArray(index, extmime, namemime, ".html",   "text/html");
	AddMimeArray(index, extmime, namemime, ".jpe",    "image/jpeg");
	AddMimeArray(index, extmime, namemime, ".jpg",    "image/jpeg");
	AddMimeArray(index, extmime, namemime, ".jpeg",   "image/jpeg");
	AddMimeArray(index, extmime, namemime, ".js",     "application/x-javascript");
	AddMimeArray(index, extmime, namemime, ".mid",    "audio/midi");
	AddMimeArray(index, extmime, namemime, ".midi",   "audio/midi");
	AddMimeArray(index, extmime, namemime, ".mov",    "video/quicktime");
	AddMimeArray(index, extmime, namemime, ".mp2",    "audio/mpeg");
	AddMimeArray(index, extmime, namemime, ".mp3",    "audio/mpeg");
	AddMimeArray(index, extmime, namemime, ".mpe",    "video/mpeg");
	AddMimeArray(index, extmime, namemime, ".mpeg",   "video/mpeg");
	AddMimeArray(index, extmime, namemime, ".mpg",    "video/mpeg");
	AddMimeArray(index, extmime, namemime, ".mpga",   "audio/mpeg");
	AddMimeArray(index, extmime, namemime, ".png",    "image/png");
	AddMimeArray(index, extmime, namemime, ".rtf",    "text/rtf");
	AddMimeArray(index, extmime, namemime, ".rtx",    "text/richtext");
	AddMimeArray(index, extmime, namemime, ".tif",    "image/tiff");
	AddMimeArray(index, extmime, namemime, ".tiff",   "image/tiff");
	AddMimeArray(index, extmime, namemime, ".txt",    "text/plain");
	AddMimeArray(index, extmime, namemime, ".vrml",   "model/vrml");
	AddMimeArray(index, extmime, namemime, ".wav",    "audio/x-wav");
	AddMimeArray(index, extmime, namemime, ".wrl",    "model/vrml");
	AddMimeArray(index, extmime, namemime, ".xbm",    "image/x-xbitmap");
	AddMimeArray(index, extmime, namemime, ".xml",    "text/xml");
	AddMimeArray(index, extmime, namemime, ".zip",    "application/zip");
	flen = ClassEStd::StrLen(filename);
	for (ctr = 0; ctr < index; ctr++)
	{
		mlen = ClassEStd::StrLen(extmime[ctr]);
		for (ictr = 0; (ictr < flen && ictr < mlen); ictr++)
		{
			if (filename[flen - 1 - ictr] != extmime[ctr][mlen - 1 - ictr])
				break;
			if (ictr == mlen - 1)
			{
				ClassEStd::StrCpy(mime, namemime[ctr]);
				return;
			}
		}
	}
	ClassEStd::StrCpy(mime, "application/octet-stream");
}

//Receive a mimetype based on the filename extension
void EHTTPRequest::GetMimeType(char *mime, const WCHAR *filename)
{
	CSTR_CHECK_BEGIN(filename);
	GetMimeType(mime, cstr_filename);
	CSTR_CHECK_END(filename);
}

/////////////////////////////////////////////////////////////////////
// EHTTPRequest::Public                                            //
/////////////////////////////////////////////////////////////////////

//HTTPRequest: constructor
EHTTPRequest::EHTTPRequest(void)
{
	paramfilename = NULL;
	paramname = NULL;
	paramvalue = NULL;
	paramvaluesize = NULL;
	mimetype = NULL;
	customheader = NULL;
	paramcount = 0;
	DefaultData();
}

//HTTPRequest: constructor with url
EHTTPRequest::EHTTPRequest(const char *seturl)
{
	bool onport = false;
	unsigned long ctr, pstart, ulen = ClassEStd::StrLen(seturl, 0xFFFF);
	paramfilename = NULL;
	paramname = NULL;
	paramvalue = NULL;
	paramvaluesize = NULL;
	mimetype = NULL;
	customheader = NULL;
	paramcount = 0;
	DefaultData();
	pstart = ulen;
	for (ctr = 0; ctr < ulen; ctr++)
		if (seturl[ctr] == '?')
			pstart = ctr;
	if (pstart < 1024 - 256)
	{
		info.port = 0;
		ClassEConnect::SplitURL(seturl, info.host, info.path, pstart);
		if (pstart + 1 < ulen)
		{
			bool onname = true;
			unsigned short nindex = 0, vindex = 0;
			char nstr[1024], vstr[1024], vdstr[1024];
			ClassEStd::StrCpy(nstr, "");
			ClassEStd::StrCpy(vstr, "");
			for (ctr = pstart + 1; ctr < ulen; ctr++)
			{
				if (onname)
				{
					if (seturl[ctr] != '=')
					{
						nstr[nindex] = seturl[ctr];
						nstr[nindex + 1] = 0;
						nindex++;
					}
					else
						onname = false;
				}
				else
				{
					if (seturl[ctr] != '&')
					{
						vstr[vindex] = seturl[ctr];
						vstr[vindex + 1] = 0;
						vindex++;
					}
					else
					{
						ClassEConnect::UrlDecode(vdstr, vstr);
						AddParam(nstr, vdstr);
						onname = true;
						nindex = 0;
						vindex = 0;
						ClassEStd::StrCpy(nstr, "");
						ClassEStd::StrCpy(vstr, "");
					}
				}
			}
			if (nstr[0] != 0)
			{
				ClassEConnect::UrlDecode(vdstr, vstr);
				AddParam(nstr, vdstr);
			}
		}
		ulen = ClassEStd::StrLen(info.host);
		for (ctr = 0; ctr < ulen; ctr++)
		{
			if (onport && info.host[ctr] >= '0' && info.host[ctr] <= '9')
			{
				info.port *= 10;
				info.port += info.host[ctr] - '0';
			}
			if (info.host[ctr] == ':')
				onport = true;
			if (onport)
				info.host[ctr] = 0;
		}
		if (info.port == 0)
			info.port = 80;
	}

}

//HTTPRequest: destructor
EHTTPRequest::~EHTTPRequest(void)
{
	unsigned long ctr;
	if (paramvalue != NULL)
	{
		for (ctr = 0; ctr < paramcount; ctr++)
			if (paramvalue[ctr] != NULL)
				ClassEMemory::DeAlloc(paramvalue[ctr]);
		ClassEMemory::DeAlloc(paramvalue);
	}
	if (paramfilename != NULL)
	{
		for (ctr = 0; ctr < paramcount; ctr++)
			if (paramfilename[ctr] != NULL)
				ClassEMemory::DeAlloc(paramfilename[ctr]);
		ClassEMemory::DeAlloc(paramfilename);
	}
	if (paramname != NULL)
	{
		for (ctr = 0; ctr < paramcount; ctr++)
			if (paramname[ctr] != NULL)
				ClassEMemory::DeAlloc(paramname[ctr]);
		ClassEMemory::DeAlloc(paramname);
	}
	if (mimetype != NULL)
	{
		for (ctr = 0; ctr < paramcount; ctr++)
			if (mimetype[ctr] != NULL)
				ClassEMemory::DeAlloc(mimetype[ctr]);
		ClassEMemory::DeAlloc(mimetype);
	}
	if (paramvaluesize != NULL)
		ClassEMemory::DeAlloc(paramvaluesize);
	if (customheader != NULL)
		ClassEMemory::DeAlloc(customheader);
}

//Add a parameter for the request
bool EHTTPRequest::AddParam(const char *name, const char *value)
{
	if (paramcount % 32 == 0)
		if (!ExpandList((unsigned char)(paramcount / 32 + 1)))
			return(false);
	paramvaluesize[paramcount] = ClassEStd::StrLen(value) + 1;
	paramname[paramcount] = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(name) + 1);
	if (paramname[paramcount] == NULL)
		return(false);
	paramvalue[paramcount] = (unsigned char *)ClassEMemory::Alloc(paramvaluesize[paramcount]);
	if (paramvalue[paramcount] == NULL)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		return(false);
	}
	ClassEStd::StrCpy(paramname[paramcount], name);
	ClassEStd::StrCpy((char *)paramvalue[paramcount], value);
	paramcount++;
	return(true);
}

//Add a parameter for the request
bool EHTTPRequest::AddParam(const char *name, long value)
{
	char vstr[32];
	ClassEStd::IntToStr(vstr, value);
	return(AddParam(name, vstr));
}

//Add a parameter as data
bool EHTTPRequest::AddData(const char *name, const unsigned char *buffer, unsigned long bufsize, const char *filename, const char *datamimetype)
{
	if (paramcount % 32 == 0)
		if (!ExpandList((unsigned char)(paramcount / 32 + 1)))
			return(false);
	paramvaluesize[paramcount] = bufsize;
	paramname[paramcount] = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(name) + 1);
	if (paramname[paramcount] == NULL)
		return(false);
	paramvalue[paramcount] = (unsigned char *)ClassEMemory::Alloc(paramvaluesize[paramcount]);
	if (paramvalue[paramcount] == NULL)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		return(false);
	}
	if (filename == NULL)
		paramfilename[paramcount] = (char *)ClassEMemory::Alloc(128);
	else
		paramfilename[paramcount] = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(filename) + 1);
	if (paramfilename[paramcount] == NULL)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		ClassEMemory::DeAlloc(paramvalue[paramcount]);
		return(false);
	}
	if (datamimetype == NULL)
		mimetype[paramcount] = (char *)ClassEMemory::Alloc(128);
	else
		mimetype[paramcount] = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(datamimetype) + 1);
	if (mimetype[paramcount] == NULL)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		ClassEMemory::DeAlloc(paramvalue[paramcount]);
		ClassEMemory::DeAlloc(paramfilename[paramcount]);
		return(false);
	}
	ClassEStd::StrCpy(paramname[paramcount], name);
	ClassEMemory::Copy(paramvalue[paramcount], buffer, paramvaluesize[paramcount]);
	if (filename == NULL)
	{
		ClassEStd::StrCpy(paramfilename[paramcount], "file");
		ClassEStd::IntToStr(&paramfilename[paramcount][ClassEStd::StrLen(paramfilename[paramcount])], paramcount + 1);
	}
	else
		ClassEStd::StrCpy(paramfilename[paramcount], filename);
	if (datamimetype == NULL)
		GetMimeType(mimetype[paramcount], filename);
	else
		ClassEStd::StrCpy(mimetype[paramcount], datamimetype);
	paramcount++;
	return(true);
}

//Add a parameter as a file
bool EHTTPRequest::AddFile(const char *name, const char *filename, const char *filemimetype)
{
	ClassEFile ef;
	bool readsuccess = true;
	unsigned long filesize = ef.Size(filename);
	if (filesize == 0)
		return(false);
	if (paramcount % 32 == 0)
		if (!ExpandList((unsigned char)(paramcount / 32 + 1)))
			return(false);
	paramvaluesize[paramcount] = filesize;
	paramname[paramcount] = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(name) + 1);
	if (paramname[paramcount] == NULL)
		return(false);
	paramvalue[paramcount] = (unsigned char *)ClassEMemory::Alloc(paramvaluesize[paramcount]);
	if (paramvalue[paramcount] == NULL)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		return(false);
	}
	paramfilename[paramcount] = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(filename) + 1);
	if (paramfilename[paramcount] == NULL)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		ClassEMemory::DeAlloc(paramvalue[paramcount]);
		return(false);
	}
	if (filemimetype == NULL)
		mimetype[paramcount] = (char *)ClassEMemory::Alloc(128);
	else
		mimetype[paramcount] = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(filemimetype) + 1);
	if (mimetype[paramcount] == NULL)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		ClassEMemory::DeAlloc(paramvalue[paramcount]);
		ClassEMemory::DeAlloc(paramfilename[paramcount]);
		return(false);
	}
	if (ef.Open(filename, EFOF_READ))
	{
		if (!ef.Read(paramvalue[paramcount], paramvaluesize[paramcount]))
			readsuccess = false;
		ef.Close();
	}
	else
		readsuccess = false;
	if (!readsuccess)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		ClassEMemory::DeAlloc(paramvalue[paramcount]);
		ClassEMemory::DeAlloc(paramfilename[paramcount]);
		ClassEMemory::DeAlloc(mimetype[paramcount]);
		return(false);
	}
	ClassEStd::StrCpy(paramname[paramcount], name);
	ClassEStd::StrCpy(paramfilename[paramcount], filename);
	if (filemimetype == NULL)
		GetMimeType(mimetype[paramcount], filename);
	else
		ClassEStd::StrCpy(mimetype[paramcount], filemimetype);
	paramcount++;
	return(true);
}

//Add a parameter as a file
bool EHTTPRequest::AddFile(const char *name, ClassEFile *ef, const char *filemimetype)
{
	bool readsuccess = true;
	unsigned long filesize = ef->Size();
	if (filesize == 0)
		return(false);
	if (paramcount % 32 == 0)
		if (!ExpandList((unsigned char)(paramcount / 32 + 1)))
			return(false);
	paramvaluesize[paramcount] = filesize;
	paramname[paramcount] = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(name) + 1);
	if (paramname[paramcount] == NULL)
		return(false);
	paramvalue[paramcount] = (unsigned char *)ClassEMemory::Alloc(paramvaluesize[paramcount]);
	if (paramvalue[paramcount] == NULL)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		return(false);
	}
	paramfilename[paramcount] = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(ef->curfilename) + 1);
	if (paramfilename[paramcount] == NULL)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		ClassEMemory::DeAlloc(paramvalue[paramcount]);
		return(false);
	}
	if (filemimetype == NULL)
		mimetype[paramcount] = (char *)ClassEMemory::Alloc(128);
	else
		mimetype[paramcount] = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(filemimetype) + 1);
	if (mimetype[paramcount] == NULL)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		ClassEMemory::DeAlloc(paramvalue[paramcount]);
		ClassEMemory::DeAlloc(paramfilename[paramcount]);
		return(false);
	}
	if (!ef->Read(paramvalue[paramcount], paramvaluesize[paramcount]))
		readsuccess = false;
	if (!readsuccess)
	{
		ClassEMemory::DeAlloc(paramname[paramcount]);
		ClassEMemory::DeAlloc(paramvalue[paramcount]);
		ClassEMemory::DeAlloc(paramfilename[paramcount]);
		ClassEMemory::DeAlloc(mimetype[paramcount]);
		return(false);
	}
	ClassEStd::StrCpy(paramname[paramcount], name);
	ClassEStd::StrCpy(paramfilename[paramcount], ef->curfilename);
	if (filemimetype == NULL)
		GetMimeType(mimetype[paramcount], ef->curfilename);
	else
		ClassEStd::StrCpy(mimetype[paramcount], filemimetype);
	paramcount++;
	return(true);
}

//Add a parameter as a file
void EHTTPRequest::AddCustomHeader(const char *hdr)
{
	if (customheader != NULL)
	{
		ClassEMemory::DeAlloc(customheader);
		customheader = NULL;
	}
	customheader = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(hdr) + 1);
	if (customheader)
		ClassEStd::StrCpy(customheader, hdr);
}
