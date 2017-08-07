// edgedevice.h
// Handles device definitions and includes
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#if !defined(EDGEDEVICE_INCLUDED)
#define EDGEDEVICE_INCLUDED

#if defined(WIN32_PLATFORM_PSPC)
	#define DEVICE_POCKETPC
	#define DEVICE_WINMOBILE    200
	#define DEVICE_WINCE
	#define DEVICE_WIN32
#elif defined(WIN32_PLATFORM_WFSP)
	#define DEVICE_SMARTPHONE
	#define DEVICE_WINMOBILE    200
	#define DEVICE_WINCE
	#define DEVICE_WIN32
#elif defined(_WIN32_WCE)
	#if defined(__GIZMONDO__)
		#define DEVICE_GIZMONDO
	#endif
	#define DEVICE_WINCE
	#define DEVICE_WIN32
#elif defined(SERIES60)
	#define DEVICE_S60
	#define DEVICE_SERIES60
	#define DEVICE_SYMBIAN      610
#elif defined(SERIES80)
	#define DEVICE_S80
	#define DEVICE_SERIES80
	#define DEVICE_SYMBIAN      600
#elif defined(SERIES90)
	#define DEVICE_NOKIA7710
	#define DEVICE_S90
	#define DEVICE_SERIES90
	#define DEVICE_SYMBIAN      700
#elif defined(SYMBIANUIQ)
	#define DEVICE_UIQ
	#define DEVICE_SYMBIAN      700
#elif defined(__linux__) || defined(__LINUX__) || defined(__GP2X__) || defined(__ALP__)
	#define DEVICE_LINUX
	#define DEVICE_POSIX
	#if defined(__GP2X__)
		#define DEVICE_GP2X
		#define DEVICE_EMBEDDEDLINUX
	#elif defined(__ALP__)
		#define DEVICE_ALP
		#define DEVICE_EMBEDDEDLINUX
		#define DEVICE_X11
	#elif defined(MAEMO_GTK) || defined(__MAEMO__)
		#if defined(__MAEMO_SIM__)
			#define DEVICE_MAEMOSIM
		#endif
		#define DEVICE_MAEMO
		#define DEVICE_EMBEDDEDLINUX
		#define DEVICE_X11
	#elif defined(ANDROID)
		#define DEVICE_ANDROID
		#define DEVICE_EMBEDDEDLINUX
	#endif
	#if !defined(DEVICE_EMBEDDEDLINUX)
		#define DEVICE_DESKTOP		
		#define DEVICE_X11
	#endif
#elif defined(__APPLE__)
	#define	DEVICE_MAC
	#define	DEVICE_APPLE
	#define DEVICE_POSIX
#elif defined(__agp__)
	#define DEVICE_ANTIX
	#define DEVICE_AGP
	#define DEVICE_OPENKODE
#elif defined(WIN32)
	#define DEVICE_DESKTOP
	#define DEVICE_WINDOWS
	#define DEVICE_WIN32
#else
	#if _MSC_VER >= 1300                       //Fix for Visual C++ express 2005 to build Win32 projects easily
		#define WIN32
		#define DEVICE_DESKTOP
		#define DEVICE_WINDOWS
		#define DEVICE_WIN32
	#else
		#error Unsupported platform
	#endif
#endif

#if defined(DEVICE_WINMOBILE)
	#if (_WIN32_WCE >= 400 && _WIN32_WCE < 1200)
		#undef DEVICE_WINMOBILE
		#define DEVICE_WM2003
		#define DEVICE_WINMOBILE_NG
		#define DEVICE_WINMOBILE    300
	#elif (_WIN32_WCE >= 1200)
		#undef DEVICE_WINMOBILE
		#define DEVICE_WINMOBILE_NG
		#define DEVICE_DIRECTX
		#if (_WIN32_WCE >= 0x502)
			#define DEVICE_WINMOBILE    600
		#else
			#define DEVICE_WINMOBILE    500
		#endif
	#endif
#elif defined(DEVICE_WINCE)
	#define DEVICE_DIRECTX
#endif

#if defined(DEVICE_WINCE)
	#undef DEVICE_WINCE
	#if (_WIN32_WCE < 400)
		#define DEVICE_WINCE 300
	#elif (_WIN32_WCE < 1200)
		#define DEVICE_WINCE 400
	#else
		#define DEVICE_WINCE 500
	#endif
#endif

#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
	#define DEVICE_DIRECTX
#endif

