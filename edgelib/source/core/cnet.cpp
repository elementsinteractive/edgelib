// cnet.cpp
// Edgecore: provides network API
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Device include
#include "edgedevice.h"

//Device SDK includes
#if defined(DEVICE_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#if defined(DEVICE_DESKTOP) || (DEVICE_WINCE >= 400)
		#include <winsock2.h>
		#include <ws2bth.h>
	#else
		#include <winsock.h>
		#include "extapi/imp_bluetooth.h"
	#endif
	#if defined(DEVICE_WINMOBILE)
		#include <initguid.h>
		#include <connmgr.h>
	#endif
	#if (DEVICE_WINCE >= 400)
		#if defined(DEVICE_GIZMONDO)
			#include <bt_api.h>
		#endif
		#include <bthapi.h>
	#endif
#elif defined(DEVICE_SYMBIAN)
	#include <eikenv.h>
	#include <es_sock.h>
	#include <in_sock.h>
	#if (DEVICE_SYMBIAN >= 610)
		#include <bt_sock.h>
		#include <btsdp.h>
		#include <btmanclient.h>
	#endif
#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
	#include <stdlib.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <netinet/in.h>
	#include <netdb.h>
	#include <unistd.h>
	#if defined(DEVICE_MAEMO)
		#include <dbus/dbus-glib-lowlevel.h>
		#include <conicconnection.h>
		#include <conicconnectionevent.h>
	#endif
#elif defined(DEVICE_OPENKODE)
	#include <KD/ATX_bluetooth.h>
#endif

//Core include
#include "edgecore.h"

#if defined(DEVICE_WIN32)
	#include "extapi\\parsesdp.h"
#endif

///////////////////////////////////////////////////////////////////
// Internal definitions and structures
///////////////////////////////////////////////////////////////////

//Array size for storing found bluetooth device ID's
#define EC_MAXBTSEARCHARRAY 16384

//Callback prototype
bool eciNetDeviceCallback(unsigned char foundtype, unsigned char *data, unsigned long datasize, void *userdata);

//Forward symbian interface class declarations
#if defined(DEVICE_SYMBIAN)
	#if (DEVICE_SYMBIAN >= 610)
		class CSymDeviceDiscoverer;
		class CSymBTAdvertiser;
	#endif
#endif

//Function definitions for bluetooth state
#if defined(DEVICE_WINCE)
	#if defined(DEVICE_WINMOBILE)
		typedef int (*FUNC_BTHSETMODE)(DWORD dwMode);
		typedef int (*FUNC_BTHGETMODE)(DWORD *pdwMode);
	#else
		typedef int (CALLBACK *FUNC_BTHGETHARDWARESTATUS)(int *);
	#endif
#endif

//Bluetooth userdata for device name callback
#if defined(DEVICE_OPENKODE)
	typedef struct
	{
		KDBdAddrATX btaddr;
		ECOREOBJ netobj;
		unsigned char arraydata[256];
		unsigned long arrsize;
	}ECI_KDBT_NETOBJ;
#endif

//Network object
typedef struct
{
	#if defined(DEVICE_WIN32)
		HANDLE btsearchthread;
		HANDLE btsearchthreadexit;
		#if defined(DEVICE_WINMOBILE)
			HANDLE hconn;
		#endif
		#if defined(DEVICE_WINCE)
			HMODULE dll_btapi;
			#if defined(DEVICE_WINMOBILE)
				FUNC_BTHSETMODE func_bthsetmode;
				FUNC_BTHGETMODE func_bthgetmode;
			#else
				FUNC_BTHGETHARDWARESTATUS func_bthgethardwarestatus;
			#endif
		#endif
		char btservicerec[128];
		unsigned char btserviceport;
	#elif defined(DEVICE_SYMBIAN)
		#if ((DEVICE_SYMBIAN >= 700) && !defined(DEVICE_UIQ)) || (DEVICE_SYMBIAN >= 800)
			RConnection rconn;
		#endif
		TBuf8<1024> sockreadbuf;
		RSocketServ sockserv;
		#if (DEVICE_SYMBIAN >= 610)
			CSymDeviceDiscoverer *discoverer;
			CSymBTAdvertiser *advertiser;
		#endif
	#elif defined(DEVICE_OPENKODE)
		ECI_KDBT_NETOBJ *btqueue;
		unsigned long btqueuesize;
		unsigned long btqueueidx;
		unsigned long queuedbtdevices;
		bool queuebtend;
	#endif
	FUNC_NETHANDLER eventhandler;
	void *handler_userdata;
	unsigned char *devicearray;
	unsigned long devarr_addidx;
	unsigned long devarr_readidx;
	unsigned long uid;
	bool abortsearch;
	bool searchcomplete;
	bool servicesearch;
}EC_NETOBJ;

//Socket object
typedef struct
{
	#if defined(DEVICE_WIN32)
		SOCKET sock;
		SOCKADDR_BTH boundaddr_bth;
		bool alreadybound;
	#elif defined(DEVICE_SYMBIAN)
		RSocket sock;
		bool server;
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		int sock;
	#elif defined(DEVICE_OPENKODE)
		KDSocket *sock;
		char foundaddr[256];
		unsigned short useport;
		unsigned long numreads;
		bool bindfrombtport;
	#endif
	EC_NETOBJ *parent;
	unsigned char protocol;
	unsigned long availport;
	unsigned long activeport;
}EC_SOCKOBJ;

///////////////////////////////////////////////////////////////////
// Network API
///////////////////////////////////////////////////////////////////

//Symbian bluetooth API interface classes
#if defined(DEVICE_SYMBIAN)
	unsigned long EdgeGetUIDNr(void);
	#if (DEVICE_SYMBIAN >= 610)
		#include "extapi\\symbianbtapi.h"
	#endif
#endif

#if defined(DEVICE_SYMBIAN)

//Wait for request with timeout
bool WaitForRequestTimeout(TRequestStatus &waitstat, unsigned long max_timeout)
{
	TRequestStatus timerstat;
	RTimer timer;
	if (timer.CreateLocal() != KErrNone)
		return(false);
	timer.After(timerstat, max_timeout * 1000);
	User::WaitForRequest(waitstat, timerstat);
	if (waitstat.Int() == KRequestPending)
	{
		timer.Close();
		return(false);
	}
	timer.Cancel();
	User::WaitForRequest(timerstat);
	timer.Close();
	return(true);
}

#endif

#if defined(DEVICE_WIN32)

//Convert bluetooth address to ascii
void eciNetBTAtoA(char *btstr, __int64 address)
{
	unsigned short nv = GET_NAP(address);
	unsigned long  sv = GET_SAP(address);
	btstr[0] = (unsigned char)((nv >> 12) & 15);
	btstr[1] = (unsigned char)((nv >> 8) & 15);
	btstr[3] = (unsigned char)((nv >> 4) & 15);
	btstr[4] = (unsigned char)((nv) & 15);
	btstr[6] = (unsigned char)((sv >> 28) & 15);
	btstr[7] = (unsigned char)((sv >> 24) & 15);
	btstr[9] = (unsigned char)((sv >> 20) & 15);
	btstr[10] = (unsigned char)((sv >> 16) & 15);
	btstr[12] = (unsigned char)((sv >> 12) & 15);
	btstr[13] = (unsigned char)((sv >> 8) & 15);
	btstr[15] = (unsigned char)((sv >> 4) & 15);
	btstr[16] = (unsigned char)((sv) & 15);
	btstr[17] = 0;
	for (nv = 0; nv < 17; nv++)
		if (btstr[nv] > 9)
			btstr[nv] = btstr[nv] - 10 + 'a';
		else
			btstr[nv] = btstr[nv] + '0';
	btstr[2] = ':';
	btstr[5] = ':';
	btstr[8] = ':';
	btstr[11] = ':';
	btstr[14] = ':';
}

//Convert a bluetooth address string to a bluetooth address
__int64 eciNetAtoBTA(char *btstr)
{
	char str[17];
	unsigned short nv;
	unsigned long  sv;
	for (nv = 0; nv < 17; nv++)
	{
		if (btstr[nv] >= 'a')
			str[nv] = btstr[nv] - 'a' + 1 + '9';
		else if (btstr[nv] >= 'A')
			str[nv] = btstr[nv] - 'A' + 1 + '9';
		else
			str[nv] = btstr[nv];
	}
	nv = ((str[0] - '0') << 12) | ((str[1] - '0') << 8) | ((str[3] - '0') << 4) | (str[4] - '0');
	sv = ((str[6] - '0') << 28) | ((str[7] - '0') << 24) | ((str[9] - '0') << 20) | ((str[10] - '0') << 16) | ((str[12] - '0') << 12) | ((str[13] - '0') << 8) | ((str[15] - '0') << 4) | (str[16] - '0');
	return(SET_NAP_SAP(nv, sv));
}

