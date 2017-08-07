// imp_bluetooth.h
// Import header: provides forward compatibility for bluetooth sockets and discoverer/advertiser API
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#if (DEVICE_WINMOBILE < 300)

///////////////////////////////////////////////////////////////////
// Winsock extension
///////////////////////////////////////////////////////////////////

#ifndef __CSADDR_DEFINED__
#define __CSADDR_DEFINED__

/*
 * SockAddr Information
 */
typedef struct _SOCKET_ADDRESS {
    LPSOCKADDR lpSockaddr ;
    INT iSockaddrLength ;
} SOCKET_ADDRESS, *PSOCKET_ADDRESS, FAR * LPSOCKET_ADDRESS ;

/*
 * CSAddr Information
 */
typedef struct _CSADDR_INFO {
    SOCKET_ADDRESS LocalAddr ;
    SOCKET_ADDRESS RemoteAddr ;
    INT iSocketType ;
    INT iProtocol ;
} CSADDR_INFO, *PCSADDR_INFO, FAR * LPCSADDR_INFO ;
#endif /* __CSADDR_DEFINED__ */

/*
 * Address list returned via SIO_ADDRESS_LIST_QUERY
 */
typedef struct _SOCKET_ADDRESS_LIST {
    INT             iAddressCount;
    SOCKET_ADDRESS  Address[1];
} SOCKET_ADDRESS_LIST, FAR * LPSOCKET_ADDRESS_LIST;

typedef struct _AFPROTOCOLS {
    INT iAddressFamily;
    INT iProtocol;
} AFPROTOCOLS, *PAFPROTOCOLS, *LPAFPROTOCOLS;

typedef enum _WSAEcomparator
{
    COMP_EQUAL = 0,
    COMP_NOTLESS
} WSAECOMPARATOR, *PWSAECOMPARATOR, *LPWSAECOMPARATOR;

typedef struct _WSAVersion
{
    DWORD           dwVersion;
    WSAECOMPARATOR  ecHow;
}WSAVERSION, *PWSAVERSION, *LPWSAVERSION;

typedef enum _WSAESETSERVICEOP
{
    RNRSERVICE_REGISTER=0,
    RNRSERVICE_DEREGISTER,
    RNRSERVICE_DELETE
} WSAESETSERVICEOP, *PWSAESETSERVICEOP, *LPWSAESETSERVICEOP;

typedef struct _WSAQuerySetA
{
    DWORD           dwSize;
    LPSTR           lpszServiceInstanceName;
    LPGUID          lpServiceClassId;
    LPWSAVERSION    lpVersion;
    LPSTR           lpszComment;
    DWORD           dwNameSpace;
    LPGUID          lpNSProviderId;
    LPSTR           lpszContext;
    DWORD           dwNumberOfProtocols;
    LPAFPROTOCOLS   lpafpProtocols;
    LPSTR           lpszQueryString;
    DWORD           dwNumberOfCsAddrs;
    LPCSADDR_INFO   lpcsaBuffer;
    DWORD           dwOutputFlags;
    LPBLOB          lpBlob;
} WSAQUERYSETA, *PWSAQUERYSETA, *LPWSAQUERYSETA;
typedef struct _WSAQuerySetW
{
    DWORD           dwSize;
    LPWSTR          lpszServiceInstanceName;
    LPGUID          lpServiceClassId;
    LPWSAVERSION    lpVersion;
    LPWSTR          lpszComment;
    DWORD           dwNameSpace;
    LPGUID          lpNSProviderId;
    LPWSTR          lpszContext;
    DWORD           dwNumberOfProtocols;
    LPAFPROTOCOLS   lpafpProtocols;
    LPWSTR          lpszQueryString;
    DWORD           dwNumberOfCsAddrs;
    LPCSADDR_INFO   lpcsaBuffer;
    DWORD           dwOutputFlags;
    LPBLOB          lpBlob;
} WSAQUERYSETW, *PWSAQUERYSETW, *LPWSAQUERYSETW;
#ifdef UNICODE
typedef WSAQUERYSETW WSAQUERYSET;
typedef PWSAQUERYSETW PWSAQUERYSET;
typedef LPWSAQUERYSETW LPWSAQUERYSET;
#else
typedef WSAQUERYSETA WSAQUERYSET;
typedef PWSAQUERYSETA PWSAQUERYSET;
typedef LPWSAQUERYSETA LPWSAQUERYSET;
#endif /* UNICODE */

