// cevent.cpp
// Edgecore: provides event framework
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Device include
#include "edgedevice.h"

//Device SDK includes
#if defined(DEVICE_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#if defined(DEVICE_WINCE)
		#if (DEVICE_WINCE >= 400)
			#include <pm.h>
		#endif
		#include "extapi/imp_powerapi.h"
	#endif
	#if defined(DEVICE_WINMOBILE)
		#include <aygshell.h>
		#if defined(DEVICE_SMARTPHONE)
			#include <cfgmgrapi.h>
		#endif
	#endif
#elif defined(DEVICE_SYMBIAN)
	#include <eikenv.h>
#elif defined(DEVICE_LINUX)
	#include <sys/mman.h>
	#include <fcntl.h>
	#include <stdlib.h>
	#include <unistd.h>
	#if defined(DEVICE_GP2X)
		#include <string.h>
	#elif defined (DEVICE_ANDROID)
		#include <string.h>
		#include <sys/stat.h>
		#include <jni.h>
		int Call_EdgelibApp_edgeWindowCreate(JNIEnv * env, jobject jthis, int options);
		void Call_EdgelibApp_setBacklight(JNIEnv * env, jobject jthis, int status);
		void Call_EdgelibApp_suspendloop(JNIEnv * env, jobject jthis, bool suspend);
		void Call_EdgelibApp_quitloop(JNIEnv * env, jobject jthis);
		int Call_EdgelibView_getWidth();
		int Call_EdgelibView_getHeight();
	#else
		#if defined(DEVICE_ALP)
			#include <gtk/gtk.h>
			#include <glade/glade.h>
			#include <alp/alp.h>
		#elif defined(DEVICE_MAEMO) && !defined(DEVICE_MAEMOSIM)
			#include <dbus/dbus-glib.h>
		#endif
		#include <X11/Xlib.h>
		#include <X11/Xutil.h>
		#include <X11/XKBlib.h>
	#endif
#elif defined(DEVICE_MAC)
	#include <stdlib.h>
	#include <unistd.h>
#elif defined(DEVICE_OPENKODE)
	#include <EGL/egl.h>
	#include <EGL/eglext.h>
	#include <KD/ATX_bluetooth.h>
	#include <KD/ATX_keyboard.h>
#endif

//Core include
#include "edgecore.h"

//S60 device tilt API
#if (DEVICE_SYMBIAN >= 900) && defined(DEVICE_S60)
	#include "extapi\\imp_symbiantilt.h"
#endif

///////////////////////////////////////////////////////////////////
// Internal definitions and structures
///////////////////////////////////////////////////////////////////

//Helper for TCHAR datatype casting
#if defined(DEVICE_WIN32)
	#if defined(UNICODE)
		#define ECTCHAR ecUnistring
	#else
		#define ECTCHAR char *
	#endif
#endif

//Gapi key lock function
#if defined(DEVICE_WINMOBILE)
	typedef int (*FUNC_GXOPENINPUT)(void);
	typedef int (*FUNC_GXCLOSEINPUT)(void);
#endif

//Setting change callback
#if !defined(SETTINGCHANGE_RESET)
	#define SETTINGCHANGE_RESET 0x3002
#endif

//Application key definitions
#if !defined(VK_APP1)
	#define VK_APP1    193
	#define VK_APP2    194
	#define VK_APP3    195
	#define VK_APP4    196
	#define VK_APP5    197
	#define VK_APP6    198
#endif

//TSLIB click event structure
#if defined(DEVICE_GP2X)
	typedef struct
	{
		short pressure;
		short x;
		short y;
		short pad;
		struct timeval stamp;
	}TS_EVENT;
#endif

//An event object
#include "instance.h"

///////////////////////////////////////////////////////////////////
// Prototypes
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_WIN32)
	BOOL CALLBACK ecInstWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
	#if defined(DEVICE_DESKTOP)
		BOOL CALLBACK ecInstDlgProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam);
	#endif
#elif defined(DEVICE_SYMBIAN)
	TInt ecInstCallIdleTimer(TAny *aAny);
#endif

#if defined(DEVICE_SYMBIAN)
	#include "extapi\\symbianframe.h"
#endif

#if defined(DEVICE_MAC)
	int StartMacWindow(int argc, char *argv[], EC_INSTANCEOBJ *obj);
	void StartMacLoop(EC_INSTANCEOBJ *obj);
	void StopMacLoop(EC_INSTANCEOBJ *obj);
	void KillMacWindow(EC_INSTANCEOBJ *obj);
	void MacGetNativeResolution(ECOREOBJ winhandle, long &xres, long &yres);
#endif

///////////////////////////////////////////////////////////////////
// Internal functions
///////////////////////////////////////////////////////////////////

//Get/set key click
void eciHandleKeyClick(EC_INSTANCEOBJ *io, unsigned char newvalue = 0, bool setvalue = false, bool restoreold = false)
{
	#if defined(DEVICE_SMARTPHONE)
		if (restoreold)
		{
			newvalue = io->keyclick;
			setvalue = true;
		}
		if (setvalue)
		{
			if (newvalue > 9)
				return;
			ecUnichar xmltext[512];
			LPWSTR retval;
			ecStrCpy(xmltext, (ecUnistring)TEXT("<wap-provisioningdoc><characteristic type='Sounds'><characteristic type='ControlPanel\\Sounds\\KeyPress'><parm name='Mode' value='X'/></characteristic></characteristic></wap-provisioningdoc>"));
			xmltext[128] = (unsigned short)(newvalue + '0');
			DMProcessConfigXML((LPCWSTR)xmltext, CFGFLAG_PROCESS, &retval);
			delete retval;
		}
		else
		{
			char retstr[512];
			unsigned long ctr, len;
			io->keyclick = 1;
			ecUnichar xmltext[512];
			LPWSTR retval;
			ecStrCpy(xmltext, (ecUnistring)TEXT("<wap-provisioningdoc><characteristic type='Sounds'><characteristic type='ControlPanel\\Sounds\\KeyPress'><parm-query name='Mode'/></characteristic></characteristic></wap-provisioningdoc>"));
			DMProcessConfigXML((LPCWSTR)xmltext, CFGFLAG_PROCESS, &retval);
			ecStrCpy(retstr, (ecUnistring)retval, 512);
			delete retval;
			len = ecStrLen(retstr);
			for (ctr = 0; ctr < len - 8; ctr++)
				if (retstr[ctr] == 'v' && retstr[ctr + 1] == 'a' && retstr[ctr + 2] == 'l' && retstr[ctr + 3] == 'u' && retstr[ctr + 4] == 'e')
					io->keyclick = retstr[ctr + 7] - '0';
		}
	#endif
}

//Handle key locking
void eciHandleKeyLock(EC_INSTANCEOBJ *io, bool lockon)
{
	#if defined(DEVICE_WINMOBILE)
		if (lockon)
		{
			if (io->dll_gapi == NULL && io->gapiok)
			{
				io->dll_gapi = LoadLibrary(TEXT("gx.dll"));
				if (io->dll_gapi)
				{
					io->func_gxopeninput = (FUNC_GXOPENINPUT)GetProcAddress(io->dll_gapi, TEXT("?GXOpenInput@@YAHXZ"));
					io->func_gxcloseinput = (FUNC_GXCLOSEINPUT)GetProcAddress(io->dll_gapi, TEXT("?GXCloseInput@@YAHXZ"));
					if (io->func_gxopeninput == NULL || io->func_gxcloseinput == NULL)
					{
						io->gapiok = false;
						FreeLibrary(io->dll_gapi);
					}
				}
				else
					io->gapiok = false;
			}
			if (!io->lockedinput)
			{
				if (io->gapiok)
				{
					io->func_gxopeninput();
				}
				else
				{
					AllKeys(TRUE);
					#if defined(DEVICE_POCKETPC)
						SHSetAppKeyWndAssoc(VK_APP1, io->wnd); SHSetAppKeyWndAssoc(VK_APP2, io->wnd); SHSetAppKeyWndAssoc(VK_APP3, io->wnd); SHSetAppKeyWndAssoc(VK_APP4, io->wnd); SHSetAppKeyWndAssoc(VK_APP5, io->wnd); SHSetAppKeyWndAssoc(VK_APP6, io->wnd);
					#endif
					SHSetAppKeyWndAssoc(VK_F1, io->wnd); SHSetAppKeyWndAssoc(VK_F2, io->wnd); SHSetAppKeyWndAssoc(VK_F3, io->wnd); SHSetAppKeyWndAssoc(VK_F4, io->wnd);
				}
				io->lockedinput = true;
			}
		}
		else
		{
			if (io->lockedinput)
			{
				if (io->gapiok)
				{
					if (io->dll_gapi)
						io->func_gxcloseinput();
				}
				else
					AllKeys(FALSE);
				io->lockedinput = false;
			}
		}
	#endif
}

//Translate linux keycodes to match ascii
int eciTranslateKeycode(int orgkey)
{
	#if defined(DEVICE_LINUX)
		unsigned char key_translater[] = {
			  0,   1,   2,   3,   4,   5,   6,   7,  22,  27,  49,  50,  51,  52,  53,  54,
			 55,  56,  57,  48, 189, 187,   8,   9,  81,  87,  69,  82,  84,  89,  85,  73,
			 79,  80, 219, 221,  13,  17,  65,  83,  68,  70,  71,  72,  74,  75,  76, 186,
			222, 192,  16, 220,  90,  88,  67,  86,  66,  78,  77, 188, 190, 191,  62,  63,
			 64,  32,  20, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 144, 145,  30,
			 37,  24,  23,  38,  28,  39,  11,  25,  40,  29,  97,  91,  92,  93,  94, 122,
			123,  36,  98,  33, 100, 101, 102,  35, 104,  34,  45,  46, 108, 109,  19, 111,
			 15,  14,  26,  41,  42,  43,  31,  44, 106, 107,  95,  96, 124, 125, 126, 127,
			128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
			 58,  18, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
			160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
			176, 177, 178, 179, 180, 181, 182, 183, 184, 185,  47,  21,  59,  99,  60,  61,
			 10, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
			208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 105,  12, 103, 110, 223,
			224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
			240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
		};
		if (orgkey >= 0 && orgkey < 256)
			orgkey = key_translater[orgkey];
		#if defined(DEVICE_MAEMO)
			if (orgkey == 108)
				orgkey = 255;
			else if (orgkey == 111)
				orgkey = 37;
			else if (orgkey == 14)
				orgkey = 38;
			else if (orgkey == 26)
				orgkey = 39;
			else if (orgkey == 42)
				orgkey = 40;
		#endif
	#elif defined(DEVICE_DESKTOP) && defined(DEVICE_MAC)
		unsigned char key_translater[] = {
			  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
			 16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,
			 32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,
			 48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
			 64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
			 80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,
			 96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111,
			112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127,
			128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143,
			144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
			160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175,
			176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191,
			192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,
			208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223,
			224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
			240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255
		};
		if (orgkey >= 0 && orgkey < 256)
			orgkey = key_translater[orgkey];
	#endif
	return(orgkey);
}

//Read device calibration
void eciReadCalibration(void *in_ptr)
{
	#if defined(DEVICE_GP2X)
		int *values = (int *)in_ptr;
		struct stat sbuf;
		char pcalbuf[200];
		char *tokptr;
		int index, fileobj;
		int defaults[] = { 6203, 0, -1501397, 0, -4200, 16132680, 65536 };
		for (index = 0; index < 7; index++)
			values[index] = defaults[index];
		if (stat("/etc/pointercal", &sbuf) == 0)
		{
			fileobj = open("/etc/pointercal", O_RDONLY);
			if (fileobj)
			{
				read(fileobj, pcalbuf, 200);
				values[0] = atoi(strtok(pcalbuf," "));
				index = 1;
				while (index < 7)
				{
					tokptr = strtok(NULL, " ");
					if (*tokptr != '\0')
					{
						values[index] = atoi(tokptr);
						index++;
					}
				}
				close(fileobj);
			}
		}
	#endif
}

//Set long rectangle array
void eciInstSetRect(long *rc, long left, long top, long right, long bottom)
{
	rc[0] = left;
	rc[1] = top;
	rc[2] = right;
	rc[3] = bottom;
}

#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)

//Handle click movement outside of window
void eciHandleClickMove(EC_INSTANCEOBJ *io, HWND hwnd)
{
	POINT newpos;
	GetCursorPos(&newpos);
	if (newpos.x != io->clickpos_simmove[0] || newpos.y != io->clickpos_simmove[1])
	{
		POINT clientp;
		clientp.x = 0;
		clientp.y = 0;
		ClientToScreen(hwnd, &clientp);
		ecEvent event;
		event.type = ECEV_CLICKMOVE;
		event.param1 = 0;
		event.param2 = newpos.x - clientp.x;
		event.param3 = newpos.y - clientp.y;
		ecOnEvent(io, &event);
		io->clickpos_simmove[0] = newpos.x;
		io->clickpos_simmove[1] = newpos.y;
	}
}