///////////////////////////////////////////////////////////////////
// Includes                                                      //
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_SYMBIAN)
	#if defined(__WINS__) || defined(__WINSCW__)
		#define DEVICE_SYMBIANEMU
	#endif
	#ifdef DEVICE_SERIES60
		#include <avkon.hrh>
		#include <avkon.rsg>
	#endif
	#ifdef DEVICE_SERIES60
		#ifdef KAknSignalOffLineMode //Fix to check for Symbian 7.0+ (avkon.hrh)
			#undef DEVICE_SYMBIAN
			#define DEVICE_SYMBIAN 800
		#endif
	#endif
	#if defined(DEVICE_UIQ) || defined(DEVICE_SERIES80)
		#include <eikon.hrh>
	#endif
	#ifdef DEVICE_SERIES80
		#ifdef EEikComboBoxInfoPrintNothingInHistoryList //Fix to check for Symbian 7.0+ (eikon.hrh)
			#undef DEVICE_SYMBIAN
			#define DEVICE_SYMBIAN 700
		#endif
	#endif
	#ifdef __EXE__
		#define DEVICE_SYMBIANEXE
	#endif
	#if !defined(DEVICE_SYMBIANEXE)
		#if defined(DEVICE_SERIES60)
			#ifdef EAknListBoxStylusMarkableList //Temporary fix to check for Symbian 9.1+ (avkon.hrh)
				#define DEVICE_SYMBIANEXE
			#endif
		#elif defined(DEVICE_UIQ)
			#ifdef EQikDlgItemCtlIsEditInPlace   //Temporary fix to check for Symbian 9.1+ (eikon.hrh, line 570)
				#define DEVICE_SYMBIANEXE
			#endif
		#endif
	#endif
	#ifdef DEVICE_SYMBIANEXE
		#undef DEVICE_SYMBIAN
		#define DEVICE_SYMBIAN 910
	#endif
	#if (defined(DEVICE_SERIES90) || (DEVICE_SYMBIAN >= 910))
		#define SYMBIAN_ALTERNATIVE_IMEI
	#endif
	#if defined(SYMBIAN_ALTERNATIVE_IMEI) || (DEVICE_SYMBIAN >= 800)
		#define SYMBIAN_ETEL3RDPARTY
	#endif
#elif defined(DEVICE_WIN32)
	#if defined(DEVICE_WINCE)
		#pragma comment(lib, "winsock.lib")
		#if defined(DEVICE_WINMOBILE)
			#pragma comment(lib, "aygshell.lib")
			#pragma comment(lib, "cellcore.lib")
			#if (DEVICE_WINMOBILE >= 300)
				#pragma comment(lib, "ole32.lib")
				#pragma comment(lib, "uuid.lib")
			#endif
		#endif
		#if (DEVICE_WINCE >= 400)
			#pragma comment(lib, "ws2.lib")
		#endif
		#if (_MSC_VER >= 1300) && (_WIN32_WCE < 1200) //Visual Studio 7.0+ and windows mobile 4.0-
			#pragma comment(lib, "ccrtrtti.lib")
			#pragma comment(lib, "secchk.lib")
		#endif
	#else
		#pragma comment(lib, "user32.lib")
		#pragma comment(lib, "wsock32.lib")
		#pragma comment(lib, "Ws2_32.lib")
	#endif
#elif defined(DEVICE_MAC)
	#include <CoreFoundation/CoreFoundation.h>
	#include <stdlib.h>
	#include <unistd.h>
    #if defined(__MAC_OS_X_VERSION_MAX_ALLOWED)
		#define DEVICE_DESKTOP
	#elif defined(TARGET_OS_IPHONE)
		#if defined(__IPHONE_3_0)
			#define DEVICE_IPHONE	300
		#elif defined(__IPHONE_2_2)
			#define DEVICE_IPHONE	220
		#elif defined(__IPHONE_2_1)
			#define DEVICE_IPHONE	210
		#elif defined(__IPHONE_2_0)
			#define DEVICE_IPHONE	200
		#else
			#define DEVICE_IPHONE	100
		#endif
	#else
		#error undefined platform
	#endif
	#if (TARGET_IPHONE_SIMULATOR)
		#define DEVICE_IPHONEEMU
	#endif
#elif defined(DEVICE_ANTIX)
	#include <KD/kd.h>
	#define NULL KD_NULL
#endif

///////////////////////////////////////////////////////////////////
// Messages for iPhone (OnNativeEvent)                           //
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_IPHONE)
	#define IM_CREATE				0
	#define IM_LAUNCHOPTIONS		1
	#define IM_NOTIFICATION			2
	#define IM_STARTNOTIFICATION	3
	#define IM_REGNOTIFTOKEN		4
	#define IM_REGNOTIFERROR		5
	#define IM_OPENURL				6
#endif

#endif