#define LUP_DEEP                0x0001
#define LUP_CONTAINERS          0x0002
#define LUP_NOCONTAINERS        0x0004
#define LUP_NEAREST             0x0008
#define LUP_RETURN_NAME         0x0010
#define LUP_RETURN_TYPE         0x0020
#define LUP_RETURN_VERSION      0x0040
#define LUP_RETURN_COMMENT      0x0080
#define LUP_RETURN_ADDR         0x0100
#define LUP_RETURN_BLOB         0x0200
#define LUP_RETURN_ALIASES      0x0400
#define LUP_RETURN_QUERY_STRING 0x0800
#define LUP_RETURN_ALL          0x0FF0
#define LUP_RES_SERVICE         0x8000

#define LUP_FLUSHCACHE       0x1000
#define LUP_FLUSHPREVIOUS    0x2000

///////////////////////////////////////////////////////////////////
// Winsock bluetooth API
///////////////////////////////////////////////////////////////////

#define BTH_MAX_NAME_SIZE          (248)    // max length of device friendly name.

typedef ULONGLONG bt_addr, *pbt_addr, BT_ADDR, *PBT_ADDR;
typedef ULONG  bt_cod, BT_COD;
typedef ULONG  bt_lap, BT_LAP;

#define NAP_MASK                ((ULONGLONG) 0xFFFF00000000)
#define SAP_MASK                ((ULONGLONG) 0x0000FFFFFFFF)

#define NAP_BIT_OFFSET          (8 * 4)
#define SAP_BIT_OFFSET          (0)

#define GET_NAP(_bt_addr)       ((USHORT) (((_bt_addr) & NAP_MASK) >> NAP_BIT_OFFSET))
#define GET_SAP(_bt_addr)       ((ULONG)  (((_bt_addr) & SAP_MASK) >> SAP_BIT_OFFSET))

#define SET_NAP(_nap) (((ULONGLONG) ((USHORT) (_nap))) << NAP_BIT_OFFSET)
#define SET_SAP(_sap) (((ULONGLONG) ((ULONG)  (_sap))) << SAP_BIT_OFFSET)

#define SET_NAP_SAP(_nap, _sap) (SET_NAP(_nap) | SET_SAP(_sap))

// Turn 1 byte packing of structures on

#ifndef L2CAP_MAX_MTU
#define L2CAP_MAX_MTU  65535
#endif

#define BT_ADDR_NULL       0x000000000000
#define BT_ADDR_IAC_FIRST  0x9E8B00
#define BT_ADDR_IAC_LAST   0x9E8B3f
#define BT_ADDR_LIAC       0x9E8B00
#define BT_ADDR_GIAC       0x9E8B33

#define BT_PORT_NULL       0
#define BT_PORT_WILDCARD   0
#define BT_PORT_ANY        -1
#define BT_PORT_MIN        0x1
#define BT_PORT_MAX        0xffff
#define BT_PORT_DYN_FIRST  0x1001
#define BT_PORT_EXCL_BIT   0x0100

#ifndef AF_BTH
#define AF_BTH  32
#define	AF_BT				AF_BTH		
#define WINDOWS_AF_BT		AF_BTH
#undef  AF_MAX
#define AF_MAX  33
#endif
#ifndef PF_BTH
#define PF_BTH  AF_BTH
#undef  PF_MAX
#define PF_MAX  AF_BTH
#endif

#define NS_BTH  16

DEFINE_GUID(SVCID_BTH_PROVIDER, 0x6aa63e0, 0x7d60, 0x41ff, 0xaf, 0xb2, 0x3e, 0xe6, 0xd2, 0xd9, 0x39, 0x2d);


// Bluetooth protocol #s are assigned according to the Bluetooth
// Assigned Numbers portion of the Bluetooth Specification
#define BTHPROTO_RFCOMM  0x0003
#define BTHPROTO_L2CAP   0x0100

#define SDP_ENUMDEVICES   0x00000010
#define SDP_SET           0x00000011
#define SDP_QUERY         0x00000012

#define RFCOMM_FLOW_CONTROL   0x00000013

#define SOL_RFCOMM  0x03
#define SOL_BTHTDI  0x100
#define SOL_SDP     0x0101

typedef struct _BTH_SOCKOPT_SECURITY {
	int				iLength;	// == 0 or 16 for link key, 0<=..<=16 for PIN. 0 = revoke
	bt_addr			btAddr;
	unsigned char	caData[16];
} BTH_SOCKOPT_SECURITY, *PBTH_SOCKOPT_SECURITY;