//Bluetooth searcher thread for Win32
DWORD WINAPI eciNetThreadBTSearch(LPVOID lpParameter)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)lpParameter;
	unsigned long flushflag = 0;
	#if defined(DEVICE_DESKTOP)
		flushflag = LUP_FLUSHCACHE;
	#endif
	union
	{
		CHAR buf[5000];
		double __unused;
	};
	HANDLE lookuphandle;
	WSAQUERYSET wsaq;
	LPWSAQUERYSET pwsaresults = (LPWSAQUERYSET)buf;
	DWORD dwSize = sizeof(buf);
	unsigned char arraydata[512];
	ecMemSet(&wsaq, 0, sizeof(wsaq));
	wsaq.dwSize      = sizeof(wsaq);
	wsaq.dwNameSpace = NS_BTH;
	wsaq.lpcsaBuffer = NULL;
	if (WSALookupServiceBegin(&wsaq, LUP_CONTAINERS | flushflag, &lookuphandle) == ERROR_SUCCESS) // | (~LUP_RES_SERVICE)
	{
		ecMemSet(pwsaresults, 0, sizeof(WSAQUERYSET));
		pwsaresults->dwSize      = sizeof(WSAQUERYSET);
		pwsaresults->dwNameSpace = NS_BTH;
		pwsaresults->lpBlob      = NULL;
		while (WSALookupServiceNext(lookuphandle, LUP_RETURN_NAME | LUP_RETURN_ADDR | flushflag, &dwSize, pwsaresults) == ERROR_SUCCESS)
		{
			unsigned long arraydatasize = 21;
			arraydata[0] = 18;
			eciNetBTAtoA((char *)&arraydata[1], ((SOCKADDR_BTH *)pwsaresults->lpcsaBuffer->RemoteAddr.lpSockaddr)->btAddr);
			arraydata[19] = 1;
			arraydata[20] = 0;
			if (pwsaresults->lpszServiceInstanceName && *(pwsaresults->lpszServiceInstanceName))
			{
				#if defined(UNICODE)
					unsigned long namelen = ecStrLen((const ecUnistring)pwsaresults->lpszServiceInstanceName);
				#else
					unsigned long namelen = ecStrLen(pwsaresults->lpszServiceInstanceName);
				#endif
				if (namelen + 1 > 255)
				{
					namelen = 255 - 1;
					pwsaresults->lpszServiceInstanceName[namelen] = 0;
				}
				arraydata[arraydatasize] = (unsigned char)namelen + 1;
				#if defined(UNICODE)
					ecStrCpy((char *)&arraydata[arraydatasize + 1], (const ecUnistring)pwsaresults->lpszServiceInstanceName);
				#else
					ecStrCpy((char *)&arraydata[arraydatasize + 1], pwsaresults->lpszServiceInstanceName);
				#endif
				arraydatasize += arraydata[arraydatasize] + 1;
			}
			else
			{
				arraydata[arraydatasize] = 1;
				arraydata[arraydatasize + 1] = 0;
				arraydatasize += arraydata[arraydatasize] + 1;
			}
			arraydata[arraydatasize] = 1;
			arraydata[arraydatasize + 1] = 0;
			arraydatasize += arraydata[arraydatasize] + 1;
			if (!eciNetDeviceCallback(0, arraydata, arraydatasize, neto))
				break;
			if (neto->servicesearch)
			{
				TCHAR searchaddr[64];
				unsigned long basicarrsize = arraydatasize - 2;
				union
				{
					CHAR lookupbuf[5000];
					double __unused;
				};
				struct
				{
					BTHNS_SETBLOB b;
					unsigned char uca[8192];
				}bigBlob;
				LPWSAQUERYSET pwservresults = (LPWSAQUERYSET)lookupbuf;
				GUID l2capguid = {0x00000100, 0x0000, 0x1000, 0x80, 0x00, 0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB};
				WSAQUERYSET wsaqserv;
				HANDLE servlookuphandle;
				DWORD dwsizeserv;
				#if defined(UNICODE)
					ecStrCpy((const ecUnistring)searchaddr, "(");
					ecStrCat((const ecUnistring)searchaddr, (char *)&arraydata[1]);
					ecStrCat((const ecUnistring)searchaddr, ")");
				#else
					ecStrCpy(searchaddr, "(");
					ecStrCat(searchaddr, (char *)&arraydata[1]);
					ecStrCat(searchaddr, ")");
				#endif
				ecMemSet(&wsaqserv, 0, sizeof(WSAQUERYSET));
				wsaqserv.dwSize      = sizeof(WSAQUERYSET);
				wsaqserv.lpServiceClassId = &l2capguid;
				wsaqserv.dwNameSpace = NS_BTH;
				wsaqserv.dwNumberOfCsAddrs = 0;
				wsaqserv.lpszContext = (LPTSTR)searchaddr;
				wsaqserv.lpcsaBuffer = NULL;
				if (WSALookupServiceBegin(&wsaqserv, flushflag, &servlookuphandle) == ERROR_SUCCESS)
				{
					pwservresults->dwSize = sizeof(WSAQUERYSET);
					BLOB blob;
					blob.pBlobData = (PBYTE)&bigBlob;
					pwservresults->lpBlob = &blob;
					dwsizeserv = sizeof(lookupbuf);
					while (WSALookupServiceNext(servlookuphandle, flushflag | LUP_RETURN_BLOB, &dwsizeserv, pwservresults) == ERROR_SUCCESS)
					{
						if (pwservresults->lpBlob != NULL)
						{
							unsigned char *bbuf = pwservresults->lpBlob->pBlobData;
							while(pwservresults->lpBlob->cbSize)
							{
								arraydatasize = basicarrsize;
								if (bbuf[0] != 0x35 && bbuf[0] != 0x36)
									break;
								if (bbuf[0] == 0x35)
								{
									ESDPRECORDINFO sdpinfo;
									CEdgeParseSDP parsesdp;
									parsesdp.SetBlob(bbuf, bbuf[1] + 2);
									parsesdp.ParseBlob(&sdpinfo);
									arraydata[20] = sdpinfo.port;
									arraydata[arraydatasize] = (unsigned char)ecStrLen(sdpinfo.servicename) + 1;
									ecStrCpy((char *)&arraydata[arraydatasize + 1], sdpinfo.servicename);
									arraydatasize += arraydata[arraydatasize] + 1;
									if (sdpinfo.port)
										if (!eciNetDeviceCallback(0, arraydata, arraydatasize, neto))
											break;
									pwservresults->lpBlob->cbSize -= bbuf[1] + 2;
									bbuf += bbuf[1] + 2;
								}
								else if (bbuf[0] == 0x36)
									bbuf += 3;
								if (neto->abortsearch)
									break;
							}
						}
						if (neto->abortsearch)
							break;
					}
					WSALookupServiceEnd(servlookuphandle);
				}
			}
			if (neto->abortsearch)
				break;
		}
		WSALookupServiceEnd(lookuphandle);
	}
	neto->searchcomplete = true;
	SetEvent(neto->btsearchthreadexit);
	return(0);
}

//Switch bluetooth service on or off
int eciNetHandleBTService(char *servicename, unsigned char port, bool state)
{
	TCHAR tservname[256];
	#if defined(UNICODE)
		ecStrCpy((const ecUnistring)tservname, servicename);
	#else
		ecStrCpy(tservname, servicename);
	#endif
	unsigned char btguid[] = {
		0x00, 0x00, 0x11, 0x01,
		0x00, 0x00,
		0x10, 0x00,
		0x80, 0x00,
		0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB
	};
	unsigned char browseguid[] = {
		0x00, 0x00, 0x10, 0x02,
		0x00, 0x00,
		0x10, 0x00,
		0x80, 0x00,
		0x00, 0x80, 0x5F, 0x9B, 0x34, 0xFB
	};
	struct {
		BTHNS_SETBLOB b;
		unsigned char uca[256];
	}bigBlob;
	CEdgeParseSDP parsesdp;
	WSAQUERYSET wsaqueryset;
	BLOB blob;
	ULONG recordhandle = 0;
	ULONG sdpversion = BTH_SDP_VERSION;
	ULONG *rhptr = &recordhandle;
	ecMemSet(&wsaqueryset, 0, sizeof(WSAQUERYSET));
	wsaqueryset.dwSize = sizeof(WSAQUERYSET);
	parsesdp.SetClassGuid(btguid);
	parsesdp.SetClassGuid(browseguid);
	parsesdp.SetSerialPort(port);
	parsesdp.SetUnknownList();
	parsesdp.SetName(servicename);
	ecMemSet(&bigBlob, 0, sizeof(bigBlob));
	ecMemSet(&blob, 0, sizeof(blob));
	bigBlob.b.pSdpVersion     = &sdpversion;
	bigBlob.b.ulRecordLength  = parsesdp.bloblength;
	ecMemCpy(bigBlob.b.pRecord, parsesdp.blob, parsesdp.bloblength);
	blob.cbSize    = sizeof(BTHNS_SETBLOB) + bigBlob.b.ulRecordLength - 1;
	blob.pBlobData = (PBYTE)&bigBlob;
	wsaqueryset.dwNameSpace = NS_BTH;    
	wsaqueryset.lpServiceClassId        = 0;
	wsaqueryset.lpszServiceInstanceName = tservname;
	wsaqueryset.lpszComment             = TEXT("EDGELIB Bluetooth Service");
	wsaqueryset.dwNumberOfCsAddrs       = 0;
	wsaqueryset.lpcsaBuffer             = 0;
	wsaqueryset.lpBlob                  = &blob;
	#if defined(DEVICE_WINCE)
		bigBlob.b.pRecordHandle   = rhptr;
		bigBlob.b.fSecurity       = 0;
		bigBlob.b.fOptions        = 0;
	#else
		bigBlob.b.pRecordHandle  = (void **)rhptr;
		bigBlob.b.fCodService    = 0;
	#endif
	if (state)
		return(WSASetService(&wsaqueryset, RNRSERVICE_REGISTER, 0));
	return(WSASetService(&wsaqueryset, RNRSERVICE_DELETE, 0));
}

#elif defined(DEVICE_SYMBIAN)

#if (DEVICE_SYMBIAN >= 610)

//Convert bluetooth address to ascii
void eciNetBTAtoA(char *btstr, TBTSockAddr address)
{
	unsigned char nv;
	for (nv = 0; nv < 6; nv++)
	{
		btstr[nv * 3] = (unsigned char)((address.BTAddr()[nv] >> 4) & 15);
		btstr[nv * 3 + 1] = (unsigned char)(address.BTAddr()[nv] & 15);
	}
	for (nv = 0; nv < 17; nv++)
		if (btstr[nv] > 9)
			btstr[nv] = btstr[nv] - 10 + 'a';
		else
			btstr[nv] = btstr[nv] + '0';
	for (nv = 0; nv < 6; nv++)
		btstr[nv * 3 + 2] = ':';
	btstr[17] = 0;
}

//Convert ascii to bluetooth address
TBTSockAddr eciNetAtoBTA(char *btstr)
{
	TBTDevAddr devaddr;
	TBTSockAddr address;
	char str[17];
	unsigned short nv;
	for (nv = 0; nv < 17; nv++)
	{
		if (btstr[nv] >= 'a')
			str[nv] = btstr[nv] - 'a' + 1 + '9';
		else if (btstr[nv] >= 'A')
			str[nv] = btstr[nv] - 'A' + 1 + '9';
		else
			str[nv] = btstr[nv];
	}
	for (nv = 0; nv < 6; nv++)
		devaddr[nv] = ((str[nv * 3] - '0') << 4) | ((str[nv * 3 + 1]) - '0');
	address.SetBTAddr(devaddr);
	return(address);
}

#endif

#elif defined(DEVICE_OPENKODE)

//Convert bluetooth address to ascii
void eciNetBTAtoA(char *btstr, unsigned char *address)
{
	int nv;
	for (nv = 0; nv < 6; nv++)
	{
		btstr[nv * 3] = (unsigned char)((address[nv] >> 4) & 15);
		btstr[nv * 3 + 1] = (unsigned char)(address[nv] & 15);
	}
	btstr[17] = 0;
	for (nv = 0; nv < 17; nv++)
		if (btstr[nv] > 9)
			btstr[nv] = btstr[nv] - 10 + 'a';
		else
			btstr[nv] = btstr[nv] + '0';
	btstr[2] = ':';
	btstr[5] = ':';
	btstr[8] = ':';
	btstr[11] = ':';
	btstr[14] = ':';
}

