// imp_powerapi.h
// Import header: provides forward compatibility for power state changing
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

HMODULE g_dll_power = NULL;

///////////////////////////////////////////////////////////////////
// Set power requirement
///////////////////////////////////////////////////////////////////

#if (DEVICE_WINCE < 400)

//Defines
#define POWER_NAME              (DWORD)(0x00000001) // default

// Device Power States
typedef enum _CEDEVICE_POWER_STATE {
    PwrDeviceUnspecified = -1,
    D0 = 0, // Full On: full power,  full functionality
    D1,     // Low Power On: fully functional at low power/performance
    D2,     // Standby: partially powered with automatic wake
    D3,     // Sleep: partially powered with device initiated wake
    D4,     // Off: unpowered
    PwrDeviceMaximum
}CEDEVICE_POWER_STATE, *PCEDEVICE_POWER_STATE;

//Import function definitions
typedef HANDLE (*FUNC_SETPOWERREQUIREMENT)(PVOID pvDevice, CEDEVICE_POWER_STATE DeviceState, ULONG DeviceFlags, PVOID pvSystemState, ULONG StateFlags);
typedef DWORD (*FUNC_RELEASEPOWERREQUIREMENT)(HANDLE hPowerReq);

//Imported power requirement functions
FUNC_SETPOWERREQUIREMENT g_func_setpowerrequirement = NULL;
FUNC_RELEASEPOWERREQUIREMENT g_func_releasepowerrequirement = NULL;

//Set power requirement
HANDLE WINAPI SetPowerRequirement(PVOID pvDevice, CEDEVICE_POWER_STATE DeviceState, ULONG DeviceFlags, PVOID pvSystemState, ULONG StateFlags)
{
	if (g_dll_power == NULL)
		g_dll_power = LoadLibrary(TEXT("coredll.dll"));
	if (g_dll_power != NULL && g_func_setpowerrequirement == NULL)
		g_func_setpowerrequirement = (FUNC_SETPOWERREQUIREMENT)GetProcAddress(g_dll_power, TEXT("SetPowerRequirement"));
	if (g_func_setpowerrequirement != NULL)
		return(g_func_setpowerrequirement(pvDevice, DeviceState, DeviceFlags, pvSystemState, StateFlags));
	return(0);
}

//Release power requirement
DWORD WINAPI ReleasePowerRequirement(HANDLE hPowerReq)
{
	if (g_dll_power == NULL)
		g_dll_power = LoadLibrary(TEXT("coredll.dll"));
	if (g_dll_power != NULL && g_func_releasepowerrequirement == NULL)
		g_func_releasepowerrequirement = (FUNC_RELEASEPOWERREQUIREMENT)GetProcAddress(g_dll_power, TEXT("ReleasePowerRequirement"));
	if (g_func_releasepowerrequirement != NULL)
		return(g_func_releasepowerrequirement(hPowerReq));
	return(0);
}

#endif

#if (DEVICE_WINCE < 500)

//Import function definitions
typedef DWORD (*FUNC_SETSYSTEMPOWERSTATE)(LPCWSTR psState, DWORD StateFlags, DWORD Options);
typedef DWORD (*FUNC_GETSYSTEMPOWERSTATE)(LPWSTR pBuffer, DWORD Length, PDWORD pFlags);

//Imported power status functions
FUNC_SETSYSTEMPOWERSTATE g_func_setsystempowerstate = NULL;
FUNC_GETSYSTEMPOWERSTATE g_func_getsystempowerstate = NULL;
bool func_getpowerstatefail = false;