typedef struct _BTH_LOCAL_VERSION {
	unsigned char	hci_version;
	unsigned short	hci_revision;
	unsigned char	lmp_version;
	unsigned short	lmp_subversion;
	unsigned short	manufacturer;
	unsigned char	lmp_features[8];
} BTH_LOCAL_VERSION, *PBTH_LOCAL_VERSION;

typedef struct _BTH_REMOTE_VERSION {
	unsigned char	lmp_version;
	unsigned short	lmp_subversion;
	unsigned short	manufacturer;
	unsigned char	lmp_features[8];
} BTH_REMOTE_VERSION, *PBTH_REMOTE_VERSION;

typedef struct _BTH_REMOTE_NAME {
	BT_ADDR			bt;
	WCHAR			szNameBuffer[248];
} BTH_REMOTE_NAME, *PBTH_REMOTE_NAME;

typedef struct _BTH_HOLD_MODE {
	unsigned short	hold_mode_max;
	unsigned short	hold_mode_min;
	unsigned short	interval;	// out
} BTH_HOLD_MODE, *PBTH_HOLD_MODE;

typedef struct _BTH_SNIFF_MODE {
	unsigned short	sniff_mode_max;
	unsigned short	sniff_mode_min;
	unsigned short	sniff_attempt;
	unsigned short	sniff_timeout;
	unsigned short	interval;	// out
} BTH_SNIFF_MODE, *PBTH_SNIFF_MODE;

typedef struct _BTH_PARK_MODE {
	unsigned short beacon_max;
	unsigned short beacon_min;
	unsigned short	interval;	// out
} BTH_PARK_MODE, *PBTH_PARK_MODE;