//Timer thread
DWORD WINAPI eciTimerThread(LPVOID lpParameter)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)lpParameter;
	ecEvent event;
	event.type = ECEV_IDLE;
	while(1)
	{
		if (io->curwinstyle.outwndclickcursor)
			eciHandleClickMove(io, (HWND)ecInstGetWindow(io));
		ecOnEvent(io, &event);
		if (io->loopsleep)
		{
			Sleep(io->loopsleep);
			io->loopsleep = 0;
		}
		if (WaitForSingleObject(io->htimethreadexit, 0) == WAIT_OBJECT_0)
			break;
	}
	SetEvent(io->htimethreadbreak);
	return(0);
}

//Start timer thread
void eciStartTimer(EC_INSTANCEOBJ *io)
{
	char eventname[64], eventname2[64];
	ecStrCpy(eventname, "edgelib_timethread_exit_");
	ecIntToStr(&eventname[ecStrLen(eventname)], GetCurrentProcessId());
	ecStrCpy(eventname2, "edgelib_timethread_break_");
	ecIntToStr(&eventname2[ecStrLen(eventname)], GetCurrentProcessId());
	io->htimethreadexit = CreateEvent(NULL, TRUE, FALSE, eventname);
	if (io->htimethreadexit == NULL)
		return;
	io->htimethreadbreak = CreateEvent(NULL, TRUE, FALSE, eventname2);
	if (io->htimethreadbreak == NULL)
		return;
	io->htimethread = CreateThread(NULL, 0, eciTimerThread, (void *)io, CREATE_SUSPENDED, NULL);
	if (io->htimethread != NULL)
	{
		Sleep(10);
		ResumeThread(io->htimethread);
	}
	else
		SetEvent(io->htimethreadexit);
}

//Kill timer thread
void eciKillTimer(EC_INSTANCEOBJ *io)
{
	if (io->htimethread)
	{
		long safectr = 200;
		SetEvent(io->htimethreadexit);
		while(safectr > 0)
		{
			if (WaitForSingleObject(io->htimethreadbreak, 0) == WAIT_OBJECT_0)
				break;
			Sleep(10);
			safectr--;
		}
		CloseHandle(io->htimethread);
		io->htimethread = NULL;
	}
	if (io->htimethreadexit)
	{
		CloseHandle(io->htimethreadexit);
		io->htimethreadexit = NULL;
	}
	if (io->htimethreadbreak)
	{
		CloseHandle(io->htimethreadbreak);
		io->htimethreadexit = NULL;
	}
}

#endif

#if defined(DEVICE_WIN32)

//Timer thread
DWORD WINAPI eciWinMsgThread(LPVOID lpParameter)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)lpParameter;
	ecEvent event;
	while(io->sustainloop)
	{
		bool skipframe = false;
		if (io->sustainbacklight)
		{
			#if defined(DEVICE_WINCE)
				#if defined(DEVICE_WINMOBILE)
					SHIdleTimerReset();
				#endif
				SystemIdleTimerReset();
			#endif
		}
		#if defined(DEVICE_WINCE)
			DWORD syspowstate;
			WCHAR devbuf[32];
			if (GetSystemPowerState(devbuf, 32, &syspowstate) == ERROR_SUCCESS)
			{
				if (syspowstate & (POWER_STATE_OFF | POWER_STATE_SUSPEND | POWER_STATE_UNATTENDED))
					skipframe = true;
			}
		#else
			if (io->curwinstyle.outwndclickcursor)
				eciHandleClickMove(io, (HWND)ecInstGetWindow(io));
		#endif
		if (!skipframe)
		{
			event.type = ECEV_IDLE;
			ecOnEvent(io, &event);
			if (io->loopsleep)
			{
				Sleep(io->loopsleep);
				io->loopsleep = 0;
			}
		}
		else
		{
			event.type = ECEV_SKIPIDLE;
			ecOnEvent(io, &event);
			Sleep(100);
		}
		if (!io->sustainloop && io->wnd)
		{
			io->manualdestroy = true;
			DestroyWindow(io->wnd);
		}
	}
	SetEvent(io->htimethreadbreak);
	return(0);
}

//Start timer thread
void eciStartWinMsgThread(EC_INSTANCEOBJ *io)
{
	TCHAR eventname[64], eventname2[64];
	#if defined(UNICODE)
		ecStrCpy((ecUnistring)eventname, (const ecUnistring)TEXT("edgelib_msgthread_exit_"));
		ecIntToStr((ecUnistring)&eventname[ecStrLen((ecUnistring)eventname)], GetCurrentProcessId());
		ecStrCpy((ecUnistring)eventname2, (const ecUnistring)TEXT("edgelib_msgthread_break_"));
		ecIntToStr((ecUnistring)&eventname2[ecStrLen((ecUnistring)eventname)], GetCurrentProcessId());
	#else
		ecStrCpy(eventname, "edgelib_msgthread_exit_");
		ecIntToStr(&eventname[ecStrLen(eventname)], GetCurrentProcessId());
		ecStrCpy(eventname2, "edgelib_msgthread_break_");
		ecIntToStr(&eventname2[ecStrLen(eventname)], GetCurrentProcessId());
	#endif
	io->htimethreadexit = CreateEvent(NULL, TRUE, FALSE, eventname);
	if (io->htimethreadexit == NULL)
		return;
	io->htimethreadbreak = CreateEvent(NULL, TRUE, FALSE, eventname2);
	if (io->htimethreadbreak == NULL)
		return;
	io->htimethread = CreateThread(NULL, 0, eciWinMsgThread, (void *)io, CREATE_SUSPENDED, NULL);
	if (io->htimethread != NULL)
	{
		Sleep(10);
		ResumeThread(io->htimethread);
	}
	else
		SetEvent(io->htimethreadexit);
}

//Kill timer thread
void eciKillWinMsgThread(EC_INSTANCEOBJ *io)
{
	if (io->htimethread)
	{
		long safectr = 200;
		SetEvent(io->htimethreadexit);
		while(safectr > 0)
		{
			if (WaitForSingleObject(io->htimethreadbreak, 0) == WAIT_OBJECT_0)
				break;
			Sleep(10);
			safectr--;
		}
		CloseHandle(io->htimethread);
		io->htimethread = NULL;
	}
	if (io->htimethreadexit)
	{
		CloseHandle(io->htimethreadexit);
		io->htimethreadexit = NULL;
	}
	if (io->htimethreadbreak)
	{
		CloseHandle(io->htimethreadbreak);
		io->htimethreadexit = NULL;
	}
}

#endif

///////////////////////////////////////////////////////////////////
// Event API
///////////////////////////////////////////////////////////////////

//Create instance object
ECOREOBJ ecInstCreate(ecErr &errcode, ecParam param)
{
	errcode = 0;
	EC_INSTANCEOBJ *io = new EC_INSTANCEOBJ;
	if (io)
	{
		#if defined(DEVICE_WIN32)
			#if defined(DEVICE_WINMOBILE)
				io->menuwnd = NULL;
				io->dll_gapi = NULL;
				io->gapiok = true;
				io->lockedinput = false;
			#endif
			#if defined(DEVICE_WINCE)
				io->hreqbacklight = 0;
				io->failpowerrequirement = false;
			#endif
			io->instance = (HINSTANCE)param;
			io->htimethread = NULL;
			io->htimethreadexit = NULL;
			io->htimethreadbreak = NULL;
			io->manualdestroy = false;
		#elif defined(DEVICE_SYMBIAN)
			#if (DEVICE_SYMBIAN >= 900) && defined(DEVICE_S60)
				io->symtilt = NULL;
			#endif
			io->frameui = NULL;
			io->idletimer = NULL;
			io->runidletimer = false;
		#elif defined(DEVICE_MAC)
			io->view = NULL;
		#elif defined(DEVICE_OPENKODE)
			io->orglandscape = 0;
			io->ispaused = false;
			io->isfocusout = false;
		#endif
		#if (DEVICE_SYMBIAN >= 900)
			io->appid = 0xF0205D9D;
		#else
			io->appid = 0x10205D9D;
		#endif
		#if defined(DEVICE_LINUX)
			#if defined(DEVICE_GP2X)
				io->devicereg_ptr = 0;
				io->clickevent_ptr = -1;
				io->devicereg = NULL;
			#elif defined(DEVICE_ANDROID)
				// nothing extra
			#else
				io->backlighttimer = 0;
				io->suspended = false;
			#endif
		#endif
		#if defined(DEVICE_MAEMO) && !defined(DEVICE_MAEMOSIM)
			g_type_init();
		#endif
		unsigned char gctr;
		for (gctr = 0; gctr < EC_MAXINSTGLOBAL; gctr++)
			io->instglobal[gctr] = 0;
		io->wnd = NULL;
		io->loopsleep = 0;
		io->returnvalue = 0;
		io->curorientation = 0;
		io->sustainbacklight = false;
		ecStrCpy(io->cmdline, "");
		ecStrCpy(io->exiterrormsg, "");
		if (errcode != 0)
		{
			delete io;
			io = NULL;
		}
	}
	return(io);
}

//Destroy instance object
void ecInstDelete(ECOREOBJ inst)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	#if defined(DEVICE_SYMBIAN)
		#if (DEVICE_SYMBIAN >= 900) && defined(DEVICE_S60)
			if (io->symtilt)
				delete io->symtilt;
		#endif
		CIdle *itmr = (CIdle *)io->idletimer;
		if (itmr != NULL)
			delete itmr;
	#endif
	#if defined(DEVICE_WINMOBILE)
		if (io->dll_gapi)
			FreeLibrary(io->dll_gapi);
	#endif
	delete io;
}

//Set the commandline parameters
void ecInstSetCommandline(ECOREOBJ inst, const ecUnistring cmdline)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	ecStrCpy(io->cmdline, cmdline);
}

//Get the commandline parameters
void ecInstGetCommandline(ECOREOBJ inst, ecUnistring cmdline)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	ecStrCpy(cmdline, io->cmdline);
}

//Set the return value for a closing application
void ecInstSetReturnvalue(ECOREOBJ inst, int value)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	io->returnvalue = value;
}

//Set the error message to display on application exit
void ecInstSetErrorMessage(ECOREOBJ inst, const ecUnistring msg)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	ecStrCpy(io->exiterrormsg, msg);
}

//Return a window handle
ecNativetype ecInstGetWindow(ECOREOBJ inst)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	return((ecNativetype)io->wnd);
}

//Terminate program execution
void ecInstExit(ECOREOBJ inst)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	io->sustainloop = false;
	#if defined(DEVICE_SYMBIAN)
		if (io->idletimer == NULL || !io->runidletimer)
			ecInstCallIdleTimer((TAny *)io);
	#elif defined(DEVICE_MAC)
		ecEvent event;
		StopMacLoop(io);
		event.type = ECEV_ENDMAINLOOP;
		ecOnEvent(inst, &event);
		ecInstDestroyWindow(inst);
	#elif defined(DEVICE_ANDROID)
		Call_EdgelibApp_quitloop(io->jenv, io->jappobject);
	#endif
}

//Set a global
bool ecInstSetGlobal(ECOREOBJ inst, ecParam global, unsigned char index)
{
	if (index >= EC_MAXINSTGLOBAL)
		return(false);
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	io->instglobal[index] = global;
	return(true);
}

//Get a global
ecParam ecInstGetGlobal(ECOREOBJ inst, unsigned char index)
{
	if (index < EC_MAXINSTGLOBAL)
	{
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		return(io->instglobal[index]);
	}
	return(0);
}

//Set sleeping on frameloop (locking FPS)
void ecInstSetLoopSleep(ECOREOBJ inst, unsigned long tosleep)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	io->loopsleep = tosleep;
}

//Keep the backlight on (enable or disable)
void ecInstSustainBacklight(ECOREOBJ inst, bool enable)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	if (io->sustainbacklight != enable)
	{
		#if defined(DEVICE_WINCE)
			if (!io->failpowerrequirement)
			{
				if (io->hreqbacklight == 0 && enable)
				{
					//This one is bugged???
					//io->hreqbacklight = SetPowerRequirement(TEXT("BKL1:"), D0, POWER_NAME, NULL, 0);
					if (io->hreqbacklight == 0)
						io->failpowerrequirement = true;
				}
				else if (io->hreqbacklight != 0 && !enable)
				{
					ReleasePowerRequirement(io->hreqbacklight);
					io->hreqbacklight = 0;
				}
			}
			if (io->failpowerrequirement)
			{
				if (io->hreqbacklight == 0 && enable)
				{
					io->hreqbacklight = (HANDLE)RegistryBackLightOn();
				}
				else if (io->hreqbacklight != 0 && !enable)
				{
					RegistryBackLightOff((DWORD)io->hreqbacklight);
					io->hreqbacklight = 0;
				}
			}
		#endif
		#if defined(DEVICE_ANDROID)
		    Call_EdgelibApp_setBacklight(io->jenv, io->jappobject, (enable ? 1 : 0));
		#endif
		io->sustainbacklight = enable;
	}
}