//Set system power state
DWORD WINAPI SetSystemPowerState(LPCWSTR psState, DWORD StateFlags, DWORD Options)
{
	if (g_dll_power == NULL)
		g_dll_power = LoadLibrary(TEXT("coredll.dll"));
	if (g_dll_power != NULL && g_func_setsystempowerstate == NULL)
		g_func_setsystempowerstate = (FUNC_SETSYSTEMPOWERSTATE)GetProcAddress(g_dll_power, TEXT("SetSystemPowerState"));
	if (g_func_setsystempowerstate != NULL)
		return(g_func_setsystempowerstate(psState, StateFlags, Options));
	return(ERROR_NOT_FOUND);
}

//Get system power state
DWORD WINAPI GetSystemPowerState(LPWSTR pBuffer, DWORD Length, PDWORD pFlags)
{
	if (func_getpowerstatefail)
		return(ERROR_NOT_FOUND);
	if (g_dll_power == NULL)
		g_dll_power = LoadLibrary(TEXT("coredll.dll"));
	if (g_dll_power != NULL && g_func_getsystempowerstate == NULL)
		g_func_getsystempowerstate = (FUNC_GETSYSTEMPOWERSTATE)GetProcAddress(g_dll_power, TEXT("GetSystemPowerState"));
	if (g_func_getsystempowerstate != NULL)
		return(g_func_getsystempowerstate(pBuffer, Length, pFlags));
	else
		func_getpowerstatefail = true;
	return(ERROR_NOT_FOUND);
}

#endif

//Restore previous backlight settings
void WINAPI RegistryBackLightOff(DWORD handle)
{
	if (handle)
	{
		DWORD curset_actimeout = (handle >> 16);
		DWORD curset_batterytimeout = handle & 0xFFFF;
		HKEY keysetting;
		unsigned long result, rtype, ka = KEY_ALL_ACCESS;
		rtype = REG_DWORD;
		if (curset_actimeout == 0xFFFF)
			curset_actimeout = 0x7FFFFFFF;
		if (curset_batterytimeout == 0xFFFF)
			curset_batterytimeout = 0x7FFFFFFF;
		RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("\\ControlPanel\\Backlight"), 0, NULL, 0, ka, NULL, &keysetting, &result);
		RegSetValueEx(keysetting, TEXT("ACTimeout"), NULL, rtype, (PBYTE)&curset_actimeout, sizeof(DWORD));
		RegSetValueEx(keysetting, TEXT("BatteryTimeout"), NULL, rtype, (PBYTE)&curset_batterytimeout, sizeof(DWORD));
		RegCloseKey(keysetting);
		HANDLE tevent = CreateEvent(NULL, FALSE, FALSE, TEXT("BackLightChangeEvent"));
		if (tevent)
			SetEvent(tevent);
	}
}

//Turn backlight on in registry settings and fire event
DWORD WINAPI RegistryBackLightOn(void)
{
	DWORD curset_actimeout;
	DWORD curset_batterytimeout;
	HKEY keysetting;
	unsigned long result, rtype, rsize, ka = KEY_ALL_ACCESS;
	rtype = REG_DWORD;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("\\ControlPanel\\Backlight"), 0, NULL, 0, ka, NULL, &keysetting, &result) == ERROR_SUCCESS)
	{
		if (RegQueryValueEx(keysetting, TEXT("ACTimeout"), NULL, &rtype, (PBYTE)&curset_actimeout, &rsize) != ERROR_SUCCESS)
			curset_actimeout = 0x7FFFFFFF;
		if (RegQueryValueEx(keysetting, TEXT("BatteryTimeout"), NULL, &rtype, (PBYTE)&curset_batterytimeout, &rsize) != ERROR_SUCCESS)
			curset_batterytimeout = 0x7FFFFFFF;
		RegCloseKey(keysetting);
	}
	else
		return(0);
	if (curset_actimeout > 0xFFFF)
		curset_actimeout = 0xFFFF;
	if (curset_batterytimeout > 0xFFFF)
		curset_batterytimeout = 0xFFFF;
	RegistryBackLightOff(0xFFFFFFFF);
	return((curset_actimeout << 16) | curset_batterytimeout);
}