//Convert ascii to bluetooth address
void eciNetAtoBTA(unsigned char *address, const char *btstr)
{
	char str[17];
	int nv;
	for (nv = 0; nv < 17; nv++)
	{
		if (btstr[nv] >= 'a')
			str[nv] = btstr[nv] - 'a' + 1 + '9';
		else if (btstr[nv] >= 'A')
			str[nv] = btstr[nv] - 'A' + 1 + '9';
		else
			str[nv] = btstr[nv];
	}
	for (nv = 0; nv < 6; nv++)
		address[nv] = ((str[nv * 3] - '0') << 4) | (str[nv * 3 + 1] - '0');
}

#endif

#if defined(DEVICE_MAEMO)

// maemo glib callbacks
static int ecNetMaemoStatus;
static GMainLoop * ecNetGlibloop;
static void ecNetGLibMessage(ConIcConnection * connection, ConIcConnectionEvent * ev, gpointer userdata)
{
	ConIcConnectionStatus status = con_ic_connection_event_get_status(ev);
	ecNetMaemoStatus = 2;
	if (status == CON_IC_STATUS_CONNECTED)
		ecNetMaemoStatus = 1;	
	g_main_loop_quit(ecNetGlibloop);
}

#endif

//Convert an ip-address to ascii
void eciNetIPAtoA(char *ipstr, unsigned long address)
{
	unsigned char ctr, daddr, spos = 0;
	#if defined(DEVICE_OPENKODE)
		address = ((address & 0xFF) << 24) | ((address & 0xFF00) << 8) | ((address & 0xFF0000) >> 8) | ((address & 0xFF000000) >> 24);
	#endif
	for (ctr = 0; ctr < 4; ctr++)
	{
		daddr = (unsigned char)(address >> 24);
		address <<= 8;
		if (daddr >= 100)
		{
			ipstr[spos] = '0' + daddr / 100;
			spos++;
		}
		if (daddr >= 10)
		{
			ipstr[spos] = '0' + daddr / 10 % 10;
			spos++;
		}
		ipstr[spos] = '0' + daddr % 10;
		spos++;
		if (ctr < 3)
		{
			ipstr[spos] = '.';
			spos++;
		}
	}
	ipstr[spos] = 0;
}

//Convert an ascii string to an ip-address
unsigned long eciNetAtoIPA(char *ipstr)
{
	unsigned long ctr, rval = 0, slen = ecStrLen(ipstr);
	unsigned char daddr = 0, dcount = 0;
	for (ctr = 0; ctr < slen; ctr++)
	{
		if (ipstr[ctr] == '.')
		{
			if (dcount >= 3)
				return(0xFFFFFFFF);
			dcount++;
			rval <<= 8;
			rval += daddr;
			daddr = 0;
		}
		else if (ipstr[ctr] >= '0' && ipstr[ctr] <= '9')
		{
			daddr *= 10;
			daddr += ipstr[ctr] - '0';
		}
		else
			return(0xFFFFFFFF);
	}
	rval <<= 8;
	rval += daddr;
	return(rval);
}

//Callback for found devices
bool eciNetDeviceCallback(unsigned char foundtype, unsigned char *data, unsigned long datasize, void *userdata)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)userdata;
	if (neto->devarr_addidx + datasize <= EC_MAXBTSEARCHARRAY)
	{
		neto->devicearray[neto->devarr_addidx] = (unsigned char)(datasize + 1);
		ecMemCpy(&neto->devicearray[neto->devarr_addidx + 1], data, datasize);
		neto->devarr_addidx += datasize + 1;
		return(true);
	}
	return(false);
}

//Create network object
ECOREOBJ ecNetCreate(unsigned long uid)
{
	EC_NETOBJ *neto = new EC_NETOBJ;
	#if defined(DEVICE_WIN32)
		if (neto)
		{
			WSADATA sockdata;
			unsigned short version = MAKEWORD(2, 0);
			int error;
			ecMemSet(&sockdata, 0, sizeof(sockdata));
  			error = WSAStartup(version, &sockdata);
			if (error == SOCKET_ERROR)
			{
				if (error == WSAVERNOTSUPPORTED)
					WSACleanup();
				delete neto;
				neto = NULL;
			}
			else
			{
				neto->btsearchthread = NULL;
				neto->btsearchthreadexit = NULL;
				ecStrCpy(neto->btservicerec, "");
				#if defined(DEVICE_WINCE)
					neto->dll_btapi = NULL;
					#if defined(DEVICE_WINMOBILE)
						neto->func_bthsetmode = NULL;
						neto->func_bthgetmode = NULL;
					#else
						neto->func_bthgethardwarestatus = NULL;
					#endif
				#endif
			}
		}
	#elif defined(DEVICE_SYMBIAN)
		if (neto)
		{
			#if (DEVICE_SYMBIAN >= 610)
				neto->discoverer = NULL;
				neto->advertiser = NULL;
			#endif
			if (neto->sockserv.Connect() != KErrNone)
			{
				delete neto;
				neto = NULL;
			}
		}
	#elif defined(DEVICE_OPENKODE)
		neto->btqueue = NULL;
	#endif
	if (neto)
	{
		neto->uid = uid;
		neto->searchcomplete = true;
		neto->devicearray = NULL;
		neto->eventhandler = NULL;
		neto->handler_userdata = NULL;
	}
	return(neto);
}

//Destroy network object
void ecNetDestroy(ECOREOBJ netobj)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
	#if defined(DEVICE_WIN32)
		ecNetStopBluetoothSearch(netobj);
		if (neto->btsearchthreadexit)
			if (WaitForSingleObject(neto->btsearchthreadexit, 5000) != WAIT_OBJECT_0)
				return;
		if (neto->btsearchthread)
		{
			if (neto->btsearchthreadexit != NULL)
			{
				CloseHandle(neto->btsearchthreadexit);
				neto->btsearchthreadexit = NULL;
			}
			CloseHandle(neto->btsearchthread);
			neto->btsearchthread = NULL;
		}
		WSACleanup();
		#if defined(DEVICE_WINCE)
			if (neto->dll_btapi != NULL)
				FreeLibrary(neto->dll_btapi);
		#endif
	#elif defined(DEVICE_SYMBIAN)
		#if (DEVICE_SYMBIAN >= 610)
			if (neto->discoverer)
				delete neto->discoverer;
		#endif
		neto->sockserv.Close();
	#elif defined(DEVICE_OPENKODE)
		if (neto->btqueue)
		{
			delete [] neto->btqueue;
			neto->btqueue = NULL;
		}
	#endif
	if (neto->devicearray)
		ecMemFree(neto->devicearray);
	delete neto;
}

//Search for bluetooth devices or services
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR
ecErr ecNetStartBluetoothSearch(ECOREOBJ netobj, bool services)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
	if (neto->devicearray == NULL)
	{
		neto->devicearray = (unsigned char *)ecMemAlloc(EC_MAXBTSEARCHARRAY);
		if (neto->devicearray == NULL)
			return(1);//E_NOMEMORY
	}
	#if defined(DEVICE_WIN32)
		DWORD threadid;
		TCHAR threadname[128];
		#if defined(UNICODE)
			ecStrCpy((ecUnistring)threadname, "edgelib_btsearch_exit_");
			ecIntToStr((ecUnistring)&threadname[ecStrLen((ecUnistring)threadname)], GetCurrentProcessId(), 10, false);
		#else
			ecStrCpy(threadname, "edgelib_btsearch_exit_");
			ecIntToStr(&threadname[ecStrLen(threadname)], GetCurrentProcessId(), 10, false);
		#endif
		ecNetStopBluetoothSearch(netobj);
		if (neto->btsearchthreadexit)
			if (WaitForSingleObject(neto->btsearchthreadexit, 5000) != WAIT_OBJECT_0)
				return(3);//E_ERROR
		if (neto->btsearchthread)
		{
			if (neto->btsearchthreadexit != NULL)
			{
				CloseHandle(neto->btsearchthreadexit);
				neto->btsearchthreadexit = NULL;
			}
			CloseHandle(neto->btsearchthread);
			neto->btsearchthread = NULL;
		}
		neto->abortsearch = false;
		neto->servicesearch = services;
		neto->devarr_addidx = 0;
		neto->devarr_readidx = 0;
		neto->btsearchthreadexit = CreateEvent(NULL, TRUE, FALSE, threadname);
		if (neto->btsearchthreadexit == NULL)
			return(1);//E_NOMEMORY
		neto->btsearchthread = CreateThread(NULL, 0, eciNetThreadBTSearch, netobj, CREATE_SUSPENDED, &threadid);
		if (neto->btsearchthread == NULL)
		{
			CloseHandle(neto->btsearchthreadexit);
			neto->btsearchthreadexit = NULL;
			return(1);//E_NOMEMORY
		}
		Sleep(10);
		neto->searchcomplete = false;
		ResumeThread(neto->btsearchthread);
	#elif defined(DEVICE_SYMBIAN)
		#if (DEVICE_SYMBIAN >= 610)
			ecNetStopBluetoothSearch(netobj);
			if (neto->discoverer)
				neto->discoverer->Cancel();
			if (neto->discoverer == NULL)
			{
				neto->discoverer = CSymDeviceDiscoverer::NewL(neto);
				if (neto->discoverer == NULL)
					return(1);//E_NOMEMORY
			}
			neto->abortsearch = false;
			neto->servicesearch = services;
			neto->devarr_addidx = 0;
			neto->devarr_readidx = 0;
			neto->searchcomplete = false;
			if (!neto->discoverer->DiscoverDevices())
				return(3);//E_ERROR
		#else
			neto->devarr_addidx = 0;
			neto->devarr_readidx = 0;
			return(2);//E_UNSUPPORTED
		#endif
	#elif defined(DEVICE_OPENKODE)
		if (neto->btqueue == NULL)
		{
			neto->btqueue = new ECI_KDBT_NETOBJ[128];
			if (neto->btqueue == NULL)
				return(1);//E_NOMEMORY
		}
		kdBtCancelSearchServicesATX(KD_NULL);
		kdBtCancelInquireDevicesATX(KD_NULL);
		neto->abortsearch = false;
		neto->servicesearch = services;
		neto->queuedbtdevices = 0;
		neto->queuebtend = false;
		neto->devarr_addidx = 0;
		neto->devarr_readidx = 0;
		neto->searchcomplete = false;
		neto->btqueuesize = 0;
		if (kdBtInquireDevicesATX(KD_NULL, KD_BT_GIAC_ATX, neto) != 0)
			return(3);//E_ERROR
	#else
		neto->devarr_addidx = 0;
		neto->devarr_readidx = 0;
		return(2);//E_UNSUPPORTED
	#endif
	return(0);//E_OK
}

