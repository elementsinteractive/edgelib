// edgeframe.cpp
// Edgelib: provides framework class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Include version information
#include "extra/verinfo.h"

//Include device definitions and core API
#include "edgedevice.h"
#include "core/edgecore.h"

//Include device SDK files
#if defined(DEVICE_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#if defined(DEVICE_WINMOBILE)
		#include <sipapi.h>
	#endif
	#if (DEVICE_WINCE >= 400)
		#include <pm.h>
	#endif
#elif defined(DEVICE_SYMBIAN)
	#if defined(DEVICE_S60) && (DEVICE_SYMBIAN >= 900)
		#include <aknutils.h>
	#endif
#endif

//Include EDGELIB API
#define EDGE_INCLUDED
#include "edgedef.h"
#include "eloader.h"
#include "estdc.h"
#include "emath.h"
#include "ememory.h"
#include "efile.h"
#include "eini.h"
#include "evtimer.h"
#include "edisplay.h"
#include "esound.h"
#include "einput.h"
#include "ereg.h"
#include "econnect.h"
#include "edevice.h"
#include "econsole.h"
#include "edgeframe.h"
#include "ecamera.h"

//Definition fixes
#if !defined(SPI_SIPMOVE)
	#define SPI_SIPMOVE 250
#endif

//Recording definitions
#define ERECORDINFOSIZE     (4 * sizeof(unsigned long))
#define EREC_TIMEDELTA      0
#define EREC_TIMEDELTA16    1
#define EREC_KEYUP          2
#define EREC_KEYDOWN        3
#define EREC_CLICKUP        4
#define EREC_CLICKDOWN      5
#define EREC_CLICKDOUBLE    6
#define EREC_CLICKMOVE      7
#define EREC_WHEEL          8

//Prototypes
bool RegisterDisplayLoaders(ClassEDisplay *display);
unsigned long EdgeGetUIDNr(void);
bool EdgeGL_Use(void);

///////////////////////////////////////////////////////////////////
// ClassEdge: protected
///////////////////////////////////////////////////////////////////

ERESULT ClassEdge::OnFrameworkConfig(EFRAMEWORKCONFIG *config)      { return(E_OK); }
ERESULT ClassEdge::OnDisplayConfig(EDISPLAYCONFIG *config)          { return(E_OK); }
ERESULT ClassEdge::OnInit(ENATIVETYPE instance)                     { return(E_OK); }
ERESULT ClassEdge::OnMinimize(void)                                 { return(E_OK); }
ERESULT ClassEdge::OnRestore(void)                                  { return(E_OK); }
ERESULT ClassEdge::OnPluginLoad(ClassEDisplay *display)             { return(E_OK); }
ERESULT ClassEdge::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)  { return(E_OK); }
ERESULT ClassEdge::OnContextLost(ClassEDisplay *display)            { return(E_OK); }
ERESULT ClassEdge::OnNextFrame(ClassEDisplay *display, unsigned long timedelta) { return(E_OK); }
void ClassEdge::OnExit(ENATIVETYPE instance)                        { }
void ClassEdge::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist) { }
void ClassEdge::OnButtonUp(unsigned long bnr, EBUTTONLIST *blist)   { }
void ClassEdge::OnWheel(long direction)                             { }
void ClassEdge::OnStylusUp(POINT pnt)                               { }
void ClassEdge::OnStylusDown(POINT pnt)                             { }
void ClassEdge::OnStylusDouble(POINT pnt)                           { }
void ClassEdge::OnStylusMove(POINT pnt)                             { }
void ClassEdge::OnDeviceTilt(long x, long y, long z)                { }
bool ClassEdge::OnNativeEvent(ENATIVETYPE winhandle, ENATIVETYPE msg, ENATIVETYPE param1, ENATIVETYPE param2){ return(false); }
bool ClassEdge::OnPreNativeEvent(ENATIVETYPE param)                 { return(false); }
void ClassEdge::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer){ }
void ClassEdge::OnDisplayChange(ClassEDisplay *display)             { }
void ClassEdge::OnCameraChange(ClassECamera *, ERESULT)			    { }
void ClassEdge::OnCameraFrame(ClassECamera *, const E_CAMERAFRAME *){ }

///////////////////////////////////////////////////////////////////
// Win32 functions for other platforms                           //
///////////////////////////////////////////////////////////////////

#if !defined(DEVICE_WIN32)

//Quickly define a rectangle
void SetRect(RECT *lprc, int xLeft, int yTop, int xRight, int yBottom)
{
	lprc->left   = xLeft;
	lprc->top    = yTop;
	lprc->right  = xRight;
	lprc->bottom = yBottom;
}

#endif

///////////////////////////////////////////////////////////////////
// Extra RECT/POINT functions                                    //
///////////////////////////////////////////////////////////////////

//Check if 2 rectangles intersect
bool RectIntersect(RECT *rc1, RECT *rc2, RECT *result)
{
	if (rc1 == NULL || rc2 == NULL)
		return(false);
	if (rc1->right <= rc1->left || rc1->bottom <= rc1->top)
		return(false);
	if (rc2->right <= rc2->left || rc2->bottom <= rc2->top)
		return(false);
	if (rc1->right <= rc2->left || rc1->left >= rc2->right)
		return(false);
	if (rc1->bottom <= rc2->top || rc1->top >= rc2->bottom)
		return(false);
	if (result != NULL)
	{
		ClassEMemory::Copy(result, rc1, sizeof(RECT));
		if (rc2->left > result->left)
			result->left = rc2->left;
		if (rc2->top > result->top)
			result->top = rc2->top;
		if (rc2->right < result->right)
			result->right = rc2->right;
		if (rc2->bottom < result->bottom)
			result->bottom = rc2->bottom;
	}
	return(true);
}

//Check if a point is inside the rectangle
bool InRect(RECT *rc, POINT *pnt)
{
	if (rc == NULL || pnt == NULL)
		return(false);
	if (pnt->x >= rc->left && pnt->x < rc->right)
		if (pnt->y >= rc->top && pnt->y < rc->bottom)
			return(true);
	return(false);
}

///////////////////////////////////////////////////////////////////
// ClassEdge: pure functions                                     //
///////////////////////////////////////////////////////////////////