///////////////////////////////////////////////////////////////////
// Idle timer reset
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_WINMOBILE)
#if defined(DEVICE_POCKETPC) && (DEVICE_WINMOBILE < 500)

//Import
HMODULE g_dll_tpcshell = NULL;
typedef void (*FUNC_SHIDLETIMERRESET)(void);
FUNC_SHIDLETIMERRESET g_func_shidletimerreset = NULL;
bool func_shidletimerfail = false;

//Reset shell idle timer
void WINAPI SHIdleTimerReset()
{
	if (func_shidletimerfail)
		return;
	if (g_dll_tpcshell == NULL)
		g_dll_tpcshell = LoadLibrary(TEXT("aygshell.dll"));
	if (g_dll_tpcshell != NULL && g_func_shidletimerreset == NULL)
		g_func_shidletimerreset = (FUNC_SHIDLETIMERRESET)GetProcAddress(g_dll_tpcshell, TEXT("SHIdleTimerReset"));
	if (g_func_shidletimerreset != NULL)
		g_func_shidletimerreset();
	else
		func_shidletimerfail = true;
}

#else
#include <tpcshell.h>
#endif
#endif

///////////////////////////////////////////////////////////////////
// Power notifier
///////////////////////////////////////////////////////////////////

#if (DEVICE_WINCE < 400)
	//Message queue options
	typedef struct MSGQUEUEOPTIONS_OS
	{
		DWORD dwSize;                           // size of the structure
		DWORD dwFlags;                          // behavior of message queue
		DWORD dwMaxMessages;                    // max # of msgs in queue
		DWORD cbMaxMessage;                     // max size of msg
		BOOL  bReadAccess;                      // read access requested
	} MSGQUEUEOPTIONS, FAR *LPMSGQUEUEOPTIONS, *PMSGQUEUEOPTIONS;
#endif

//Import function definitions
typedef HANDLE (*FUNC_CREATEMSGQUEUE)(LPCWSTR lpName, LPMSGQUEUEOPTIONS lpOptions);
typedef BOOL (*FUNC_CLOSEMSGQUEUE)(HANDLE hMsgQ);
typedef DWORD (*FUNC_READMSGQUEUE)(HANDLE hMsgQ, LPVOID lpBuffer, DWORD cbBufferSize, LPDWORD lpNumberOfBytesRead, DWORD dwTimeout, DWORD *pdwFlags);

//Imported message queue functions
FUNC_CREATEMSGQUEUE g_func_createmsgqueue = NULL;
FUNC_CLOSEMSGQUEUE g_func_closemsgqueue = NULL;
FUNC_READMSGQUEUE g_func_readmsgqueue = NULL;

//Create message queue
HANDLE Imp_CreateMsgQueue(LPCWSTR lpName, LPMSGQUEUEOPTIONS lpOptions)
{
	if (g_dll_power == NULL)
		g_dll_power = LoadLibrary(TEXT("coredll.dll"));
	if (g_dll_power != NULL && g_func_createmsgqueue == NULL)
		g_func_createmsgqueue = (FUNC_CREATEMSGQUEUE)GetProcAddress(g_dll_power, TEXT("CreateMsgQueue"));
	if (g_func_createmsgqueue)
		return(g_func_createmsgqueue(lpName, lpOptions));
	return(NULL);
}

//Close message queue
BOOL Imp_CloseMsgQueue(HANDLE hMsgQ)
{
	if (g_dll_power == NULL)
		g_dll_power = LoadLibrary(TEXT("coredll.dll"));
	if (g_dll_power != NULL && g_func_closemsgqueue == NULL)
		g_func_closemsgqueue = (FUNC_CLOSEMSGQUEUE)GetProcAddress(g_dll_power, TEXT("CloseMsgQueue"));
	if (g_func_closemsgqueue)
		return(g_func_closemsgqueue(hMsgQ));
	return(FALSE);
}