//Native event callback procedure
int ecInstNativeCallback(ECOREOBJ inst, ecParam param1, ecParam param2, ecParam param3)
{
	if (inst == NULL)
		return(0);
	int nativereturn = 0;
	#if defined(DEVICE_WIN32)
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		ecEvent event;
		UINT wmsg = (UINT)param1;
		WPARAM wparam = (WPARAM)param2;
		LPARAM lparam = (LPARAM)param3;
		event.type = ECEV_NATIVE;
		event.param1 = param1;
		event.param2 = param2;
		event.param3 = param3;
		if (ecOnEvent(inst, &event))
			nativereturn = 1;
		switch(wmsg)
		{
			case WM_KEYDOWN:
				if (!(lparam & 0x40000000) && wparam > 0)
				{
					event.type = ECEV_KEYDOWN;
					event.param1 = wparam;
					ecOnEvent(inst, &event);
				}
				return(1);
			case WM_KEYUP:
				if (wparam > 0)
				{
					event.type = ECEV_KEYUP;
					event.param1 = wparam;
					ecOnEvent(inst, &event);
				}
				return(1);
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
				#if defined(DEVICE_DESKTOP)
					SetCapture(io->wnd);
				#endif
				event.type = ECEV_CLICKDOWN;
				if (wmsg == WM_LBUTTONDOWN)
					event.param1 = 0;
				else if (wmsg == WM_RBUTTONDOWN)
					event.param1 = 1;
				else if (wmsg == WM_MBUTTONDOWN)
					event.param1 = 2;
				event.param2 = LOWORD(lparam);
				event.param3 = HIWORD(lparam);
				#if defined(DEVICE_DESKTOP)
					io->clickstate_capt[event.param1] = true;
				#endif
				ecOnEvent(inst, &event);
				return(1);
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
				event.type = ECEV_CLICKUP;
				if (wmsg == WM_LBUTTONUP)
					event.param1 = 0;
				else if (wmsg == WM_RBUTTONUP)
					event.param1 = 1;
				else if (wmsg == WM_MBUTTONUP)
					event.param1 = 2;
				event.param2 = LOWORD(lparam);
				event.param3 = HIWORD(lparam);
				#if defined(DEVICE_DESKTOP)
					io->clickstate_capt[event.param1] = false;
				#endif
				ecOnEvent(inst, &event);
				#if defined(DEVICE_DESKTOP)
					ReleaseCapture();
				#endif
				return(1);
			case WM_LBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
				#if defined(DEVICE_DESKTOP)
					SetCapture(io->wnd);
				#endif
				event.type = ECEV_CLICKDOUBLE;
				if (wmsg == WM_LBUTTONDBLCLK)
					event.param1 = 0;
				else if (wmsg == WM_RBUTTONDBLCLK)
					event.param1 = 1;
				else if (wmsg == WM_MBUTTONDBLCLK)
					event.param1 = 2;
				event.param2 = LOWORD(lparam);
				event.param3 = HIWORD(lparam);
				#if defined(DEVICE_DESKTOP)
					io->clickstate_capt[event.param1] = true;
				#endif
				ecOnEvent(inst, &event);
				return(1);
			case WM_MOUSEMOVE:
				event.type = ECEV_CLICKMOVE;
				event.param1 = 0;
				event.param2 = LOWORD(lparam);
				event.param3 = HIWORD(lparam);
				ecOnEvent(inst, &event);
				return(1);
			case WM_CHAR:
				event.type = ECEV_TYPECHAR;
				event.param1 = wparam;
				event.param2 = MapVirtualKey(lparam >> 16, 1);
				ecOnEvent(inst, &event);
				return(1);
			#if defined(DEVICE_DESKTOP)
				case 0x20A: //WM_MOUSEWHEEL
					event.type = ECEV_WHEELSCROLL;
					if ((int)(wparam) < 0)
						event.param1 = -1;
					else
						event.param1 = 1;
					ecOnEvent(inst, &event);
					return(1);
			#endif
			#if defined(DEVICE_WINMOBILE)
				case WM_USER + 11111: //WM_USER_TILTDATA
					event.type = ECEV_CHANGETILT;
					event.param1 = (wparam & 65535) - 32768;
					event.param2 = 32768 - (wparam >> 16);
					event.param3 = 0;
					ecOnEvent(inst, &event);
					return(1);
			#endif
			case WM_CREATE:
				#if defined(DEVICE_WINMOBILE)
					if (io->curwinstyle.menu)
					{
						SHMENUBARINFO mbi;
						ecMemSet(&mbi, 0, sizeof(SHMENUBARINFO));
						mbi.cbSize     = sizeof(SHMENUBARINFO);
						mbi.hwndParent = io->wnd;
						mbi.nToolBarId = io->curwinstyle.menu;
						mbi.hInstRes   = io->instance;
						mbi.nBmpId     = 0;
						mbi.cBmpImages = 0;
						if (SHCreateMenuBar(&mbi))
							io->menuwnd = mbi.hwndMB;
					}
					SHSipPreference(io->wnd, SIP_DOWN);
					ecMemSet(&io->activateinfo, 0, sizeof(SHACTIVATEINFO));
					io->activateinfo.cbSize = sizeof(SHACTIVATEINFO);
				#elif defined(DEVICE_DESKTOP)
					POINT realcursorpos_create;
					if (!io->curwinstyle.showclickcursor)
						ShowCursor(FALSE);
					GetCursorPos(&realcursorpos_create);
					io->clickpos_simmove[0] = realcursorpos_create.x;
					io->clickpos_simmove[1] = realcursorpos_create.y;
				#endif
				break;
			case WM_TIMER:
				#if defined(DEVICE_DESKTOP)
					if (io->curwinstyle.outwndclickcursor)
						eciHandleClickMove(io, (HWND)ecInstGetWindow(io));
				#endif
				event.type = ECEV_IDLE;
				ecOnEvent(io, &event);
				break;
			#if defined(DEVICE_DESKTOP)
				case WM_INITDIALOG:
					POINT realcursorpos_dlg;
					io->sustainloop = true;
					event.type = ECEV_CREATEWINDOW;
					event.param1 = 1;
					ecOnEvent(io, &event);
					GetCursorPos(&realcursorpos_dlg);
					io->clickpos_simmove[0] = realcursorpos_dlg.x;
					io->clickpos_simmove[1] = realcursorpos_dlg.y;
					if (io->sustainloop)
					{
						if (io->curwinstyle.corethread)
							eciStartTimer(io);
						else
							SetTimer(io->wnd, 1, 10, NULL);
					}
					else
						EndDialog(io->wnd, IDABORT);
					break;
			#endif
			case WM_SYSCOMMAND:
				DWORD dwCommand;
				dwCommand = (wparam & 0xFFF0);
				switch(dwCommand)
				{
					//case SC_MINIMIZE:
						//SendMessage(io->wnd, WM_SHOWWINDOW, FALSE, 0);
						//return(0);
					case SC_CLOSE:
						ecInstExit(io);
						return(0);
					#if defined(DEVICE_DESKTOP)
						case SC_MOVE:
						case SC_SIZE:
						case SC_MAXIMIZE:
						case SC_KEYMENU:
						case SC_MONITORPOWER:
							if (io->curwinstyle.fullscreen)
								return(1);
							break;
					#endif
				}
				break;
			case WM_SHOWWINDOW:
				if (wparam == FALSE)
				{
					#if defined(DEVICE_WINMOBILE)
						if (io->curwinstyle.fullscreen == 2)
							SHFullScreen(io->wnd, SHFS_SHOWSTARTICON | SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON);
						else if (io->curwinstyle.fullscreen == 1 && io->curwinstyle.menu == 0)
							SHFullScreen(io->wnd, SHFS_SHOWSIPBUTTON);
						if (io->curwinstyle.lockkeys)
							eciHandleKeyLock(io, false);
						if (!io->curwinstyle.keysounds)
							eciHandleKeyClick(io, 0, true, true);
					#endif
					event.type = ECEV_CHANGEFOCUS;
					event.param1 = 2;
					ecOnEvent(io, &event);
				}
				return(0);
			case WM_KILLFOCUS:
				SendMessage(io->wnd, WM_ACTIVATE, WA_INACTIVE, 0);
				return(0);
			case WM_ACTIVATE:
				#if defined(DEVICE_WINMOBILE)
					if (io->curwinstyle.fullscreen == 1)
						SHHandleWMActivate(io->wnd, wparam, lparam, &io->activateinfo, FALSE);
				#endif
				event.type = ECEV_CHANGEFOCUS;
				if (LOWORD(wparam) == WA_INACTIVE)
				{
					#if defined(DEVICE_WINMOBILE)
						if (io->curwinstyle.fullscreen == 2)
							SHFullScreen(io->wnd, SHFS_SHOWSTARTICON | SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON);
						else if (io->curwinstyle.fullscreen == 1 && io->curwinstyle.menu == 0)
							SHFullScreen(io->wnd, SHFS_SHOWSIPBUTTON);
						if (io->curwinstyle.lockkeys)
							eciHandleKeyLock(io, false);
						if (!io->curwinstyle.keysounds)
							eciHandleKeyClick(io, 0, true, true);
					#endif
					event.param1 = 1;
				}
				else
				{
					#if defined(DEVICE_WINMOBILE)
						if (io->curwinstyle.fullscreen > 0)
						{
							if (io->curwinstyle.fullscreen == 2)
							{
								SHSipPreference(io->wnd, SIP_DOWN);
								SHFullScreen(io->wnd, SHFS_HIDESTARTICON | SHFS_HIDETASKBAR | SHFS_HIDESIPBUTTON);
							}
							else if (io->curwinstyle.menu == 0)
							{
								SHSipPreference(io->wnd, SIP_DOWN);
								SHFullScreen(io->wnd, SHFS_HIDESIPBUTTON);
							}
						}
						if (io->curwinstyle.lockkeys)
							eciHandleKeyLock(io, true);
						if (!io->curwinstyle.keysounds)
						{
							eciHandleKeyClick(io);
							eciHandleKeyClick(io, 0, true);
						}
					#endif
					event.param1 = 0;
				}
				ecOnEvent(io, &event);
				break;
			case WM_SETTINGCHANGE:
				#if defined(DEVICE_WINMOBILE)
					SHHandleWMSettingChange(io->wnd, wparam, lparam, &io->activateinfo);
				#endif
				#if defined(DEVICE_WINCE)
					if (wparam == SETTINGCHANGE_RESET)
					{
						event.type = ECEV_CHANGEORIENTATION;
						event.param1 = ecDspInfGetOrientation((ecNativetype)io->wnd);
						io->curorientation = event.param1;
						ecOnEvent(io, &event);
					}
				#endif
     			break;
			#if defined(DEVICE_DESKTOP)
				case WM_SETCURSOR:
					if (!io->curwinstyle.showclickcursor)
						SetCursor(NULL);
					break;
				case WM_CAPTURECHANGED:
					if (lparam != (LPARAM)io->wnd)
					{
						POINT curp;
						DWORD curplp;
						GetCursorPos(&curp);
						ScreenToClient(io->wnd, &curp);
						curplp = MAKELPARAM(curp.x, curp.y);
						if (io->clickstate_capt[0])
							SendMessage(io->wnd, WM_LBUTTONUP, 0, curplp);
						if (io->clickstate_capt[1])
							SendMessage(io->wnd, WM_RBUTTONUP, 0, curplp);
						if (io->clickstate_capt[2])
							SendMessage(io->wnd, WM_MBUTTONUP, 0, curplp);
					}
					break;
				case WM_SIZE:
					if ((wparam == SIZE_MAXHIDE) || (wparam == SIZE_MINIMIZED))
						SendMessage(io->wnd, WM_SHOWWINDOW, FALSE, 0);
					return(0);
				case WM_NCHITTEST:
					if (io->curwinstyle.fullscreen)
						return(HTCLIENT);
					break;
			#endif
			case WM_PAINT:
				event.type = ECEV_REDRAW;
				ecOnEvent(io, &event);
				break;
			case WM_CLOSE:
				#if defined(DEVICE_DESKTOP)
					if (io->curwinstyle.dialogbox)
					{
						EndDialog(io->wnd, IDABORT);
						io->wnd = NULL;
						return(1);
					}
				#endif
				return(0);
			case WM_DESTROY:
				#if defined(DEVICE_WINMOBILE)
					if (io->curwinstyle.menu)
						if (io->menuwnd)
							CommandBar_Destroy(io->menuwnd);
				#else
					#if defined(DEVICE_DESKTOP)
						if (io->curwinstyle.dialogbox)
						{
							if (io->curwinstyle.corethread)
								eciKillTimer(io);
							else
								KillTimer(io->wnd, 1);
						}
					#endif
					if (!io->curwinstyle.showclickcursor)
					{
						SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
						ShowCursor(TRUE);
					}
				#endif
				event.type = ECEV_DESTROYWINDOW;
				ecOnEvent(io, &event);
				if (!io->manualdestroy)
				{
					long errcreate;
					io->wnd = NULL;
					io->sustainloop = false;
					event.type = ECEV_DESTROYINSTANCE;
					ecOnEvent(inst, &event);
					#if defined(DEVICE_DESKTOP)
						if (io->curwinstyle.dialogbox)
							if (io->exiterrormsg[0])
								MessageBoxW(NULL, (LPCWSTR)io->exiterrormsg, L"Application error", MB_OK | MB_ICONERROR);
					#endif
					errcreate = io->returnvalue;
					PostQuitMessage(errcreate);
				}
				return(1);
		}
	#elif defined(DEVICE_SYMBIAN)
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		ecEvent event;
		TWsEvent *aEvent = (TWsEvent *)param1;
		event.type = ECEV_NATIVE;
		event.param1 = param1;
		event.param2 = param2;
		event.param3 = param3;
		if (ecOnEvent(inst, &event))
			nativereturn = 1;
		switch(aEvent->Type())
		{
			case EEventKey:
				{
					ecParam keynr;
					ecParam asciikey;
					asciikey = aEvent->Key()->iCode;
					keynr = aEvent->Key()->iScanCode;
					if (keynr >= EStdKeyDevice0 && keynr <= EStdKeyDeviceF)
						asciikey = 0;
					if (keynr >= EStdKeyApplication0 && keynr <= EStdKeyApplication9)
						asciikey = 0;
					if (keynr == EStdKeyUpArrow || keynr == EStdKeyDownArrow || keynr == EStdKeyLeftArrow || keynr == EStdKeyRightArrow)
						asciikey = 0;
					if (asciikey != 0)
					{
						event.type = ECEV_TYPECHAR;
						event.param1 = asciikey;
						event.param2 = keynr;
						ecOnEvent(io, &event);
					}
					nativereturn = 0;
				}
				break;
			case EEventKeyUp:
			case EEventKeyDown:
				{
					ecParam keynr;
					#if defined(DEVICE_UIQ)
						if (aEvent->Type() == EEventKeyDown)
						{
							if (aEvent->Key()->iScanCode == EStdKeyDevice1)
							{
								event.type = ECEV_WHEELSCROLL;
								event.param1 = 1;
								ecOnEvent(inst, &event);
								return(1);
							}
							else if (aEvent->Key()->iScanCode == EStdKeyDevice2)
							{
								event.type = ECEV_WHEELSCROLL;
								event.param1 = -1;
								ecOnEvent(inst, &event);
								return(1);
							}
						}
					#endif
					keynr = aEvent->Key()->iScanCode;
					if (aEvent->Type() == EEventKeyUp)
						event.type = ECEV_KEYUP;
					else
						event.type = ECEV_KEYDOWN;
					#if defined(DEVICE_SERIES80)
						if (keynr == EStdKeyDevice8)
							keynr = EStdKeyUpArrow;
						else if (keynr == EStdKeyDevice9)
							keynr = EStdKeyDownArrow;
						else if (keynr == EStdKeyDevice6)
							keynr = EStdKeyLeftArrow;
						else if (keynr == EStdKeyDevice7)
							keynr = EStdKeyRightArrow;
					#elif defined(DEVICE_UIQ)
						if (keynr == EStdKeyDevice4)
							keynr = EStdKeyUpArrow;
						else if (keynr == EStdKeyDevice5)
							keynr = EStdKeyDownArrow;
						else if (keynr == EStdKeyDevice6)
							keynr = EStdKeyLeftArrow;
						else if (keynr == EStdKeyDevice7)
							keynr = EStdKeyRightArrow;
					#endif
					event.param1 = keynr;
					ecOnEvent(inst, &event);
				}
				return(1);
			case EEventPointer:
				{
					if (aEvent->Pointer()->iType == TPointerEvent::EButton2Down || aEvent->Pointer()->iType == TPointerEvent::EButton2Up)
						event.param1 = 1;
					else if (aEvent->Pointer()->iType == TPointerEvent::EButton3Down || aEvent->Pointer()->iType == TPointerEvent::EButton3Up)
						event.param1 = 2;
					else
						event.param1 = 0;
					event.param2 = aEvent->Pointer()->iPosition.iX;
					event.param3 = aEvent->Pointer()->iPosition.iY;
					switch(aEvent->Pointer()->iType)
					{
						case TPointerEvent::EButton1Down:
						case TPointerEvent::EButton2Down:
						case TPointerEvent::EButton3Down:
						case TPointerEvent::EButtonRepeat:
							if ((aEvent->Pointer()->iModifiers & EModifierDoubleClick) == EModifierDoubleClick)
								event.type = ECEV_CLICKDOUBLE;
							else
								event.type = ECEV_CLICKDOWN;
							break;
						case TPointerEvent::EButton1Up:
						case TPointerEvent::EButton2Up:
						case TPointerEvent::EButton3Up:
							event.type = ECEV_CLICKUP;
							break;
						case TPointerEvent::EDrag:
							event.type = ECEV_CLICKMOVE;
							break;
						default:
							event.type = ECEV_CLICKMOVE;
							break;
					}
					ecOnEvent(inst, &event);
				}
				return(1);
			//case EEventScreenDeviceChanged:
			case EEventSwitchOff:
			case EEventCaseClosed:
			case EEventFocusLost:
				{
					if (io->idletimer && io->runidletimer && io->curwinstyle.focussuspend)
					{
						CIdle *itmr = (CIdle *)io->idletimer;
						itmr->Cancel();
						io->runidletimer = false;
					}
					event.type = ECEV_CHANGEFOCUS;
					event.param1 = 2;
					ecOnEvent(inst, &event);
				}
				return(1);
			case EEventFocusGained:
				{
					event.type = ECEV_CHANGEFOCUS;
					event.param1 = 0;
					ecOnEvent(inst, &event);
					if (io->idletimer && !io->runidletimer)
					{
						CIdle *itmr = (CIdle *)io->idletimer;
						itmr->Start(TCallBack(ecInstCallIdleTimer, inst));
						io->runidletimer = true;
					}
				}
				return(1);
		}
	#elif defined(DEVICE_X11)
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		XEvent evt;
		XComposeStatus xcompose;
		KeySym xkeysim;
		ecEvent event;
		char xkeybuf[20];
		int xkeybufsize = 20, xcharcount;
		bool wassuspended = io->suspended && io->curwinstyle.focussuspend; // allow for a reaction before the place is locked down.
		while (XPending((Display *)io->wnd[0]) || (io->suspended && wassuspended) )
		{
			XNextEvent((Display *)io->wnd[0], &evt); // will block when there are no events (saving power)
			event.type = ECEV_NATIVE;
			event.param1 = (ecNativetype)evt.type;
			event.param2 = (ecNativetype)&evt;
			event.param3 = 0;
			if (ecOnEvent(inst, &event))
				nativereturn = 1;
			switch(evt.type)
			{
				case KeyPress:
				case KeyRelease:
					if (evt.type == KeyPress)
					{
						xcharcount = XLookupString(&evt.xkey, xkeybuf, xkeybufsize, &xkeysim, &xcompose);
						if (xkeysim > 0/* && xkeysim < 32768*/ && xcharcount > 0)
						{
							int kbbufctr;
							event.type = ECEV_TYPECHAR;
							for (kbbufctr = 0; kbbufctr < xcharcount; kbbufctr++)
							{
								event.param1 = xkeybuf[kbbufctr];
								event.param2 = evt.xkey.keycode;
								ecOnEvent(io, &event);
							}
						}
						event.type = ECEV_KEYDOWN;
					}
					else
						event.type = ECEV_KEYUP;
					if (evt.xkey.keycode < 256)
					{
						if (event.type == ECEV_KEYDOWN && (io->xdblkeyboard[evt.xkey.keycode / 8] & (1 << (evt.xkey.keycode % 8))))
							break;
						if (event.type == ECEV_KEYUP && !(io->xdblkeyboard[evt.xkey.keycode / 8] & (1 << (evt.xkey.keycode % 8))))
							break;
						if (event.type == ECEV_KEYDOWN)
							io->xdblkeyboard[evt.xkey.keycode / 8] |= (1 << (evt.xkey.keycode % 8));
						else
							io->xdblkeyboard[evt.xkey.keycode / 8] -= io->xdblkeyboard[evt.xkey.keycode / 8] & (1 << (evt.xkey.keycode % 8));
					}
					event.param1 = eciTranslateKeycode(evt.xkey.keycode);
					ecOnEvent(io, &event);
					break;
				case ButtonPress:
				case ButtonRelease:
					if (evt.xbutton.button == 4 || evt.xbutton.button == 5)
					{
						if (evt.type == ButtonPress)
						{
							event.type = ECEV_WHEELSCROLL;
							event.param1 = 1 - (evt.xbutton.button - 4) * 2;
							ecOnEvent(io, &event);
						}
						break;
					}
					else if (evt.xbutton.button == 0)
						break;
					if (evt.type == ButtonPress)
						event.type = ECEV_CLICKDOWN;
					else
						event.type = ECEV_CLICKUP;
					event.param1 = evt.xbutton.button - 1;
					event.param2 = evt.xbutton.x;
					event.param3 = evt.xbutton.y;
					ecOnEvent(io, &event);
					break;
				case MotionNotify:
					event.type = ECEV_CLICKMOVE;
					event.param1 = 0;
					event.param2 = evt.xmotion.x;
					event.param3 = evt.xmotion.y;
					ecOnEvent(io, &event);
					break;
				/*case VisibilityNotify:
					event.type = ECEV_CHANGEFOCUS;
					if (evt.xvisibility.state == VisibilityUnobscured)
						event.param1 = 0;
					else
						event.param1 = 1;
					ecOnEvent(io, &event);
					break;*/
				case FocusIn:
				case FocusOut:
					event.type = ECEV_CHANGEFOCUS;
					if (evt.type == FocusIn)
					{
						event.param1 = 0;
						io->suspended = false;
					}
					else
					{
						event.param1 = 1;
						io->suspended = true;
					}
					ecOnEvent(io, &event);
					break;
				case Expose:
					if (evt.xexpose.count == 0)
					{
						event.type = ECEV_REDRAW;
						ecOnEvent(io, &event);
					}
					break;
				case MappingNotify:
					XRefreshKeyboardMapping(&evt.xmapping);
					break;
				case ClientMessage:
					if (evt.xclient.data.l[0] == io->xwm_delete)
						io->sustainloop = false;
					return(1);
				case DestroyNotify:
					io->sustainloop = false;
					break;
			}
		}
	#elif defined(DEVICE_OPENKODE)
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		KDEvent *kdevent = (KDEvent *)param1;
		ecEvent event;
		event.type = ECEV_NATIVE;
		event.param1 = (ecNativetype)kdevent->type;
		event.param2 = (ecNativetype)kdevent;
		event.param3 = 0;
		if (ecOnEvent(inst, &event))
			nativereturn = 1;
		switch(kdevent->type)
		{
			case KD_EVENT_INPUT_KEYCHAR_ATX:
				event.type = ECEV_TYPECHAR;
				event.param1 = kdevent->data.inputkeycharATX.keychar;
				event.param2 = 0;
				if (event.param1 < 65536)
					ecOnEvent(io, &event);
				break;
			case KD_EVENT_INPUT:
				if (kdevent->data.input.value.i)
					event.type = ECEV_KEYDOWN;
				else
					event.type = ECEV_KEYUP;
				event.param1 = kdevent->data.input.index;
				if (event.param1 >= KD_INPUT_GAMEKEYSNC_UP && event.param1 <= KD_INPUT_GAMEKEYSNC_D)
				{
					int gkavail;
					kdStateGeti(KD_STATE_GAMEKEYS_AVAILABILITY, 1, &gkavail);
					if ((gkavail & 31) != 31)
						event.param1 = event.param1 - KD_INPUT_GAMEKEYSNC_UP + KD_INPUT_GAMEKEYS_UP;
					else if (!(gkavail & (1 << (event.param1 - KD_INPUT_GAMEKEYSNC_UP))))
						event.param1 = event.param1 - KD_INPUT_GAMEKEYSNC_UP + KD_INPUT_GAMEKEYS_UP;
				}
				ecOnEvent(inst, &event);
				if (event.type == ECEV_KEYDOWN)
				{
					event.param1 = 0;
					if (event.param1 >= KD_INPUT_PHONEKEYPAD_0 && event.param1 <= KD_INPUT_PHONEKEYPAD_9)
						event.param1 = kdevent->data.input.index - KD_INPUT_PHONEKEYPAD_0 + '0';
					if (event.param1 == KD_INPUT_PHONEKEYPAD_STAR)
						event.param1 = '*';
					if (event.param1 == KD_INPUT_PHONEKEYPAD_HASH)
						event.param1 = '#';
					if (event.param1)
					{
						event.type = ECEV_TYPECHAR;
						event.param2 = kdevent->data.input.index;
						ecOnEvent(inst, &event);
					}
				}
				break;
			case KD_EVENT_INPUT_JOG:
				event.type = ECEV_WHEELSCROLL;
				event.param1 = 0;
				if (kdevent->data.inputjog.index == 24577)
					event.param1 += kdevent->data.inputjog.count;
				else if (kdevent->data.inputjog.index == 24580)
					event.param1 -= kdevent->data.inputjog.count;
				if (event.param1)
					ecOnEvent(inst, &event);
				break;
			case KD_EVENT_INPUT_POINTER:
				if (kdevent->data.inputpointer.index & 2)
				{
					if (kdevent->data.inputpointer.select)
						event.type = ECEV_CLICKDOWN;
					else
						event.type = ECEV_CLICKUP;
				}
				else
					event.type = ECEV_CLICKMOVE;
				event.param1 = 0;
				event.param2 = kdevent->data.inputpointer.x;
				event.param3 = kdevent->data.inputpointer.y;
				ecOnEvent(inst, &event);
				break;
			case KD_EVENT_PAUSE:
				io->ispaused = true;
				event.type = ECEV_CHANGEFOCUS;
				event.param1 = 2;
				ecOnEvent(inst, &event);
				break;
			case KD_EVENT_RESUME:
				io->ispaused = false;
				event.type = ECEV_CHANGEFOCUS;
				event.param1 = 0;
				if (!io->isfocusout)
					ecOnEvent(inst, &event);
				break;
			case KD_EVENT_WINDOW_FOCUS:
				event.type = ECEV_CHANGEFOCUS;
				if (kdevent->data.windowfocus.focusstate)
				{
					io->isfocusout = false;
					event.param1 = 0;
				}
				else
				{
					io->isfocusout = true;
					event.param1 = 1;
				}
				if (!io->ispaused)
					ecOnEvent(inst, &event);
				break;
			case KD_EVENT_WINDOW_REDRAW:
				event.type = ECEV_REDRAW;
				ecOnEvent(inst, &event);
				break;
			case KD_EVENT_ORIENTATION: //Dirty window size check due to lack of orientation parameter
				{
					KDint32 windowsize[2];
					kdGetWindowPropertyiv((KDWindow *)io->wnd_arr[3], KD_WINDOWPROPERTY_SIZE, windowsize);
					event.type = ECEV_CHANGEORIENTATION;
					if (io->orglandscape && windowsize[0] < windowsize[1])
						event.param1 = 90;
					else if (!io->orglandscape && windowsize[0] > windowsize[1])
						event.param1 = 270;
					else
						event.param1 = 0;
					ecOnEvent(inst, &event);
				}
				break;
			case KD_EVENT_WINDOWPROPERTY_CHANGE:
				if (kdevent->data.windowproperty.pname == KD_WINDOWPROPERTY_SIZE)
				{
					KDint32 windowsize[2];
					kdGetWindowPropertyiv((KDWindow *)io->wnd_arr[3], KD_WINDOWPROPERTY_SIZE, windowsize);
					event.type = ECEV_CHANGEORIENTATION;
					if (io->orglandscape > 0 && windowsize[0] < windowsize[1])
						event.param1 = 270;
					else if (io->orglandscape < 0 && windowsize[0] > windowsize[1])
						event.param1 = 90;
					else
						event.param1 = 0;
					ecOnEvent(inst, &event);
				}
				break;
			case KD_EVENT_NAME_LOOKUP_COMPLETE:
				ecNetOnHandler(kdevent->userptr, ECEV_NET_HOSTLOOKUP, &kdevent->data.namelookup);
				break;
			case KD_EVENT_SOCKET_CONNECT_COMPLETE:
				ecNetOnHandler(kdevent->userptr, ECEV_NET_HOSTCONNECT, &kdevent->data.socketconnect);
				break;
			case KD_EVENT_SOCKET_INCOMING:
				ecNetOnHandler(kdevent->userptr, ECEV_NET_ADDCLIENT, &kdevent->data.socketincoming);
				break;
			case KD_EVENT_SOCKET_READABLE:
				ecNetOnHandler(kdevent->userptr, ECEV_NET_READABLE, &kdevent->data.socketincoming);
				break;
			case KD_EVENT_BT_DEVICE_DISCOVERED_ATX:
				ecNetOnHandler(kdevent->userptr, ECEV_NET_FOUNDBTDEVICE, &kdevent->data.btdeviceATX);
				break;
			case KD_EVENT_BT_SERVICE_DISCOVERED_ATX:
				ecNetOnHandler(kdevent->userptr, ECEV_NET_FOUNDBTSERVICE, &kdevent->data.btserviceATX);
				break;
			case KD_EVENT_BT_NAME_ATX:
				ecNetOnHandler(kdevent->userptr, ECEV_NET_BTDEVICENAME, &kdevent->data.btnameATX);
				break;
		}
	#endif
	return(nativereturn);
}