//Add an event to the record structure
void Edge_AddRecordEvent(unsigned char *eventdata, bool &recording, unsigned char type, long prop1 = 0, long prop2 = 0, long prop3 = 0)
{
	unsigned long *evr_hdr = (unsigned long *)eventdata;
	if (type == EREC_TIMEDELTA)
	{
		if (prop1 >= 256)
			type = EREC_TIMEDELTA16;
	}
	if (type == EREC_TIMEDELTA)
	{
		if (evr_hdr[0] + evr_hdr[2] + 2 <= evr_hdr[1])
		{
			eventdata[evr_hdr[0] + evr_hdr[2]] = type;
			eventdata[evr_hdr[0] + evr_hdr[2] + 1] = (unsigned char)prop1;
			evr_hdr[2] += 2;
		}
		else
			recording = false;
	}
	else if (type == EREC_TIMEDELTA16 || type == EREC_KEYUP || type == EREC_KEYDOWN)
	{
		if (prop1 > 0xFFFF)
			prop1 = 0xFFFF;
		if (evr_hdr[0] + evr_hdr[2] + 3 <= evr_hdr[1])
		{
			eventdata[evr_hdr[0] + evr_hdr[2]] = type;
			eventdata[evr_hdr[0] + evr_hdr[2] + 1] = (unsigned char)(prop1 >> 8);
			eventdata[evr_hdr[0] + evr_hdr[2] + 2] = (unsigned char)(prop1 & 255);
			evr_hdr[2] += 3;
		}
		else
			recording = false;
	}
	else if (type == EREC_CLICKUP || type == EREC_CLICKDOWN || type == EREC_CLICKDOUBLE)
	{
		prop1 += 32768;
		prop2 += 32768;
		if (prop1 < 0)
			prop1 = 0;
		else if (prop1 > 0xFFFF)
			prop1 = 0xFFFF;
		if (prop2 < 0)
			prop2 = 0;
		else if (prop2 > 0xFFFF)
			prop2 = 0xFFFF;
		if (evr_hdr[0] + evr_hdr[2] + 6 <= evr_hdr[1])
		{
			eventdata[evr_hdr[0] + evr_hdr[2]] = type;
			eventdata[evr_hdr[0] + evr_hdr[2] + 1] = (unsigned char)(prop1 >> 8);
			eventdata[evr_hdr[0] + evr_hdr[2] + 2] = (unsigned char)(prop1 & 255);
			eventdata[evr_hdr[0] + evr_hdr[2] + 3] = (unsigned char)(prop2 >> 8);
			eventdata[evr_hdr[0] + evr_hdr[2] + 4] = (unsigned char)(prop2 & 255);
			eventdata[evr_hdr[0] + evr_hdr[2] + 5] = (unsigned char)(prop3);
			evr_hdr[2] += 6;
		}
		else
			recording = false;
	}
	else if (type == EREC_CLICKMOVE)
	{
		prop1 += 32768;
		prop2 += 32768;
		if (prop1 < 0)
			prop1 = 0;
		else if (prop1 > 0xFFFF)
			prop1 = 0xFFFF;
		if (prop2 < 0)
			prop2 = 0;
		else if (prop2 > 0xFFFF)
			prop2 = 0xFFFF;
		if (evr_hdr[0] + evr_hdr[2] + 5 <= evr_hdr[1])
		{
			eventdata[evr_hdr[0] + evr_hdr[2]] = type;
			eventdata[evr_hdr[0] + evr_hdr[2] + 1] = (unsigned char)(prop1 >> 8);
			eventdata[evr_hdr[0] + evr_hdr[2] + 2] = (unsigned char)(prop1 & 255);
			eventdata[evr_hdr[0] + evr_hdr[2] + 3] = (unsigned char)(prop2 >> 8);
			eventdata[evr_hdr[0] + evr_hdr[2] + 4] = (unsigned char)(prop2 & 255);
			evr_hdr[2] += 5;
		}
		else
			recording = false;
	}
	else if (type == EREC_WHEEL)
	{
		if (prop1 < 0)
			prop1 = 0;
		else
			prop1 = 1;
		if (evr_hdr[0] + evr_hdr[2] + 2 <= evr_hdr[1])
		{
			eventdata[evr_hdr[0] + evr_hdr[2]] = type;
			eventdata[evr_hdr[0] + evr_hdr[2] + 1] = (unsigned char)(prop1);
			evr_hdr[2] += 2;
		}
		else
			recording = false;
	}
}

///////////////////////////////////////////////////////////////////
// ClassEdge: private
///////////////////////////////////////////////////////////////////

//Initialize configuration structures
void ClassEdge::InitConfig(void)
{
	ClassEStd::StrCpy(e_displayconfig.caption, E_ENGINENAME " Application");
	e_displayconfig.width = 240;
	e_displayconfig.height = 320;
	e_displayconfig.videotype = EDSP_DEFAULT;
	e_displayconfig.emulvideotype = EDSP_DEFAULT;
	e_displayconfig.orientation = DOR_NONE;
	e_displayconfig.dlgleft = 0;
	e_displayconfig.dlgtop = 0;
	e_displayconfig.openmethod = DOM_DEFAULT;
	e_displayconfig.surfacedefault = EST_SYSMEMORY;
	#if defined(DEVICE_DESKTOP)
		e_displayconfig.fullscreen = false;
	#else
		e_displayconfig.fullscreen = true;
	#endif
	e_displayconfig.fullscreenmouse = false;
	e_displayconfig.dspnative = true;
	e_displayconfig.enginestatus = true;
	e_displayconfig.engineconsole = false;
	e_displayconfig.surfprerotate = true;
	e_displayconfig.emulatelowres = false;
	e_displayconfig.stencil = false;
	e_displayconfig.icon = 0;
	e_displayconfig.menu = 0;
	e_displayconfig.dialogbox = 0;
	e_displayconfig.supportedorientations = 0xf;
	e_frameworkconfig.ffwdkey = 0;
	e_frameworkconfig.slomokey = 0;
	e_frameworkconfig.maxfps = 60;
	e_frameworkconfig.flags = 0;
	e_frameworkconfig.numconnections = 1;
	e_frameworkconfig.smartfps = true;
	#if defined(DEVICE_DESKTOP)
		e_frameworkconfig.focusminimize = false;
	#else
		e_frameworkconfig.focusminimize = true;
	#endif
	e_frameworkconfig.jogtiltarrow = false;
	e_frameworkconfig.focussuspend = true;
	e_frameworkconfig.backlighton = true;
	e_frameworkconfig.netlogging = false;
	e_frameworkconfig.lockkeys = true;
	e_frameworkconfig.keysounds = false;
}

//Initialize variables
void ClassEdge::InitVars(void)
{
	winhandle      = 0;
	fps            = 0;
	lockframetmr   = 0;
	lockframetmrex = 0;
	realframetmr   = 0;
	ChangeAppStatus(EAPP_NOTRUNNING);
	e_nextminimize = false;
	hold_slomo     = false;
	hold_ffwd      = false;
	recording      = false;
	e_tiltemu.enablestylus = false;
	e_tiltemu.enabledpad   = false;
	e_tiltemu.last_enablestylus = false;
	e_tiltemu.last_enabledpad   = false;
}

//Check configuration
bool ClassEdge::CheckConfig(void)
{
	#if defined(DEVICE_DESKTOP)
		if (e_displayconfig.width % 4 != 0)
		{
			SetErrorMsg("Wrong display size: width not divisible by 4");
			return(false);
		}
		#if defined(DEVICE_WIN32)
			if (!e_displayconfig.fullscreen && (e_displayconfig.width > (unsigned long)GetSystemMetrics(SM_CXSCREEN) || e_displayconfig.height > (unsigned long)GetSystemMetrics(SM_CYSCREEN)))
			{
				SetErrorMsg("Can't create window, it's too big for the current resolution");
				return(false);
			}
		#endif
	#endif
	if (e_displayconfig.orientation != DOR_NONE && e_displayconfig.orientation != DOR_90DEG && e_displayconfig.orientation != DOR_180DEG && e_displayconfig.orientation != DOR_270DEG && e_displayconfig.orientation != DOR_PORTRAIT && e_displayconfig.orientation != DOR_LANDSCAPE && e_displayconfig.orientation != DOR_AUTO && e_displayconfig.orientation != DOR_STARTUP)
	{
		SetErrorMsg("Unknown display rotation");
		return(false);
	}
	if (e_frameworkconfig.maxfps > 0 && e_frameworkconfig.maxfps < 5)
	{
		SetErrorMsg("FPS can't be locked below 5");
		return(false);
	}
	if (e_frameworkconfig.focussuspend && e_frameworkconfig.flags & EFL_RUNMINIMIZED)
	{
		SetErrorMsg("EFL_RUNMINIMIZED requires focussuspend to be disabled");
		return(false);
	}
	if (EdgeGL_Use())
	{
		e_displayconfig.enginestatus = false;
		e_displayconfig.dialogbox = 0;
	}
	return(true);
}