//Search for bluetooth devices or services
void ecNetStopBluetoothSearch(ECOREOBJ netobj)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
	neto->abortsearch = true;
}

//Fetch bluetooth services and ID's
//Errcode: E_OK, E_NOENTRY, E_NODATA, E_NOACTION
ecErr ecNetFetchBluetoothID(ECOREOBJ netobj, unsigned char *&data, unsigned char &type)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
	if (neto->devicearray == NULL || neto->abortsearch)
		return(3);//E_NOACTION
	if (neto->devarr_readidx < neto->devarr_addidx)
	{
		type = 0;
		data = &neto->devicearray[neto->devarr_readidx + 1];
		neto->devarr_readidx += neto->devicearray[neto->devarr_readidx];
		return(0);//E_OK
	}
	if (neto->searchcomplete)
		return(2);//E_NODATA
	return(1);//E_NOENTRY
}

//Start advertising bluetooth service
//Errcode: E_OK, E_UNSUPPORTED, E_ERROR
ecErr ecNetStartBluetoothService(ECOREOBJ netobj, ECOREOBJ sockobj, char *servicename, unsigned char port)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
	if (servicename[0] == 0)
		return(2);//E_ERROR
	ecNetStopBluetoothService(netobj);
	#if defined(DEVICE_WIN32)
		ecStrCpy(neto->btservicerec, servicename, 128);
		neto->btserviceport = port;
		if (eciNetHandleBTService(servicename, port, true) == SOCKET_ERROR)
			return(2);//E_ERROR
		return(0);//E_OK
	#elif defined(DEVICE_SYMBIAN)
		#if (DEVICE_SYMBIAN >= 610)
			if (neto->advertiser == NULL)
				neto->advertiser = CSymBTAdvertiser::NewL();
			if (neto->advertiser)
			{
				if (!neto->advertiser->StartAdvertising(servicename, port))
					return(2);//E_ERROR
				return(0);//E_OK
			}
		#endif
	#elif defined(DEVICE_OPENKODE)
		EC_SOCKOBJ *socko = (EC_SOCKOBJ *)sockobj;
		KDBtServiceRecordATX *rec;
		KDUuidATX uuid;
		uuid.i1 = neto->uid;
		uuid.i2 = 0x40B12164;
		uuid.i3 = 0x897F3023;
		uuid.i4 = 0x5C412012;
		rec = kdBtServiceRecordCreateATX(socko->sock, &uuid);
		if (rec == KD_NULL)
			return(2);//E_ERROR
		if (kdBtRegisterServiceATX(socko->sock, rec) == -1)
		{
			kdBtServiceRecordFreeATX(rec);
			return(2);//E_ERROR
		}
		kdBtServiceRecordFreeATX(rec);
		return(0);//E_OK
	#endif
	return(1);//E_UNSUPPORTED
}

//Stop advertising bluetooth service
void ecNetStopBluetoothService(ECOREOBJ netobj)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
	#if defined(DEVICE_WIN32)
		if (neto->btservicerec[0])
		{
			eciNetHandleBTService(neto->btservicerec, neto->btserviceport, false);
			neto->btservicerec[0] = 0;
		}
	#elif defined(DEVICE_SYMBIAN)
		#if (DEVICE_SYMBIAN >= 610)
			if (neto->advertiser)
			{
				neto->advertiser->StopAdvertising();
				delete neto->advertiser;
				neto->advertiser = NULL;
			}
		#endif
	#endif
}

//Return status of bluetooth
//0 = unknown, 1 = off, 2 = on, 3 = discoverable
unsigned char ecNetBluetoothState(ECOREOBJ netobj)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
	#if defined(DEVICE_WINCE)
		#if defined(DEVICE_WINMOBILE)
			if (neto->dll_btapi == NULL)
				neto->dll_btapi = LoadLibrary(TEXT("bthutil.dll"));
			if (neto->dll_btapi != NULL && neto->func_bthgetmode == NULL)
				neto->func_bthgetmode = (FUNC_BTHGETMODE)GetProcAddress(neto->dll_btapi, TEXT("BthGetMode"));
			if (neto->func_bthgetmode != NULL)
			{
				DWORD mode;
				neto->func_bthgetmode(&mode);
				if (mode == 0)
					return(1); //Off
				else if (mode == 1)
					return(2); //On
				else if (mode == 2)
					return(3); //Discoverable + on
			}
		#else
			if (neto->dll_btapi == NULL)
				neto->dll_btapi = LoadLibrary(TEXT("Btdrt.dll"));
			if (neto->dll_btapi != NULL && neto->func_bthgethardwarestatus == NULL)
				neto->func_bthgethardwarestatus = (FUNC_BTHGETHARDWARESTATUS)GetProcAddress(neto->dll_btapi, TEXT("BthGetHardwareStatus"));
			if (neto->func_bthgethardwarestatus != NULL)
			{
				int hcistatus;
				if (neto->func_bthgethardwarestatus(&hcistatus) == ERROR_SUCCESS)
				{
					if (hcistatus == HCI_HARDWARE_RUNNING)
						return(3); //Discoverable + on
					else if (hcistatus != HCI_HARDWARE_UNKNOWN)
						return(1); //Off
				}
			}
		#endif
	#endif
	return(0); //Unknown
}

//Set bluetooth status
void ecNetBluetoothSetState(ECOREOBJ netobj, unsigned char state)
{
	if (state == 0 || state > 3)
		return;
	if (ecNetBluetoothState(netobj) == state)
		return;
	EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
	#if defined(DEVICE_WINCE)
		#if defined(DEVICE_WINMOBILE)
			DWORD mode;
			if (neto->dll_btapi == NULL)
				neto->dll_btapi = LoadLibrary(TEXT("bthutil.dll"));
			if (neto->dll_btapi != NULL && neto->func_bthsetmode == NULL)
				neto->func_bthsetmode = (FUNC_BTHSETMODE)GetProcAddress(neto->dll_btapi, TEXT("BthSetMode"));
			if (neto->func_bthsetmode != NULL)
			{
				mode = state - 1;
				neto->func_bthsetmode(mode);
			}
		#else
			unsigned char safectr, kstate;
			UINT Bluetooth_Message = RegisterWindowMessage(TEXT("BT_MSG"));
			if (state == 1)
				kstate = 0;
			else
				kstate = 1;
			SendMessage(HWND_BROADCAST, Bluetooth_Message, kstate, 0);
			for (safectr = 0; safectr < 100; safectr++)
			{
				if (ecNetBluetoothState(netobj) == state)
				{
					Sleep(100);
					break;
				}
				Sleep(25);
			}
		#endif
	#elif defined(DEVICE_SYMBIAN)
	#endif
}

//Open internet connection
bool ecNetOpenInet(ECOREOBJ netobj)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_WINMOBILE)
			CONNMGR_CONNECTIONINFO info;
			ecMemSet(&info, 0, sizeof(CONNMGR_CONNECTIONINFO));
			info.cbSize      = sizeof(CONNMGR_CONNECTIONINFO);
			info.dwParams    = CONNMGR_PARAM_GUIDDESTNET;
			info.dwFlags     = CONNMGR_FLAG_PROXY_HTTP | CONNMGR_FLAG_PROXY_WAP;
			info.dwPriority  = CONNMGR_PRIORITY_USERINTERACTIVE;
			info.bExclusive  = FALSE;
			info.bDisabled   = FALSE;
			info.hWnd        = NULL;
			info.guidDestNet = IID_DestNetInternet;
			if (ConnMgrEstablishConnection(&info, &neto->hconn) == S_OK)
				return(true);
		#else
			return(true);
		#endif
	#elif defined(DEVICE_SYMBIAN)
		#if ((DEVICE_SYMBIAN >= 700) && !defined(DEVICE_UIQ)) || (DEVICE_SYMBIAN >= 800)
			TRequestStatus status;
			if (neto->rconn.Open(neto->sockserv, KConnectionTypeDefault) != KErrNone)
				return(false);
			if (neto->rconn.Start() != KErrNone)
			{
				neto->rconn.Close();
				return(false);
			}
			return(true);
		#else
			return(true);
		#endif
	#elif defined(DEVICE_MAEMO)
		DBusConnection * dbus;
		GMainLoop * gloop;
		gloop = g_main_loop_new(NULL, FALSE);
		ecNetGlibloop = gloop;
		dbus = dbus_bus_get(DBUS_BUS_SYSTEM, NULL);
		dbus_connection_setup_with_g_main(dbus, NULL);
		ConIcConnection * conic = con_ic_connection_new();
		if (!conic) 
		{
			g_main_loop_unref(gloop);
			return(false);
		}
		g_signal_connect(G_OBJECT(conic), "connection-event", G_CALLBACK(ecNetGLibMessage), NULL);
		ecNetMaemoStatus = 0;		
		if (con_ic_connection_connect(conic, CON_IC_CONNECT_FLAG_NONE) == FALSE)
		{
			g_main_loop_unref(gloop);
			g_object_unref(conic);
			return(false);
		}
		g_main_loop_run(gloop);	
		g_main_loop_unref(gloop);
		g_object_unref(conic);
		return((ecNetMaemoStatus == 1) ? true : false);	
	#else
		return(true);
	#endif
	return(false);
}

//Close internet connection
void ecNetCloseInet(ECOREOBJ netobj)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_WINMOBILE)
			ConnMgrReleaseConnection(neto->hconn, FALSE);
		#endif
	#elif defined(DEVICE_SYMBIAN)
		#if ((DEVICE_SYMBIAN >= 700) && !defined(DEVICE_UIQ)) || (DEVICE_SYMBIAN >= 800)
			neto->rconn.Close();
		#endif
	#endif
}