#define SO_BTH_AUTHENTICATE			0x00000001	// optlen=0, optval ignored
#define SO_BTH_ENCRYPT				0x00000002	// optlen=sizeof(unsigned int), optval = &(unsigned int)TRUE/FALSE
#define SO_BTH_SET_PIN				0x00000003	// bound only! survives socket! optlen=sizeof(BTH_SOCKOPT_SECURITY), optval=&BTH_SOCKOPT_SECURITY
#define SO_BTH_SET_LINK				0x00000004	// bound only! survives socket! optlen=sizeof(BTH_SOCKOPT_SECURITY), optval=&BTH_SOCKOPT_SECURITY
#define SO_BTH_GET_LINK				0x00000005	// bound only! optlen=sizeof(BTH_SOCKOPT_SECURITY), optval=&BTH_SOCKOPT_SECURITY
#define SO_BTH_SET_MTU				0x00000006	// unconnected only! optlen=sizeof(unsigned int), optval = &mtu
#define SO_BTH_GET_MTU				0x00000007	// optlen=sizeof(unsigned int), optval = &mtu
#define SO_BTH_SET_MTU_MAX			0x00000008	// unconnected only! optlen=sizeof(unsigned int), optval = &max. mtu
#define SO_BTH_GET_MTU_MAX			0x00000009	// bound only! optlen=sizeof(unsigned int), optval = &max. mtu
#define SO_BTH_SET_MTU_MIN			0x0000000a	// unconnected only! optlen=sizeof(unsigned int), optval = &min. mtu
#define SO_BTH_GET_MTU_MIN			0x0000000b	// bound only! optlen=sizeof(unsigned int), optval = &min. mtu
#define SO_BTH_SET_XON_LIM			0x0000000c	// optlen=sizeof(unsigned int), optval = &xon limit (set flow off)
#define SO_BTH_GET_XON_LIM			0x0000000d	// optlen=sizeof(unsigned int), optval = &xon
#define SO_BTH_SET_XOFF_LIM			0x0000000e	// optlen=sizeof(unsigned int), optval = &xoff limit (set flow on)
#define SO_BTH_GET_XOFF_LIM			0x0000000f	// optlen=sizeof(unsigned int), optval = &xoff
#define SO_BTH_SET_SEND_BUFFER		0x00000010	// optlen=sizeof(unsigned int), optval = &max buffered size for send
#define SO_BTH_GET_SEND_BUFFER		0x00000011	// optlen=sizeof(unsigned int), optval = &max buffered size for send
#define SO_BTH_SET_RECV_BUFFER		0x00000012	// optlen=sizeof(unsigned int), optval = &max buffered size for recv
#define SO_BTH_GET_RECV_BUFFER		0x00000013	// optlen=sizeof(unsigned int), optval = &max buffered size for recv
#define SO_BTH_GET_V24_BR			0x00000014	// connected only! optlen=2*sizeof(unsigned int), optval = &{v24 , br}
#define SO_BTH_GET_RLS				0x00000015	// connected only! optlen=sizeof(unsigned int), optval = &rls
#define SO_BTH_SEND_MSC				0x00000016	// connected only! optlen=2*sizeof(unsigned int), optval = &{v24, br}
#define SO_BTH_SEND_RLS				0x00000017	// connected only! optlen=sizeof(unsigned int), optval = &rls
#define SO_BTH_GET_FLOW_TYPE		0x00000018	// connected only! optlen=sizeof(unsigned int), optval=&1=credit-based, 0=legacy
#define SO_BTH_SET_PAGE_TO			0x00000019	// no restrictions. optlen=sizeof(unsigned int), optval = &page timeout
#define SO_BTH_GET_PAGE_TO			0x0000001a	// no restrictions. optlen=sizeof(unsigned int), optval = &page timeout
#define SO_BTH_SET_SCAN				0x0000001b	// no restrictions. optlen=sizeof(unsigned int), optval = &scan mode
#define SO_BTH_GET_SCAN				0x0000001c	// no restrictions. optlen=sizeof(unsigned int), optval = &scan mode
#define SO_BTH_SET_COD				0x0000001d	// no restrictions. optlen=sizeof(unsigned int), optval = &cod
#define SO_BTH_GET_COD				0x0000001e	// no restrictions. optlen=sizeof(unsigned int), optval = &cod
#define SO_BTH_GET_LOCAL_VER		0x0000001f	// no restrictions. optlen=sizeof(BTH_LOCAL_VERSION), optval = &BTH_LOCAL_VERSION
#define SO_BTH_GET_REMOTE_VER		0x00000020	// connected only! optlen=sizeof(BTH_REMOTE_VERSION), optval = &BTH_REMOTE_VERSION
#define SO_BTH_GET_AUTHN_ENABLE		0x00000021	// no restrictions. optlen=sizeof(unsigned int), optval = &authentication enable
#define SO_BTH_SET_AUTHN_ENABLE		0x00000022	// no restrictions. optlen=sizeof(unsigned int), optval = &authentication enable
#define SO_BTH_SET_READ_REMOTE_NAME	0x00000023	// no restrictions. optlen=sizeof(BTH_REMOTE_NAME), optval=&BTH_REMOTE_NAME
#define SO_BTH_GET_LINK_POLICY		0x00000024	// connected only! optlen=sizeof(unsigned int), optval = &link policy
#define SO_BTH_SET_LINK_POLICY		0x00000025	// connected only! optlen=sizeof(unsigned int), optval = &link policy
#define SO_BTH_ENTER_HOLD_MODE		0x00000026  // connected only! optlen=sizeof(BTH_HOLD_MODE), optval = &BTH_HOLD_MODE
#define SO_BTH_ENTER_SNIFF_MODE		0x00000027  // connected only! optlen=sizeof(BTH_SNIFF_MODE), optval = &BTH_SNIFF_MODE
#define SO_BTH_EXIT_SNIFF_MODE		0x00000028  // connected only! optlen=0, optval - ignored
#define SO_BTH_ENTER_PARK_MODE		0x00000029  // connected only! optlen=sizeof(BTH_PARK_MODE), optval = &BTH_PARK_MODE
#define SO_BTH_EXIT_PARK_MODE		0x0000002a  // connected only! optlen=0, optval - ignored
#define SO_BTH_GET_MODE				0x0000002b	// connected only! optlen=sizeof(int), optval = &mode

typedef struct _SOCKADDR_BTH
{
    USHORT   addressFamily;
    bt_addr  btAddr;
    GUID     serviceClassId;
    ULONG    port;
} SOCKADDR_BTH, *PSOCKADDR_BTH;

typedef struct __bth_inquiry_result {
	BT_ADDR			ba;
	unsigned int	cod;
	unsigned short	clock_offset;
	unsigned char	page_scan_mode;
	unsigned char	page_scan_period_mode;
	unsigned char   page_scan_repetition_mode;
} BthInquiryResult;

#define BTH_SDP_VERSION 1
typedef struct _BTHNS_SETBLOB
    {
    ULONG __RPC_FAR *pSdpVersion;
    ULONG __RPC_FAR *pRecordHandle;
    ULONG Reserved[ 4 ];
    ULONG fSecurity;
    ULONG fOptions;
    ULONG ulRecordLength;
    UCHAR pRecord[ 1 ];
    }	BTHNS_SETBLOB;

typedef struct _BTHNS_SETBLOB __RPC_FAR *PBTHNS_SETBLOB;