//Start up the framework
ERESULT ClassEdge::StartFramework(void)
{
	if (curwinstyle == NULL)
		return(E_NOMEMORY);
	ERESULT er = E_OK;
	EC_WINDOWSTYLE *winstyle = (EC_WINDOWSTYLE *)curwinstyle;
	InitConfig();
	InitVars();
	er = OnFrameworkConfig(&e_frameworkconfig);
	if (er != E_OK)
		return(er);
	er = OnDisplayConfig(&e_displayconfig);
	if (er != E_OK)
		return(er);
	if (!CheckConfig())
		return(E_ERROR);
	e_console.LinkInstance(curinst, e_data.instance);
	#if defined(DEVICE_DESKTOP)
		winstyle->allowdoubleinst = true;
	#else
		winstyle->allowdoubleinst = false;
	#endif
	#if defined(DEVICE_ANDROID)
        if (e_displayconfig.orientation == DOR_AUTO) // force = 3 for android/startup
            winstyle->forceorientation = 0;
    #else
        if (e_displayconfig.orientation == DOR_AUTO || e_displayconfig.orientation == DOR_STARTUP)
            winstyle->forceorientation = 0;
    #endif
	else if (e_displayconfig.orientation == DOR_PORTRAIT)
		winstyle->forceorientation = 1;
	else if (e_displayconfig.orientation == DOR_LANDSCAPE)
		winstyle->forceorientation = 2;
	else
		winstyle->forceorientation = 3;
	if (e_displayconfig.fullscreen)
		winstyle->fullscreen = 2;
	else
		winstyle->fullscreen = 0;
	#if defined(DEVICE_GIZMONDO) || defined(DEVICE_SYMBIAN)
		winstyle->fullscreen = 2;
	#endif
	#if defined(DEVICE_DESKTOP)
		if (winstyle->fullscreen > 0)
			winstyle->showclickcursor = e_displayconfig.fullscreenmouse;
		else
			winstyle->showclickcursor = true;
	#else
		if (winstyle->fullscreen == 0)
			winstyle->fullscreen = 1;
		winstyle->showclickcursor = false;
	#endif
	if (winstyle->fullscreen)
		winstyle->statusbar = false;
	else
		winstyle->statusbar = e_displayconfig.enginestatus;
	winstyle->width = e_displayconfig.width;
	winstyle->height = e_displayconfig.height;
	winstyle->icon = e_displayconfig.icon;
	winstyle->menu = e_displayconfig.menu;
	winstyle->dialogbox = e_displayconfig.dialogbox;
	winstyle->emulatelowres = e_displayconfig.emulatelowres;
	winstyle->lockkeys = e_frameworkconfig.lockkeys;
	winstyle->keysounds = e_frameworkconfig.keysounds;
	winstyle->focussuspend = e_frameworkconfig.focussuspend;
	winstyle->corethread = ((e_frameworkconfig.flags & EFL_DIALOGTHREAD) > 0);
	winstyle->winborder = ((e_frameworkconfig.flags & EFL_BORDERLESSWND) == 0);
	winstyle->outwndclickcursor = ((e_frameworkconfig.flags & EFL_MORECLICKMOVE) > 0);
	winstyle->hasopengl = EdgeGL_Use();
	ClassEStd::StrCpy(winstyle->title, e_displayconfig.caption);
	if (e_frameworkconfig.numconnections)
	{
		unsigned char ctr;
		if (e_frameworkconfig.numconnections > MAX_ECONNECT)
			e_frameworkconfig.numconnections = MAX_ECONNECT;
		for (ctr = 0; ctr < e_frameworkconfig.numconnections; ctr++)
		{
			ecd.net[ctr] = new ClassEConnect;
			ecd.net[ctr]->SetConnectID(ctr);
		}
	}
	if (ecInstCreateWindow(curinst, winstyle) >= 2)
	{
		SetErrorMsg("Can't create window");
		return(E_ERROR);
	}
	return(E_OK);
}

//Start the display device and main loop
ERESULT ClassEdge::StartDisplay(void)
{
	ERESULT er = E_OK;
	e_input.GetButtonList(&e_buttons);
	e_input.SetDefaultMappings();
	e_vtimer.Reset();
	e_vtimer.SetSpeed(1000);
	e_math.Init();
	er = OnInit(e_data.instance);
	if (er != E_OK)
		return(er);
	RegisterDisplayLoaders(&e_display);
	er = OnPluginLoad(&e_display);
	if (er != E_OK)
		return(er);
	er = e_display.Open(curinst, curwinstyle, &ecd);
	if (er == E_OK)
	{
		er = OnDisplayInit(e_data.instance, &e_display);
		if (er != E_OK)
		{
			e_display.Close();
			return(er);
		}
	}
	else
	{
		if (er == E_INVALIDPARAM)
			SetErrorMsg("Display has received invalid parameters");
		else if (er == E_UNSUPPORTED)
			SetErrorMsg("Display couldn't be opened by EDGELIB");
		else if (er == E_ERROR)
			SetErrorMsg("Couldn't open the display device");
		return(er);
	}
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		if (e_displayconfig.engineconsole && e_displayconfig.fullscreen == false)
		{
			ecEvent t_event;
			e_console.ActivateWindow();
			t_event.param1 = 1;
			ecInstEventFromButton(curinst, &t_event, true);
		}
	#endif
	SetStatusMsg("Running " E_ENGINENAME "...");
	ChangeAppStatus(EAPP_IDLE);	
	if (e_frameworkconfig.flags & EFL_TILTEMULATOR)
		StartTiltEmulator();
	if (((EC_WINDOWSTYLE *)(curwinstyle))->dialogbox == 0)
		ecInstStartLoop(curinst);	
	return(E_OK);
}

//Change the status of the application
void ClassEdge::ChangeAppStatus(unsigned char newstatus)
{
	unsigned char prevappstatus = appstatus;
	switch(newstatus)
	{
		case EAPP_IDLE:
			if (prevappstatus == EAPP_NOFOCUS || prevappstatus == EAPP_MINIMIZED)
			{
				appstatus = newstatus;
				e_sound.Resume();
				e_display.Resume();
				e_input.ResetState();
				if (((prevappstatus == EAPP_MINIMIZED) && !(e_frameworkconfig.flags & EFL_RUNMINIMIZED)) || ((prevappstatus == EAPP_NOFOCUS) && e_frameworkconfig.focussuspend))
					e_vtimer.Resume();
				if (prevappstatus == EAPP_MINIMIZED || e_frameworkconfig.focussuspend)				
					OnRestore();
			}
			else if (prevappstatus == EAPP_NOTRUNNING)
				appstatus = newstatus;
			break;
		case EAPP_NOFOCUS:
			if (prevappstatus == EAPP_IDLE)
			{
				appstatus = newstatus;
				if (e_frameworkconfig.focussuspend || e_display.fullscreenmode)
				{
					#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
						if (e_display.fullscreenmode)
							ShowWindow((HWND)winhandle, SW_MINIMIZE);
					#endif
					e_display.Suspend();
				}
			}
			break;
		case EAPP_MINIMIZED:
			if (prevappstatus == EAPP_IDLE || prevappstatus == EAPP_NOFOCUS)
			{
				appstatus = newstatus;
				OnMinimize();
				if (!(e_frameworkconfig.flags & EFL_RUNMINIMIZED))
					e_vtimer.Suspend();
				e_display.Suspend();
				e_sound.Suspend();
			}
			break;
		case EAPP_DESTROY:
			if (prevappstatus == EAPP_IDLE || prevappstatus == EAPP_NOFOCUS || prevappstatus == EAPP_MINIMIZED || prevappstatus == EAPP_NOTRUNNING)
				appstatus = newstatus;
			break;
		case EAPP_NOTRUNNING:
			appstatus = newstatus;
			break;
		case EAPP_INVALID:
			if (appstatus == EAPP_DESTROY)
			{
				appstatus = newstatus;
				OnExit(e_data.instance);
				e_display.Close();
				e_sound.Close();
			}
			else if (appstatus == EAPP_NOTRUNNING)
				appstatus = newstatus;
			break;
	}
}

//Calculate the current amount of frames per second
void ClassEdge::CalculateFPS(void)
{
	unsigned long newtime = e_vtimer.real.Count();
	unsigned long diff;
	if (fpsstamp > newtime)
		diff = 0xFFFFFFFF - fpsstamp + newtime + 1;
	else
		diff = newtime - fpsstamp;
	if (diff > 500)
	{
		fps = fpspassed * 100000 / diff;
		fpsstamp = newtime;
		fpspassed = 0;
	}
	fpspassed++;
}

//A simple framerate lock defined by the maxfps config setting
void ClassEdge::LockFPS(void)
{
	unsigned long timetosleep = 0;
	if (e_frameworkconfig.maxfps)
	{
		unsigned long savedtmr = lockframetmrex;
		realframetmr += e_vtimer.RetrieveDelta() * 1000;
		lockframetmr += (1000000 + savedtmr) / e_frameworkconfig.maxfps;
		lockframetmrex = (1000000 + savedtmr) % e_frameworkconfig.maxfps;
		if (lockframetmr > realframetmr)
			timetosleep = (lockframetmr - realframetmr) / 1000;
		else
			lockframetmr = realframetmr;
	}
	else
		lockframetmr = realframetmr;
	if (timetosleep == 0 && e_frameworkconfig.smartfps)
		timetosleep = 1;
	ecInstSetLoopSleep(curinst, timetosleep);
	if (realframetmr >= 10000000 && lockframetmr >= 10000000)
	{
		realframetmr %= 10000000;
		lockframetmr %= 10000000;
	}
}