//Read message queue
BOOL Imp_ReadMsgQueue(HANDLE hMsgQ, LPVOID lpBuffer, DWORD cbBufferSize, LPDWORD lpNumberOfBytesRead, DWORD dwTimeout, DWORD *pdwFlags)
{
	if (g_dll_power == NULL)
		g_dll_power = LoadLibrary(TEXT("coredll.dll"));
	if (g_dll_power != NULL && g_func_readmsgqueue == NULL)
		g_func_readmsgqueue = (FUNC_READMSGQUEUE)GetProcAddress(g_dll_power, TEXT("ReadMsgQueue"));
	if (g_func_readmsgqueue)
		return(g_func_readmsgqueue(hMsgQ, lpBuffer, cbBufferSize, lpNumberOfBytesRead, dwTimeout, pdwFlags));
	return(FALSE);
}

//Import function definitions
typedef HANDLE (*FUNC_REQUESTPOWERNOTIFICATIONS)(HANDLE hMsgQ, DWORD Flags);
typedef DWORD (*FUNC_STOPPOWERNOTIFICATIONS)(HANDLE h);

//Imported power notifications functions
FUNC_REQUESTPOWERNOTIFICATIONS g_func_requestpowernotifications = NULL;
FUNC_STOPPOWERNOTIFICATIONS g_func_stoppowernotifications = NULL;

//Request power notifications
HANDLE Imp_RequestPowerNotifications(HANDLE hMsgQ, DWORD Flags)
{
	if (g_dll_power == NULL)
		g_dll_power = LoadLibrary(TEXT("coredll.dll"));
	if (g_dll_power != NULL && g_func_requestpowernotifications == NULL)
		g_func_requestpowernotifications = (FUNC_REQUESTPOWERNOTIFICATIONS)GetProcAddress(g_dll_power, TEXT("RequestPowerNotifications"));
	if (g_func_requestpowernotifications)
		return(g_func_requestpowernotifications(hMsgQ, Flags));
	return(NULL);
}

//Stop power notifications
DWORD Imp_StopPowerNotifications(HANDLE h)
{
	if (g_dll_power == NULL)
		g_dll_power = LoadLibrary(TEXT("coredll.dll"));
	if (g_dll_power != NULL && g_func_stoppowernotifications == NULL)
		g_func_stoppowernotifications = (FUNC_STOPPOWERNOTIFICATIONS)GetProcAddress(g_dll_power, TEXT("StopPowerNotifications"));
	if (g_func_stoppowernotifications)
		return(g_func_stoppowernotifications(h));
	return(FALSE);
}

#define MSGQUEUE_MAXITEMSIZE   256

#ifndef POWER_STATE_ON
	#define POWER_STATE(f)           ((f) &  0xFFFF0000)        // power state mask
	#define POWER_STATE_ON           (DWORD)(0x00010000)        // on state
	#define POWER_STATE_OFF          (DWORD)(0x00020000)        // no power, full off
	#define POWER_STATE_CRITICAL     (DWORD)(0x00040000)        // critical off
	#define POWER_STATE_BOOT         (DWORD)(0x00080000)        // boot state
	#define POWER_STATE_IDLE         (DWORD)(0x00100000)        // idle state
	#define POWER_STATE_SUSPEND      (DWORD)(0x00200000)        // suspend state
	#define POWER_STATE_UNATTENDED   (DWORD)(0x00400000)        // Unattended state.
	#define POWER_STATE_RESET        (DWORD)(0x00800000)        // reset state
	#define POWER_STATE_USERIDLE     (DWORD)(0x01000000)        // user idle state
	#define POWER_STATE_PASSWORD     (DWORD)(0x10000000)        // This state is password protected.
#endif

#ifndef POWER_STATE_UNATTENDED
	#define POWER_STATE_UNATTENDED   (DWORD)(0x00400000)        // Unattended state.
#endif

