// edgedef.h
// Contains EDGELIB definitions
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Include important device SDK headers
#if defined(DEVICE_WIN32)
	#define _WINSOCKAPI_
	#include <windows.h>
	#undef _WINSOCKAPI_
#elif defined(DEVICE_SYMBIAN)
	#include <e32std.h>
#elif defined(DEVICE_LINUX)
	#include <stdlib.h>
	#include <unistd.h>
#endif

///////////////////////////////////////////////////////////////////
// Definition fixes
///////////////////////////////////////////////////////////////////

#if defined(StrCpy)
	#undef StrCpy
#endif
#if defined(StrCat)
	#undef StrCat
#endif
#if defined(StrLen)
	#undef StrLen
#endif
#if defined(StrTrim)
	#undef StrTrim
#endif

#if defined(PlaySound)
	#undef PlaySound
#endif

#if defined(GetNextSibling)
	#undef GetNextSibling
#endif

#if defined(DEVICE_SYMBIAN) && !defined(NULL)
	#define NULL 0
#endif

///////////////////////////////////////////////////////////////////
// Internal macro's
///////////////////////////////////////////////////////////////////

#define FULL_FREE(p)   { ClassEMemory::DeAlloc(p); (p) = NULL; }
#define FULL_FREE_L(p) { ecd->mem->DeAllocList(p); (p) = NULL; }