//Play recording
void ClassEdge::PlayRecordingLoop(unsigned long timedelta)
{
	bool quitframe = false;
	unsigned long *evr_hdr = (unsigned long *)e_eventrecordplay;
	recframetmr += timedelta;
	while(recframetmr && !quitframe)
	{
		POINT clickpnt;
		unsigned long tvar;
		switch(e_eventrecordplay[evr_hdr[0] + evr_hdr[1]])
		{
			case EREC_TIMEDELTA:
			case EREC_TIMEDELTA16:
				if (e_eventrecordplay[evr_hdr[0] + evr_hdr[1]] == EREC_TIMEDELTA)
				{
					tvar = e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 1];
					if (tvar <= recframetmr)
						evr_hdr[1] += 2;
				}
				else
				{
					tvar = (e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 1] << 8) + e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 2];
					if (tvar <= recframetmr)
						evr_hdr[1] += 3;
				}
				if (tvar <= recframetmr)
				{
					e_input.CheckRepeat(tvar);
					OnNextFrame(&e_display, tvar);
					e_input.ResetEvents();
					recframetmr -= tvar;
				}
				else
					quitframe = true;
				break;
			case EREC_KEYUP:
				tvar = (e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 1] << 8) + e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 2];
				e_input.OnKeyUp(tvar);
				OnButtonUp(tvar, &e_buttons);
				evr_hdr[1] += 3;
				break;
			case EREC_KEYDOWN:
				tvar = (e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 1] << 8) + e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 2];
				e_input.OnKeyDown(tvar);
				OnButtonDown(tvar, &e_buttons);
				evr_hdr[1] += 3;
				break;
			case EREC_CLICKUP:
			case EREC_CLICKDOWN:
			case EREC_CLICKDOUBLE:
				clickpnt.x = (e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 1] << 8) + e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 2];
				clickpnt.y = (e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 3] << 8) + e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 4];
				clickpnt.x -= 32768;
				clickpnt.y -= 32768;
				if (e_eventrecordplay[evr_hdr[0] + evr_hdr[1]] == EREC_CLICKUP)
				{
					if (e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 5] == EIC_LEFT)
						OnStylusUp(clickpnt);
					e_input.OnClickUp(clickpnt.x, clickpnt.y, e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 5]);
				}
				else if (e_eventrecordplay[evr_hdr[0] + evr_hdr[1]] == EREC_CLICKDOWN)
				{
					if (e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 5] == EIC_LEFT)
						OnStylusDown(clickpnt);
					e_input.OnClickDown(clickpnt.x, clickpnt.y, e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 5]);
				}
				else
				{
					if (e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 5] == EIC_LEFT)
						OnStylusDouble(clickpnt);
					e_input.OnClickDouble(clickpnt.x, clickpnt.y, e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 5]);
				}
				evr_hdr[1] += 6;
				break;
			case EREC_CLICKMOVE:
				clickpnt.x = (e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 1] << 8) + e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 2];
				clickpnt.y = (e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 3] << 8) + e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 4];
				clickpnt.x -= 32768;
				clickpnt.y -= 32768;
				OnStylusMove(clickpnt);
				e_input.OnClickMove(clickpnt.x, clickpnt.y);
				evr_hdr[1] += 5;
				break;
			case EREC_WHEEL:
				if (e_eventrecordplay[evr_hdr[0] + evr_hdr[1] + 1] > 0)
					clickpnt.x = 1;
				else
					clickpnt.x = -1;
				OnWheel(clickpnt.x);
				e_input.OnWheelScroll(clickpnt.x);
				evr_hdr[1] += 2;
				break;
			default:
				quitframe = true;
				break;
		}
	}
	if (evr_hdr[1] >= evr_hdr[2])
	{
		e_eventrecordplay = NULL;
		e_console.ShowMsg("Edge recording stopped");
	}
}

//Called each frame
void ClassEdge::MainLoop(void)
{
	unsigned char cctr;
	for (cctr = 0; cctr < e_frameworkconfig.numconnections; cctr++)
		if (ecd.net[cctr] != NULL)
		{
			ecd.net[cctr]->CheckSockets();
			ecd.net[cctr]->CheckBluetoothDevices();
		}
	if (e_nextminimize)
	{
		ecInstMinimize(curinst);
		e_nextminimize = false;
	}
	if (appstatus != EAPP_IDLE || !e_frameworkconfig.backlighton)
	{
		if (sustainbacklight)
		{
			ecInstSustainBacklight(curinst, false);
			sustainbacklight = false;
		}
	}
	else if (e_frameworkconfig.backlighton && appstatus == EAPP_IDLE)
	{
		if (!sustainbacklight)
		{
			ecInstSustainBacklight(curinst, true);
			sustainbacklight = true;
		}
	}
	#if defined(DEVICE_LINUX)
		long tiltangle[3];
		if (ecd.input->GetTiltData(&ecd, &tiltangle[0], &tiltangle[1], &tiltangle[2]) == E_OK)
			OnCoreEvent(curinst, ECEV_CHANGETILT, tiltangle[0] * 32, tiltangle[1] * 32, tiltangle[2] * 32);
	#endif
	if (appstatus == EAPP_IDLE || (appstatus == EAPP_NOFOCUS && !e_frameworkconfig.focussuspend) || (appstatus == EAPP_MINIMIZED && (e_frameworkconfig.flags & EFL_RUNMINIMIZED)))
	{
		if (hold_slomo)
			e_vtimer.SetSpeed(400);
		else if (hold_ffwd)
			e_vtimer.SetSpeed(2500);
		else
			e_vtimer.SetSpeed(1000);
		e_vtimer.Update();
		e_device.Update(e_vtimer.RetrieveDelta());
		if (((EC_WINDOWSTYLE *)(curwinstyle))->dialogbox == 0 || (((EC_WINDOWSTYLE *)(curwinstyle))->corethread))
			LockFPS();
		CalculateFPS();
		#if defined(E_EVALUATIONTIMER)
			if (e_display.demotimer > 0 && e_display.demotimer < E_EVALUATIONTIMER * 1000)
			{
				unsigned long demodelta = e_vtimer.RetrieveDelta();
				if (demodelta > 100)
					demodelta = 100;
				e_display.demotimer += demodelta;
			}
		#endif
		if (recording)
			Edge_AddRecordEvent(e_eventrecord, recording, EREC_TIMEDELTA, e_vtimer.RetrieveDelta());
		if (e_eventrecordplay)
			PlayRecordingLoop(e_vtimer.RetrieveDelta());
		else
		{
			e_input.CheckRepeat(e_vtimer.RetrieveDelta());
			e_display.buffer.SetDirty();
			if (OnNextFrame(&e_display, e_vtimer.RetrieveDelta()) != E_OK)
				Quit();
			e_input.ResetEvents();
		}
		e_sound.Update(e_vtimer.RetrieveDelta());
		if (!(e_frameworkconfig.flags & EFL_MANUALFLIP) && !waitflipredraw)
			e_display.Flip(false);
	}
	else
	{
		if (((EC_WINDOWSTYLE *)(curwinstyle))->dialogbox == 0 || (((EC_WINDOWSTYLE *)(curwinstyle))->corethread))
		{
			if (e_frameworkconfig.maxfps > 0 && e_frameworkconfig.maxfps < 1000)
			{
				ecInstSetLoopSleep(curinst, 1000 / e_frameworkconfig.maxfps);
			}
			else // safety
				ecInstSetLoopSleep(curinst, 100);
		}
		e_vtimer.Update();
	}
}

///////////////////////////////////////////////////////////////////
// ClassEdge: public
///////////////////////////////////////////////////////////////////