//Open socket object
//Protocol: 0 = TCP, 1 = Bluetooth
ECOREOBJ ecNetOpenSocket(ECOREOBJ netobj, unsigned char protocol)
{
	EC_SOCKOBJ *socko = new EC_SOCKOBJ;
	if (socko)
	{
		socko->parent = (EC_NETOBJ *)netobj;
		socko->protocol = protocol;
		socko->availport = 0;
		#if defined(DEVICE_WIN32)
			socko->alreadybound = false;
			if (protocol == 0)
				socko->sock = socket(AF_INET, SOCK_STREAM, 0);
			else
				socko->sock = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM);
			if (socko->sock == INVALID_SOCKET)
			{
				delete socko;
				socko = NULL;
			}
		#elif defined(DEVICE_SYMBIAN)
			TInt result;
			EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
			if (protocol == 0)
			{
				#if ((DEVICE_SYMBIAN >= 700) && !defined(DEVICE_UIQ)) || (DEVICE_SYMBIAN >= 800)
					result = socko->sock.Open(neto->sockserv, KAfInet, KSockStream, KProtocolInetTcp, neto->rconn);
				#else
					result = socko->sock.Open(neto->sockserv, KAfInet, KSockStream, KProtocolInetTcp);
				#endif
			}
			else
			{
				#if (DEVICE_SYMBIAN >= 610)
					_LIT(protocolrfcomm, "RFCOMM");
					result = socko->sock.Open(neto->sockserv, protocolrfcomm);
				#else
					result = KErrNone + 1;
				#endif
			}
			if (result != KErrNone)
			{
				delete socko;
				socko = NULL;
			}
		#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
			if (protocol == 0)
			{
				socko->sock = socket(AF_INET, SOCK_STREAM, 0);
				if (socko->sock == -1)
				{
					delete socko;
					socko = NULL;
				}
			}
			else
			{
				delete socko;
				socko = NULL;
			}
		#elif defined(DEVICE_OPENKODE)
			socko->numreads = 0;
			socko->bindfrombtport = false;
			if (protocol == 0)
				socko->sock = kdSocketCreate(KD_SOCK_TCP, socko);
			else
			{
				socko->sock = kdSocketCreate(KD_SOCK_RFCOMM_ATX, socko);
				if (kdBtSocketSetFlagsATX(socko->sock, 0) != 0)
				{
					kdSocketClose(socko->sock);
					socko->sock = NULL;
				}
			}
			if (socko->sock == KD_NULL)
			{
				delete socko;
				socko = NULL;
			}
		#endif
	}
	return(socko);
}

//Destroy socket object
void ecNetDestroySocket(ECOREOBJ netobj, ECOREOBJ sockobj)
{
	EC_SOCKOBJ *socko = (EC_SOCKOBJ *)sockobj;
	#if defined(DEVICE_WIN32)
		closesocket(socko->sock);
	#elif defined(DEVICE_SYMBIAN)
		socko->sock.Close();
	#elif defined(DEVICE_MAC) || defined(DEVICE_LINUX)
		close(socko->sock);
	#elif defined(DEVICE_OPENKODE)
		kdSocketClose(socko->sock);
	#endif
	delete socko;
}

///////////////////////////////////////////////////////////////////
// Sockets API
///////////////////////////////////////////////////////////////////

//Get available port
unsigned long ecNetSockAvailablePort(ECOREOBJ sockobj)
{
	EC_SOCKOBJ *socko = (EC_SOCKOBJ *)sockobj;
	if (socko->availport == 0)
	{
		#if defined(DEVICE_WIN32)
			if (!socko->alreadybound && socko->protocol == 1)
			{
				int namesize = sizeof(socko->boundaddr_bth);
				ecMemSet(&socko->boundaddr_bth, 0, sizeof(socko->boundaddr_bth));
				socko->boundaddr_bth.addressFamily = AF_BTH;
				#if defined(DEVICE_DESKTOP)
					socko->boundaddr_bth.port = BT_PORT_ANY;
				#else
					socko->boundaddr_bth.port = 0;
				#endif
				if (bind(socko->sock, (sockaddr *)&socko->boundaddr_bth, namesize) == 0)
				{
					getsockname(socko->sock, (sockaddr *)&socko->boundaddr_bth, &namesize);
					socko->availport = socko->boundaddr_bth.port;
					socko->alreadybound = true;
				}
			}
		#elif defined(DEVICE_SYMBIAN)
			#if (DEVICE_SYMBIAN >= 610)
				TInt channel;
				if (socko->sock.GetOpt(KRFCOMMGetAvailableServerChannel, KSolBtRFCOMM, channel) == KErrNone)
					socko->availport = channel;
			#endif
		#elif defined(DEVICE_OPENKODE)
			if (socko->protocol == 1)
			{
				KDSockaddr server_addr;
				ecMemSet(&server_addr, 0, sizeof(server_addr));
				server_addr.family = KD_AF_BLUETOOTH_ATX;
				if (kdSocketBind(socko->sock, &server_addr, false) == 0)
				{
					KDSockaddr name_addr;
					ecMemSet(&name_addr, 0, sizeof(KDSockaddr));
					if (kdSocketGetName(socko->sock, &name_addr) == 0)
						socko->availport = name_addr.data.sbtrcATX.channel;
					socko->bindfrombtport = true;
				}
			}
		#endif
	}
	return(socko->availport);
}

//Bind socket and return address
bool ecNetSockBind(ECOREOBJ sockobj, char *address, unsigned long port)
{
	EC_SOCKOBJ *socko = (EC_SOCKOBJ *)sockobj;
	address[0] = 0;
	socko->activeport = port;
	#if defined(DEVICE_WIN32)
		int namesize;
		if (socko->protocol == 0)
		{
			SOCKADDR_IN bsockaddr;
			unsigned short s_port = (unsigned short)port;
			bool foundaddr = false;
			SOCKET tmp_resolve = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			if (tmp_resolve != INVALID_SOCKET)
			{
				bsockaddr.sin_family = AF_INET;
				bsockaddr.sin_port = htons(s_port);
				bsockaddr.sin_addr.S_un.S_addr = (127 << 24) | 1;
				if (connect(tmp_resolve, (sockaddr *)&bsockaddr, sizeof(bsockaddr)) != SOCKET_ERROR)
				{
					namesize = sizeof(bsockaddr);
					if (getsockname(tmp_resolve, (sockaddr *)&bsockaddr, &namesize) != SOCKET_ERROR)
						if (bsockaddr.sin_addr.S_un.S_addr != 0)
						{
							ecStrCpy(address, inet_ntoa(*(LPIN_ADDR)&bsockaddr.sin_addr));
							foundaddr = true;
						}
				}
				shutdown(tmp_resolve, 1);
				closesocket(tmp_resolve);
			}
			if (!foundaddr)
			{
				struct hostent *lphostent;
				char hostname[64];
				unsigned long namelen = 64;
				gethostname(hostname, namelen);
				ecStrCpy(address, hostname);
				lphostent = gethostbyname(hostname);
				ecStrCpy(address, inet_ntoa(*(LPIN_ADDR)lphostent->h_addr_list[0]));
				bsockaddr.sin_addr = *(LPIN_ADDR)lphostent->h_addr_list[0];
			}
			namesize = sizeof(bsockaddr);
			bsockaddr.sin_family = AF_INET;
			bsockaddr.sin_port = htons(s_port);
			if (bind(socko->sock, (sockaddr *)&bsockaddr, namesize) == 0)
				return(true);
		}
		else
		{
			if (!socko->alreadybound)
			{
				namesize = sizeof(socko->boundaddr_bth);
				ecMemSet(&socko->boundaddr_bth, 0, sizeof(socko->boundaddr_bth));
				socko->boundaddr_bth.addressFamily = AF_BTH;
				socko->boundaddr_bth.port = port;
				if (bind(socko->sock, (sockaddr *)&socko->boundaddr_bth, namesize) == 0)
					socko->alreadybound = true;
			}
			if (socko->alreadybound)
			{
				namesize = sizeof(socko->boundaddr_bth);
				getsockname(socko->sock, (sockaddr *)&socko->boundaddr_bth, &namesize);
				eciNetBTAtoA(address, socko->boundaddr_bth.btAddr);
				return(true);
			}
		}
	#elif defined(DEVICE_SYMBIAN)
		TInt result;
		socko->server = true;
		if (socko->protocol == 0)
		{
			TSockAddr inetaddr;
			inetaddr.SetPort(port);
			result = socko->sock.Bind(inetaddr);
		}
		else
		{
			#if (DEVICE_SYMBIAN >= 610)
				TBTSockAddr btaddr;
				btaddr.SetPort(port);
				#if (DEVICE_SYMBIAN >= 800)
					TUid KUidEdge = { EdgeGetUIDNr() };
					TBTServiceSecurity serviceSecurity;
					serviceSecurity.SetUid(KUidEdge);
					serviceSecurity.SetAuthentication(EFalse);
					serviceSecurity.SetEncryption(EFalse);
					serviceSecurity.SetAuthorisation(EFalse);
					serviceSecurity.SetDenied(EFalse);
					btaddr.SetSecurity(serviceSecurity);
				#endif
				result = socko->sock.Bind(btaddr);
			#else
				result = KErrNone + 1;
			#endif
		}
		if (result != KErrNone)
			return(false);
		RHostResolver resolver;
		if (socko->protocol == 0)
		{
			if (!resolver.SubSessionHandle())
			{
				#if ((DEVICE_SYMBIAN >= 700) && !defined(DEVICE_UIQ)) || (DEVICE_SYMBIAN >= 800)
					result = resolver.Open(socko->parent->sockserv, KAfInet, KProtocolInetTcp, socko->parent->rconn);
				#else
					result = resolver.Open(socko->parent->sockserv, KAfInet, KProtocolInetTcp);
				#endif
				if (result == KErrNone)
				{
					/*TNameEntry nameentry;
					TRequestStatus status;
					WCHAR waddr[256];
					ClassEStd::StrCpy(waddr, "");
					THostName servname(waddr);
					hr.GetByName(servname, nameentry, status);
					User::WaitForRequest(status);
					if (status == KErrNone)
					{
						inethostaddr = nameentry().iAddr; 
						IPAtoA(btaddr, inethostaddr.Address());
					}*/
					resolver.Close();
				}
			}
		}
		else
		{
			#if (DEVICE_SYMBIAN >= 610)
				TProtocolDesc pInfo;
				if (socko->parent->sockserv.FindProtocol(_L("BTLinkManager"), pInfo) == KErrNone)
				{
					if (resolver.Open(socko->parent->sockserv, pInfo.iAddrFamily, pInfo.iProtocol) == KErrNone)
					{
						THostName name;
						if (resolver.GetHostName(name) == KErrNone)
						{
							for (long ctr = 0; ctr < name.Length(); ctr++)
							{
								address[ctr] = name[ctr];
								address[ctr + 1] = 0;
							}
							//TRequestStatus status;
							//TNameEntry entry;
							//resolver.GetByName(name, entry, status);
							//User::WaitForRequest(status);
							//eciNetBTAtoA(address, entry().iAddr);
						}
						else
							ecStrCpy(address, "Unknown");
						resolver.Close();
					}
				}
			#endif
		}
		return(true);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		socklen_t namesize;
		sockaddr_in bsockaddr;
		unsigned short s_port = (unsigned short)port;
		bool foundaddr = false;
		int tmp_resolve = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (tmp_resolve != -1)
		{
			bsockaddr.sin_family = AF_INET;
			bsockaddr.sin_port = htons(s_port);
			bsockaddr.sin_addr.s_addr = (127 << 24) | 1;
			if (connect(tmp_resolve, (sockaddr *)&bsockaddr, sizeof(bsockaddr)) != -1)
			{
				namesize = sizeof(bsockaddr);
				if (getsockname(tmp_resolve, (sockaddr *)&bsockaddr, &namesize) != -1)
					if (bsockaddr.sin_addr.s_addr != 0)
					{
						ecStrCpy(address, inet_ntoa(*(in_addr *)&bsockaddr.sin_addr));
						foundaddr = true;
					}
			}
			shutdown(tmp_resolve, 1);
		}
		if (!foundaddr)
		{
			struct hostent *lphostent;
			char hostname[64];
			unsigned long namelen = 64;
			namesize = sizeof(bsockaddr);
			gethostname(hostname, namelen);
			ecStrCpy(address, hostname);
			lphostent = gethostbyname(hostname);
			if (lphostent != NULL)
			{
				ecStrCpy(address, inet_ntoa(*(in_addr *)lphostent->h_addr_list[0]));
				bsockaddr.sin_addr.s_addr = inet_addr(address);
			}
			else
				bsockaddr.sin_addr.s_addr = 0;
		}
		bsockaddr.sin_family = AF_INET;
		bsockaddr.sin_port = htons(s_port);
		if (bind(socko->sock, (sockaddr *)&bsockaddr, namesize) == 0)
			return(true);
	#elif defined(DEVICE_OPENKODE)
		KDSockaddr server_addr;
		if (!socko->bindfrombtport)
		{
			if (socko->protocol == 0)
			{
				server_addr.family = KD_AF_INET;
				server_addr.data.sin.address = KD_INADDR_ANY;
				server_addr.data.sin.port = kdHtons(port);
			}
			else
			{
				ecMemSet(&server_addr, 0, sizeof(server_addr));
				server_addr.family = KD_AF_BLUETOOTH_ATX;
				server_addr.data.sbtrcATX.channel = port;
			}
			if (kdSocketBind(socko->sock, &server_addr, false) == 0)
			{
				KDSockaddr name_addr;
				ecMemSet(&name_addr, 0, sizeof(KDSockaddr));
				if (socko->protocol == 0)
				{
					name_addr.data.sin.port = kdHtons(port);
					name_addr.data.sin.address = KD_INADDR_ANY;
					if (kdSocketGetName(socko->sock, &name_addr) == 0)
						eciNetIPAtoA(address, name_addr.data.sin.address);
				}
				else
				{
					if (kdSocketGetName(socko->sock, &name_addr) == 0)
						eciNetBTAtoA(address, name_addr.data.sbtrcATX.bdaddr.b);
				}
				return(true);
			}
		}
		else if (socko->protocol == 1)
		{
			KDSockaddr name_addr;
			ecMemSet(&name_addr, 0, sizeof(KDSockaddr));
			if (kdSocketGetName(socko->sock, &name_addr) == 0)
				eciNetBTAtoA(address, name_addr.data.sbtrcATX.bdaddr.b);
			return(true);
		}
		return(false);
	#endif
	return(false);
}