//Create a window
//Errcode: E_OK, E_ALREADYFOUND, E_UNSUPPORTED, E_ERROR
ecErr ecInstCreateWindow(ECOREOBJ inst, EC_WINDOWSTYLE *winstyle)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	if (winstyle != NULL)
		ecMemCpy(&io->curwinstyle, winstyle, sizeof(EC_WINDOWSTYLE));
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_DESKTOP)
			for (unsigned char clickstatectr = 0; clickstatectr < 3; clickstatectr++)
				io->clickstate_capt[clickstatectr] = false;
			io->statuswnd = NULL;
			io->cbuttonwnd = NULL;
			io->ibuttonwnd = NULL;
			if (winstyle->dialogbox)
			{
				DialogBoxParam(io->instance, MAKEINTRESOURCE(winstyle->dialogbox), NULL, ecInstDlgProc, (LPARAM)io);
				return(0);
			}
		#else
			io->curwinstyle.dialogbox = 0;
		#endif
		WNDCLASS wc;
		DWORD dwStyle = 0, dwExStyle = 0;
		TCHAR wndclass[64], caption[64];
		#if defined(DEVICE_WINMOBILE)
			SHInitExtraControls();
		#endif
		ecStrCpy((ECTCHAR)wndclass, winstyle->title);
		ecStrCat((ECTCHAR)wndclass, (ecUnistring)L"_class");
		ecStrCpy((ECTCHAR)caption, winstyle->title);
		if (!winstyle->allowdoubleinst)
		{
			HWND hwnd;
			hwnd = FindWindow(wndclass, caption);
			if (hwnd)
			{
				SetForegroundWindow((HWND)(((DWORD)hwnd) | 0x01));
				return(1);
			}
		}
		ecMemSet(&wc, 0, sizeof(wc));
		if (winstyle->icon == 0)
		{
			#if defined(DEVICE_DESKTOP)
				wc.hIcon = LoadIcon(io->instance, IDI_APPLICATION);
			#else
				wc.hIcon = 0;
			#endif
		}
		else
			wc.hIcon = LoadIcon(io->instance, MAKEINTRESOURCE(winstyle->icon));
		if (winstyle->menu)
		{
			#if defined(DEVICE_WINMOBILE)
				wc.lpszMenuName = NULL;
			#else
				wc.lpszMenuName = MAKEINTRESOURCE(winstyle->menu);
			#endif
		}
		else
			wc.lpszMenuName = NULL;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.style         = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;
		wc.lpfnWndProc   = (WNDPROC)ecInstWinProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = io->instance;
		wc.hbrBackground = 0;
		wc.lpszClassName = wndclass;
		if (RegisterClass(&wc) == NULL)
			return(3);
		if (winstyle->fullscreen == 0)
		{
			long height = GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
			if (winstyle->winborder)
				dwStyle = WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_DLGFRAME;
			else
				dwStyle = WS_VISIBLE | WS_POPUP | WS_MINIMIZEBOX | WS_SYSMENU;
			if (winstyle->menu)
				height += GetSystemMetrics(SM_CYMENU);
			io->wnd = CreateWindowEx(dwExStyle, wndclass, caption, dwStyle, GetSystemMetrics(SM_CXSCREEN) / 2 - 64, GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2, 128, height, NULL, NULL, io->instance, (LPVOID)io);
		}
		else
		{
			dwStyle = WS_VISIBLE;
			#if defined(DEVICE_DESKTOP)
				dwStyle |= WS_POPUP;
			#else
				//if (winstyle->fullscreen == 2)
				//	dwStyle |= WS_POPUP;
			#endif
			io->wnd = CreateWindowEx(dwExStyle, wndclass, caption, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, io->instance, (LPVOID)io);
			#if defined(DEVICE_WINMOBILE)
				if (winstyle->fullscreen == 1)
				{
					RECT winrc;
					if (winstyle->menu > 0)
					{
						GetWindowRect(io->wnd, &winrc);
						if (winrc.bottom == GetSystemMetrics(SM_CYSCREEN))
							winrc.bottom -= winrc.top;
						MoveWindow(io->wnd, winrc.left, winrc.top, winrc.right - winrc.left, winrc.bottom - winrc.top, FALSE);
					}
					else
					{
						GetWindowRect(io->wnd, &winrc);
						winrc.bottom = GetSystemMetrics(SM_CYSCREEN);
						MoveWindow(io->wnd, winrc.left, winrc.top, winrc.right - winrc.left, winrc.bottom - winrc.top, FALSE);
					}
				}
				else
					MoveWindow(io->wnd, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), TRUE);
			#endif
		}
		if (io->wnd == NULL)
			return(3);
		SetFocus(io->wnd);
		ShowWindow(io->wnd, SW_SHOW);
		ecEvent event;
		#if defined(DEVICE_WINCE)
			event.type = ECEV_CHANGEORIENTATION;
			io->curorientation = ecDspInfGetOrientation((ecNativetype)io->wnd);
			event.param1 = io->curorientation;
			if (event.param1 != 0)
				ecOnEvent(io, &event);
		#endif
		event.type = ECEV_CREATEWINDOW;
		event.param1 = 0;
		ecOnEvent(io, &event);
		return(0);
	#elif defined(DEVICE_SYMBIAN)
		#if (DEVICE_SYMBIAN >= 900) && defined(DEVICE_S60)
			io->symtilt = new CSymSensorTilt(io);
			if (io->symtilt == NULL)
				return(3);
		#endif
		io->frameui = new CSymCoreUi(inst);
		if (io->frameui == NULL)
			return(3);
		return(0);
	#elif defined(DEVICE_LINUX)
		struct stat sbuf;
		#if defined(DEVICE_GP2X)
			io->devicereg_ptr = open("/dev/mem", O_RDWR);
			if (io->devicereg_ptr)
			{
				io->devicereg = (unsigned short *)mmap(0, 0x10000, PROT_READ | PROT_WRITE, MAP_SHARED, io->devicereg_ptr, 0xc0000000);
				if (io->devicereg == NULL)
				{
					close(io->devicereg_ptr);
					return(3);
				}
			}
			else
				return(3);
			if (stat("/dev/touchscreen/wm97xx", &sbuf) == 0)
			{
				io->clickevent_ptr = open("/dev/touchscreen/wm97xx", O_RDONLY | O_NOCTTY);
				if (io->clickevent_ptr >= 0)
					eciReadCalibration(io->clickcalib);
			}
			io->wnd = (void *)1;//Dummy
		#elif defined(DEVICE_ANDROID)
			// delegate to java
			return Call_EdgelibApp_edgeWindowCreate(io->jenv, io->jappobject, 
					(winstyle->fullscreen ? 1 : 0) |
					(io->sustainbacklight ? 2 : 0) |
					((winstyle->forceorientation & 3) << 2)
					);
		#else // X11
			Display *xdsp;
			Bool supportkbrep;
			xdsp = XOpenDisplay(":0.0");
			if (xdsp == 0)
				xdsp = XOpenDisplay(0);
			if (xdsp)
				io->wnd_arr[0] = (ecNativetype)xdsp;
			else
				return(3);
			long screennumber = DefaultScreen(xdsp);
			unsigned long black = BlackPixel(xdsp, screennumber);
			char caption[256];
			ecStrToUtf8(caption, winstyle->title);
			if (winstyle->fullscreen)
			{
				long xsize = winstyle->width, ysize = winstyle->height;
				#if defined(DEVICE_EMBEDDEDLINUX)
					xsize = DisplayWidth(xdsp, screennumber);
					ysize = DisplayHeight(xdsp, screennumber);
				#else
					xsize = winstyle->width;
					ysize = winstyle->height;
				#endif
				io->xwin = XCreateSimpleWindow(xdsp, DefaultRootWindow(xdsp), 0, 0, xsize, ysize, 0, black, black);
			}
			else
			{
				XSizeHints sizehints;
				io->xwin = XCreateSimpleWindow(xdsp, DefaultRootWindow(xdsp), 0, 0, winstyle->width, winstyle->height, 0, black, black);
				sizehints.flags = PMinSize | PMaxSize;
				sizehints.min_width = winstyle->width;
				sizehints.max_width = winstyle->width;
				sizehints.min_height = winstyle->height;
				sizehints.max_height = winstyle->height;
				XSetWMNormalHints(xdsp, io->xwin, &sizehints);
			}
			XSetWindowAttributes wattr;
			wattr.background_pixel = black;
			XChangeWindowAttributes(xdsp, io->xwin, CWBackPixel, &wattr);
			XWMHints wmhints;
			ecMemSet(&wmhints, 0, sizeof(wmhints));
			wmhints.flags = InputHint;
			wmhints.input = true;
			XSetWMHints(xdsp, io->xwin, &wmhints);
			io->wnd = io->wnd_arr;
			io->wnd[1] = (ecNativetype)&io->xwin;
			XkbSetDetectableAutoRepeat((Display *)io->wnd[0], True, &supportkbrep);
			XStoreName((Display *)io->wnd[0], *(Window *)io->wnd[1], caption);
			ecMemSet(io->xdblkeyboard, 0, 32);
			if (winstyle->fullscreen == 2)
			{
				unsigned long decors[5] = { 2, 0, 0, 0, 0 };
				XChangeProperty((Display *)io->wnd[0], *(Window *)io->wnd[1], XInternAtom((Display *)io->wnd[0], "_MOTIF_WM_HINTS", False), XInternAtom((Display *)io->wnd[0], "_MOTIF_WM_HINTS", False), 32, PropModeReplace, (const unsigned char *)decors, 5);
			}
			
			//To have some WMs recognise the window as an application
			XClassHint *xname = XAllocClassHint();
			xname->res_name = caption;
			xname->res_class = caption;
			XSetClassHint((Display *)io->wnd[0], *(Window *)io->wnd[1], xname);
			
			//Continue creating window
			XMapWindow((Display *)io->wnd[0], *(Window *)io->wnd[1]);
			XSelectInput((Display *)io->wnd[0], *(Window *)io->wnd[1], KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | ButtonMotionMask | EnterWindowMask | LeaveWindowMask | VisibilityChangeMask | FocusChangeMask | KeymapStateMask | ExposureMask | StructureNotifyMask | SubstructureNotifyMask);
			XEvent evt;
			do
			{
				XNextEvent((Display *)io->wnd[0], &evt);
			}while(evt.type != MapNotify);
			io->xwm_delete = XInternAtom((Display *)io->wnd[0], "WM_DELETE_WINDOW", False);
			XSetWMProtocols((Display *)io->wnd[0], *(Window *)io->wnd[1], &io->xwm_delete, 1);
			if (winstyle->fullscreen == 2)
			{
				//XSetWindowAttributes winatt;
				//winatt.override_redirect = 1;
				//XChangeWindowAttributes(io->xdsp, *(Window *)io->wnd[1], CWOverrideRedirect, &winatt); //these 3 lines is a hack?
				evt.xclient.type = ClientMessage;
				evt.xclient.serial = 0;
				evt.xclient.send_event = True;
				evt.xclient.message_type = XInternAtom((Display *)io->wnd[0], "_NET_WM_STATE", False);
				evt.xclient.window = *(Window *)io->wnd[1];
				evt.xclient.format = 32;
				evt.xclient.data.l[0] = 1;
				evt.xclient.data.l[1] = XInternAtom((Display *)io->wnd[0], "_NET_WM_STATE_FULLSCREEN", False);
				evt.xclient.data.l[2] = 0;
				evt.xclient.data.l[3] = 0;
				evt.xclient.data.l[4] = 0;
				XSendEvent((Display *)io->wnd[0], DefaultRootWindow((Display *)io->wnd[0]), False, SubstructureRedirectMask | SubstructureNotifyMask, &evt);
			}
		#endif
		ecEvent event;
		event.type = ECEV_CREATEWINDOW;
		ecOnEvent(io, &event);
		event.type = ECEV_DESTROYWINDOW;
		ecOnEvent(io, &event);
		#if defined(DEVICE_GP2X)
			if (io->clickevent_ptr >= 0)
				close(io->clickevent_ptr);
			munmap((void *)io->devicereg, 0x10000);
			close(io->devicereg_ptr);
		#endif
		return(0);
	#elif defined(DEVICE_MAC)
		int returnvalue = StartMacWindow(io->argc, io->argv, io);
		if (returnvalue)
			return(3);	//E_ERROR
		return(0);
	#elif defined(DEVICE_OPENKODE)
		EGLDisplay display = EGL_NO_DISPLAY;
		EGLint majorVersion, minorVersion;
		KDWindow *window;
		EGLNativeWindowType windowNative;
		EGLConfig config;
		EGLint numConfigs = 0;
		display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
		if (display == EGL_NO_DISPLAY)
			return(3); //E_ERROR
		io->wnd_arr[0] = display;
		if (!eglInitialize(display, &majorVersion, &minorVersion))
		{
			display = EGL_NO_DISPLAY;
			return(3); //E_ERROR
		}
		if (winstyle->hasopengl)
		{
			static const EGLint configAttribs[] =
			{
				EGL_SURFACE_TYPE,
				EGL_WINDOW_BIT,
				EGL_RENDERABLE_TYPE,
				EGL_OPENGL_ES_BIT,
				EGL_NONE
			};
			eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);
		}
		else
		{
			static const EGLint config_default[] =
			{
				EGL_SURFACE_TYPE,
				EGL_WINDOW_BIT | EGL_LOCK_SURFACE_BIT_KHR,
				EGL_MATCH_FORMAT_KHR,
				EGL_FORMAT_RGB_565_EXACT_KHR,
				EGL_RENDERABLE_TYPE,
				0,
				EGL_NONE
			};
			static const EGLint config_optimal[] =
			{
				EGL_SURFACE_TYPE,
				EGL_WINDOW_BIT | EGL_LOCK_SURFACE_BIT_KHR | EGL_OPTIMAL_FORMAT_BIT_KHR,
				EGL_RENDERABLE_TYPE,
				0,
				EGL_NONE
			};
			eglChooseConfig(display, config_optimal, &config, 1, &numConfigs);
			if (numConfigs == 0)
				eglChooseConfig(display, config_default, &config, 1, &numConfigs);
		}
		if (numConfigs == 0)
		{
			eglTerminate(display);
			return(3); //E_ERROR
		}
		io->wnd_arr[2] = config;
		window = kdCreateWindow(display, config, KD_NULL);
		if (window == KD_NULL)
		{
			eglTerminate(display);
			return(3); //E_ERROR
		}
		if (kdRealizeWindow(window, &windowNative))
		{
			eglTerminate(display);
			return(3); //E_ERROR
		}
		io->wnd_arr[1] = (ecNativetype)windowNative;
		io->wnd_arr[3] = (ecNativetype)window;
		io->wnd = io->wnd_arr;
		ecEvent event;
		event.type = ECEV_CREATEWINDOW;
		ecOnEvent(io, &event);
		event.type = ECEV_DESTROYWINDOW;
		ecOnEvent(io, &event);
		return(0);
	#else
		return(2);
	#endif
}