//ClassEdge: constructor
ClassEdge::ClassEdge(void)
{
	unsigned char ctr;
	EC_WINDOWSTYLE *winstyle = new EC_WINDOWSTYLE;
	for (ctr = 0; ctr < MAX_ECONNECT; ctr++)
		ecd.net[ctr] = NULL;
	curwinstyle = winstyle;
	ClassEStd::StrCpy(i_errormsg, "");
	ecd.dat = &e_data;
	ecd.maincfg = &e_frameworkconfig;
	ecd.dispcfg = &e_displayconfig;
	ecd.std = &e_std;
	ecd.dsp = &e_display;
	ecd.snd = &e_sound;
	ecd.input = &e_input;
	ecd.mem = &e_memory;
	ecd.tmr = &e_vtimer;
	ecd.file = &e_file;
	ecd.ini = &e_ini;
	ecd.math = &e_math;
	ecd.reg = &e_reg;
	ecd.device = &e_device;
	ecd.console = &e_console;
	ecd.dsp->edge = this;
	e_eventrecordplay = NULL;
	e_eventrecord = NULL;
	powerhandle = 0;
	sustainbacklight = false;
	#if defined(DEVICE_OPENKODE)
		waitflipredraw = true;
	#else
		waitflipredraw = false;
	#endif
	for (ctr = 0; ctr < 4; ctr++)
		validcams[ctr] = NULL;
}

//ClassEdge: destructor
ClassEdge::~ClassEdge(void)
{
	unsigned char ctr;
	if (curwinstyle)
	{
		EC_WINDOWSTYLE *winstyle = (EC_WINDOWSTYLE *)curwinstyle;
		delete winstyle;
	}
	for (ctr = 0; ctr < MAX_ECONNECT; ctr++)
		if (ecd.net[ctr])
			delete ecd.net[ctr];
	FreeRecording();
	if (sustainbacklight)
		ecInstSustainBacklight(curinst, false);
}

//Start recording events
bool ClassEdge::StartRecording(unsigned long recordsize)
{
	#if (EDITION_SDK < 500)
		return(false);
	#else
		unsigned long *evr_hdr;
		StopRecording();
		if (e_eventrecord != NULL)
		{
			evr_hdr = (unsigned long *)e_eventrecord;
			if (evr_hdr[1] != recordsize)
				FreeRecording();
		}
		if (e_eventrecord == NULL)
		{
			if (recordsize <= ERECORDINFOSIZE)
				return(false);
			e_eventrecord = (unsigned char *)ClassEMemory::Alloc(recordsize);
		}
		if (e_eventrecord != NULL)
		{
			evr_hdr = (unsigned long *)e_eventrecord;
			evr_hdr[0] = ERECORDINFOSIZE;
			evr_hdr[1] = recordsize;
			evr_hdr[2] = 0;
			evr_hdr[3] = ecd.math->GetRandSeed();
			ecd.input->ResetState();
			recording = true;
			return(true);
		}
		return(false);
	#endif
}

//Stop recording events
void ClassEdge::StopRecording(void)
{
	recording = false;
	e_eventrecordplay = NULL;
}

//Free resources used for recording
void ClassEdge::FreeRecording(void)
{
	StopRecording();
	if (e_eventrecord != NULL)
	{
		ClassEMemory::DeAlloc(e_eventrecord);
		e_eventrecord = NULL;
	}
}

//Get the size of the Edge event recording
unsigned long ClassEdge::GetRecordingDataSize(void)
{
	if (e_eventrecord != NULL)
	{
		unsigned long *evr_hdr = (unsigned long *)e_eventrecord;
		return(evr_hdr[0] + evr_hdr[2]);
	}
	return(0);
}

//Get Edge recording event data
void *ClassEdge::GetRecordingData(void)
{
	return(e_eventrecord);
}

//Play a recording
void ClassEdge::PlayRecording(void *recorddata)
{
	#if (EDITION_SDK < 500)
		return;
	#else
		StopRecording();
		e_eventrecordplay = (unsigned char *)recorddata;
		unsigned long *evr_hdr = (unsigned long *)e_eventrecordplay;
		ecd.input->ResetState();
		ecd.math->SetRandSeed(evr_hdr[3]);
		evr_hdr[1] = 0;
		recframetmr = 0;
	#endif
}

//Get position of softkey labels
unsigned char ClassEdge::GetSoftlabelPosition(unsigned char *firstkeyposition)
{
	unsigned char ctr, pos, key1pos;
	#if defined(DEVICE_GIZMONDO) || defined(DEVICE_GP2X)
		pos = EPOS_TOP;
		key1pos = EPOS_LEFT;
	#elif defined(DEVICE_SERIES80) || defined(DEVICE_SERIES90)
		pos = EPOS_RIGHT;
		key1pos = EPOS_TOP;
	#else
		#if defined(DEVICE_S60) && (DEVICE_SYMBIAN >= 900)
			AknLayoutUtils::TAknCbaLocation cbalocation = AknLayoutUtils::CbaLocation();
			if (e_display.sysorientation == DOR_90DEG)
			{
				if (cbalocation == AknLayoutUtils::EAknCbaLocationRight)
					cbalocation = AknLayoutUtils::EAknCbaLocationBottom;
				else if (cbalocation == AknLayoutUtils::EAknCbaLocationBottom)
					cbalocation = AknLayoutUtils::EAknCbaLocationLeft;
			}
			else if (e_display.sysorientation == DOR_270DEG)
			{
				if (cbalocation == AknLayoutUtils::EAknCbaLocationLeft)
					cbalocation = AknLayoutUtils::EAknCbaLocationBottom;
				else if (cbalocation == AknLayoutUtils::EAknCbaLocationBottom)
					cbalocation = AknLayoutUtils::EAknCbaLocationRight;
			}
			if (cbalocation == AknLayoutUtils::EAknCbaLocationLeft)
			{
				pos = EPOS_LEFT;
				key1pos = EPOS_TOP;
			}
			else if (cbalocation == AknLayoutUtils::EAknCbaLocationRight)
			{
				pos = EPOS_RIGHT;
				key1pos = EPOS_TOP;
			}
			else
			{
				pos = EPOS_BOTTOM;
				key1pos = EPOS_LEFT;
			}
		#else
			pos = EPOS_BOTTOM;
			key1pos = EPOS_LEFT;
		#endif
	#endif
	for (ctr = 0; ctr < e_display.rotation; ctr++)
	{
		switch(pos)
		{
			case EPOS_TOP:
				pos = EPOS_LEFT;
				break;
			case EPOS_BOTTOM:
				pos = EPOS_RIGHT;
				break;
			case EPOS_LEFT:
				pos = EPOS_BOTTOM;
				break;
			case EPOS_RIGHT:
				pos = EPOS_TOP;
				break;
		}
		switch(key1pos)
		{
			case EPOS_TOP:
				key1pos = EPOS_LEFT;
				break;
			case EPOS_BOTTOM:
				key1pos = EPOS_RIGHT;
				break;
			case EPOS_LEFT:
				key1pos = EPOS_BOTTOM;
				break;
			case EPOS_RIGHT:
				key1pos = EPOS_TOP;
				break;
		}
	}
	if (firstkeyposition)
		*firstkeyposition = key1pos;
	return(pos);
}

//Set the OnExit error message
void ClassEdge::SetErrorMsg(const char *msg){ ClassEStd::StrCpy(i_errormsg, msg); }
void ClassEdge::SetErrorMsg(const WCHAR *msg){ ClassEStd::StrCpy(i_errormsg, msg); }

//Set the text in the status bar
void ClassEdge::SetStatusMsg(const char *msg)
{
	USTR_CHECK_BEGIN(msg);
	SetStatusMsg(ustr_msg);
	USTR_CHECK_END(msg);
}

//Set the text in the status bar
void ClassEdge::SetStatusMsg(const WCHAR *msg)
{
	ecInstStatusMsg(curinst, (ecUnistring)msg);
}