///////////////////////////////////////////////////////////////////
// Imported API
///////////////////////////////////////////////////////////////////

//Import function definitions
typedef INT (*FUNC_WSALOOKUPSERVICEBEGIN)(IN LPWSAQUERYSETW lpqsRestrictions, IN DWORD dwControlFlags, OUT LPHANDLE lphLookup);
typedef INT (*FUNC_WSALOOKUPSERVICENEXT)(IN HANDLE hLookup, IN DWORD dwControlFlags, IN OUT LPDWORD lpdwBufferLength, OUT LPWSAQUERYSETW lpqsResults);
typedef INT (*FUNC_WSALOOKUPSERVICEEND)(IN HANDLE hLookup);
typedef INT (*FUNC_WSASETSERVICE)(IN LPWSAQUERYSETW lpqsRegInfo, IN WSAESETSERVICEOP essoperation, IN DWORD dwControlFlags);

//Global dll handle and import functions
HMODULE g_dll_bluetooth = NULL;
FUNC_WSALOOKUPSERVICEBEGIN g_func_wsalookupservicebegin = NULL;
FUNC_WSALOOKUPSERVICENEXT g_func_wsalookupservicenext = NULL;
FUNC_WSALOOKUPSERVICEEND g_func_wsalookupserviceend = NULL;
FUNC_WSASETSERVICE g_func_wsasetservice = NULL;

//Start service discovery
INT WINAPI WSALookupServiceBegin(IN LPWSAQUERYSETW lpqsRestrictions, IN DWORD dwControlFlags, OUT LPHANDLE lphLookup)
{
	if (g_dll_bluetooth == NULL)
		g_dll_bluetooth = LoadLibrary(TEXT("ws2.dll"));
	if (g_dll_bluetooth != NULL && g_func_wsalookupservicebegin == NULL)
		g_func_wsalookupservicebegin = (FUNC_WSALOOKUPSERVICEBEGIN)GetProcAddress(g_dll_bluetooth, TEXT("WSALookupServiceBeginW"));
	if (g_func_wsalookupservicebegin != NULL)
		return(g_func_wsalookupservicebegin(lpqsRestrictions, dwControlFlags, lphLookup));
	return(SOCKET_ERROR);
}

//Continue service discovery
INT WINAPI WSALookupServiceNext(IN HANDLE hLookup, IN DWORD dwControlFlags, IN OUT LPDWORD lpdwBufferLength, OUT LPWSAQUERYSETW lpqsResults)
{
	if (g_dll_bluetooth == NULL)
		g_dll_bluetooth = LoadLibrary(TEXT("ws2.dll"));
	if (g_dll_bluetooth != NULL && g_func_wsalookupservicenext == NULL)
		g_func_wsalookupservicenext = (FUNC_WSALOOKUPSERVICENEXT)GetProcAddress(g_dll_bluetooth, TEXT("WSALookupServiceNextW"));
	if (g_func_wsalookupservicenext != NULL)
		return(g_func_wsalookupservicenext(hLookup, dwControlFlags, lpdwBufferLength, lpqsResults));
	return(SOCKET_ERROR);
}

//End service discovery
INT WINAPI WSALookupServiceEnd(IN HANDLE hLookup)
{
	if (g_dll_bluetooth == NULL)
		g_dll_bluetooth = LoadLibrary(TEXT("ws2.dll"));
	if (g_dll_bluetooth != NULL && g_func_wsalookupserviceend == NULL)
		g_func_wsalookupserviceend = (FUNC_WSALOOKUPSERVICEEND)GetProcAddress(g_dll_bluetooth, TEXT("WSALookupServiceEnd"));
	if (g_func_wsalookupserviceend != NULL)
		return(g_func_wsalookupserviceend(hLookup));
	return(SOCKET_ERROR);
}

//Start service
INT WINAPI WSASetService(IN LPWSAQUERYSETW lpqsRegInfo, IN WSAESETSERVICEOP essoperation, IN DWORD dwControlFlags)
{
	if (g_dll_bluetooth == NULL)
		g_dll_bluetooth = LoadLibrary(TEXT("ws2.dll"));
	if (g_dll_bluetooth != NULL && g_func_wsasetservice == NULL)
		g_func_wsasetservice = (FUNC_WSASETSERVICE)GetProcAddress(g_dll_bluetooth, TEXT("WSASetServiceW"));
	if (g_func_wsasetservice != NULL)
		return(g_func_wsasetservice(lpqsRegInfo, essoperation, dwControlFlags));
	return(SOCKET_ERROR);
}

#endif