//Adjust window position, size and other properties
void ecInstAdjustWindow(ECOREOBJ inst, EC_WINDOWSTYLE *winstyle)
{
	#if defined(DEVICE_WIN32)
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		if (io->wnd)
		{
			long width = winstyle->width;
			long height = winstyle->height;
			if (io->curwinstyle.fullscreen == 0)
			{
				if (io->curwinstyle.winborder)
				{
					width += GetSystemMetrics(SM_CXDLGFRAME) * 2;
					height += GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION);
				}
				if (io->curwinstyle.menu)
					height += GetSystemMetrics(SM_CYMENU);
				#if defined(DEVICE_DESKTOP)
					if (io->curwinstyle.statusbar)
						height += GetSystemMetrics(SM_CYMENU) * 5 / 4;
				#endif
			}
			MoveWindow(io->wnd, GetSystemMetrics(SM_CXSCREEN) / 2 - winstyle->width / 2, GetSystemMetrics(SM_CYSCREEN) / 2 - height / 2, width, height, TRUE);
			#if defined(DEVICE_DESKTOP)
				unsigned long statheight;
				statheight = GetSystemMetrics(SM_CYMENU) * 5 / 4;
				if (io->statuswnd)
					MoveWindow(io->statuswnd, 0, winstyle->height, winstyle->width, statheight, TRUE);
				else
					io->statuswnd = CreateWindowEx(WS_EX_STATICEDGE, TEXT("STATIC"), TEXT(" Starting display..."), WS_VISIBLE | WS_CHILD, 0, winstyle->height, winstyle->width, statheight, io->wnd, NULL, io->instance, NULL);
				if (io->ibuttonwnd)
					MoveWindow(io->ibuttonwnd, winstyle->width - statheight, winstyle->height, statheight, statheight, TRUE);
				else
					io->ibuttonwnd = CreateWindow(TEXT("BUTTON"), TEXT("i"), WS_VISIBLE | WS_CHILD, winstyle->width - statheight, winstyle->height, statheight, statheight, io->wnd, NULL, io->instance, NULL);
				if (io->cbuttonwnd)
					MoveWindow(io->cbuttonwnd, winstyle->width - statheight * 2, winstyle->height, statheight, statheight, TRUE);
				else
					io->cbuttonwnd = CreateWindow(TEXT("BUTTON"), TEXT("c"), WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_PUSHLIKE, winstyle->width - statheight * 2, winstyle->height, statheight, statheight, io->wnd, NULL, io->instance, NULL);
			#endif
			io->curwinstyle.width = winstyle->width;
			io->curwinstyle.height = winstyle->height;
		}
	#endif
}