//Start the tiltcontrol emulator
void ClassEdge::StartTiltEmulator(short centeranglex, short centerangley, bool emulatestylus, unsigned long stylussensitivity, bool emulatedpad, unsigned long dpadsensitivity)
{
	#if (EDITION_SDK >= 500)
		e_tiltemu.enablestylus = emulatestylus;
		e_tiltemu.stylussensitivity = stylussensitivity;
		e_tiltemu.enabledpad = emulatedpad;
		e_tiltemu.dpaddeadpointx = dpadsensitivity;
		e_tiltemu.dpaddeadpointy = dpadsensitivity;
		e_tiltemu.centerangle_x = centeranglex << 5;
		e_tiltemu.centerangle_y = centerangley << 5;
		if (!e_tiltemu.last_enabledpad)
		{
			e_tiltemu.last_deadpointx = e_tiltemu.dpaddeadpointx;
			e_tiltemu.last_deadpointy = e_tiltemu.dpaddeadpointy;
		}
	#endif
}

//Retrieve the current amount of frames per second with 2 decimals
void ClassEdge::GetFPS(unsigned long &full, unsigned long &decimal)
{
	full = fps / 100;
	decimal = fps % 100;
}

//Retrieve the current amount of frames per second
unsigned long ClassEdge::GetFPS(void)
{
	return(fps / 100);
}

//Get the window handle
ENATIVETYPE ClassEdge::GetWindowHandle(void)
{
	return(ecInstGetWindow(curinst));
}

//Create a new logfile
void ClassEdge::NewLogFile(const char *text)
{
	ClassEFile ef;
	if (ef.New("log.txt"))
		ef.Close();
}

//Log a line of text
void ClassEdge::Log(const char *text, bool newline)
{
	bool opened;
	ClassEFile ef;
	opened = ef.Open("log.txt");
	if (!opened)
		opened = ef.New("log.txt");
	if (opened)
	{
		ef.Seek(ef.Size());
		ef.Write(text, ClassEStd::StrLen(text));
		if (newline)
		{
			char nstr[4];
			ClassEStd::StrCpy(nstr, "\r\n");
			ef.Write(nstr, 2);
		}
		ef.Close();
	}
}

//Log a line of text
void ClassEdge::Log(const WCHAR *text, bool newline)
{
	CSTR_CHECK_BEGIN(text);
	Log(cstr_text, newline);
	CSTR_CHECK_END(text);
}

//Log a value
void ClassEdge::Log(long value, bool newline)
{
	char vstr[32];
	ClassEStd::IntToStr(vstr, value, 10);
	Log(vstr, newline);
}

//Log reroutes
void _ti_edgelib_log(const char *text, bool newline){ ClassEdge::Log(text, newline); }
void _ti_edgelib_log(const WCHAR *text, bool newline){ ClassEdge::Log(text, newline); }
void _ti_edgelib_log(long value, bool newline){ ClassEdge::Log(value, newline); }

//Minimize application
void ClassEdge::Minimize(void)
{
	ChangeAppStatus(EAPP_MINIMIZED);
	e_nextminimize = true;
}

//Quit framework
void ClassEdge::Quit(void)
{
	ecInstExit(curinst);
}