//Listen on socket
bool ecNetSockListen(ECOREOBJ sockobj, unsigned char maxclients)
{
	EC_SOCKOBJ *socko = (EC_SOCKOBJ *)sockobj;
	#if defined(DEVICE_WIN32)
		if (listen(socko->sock, maxclients) == 0)
			return(true);
	#elif defined(DEVICE_SYMBIAN)
		#if ((DEVICE_SYMBIAN >= 610) && (DEVICE_SYMBIAN < 800))
			if (socko->protocol == 1)
			{
				TUid KUidEdge = { EdgeGetUIDNr() };
				RBTMan secManager;
				RBTSecuritySettings secSettingsSession;
				if (secManager.Connect() != KErrNone)
					return(false);
				CleanupClosePushL(secManager);
				if (secSettingsSession.Open(secManager) != KErrNone)
				{
					secManager.Close();
					CleanupStack::PopAndDestroy();
					return(false);
				}
				CleanupClosePushL(secSettingsSession);
				TBTServiceSecurity serviceSecurity(KUidEdge, KSolBtRFCOMM, 0);
				serviceSecurity.SetAuthentication(EFalse);
				serviceSecurity.SetEncryption(EFalse);
				serviceSecurity.SetAuthorisation(EFalse);
				serviceSecurity.SetChannelID(socko->activeport);
				TRequestStatus status;
				secSettingsSession.RegisterService(serviceSecurity, status);
				if (!WaitForRequestTimeout(status, 2000))
				{
					TRequestStatus tmp;
					secSettingsSession.CancelRequest(tmp);
					User::WaitForRequest(tmp);
				}
				CleanupStack::PopAndDestroy();
				CleanupStack::PopAndDestroy();
				if (status.Int() != KErrNone)
					return(false);
			}
		#endif
		if (socko->sock.Listen(maxclients) == KErrNone)
			return(true);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		if (listen(socko->sock, maxclients) == 0)
			return(true);
	#elif defined(DEVICE_OPENKODE)
		if (kdSocketListen(socko->sock, maxclients) == 0)
			return(true);
	#endif
	return(false);
}

//Shutdown socket
void ecNetSockShutdown(ECOREOBJ sockobj)
{
	EC_SOCKOBJ *socko = (EC_SOCKOBJ *)sockobj;
	#if defined(DEVICE_WIN32) || defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		//char tmpbuf[256];
		//unsigned long rctr = 0, len = 1;
		shutdown(socko->sock, 1);
		//while (len > 0 && rctr < 1024)
		//{
		//	len  = recv(socko->sock, tmpbuf, sizeof(tmpbuf), 0);
		//	rctr++;
		//}
	#elif defined(DEVICE_SYMBIAN)
		TRequestStatus status;
		socko->sock.CancelAll();
		socko->sock.Shutdown(RSocket::EImmediate, status);
		WaitForRequestTimeout(status, 1000);
	#endif
}

//Accept socket connection
ECOREOBJ ecNetSockAccept(ECOREOBJ sockobj, char *address)
{
	EC_SOCKOBJ *socko = (EC_SOCKOBJ *)sockobj;
	#if defined(DEVICE_WIN32)
		EC_SOCKOBJ *accsock = new EC_SOCKOBJ;
		if (accsock == NULL)
			return(NULL);
		int psize;
		accsock->parent = socko->parent;
		accsock->protocol = socko->protocol;
		accsock->availport = 0;
		accsock->activeport = 0;
		if (socko->protocol == 0)
		{
			SOCKADDR_IN asockaddr;
			psize = sizeof(asockaddr);
			accsock->sock = accept(socko->sock, (LPSOCKADDR)&asockaddr, &psize);
			ecStrCpy(address, inet_ntoa(asockaddr.sin_addr));
		}
		else
		{
			SOCKADDR_BTH SockAddrBthLocal = { 0 };
			psize = sizeof(SockAddrBthLocal);
			accsock->sock = accept(socko->sock, (LPSOCKADDR)&SockAddrBthLocal, &psize);
			eciNetBTAtoA(address, SockAddrBthLocal.btAddr);
		}
		if (accsock->sock == INVALID_SOCKET)
		{
			delete accsock;
			accsock = NULL;
		}
		return(accsock);
	#elif defined(DEVICE_SYMBIAN)
		EC_SOCKOBJ *accsock = new EC_SOCKOBJ;
		if (accsock == NULL)
			return(NULL);
		if (accsock->sock.Open(socko->parent->sockserv) == KErrNone)
		{
			TRequestStatus status;
			socko->sock.Accept(accsock->sock, status);
			if (!WaitForRequestTimeout(status, 2000))
			{
				socko->sock.CancelAccept();
				User::WaitForRequest(status);
			}
			if (status == KErrNone)
			{
				accsock->parent = socko->parent;
				accsock->protocol = socko->protocol;
				accsock->availport = 0;
				accsock->activeport = 0;
				accsock->server = false;
				if (socko->protocol == 0)
				{
					ecStrCpy(address, "");
				}
				else
				{
					#if (DEVICE_SYMBIAN >= 610)
						TBTSockAddr rAddr;
						accsock->sock.RemoteName(rAddr);
						eciNetBTAtoA(address, rAddr);
					#endif
				}
			}
			else
			{
				accsock->sock.Close();
				delete accsock;
				accsock = NULL;
			}
		}
		else
		{
			delete accsock;
			accsock = NULL;
		}
		return(accsock);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		EC_SOCKOBJ *accsock = new EC_SOCKOBJ;
		if (accsock == NULL)
			return(NULL);
		if (socko->protocol == 0)
		{
			sockaddr_in asockaddr;
			socklen_t tsize;
			accsock->sock = accept(socko->sock, (sockaddr *)&asockaddr, &tsize);
			accsock->parent = socko->parent;
			accsock->protocol = socko->protocol;
			accsock->availport = 0;
			accsock->activeport = 0;
			ecStrCpy(address, inet_ntoa(asockaddr.sin_addr));
		}
		if (accsock->sock == -1)
		{
			delete accsock;
			accsock = NULL;
		}
		return(accsock);
	#elif defined(DEVICE_OPENKODE)
		KDSockaddr asockaddr;
		EC_SOCKOBJ *accsock = new EC_SOCKOBJ;
		if (accsock == NULL)
			return(NULL);
		accsock->numreads = 0;
		ecMemSet(&asockaddr, 0, sizeof(KDSockaddr));
		accsock->sock = kdSocketAccept(socko->sock, &asockaddr, accsock);
		accsock->parent = socko->parent;
		accsock->protocol = socko->protocol;
		accsock->availport = 0;
		accsock->activeport = 0;
		if (socko->protocol == 0)
			eciNetIPAtoA(address, asockaddr.data.sin.address);
		else
			eciNetBTAtoA(address, asockaddr.data.sbtrcATX.bdaddr.b);
		if (accsock->sock == NULL)
		{
			socko->numreads = 0;
			delete accsock;
			accsock = NULL;
		}
		return(accsock);
	#endif
	return(NULL);
}