#define USTR_CHECK_BEGIN(s) \
	WCHAR *h_ustr_##s = NULL; \
	WCHAR l_ustr_##s[256]; \
	WCHAR *ustr_##s = l_ustr_##s; \
	if (s != NULL) \
	{ \
		if (ClassEStd::StrLen(s) >= 256) \
		{ \
			h_ustr_##s = (WCHAR *)ClassEMemory::Alloc((ClassEStd::StrLen(s) + 1) * sizeof(WCHAR)); \
			if (h_ustr_##s == NULL) \
				ClassEStd::StrCpy(l_ustr_##s, s, 256); \
			else \
				ClassEStd::StrCpy(h_ustr_##s, s); \
		} \
		else \
			ClassEStd::StrCpy(l_ustr_##s, s); \
		if (h_ustr_##s != NULL) \
			ustr_##s = h_ustr_##s; \
	} \
	else \
		ustr_##s = NULL

#define USTR_CHECK_END(s) FULL_FREE(h_ustr_##s)

#define CSTR_CHECK_BEGIN(s) \
	char *h_cstr_##s = NULL; \
	char l_cstr_##s[256]; \
	char *cstr_##s = l_cstr_##s; \
	if (s != NULL) \
	{ \
		if (ClassEStd::StrLen(s) >= 256) \
		{ \
			h_cstr_##s = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(s) + 1); \
			if (h_cstr_##s == NULL) \
				ClassEStd::StrCpy(l_cstr_##s, s, 256); \
			else \
				ClassEStd::StrCpy(h_cstr_##s, s); \
		} \
		else \
			ClassEStd::StrCpy(l_cstr_##s, s); \
		if (h_cstr_##s != NULL) \
			cstr_##s = h_cstr_##s; \
	} \
	else \
		cstr_##s = NULL

#define CSTR_CHECK_END(s) FULL_FREE(h_cstr_##s)

///////////////////////////////////////////////////////////////////
// Global macro's
///////////////////////////////////////////////////////////////////

//RGB macro for other platforms
#if !defined(RGB)
	#define RGB(r, g, b)     (((b) << 16) | ((g) << 8) | (r))
#endif

//Sleep function as macro for other platforms
#if defined(DEVICE_SYMBIAN)
	//#define abs(x) ((x) >= 0 ? (x) : -(x))
	#define Sleep(ms) User::After((ms) * 1000)
#elif defined(DEVICE_POSIX)
	#define Sleep(ms) usleep((ms) * 1000)
#elif defined(DEVICE_OPENKODE)
	#define Sleep(ms) { }
#endif

//Text macro for other platforms
#if !defined(TEXT)
	#if defined(UNICODE)
		#define TEXT(t)      Lt
	#else
		#define TEXT(t)      t
	#endif
#endif

//Helper macro's
#define REDUCE_RGB(col, rm, gm, bm) (((((col) & 0xFF) >> (8 - (rm))) & 0xFF) << (8 - (rm))) | (((((col) & 0xFF00) >> (8 - (gm))) & 0xFF00) << (8 - (gm))) | (((((col) & 0xFF0000) >> (8 - (bm))) & 0xFF0000) << (8 - (bm)))
#define E_RGBX(r, g, b)             (((b) << 16) | ((g) << 8) | (r))
#define E_RGBA(r, g, b, a)          (((a) << 24) | ((b) << 16) | ((g) << 8) | (r))
#define E_GETRGB(c)                 ((c) & 0xFFFFFF)
#define E_GETR(c)                   ((c) & 0xFF)
#define E_GETG(c)                   (((c) >> 8) & 0xFF)
#define E_GETB(c)                   (((c) >> 16) & 0xFF)
#define E_GETA(c)                   (((c) >> 24) & 0xFF)
#define E_MONEYGETMINOR(m)          (m % 100)
#define E_MONEYGETMAJOR(m)          (m / 100)
#define E_MONEYCOMBINE(major, minor) (major * 100 + minor)

///////////////////////////////////////////////////////////////////
// Definitions
///////////////////////////////////////////////////////////////////

// pointer sizes
#if !defined(EINTPTR)
	#if defined(__amd64__) || defined(__x86_64__)
		#define EINTPTR long
        #define EINT64 long
		#define EINT32 int
		#define EINT16 short
	#else
		#define EINTPTR int
		#define EINT64 long long
		#define EINT32 int
		#define EINT16 short
	#endif
#endif

//Version info
#define E_VERSION_NR         4020
#define E_VERSION         "4.02"
#define E_ENGINENAME    "EDGELIB"

//Generic data types
#define ERESULT long
#define ENATIVETYPE EINTPTR
#define ECOREOBJ void *

//Additional datatypes
#define ETYPE_TIMESTAMP      unsigned long
#define ETYPE_MONEY          unsigned long

//Limits
#define MAX_ECONNECT        16
#define MAX_ESTR           256
#define MAX_EFILESTR       512
#define MAX_CMDLINE       4096

///////////////////////////////////////////////////////////////////
// Other definitions
///////////////////////////////////////////////////////////////////

//Results
#define E_OK                 0
#define E_ERROR              1
#define E_NOMEMORY           2
#define E_UNSUPPORTED        3
#define E_INVALIDPARAM       4
#define E_NOENTRY            5
#define E_TOOBIG             6
#define E_NOTEXISTS          7
#define E_NODATA             8
#define E_ALREADYFOUND       9
#define	E_NOACTION          10
#define E_TYPEMISMATCH      11
#define E_LOCKFAILED        12
#define E_NOTCONNECTED      13
#define E_NOFEATURE         14
#define E_ABORTED           15
#define E_PERMISSION        16
#define E_CUSTOM         65535

//Display rotations
#define DOR_NONE             0
#define DOR_90DEG            1
#define DOR_180DEG           2
#define DOR_270DEG           3
#define DOR_PORTRAIT         4
#define DOR_LANDSCAPE        5
#define DOR_AUTO             6
#define DOR_STARTUP          7

#define ESO_NONE             1
#define ESO_90DEG            2
#define ESO_180DEG           4
#define ESO_270DEG           8
#define ESO_ALL              15
 
//Positioning
#define EPOS_TOP              0
#define EPOS_BOTTOM           1
#define EPOS_LEFT             2
#define EPOS_RIGHT            3

//Edge main engine flags
#define EFL_NONE                0
#define EFL_MANUALFLIP          1
#define EFL_GETSHADOWKEYS       2
#define EFL_NOTEARING           4
#define EFL_NOINTERNALFONT      8
#define EFL_SCREENCAPTURE      16
#define EFL_TILTEMULATOR       32
#define EFL_RUNMINIMIZED       64
#define EFL_BORDERLESSWND     128
#define EFL_MORECLICKMOVE     256
#define EFL_DIALOGTHREAD 0x400000
#define EFL_IGNOREREDRAW 0x800000

//Edge internal flags
#define EIF_NONE             0
#define EIF_DIVIDESTYLUS     1
#define EIF_STARTROTATED     2
#define EIF_NOWNDRESIZE      4

//App (window) statuses
#define EAPP_IDLE            0
#define EAPP_NOFOCUS         1
#define EAPP_MINIMIZED       2
#define EAPP_NOTRUNNING      3
#define EAPP_DESTROY         4
#define EAPP_INVALID       255

//Network messages
#define ENET_SERVERFOUND     0
#define ENET_RUNNINGSERVER   1
#define ENET_ADDCLIENT       2
#define ENET_CLOSEDSOCKET    3
#define ENET_RECEIVEDATA     4
#define ENET_PINGRECEIVED    5
#define ENET_DEVICEFOUND    16
#define ENET_SERVICEFOUND   17
#define ENET_ENDSEARCH      18
#define ENET_CPGBALANCE     64
#define ENET_CPGGROSSPRICE  65
#define ENET_CPGVATPERC     66
#define ENET_CPGTRANSACTION 67
#define ENET_CPGVALIDATED   68
#define ENET_CPGERROR       69
#define ENET_USER          128
#define ENET_DEBUG         255

//Stack pop return messages
#define EPOP_NODATA          0
#define EPOP_RECEIVEDATA     1
#define EPOP_REQUESTPING     2
#define EPOP_RESPONDPING     3

//Other definitions
#define EDT_TIMESTAMPNOW     0xFFFFFFFF

///////////////////////////////////////////////////////////////////
// Redefinition of special data types
///////////////////////////////////////////////////////////////////

//Wide character datatype
#ifndef WCHAR
	#define WCHAR unsigned short
#endif

//TCHAR datatype
#ifndef TCHAR
	#ifdef UNICODE
		#define TCHAR unsigned short
	#else
		#define TCHAR char
	#endif
#endif

//RGB color datatype
#ifndef COLORREF
	#define COLORREF unsigned long
#endif

///////////////////////////////////////////////////////////////////
// Predefined classes                                            //
///////////////////////////////////////////////////////////////////

class ClassEdge;
class ClassEStd;
class ClassEDisplay;
class ClassESound;
class ClassEInput;
class ClassEVTimer;
class ClassERTimer;
class ClassEFile;
class ClassEIni;
class ClassEMemory;
class ClassEMath;
class ClassERegister;
class ClassEConnect;
class ClassEDevice;
class ClassEConsole;
class ClassECamera;

///////////////////////////////////////////////////////////////////
// Enums
///////////////////////////////////////////////////////////////////

//UI event
typedef enum
{
	UI_CLICK = 0
}UI_EVENT;

///////////////////////////////////////////////////////////////////
// Structures
///////////////////////////////////////////////////////////////////

#ifndef DEVICE_WIN32

//Rectangle
typedef struct tagRECT
{
	long left;
	long top;
	long right;
	long bottom;
}RECT, *LPRECT;

//2D point
typedef struct tagPOINT
{
	long x;
	long y;
}POINT, *LPPOINT;

#endif

//A date
typedef struct
{
	unsigned long year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
}E_DATETIME;

//Tiltcontrol emulation settings
typedef struct
{
	bool enablestylus;
	bool enabledpad;
	bool last_enablestylus;
	bool last_enabledpad;
	long last_x;
	long last_y;
	long last_deadpointx;
	long last_deadpointy;
	long dpaddeadpointx;
	long dpaddeadpointy;
	long stylussensitivity;
	long centerangle_x;
	long centerangle_y;
}EDGETILTEMUSETTINGS;

//Engine environment
typedef struct
{
	ENATIVETYPE instance;
	WCHAR currentpath[MAX_EFILESTR];
	WCHAR readonlypath[MAX_EFILESTR];
	char commandline[MAX_CMDLINE];
	long returnvalue;
	long global1;
	long global2;
}EDGEDATA;

//Default button assignments
typedef struct
{
	unsigned long ButtonA;
	unsigned long ButtonB;
	unsigned long ButtonC;
	unsigned long ButtonStart;
	unsigned long DPadUp;
	unsigned long DPadDown;
	unsigned long DPadLeft;
	unsigned long DPadRight;
	unsigned long JogLeft;
	unsigned long JogRight;
	unsigned long Soft1;
	unsigned long Soft2;
	unsigned long Soft3;
	unsigned long Soft4;
	unsigned long Home;
	unsigned long KeyEnter;
	unsigned long KeyShift;
	unsigned long KeyShift2;
	unsigned long KeyControl;
	unsigned long KeyControl2;
	unsigned long KeyEscape;
	unsigned long KeySpace;
	unsigned long KeyBackspace;
	unsigned long KeyTab;
	unsigned long KeyPageup;
	unsigned long KeyPagedown;
	unsigned long KeyInsert;
	unsigned long KeyDelete;
	unsigned long KeyHome;
	unsigned long KeyEnd;
	unsigned long KeyMenu;
}EBUTTONLIST;

//Framework configuration
typedef struct
{
	unsigned long ffwdkey;
	unsigned long slomokey;
	unsigned long maxfps;
	unsigned long flags;
	unsigned char numconnections;
	bool smartfps;
	bool focusminimize;
	bool jogtiltarrow;
	bool focussuspend;
	bool backlighton;
	bool netlogging;
	bool lockkeys;
	bool keysounds;
}EFRAMEWORKCONFIG;

//Display configuration
typedef struct
{
	WCHAR caption[MAX_ESTR];
	unsigned long icon;
	unsigned long menu;
	unsigned long dialogbox;
	unsigned long width;
	unsigned long height;
	unsigned long dlgleft;
	unsigned long dlgtop;
	unsigned char videotype;
	unsigned char emulvideotype;
	unsigned char orientation;
	unsigned char openmethod;
	unsigned char surfacedefault;
	unsigned char supportedorientations;
	bool fullscreen;
	bool fullscreenmouse;
	bool dspnative;
	bool enginestatus;
	bool engineconsole;
	bool surfprerotate;
	bool stencil;
	unsigned char emulatelowres;
}EDISPLAYCONFIG;

typedef struct
{
    long x;
    long y;
    long format;
    long expectedfps;
}E_CAMERAMODE;

typedef struct
{
    const unsigned char * data;
    long width;
    long height;
    long pitch;
    long format;
    long orientation;
}E_CAMERAFRAME;

//Engine common data
typedef struct
{
	ClassEStd      *std;
	ClassEDisplay  *dsp;
	ClassESound    *snd;
	ClassEInput    *input;
	ClassEVTimer   *tmr;
	ClassEFile     *file;
	ClassEIni      *ini;
	ClassEMemory   *mem;
	ClassEMath     *math;
	ClassERegister *reg;
	ClassEDevice   *device;
	ClassEConsole  *console;
	ClassEConnect  *net[16];
	EFRAMEWORKCONFIG *maincfg;
	EDISPLAYCONFIG   *dispcfg;
	EDGEDATA         *dat;
}ECD;

///////////////////////////////////////////////////////////////////
// Windows functions for symbian & rect functions                //
///////////////////////////////////////////////////////////////////

#if !defined(DEVICE_WIN32)

void SetRect(RECT *lprc, int xLeft, int yTop, int xRight, int yBottom);

#endif

bool RectIntersect(RECT *rc1, RECT *rc2, RECT *result = NULL);
bool InRect(RECT *rc, POINT *pnt);

void _ti_edgelib_log(const char *text, bool newline = true);
void _ti_edgelib_log(const WCHAR *text, bool newline = true);
void _ti_edgelib_log(long value, bool newline = true);