//Destroy the window
void ecInstDestroyWindow(ECOREOBJ inst)
{
	#if defined(DEVICE_WIN32)
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		if (io->wnd != NULL)
		{
			io->manualdestroy = true;
			DestroyWindow(io->wnd);
			io->wnd = NULL;
			#if defined(DEVICE_WINMOBILE)
				if (io->curwinstyle.fullscreen == 2)
					SHFullScreen(io->wnd, SHFS_SHOWSTARTICON | SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON);
				else if (io->curwinstyle.fullscreen == 1 && io->curwinstyle.menu == 0)
					SHFullScreen(io->wnd, SHFS_SHOWSIPBUTTON);
			#endif
		}
	#elif defined(DEVICE_X11)		
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		if (io->wnd)
		{
			XDestroyWindow((Display *)io->wnd[0], *(Window *)io->wnd[1]);
			XCloseDisplay((Display *)io->wnd[0]);
			io->wnd = NULL;
		}
	#elif defined(DEVICE_MAC)
		KillMacWindow((EC_INSTANCEOBJ *)inst);
	#elif defined(DEVICE_OPENKODE)
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		if (io->wnd)
		{
			if (io->wnd[0] != EGL_NO_DISPLAY)
				eglTerminate(io->wnd[0]);
			eglReleaseThread();
			io->wnd = NULL;
		}
	#endif
}

//Get window client rectangle
bool ecInstGetWindowRect(ECOREOBJ inst, long *rect, bool aligntoscreen)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	#if defined(DEVICE_WIN32)
		RECT rc;
		if (io->curwinstyle.fullscreen == 2)
		{
			#if defined(DEVICE_WINMOBILE)
				SetRect(&rc, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
			#else
				return(false);
			#endif
		}
		else
			GetWindowRect(io->wnd, &rc);
		#if defined(DEVICE_WINMOBILE)
			if (io->curwinstyle.fullscreen == 1 && io->curwinstyle.menu)
			{
				if (rc.bottom == GetSystemMetrics(SM_CYSCREEN))
					rc.bottom -= rc.top;
			}
			#if defined(DEVICE_SMARTPHONE)
				if (io->curwinstyle.menu == 0)
					rc.bottom = GetSystemMetrics(SM_CYSCREEN);
			#endif
			if (aligntoscreen)
			{
				if (io->curorientation == 270)
					SetRect(&rc, rc.top, GetSystemMetrics(SM_CXSCREEN) - rc.right, rc.bottom, GetSystemMetrics(SM_CXSCREEN) - rc.left);
				else if (io->curorientation == 180)
					SetRect(&rc, GetSystemMetrics(SM_CXSCREEN) - rc.right, GetSystemMetrics(SM_CYSCREEN) - rc.bottom, GetSystemMetrics(SM_CXSCREEN) - rc.left, GetSystemMetrics(SM_CYSCREEN) - rc.top);
				else if (io->curorientation == 90)
					SetRect(&rc, GetSystemMetrics(SM_CYSCREEN) - rc.bottom, rc.left, GetSystemMetrics(SM_CYSCREEN) - rc.top, rc.right);
			}
		#else
			if (GetWindowLong(io->wnd, GWL_STYLE) & WS_CAPTION)
			{
				rc.left += GetSystemMetrics(SM_CXFIXEDFRAME);
				rc.top += GetSystemMetrics(SM_CXDLGFRAME) + GetSystemMetrics(SM_CYCAPTION);
				rc.right -= GetSystemMetrics(SM_CXFIXEDFRAME);
				rc.bottom -= GetSystemMetrics(SM_CXFIXEDFRAME);
			}
			if (io->curwinstyle.menu)
				rc.top += GetSystemMetrics(SM_CYMENU);
		#endif
		rect[0] = rc.left; rect[1] = rc.top; rect[2] = rc.right; rect[3] = rc.bottom;
		return(true);
	#elif defined(DEVICE_SYMBIAN)
		if (io->curwinstyle.fullscreen)
		{
			rect[0] = 0;
			rect[1] = 0;
			rect[2] = CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth;
			rect[3] = CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iHeight;
		}
		else
		{
			RWindow *nativewin = (RWindow *)io->wnd;
			rect[0] = nativewin->Position().iX;
			rect[1] = nativewin->Position().iY;
			rect[2] = rect[0] + nativewin->Size().iWidth;
			rect[3] = rect[1] + nativewin->Size().iHeight;
		}
		if (aligntoscreen)
		{
			if (io->curorientation == 270)
				eciInstSetRect(rect, rect[1], CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth - rect[2], rect[3], CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth - rect[0]);
			else if (io->curorientation == 180)
				eciInstSetRect(rect, CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth - rect[2], CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iHeight - rect[3], CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iWidth - rect[0], CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iHeight - rect[1]);
			else if (io->curorientation == 90)
				eciInstSetRect(rect, CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iHeight - rect[3], rect[0], CEikonEnv::Static()->ScreenDevice()->SizeInPixels().iHeight - rect[1], rect[2]);
		}
		return(true);
	#elif defined(DEVICE_X11)
		XWindowAttributes windowattr;
		rect[0] = 0;
		rect[1] = 0;
		if (XGetWindowAttributes((Display *)io->wnd_arr[0], io->xwin, &windowattr) != 0)
		{
			rect[2] = windowattr.width;
			rect[3] = windowattr.height;
			return(true);
		}
	#elif defined(DEVICE_MAC)
		long xres, yres;
		MacGetNativeResolution(io->wnd, xres, yres);
		rect[0] = 0;
		rect[1] = 0;
		rect[2] = xres;
		rect[3] = yres;
		return(true);
	#elif defined(DEVICE_ANDROID)
		rect[0] = 0;
		rect[1] = 0;
		rect[2] = Call_EdgelibView_getWidth();
		rect[3] = Call_EdgelibView_getHeight();
		return(true);
	#elif defined(DEVICE_OPENKODE)
		KDint32 windowsize[2];
		kdGetWindowPropertyiv((KDWindow *)io->wnd_arr[3], KD_WINDOWPROPERTY_SIZE, windowsize);
		rect[0] = 0;
		rect[1] = 0;
		rect[2] = windowsize[0];
		rect[3] = windowsize[1];
		return(true);
	#endif
	return(false);
}