//React on Edgecore events
bool ClassEdge::OnCoreEvent(ECOREOBJ instance, int type, int param1, int param2, int param3)
{
	switch(type)
	{
		case ECEV_CREATEINSTANCE:
			curinst = instance;
			#if defined(DEVICE_GIZMONDO)
				{
					char mdl_str[128];
					ClassEDevice::GetModelName(mdl_str);
					if (!ClassEStd::StrEqual(mdl_str, "Gizmondo", false))
					{
						SetErrorMsg("This program is not designed for this device.");
						return(true);
					}
				}
			#endif
			StartFramework();
			return(true);
		case ECEV_CREATEWINDOW:
			winhandle = (ENATIVETYPE)ecInstGetWindow(instance);
			ecFGetBinaryPath(e_data.currentpath);
			ecFGetReadOnlyPath(e_data.readonlypath);
			if (StartDisplay() != E_OK)
			{
				ecInstExit(instance);
				return(false);
			}
			return(true);
		case ECEV_DESTROYWINDOW:
			ChangeAppStatus(EAPP_DESTROY);
			ChangeAppStatus(EAPP_INVALID);
			#if defined(DEVICE_SYMBIAN)
				if (i_errormsg[0])
					ecInstSetErrorMessage(instance, i_errormsg);
			#endif
			return(true);
		case ECEV_DESTROYINSTANCE:
			#if defined(DEVICE_WIN32)
				if (i_errormsg[0])
					ecInstSetErrorMessage(instance, i_errormsg);
			#endif
			ecInstSetReturnvalue(instance, e_data.returnvalue);
			return(true);
		case ECEV_CHANGEORIENTATION:
			if (param1 == 90)
				e_display.sysorientation = DOR_90DEG;
			else if (param1 == 180)
				e_display.sysorientation = DOR_180DEG;
			else if (param1 == 270)
				e_display.sysorientation = DOR_270DEG;
			else if (param1 != -1)
				e_display.sysorientation = DOR_NONE;
			
			if (param1 == -1 && e_display.fullscreenmode && e_displayconfig.orientation == DOR_AUTO && ((1 << param2) & e_displayconfig.supportedorientations))
				// software rotation event
				e_display.ChangeOrientation(param2);
			else if (param1 != -1 && e_display.fullscreenmode && e_displayconfig.orientation == DOR_AUTO && ((1 << e_display.sysorientation) & e_displayconfig.supportedorientations))
				// autorotate to physical
				e_display.ChangeOrientation(e_display.sysorientation);
			else if (param1 != -1)		
				// rotate to current 
				e_display.ChangeOrientation(e_display.rotation);
			else
			{
				// no need to change orientations
			}
			
			return(true);
		case ECEV_CHANGEFOCUS:
			if (param1 == 1)
			{
				if (e_frameworkconfig.focusminimize)
					Minimize();
				else
					ChangeAppStatus(EAPP_NOFOCUS);
			}
			else if (param1 == 0)
				ChangeAppStatus(EAPP_IDLE);
			else if (param1 == 2)
				ChangeAppStatus(EAPP_MINIMIZED);
			return(true);
		case ECEV_PHONECALL:
			//ecInstMinimize(curinst);
			//Minimize();
			return(true);
		case ECEV_SETCOMMANDLINE:
			#if defined(DEVICE_MAC)
				ecUnichar *tcmdline;
				tcmdline = (ecUnichar *) param1;
				ClassEStd::StrCpy(e_data.commandline, tcmdline);
			#endif
			return(true);
		case ECEV_REDRAW:
			#if defined(DEVICE_DESKTOP) || defined(DEVICE_ANTIX)
			{
				bool allowdraw;
				#if defined(DEVICE_ANTIX)
					allowdraw = !(e_frameworkconfig.flags & EFL_IGNOREREDRAW);
				#else
					allowdraw = (!e_display.fullscreenmode && !(e_frameworkconfig.flags & EFL_IGNOREREDRAW));
				#endif
				if (allowdraw)
				{
					bool tmpsusp = e_display.suspended;
					if (tmpsusp)
						e_display.Resume();
					e_display.Flip();
					if (tmpsusp)
						e_display.Suspend();
				}
			}
			#elif defined(DEVICE_WINMOBILE)
				e_display.suspendfrommode = false;
			#endif
			waitflipredraw = false;
			break;
		case ECEV_IDLE:
			MainLoop();
			return(true);
		case ECEV_SKIPIDLE:
			e_vtimer.Update();
			return(true);
		case ECEV_KEYDOWN:
		case ECEV_KEYUP:
			if (appstatus == EAPP_IDLE)
			{
				#if defined(DEVICE_POCKETPC)
					if (!(e_frameworkconfig.flags & EFL_GETSHADOWKEYS) && (param1 == 91 || param1 == 132))
						param1 = 0;
				#elif defined(DEVICE_SMARTPHONE)
					if (type == ECEV_KEYDOWN)
					{
						if (param1 == VK_LWIN)
						{
							Minimize();
							break;
						}
						else if (param1 == VK_F4)
						{
							Quit();
							break;
						}
					}
					if (!(e_frameworkconfig.flags & EFL_GETSHADOWKEYS) && param1 == 132)
						param1 = 0;
				#endif
				if (param1 > 0)
				{
					unsigned long keynr = param1;
					e_display.VirtualDPad(keynr, &e_buttons);
					if (type == ECEV_KEYDOWN)
					{
						OnButtonDown(keynr, &e_buttons);
						e_input.OnKeyDown(keynr);
					}
					else
					{
						OnButtonUp(keynr, &e_buttons);
						e_input.OnKeyUp(keynr);
					}
					if (e_frameworkconfig.ffwdkey)
						if (keynr == e_frameworkconfig.ffwdkey)
							hold_ffwd = (type == ECEV_KEYDOWN);
					if (e_frameworkconfig.slomokey)
						if (keynr == e_frameworkconfig.slomokey)
							hold_slomo = (type == ECEV_KEYDOWN);
					if (recording)
					{
						if (type == ECEV_KEYUP)
							Edge_AddRecordEvent(e_eventrecord, recording, EREC_KEYUP, keynr);
						else
							Edge_AddRecordEvent(e_eventrecord, recording, EREC_KEYDOWN, keynr);
					}
				}
			}
			else
			{
				hold_slomo = false;
				hold_ffwd = false;
			}
			return(true);
		case ECEV_TYPECHAR:
			#if defined(DEVICE_SMARTPHONE)
				if (param1 == 27)
					param1 = '\b';
			#endif
			e_input.PutChar(param1, param2);
			return(true);
		case ECEV_CLICKUP:
		case ECEV_CLICKDOWN:
		case ECEV_CLICKDOUBLE:
		case ECEV_CLICKMOVE:
			if (appstatus == EAPP_IDLE)
			{
				POINT p;
				unsigned char buttonnr = EIC_ALL;
				if (param1 == 0)
					buttonnr = EIC_LEFT;
				else if (param1 == 1)
					buttonnr = EIC_RIGHT;
				else if (param1 == 2)
					buttonnr = EIC_MIDDLE;
				else if (param1 > 2)
					buttonnr = param1 + 1;
				p.x = param2;
				p.y = param3;
				e_display.VirtualMouse(p.x, p.y);
				if (buttonnr == EIC_ALL || buttonnr == EIC_LEFT)
				{
					if (type == ECEV_CLICKUP)
						OnStylusUp(p);
					else if (type == ECEV_CLICKDOWN)
						OnStylusDown(p);
					else if (type == ECEV_CLICKDOUBLE)
					{
						OnStylusDown(p);
						OnStylusDouble(p);
					}
					else
						OnStylusMove(p);
				}
				if (type == ECEV_CLICKUP)
					e_input.OnClickUp(p.x, p.y, buttonnr);
				else if (type == ECEV_CLICKDOWN)
					e_input.OnClickDown(p.x, p.y, buttonnr);
				else if (type == ECEV_CLICKDOUBLE)
				{
					e_input.OnClickDown(p.x, p.y, buttonnr);
					e_input.OnClickDouble(p.x, p.y, buttonnr);
				}
				else
					e_input.OnClickMove(p.x, p.y, buttonnr);
				if (recording)
				{
					if (type == ECEV_CLICKUP)
						Edge_AddRecordEvent(e_eventrecord, recording, EREC_CLICKUP, p.x, p.y, buttonnr);
					else if (type == ECEV_CLICKDOWN)
						Edge_AddRecordEvent(e_eventrecord, recording, EREC_CLICKDOWN, p.x, p.y, buttonnr);
					else if (type == ECEV_CLICKDOUBLE)
						Edge_AddRecordEvent(e_eventrecord, recording, EREC_CLICKDOUBLE, p.x, p.y, buttonnr);
					else
						Edge_AddRecordEvent(e_eventrecord, recording, EREC_CLICKMOVE, p.x, p.y);
				}
			}
			return(true);
		case ECEV_WHEELSCROLL:
			if (appstatus == EAPP_IDLE)
			{
				OnWheel(param1);
				e_input.OnWheelScroll(param1);
				if (recording)
					Edge_AddRecordEvent(e_eventrecord, recording, EREC_WHEEL, param1);
			}
			return(true);
		case ECEV_CHANGETILT:
			#if (EDITION_SDK >= 500)
				if (appstatus == EAPP_IDLE)
				{
					POINT p, pnt;
					p.x = param1;
					p.y = param2;
					e_display.VirtualTilt(p.x, p.y);
					OnDeviceTilt(p.x >> 5, p.y >> 5, param3 >> 5);
					e_input.OnTilt(p.x >> 5, p.y >> 5, param3 >> 5);
					p.x -= e_tiltemu.centerangle_x;
					p.y -= e_tiltemu.centerangle_y;
					if (e_tiltemu.enabledpad)
					{
						unsigned long simkey = 0;
						if (e_tiltemu.last_x <= -e_tiltemu.last_deadpointx && p.x > -e_tiltemu.last_deadpointx)
							simkey = e_buttons.DPadLeft;
						if (e_tiltemu.last_y <= -e_tiltemu.last_deadpointy && p.y > -e_tiltemu.last_deadpointy)
							simkey = e_buttons.DPadUp;
						if (e_tiltemu.last_x >= e_tiltemu.last_deadpointx && p.x < e_tiltemu.last_deadpointx)
							simkey = e_buttons.DPadRight;
						if (e_tiltemu.last_y >= e_tiltemu.last_deadpointy && p.y < e_tiltemu.last_deadpointy)
							simkey = e_buttons.DPadDown;
						if (simkey)
						{
							OnButtonUp(simkey, &e_buttons);
							e_input.OnKeyUp(simkey);
							simkey = 0;
						}
						if (p.x <= -e_tiltemu.dpaddeadpointx && e_tiltemu.last_x > -e_tiltemu.dpaddeadpointx)
							simkey = e_buttons.DPadLeft;
						if (p.y <= -e_tiltemu.dpaddeadpointy && e_tiltemu.last_y > -e_tiltemu.dpaddeadpointy)
							simkey = e_buttons.DPadUp;
						if (p.x >= e_tiltemu.dpaddeadpointx && e_tiltemu.last_x < e_tiltemu.dpaddeadpointx)
							simkey = e_buttons.DPadRight;
						if (p.y >= e_tiltemu.dpaddeadpointy && e_tiltemu.last_y < e_tiltemu.dpaddeadpointy)
							simkey = e_buttons.DPadDown;
						if (simkey)
						{
							OnButtonDown(simkey, &e_buttons);
							e_input.OnKeyDown(simkey);
						}
						e_tiltemu.last_enabledpad = true;
					}
					else if (e_tiltemu.last_enabledpad)
					{
						unsigned long upsimkey = 0;
						if (e_tiltemu.last_x <= -e_tiltemu.last_deadpointx)
							upsimkey = e_buttons.DPadLeft;
						if (e_tiltemu.last_y <= -e_tiltemu.last_deadpointy)
							upsimkey = e_buttons.DPadUp;
						if (e_tiltemu.last_x >= e_tiltemu.last_deadpointx)
							upsimkey = e_buttons.DPadRight;
						if (e_tiltemu.last_x >= e_tiltemu.last_deadpointy)
							upsimkey = e_buttons.DPadDown;
						if (upsimkey)
						{
							OnButtonUp(upsimkey, &e_buttons);
							e_input.OnKeyUp(upsimkey);
						}
						e_tiltemu.last_enabledpad = false;
					}
					if (e_tiltemu.enablestylus || e_tiltemu.last_enablestylus)
					{
						long screensize = e_display.buffer.GetWidth();
						if (e_display.buffer.GetHeight() > (unsigned)screensize)
							screensize = e_display.buffer.GetHeight();
						if (screensize == 0)
							screensize = 1;
						ClassEMemory::Copy(&pnt, &p, sizeof(POINT));
						if (pnt.x < -e_tiltemu.stylussensitivity)
							pnt.x = -e_tiltemu.stylussensitivity;
						if (pnt.x > e_tiltemu.stylussensitivity)
							pnt.x = e_tiltemu.stylussensitivity;
						if (pnt.y < -e_tiltemu.stylussensitivity)
							pnt.y = -e_tiltemu.stylussensitivity;
						if (pnt.y > e_tiltemu.stylussensitivity)
							pnt.y = e_tiltemu.stylussensitivity;
						pnt.x += e_tiltemu.stylussensitivity;
						pnt.y += e_tiltemu.stylussensitivity;
						pnt.x = pnt.x * screensize / (e_tiltemu.stylussensitivity * 2);
						pnt.y = pnt.y * screensize / (e_tiltemu.stylussensitivity * 2);
						pnt.x -= (screensize - e_display.buffer.GetWidth()) / 2;
						pnt.y -= (screensize - e_display.buffer.GetHeight()) / 2;
						if (e_tiltemu.enablestylus)
						{
							if (!e_tiltemu.last_enablestylus)
							{
								e_input.OnClickDown(pnt.x, pnt.y);
								OnStylusDown(pnt);
							}
							else
							{
								e_input.OnClickMove(pnt.x, pnt.y);
								OnStylusMove(pnt);
							}
							e_tiltemu.last_enablestylus = true;
						}
						else if (e_tiltemu.last_enablestylus)
						{
							OnStylusUp(pnt);
							e_input.OnClickUp(pnt.x, pnt.y);
							e_tiltemu.last_enablestylus = false;
						}
					}
					e_tiltemu.last_x = p.x;
					e_tiltemu.last_y = p.y;
					e_tiltemu.last_deadpointx = e_tiltemu.dpaddeadpointx;
					e_tiltemu.last_deadpointy = e_tiltemu.dpaddeadpointy;
				}
				else
				{
					#if (DEVICE_SYMBIAN >= 900) && defined(DEVICE_S60)
						if (param1 == 0 && param2 == 0 && param3 == 0xFFFFFF)
							e_input.OnTilt(0, 0, 0);
					#endif
				}
			#endif
			return(true);
		case ECEV_CONTEXTLOST:
			e_display.OnContextLost();
            if (OnContextLost(&e_display) != E_OK) Quit();
            return true;
		case ECEV_NATIVE:
			#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
				if (param1 == WM_INITDIALOG)
					winhandle = (ENATIVETYPE)ecInstGetWindow(curinst);
			#endif

			#if defined(DEVICE_WIN32)			
				if (param1 == WM_CAMERAFRAME)
				{		
					ClassECamera * wparam = (ClassECamera *) param2;					
					bool accept = false;
					for (int i = 0; i < 4; i++)
					{
						if (validcams[i] == wparam)
							accept = true;
					}

					if(accept)
					{
						E_CAMERAFRAME * frame = (E_CAMERAFRAME *) ecCamAsyncFrameRetrieve(wparam->cameraclass, param3);
						if (frame)
						{
							OnCameraFrame(wparam, frame);
							ecCamAsyncFrameComplete(wparam->cameraclass, param3);
						}
					}
				}				
			#endif

			bool nativereturn;
			nativereturn = OnNativeEvent((ENATIVETYPE)winhandle, (ENATIVETYPE)param1, (ENATIVETYPE)param2, (ENATIVETYPE)param3);
			#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
				if (((EC_WINDOWSTYLE *)(curwinstyle))->dialogbox == 0)
				{
					unsigned char buttonid;
					ecEvent b_event;
					b_event.type = type;
					b_event.param1 = param1;
					b_event.param2 = param2;
					b_event.param3 = param3;
					buttonid = ecInstEventFromButton(curinst, &b_event, false);
					if (buttonid == 1)
					{
						char msgstr[512];
						unsigned long fpsl, fpsd;
						GetFPS(fpsl, fpsd);
						ClassEStd::StrCpy(msgstr, E_ENGINENAME);
						ClassEStd::StrCat(msgstr, " ");
						ClassEStd::StrCat(msgstr, E_VERSION);
						ClassEStd::StrCat(msgstr, "\n");
						ClassEStd::StrCat(msgstr, "Current FPS: ");
						ClassEStd::StrCat(msgstr, fpsl);
						ClassEStd::StrCat(msgstr, ".");
						ClassEStd::StrCat(msgstr, fpsd);
						MessageBox((HWND)ecInstGetWindow(curinst), msgstr, E_ENGINENAME " information", MB_OK | MB_ICONINFORMATION);
					}
					else if (buttonid == 2)
						e_console.ActivateWindow();
					else if (buttonid == 3)
						e_console.ActivateWindow(true);
					if (buttonid)
						return(true);
				}
			#elif defined(DEVICE_WINMOBILE)
				if (!e_display.fullscreenmode)
				{
					if (param1 == WM_SETTINGCHANGE)
					{
						if (param2 == SPI_SIPMOVE)
						{
							e_display.tmpsuspend = true;
							nativereturn = true;
						}
						else if (param2 == SPI_SETSIPINFO && !e_display.tmpsuspend)
							e_display.resizefromsip = true;
					}
					else if (param1 == WM_SIZE && e_display.resizefromsip)
					{
						e_display.OnChangeSize();
						e_display.resizefromsip = false;
					}
					if (param1 == WM_CANCELMODE)
						e_display.suspendfrommode = true;
				}
			#endif
			return(nativereturn);
		case ECEV_PRENATIVE:
			return(OnPreNativeEvent(param1));
	}
	return(false);
}