//Connect to address
bool ecNetSockConnect(ECOREOBJ sockobj, const char *host, unsigned long port, char *address)
{
	EC_SOCKOBJ *socko = (EC_SOCKOBJ *)sockobj;
	#if defined(DEVICE_WIN32)
		if (socko->protocol == 0)
		{
			SOCKADDR_IN csockaddr;
			unsigned long iaddr = inet_addr(host);
			unsigned short s_port = (unsigned short)port;
			csockaddr.sin_family = AF_INET;
			csockaddr.sin_port = htons(s_port);
			if (iaddr == INADDR_NONE)
			{
				struct hostent *lphostent;
				lphostent = gethostbyname(host);
				if (lphostent != NULL)
					csockaddr.sin_addr = *(LPIN_ADDR)lphostent->h_addr;
				else
					csockaddr.sin_addr.S_un.S_addr = 0;
			}
			else
				csockaddr.sin_addr.S_un.S_addr = iaddr;
			if (connect(socko->sock, (LPSOCKADDR)&csockaddr, sizeof(SOCKADDR_IN)) == 0)
			{
				int namelen = sizeof(SOCKADDR_IN);
				ecStrCpy(address, inet_ntoa(csockaddr.sin_addr));
				getsockname(socko->sock, (LPSOCKADDR)&csockaddr, &namelen);
				socko->parent->eventhandler(ECEV_NET_HOSTCONNECT, address, socko->parent->handler_userdata);
				return(true);
			}
		}
		else
		{
			GUID btGuid = {0x6aa63e0, 0x7d60, 0x41ff, 0xaf, 0xb2, 0x3e, 0xe6, 0xd2, 0xd9, 0x39, 0x2d};
			SOCKADDR_BTH SockAddrBthServer = {0};
			SockAddrBthServer.addressFamily  = AF_BTH;
			SockAddrBthServer.btAddr         = eciNetAtoBTA((char *)host);
			SockAddrBthServer.port           = port;
			SockAddrBthServer.serviceClassId = btGuid;
			if (connect(socko->sock, (struct sockaddr *)&SockAddrBthServer, sizeof(SOCKADDR_BTH)) == 0)
			{
				ecStrCpy(address, host);
				socko->parent->eventhandler(ECEV_NET_HOSTCONNECT, address, socko->parent->handler_userdata);
				return(true);
			}
		}
	#elif defined(DEVICE_SYMBIAN)
		TRequestStatus status;
		socko->server = false;
		if (socko->protocol == 0)
		{
			unsigned long iaddr = eciNetAtoIPA((char *)host);
			TInetAddr sockaddr;
			if (iaddr != 0xFFFFFFFF)
			{
				TUint32 KInetAddr = INET_ADDR((iaddr >> 24), ((iaddr >> 16) & 255), ((iaddr >> 8) & 255), (iaddr & 255));
				sockaddr.SetAddress(KInetAddr); 
				ecStrCpy(address, host);
			}
			else
			{
				RHostResolver resolver;
				if (!resolver.SubSessionHandle())
				{
					TInt result;
					#if ((DEVICE_SYMBIAN >= 700) && !defined(DEVICE_UIQ)) || (DEVICE_SYMBIAN >= 800)
						result = resolver.Open(socko->parent->sockserv, KAfInet, KProtocolInetTcp, socko->parent->rconn);
					#else
						result = resolver.Open(socko->parent->sockserv, KAfInet, KProtocolInetTcp);
					#endif
					if (result == KErrNone)
					{
						TNameEntry nameentry;
						unsigned short waddr[256];
						waddr[0] = 0;
						for (long ctr = 0; ctr < ecStrLen(host); ctr++)
						{
							waddr[ctr] = host[ctr];
							waddr[ctr + 1] = 0;
						}
						ecStrCpy(address, waddr);
						THostName servname(waddr);
						resolver.GetByName(servname, nameentry, status);
						if (!WaitForRequestTimeout(status, 5000))
						{
							resolver.Cancel();
							User::WaitForRequest(status);
						}
						if (status.Int() != KErrNone)
						{
							resolver.Close();
							return(false);
						}
						else
							sockaddr = nameentry().iAddr; 
						resolver.Close();
					}
					else
						return(false);
				}
				else
					return(false);
			}
			sockaddr.SetPort(port);
			socko->sock.Connect(sockaddr, status);
		}
		else
		{
			#if (DEVICE_SYMBIAN >= 610)
				TBTSockAddr sockaddr;
				#if (DEVICE_SYMBIAN >= 800)
					TUid KUidEdge = { EdgeGetUIDNr() };
					TBTServiceSecurity serviceSecurity;
					serviceSecurity.SetUid(KUidEdge);
					serviceSecurity.SetAuthentication(EFalse);
					serviceSecurity.SetEncryption(EFalse);
					serviceSecurity.SetAuthorisation(EFalse);
					serviceSecurity.SetDenied(EFalse);
					sockaddr.SetSecurity(serviceSecurity);
				#endif
				sockaddr = eciNetAtoBTA((char *)host);
				sockaddr.SetPort(port);
				socko->sock.Connect(sockaddr, status);
				ecStrCpy(address, host);
			#else
				return(false);
			#endif
		}
		if (!WaitForRequestTimeout(status, 5000))
		{
			socko->sock.CancelConnect();
			User::WaitForRequest(status);
		}
		if (status.Int() != KErrNone)
			return(false);
		socko->parent->eventhandler(ECEV_NET_HOSTCONNECT, address, socko->parent->handler_userdata);
		return(true);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		sockaddr_in csockaddr;
		unsigned long iaddr = inet_addr(host);
		unsigned short s_port = (unsigned short)port;
		csockaddr.sin_family = AF_INET;
		csockaddr.sin_port = htons(s_port);
		if (iaddr == 0xFFFFFFFF)
		{
			struct hostent *lphostent;
			lphostent = gethostbyname(host);
			if (lphostent != NULL)
			{
				ecStrCpy(address, inet_ntoa(*(in_addr *)lphostent->h_addr_list[0]));
				csockaddr.sin_addr.s_addr = inet_addr(address);
			}
			else
				return(false);
		}
		else
			csockaddr.sin_addr.s_addr = iaddr;
		if (connect(socko->sock, (sockaddr *)&csockaddr, sizeof(sockaddr_in)) == 0)
		{
			socklen_t namelen = sizeof(sockaddr_in);
			ecStrCpy(address, inet_ntoa(csockaddr.sin_addr));
			getsockname(socko->sock, (sockaddr *)&csockaddr, &namelen);
			socko->parent->eventhandler(ECEV_NET_HOSTCONNECT, address, socko->parent->handler_userdata);
			return(true);
		}
	#elif defined(DEVICE_OPENKODE)
		if (socko->protocol == 0)
		{
			socko->useport = port;
			if (kdNameLookup(KD_AF_INET, host, socko) == 0)
				return(true);
		}
		else
		{
			KDSockaddr server_addr;
			ecMemSet(&server_addr, 0, sizeof(KDSockaddr));
			socko->useport = port;
			server_addr.family = KD_AF_BLUETOOTH_ATX;
			server_addr.data.sbtrcATX.channel = port;
			eciNetAtoBTA(server_addr.data.sbtrcATX.bdaddr.b, host);
			if (kdSocketConnect(socko->sock, &server_addr) == 0)
			{
				socko->parent->eventhandler(ECEV_NET_HOSTCONNECT, (void *)host, socko->parent->handler_userdata);
				return(true);
			}
		}
	#endif
	return(false);
}

//Check socket state flags
//1 = read/accept, 2 = write, 4 = exception
unsigned char ecNetSockState(ECOREOBJ sockobj)
{
	EC_SOCKOBJ *socko = (EC_SOCKOBJ *)sockobj;
	#if defined(DEVICE_WIN32) || defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		unsigned char rflags = 0;
		timeval timev;
		fd_set writefields;
		fd_set readfields;
		fd_set errorfields;
		FD_ZERO(&writefields);
		FD_ZERO(&readfields);
		FD_ZERO(&errorfields);
		FD_SET(socko->sock, &writefields);
		FD_SET(socko->sock, &readfields);
		FD_SET(socko->sock, &errorfields);
		timev.tv_sec = 0;
		timev.tv_usec = 0;
		#if defined(DEVICE_LINUX) || defined(DEVICE_MAC)
			if (select(socko->sock+1, &readfields, &writefields, &errorfields, &timev) >= 0)
			{
				if (FD_ISSET(socko->sock, &readfields))
					rflags |= 1;
				if (FD_ISSET(socko->sock, &writefields))
					rflags |= 2;
				if (FD_ISSET(socko->sock, &errorfields))
					rflags |= 4;
			}
		#else
			if (select(0, &readfields, &writefields, &errorfields, &timev) >= 0)
			{
				if (FD_ISSET(socko->sock, &readfields))
					rflags |= 1;
				if (FD_ISSET(socko->sock, &writefields))
					rflags |= 2;
				if (FD_ISSET(socko->sock, &errorfields))
					rflags |= 4;
			}
		#endif
		return(rflags);
	#elif defined(DEVICE_SYMBIAN)
		unsigned char rflags = 0;
		TInt result;
		if (socko->sock.GetOpt(KSOSelectPoll, KSOLSocket, result) == KErrNone)
		{
			if (result & KSockSelectRead)
				rflags |= 1;
			if (!socko->server) //Hack through non-blocking sockets
				rflags |= 1;
			if (result & KSockSelectWrite)
				rflags |= 2;
			if (result & KSockSelectExcept)
				rflags |= 4;
		}
		else
			rflags |= 4;
		return(rflags);
	#elif defined(DEVICE_OPENKODE)
		if (socko->numreads)
			return(1);
	#endif
	return(0);
}