//Start the main loop
//Errcode: E_OK, E_ERROR
ecErr ecInstStartLoop(ECOREOBJ inst)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	ecEvent event;
	event.type = ECEV_IDLE;
	io->sustainloop = true;
	#if defined(DEVICE_WIN32)
		if (io->curwinstyle.corethread)
			eciStartWinMsgThread(io);
		while(io->sustainloop || io->curwinstyle.corethread)
		{
			if (!io->curwinstyle.corethread)
			{
				bool skipframe = false;
				if (io->sustainbacklight)
				{
					#if defined(DEVICE_WINCE)
						#if defined(DEVICE_WINMOBILE)
							SHIdleTimerReset();
						#endif
						SystemIdleTimerReset();
					#endif
				}
				#if defined(DEVICE_WINCE)
					DWORD syspowstate;
					WCHAR devbuf[32];
					if (GetSystemPowerState(devbuf, 32, &syspowstate) == ERROR_SUCCESS)
					{
						if (syspowstate & (POWER_STATE_OFF | POWER_STATE_SUSPEND | POWER_STATE_UNATTENDED))
							skipframe = true;
					}
				#else
					if (io->curwinstyle.outwndclickcursor)
						eciHandleClickMove(io, (HWND)ecInstGetWindow(io));
				#endif
				if (!skipframe)
				{
					event.type = ECEV_IDLE;
					ecOnEvent(io, &event);
					if (io->loopsleep)
					{
						Sleep(io->loopsleep);
						io->loopsleep = 0;
					}
				}
				else
				{
					event.type = ECEV_SKIPIDLE;
					ecOnEvent(io, &event);
					Sleep(100);
				}
			}
			MSG wmsg;
			while (PeekMessage(&wmsg, NULL, 0, 0, PM_REMOVE))
			{
				if (wmsg.message == WM_QUIT)
					break;
				event.type = ECEV_PRENATIVE;
				event.param1 = (ecNativetype)&wmsg;
				ecOnEvent(io, &event);
				TranslateMessage(&wmsg);
				DispatchMessage(&wmsg);
			}
			if (!io->curwinstyle.corethread)
			{
				if (!io->sustainloop && io->wnd)
				{
					io->manualdestroy = true;
					DestroyWindow(io->wnd);
				}
			}
			else
				Sleep(10);
		}
		if (io->curwinstyle.corethread)
			eciKillWinMsgThread(io);
		event.type = ECEV_ENDMAINLOOP;
		ecOnEvent(io, &event);
	#elif defined(DEVICE_SYMBIAN)
		CIdle *itmr;
		io->idletimer = CIdle::New(CActive::EPriorityIdle);
		itmr = (CIdle *)io->idletimer;
		if (itmr)
		{
			#if (DEVICE_SYMBIAN >= 900) && defined(DEVICE_S60)
				io->symtilt->StartMonitoring();
			#endif
			itmr->Start(TCallBack(ecInstCallIdleTimer, inst));
			io->runidletimer = true;
		}
		else
			return(1);
	#elif defined(DEVICE_ANDROID)
		io->acceptevents = true;
		Call_EdgelibApp_suspendloop(io->jenv, io->jappobject, false);
	#elif defined(DEVICE_LINUX)
		#if defined(DEVICE_GP2X)
			unsigned long keyholdstate = 0xFFFFFFFF;
			unsigned long eventbuf_start = 0, eventbuf_size = 0;
			short prev_pressure = 0, prev_clickx = 0, prev_clicky = 0;
		#endif
		while(io->sustainloop)
		{
			#if defined(DEVICE_GP2X)
				unsigned char eventbuf[sizeof(TS_EVENT) * 16];
				bool eventreading = true;
				unsigned char keyctr, eventctr;
				unsigned long s_pos, prevstate = keyholdstate, newstate = (io->devicereg[0x1198 >> 1] & 0x00FF);
				if (newstate == 0xFD) newstate = 0xFA;
				if (newstate == 0xF7) newstate = 0xEB;
				if (newstate == 0xDF) newstate = 0xAF;
				if (newstate == 0x7F) newstate = 0xBE;
				newstate = ~((io->devicereg[0x1184 >> 1] & 0xFF00) | newstate | (io->devicereg[0x1186 >> 1] << 16));
				s_pos = 1;
				for (keyctr = 0; keyctr < 32; keyctr++)
				{
					if ((newstate & s_pos) && !(prevstate & s_pos))
					{
						unsigned long keynr = keyctr + 1;
						event.type = ECEV_KEYDOWN;
						event.param1 = keynr;
						ecOnEvent(inst, &event);
					}
					else if (!(newstate & s_pos) && (prevstate & s_pos))
					{
						unsigned long keynr = keyctr + 1;
						event.type = ECEV_KEYUP;
						event.param1 = keynr;
						ecOnEvent(inst, &event);
					}
					s_pos <<= 1;
				}
				keyholdstate = newstate;
				if (io->clickevent_ptr >= 0)
				{
					unsigned long readcount = read(io->clickevent_ptr, (char *)&eventbuf[eventbuf_start], sizeof(TS_EVENT) * 16 - eventbuf_start);
					eventbuf_size += readcount;
					eventbuf_start += readcount;
					while(eventbuf_size >= sizeof(TS_EVENT))
					{
						TS_EVENT ievent;
						short newclickx, newclicky;
						ecMemCpy(&ievent, eventbuf, sizeof(TS_EVENT));
						ecMemCpy(eventbuf, &eventbuf[sizeof(TS_EVENT)], eventbuf_size - sizeof(TS_EVENT));
						eventbuf_size -= sizeof(TS_EVENT);
						eventbuf_start -= sizeof(TS_EVENT);
						if (io->clickcalib[6] == 65536)
						{
							newclickx = (io->clickcalib[2] + io->clickcalib[0] * (int)ievent.x) >> 16;
							newclicky = (io->clickcalib[5] + io->clickcalib[4] * (int)ievent.y) >> 16;
						}
						else
						{
							newclickx = (io->clickcalib[2] + io->clickcalib[0] * (int)ievent.x) / io->clickcalib[6];
							newclicky = (io->clickcalib[5] + io->clickcalib[4] * (int)ievent.y) / io->clickcalib[6];
						}
						if ((newclickx < 0) || (newclickx > 320) || (newclicky < 0) || (newclicky > 240))
						{
							newclickx = prev_clickx;
							newclicky = prev_clicky;
						}
						if (newclickx != prev_clickx || newclicky != prev_clicky)
						{
							event.type = ECEV_CLICKMOVE;
							event.param1 = 0;
							event.param2 = newclickx;
							event.param3 = newclicky;
							ecOnEvent(io, &event);
						}
						prev_clickx = newclickx;
						prev_clicky = newclicky;
						if ((ievent.pressure < 0 && prev_pressure >= 0) || (prev_pressure < 0 && ievent.pressure >= 0))
						{
							if (ievent.pressure >= 0)
								event.type = ECEV_CLICKUP;
							else
								event.type = ECEV_CLICKDOWN;
							event.param1 = 0;
							event.param2 = prev_clickx;
							event.param3 = prev_clicky;
							ecOnEvent(io, &event);
						}
						prev_pressure = ievent.pressure;
					}
				}
			#else
				if (io->sustainbacklight)
				{
					#if defined(DEVICE_MAEMO) && !defined(DEVICE_MAEMOSIM)
						io->backlighttimer++;
						if (io->backlighttimer >= 500)
						{
							GError *error = NULL;
							DBusGConnection *connection = dbus_g_bus_get(DBUS_BUS_SYSTEM, &error);
							if (connection)
							{
								DBusGProxy *proxy;
								proxy = dbus_g_proxy_new_for_name(connection, "com.nokia.mce", "/com/nokia/mce/request", "com.nokia.mce.request");//Name, path, interface
								if (dbus_g_proxy_call(proxy, "req_display_state_on", &error, G_TYPE_INVALID, G_TYPE_INVALID) == 0)
									g_clear_error(&error);
								g_object_unref(proxy);
							}
							else
								g_clear_error(&error);
							io->backlighttimer = 0;
						}
					#endif
				}
				ecInstNativeCallback(io, 0, 0, 0);
			#endif
			event.type = ECEV_IDLE;
			ecOnEvent(io, &event);
			if (io->loopsleep)
			{
				usleep(io->loopsleep * 1000);
				io->loopsleep = 0;
			}
		}
		event.type = ECEV_ENDMAINLOOP;
		ecOnEvent(io, &event);
	#elif defined(DEVICE_MAC)
		StartMacLoop(io);
	#elif defined(DEVICE_OPENKODE)
		io->loopsleep = 10;
		while(io->sustainloop)
		{
			KDEvent *kdevent;
			KDust curtick = kdGetTimeUST();
			bool towait = true;
			if (io->orglandscape == 0)
			{
				KDint32 windowsize[2];
				if (kdGetWindowPropertyiv((KDWindow *)io->wnd_arr[3], KD_WINDOWPROPERTY_SIZE, windowsize) == 0)
				{
					if (windowsize[0] >= windowsize[1])
						io->orglandscape = 1;
					else
						io->orglandscape = -1;
				}
			}
			while (1)
			{
				if (towait)
				{
					kdevent = (KDEvent *)kdWaitEvent((KDust)(io->loopsleep) * 1000000);
					towait = false;
				}
				else
					kdevent = (KDEvent *)kdWaitEvent(0);
				if (kdevent != KD_NULL)
				{
					if (kdevent->type == KD_EVENT_QUIT)
						io->sustainloop = false;
					else if (ecInstNativeCallback(io, (ecParam)kdevent, 0, 0) == 0)
						kdDefaultEvent(kdevent);
				}
				else
				{
					unsigned long tickdiff = (unsigned long)((kdGetTimeUST() - curtick) / 1000000);
					curtick = kdGetTimeUST();
					if (tickdiff < io->loopsleep)
					{
						io->loopsleep -= tickdiff;
						towait = true;
					}
					else
						break;
				}
			}
			io->loopsleep = 0;
			ecEvent event;
			event.type = ECEV_IDLE;
			ecOnEvent(io, &event);
		}
		event.type = ECEV_ENDMAINLOOP;
		ecOnEvent(io, &event);
	#else
		while(io->sustainloop)
		{
			ecOnEvent(io, &event);
			if (io->loopsleep)
			{
				Sleep(io->loopsleep);
				io->loopsleep = 0;
			}
		}
		event.type = ECEV_ENDMAINLOOP;
		ecOnEvent(io, &event);
	#endif
	return(0);
}

//Return if specified event comes from a button
//Return type: 0 = none, 1 = push info button, 2 = console on, 3 = console off
unsigned char ecInstEventFromButton(ECOREOBJ inst, ecEvent *event, bool setvalue)
{
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		if (setvalue)
		{
			if (io->cbuttonwnd)
			{
				if (event->param1 == 0)
					SendMessage(io->cbuttonwnd, BM_SETCHECK, BST_UNCHECKED, 0);
				else
					SendMessage(io->cbuttonwnd, BM_SETCHECK, BST_CHECKED, 0);
			}
		}
		else
		{
			UINT wmsg = (UINT)event->param1;
			if (wmsg == WM_COMMAND)
			{
				WPARAM wparam = (WPARAM)event->param2;
				LPARAM lparam = (LPARAM)event->param3;
				if (lparam == (LPARAM)io->ibuttonwnd)
					return(1);
				if (lparam == (LPARAM)io->cbuttonwnd)
				{
					LRESULT bstate = SendMessage(io->cbuttonwnd, BM_GETCHECK, 0, 0);
					if (bstate == BST_CHECKED)
						return(2);
					return(3);
				}
			}
		}
	#endif
	return(0);
}

