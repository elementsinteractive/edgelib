// imp_dspchange.h
// Import header: provides forward compatibility for native resolution and orientation changes
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#if (DEVICE_WINMOBILE < 300)

//Orientations
#define DMDO_0      0
#define DMDO_90     1
#define DMDO_180    2
#define DMDO_270    4

//EnumDisplaySettings parameters
#define ENUM_CURRENT_SETTINGS ((DWORD)-1)

//ChangeDisplaySettingsEx parameters
#define DM_DISPLAYORIENTATION  0x00800000L
#define CDS_TEST               0x00000002
#define DISP_CHANGE_SUCCESSFUL 0

//Devmode structure
typedef struct
{
	WCHAR  dmDeviceName[CCHDEVICENAME];
	WORD dmSpecVersion;
	WORD dmDriverVersion;
	WORD dmSize;
	WORD dmDriverExtra;
	DWORD dmFields;
	short dmOrientation;
	short dmPaperSize;
	short dmPaperLength;
	short dmPaperWidth;
	short dmScale;
	short dmCopies;
	short dmDefaultSource;
	short dmPrintQuality;
	short dmColor;
	short dmDuplex;
	short dmYResolution;
	short dmTTOption;
	short dmCollate;
	WCHAR  dmFormName[CCHFORMNAME];
	WORD   dmLogPixels;
	DWORD  dmBitsPerPel;
	DWORD  dmPelsWidth;
	DWORD  dmPelsHeight;
	DWORD  dmDisplayFlags;
	DWORD  dmDisplayFrequency;
	DWORD  dmDisplayOrientation;
// #if (WINVER >= 0x0400)
//     DWORD  dmICMMethod;
//     DWORD  dmICMIntent;
//     DWORD  dmMediaType;
//     DWORD  dmDitherType;
//     DWORD  dmReserved1;
//     DWORD  dmReserved2;
// #endif /* WINVER */
}DEVMODE_IMP_X;

//Import function definitions
typedef BOOL (*FUNC_ENUMDISPLAYSETTINGS)(IN LPCTSTR lpszDeviceName, IN DWORD iModeNum, OUT DEVMODE *lpDevMode);
typedef LONG (*FUNC_CHANGEDISPLAYSETTINGSEX)(IN LPCTSTR lpszDeviceName, IN DEVMODE *lpDevMode, IN HWND hwnd, IN DWORD dwflags, LPVOID lParam);

//Global dll handle and import functions
HMODULE g_dll_dispchange = NULL;
FUNC_ENUMDISPLAYSETTINGS g_func_enumdisplaysettings = NULL;
FUNC_CHANGEDISPLAYSETTINGSEX g_func_changedisplaysettingsex = NULL;

//Enumerate display settings
BOOL WINAPI EnumDisplaySettings(IN LPCTSTR lpszDeviceName, IN DWORD iModeNum, OUT DEVMODE_IMP_X *lpDevMode)
{
	if (g_dll_dispchange == NULL)
		g_dll_dispchange = LoadLibrary(TEXT("coredll.dll"));
	if (g_dll_dispchange != NULL && g_func_enumdisplaysettings == NULL)
		g_func_enumdisplaysettings = (FUNC_ENUMDISPLAYSETTINGS)GetProcAddress(g_dll_dispchange, TEXT("EnumDisplaySettings"));
	if (g_func_enumdisplaysettings != NULL)
		return(g_func_enumdisplaysettings(lpszDeviceName, iModeNum, (DEVMODE *)lpDevMode));
	return(FALSE);
}

//Change/query display settings
LONG WINAPI ChangeDisplaySettingsEx(IN LPCTSTR lpszDeviceName, IN DEVMODE_IMP_X *lpDevMode, IN HWND hwnd, IN DWORD dwflags, LPVOID lParam)
{
	if (g_dll_dispchange == NULL)
		g_dll_dispchange = LoadLibrary(TEXT("coredll.dll"));
	if (g_dll_dispchange != NULL && g_func_changedisplaysettingsex == NULL)
		g_func_changedisplaysettingsex = (FUNC_CHANGEDISPLAYSETTINGSEX)GetProcAddress(g_dll_dispchange, TEXT("ChangeDisplaySettingsEx"));
	if (g_func_changedisplaysettingsex != NULL)
		return(g_func_changedisplaysettingsex(lpszDeviceName, (DEVMODE *)lpDevMode, hwnd, dwflags, lParam));
	return(1);
}

#else

#define DEVMODE_IMP_X DEVMODE

#endif