//Receive packets (return false when socket is closed)
bool ecNetSockRecv(ECOREOBJ sockobj, unsigned char *data, unsigned long &length)
{
	EC_SOCKOBJ *socko = (EC_SOCKOBJ *)sockobj;
	#if defined(DEVICE_WIN32) || defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		long buflen = recv(socko->sock, (char *)data, length, 0);
		if (buflen == -1)
		{
			#if defined(DEVICE_WIN32)
				if (WSAGetLastError() == WSAEMSGSIZE)
					buflen = length;
				else
					length = 0;
					return(false);
			#else
				length = 0;
				return(false);
			#endif
		}
		else if (buflen == 0)
		{
			length = 0;
			return(false);
		}
		length = buflen;
		return(true);
	#elif defined(DEVICE_SYMBIAN)
		TSockXfrLength len;
		TRequestStatus status;
		socko->sock.SetOpt(KSONonBlockingIO, KSOLSocket);
		socko->sock.RecvOneOrMore(socko->parent->sockreadbuf, 0, status, len);
		User::WaitForRequest(status);
		if (status.Int() == KErrNone)
		{
			length = socko->parent->sockreadbuf.Length();
			ecMemCpy(data, socko->parent->sockreadbuf.Ptr(), length);
		}
		else
		{
			length = 0;
			if (status.Int() == KErrEof || status.Int() == KErrDisconnected || status.Int() == KErrNotReady)
			{
				socko->sock.SetOpt(KSOBlockingIO, KSOLSocket);
				return(false);
			}
		}
		socko->sock.SetOpt(KSOBlockingIO, KSOLSocket);
		return(true);
	#elif defined(DEVICE_OPENKODE)
		long buflen;
		buflen = kdSocketRecv(socko->sock, data, length);
		if (buflen == -1)
		{
			length = 0;
			if (kdGetError() == KD_EAGAIN)
			{
				socko->numreads = 0;
				buflen = 0;
				return(true);
			}
			return(false);
		}
		else if (buflen == 0)
		{
			length = 0;
			return(false);
		}
		length = buflen;
		return(true);
	#endif
	length = 0;
	return(false);
}

//Send data through socket
bool ecNetSockSend(ECOREOBJ sockobj, const unsigned char *data, unsigned long length)
{
	if (length == 0)
		return(false);
	EC_SOCKOBJ *socko = (EC_SOCKOBJ *)sockobj;
	#if defined(DEVICE_WIN32)
		if (send(socko->sock, (const char *)data, length, 0) != SOCKET_ERROR)
			return(true);
	#elif defined(DEVICE_SYMBIAN)
		TPtrC8 dbuf;
		TRequestStatus status;
		dbuf.Set(data, length);
		socko->sock.Send(dbuf, 0, status);
		if (!WaitForRequestTimeout(status, 500))
		{
			socko->sock.CancelSend();
			User::WaitForRequest(status);
		}
		if (status.Int() == KErrNone)
			return(true);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		if (send(socko->sock, (const char *)data, length, 0) != -1)
			return(true);
	#elif defined(DEVICE_OPENKODE)
		if (kdSocketSend(socko->sock, data, length) != -1)
			return(true);
	#endif
	return(false);
}

//Register network event handler
void ecNetRegisterHandler(ECOREOBJ netobj, FUNC_NETHANDLER handler, void *userdata)
{
	EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
	neto->eventhandler = handler;
	neto->handler_userdata = userdata;
}

//Callback handler for event based systems
void ecNetOnHandler(ECOREOBJ netobj, unsigned short eventtype, void *eventdata)
{
	#if defined(DEVICE_OPENKODE)
		if (eventtype == ECEV_NET_HOSTLOOKUP)
		{
			EC_SOCKOBJ *socko = (EC_SOCKOBJ *)netobj;
			EC_NETOBJ *neto = (EC_NETOBJ *)socko->parent;
			KDEventNameLookup *lookup = (KDEventNameLookup *)eventdata;
			if (lookup->error == 0)
			{
				KDSockaddr server_addr;
				ecMemCpy(&server_addr, lookup->result, (KDsize)lookup->resultlen);
				server_addr.data.sin.port = kdHtons(socko->useport);
				server_addr.family = KD_AF_INET;
				eciNetIPAtoA(socko->foundaddr, server_addr.data.sin.address);
				kdSocketConnect(socko->sock, &server_addr);
			}
		}
		else if (eventtype == ECEV_NET_HOSTCONNECT)
		{
			EC_SOCKOBJ *socko = (EC_SOCKOBJ *)netobj;
			EC_NETOBJ *neto = (EC_NETOBJ *)socko->parent;
			KDEventSocketConnect *connect = (KDEventSocketConnect *)eventdata;
			socko->parent->eventhandler(ECEV_NET_HOSTCONNECT, socko->foundaddr, socko->parent->handler_userdata);
		}
		else if (eventtype == ECEV_NET_ADDCLIENT)
		{
			EC_SOCKOBJ *socko = (EC_SOCKOBJ *)netobj;
			socko->numreads++;
		}
		else if (eventtype == ECEV_NET_READABLE)
		{
			EC_SOCKOBJ *socko = (EC_SOCKOBJ *)netobj;
			socko->numreads++;
		}
		else if (eventtype == ECEV_NET_FOUNDBTDEVICE)
		{
			EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
			KDEventBtDeviceATX *device = (KDEventBtDeviceATX *)eventdata;
			if (neto->abortsearch || neto->btqueuesize >= 128)
				device->result = NULL;
			if (device->result)
			{
				ECI_KDBT_NETOBJ *kdbtobj = &neto->btqueue[neto->btqueuesize++];
				ecMemCpy(&kdbtobj->btaddr, device->result, sizeof(KDBdAddrATX));
				kdbtobj->arrsize = 21;
				kdbtobj->arraydata[0] = 18;
				eciNetBTAtoA((char *)&kdbtobj->arraydata[1], kdbtobj->btaddr.b);
				kdbtobj->arraydata[19] = 1;
				kdbtobj->arraydata[20] = 0;
			}
			else
			{
				if (neto->btqueuesize && !neto->abortsearch)
				{
					neto->btqueueidx = 0;
					kdBtGetFriendlyNameATX(KD_NULL, &neto->btqueue[neto->btqueueidx].btaddr, neto);
				}
				else
				{
					neto->searchcomplete = true;
					kdBtCancelInquireDevicesATX(netobj);
				}
			}
		}
		else if (eventtype == ECEV_NET_FOUNDBTSERVICE)
		{
			EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
			ECI_KDBT_NETOBJ *kdbtobj = (ECI_KDBT_NETOBJ *)&neto->btqueue[neto->btqueueidx];
			KDEventBtServiceATX *service = (KDEventBtServiceATX *)eventdata;
			bool endsearch = false;
			if (neto->abortsearch)
				service->result = NULL;
			if (service->result)
			{
				char servicename[80];
				ecStrCpy(servicename, "EDGELIB Serial Port [");
				ecIntToStr(&servicename[ecStrLen(servicename)], neto->uid, 16, false);
				ecStrCat(servicename, "]");
				kdbtobj->arraydata[20] = (unsigned char)(kdBtServiceRecordGetRfcommChannelATX(service->result));
				kdbtobj->arraydata[kdbtobj->arrsize - 2] = ecStrLen(servicename) + 1;
				ecStrCpy((char *)&kdbtobj->arraydata[kdbtobj->arrsize - 2 + 1], servicename);
				kdbtobj->arrsize += kdbtobj->arraydata[kdbtobj->arrsize - 2] - 2 + 1;
				if (kdbtobj->arraydata[20] > 0)
					if (!eciNetDeviceCallback(0, kdbtobj->arraydata, kdbtobj->arrsize, neto))
						neto->abortsearch = true;
			}
			else
			{
				if (neto->btqueueidx + 1 < neto->btqueuesize && !neto->abortsearch)
				{
					KDUuidATX uuid[2];
					neto->btqueueidx++;
					uuid[0].i1 = neto->uid;
					uuid[0].i2 = 0x40B12164;
					uuid[0].i3 = 0x897F3023;
					uuid[0].i4 = 0x5C412012;
					ecMemSet(&uuid[1], 0, sizeof(KDUuidATX));
					if (kdBtSearchServicesATX(KD_NULL, &neto->btqueue[neto->btqueueidx].btaddr, KD_NULL, uuid, neto) != 0)
						neto->abortsearch = true;
				}
				else
					endsearch = true;
			}
			if (neto->abortsearch || endsearch)
				neto->searchcomplete = true;
		}
		else if (eventtype == ECEV_NET_BTDEVICENAME)
		{
			EC_NETOBJ *neto = (EC_NETOBJ *)netobj;
			ECI_KDBT_NETOBJ *kdbtobj = (ECI_KDBT_NETOBJ *)&neto->btqueue[neto->btqueueidx];
			KDEventBtNameATX *name = (KDEventBtNameATX *)eventdata;
			char devicename[128];
			bool endsearch = false;
			if (name->result)
				ecStrCpy(devicename, name->result);
			else
				ecStrCpy(devicename, "");
			kdbtobj->arraydata[kdbtobj->arrsize] = ecStrLen(devicename) + 1;
			kdbtobj->arraydata[kdbtobj->arrsize + 1] = 0;
			ecStrCpy((char *)&kdbtobj->arraydata[kdbtobj->arrsize + 1], devicename);
			kdbtobj->arrsize += kdbtobj->arraydata[kdbtobj->arrsize] + 1;
			kdbtobj->arraydata[kdbtobj->arrsize] = 1;
			kdbtobj->arraydata[kdbtobj->arrsize + 1] = 0;
			kdbtobj->arrsize += kdbtobj->arraydata[kdbtobj->arrsize] + 1;
			if (!eciNetDeviceCallback(0, kdbtobj->arraydata, kdbtobj->arrsize, neto))
				neto->abortsearch = true;
			if (neto->btqueueidx + 1 < neto->btqueuesize && !neto->abortsearch)
			{
				neto->btqueueidx++;
				if (kdBtGetFriendlyNameATX(KD_NULL, &neto->btqueue[neto->btqueueidx].btaddr, neto) != 0)
					neto->abortsearch = true;
			}
			else
				endsearch = true;
			if (endsearch && !neto->abortsearch)
			{
				KDUuidATX uuid[2];
				neto->btqueueidx = 0;
				uuid[0].i1 = neto->uid;
				uuid[0].i2 = 0x40B12164;
				uuid[0].i3 = 0x897F3023;
				uuid[0].i4 = 0x5C412012;
				ecMemSet(&uuid[1], 0, sizeof(KDUuidATX));
				if (kdBtSearchServicesATX(KD_NULL, &neto->btqueue[neto->btqueueidx].btaddr, KD_NULL, uuid, neto) != 0)
					neto->abortsearch = true;
			}
			if (neto->abortsearch)
				neto->searchcomplete = true;
		}
	#endif
}