//Set message for the status bar
void ecInstStatusMsg(ECOREOBJ inst, ecUnistring msg)
{
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		if (io->statuswnd)
			SetWindowTextW(io->statuswnd, (LPCWSTR)msg);
	#endif
}

//Minimize application
void ecInstMinimize(ECOREOBJ inst)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
	#if defined(DEVICE_WIN32)
		ShowWindow(io->wnd, SW_MINIMIZE);
	#elif defined(DEVICE_SYMBIAN)
		if (io->frameui)
		{
			CSymCoreUi *appui = (CSymCoreUi *)io->frameui;
			if (appui->iAppView != NULL)
			{
				appui->iAppView->SetFocus(EFalse);
				#if !defined(DEVICE_UIQ) || (DEVICE_SYMBIAN < 900)
					appui->iAppView->SetExtent(TPoint(0, 0), TSize(0, 0));
				#endif
				appui->RequestMinimize();
			}
		}
	#endif
}

///////////////////////////////////////////////////////////////////
// Platform dependent callbacks
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_WIN32)

BOOL CALLBACK ecInstWinProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	int wresult;
	EC_INSTANCEOBJ *io;
	UINT checkmsg;
	#if defined(DEVICE_DESKTOP)
		checkmsg = WM_NCCREATE;
	#else
		checkmsg = WM_CREATE;
	#endif
	if (wmsg == checkmsg)
	{
		io = (EC_INSTANCEOBJ *)((LPCREATESTRUCT)lparam)->lpCreateParams;
		io->wnd = hwnd;
		#if defined(DEVICE_DESKTOP)
			SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)io);
		#else
			SetWindowLong(hwnd, GWL_USERDATA, (LONG)io);
		#endif
	}
	else
	{
		#if defined(DEVICE_DESKTOP)
			io = (EC_INSTANCEOBJ *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		#else
			io = (EC_INSTANCEOBJ *)GetWindowLong(hwnd, GWL_USERDATA);
		#endif
	}
	wresult = 0;
	if (io)
		wresult = ecInstNativeCallback(io, (ecParam)wmsg, (ecParam)wparam, (ecParam)lparam);
	if (wresult == 0)
		return(DefWindowProc(hwnd, wmsg, wparam, lparam));
	return(TRUE);
}

#if defined(DEVICE_DESKTOP)

//Subclassed dialog window procedure
LRESULT CALLBACK ecInstDlgSubProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	if (wmsg == WM_GETDLGCODE)
		return(DLGC_WANTALLKEYS);
	WNDPROC origwinproc;
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	if (io != NULL)
	{
		origwinproc = (WNDPROC)ecInstGetGlobal(io, 8);
 		return(CallWindowProc(origwinproc, hwnd, wmsg, wparam, lparam));
	}
	return(FALSE);
}

//Dialog box window procedure
BOOL CALLBACK ecInstDlgProc(HWND hwnd, UINT wmsg, WPARAM wparam, LPARAM lparam)
{
	int wresult;
	EC_INSTANCEOBJ *io;
	WNDPROC origwinproc = NULL;
	if (wmsg == WM_INITDIALOG)
	{
		io = (EC_INSTANCEOBJ *)lparam;
		io->wnd = hwnd;
		SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG_PTR)io);
		origwinproc = (WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)ecInstDlgSubProc);
		ecInstSetGlobal(io, (ecParam)origwinproc, 8);
	}
	else
	{
		io = (EC_INSTANCEOBJ *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (io != NULL)
			origwinproc = (WNDPROC)ecInstGetGlobal(io, 8);
		if (wmsg == WM_DESTROY)
			SetWindowLong(hwnd, GWL_WNDPROC, (LONG)origwinproc);
	}
	wresult = 0;
	if (io)
		wresult = ecInstNativeCallback(io, (ecParam)wmsg, (ecParam)wparam, (ecParam)lparam);
	if (wresult == 0)
		return(FALSE);
	return(TRUE);
}

#endif

#elif defined(DEVICE_SYMBIAN)

//Symbian: error message on exit
void eciInstSymbianExitMsg(ecUnistring msg)
{
	if (ecStrLen(msg) > 17)
	{
		unsigned char onpos = 17;
		ecUnichar exstring[64];
		while(msg[onpos] != ' ')
		{
			if (onpos == 0)
				break;
			onpos--;
		}
		if (onpos == 0)
		{
			ecStrCpy(exstring, &msg[17]);
			msg[17] = 0;
		}
		else
		{
			ecStrCpy(exstring, &msg[onpos + 1]);
			msg[onpos] = 0;
		}
		TBuf16<32> msg1 = msg;
		TBuf16<64> msg2 = exstring;
		CEikonEnv::Static()->AlertWin(msg1, msg2);
	}
	else
	{
		TBuf16<32> msgb = msg;
		CEikonEnv::Static()->AlertWin(msgb);
	}
}

//Symbian: timer callback function calls the main loop
TInt ecInstCallIdleTimer(TAny *aAny)
{
	EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)aAny;
	ecEvent event;
	if (!io->sustainloop)
	{
		CSymCoreUi *appui = (CSymCoreUi *)io->frameui;
		event.type = ECEV_DESTROYWINDOW;
		ecOnEvent(io, &event);
		if (io->exiterrormsg[0])
		{
			eciInstSymbianExitMsg(io->exiterrormsg);
			User::After(1000);
		}
		appui->RequestExit();
		return(0);
	}
	if (io->sustainbacklight)
		User::ResetInactivityTime();
	event.type = ECEV_IDLE;
	ecOnEvent(io, &event);
	if (io->loopsleep)
	{
		User::After((io->loopsleep) * 1000);
		io->loopsleep = 0;
	}
	return(1);
}

#endif

///////////////////////////////////////////////////////////////////
// Platform dependent entry point (Win32)
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_WIN32)

//WinMain
#ifdef _tWinMain
	int APIENTRY _tWinMain(HINSTANCE w_inst, HINSTANCE w_previnst, LPTSTR cmdline, int cmdshow)
#else
	int APIENTRY WinMain(HINSTANCE w_inst, HINSTANCE w_previnst, LPSTR cmdline, int cmdshow)
#endif
{
	ECOREOBJ inst;
	ecErr errcreate;
	inst = ecInstCreate(errcreate, (ecParam)w_inst);
	if (inst)
	{
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		ecEvent event;
		ecUnichar tcmdline[EC_MAXCMDLINE];
		ecStrCpy(tcmdline, (ECTCHAR)cmdline);
		ecInstSetCommandline(inst, tcmdline);
		event.type = ECEV_CREATEINSTANCE;
		event.param1 = (ecParam)io->instance;
		ecOnEvent(inst, &event);
		event.type = ECEV_DESTROYINSTANCE;
		ecOnEvent(inst, &event);
		if (io->exiterrormsg[0])
			MessageBoxW(NULL, (LPCWSTR)io->exiterrormsg, L"Application error", MB_OK | MB_ICONERROR);
		errcreate = io->returnvalue;
		ecInstDelete(inst);
	}
	else
		MessageBoxW(NULL, L"Instance create error", L"Can't run application", MB_OK | MB_ICONERROR);
	return(errcreate);
}

#endif

///////////////////////////////////////////////////////////////////
// Platform dependent entry point (Symbian)
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_SYMBIAN)

//Start the core application
CSymCoreApp* StartCoreApp(void)
{
	ECOREOBJ inst;
	ecErr errcreate;
	inst = ecInstCreate(errcreate, 0);
	if (inst)
	{
		#if (DEVICE_SYMBIAN >= 900)
			long cmdlen = User::CommandLineLength();
			if (cmdlen > 0 && cmdlen < EC_MAXCMDLINE)
			{
				TBuf16<EC_MAXCMDLINE> linedes;
				User::CommandLine(linedes);
				ecInstSetCommandline(inst, (const ecUnistring)linedes.PtrZ());
			}
		#else
			RProcess me;
			long cmdlen = me.CommandLineLength();
			if (cmdlen > 0 && cmdlen < EC_MAXCMDLINE)
			{
				TBuf16<EC_MAXCMDLINE> linedes;
				me.CommandLine(linedes);
				ecInstSetCommandline(inst, (const ecUnistring)linedes.PtrZ());
			}
		#endif
		return(new CSymCoreApp(inst));
	}
	return(NULL);
}

#ifdef DEVICE_SYMBIANEXE

#include <eikstart.h>

//Symbian EXE entry points
LOCAL_C CApaApplication* NewApplication(){ return(StartCoreApp()); }
GLDEF_C TInt E32Main()
{
	RHeap *bigheap = UserHeap::ChunkHeap(0, 0x100000, 0x1000000);
	User::SwitchHeap(bigheap);
	return(EikStart::RunApplication(NewApplication));
}

#else

//Symbian App/Dll entry points
GLDEF_C TInt E32Dll(TDllReason){ return(KErrNone); }
EXPORT_C CApaApplication* NewApplication(){ return(StartCoreApp()); }

#endif

#endif

///////////////////////////////////////////////////////////////////
// Platform dependent entry point (Linux)
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_LINUX) && !defined(DEVICE_ANDROID)

int main(int argc, char *argv[])
{
	ECOREOBJ inst;
	ecErr errcreate;
	inst = ecInstCreate(errcreate, 0);
	if (inst)
	{
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		if (argc > 1)
		{
			int cmdctr;
			ecUnichar cmdline[EC_MAXCMDLINE];
			for (cmdctr = 1; cmdctr < argc; cmdctr++)
			{
				if (cmdctr > 1)
				{
					ecStrCat(cmdline, " ");
					ecStrCat(cmdline, argv[cmdctr]);
				}
				else
					ecStrCpy(cmdline, argv[cmdctr]);
			}
			ecInstSetCommandline(inst, cmdline);
		}
		ecEvent event;
		event.type = ECEV_CREATEINSTANCE;
		event.param1 = 0;
		ecOnEvent(inst, &event);
		event.type = ECEV_DESTROYINSTANCE;
		ecOnEvent(inst, &event);
		errcreate = io->returnvalue;
		#if !defined(DEVICE_GP2X)
			ecInstDestroyWindow(io);
		#endif
		ecInstDelete(inst);
	}
	#if defined(DEVICE_GP2X)
		chdir("/usr/gp2x");
		execl("gp2xmenu", "gp2xmenu", NULL);
	#endif
	return(errcreate);
}

#if defined(DEVICE_ALP)
	int alp_main(int argc, char *argv[])
	{
		main(argc, argv);
	}
#endif

#endif

///////////////////////////////////////////////////////////////////
// Platform dependent entry point (Mac OS X)
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_MAC)
extern ECOREOBJ userdata;

int main(int argc, char *argv[])
{
	int result;
	ECOREOBJ inst;
	ecErr errcreate;
	inst = ecInstCreate(errcreate, 0);
	if (inst)
	{
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		if (argc > 1)
		{
			int cmdctr;
			ecUnichar cmdline[EC_MAXCMDLINE];
			for (cmdctr = 1; cmdctr < argc; cmdctr++)
			{
				if (cmdctr > 1)
				{
					ecStrCat(cmdline, " ");
					ecStrCat(cmdline, argv[cmdctr]);
				}
				else
					ecStrCpy(cmdline, argv[cmdctr]);
			}
			ecInstSetCommandline(inst, cmdline);
		}
		ecEvent event;
		io->argc = argc;
		io->argv = argv;
		event.type = ECEV_CREATEINSTANCE;
		event.param1 = 0;
		ecOnEvent(inst, &event);
	}
	return(result);
}

#endif

///////////////////////////////////////////////////////////////////
// Platform dependent entry point (OpenKode)
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_OPENKODE)

//int main(int argc, char *argv[])
KDint kdMain(KDint argc, const KDchar *const *argv)
{
	ECOREOBJ inst;
	ecErr errcreate;
	inst = ecInstCreate(errcreate, 0);
	if (inst)
	{
		EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
		if (argc > 1)
		{
			int cmdctr;
			ecUnichar cmdline[EC_MAXCMDLINE];
			for (cmdctr = 1; cmdctr < argc; cmdctr++)
			{
				if (cmdctr > 1)
				{
					ecStrCat(cmdline, " ");
					ecStrCat(cmdline, argv[cmdctr]);
				}
				else
					ecStrCpy(cmdline, argv[cmdctr]);
			}
			ecInstSetCommandline(inst, cmdline);
		}
		ecEvent event;
		event.type = ECEV_CREATEINSTANCE;
		event.param1 = 0;
		ecOnEvent(inst, &event);
		event.type = ECEV_DESTROYINSTANCE;
		ecOnEvent(inst, &event);
		errcreate = io->returnvalue;
		ecInstDestroyWindow(io);
		ecInstDelete(inst);
	}
	return(errcreate);
}

#endif
