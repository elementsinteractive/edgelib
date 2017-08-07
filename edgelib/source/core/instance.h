// instance.h
// Edgecore: instance
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#ifndef ECORE_INSTANCE_H
#define ECORE_INSTANCE_H

// DEVICE_* macros
#include "edgedevice.h"

//Limits
#define EC_MAXCMDLINE   4096
#define EC_MAXINSTGLOBAL  16 //0-7 for underlying framework, others for itself

typedef struct
{
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_WINCE)
			#if defined(DEVICE_WINMOBILE)
				HMODULE dll_gapi;
				FUNC_GXOPENINPUT func_gxopeninput;
				FUNC_GXCLOSEINPUT func_gxcloseinput;
				SHACTIVATEINFO activateinfo;
				HWND menuwnd;
				unsigned char keyclick;
				bool lockedinput;
				bool gapiok;
			#endif
			HANDLE hreqbacklight;
			bool failpowerrequirement;
		#elif defined(DEVICE_DESKTOP)
			HWND statuswnd;
			HWND cbuttonwnd;
			HWND ibuttonwnd;
			bool clickstate_capt[3];
			long clickpos_simmove[2];
		#endif
		HANDLE htimethread;
		HANDLE htimethreadexit;
		HANDLE htimethreadbreak;
		HINSTANCE instance;
		HWND wnd;
		bool manualdestroy;
	#elif defined(DEVICE_SYMBIAN)
		#if (DEVICE_SYMBIAN >= 900) && defined(DEVICE_S60)
			CSymSensorTilt *symtilt;
		#endif
		bool runidletimer;
		void *idletimer;
		void *frameui;
		RWindow *wnd;
	#elif defined(DEVICE_LINUX)
		#if defined(DEVICE_GP2X)
			int clickcalib[7];
			int devicereg_ptr;
			int clickevent_ptr;
			volatile unsigned short *devicereg;
			void *wnd;
		#elif defined(DEVICE_ANDROID)
			JNIEnv * jenv;
			jobject jappobject;
			jobject jviewobject;
			jobject jassetmanager;
			jobject jutilobject;
			void *wnd;
			bool acceptevents;
			char * devname;
			char * devmodel;
			char * rwpath;
			char * ropath;
			int api;
		#else
			ecNativetype wnd_arr[3];
			Window xwin;
			ecNativetype *wnd;
			Atom xwm_delete;
			unsigned char xdblkeyboard[32];
			unsigned long backlighttimer;
			bool suspended;
		#endif
	#elif defined(DEVICE_MAC)
		#if defined(DEVICE_DESKTOP)
			void *app;
			int screenx;
			int screeny;
		#endif
		void *wnd;
		void *view;
		int argc;
		char **argv;
	#elif defined(DEVICE_OPENKODE)
		ecNativetype wnd_arr[4];
		ecNativetype *wnd;
		int orglandscape;
		bool ispaused;
		bool isfocusout;
	#else
		void *wnd;
	#endif
	EC_WINDOWSTYLE curwinstyle;
	ecUnichar cmdline[EC_MAXCMDLINE];
	ecUnichar exiterrormsg[128];
	ecParam instglobal[EC_MAXINSTGLOBAL];
	unsigned long curorientation;
	unsigned long appid;
	unsigned long loopsleep;
	int returnvalue;
	bool sustainloop;
	bool sustainbacklight;
}EC_INSTANCEOBJ;

//Event callback
bool ecOnEvent(ECOREOBJ instance, ecEvent *event);

#endif