void ClassEdge::RegisterCam(ECOREOBJ cam)
{
    int i;
    for (i = 0; i < 3; i++)
        if (validcams[i] == NULL)
            break;
    validcams[i] = cam;
}

void ClassEdge::UnregisterCam(ECOREOBJ cam)
{
    for (int i = 0; i < 3; i++)
        if (validcams[i] == cam)
            validcams[i] = NULL;


}

///////////////////////////////////////////////////////////////////
// Event handler
///////////////////////////////////////////////////////////////////

//Edgemain entry point prototype
ClassEdge *EdgeMain(EDGESTARTUP *data);

//Collect data function
void CollectEdgeCoreData(ECOREOBJ instance, EDGEDATA *data, EDGESTARTUP *startup)
{
	data->instance = startup->instance;
	ClassEStd::StrCpy(data->commandline, startup->commandline);
	ClassEStd::StrCpy(data->currentpath, "");
	data->returnvalue = startup->returnvalue;
	data->global1 = 0;
	data->global2 = 0;
}

//Go to entry point
ClassEdge *GoEdgeMain(ECOREOBJ instance, ENATIVETYPE insthandle, long *rval)
{
	EDGESTARTUP *startup;
	ecUnichar *cmdline;
	startup = (EDGESTARTUP *)ClassEMemory::Alloc(sizeof(EDGESTARTUP));
	if (startup == NULL)
		return(NULL);
	cmdline = (ecUnichar *)ClassEMemory::Alloc(MAX_CMDLINE * sizeof(ecUnichar));
	if (cmdline)
	{
		ecInstGetCommandline(instance, cmdline);
		ClassEStd::StrCpy(startup->commandline, cmdline);
		ClassEMemory::DeAlloc(cmdline);
	}
	else
		ClassEStd::StrCpy(startup->commandline, "");
	startup->instance = insthandle;
	startup->returnvalue = 0;
	ClassEdge *classinst = EdgeMain(startup);
	*rval = startup->returnvalue;
	if (classinst)
		CollectEdgeCoreData(instance, classinst->ecd.dat, startup);
	ClassEMemory::DeAlloc(startup);
	return(classinst);
}

//Event callback
bool ecOnEvent(ECOREOBJ instance, ecEvent *event)
{
	ClassEdge *curclassinst;
	bool returnvalue = false;
	if (event->type == ECEV_REQUESTUID)
	{
		event->param1 = EdgeGetUIDNr();
		return(true);
	}
	if (event->type == ECEV_CREATEINSTANCE)
	{
		long rval = 0;
		curclassinst = GoEdgeMain(instance, (ENATIVETYPE)event->param1, &rval);
		ecInstSetGlobal(instance, (ecParam)curclassinst, 0);
		if (curclassinst == NULL)
		{
			ecInstSetReturnvalue(instance, rval);
			return(true);
		}
	}
	else
		curclassinst = (ClassEdge *)ecInstGetGlobal(instance, 0);
	if (curclassinst)
		returnvalue = curclassinst->OnCoreEvent(instance, event->type, event->param1, event->param2, event->param3);
	if (event->type == ECEV_DESTROYINSTANCE)
		if (curclassinst)
		{
			delete curclassinst;
			ecInstSetGlobal(instance, 0, 0);
		}
	return(returnvalue);
}