#ifndef MSGQUEUE_NOPRECOMMIT
	#define MSGQUEUE_NOPRECOMMIT            0x00000001
#endif

#ifndef PBT_POWERINFOCHANGE
	#define PBT_TRANSITION          0x00000001  // broadcast specifying system power state transition
	#define PBT_POWERINFOCHANGE     0x00000008  // some system power status field has changed
	#if (DEVICE_WINCE < 400)
		typedef struct _POWER_BROADCAST {
			DWORD       Message;    // one of PBT_Xxx
			DWORD       Flags;      // one of POWER_STATE_Xxx
			DWORD       Length;     // byte count of data starting at SystemPowerStateName
			WCHAR       SystemPowerState[1];    // variable length field, must be smaller than MAX_PATH + 1
		} POWER_BROADCAST, *PPOWER_BROADCAST;
	#endif
#endif

//Power helper class
class PowerNotificationManager
{
	private:
		HANDLE hNotif;
		HANDLE hReadQueue;
		HANDLE hWriteQueue;
		DWORD dwPowerState;
	public:
		PowerNotificationManager();
		~PowerNotificationManager();
		BOOL Init(void);
		DWORD GetPowerState(void);
		void WaitForTransition(DWORD dwTime){ if (hReadQueue) WaitForSingleObject(hReadQueue, dwTime); }
};

//PowerNotificationManager: constructor
PowerNotificationManager::PowerNotificationManager(void)
{
	hNotif = hReadQueue = hWriteQueue = NULL;
	dwPowerState = POWER_STATE_ON;
}

//PowerNotificationManager: destructor
PowerNotificationManager::~PowerNotificationManager(void)
{
	if (hNotif)
	{
		Imp_StopPowerNotifications(hNotif);
		hNotif = NULL;
	}
	if (hReadQueue)
	{
		Imp_CloseMsgQueue(hReadQueue);
		hReadQueue = NULL;
	}
	if (hWriteQueue)
	{
		Imp_CloseMsgQueue(hWriteQueue);
		hWriteQueue = NULL;
	}
}

//Initialize
BOOL PowerNotificationManager::Init()
{
	MSGQUEUEOPTIONS mqo = { sizeof(mqo), MSGQUEUE_NOPRECOMMIT, 100, MSGQUEUE_MAXITEMSIZE, FALSE };
	hWriteQueue = Imp_CreateMsgQueue(TEXT("EDGELIBPowerMsgQueue"), &mqo);
	if (hWriteQueue)
	{
		mqo.bReadAccess = TRUE;
		hReadQueue = Imp_CreateMsgQueue(TEXT("EDGELIBPowerMsgQueue"), &mqo);
		if ( hReadQueue )
		{
			hNotif = Imp_RequestPowerNotifications(hWriteQueue, PBT_TRANSITION);
			if (hNotif)
				return(TRUE);
			Imp_CloseMsgQueue(hReadQueue);
			hReadQueue = NULL;
		}
		Imp_CloseMsgQueue(hWriteQueue);
		hWriteQueue = NULL;
	}
	return(FALSE);
}

//Get power state
DWORD PowerNotificationManager::GetPowerState()
{
	if (hReadQueue)
	{
		DWORD dwHasObject = WaitForSingleObject(hReadQueue, 0);
		if (dwHasObject == WAIT_OBJECT_0)
		{
			DWORD dwRead, dwFlags;
			LPBYTE lpBuf[MSGQUEUE_MAXITEMSIZE];
			while (Imp_ReadMsgQueue(hReadQueue, lpBuf, MSGQUEUE_MAXITEMSIZE, &dwRead, 0, &dwFlags))
			{
				POWER_BROADCAST *pPB = (POWER_BROADCAST *)lpBuf;
				if (pPB->Message == PBT_TRANSITION)
					dwPowerState = POWER_STATE(pPB->Flags);
			}
		}
	}
	return(dwPowerState);
}
