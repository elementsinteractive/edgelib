// cdisplay.cpp
// Edgecore: provides display interface
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Device include
#include "edgedevice.h"

//Device SDK includes
#if defined(DEVICE_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_WINMOBILE)
			#if (DEVICE_WINMOBILE < 500)
				#include "extapi/imp_ddraw.h"
			#else
				#include <ddraw.h>
			#endif
			#include "extapi/imp_dspchange.h"
			#include <gx.h>
			#if (DEVICE_WINMOBILE >= 600)
				#include "extapi/imp_gx.h"
			#endif
		#else
			#if defined(DEVICE_WINCE)
				#include "extapi/imp_dspchange.h"
				#include "extapi/imp_gx.h"
			#endif
			#include <ddraw.h>
		#endif
	#endif
#elif defined(DEVICE_SYMBIAN)
	#include <fbs.h>
	#include <hal.h>
	#include <eikenv.h>
	#if (DEVICE_SYMBIAN >= 610)
		#include <w32std.h>
		#include "extapi/symbiandsa.h"
	#endif
#elif defined(DEVICE_LINUX)
	#if defined(DEVICE_X11)
		#include <X11/Xlib.h>
		#include <X11/Xutil.h>
		#if !defined(DEVICE_EMBEDDEDLINUX)
			//#include <X11/extensions/xf86vmode.h>
		#endif
		#include <X11/extensions/Xv.h>
		#include <X11/extensions/Xvlib.h>
	#endif
	#include <sys/mman.h>
	#include <sys/ioctl.h>
	#include <linux/fb.h>
	#include <fcntl.h>
	#include <stdlib.h>
	#include <unistd.h>
	#if defined(DEVICE_ANDROID)
		#include <sys/stat.h>
		#include <stdio.h>
		#include <jni.h>
		#include "extapi/androidbuffer.h"
		int Call_EdgelibView_getWidth();
		int Call_EdgelibView_getHeight();
	#endif
#elif defined(DEVICE_OPENKODE)
	#include <EGL/egl.h>
	#include <EGL/eglext.h>
#endif

//Core include
#include "edgecore.h"
#if defined(DEVICE_ANDROID)
    #include "instance.h"
    int Call_EdgelibView_getWidth();
    int Call_EdgelibView_getHeight();
    extern EC_INSTANCEOBJ * android_global_instance;
#endif

#if defined(DEVICE_MAC)
	void MacGetNativeResolution(ECOREOBJ winhandle, long &xres, long &yres);
	bool MacOpenDisplay(ECOREOBJ winhandle, long &xres, long &yres);
	bool MacAllocSurface(ECOREOBJ winhandle, ECOREOBJ &surf, void *buffer, long xres, long yres, long lpitch, long bitdepth);
	bool MacDeallocSurface(ECOREOBJ surf);
	bool MacFlipSurface(ECOREOBJ winhandle, ECOREOBJ surf, long *pos, long *rect);
#endif

///////////////////////////////////////////////////////////////////
// DLL imports
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_WIN32)
	#if defined(DEVICE_DESKTOP)
		typedef HRESULT (WINAPI *FUNC_DIRECTDRAWCREATE)(GUID FAR *, LPDIRECTDRAW FAR *, IUnknown FAR *);
	#else
		typedef LONG (*FUNC_DIRECTDRAWCREATE)(LPGUID, LPUNKNOWN *, LPUNKNOWN *);
	#endif
#endif

#if defined(DEVICE_WINCE)
	typedef int (*FUNC_GXOPENDISPLAY)(HWND hWnd, DWORD dwFlags);
	typedef int (*FUNC_GXCLOSEDISPLAY)(void);
	typedef GXDisplayProperties (*FUNC_GXGETDISPLAYPROPERTIES)(void);
	typedef int (*FUNC_GXSETVIEWPORT)(DWORD dwTop, DWORD dwHeight, DWORD dwReserved1, DWORD dwReserved2);
	typedef int (*FUNC_GXSUSPEND)(void);
	typedef int (*FUNC_GXRESUME)(void);
	typedef void * (*FUNC_GXBEGINDRAW)(void);
	typedef int (*FUNC_GXENDDRAW)(void);
#endif

///////////////////////////////////////////////////////////////////
// Internal definitions and structures
///////////////////////////////////////////////////////////////////

//A display object
typedef struct
{
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_DESKTOP)
			LPDIRECTDRAWPALETTE dpalette;
		#else
			HMODULE dll_gapi;
			FUNC_GXOPENDISPLAY func_gxopendisplay;
			FUNC_GXCLOSEDISPLAY func_gxclosedisplay;
			FUNC_GXGETDISPLAYPROPERTIES func_gxgetdisplayproperties;
			FUNC_GXSETVIEWPORT func_gxsetviewport;
			FUNC_GXSUSPEND func_gxsuspend;
			FUNC_GXRESUME func_gxresume;
			FUNC_GXBEGINDRAW func_gxbegindraw;
			FUNC_GXENDDRAW func_gxenddraw;
			HDC framelockdc;
			bool gapiopen;
			bool extescapeopen;
			bool emulcoords;
			bool rotatedextescape;
		#endif
		HMODULE dll_ddraw;
		FUNC_DIRECTDRAWCREATE func_ddrawcreate;
		LPDIRECTDRAW ddraw;
		LPDIRECTDRAWSURFACE dfrontbuf;
		HDC gdiwindc;
		HDC gdidc;
	#elif defined(DEVICE_SYMBIAN)
		#if (DEVICE_SYMBIAN >= 610)
			ClassDSA *dsa;
		#else
			void *dsa;
		#endif
		TDisplayMode dispmode;
		CFbsScreenDevice *screendevice;
		CFbsBitGc *bitgc;
		unsigned long bufoffs;	
	#elif defined(DEVICE_LINUX)
		#if defined(DEVICE_ANDROID)
		    AndroidBuffer * backbuffer;
		#endif
		#if defined(DEVICE_X11)
			XImage *ximg_framebuf;
			XvImage *xvimg_overlay;
			unsigned char bitdepth;
			int xv_port;
			int xv_formatid;
		#endif
		int framebuf_ptr;
		unsigned char *framebuffer;
		long framebufsize;
	#elif defined(DEVICE_MAC)
		unsigned char bitdepth;
	#elif defined(DEVICE_OPENKODE)
		PFNEGLLOCKSURFACEKHR locksurface;
		PFNEGLUNLOCKSURFACEKHR unlocksurface;
		EGLSurface windowsurface;
		bool displayalpha;
		bool flippedorigin;
	#endif
	ecNativetype winhandle;
	bool unlockrefresh;
}EC_DISPLAYOBJ;

//A native surface object
typedef struct
{
	#if defined(DEVICE_WIN32)
		LPDIRECTDRAWSURFACE surf;
		HDC bmpdc;
		HBITMAP hbitmap;
		unsigned long hbmppitch;
		void *hbmppix;
	#elif defined(DEVICE_SYMBIAN)
		CFbsBitmap *surf;
		TBitmapUtil *surflock;
	#elif defined(DEVICE_ANDROID)
		AndroidBuffer * surf;
	#elif defined(DEVICE_X11)
		XImage *surf;
		XvImage *overlay;
		void *membuffer;
		long lpitch;
		long membufsize;
	#elif defined(DEVICE_MAC)
		void *membuffer;
		void *surf;
		long lpitch;
	#elif defined(DEVICE_OPENKODE)
		EGLSurface surf;
		unsigned long width;
		unsigned long height;
	#else
		void *surf;
	#endif
}EC_SURFACEOBJ;

//Raw frame buffer support for old Windows Mobile versions
#if defined(DEVICE_WINCE)
	#if !defined(GETRAWFRAMEBUFFER)
		typedef struct
		{
			WORD wFormat;
			WORD wBPP;
			VOID *pFramePointer;
			int cxStride;
			int cyStride;
			int cxPixels;
			int cyPixels;
		}RawFrameBufferInfo;

		#define GETRAWFRAMEBUFFER      0x00020001
		#define FORMAT_565             1
		#define FORMAT_555             2
		#define FORMAT_OTHER           3 //GAPI extension
	#endif
#endif

//Definition fixes
#if defined(DEVICE_WINMOBILE)
	#define DDLOCK_WAIT DDLOCK_WAITNOTBUSY
	#define DDERR_NODIRECTDRAWHW DDERR_UNSUPPORTED
	#define DDBLT_WAIT DDBLT_WAITNOTBUSY
#endif

#if defined(DEVICE_SYMBIAN)
	#ifndef EColor16MU
		#define EColor16MU 11
	#endif
	#ifndef EColor16MA
		#define EColor16MA 12
	#endif
#endif


///////////////////////////////////////////////////////////////////
// Diagnostics functions
///////////////////////////////////////////////////////////////////

//Check windows mobile properties
bool eciDspWindowsMobileDiag(ecNativetype winhandle)
{
	#if defined(DEVICE_WINMOBILE)
		ECOREOBJ fileobj = ecExCreateLogFile();
		if (fileobj)
		{
			//Check versioning info
			EC_DISPLAYOBJ *dspo;
			OSVERSIONINFO verinfo;
			ecExWriteLog(fileobj, "- Starting display diagnostics -");
			if (FindResource(GetModuleHandle(NULL), TEXT("HI_RES_AWARE"), TEXT("CEUX")) == NULL)
				ecExWriteLog(fileobj, "Native high-resolution flag not detected, diagnostics may return bad values");
			ecMemSet(&verinfo, 0, sizeof(OSVERSIONINFO));
			verinfo.dwOSVersionInfoSize = sizeof(verinfo);
			if (GetVersionEx(&verinfo) == TRUE)
			{
				ecExWriteLog(fileobj, "OS version: ", false);
				ecExWriteLog(fileobj, verinfo.dwMajorVersion, false);
				ecExWriteLog(fileobj, ", ", false);
				ecExWriteLog(fileobj, verinfo.dwMinorVersion, false);
				ecExWriteLog(fileobj, " / ", false);
				ecExWriteLog(fileobj, verinfo.dwBuildNumber, false);
				ecExWriteLog(fileobj, " (", false);
				ecExWriteLog(fileobj, verinfo.dwPlatformId, false);
				ecExWriteLog(fileobj, ")");
				//char vinfo[256];
				//ecStrCpy(vinfo, verinfo.szCSDVersion); //http://msdn.microsoft.com/en-us/library/ms942638.aspx
				//ecExWriteLog(fileobj, vinfo);
			}
			else
				ecExWriteLog(fileobj, "Couldn't retrieve OS version");

			//Get AKU (adoption kit update)
			HKEY keyreg;
			unsigned long result, rsize = 512, rtype = REG_SZ, ka = KEY_ALL_ACCESS;
			ecUnichar keyvalue[512];
			ecStrCpy(keyvalue, "");
			if (RegCreateKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\Versions", 0, NULL, 0, ka, NULL, &keyreg, &result) == ERROR_SUCCESS)
			{
				RegQueryValueEx(keyreg, L"Aku", NULL, &rtype, (PBYTE)keyvalue, &rsize);
				RegCloseKey(keyreg);
			}
			if (ecStrLen(keyvalue))
			{
				char ckeyvalue[512];
				ecStrCpy(ckeyvalue, keyvalue);
				ecExWriteLog(fileobj, "AKU: ", false);
				ecExWriteLog(fileobj, ckeyvalue);
			}
			else
				ecExWriteLog(fileobj, "Couldn't determine AKU");

			//Display test
			dspo = (EC_DISPLAYOBJ *)ecDspCreate();
			if (dspo)
			{
				unsigned long orientation;
				long resdata[2];
				ecExWriteLog(fileobj, "Testing fullscreen configuration");
				ecDspInfNativeResolution(dspo, resdata);
				orientation = ecDspInfGetOrientation(winhandle);
				ecExWriteLog(fileobj, "Found resolution and orientation settings (width, height, orientation): ", false);
				ecExWriteLog(fileobj, resdata[0], false);
				ecExWriteLog(fileobj, ", ", false);
				ecExWriteLog(fileobj, resdata[1], false);
				ecExWriteLog(fileobj, ", ", false);
				ecExWriteLog(fileobj, orientation);

				//Native orientation
				DEVMODE_IMP_X DevMode;
				ecMemSet(&DevMode, 0, sizeof(DevMode));
				DevMode.dmSize   = sizeof(DevMode);
				DevMode.dmFields = DM_DISPLAYORIENTATION;
				if (ChangeDisplaySettingsEx(NULL, &DevMode, NULL, CDS_TEST, NULL) == DISP_CHANGE_SUCCESSFUL)
				{
					if (DevMode.dmDisplayOrientation == DMDO_90)
						orientation = 90;
					else if (DevMode.dmDisplayOrientation == DMDO_180)
						orientation = 180;
					else if (DevMode.dmDisplayOrientation == DMDO_270)
						orientation = 270;
					else if (DevMode.dmDisplayOrientation == DMDO_0)
						orientation = 0;
					else
						orientation = 45;
					ecExWriteLog(fileobj, "Orientation (uncompensated): ", false);
					ecExWriteLog(fileobj, orientation);
				}
				else
					ecExWriteLog(fileobj, "Couldn't test for orientation (no compensation)");

				//Start
				if (LoadLibrary(L"PhysicsEngine.dll"))
					ecExWriteLog(fileobj, "PhysicsEngine.dll found");
				ecExWriteLog(fileobj, "Display created, analyzing display methods...\r\n");

				//Direct Draw
				ecExWriteLog(fileobj, "Direct Draw:");
				if (dspo->dll_ddraw == NULL)
					dspo->dll_ddraw = LoadLibrary(TEXT("ddraw.dll"));
				if (dspo->dll_ddraw)
				{
					ecExWriteLog(fileobj, "DLL opened successfully, importing methods");
					dspo->func_ddrawcreate = (FUNC_DIRECTDRAWCREATE)GetProcAddress(dspo->dll_ddraw, TEXT("DirectDrawCreate"));
					if (dspo->func_ddrawcreate)
					{
						LONG cresult = dspo->func_ddrawcreate(NULL, (LPUNKNOWN *)&dspo->ddraw, NULL);
						if (cresult == DD_OK)
						{
							if (dspo->ddraw != NULL)
							{
								HRESULT hr;
								DDCAPS ddcaps, ddhelcaps;
								LPDIRECTDRAWCLIPPER dclipper;
								ecExWriteLog(fileobj, "DirectDraw object created");
								hr = dspo->ddraw->SetCooperativeLevel((HWND)winhandle, DDSCL_NORMAL);
								if (hr != DD_OK)
								{
									ecExWriteLog(fileobj, "Diagnostics aborted, reason: couldn't set cooperative level");
									return(false);
								}
								ddcaps.dwSize = sizeof(DDCAPS);
								ddhelcaps.dwSize = sizeof(DDCAPS);
								if (dspo->ddraw->GetCaps(&ddcaps, &ddhelcaps) != DD_OK)
								{
									ecExWriteLog(fileobj, "Diagnostics aborted, reason: couldn't get capabilities");
									return(false);
								}
								DDSURFACEDESC ddesc;
								ecMemSet(&ddesc, 0, sizeof(DDSURFACEDESC));
								ddesc.dwSize = sizeof(DDSURFACEDESC);
								ddesc.dwFlags = DDSD_CAPS;
								ddesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
								if (dspo->ddraw->CreateSurface(&ddesc, &dspo->dfrontbuf, NULL) != DD_OK)
								{
									ecExWriteLog(fileobj, "Diagnostics aborted, reason: couldn't create primary surface");
									return(false);
								}
								ecMemSet(&ddesc, 0, sizeof(DDSURFACEDESC));
								ddesc.dwSize = sizeof(DDSURFACEDESC);
								ddesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PITCH;
								ddesc.dwFlags |= DDSD_XPITCH;
								if (dspo->dfrontbuf->GetSurfaceDesc(&ddesc) != DD_OK)
								{
									ecExWriteLog(fileobj, "Diagnostics aborted, reason: couldn't get surface properties");
									return(false);
								}
								ecExWriteLog(fileobj, "Found surface properties (width, height, xpitch, ypitch): ", false);
								ecExWriteLog(fileobj, ddesc.dwWidth, false);
								ecExWriteLog(fileobj, ", ", false);
								ecExWriteLog(fileobj, ddesc.dwHeight, false);
								ecExWriteLog(fileobj, ", ", false);
								ecExWriteLog(fileobj, ddesc.lXPitch, false);
								ecExWriteLog(fileobj, ", ", false);
								ecExWriteLog(fileobj, ddesc.lPitch);
								if (dspo->ddraw->CreateClipper(0, &dclipper, NULL) != DD_OK)
								{
									ecExWriteLog(fileobj, "Diagnostics aborted, reason: couldn't create clipper");
									return(false);
								}
								hr = DD_OK + 1;
								if (dclipper->SetHWnd(0, (HWND)winhandle) == DD_OK)
									if (dspo->dfrontbuf->SetClipper(dclipper) == DD_OK)
										hr = DD_OK;
								dclipper->Release();
								if (hr != DD_OK)
								{
									ecExWriteLog(fileobj, "Diagnostics aborted, reason: set clipper error");
									return(false);
								}
								DDPIXELFORMAT dpixinfo;
								dpixinfo.dwSize = sizeof(DDPIXELFORMAT);
								if (dspo->dfrontbuf->GetPixelFormat(&dpixinfo) == DD_OK)
								{
									ecExWriteLog(fileobj, "Found pixel format (bitcount, masks): ", false);
									ecExWriteLog(fileobj, dpixinfo.dwRGBBitCount, false);
									ecExWriteLog(fileobj, ", ", false);
									ecExWriteLog(fileobj, dpixinfo.dwRBitMask, false);
									ecExWriteLog(fileobj, ", ", false);
									ecExWriteLog(fileobj, dpixinfo.dwGBitMask, false);
									ecExWriteLog(fileobj, ", ", false);
									ecExWriteLog(fileobj, dpixinfo.dwBBitMask);
								}
								else
								{
									ecExWriteLog(fileobj, "Diagnostics aborted, reason: couldn't get pixel format");
									return(false);
								}
							}
							else
							{
								ecExWriteLog(fileobj, "Diagnostics aborted, reason: incorrect DirectDraw object");
								return(false);
							}
						}
						else
						{
							ecExWriteLog(fileobj, "Diagnostics aborted, reason: couldn't create DirectDraw object");
							return(false);
						}
					}
					else
						ecExWriteLog(fileobj, "Can't import DirectDraw function, skipping...");
				}
				else
					ecExWriteLog(fileobj, "Can't import DirectDraw DLL, skipping...");
				ecDspClose(dspo);

				//Raw framebuffer
				RawFrameBufferInfo rfbi;
				HDC hdc = NULL;
				ecExWriteLog(fileobj, "\r\nRaw framebuffer:");
				hdc = GetDC(NULL);
				if (hdc == NULL)
				{
					ecExWriteLog(fileobj, "Diagnostics aborted, reason: couldn't get device context");
					return(false);
				}
				if (ExtEscape(hdc, GETRAWFRAMEBUFFER, 0, NULL, sizeof(RawFrameBufferInfo), (char *)&rfbi) <= 0)
				{
					ReleaseDC(NULL, hdc);
					ecExWriteLog(fileobj, "Diagnostics aborted, reason: error calling ExtEscape");
					return(false);
				}
				ReleaseDC(NULL, hdc);
				ecExWriteLog(fileobj, "Framebuffer opened successfully");
				ecExWriteLog(fileobj, "Found framebuffer properties (width, height, xpitch, ypitch, bitdepth): ", false);
				ecExWriteLog(fileobj, rfbi.cxPixels, false);
				ecExWriteLog(fileobj, ", ", false);
				ecExWriteLog(fileobj, rfbi.cyPixels, false);
				ecExWriteLog(fileobj, ", ", false);
				ecExWriteLog(fileobj, rfbi.cxStride, false);
				ecExWriteLog(fileobj, ", ", false);
				ecExWriteLog(fileobj, rfbi.cyStride, false);
				ecExWriteLog(fileobj, ", ", false);
				ecExWriteLog(fileobj, rfbi.wBPP);
				ecDspClose(dspo);

				//Done
				ecDspDestroy(dspo);
				ecExWriteLog(fileobj, "\r\nDone");
			}
			ecFClose(fileobj);
			return(true);
		}
	#endif
	return(false);
}


///////////////////////////////////////////////////////////////////
// Internal functions
///////////////////////////////////////////////////////////////////

//Try opening DirectDraw
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenDDraw(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_WINMOBILE)
			if (dspo->emulcoords && config->prefermethod != 3)
				return(2);//E_UNSUPPORTED
		#endif
		if (dspo->dll_ddraw == NULL)
			dspo->dll_ddraw = LoadLibrary(TEXT("ddraw.dll"));
		if (dspo->dll_ddraw)
		{
			dspo->func_ddrawcreate = (FUNC_DIRECTDRAWCREATE)GetProcAddress(dspo->dll_ddraw, TEXT("DirectDrawCreate"));
			if (dspo->func_ddrawcreate)
			{
				#if defined(DEVICE_DESKTOP)
					HRESULT cresult = dspo->func_ddrawcreate(NULL, &dspo->ddraw, NULL);
				#else
					LONG cresult = dspo->func_ddrawcreate(NULL, (LPUNKNOWN *)&dspo->ddraw, NULL);
				#endif
				if (cresult == DD_OK)
				{
					if (dspo->ddraw != NULL)
					{
						HRESULT hr;
						DDCAPS ddcaps, ddhelcaps;
						LPDIRECTDRAWCLIPPER dclipper;
						if (config->fullscreen)
						{
							#if defined(DEVICE_WINMOBILE)
								hr = dspo->ddraw->SetCooperativeLevel((HWND)winhandle, DDSCL_NORMAL);
							#else
								hr = dspo->ddraw->SetCooperativeLevel((HWND)winhandle, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);
							#endif
						}
						else
							hr = dspo->ddraw->SetCooperativeLevel((HWND)winhandle, DDSCL_NORMAL);
						if (hr != DD_OK)
							return(3);
						ddcaps.dwSize = sizeof(DDCAPS);
						ddhelcaps.dwSize = sizeof(DDCAPS);
						if (dspo->ddraw->GetCaps(&ddcaps, &ddhelcaps) != DD_OK)
							return(3);
						#if defined(DEVICE_DESKTOP)
							if (config->fullscreen)
							{
								if (dspo->ddraw->SetDisplayMode(config->xres, config->yres, config->bitdepth) != DD_OK)
									return(3);
								if (config->bitdepth <= 8)
								{
									PALETTEENTRY blackentries[256];
									ecMemSet(blackentries, 0, sizeof(PALETTEENTRY) * 256);
									if (dspo->ddraw->CreatePalette(DDPCAPS_8BIT, blackentries, &dspo->dpalette, NULL) != DD_OK)
										return(3);
								}
							}
						#endif
						DDSURFACEDESC ddesc;
						ecMemSet(&ddesc, 0, sizeof(DDSURFACEDESC));
						ddesc.dwSize = sizeof(DDSURFACEDESC);
						ddesc.dwFlags = DDSD_CAPS;
						ddesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
						if (dspo->ddraw->CreateSurface(&ddesc, &dspo->dfrontbuf, NULL) != DD_OK)
							return(3);
						config->xpitch = 0;
						#if defined(DEVICE_WINCE)
							ecMemSet(&ddesc, 0, sizeof(DDSURFACEDESC));
							ddesc.dwSize = sizeof(DDSURFACEDESC);
							ddesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PITCH;
							#if defined(DEVICE_WINMOBILE)
								ddesc.dwFlags |= DDSD_XPITCH;
							#endif
							if (dspo->dfrontbuf->GetSurfaceDesc(&ddesc) != DD_OK)
								return(3);
							config->xres = ddesc.dwWidth;
							config->yres = ddesc.dwHeight;
							#if defined(DEVICE_WINMOBILE)
								config->xpitch = ddesc.lXPitch;
							#endif
							config->ypitch = ddesc.lPitch;
						#else
							ecMemSet(&ddesc, 0, sizeof(DDSURFACEDESC));
							ddesc.dwSize = sizeof(DDSURFACEDESC);
							ddesc.dwFlags = DDSD_PITCH;
							if (dspo->dfrontbuf->GetSurfaceDesc(&ddesc) != DD_OK)
								return(3);
							config->ypitch = ddesc.lPitch;
						#endif
						if (dspo->ddraw->CreateClipper(0, &dclipper, NULL) != DD_OK)
							return(3);
						hr = DD_OK + 1;
						if (dclipper->SetHWnd(0, (HWND)winhandle) == DD_OK)
							if (dspo->dfrontbuf->SetClipper(dclipper) == DD_OK)
								hr = DD_OK;
						dclipper->Release();
						if (hr != DD_OK)
							return(3);
						DDPIXELFORMAT dpixinfo;
						dpixinfo.dwSize = sizeof(DDPIXELFORMAT);
						if (dspo->dfrontbuf->GetPixelFormat(&dpixinfo) == DD_OK)
						{
							switch(dpixinfo.dwRGBBitCount)
							{
								case 1:
								case 2:
								case 4:
								case 8:
									config->bitdepth = (unsigned char)dpixinfo.dwRGBBitCount;
									config->colorchnl = 0;
									break;
								case 16:
									config->bitdepth = 16;
									if (dpixinfo.dwGBitMask == 992)
										config->colorchnl = 5 | (5 << 8) | (5 << 16);
									else
										config->colorchnl = 5 | (6 << 8) | (5 << 16);
									break;
								case 24:
									config->bitdepth = 24;
									config->colorchnl = 8 | (8 << 8) | (8 << 16);
									break;
								case 32:
									config->bitdepth = 32;
									config->colorchnl = 8 | (8 << 8) | (8 << 16);
									break;
							}
							if (config->xpitch == 0)
								config->xpitch = (dpixinfo.dwRGBBitCount + 7) / 8;
						}
						else
							return(3);
						return(0);
					}
					else
						return(3);
				}
				else
					return(3);
			}
			else
				return(4);
		}
		else
			return(4);
	#endif
	return(2);
}

//Try opening GDI
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenGDI(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	#if defined(DEVICE_WIN32)
		dspo->gdiwindc = GetDC((HWND)winhandle);
		if (dspo->gdiwindc)
		{
			dspo->gdidc = CreateCompatibleDC(dspo->gdiwindc);
			if (dspo->gdidc)
			{
				unsigned long bytedepth;
				config->bitdepth = GetDeviceCaps(dspo->gdiwindc, BITSPIXEL);
				if (config->bitdepth != 15 && config->bitdepth != 16 && config->bitdepth != 24 && config->bitdepth != 32)
					return(2);//E_UNSUPPORTED
				bytedepth = (config->bitdepth + 7) / 8;
				if (config->fullscreen)
				{
					config->xres = GetSystemMetrics(SM_CXSCREEN);
					config->yres = GetSystemMetrics(SM_CYSCREEN);
				}
				config->xpitch = bytedepth;
				config->ypitch = config->yres * bytedepth;
				switch(config->bitdepth)
				{
					case 15:
						config->colorchnl = 5 | (5 << 8) | (5 << 16);
						config->bitdepth = 16;
						break;
					case 16:
						config->colorchnl = 5 | (6 << 8) | (5 << 16);
						break;
					case 24:
					case 32:
						config->colorchnl = 8 | (8 << 8) | (8 << 16);
						break;
					default:
						return(3);//E_ERROR
				}
			}
			else
				return(3);//E_ERROR
		}
		else
			return(3);//E_ERROR
		return(0);//E_OK
	#endif
	return(2);//E_UNSUPPORTED
}

//Try opening GAPI
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenGapi(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	#if defined(DEVICE_WINCE)
		#if defined(DEVICE_WINMOBILE)
			if (!dspo->emulcoords && config->prefermethod != 3)
				return(2);//E_UNSUPPORTED
		#endif
		if (dspo->dll_gapi == NULL)
			dspo->dll_gapi = LoadLibrary(TEXT("gx.dll"));
		if (dspo->dll_gapi)
		{
			GXDisplayProperties gxprops;
			int gr;
			dspo->func_gxopendisplay = (FUNC_GXOPENDISPLAY)GetProcAddress(dspo->dll_gapi, TEXT("?GXOpenDisplay@@YAHPAUHWND__@@K@Z"));
			dspo->func_gxclosedisplay = (FUNC_GXCLOSEDISPLAY)GetProcAddress(dspo->dll_gapi, TEXT("?GXCloseDisplay@@YAHXZ"));
			dspo->func_gxgetdisplayproperties = (FUNC_GXGETDISPLAYPROPERTIES)GetProcAddress(dspo->dll_gapi, TEXT("?GXGetDisplayProperties@@YA?AUGXDisplayProperties@@XZ"));
			dspo->func_gxsetviewport = (FUNC_GXSETVIEWPORT)GetProcAddress(dspo->dll_gapi, TEXT("?GXSetViewport@@YAHKKKK@Z"));
			dspo->func_gxsuspend = (FUNC_GXSUSPEND)GetProcAddress(dspo->dll_gapi, TEXT("?GXSuspend@@YAHXZ"));
			dspo->func_gxresume = (FUNC_GXRESUME)GetProcAddress(dspo->dll_gapi, TEXT("?GXResume@@YAHXZ"));
			dspo->func_gxbegindraw = (FUNC_GXBEGINDRAW)GetProcAddress(dspo->dll_gapi, TEXT("?GXBeginDraw@@YAPAXXZ"));
			dspo->func_gxenddraw = (FUNC_GXENDDRAW)GetProcAddress(dspo->dll_gapi, TEXT("?GXEndDraw@@YAHXZ"));
			if (dspo->func_gxopendisplay == NULL)
				return(4);//E_NOENTRY
			if (dspo->func_gxclosedisplay == NULL)
				return(4);//E_NOENTRY
			if (dspo->func_gxgetdisplayproperties == NULL)
				return(4);//E_NOENTRY
			if (dspo->func_gxsetviewport == NULL)
				return(4);//E_NOENTRY
			if (dspo->func_gxsuspend == NULL)
				return(4);//E_NOENTRY
			if (dspo->func_gxresume == NULL)
				return(4);//E_NOENTRY
			if (dspo->func_gxbegindraw == NULL)
				return(4);//E_NOENTRY
			if (dspo->func_gxenddraw == NULL)
				return(4);//E_NOENTRY
			if (config->fullscreen)
				gr = dspo->func_gxopendisplay((HWND)winhandle, GX_FULLSCREEN);
			else
			{
				#if defined(DEVICE_SMARTPHONE) && (DEVICE_WINMOBILE <= 200)
					if (config->hasmenu)
						gr = dspo->func_gxopendisplay((HWND)winhandle, 0);
					else
						gr = dspo->func_gxopendisplay((HWND)winhandle, GX_FULLSCREEN);
				#else
					gr = dspo->func_gxopendisplay((HWND)winhandle, 0);
				#endif
			}
			if (gr == 0)
				return(3);//E_ERROR
			dspo->gapiopen = true;
			gxprops = dspo->func_gxgetdisplayproperties();
			config->xres = gxprops.cxWidth;
			config->yres = gxprops.cyHeight;
			config->xpitch = gxprops.cbxPitch;
			config->ypitch = gxprops.cbyPitch;
			config->bitdepth = (unsigned char)gxprops.cBPP;
			switch(config->bitdepth)
			{
				case 1:
				case 2:
				case 4:
				case 8:
					config->colorchnl = 0;
					break;
				case 16:
					if (gxprops.ffFormat & kfDirect444)
						config->colorchnl = 4 | (4 << 8) | (4 << 16);
					else if (gxprops.ffFormat & kfDirect555)
						config->colorchnl = 5 | (5 << 8) | (5 << 16);
					else
						config->colorchnl = 5 | (6 << 8) | (5 << 16);
					break;
				case 24:
				case 32:
					config->colorchnl = 8 | (8 << 8) | (8 << 16);
					break;
				default:
					return(3);//E_ERROR
			}
			return(0);//E_OK
		}
		else
			return(4);//E_NOENTRY
	#endif
	return(2);//E_UNSUPPORTED
}

//Try opening framebuffer through ExtEscape
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenExtEscape(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	#if defined(DEVICE_WINCE)
		#if defined(DEVICE_WINMOBILE)
			if (dspo->emulcoords && config->prefermethod != 3)
				return(2);//E_UNSUPPORTED
		#endif
		RawFrameBufferInfo rfbi;
		HDC hdc = NULL;
		hdc = GetDC(NULL);
		if (hdc == NULL)
			return(3);//E_ERROR
		if (ExtEscape(hdc, GETRAWFRAMEBUFFER, 0, NULL, sizeof(RawFrameBufferInfo), (char *)&rfbi) <= 0)
		{
			ReleaseDC(NULL, hdc);
			return(3);//E_ERROR
		}
		ReleaseDC(NULL, hdc);
		if (rfbi.cxPixels == 0 || rfbi.cyPixels == 0)
			return(2);//E_UNSUPPORTED
		dspo->extescapeopen = true;
		config->xres = rfbi.cxPixels;
		config->yres = rfbi.cyPixels;
		config->xpitch = rfbi.cxStride;
		config->ypitch = rfbi.cyStride;
		config->bitdepth = (unsigned char)rfbi.wBPP;
		switch(config->bitdepth)
		{
			case 1:
			case 2:
			case 4:
			case 8:
				config->colorchnl = 0;
				break;
			case 16:
				if (rfbi.wFormat & FORMAT_555)
					config->colorchnl = 5 | (5 << 8) | (5 << 16);
				else if (rfbi.wFormat == FORMAT_565)
					config->colorchnl = 5 | (6 << 8) | (5 << 16);
				else
					config->colorchnl = 4 | (4 << 8) | (4 << 16);
				break;
			case 24:
			case 32:
				config->colorchnl = 8 | (8 << 8) | (8 << 16);
				break;
			default:
				return(3);//E_ERROR
		}

		//Check if this method rotates along with the display
		OSVERSIONINFO verinfo;
		ecMemSet(&verinfo, 0, sizeof(OSVERSIONINFO));
		verinfo.dwOSVersionInfoSize = sizeof(verinfo);
		if (GetVersionEx(&verinfo) == TRUE)
		{
			if (verinfo.dwMajorVersion >= 6 || (verinfo.dwMajorVersion == 5 && verinfo.dwMinorVersion > 2))
				dspo->rotatedextescape = true;
			if (verinfo.dwMajorVersion == 5 && verinfo.dwMinorVersion == 2)
			{
				HMODULE hmodule = LoadLibrary(L"PhysicsEngine.dll");
				if (hmodule)
				{
					FreeLibrary(hmodule);
					dspo->rotatedextescape = true;
				}
			}
		}
		return(0);//E_OK
	#endif
	return(2);//E_UNSUPPORTED
}

//Try opening framebuffer of Linux
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenLinuxFramebuffer(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	#if defined(DEVICE_LINUX)
		struct stat sbuf;
		if (stat("/dev/fb0", &sbuf) != 0)
			dspo->framebuf_ptr = open("/dev/graphics/fb0", O_RDWR);
		else
			dspo->framebuf_ptr = open("/dev/fb0", O_RDWR);
		if (dspo->framebuf_ptr < 0)
		{
			dspo->framebuf_ptr = 0;
			return(4);
		}
		struct fb_var_screeninfo screeninfo;
		screeninfo.bits_per_pixel = 0;
		ioctl(dspo->framebuf_ptr, FBIOGET_VSCREENINFO, &screeninfo);
		config->xres = screeninfo.xres;
		config->yres = screeninfo.yres;
		config->bitdepth = screeninfo.bits_per_pixel;
		config->xpitch = config->bitdepth / 8;
		config->ypitch = config->xpitch * config->xres;
		switch(config->bitdepth)
		{
			case 1:
			case 2:
			case 4:
			case 8:
				config->colorchnl = 0;
				break;
			case 16:
				config->colorchnl = 5 | (6 << 8) | (5 << 16);
				break;
			case 24:
			case 32:
				config->colorchnl = 8 | (8 << 8) | (8 << 16);
				break;
			default:
				close(dspo->framebuf_ptr);
				dspo->framebuf_ptr = 0;
				return(3);//E_ERROR
		}
		dspo->framebufsize = config->ypitch * config->yres;
		dspo->framebuffer = (unsigned char *)mmap(0, dspo->framebufsize, PROT_READ | PROT_WRITE, MAP_SHARED, dspo->framebuf_ptr, 0);
		if (dspo->framebuffer == NULL)
		{
			close(dspo->framebuf_ptr);
			dspo->framebuf_ptr = 0;
			return(3);//E_ERROR
		}
		return(0);//E_OK
	#endif
	return(2);//E_UNSUPPORTED
}

//Try opening Xlib framebuffer of Linux
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenLinuxXBuffer(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	#if defined(DEVICE_X11)
		ecNativetype *wnd = (ecNativetype *)winhandle;
		long screennumber = DefaultScreen((Display *)wnd[0]);
		bool foundxv = false;
		if (config->fullscreen)
		{
			/*#if !defined(DEVICE_EMBEDDEDLINUX)
				int modecount, mctr;
				XF86VidModeModeInfo **modes;
				XF86VidModeModeInfo *adjmode = NULL;
				XF86VidModeGetAllModeLines((Display *)wnd[0], 0, &modecount, &modes);
				for (mctr = 0; mctr < modecount; mctr++)
					if (modes[mctr]->hdisplay == config->xres && modes[mctr]->vdisplay == config->yres)
					{
						adjmode = modes[mctr];
						break;
					}
				if (adjmode)
				{
					XF86VidModeSwitchToMode((Display *)wnd[0], 0, adjmode);
					XF86VidModeSwitchToMode((Display *)wnd[0], 0, adjmode);
				}
				else
					return(3);
			#endif*/
			config->xres = DisplayWidth((Display *)wnd[0], screennumber);
			config->yres = DisplayHeight((Display *)wnd[0], screennumber);
		}

		Visual *vis = DefaultVisual((Display *)wnd[0], screennumber);
		if (vis->blue_mask == 0xFF && vis->green_mask == 0xFF00 && vis->red_mask == 0xFF0000)
		{
			config->bitdepth = 32;
			config->colorchnl = 8 | (8 << 8) | (8 << 16);
		}
		else if (vis->blue_mask == 31 && vis->green_mask == 2016 && vis->red_mask == 63488)
		{
			config->bitdepth = 16;
			config->colorchnl = 5 | (6 << 8) | (5 << 16);
		}
		else if (vis->blue_mask == 31 && vis->green_mask == 992 && vis->red_mask == 31744)
		{
			config->bitdepth = 16;
			config->colorchnl = 5 | (5 << 8) | (5 << 16);
		}
		else
			return(2);
		config->xpitch = config->bitdepth / 8;
		config->ypitch = config->xres * config->xpitch;
		if (config->ypitch % 4 != 0)
			config->ypitch += 4 - config->ypitch % 4;
		dspo->framebufsize = (config->yres << 16) | config->xres;
		dspo->framebuffer = (unsigned char *)ecMemAlloc(config->yres * config->ypitch);
		if (dspo->framebuffer == NULL)
			return(1);

		dspo->xv_port = 0;
		if (config->prefermethod == 3)
		{
			XvAdaptorInfo *xvai;
			unsigned int adaptor, adaptorcount = 0;
			XvQueryAdaptors((Display *)wnd[0], DefaultRootWindow((Display *)wnd[0]), &adaptorcount, &xvai);
			foundxv = false;
			for (adaptor = 0; adaptor < adaptorcount; adaptor++) //Foundxv will be false if no adaptor is found
			{
				XvImageFormatValues *ifv;
				int format, formatcount = 0;
				ifv = XvListImageFormats((Display *)wnd[0], xvai[adaptor].base_id, &formatcount);
				for (format = 0; format < formatcount; format++)
				{
					if ((config->bitdepth == 16 && ifv[format].id == 0x36315652) || (config->bitdepth != 16 && ifv[format].id == 0x32335652))
					{
						if (XvGrabPort((Display *)wnd[0], xvai[adaptor].base_id, CurrentTime) == 0)
						{
							dspo->xv_port = xvai[adaptor].base_id;
							dspo->xv_formatid = ifv[format].id;
							foundxv = true;
							break;
						}
					}
				}
				if (ifv)
					XFree(ifv);
				if (foundxv)
					break;
			}
			if (xvai)
				XvFreeAdaptorInfo(xvai);
		}
		if (foundxv)
		{
			XvSetPortAttribute((Display *)wnd[0], dspo->xv_port, XInternAtom((Display*)wnd[0], "XV_AUTOPAINT_COLORKEY", False), 1);
			dspo->xvimg_overlay = XvCreateImage((Display *)wnd[0], dspo->xv_port, dspo->xv_formatid, NULL, config->xres, config->yres);
			if (dspo->xvimg_overlay)
			{
				dspo->xvimg_overlay->data = (char *)dspo->framebuffer;
				XSync((Display*)wnd[0], False);
			}
			else
				foundxv = false;
		}
		if (!foundxv)
		{
			dspo->ximg_framebuf = XCreateImage((Display *)wnd[0], vis, DefaultDepth((Display *)wnd[0], screennumber), ZPixmap, 0, (char *)dspo->framebuffer, config->xres, config->yres, 32, config->ypitch);
			if (dspo->ximg_framebuf == NULL)
			{
				ecMemFree(dspo->framebuffer);
				return(4);
			}
		}
		dspo->bitdepth = config->bitdepth;
		ecMemSet(dspo->framebuffer, 0, config->yres * config->ypitch);
		return(0);
	#endif
	return(2);//E_UNSUPPORTED
}

//Find the pixel offset for framebuffer access
unsigned long eciDspGetSymbianFrameBufOffs(EC_DISPLAYOBJ *dspo)
{
	#if defined(DEVICE_SYMBIAN) && !defined(DEVICE_SYMBIANEMU) && (DEVICE_SYMBIAN < 900)
		unsigned char *framebuf;
		TScreenInfoV01 screeninfo;
		TPckg<TScreenInfoV01> sinfo(screeninfo);
		UserSvr::ScreenInfo(sinfo);
		framebuf = (unsigned char *)(screeninfo.iScreenAddress);
		if (framebuf)
		{
			TBuf8<128> scanline;
			unsigned char *scanlinemem = NULL;
			unsigned long ctr, ctr2;
			dspo->screendevice->GetScanLine(scanline, TPoint(0, 0), 128, dspo->dispmode);
			scanlinemem = (unsigned char *)&scanline[0];
			for (ctr = 0; ctr < 128; ctr++)
				for (ctr2 = 0; ctr2 < 128; ctr2++)
				{
					if (scanlinemem[ctr2] != framebuf[ctr + ctr2])
						break;
					if (ctr2 == 127)
						return(ctr);
				}
		}
		return(32);
	#endif
	return(0);
}

//Try opening DirectScreenAccess
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenDirectScreenAccess(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	#if defined(DEVICE_SYMBIAN) && (DEVICE_SYMBIAN >= 610)
		dspo->dsa = new ClassDSA((RWindow *)winhandle);
		if (dspo->dsa)
		{
			if (dspo->dsa->ConstructL())
			{
				TSize vsize;
				dspo->dsa->StartDSA();
				dspo->dispmode = dspo->dsa->iScreenDevice->DisplayMode();
				vsize = dspo->dsa->iScreenDevice->SizeInPixels();
				config->bitdepth = 0;
				config->xres = vsize.iWidth;
				config->yres = vsize.iHeight;
				switch(dspo->dispmode)
				{
					case EColor256:
						config->bitdepth = 8;
						config->colorchnl = 0;
						break;
					case EColor4K:
						config->bitdepth = 16;
						config->colorchnl = 4 | (4 << 8) | (4 << 16);
						break;
					case EColor64K:
						config->bitdepth = 16;
						config->colorchnl = 5 | (6 << 8) | (5 << 16);
						break;
					case EColor16M:
						config->bitdepth = 24;
						config->colorchnl = 8 | (8 << 8) | (8 << 16);
						break;
					case EColor16MU:
					case EColor16MA:
						config->bitdepth = 32;
						config->colorchnl = 8 | (8 << 8) | (8 << 16);
						break;
					default:
						break;
				}
				config->xpitch = config->bitdepth / 8;
				config->ypitch = config->xpitch * config->xres;
			}
			else
				return(3);//E_ERROR
		}
		else
			return(3);//E_ERROR
		return(0);//E_OK
	#endif
	return(2);//E_UNSUPPORTED
}

//Try opening the Symbian bit context
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenBitGc(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	#if defined(DEVICE_SYMBIAN)
		TSize vsize;
		dspo->dispmode = CEikonEnv::Static()->ScreenDevice()->DisplayMode();
		dspo->screendevice = CFbsScreenDevice::NewL(_L("scdv"), dspo->dispmode);
		if (dspo->screendevice == NULL)
			return(1);//E_NOMEMORY
		dspo->screendevice->CreateContext(dspo->bitgc);
		if (dspo->bitgc == NULL)
			return(3);//E_NOMEMORY
		dspo->bitgc->SetOrientation(CFbsBitGc::EGraphicsOrientationNormal);
		vsize = dspo->screendevice->SizeInPixels();
		config->bitdepth = 0;
		config->xres = vsize.iWidth;
		config->yres = vsize.iHeight;
		switch(dspo->dispmode)
		{
			case EColor256:
				config->bitdepth = 8;
				config->colorchnl = 0;
				break;
			case EColor4K:
				config->bitdepth = 16;
				config->colorchnl = 4 | (4 << 8) | (4 << 16);
				break;
			case EColor64K:
				config->bitdepth = 16;
				config->colorchnl = 5 | (6 << 8) | (5 << 16);
				break;
			case EColor16M:
				config->bitdepth = 24;
				config->colorchnl = 8 | (8 << 8) | (8 << 16);
				break;
			case EColor16MU:
			case EColor16MA:
				config->bitdepth = 32;
				config->colorchnl = 8 | (8 << 8) | (8 << 16);
				break;
			default:
				break;
		}
		config->xpitch = config->bitdepth / 8;
		config->ypitch = config->xpitch * config->xres;
		dspo->bufoffs = eciDspGetSymbianFrameBufOffs(dspo);
		return(0);//E_OK
	#endif
	return(2);//E_UNSUPPORTED
}

//Try opening the Symbian framebuffer
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenSymbianFramebuffer(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	#if defined(DEVICE_SYMBIAN) && !defined(DEVICE_SYMBIANEMU) && (DEVICE_SYMBIAN < 900)
		TSize vsize;
		dspo->dispmode = CEikonEnv::Static()->ScreenDevice()->DisplayMode();
		dspo->screendevice = CFbsScreenDevice::NewL(_L("scdv"), dspo->dispmode);
		if (dspo->screendevice == NULL)
			return(1);//E_NOMEMORY
		vsize = dspo->screendevice->SizeInPixels();
		config->bitdepth = 0;
		config->xres = vsize.iWidth;
		config->yres = vsize.iHeight;
		switch(dspo->dispmode)
		{
			case EColor256:
				config->bitdepth = 8;
				config->colorchnl = 0;
				break;
			case EColor4K:
				config->bitdepth = 16;
				config->colorchnl = 4 | (4 << 8) | (4 << 16);
				break;
			case EColor64K:
				config->bitdepth = 16;
				config->colorchnl = 5 | (6 << 8) | (5 << 16);
				break;
			case EColor16M:
				config->bitdepth = 24;
				config->colorchnl = 8 | (8 << 8) | (8 << 16);
				break;
			case EColor16MU:
			case EColor16MA:
				config->bitdepth = 32;
				config->colorchnl = 8 | (8 << 8) | (8 << 16);
				break;
			default:
				break;
		}
		config->xpitch = config->bitdepth / 8;
		config->ypitch = config->xpitch * config->xres;
		dspo->bufoffs = eciDspGetSymbianFrameBufOffs(dspo);
		return(0);//E_OK
	#endif
	return(2);//E_UNSUPPORTED
}

//Try opening the Mac Quartz context
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenMacQuartz(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	#if defined(DEVICE_MAC)
		if (!MacOpenDisplay((ECOREOBJ)winhandle, config->xres, config->yres))
			return(3);
		config->bitdepth = 32;
		config->colorchnl = 8 | (8 << 8) | (8 << 16);
		config->xpitch = config->bitdepth / 8;
		config->ypitch = config->xpitch * config->xres;
		dspo->bitdepth = config->bitdepth;
		return(0);//E_OK
	#endif
	return(2);//E_UNSUPPORTED
}

//Try using the EGL surface lock API
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenEglSurface(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	#if defined(DEVICE_OPENKODE)
		ecNativetype *winhandlearr = (ecNativetype *)winhandle;
		if (dspo->locksurface == NULL)
		{
			dspo->locksurface = (PFNEGLLOCKSURFACEKHR)eglGetProcAddress("eglLockSurfaceKHR");
			if (dspo->locksurface == NULL)
				return(4);//E_NOENTRY
		}
		if (dspo->unlocksurface == NULL)
		{
			dspo->unlocksurface = (PFNEGLUNLOCKSURFACEKHR)eglGetProcAddress("eglUnlockSurfaceKHR");
			if (dspo->unlocksurface == NULL)
				return(4);//E_NOENTRY
		}
		dspo->windowsurface = eglCreateWindowSurface(winhandlearr[0], winhandlearr[2], (EGLNativeWindowType)winhandlearr[1], KD_NULL);
		if (dspo->windowsurface == EGL_NO_SURFACE)
			return(3);
		if (dspo->locksurface(winhandlearr[0], dspo->windowsurface, KD_NULL) != EGL_FALSE)
		{
			EGLint origin, bpp, alphachannel;
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_WIDTH, (EGLint *)&config->xres);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_HEIGHT, (EGLint *)&config->yres);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_BITMAP_ORIGIN_KHR, &origin);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_BITMAP_PITCH_KHR, (EGLint *)&config->ypitch);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_BITMAP_PIXEL_SIZE_KHR, &bpp);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_BITMAP_PIXEL_ALPHA_OFFSET_KHR, &alphachannel);
			if (alphachannel > 0 && bpp == 32)
				dspo->displayalpha = true;
			else
				dspo->displayalpha = false;
			if (origin == EGL_LOWER_LEFT_KHR)
			{
				dspo->flippedorigin = true;
				config->ypitch = -config->ypitch;
			}
			else
				dspo->flippedorigin = false;
			dspo->unlocksurface(winhandlearr[0], dspo->windowsurface);
			config->bitdepth = bpp;
			if (bpp == 16)
				config->colorchnl = 5 | (6 << 8) | (5 << 16); //Hard coded to 5-6-5 format
			else
				config->colorchnl = 8 | (8 << 8) | (8 << 16);
			config->xpitch = config->bitdepth / 8;
		}
		else
			return(4);//E_NOENTRY
		return(0);//E_OK
	#endif
	return(2);//E_UNSUPPORTED
}

//Initialize the display from the android interface
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
ecErr eciDspOpenAndroid(EC_DISPLAYOBJ *dspo, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
    #if defined(DEVICE_ANDROID)
	int width = Call_EdgelibView_getWidth();
	int height = Call_EdgelibView_getHeight();
	char msg[32];
	config->xres = width;
	config->yres = height;
	config->fullscreen = true;
	config->bitdepth = 32;
	config->colorchnl = 8 | (8 << 8) | (8 << 16);
	config->xpitch = 4;
	config->ypitch = width * 4;
	dspo->backbuffer = defaultbuffer;
	dspo->framebufsize = (config->yres << 16) | config->xres;
	return 0;
    #endif
    return(2);//E_UNSUPPORTED
}

///////////////////////////////////////////////////////////////////
// Display API
///////////////////////////////////////////////////////////////////

//Create display object
ECOREOBJ ecDspCreate(void)
{
	EC_DISPLAYOBJ *dspo = new EC_DISPLAYOBJ;
	if (dspo)
	{
		#if defined(DEVICE_WIN32)
			dspo->dll_ddraw = NULL;
			dspo->ddraw = NULL;
			#if defined(DEVICE_WINCE)
				dspo->dll_gapi = NULL;
				dspo->gapiopen = false;
				dspo->extescapeopen = false;
				dspo->emulcoords = false;
				dspo->rotatedextescape = false;
				#if (DEVICE_WINMOBILE < 500)
					if (FindResource(GetModuleHandle(NULL), TEXT("HI_RES_AWARE"), TEXT("CEUX")) == NULL)
						dspo->emulcoords = true;
				#endif
			#endif
		#elif defined(DEVICE_SYMBIAN)
			dspo->dsa = NULL;
			dspo->screendevice = NULL;
			dspo->bitgc = NULL;
		#elif defined(DEVICE_LINUX)
			#if defined(DEVICE_ANDROID)
				dspo->backbuffer = NULL;
			#endif
			#if defined(DEVICE_X11)
				dspo->ximg_framebuf = NULL;
				dspo->xvimg_overlay = NULL;
				dspo->xv_port = 0;
			#endif
			dspo->framebuf_ptr = 0;
			dspo->framebuffer = NULL;
		#elif defined(DEVICE_OPENKODE)
			dspo->locksurface = NULL;
			dspo->unlocksurface = NULL;
			dspo->windowsurface = EGL_NO_SURFACE;
		#endif
		dspo->winhandle = NULL;
		dspo->unlockrefresh = false;
	}
	return(dspo);
}

//Destroy display object
void ecDspDestroy(ECOREOBJ dspobj)
{
	EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_WINCE)
			if (dspo->dll_gapi)
				FreeLibrary(dspo->dll_gapi);
		#endif
		//if (dspo->dll_ddraw)
		//	FreeLibrary(dspo->dll_ddraw);
	#endif
	delete dspo;
}

//Open display device
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY
//config->prefermethod: 0 = Framebuffer, 1 = DirectDraw/DSA, 2 = GAPI
ecErr ecDspOpen(ECOREOBJ dspobj, ecNativetype winhandle, EC_DISPLAYCONFIG *config)
{
	//eciDspWindowsMobileDiag(winhandle);
	//return(3);
	EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
	ecErr errcode = 0;
	dspo->winhandle = winhandle;
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_DESKTOP)
			dspo->dpalette = NULL;
		#else
			dspo->gapiopen = false;
			dspo->extescapeopen = false;
		#endif
		dspo->ddraw = NULL;
		dspo->dfrontbuf = NULL;
		dspo->gdidc = NULL;
		#if defined(DEVICE_WINCE)
			if (config->prefermethod == 0)
				errcode = eciDspOpenExtEscape(dspo, winhandle, config);
			if (config->prefermethod == 1)
				errcode = eciDspOpenDDraw(dspo, winhandle, config);
			if (config->prefermethod == 2)
				errcode = eciDspOpenGDI(dspo, winhandle, config);
			if (config->prefermethod == 3)
				errcode = eciDspOpenGapi(dspo, winhandle, config);
			#if defined(DEVICE_WINMOBILE)
				if (config->prefermethod == 4)
				{
					if (dspo->emulcoords)
					{
						errcode = eciDspOpenExtEscape(dspo, winhandle, config);
						if (errcode != 0)
						{
							errcode = eciDspOpenDDraw(dspo, winhandle, config);
							if (errcode == 0)
								config->prefermethod = 1;
						}
						else
							config->prefermethod = 0;
					}
					else
					{
						errcode = eciDspOpenGapi(dspo, winhandle, config);
						if (errcode == 0)
							config->prefermethod = 3;
					}
				}
			#endif
		#else
			if (config->prefermethod == 1)
				errcode = eciDspOpenDDraw(dspo, winhandle, config);
			else if (config->prefermethod == 2)
				errcode = eciDspOpenGDI(dspo, winhandle, config);
			else
				errcode = 2;//E_UNSUPPORTED
		#endif
		#if defined(DEVICE_WINCE)
			if (errcode == 0 && !config->fullscreen)
			{
				RECT wrc;
				GetWindowRect((HWND)winhandle, &wrc);
				if (config->prefermethod == 3) //GAPI
				{
					#if defined(DEVICE_WINMOBILE)
						if (!config->hasmenu)
							wrc.bottom = GetSystemMetrics(SM_CYSCREEN);
					#endif
					dspo->func_gxsetviewport(wrc.top, wrc.bottom - wrc.top, 0, 0);
				}
			}
		#endif
	#elif defined(DEVICE_SYMBIAN)
		dspo->screendevice = NULL;
		dspo->bitgc = NULL;
		dspo->dsa = NULL;
		if (config->prefermethod == 0)
			errcode = eciDspOpenSymbianFramebuffer(dspo, winhandle, config);
		else if (config->prefermethod == 1)
			errcode = eciDspOpenDirectScreenAccess(dspo, winhandle, config);
		else
			errcode = eciDspOpenBitGc(dspo, winhandle, config);
	#elif defined(DEVICE_LINUX)
		if (config->prefermethod == 0)
			errcode = eciDspOpenLinuxFramebuffer(dspo, winhandle, config);
		else if (config->prefermethod == 4)
			errcode = eciDspOpenAndroid(dspo, winhandle, config);
		#if defined(DEVICE_X11)	
			else if (config->prefermethod == 2) // XImage
				errcode = eciDspOpenLinuxXBuffer(dspo, winhandle, config);
			else if (config->prefermethod == 3) // Xv
				errcode = eciDspOpenLinuxXBuffer(dspo, winhandle, config);
		#endif
		else
			errcode = 2;//E_UNSUPPORTED
	#elif defined(DEVICE_MAC)
		if (config->prefermethod == 2)
			errcode = eciDspOpenMacQuartz(dspo, winhandle, config);
		else
			errcode = 2;
	#elif defined(DEVICE_OPENKODE)
		if (config->prefermethod == 0)
			errcode = eciDspOpenEglSurface(dspo, winhandle, config);
		else
			errcode = 2;//E_UNSUPPORTED
	#else
		errcode = 2;//E_UNSUPPORTED
	#endif
	if (errcode)
		ecDspClose(dspo);
	return(errcode);
}

//Close display device
void ecDspClose(ECOREOBJ dspobj)
{
	EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_WINCE)
			if (dspo->dll_gapi)
				if (dspo->gapiopen)
				{
					dspo->func_gxclosedisplay();
					dspo->gapiopen = false;
				}
			if (dspo->extescapeopen)
				dspo->extescapeopen = false;
		#endif
		if (dspo->dll_ddraw)
		{
			if (dspo->ddraw)
			{
				if (dspo->dfrontbuf)
					dspo->dfrontbuf->Release();
				#if defined(DEVICE_DESKTOP)
					if (dspo->dpalette)
						dspo->dpalette->Release();
				#endif
				dspo->ddraw->SetCooperativeLevel((HWND)dspo->winhandle, DDSCL_NORMAL);
				dspo->ddraw->Release();
				dspo->ddraw = NULL;
			}
		}
		else if (dspo->gdidc)
		{
			DeleteDC(dspo->gdidc);
			dspo->gdidc = NULL;
		}
	#elif defined(DEVICE_SYMBIAN)
		#if (DEVICE_SYMBIAN >= 610)
			if (dspo->dsa)
			{
				delete dspo->dsa;
				dspo->dsa = NULL;
			}
		#endif
		if (dspo->screendevice)
		{
			if (dspo->bitgc)
			{
				delete dspo->bitgc;
				dspo->bitgc = NULL;
			}
			delete dspo->screendevice;
			dspo->screendevice = NULL;
		}
	#elif defined(DEVICE_LINUX)
		#if defined(DEVICE_X11)
			if (dspo->ximg_framebuf)
			{
				XDestroyImage(dspo->ximg_framebuf);
				dspo->ximg_framebuf = NULL;
				dspo->framebuffer = NULL;
			}
			if (dspo->xvimg_overlay)
			{
				XFree(dspo->xvimg_overlay);
				dspo->xvimg_overlay = NULL;
			}
			if (dspo->xv_port)
			{
				ecNativetype *wnd = (ecNativetype *)dspo->winhandle;
				long screennumber = DefaultScreen((Display *)wnd[0]);
				XvUngrabPort((Display *)wnd[0], dspo->xv_port, CurrentTime);

				//Should fill the screen with a black color, xvideo shows a green screen by default (but this doesn't work)
				//XSetForeground((Display *)wnd[0], DefaultGC((Display *)wnd[0], screennumber), BlackPixel((Display *)wnd[0], screennumber));
				//XSetBackground((Display *)wnd[0], DefaultGC((Display *)wnd[0], screennumber), BlackPixel((Display *)wnd[0], screennumber));
				//XFillRectangle((Display *)wnd[0], *(Window *)wnd[1], DefaultGC((Display *)wnd[0], screennumber), 0, 0, 800, 480);
				dspo->xv_port = 0;
			}
		#endif
		if (dspo->framebuf_ptr > 0)
		{
			if (dspo->framebuffer)
			{
 				munmap(dspo->framebuffer, dspo->framebufsize);
				dspo->framebuffer = NULL;
			}
			close(dspo->framebuf_ptr);
			dspo->framebuf_ptr = 0;
		}
	#elif defined(DEVICE_OPENKODE)
		if (dspo->windowsurface != EGL_NO_SURFACE)
		{
			ecNativetype *winhandlearr = (ecNativetype *)dspo->winhandle;
			eglDestroySurface(winhandlearr[0], dspo->windowsurface);
			dspo->windowsurface = EGL_NO_SURFACE;
		}
	#endif
	dspo->winhandle = NULL;
}

//Lock display device
void *ecDspLock(ECOREOBJ dspobj, long *lpitch, bool checkonly)
{
	EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
	dspo->unlockrefresh = !checkonly;
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_WINCE)
			if (dspo->dll_gapi)
				if (dspo->gapiopen)
					return(dspo->func_gxbegindraw());
			if (dspo->extescapeopen)
			{
				dspo->framelockdc = GetDC(NULL);
				if (dspo->framelockdc)
				{
					RawFrameBufferInfo rfbi;
					if (ExtEscape(dspo->framelockdc, GETRAWFRAMEBUFFER, 0, NULL, sizeof(RawFrameBufferInfo), (char *)&rfbi) > 0)
						return(rfbi.pFramePointer);
				}
				return(NULL);
			}
		#endif
		if (dspo->dll_ddraw)
			if (dspo->ddraw)
			{
				HRESULT hr;
				DDSURFACEDESC ddesc;
				ddesc.dwSize = sizeof(ddesc);
				hr = dspo->dfrontbuf->Lock(NULL, &ddesc, DDLOCK_WAIT, NULL);
				if (FAILED(hr))
				{
					if (hr == DDERR_SURFACELOST)
						dspo->dfrontbuf->Restore();
					return(NULL);
				}
				if (lpitch)
				{
					#if defined(DEVICE_WINMOBILE)
						lpitch[0] = ddesc.lXPitch;
					#else
						lpitch[0] = ddesc.ddpfPixelFormat.dwRGBBitCount / 8;
					#endif
					lpitch[1] = ddesc.lPitch;
				}
				return(ddesc.lpSurface);
			}
	#elif defined(DEVICE_SYMBIAN) && !defined(DEVICE_SYMBIANEMU) && (DEVICE_SYMBIAN < 900)
		if (dspo->screendevice)
		{
			TScreenInfoV01 screeninfo;
			TPckg<TScreenInfoV01> sinfo(screeninfo);
			UserSvr::ScreenInfo(sinfo);
			return((unsigned char *)(screeninfo.iScreenAddress) + dspo->bufoffs);
		}	
	#elif defined(DEVICE_LINUX)
		#if defined(DEVICE_ANDROID)
		    if (dspo->backbuffer) 
		    {
			lpitch[0] = 4;
			lpitch[1] = 4 * dspo->backbuffer->width;
			return(dspo->backbuffer->lock(android_global_instance->jenv));
		    }
		#endif
		return(dspo->framebuffer);
	#elif defined(DEVICE_OPENKODE)
		ecNativetype *winhandlearr = (ecNativetype *)dspo->winhandle;
		EGLint lockptr;
		if (dspo->locksurface(winhandlearr[0], dspo->windowsurface, KD_NULL) != EGL_FALSE)
		{
			EGLint width, height, bpp;//, origin
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_WIDTH, &width);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_HEIGHT, &height);
			//eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_BITMAP_ORIGIN_KHR, &origin);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_BITMAP_PITCH_KHR, (EGLint *)&lpitch[1]);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_BITMAP_POINTER_KHR, &lockptr);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_BITMAP_PIXEL_SIZE_KHR, &bpp);
			if (dspo->flippedorigin/*origin == EGL_LOWER_LEFT_KHR*/)
			{
				lockptr += lpitch[1] * (height - 1);
				lpitch[1] = -lpitch[1];
			}
			lpitch[0] = bpp / 8;
			if (width == 0 || height == 0)
			{
				dspo->unlocksurface(winhandlearr[0], dspo->windowsurface);
				return(NULL);
			}
			return((unsigned char *)lockptr);
		}
	#endif
	return(NULL);
}

//Unlock display device
void ecDspUnlock(ECOREOBJ dspobj)
{
	EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_WINCE)
			if (dspo->dll_gapi)
				if (dspo->gapiopen)
					dspo->func_gxenddraw();
			if (dspo->extescapeopen)
				ReleaseDC(NULL, dspo->framelockdc);
		#endif
		if (dspo->dll_ddraw)
			if (dspo->ddraw)
			{
				dspo->dfrontbuf->Unlock(NULL);
				return;
			}
	#elif defined(DEVICE_SYMBIAN) && !defined(DEVICE_SYMBIANEMU) && (DEVICE_SYMBIAN < 900)
		if (dspo->unlockrefresh)
		{
			TRawEvent redraw;
			redraw.Set(TRawEvent::ERedraw);
			dspo->screendevice->Update();
			UserSvr::AddEvent(redraw);
		}
	#elif defined(DEVICE_LINUX)
		#if defined(DEVICE_ANDROID)
			if (dspo->backbuffer) 
			{
				dspo->backbuffer->unlock(android_global_instance->jenv);
				return;
			}
		#endif
		#if defined(DEVICE_X11)
			if (dspo->unlockrefresh)
			{
				if (dspo->xvimg_overlay)
				{
					ecNativetype *wnd = (ecNativetype *)dspo->winhandle;
					long screennumber = DefaultScreen((Display *)wnd[0]);
					XvPutImage((Display *)wnd[0], dspo->xv_port, *(Window *)wnd[1], DefaultGC((Display *)wnd[0], screennumber), dspo->xvimg_overlay, 0, 0, dspo->framebufsize & 0xffff, dspo->framebufsize >> 16, 0, 0, dspo->framebufsize & 0xffff, dspo->framebufsize >> 16);
				}
				else if (dspo->ximg_framebuf)
				{
					ecNativetype *wnd = (ecNativetype *)dspo->winhandle;
					long screennumber = DefaultScreen((Display *)wnd[0]);
					XPutImage((Display *)wnd[0], *(Window *)wnd[1], DefaultGC((Display *)wnd[0], screennumber), dspo->ximg_framebuf, 0, 0, 0, 0, dspo->framebufsize & 0xFFFF, dspo->framebufsize >> 16);
				}
			}
		#endif
	#elif defined(DEVICE_OPENKODE)
		ecNativetype *winhandlearr = (ecNativetype *)dspo->winhandle;
		if (dspo->displayalpha && dspo->unlockrefresh)
		{
			EGLint height, width, bpp, pitch, lockptr, tctr;
			unsigned long *surfacemem;
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_WIDTH, &width);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_HEIGHT, &height);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_BITMAP_PITCH_KHR, &pitch);
			eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_BITMAP_POINTER_KHR, &lockptr);
			surfacemem = (unsigned long *)lockptr;
			if (surfacemem)
			{
				EGLint ctr;
				for (ctr = 0; ctr < width * height; ctr++)
					surfacemem[ctr / width * pitch / 4 + ctr % width] |= 0xFF << 24;
			}
		}
		if (dspo->unlocksurface(winhandlearr[0], dspo->windowsurface) != EGL_FALSE)
		{
			if (dspo->unlockrefresh)
				eglSwapBuffers(winhandlearr[0], dspo->windowsurface);
		}
	#endif
}

//Wait for vertical blank
void ecDspWaitSync(ECOREOBJ dspobj)
{
	#if defined(DEVICE_WIN32)
		EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
		if (dspo->ddraw)
			dspo->ddraw->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, NULL);
	#endif
}

//Flip native surface to the display
unsigned char ecDspFlipNativeSurface(ECOREOBJ dspobj, ECOREOBJ surfobj, long *pos, long *rect)
{
	EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
	EC_SURFACEOBJ *surfo = (EC_SURFACEOBJ *)surfobj;
	#if defined(DEVICE_WIN32)
		if (dspo->dll_ddraw)
		{
			if (dspo->ddraw)
			{
				RECT src, drc;
				SetRect(&src, rect[0], rect[1], rect[2], rect[3]);
				SetRect(&drc, pos[0], pos[1], pos[0] + rect[2] - rect[0], pos[1] + rect[3] - rect[1]);
				HRESULT hr = dspo->dfrontbuf->Blt(&drc, surfo->surf, &src, DDBLT_WAIT, NULL);
				#if defined(DEVICE_WINMOBILE)
					if (dspo->dfrontbuf->IsLost() == DDERR_SURFACELOST)
						hr = DDERR_SURFACELOST;
				#endif
				if (FAILED(hr))
				{
					if (hr == DDERR_SURFACELOST)
					{
						hr = dspo->dfrontbuf->Restore();
						if (hr == DDERR_WRONGMODE)
							return(2);
					}
					return(1);
				}
			}
		}
		else if (dspo->gdidc)
		{
			SelectObject(surfo->bmpdc, surfo->hbitmap);
			BitBlt(dspo->gdiwindc, pos[0], pos[1], rect[2] - rect[0], rect[3] - rect[1], surfo->bmpdc, rect[0], rect[1], SRCCOPY);
		}
	#elif defined(DEVICE_SYMBIAN)
		#if (DEVICE_SYMBIAN >= 610)
			if (dspo->dsa)
			{
				TRect frc(rect[0], rect[1], rect[2], rect[3]);
				TRawEvent redraw;
				RRegion fregion;
				fregion.Clear();
				fregion.AddRect(frc);
				redraw.Set(TRawEvent::ERedraw);
				dspo->dsa->StartFlip(TPoint(pos[0], pos[1]), surfo->surf, frc, fregion);
				UserSvr::AddEvent(redraw);
				fregion.Close();
				return(0);
			}
		#endif
		if (dspo->bitgc)
		{
			TRect frc(rect[0], rect[1], rect[2], rect[3]);
			TRawEvent redraw;
			RRegion fregion;
			fregion.Clear();
			fregion.AddRect(frc);
			redraw.Set(TRawEvent::ERedraw);
			dspo->bitgc->BitBlt(TPoint(pos[0], pos[1]), surfo->surf, frc);
			dspo->screendevice->Update(fregion);
			UserSvr::AddEvent(redraw);
			fregion.Close();
		}
	#elif defined(DEVICE_X11)
		ecNativetype *wnd = (ecNativetype *)dspo->winhandle;
		long screennumber = DefaultScreen((Display *)wnd[0]);
		if (surfo->overlay)
			XvPutImage((Display *)wnd[0], dspo->xv_port, *(Window *)wnd[1], DefaultGC((Display *)wnd[0], screennumber), surfo->overlay, 0, 0, surfo->membufsize & 0xffff, surfo->membufsize >> 16, 0, 0, surfo->membufsize & 0xffff, surfo->membufsize >> 16);
		else if (surfo->surf)
			XPutImage((Display *)wnd[0], *(Window *)wnd[1], DefaultGC((Display *)wnd[0], screennumber), surfo->surf, 0, 0, 0, 0, surfo->membufsize & 0xFFFF, surfo->membufsize >> 16);
	#elif defined(DEVICE_MAC)
		ecNativetype *wnd = (ecNativetype *)dspo->winhandle;
		MacFlipSurface(wnd, surfo->surf, pos, rect);
	#elif defined(DEVICE_OPENKODE)
		ecNativetype *winhandlearr = (ecNativetype *)dspo->winhandle;
		if (dspo->displayalpha)
		{
			if (dspo->locksurface(winhandlearr[0], surfo->surf, KD_NULL) != EGL_FALSE)
			{
				EGLint height, width, bpp, pitch, lockptr, origin, tctr;
				unsigned long *surfacemem;
				eglQuerySurface(winhandlearr[0], surfo->surf, EGL_WIDTH, &width);
				eglQuerySurface(winhandlearr[0], surfo->surf, EGL_HEIGHT, &height);
				eglQuerySurface(winhandlearr[0], surfo->surf, EGL_BITMAP_PITCH_KHR, &pitch);
				eglQuerySurface(winhandlearr[0], surfo->surf, EGL_BITMAP_POINTER_KHR, &lockptr);
				eglQuerySurface(winhandlearr[0], surfo->surf, EGL_BITMAP_ORIGIN_KHR, &origin);
				surfacemem = (unsigned long *)lockptr;
				if (surfacemem)
				{
					EGLint x, y;
					if (origin == EGL_LOWER_LEFT_KHR)
					{
						surfacemem += (height - 1) * pitch / 4;
						pitch = -pitch;
					}
					for (y = 0; y < rect[3] - rect[1]; y++)
						for (x = 0; x < rect[2] - rect[0]; x++)
							surfacemem[(pos[1] + y) * pitch / 4 + pos[0] + x] |= 0xFF << 24;
					//for (ctr = 0; ctr < width * height; ctr++)
					//	surfacemem[ctr / width * pitch / 4 + ctr % width] |= 0xFF << 24;
				}
				dspo->unlocksurface(winhandlearr[0], surfo->surf);
			}
		}
		eglSwapBuffers(winhandlearr[0], surfo->surf);
	#endif
	return(0);
}

//Suspend display output
void ecDspSuspend(ECOREOBJ dspobj)
{
	EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
	#if defined(DEVICE_WINCE)
		if (dspo->dll_gapi)
			if (dspo->gapiopen)
				dspo->func_gxsuspend();
	#elif defined(DEVICE_SYMBIAN) && (DEVICE_SYMBIAN < 900)
		if (dspo->dsa == NULL)
		{
			TWsEvent tEvent;
			tEvent.SetType(EEventScreenDeviceChanged);
			tEvent.SetTimeNow();
			CEikonEnv::Static()->WsSession().SendEventToAllWindowGroups(tEvent);
		}
	#endif
}

//Resume display output
void ecDspResume(ECOREOBJ dspobj)
{
	EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
	#if defined(DEVICE_WINCE)
		if (dspo->dll_gapi)
			if (dspo->gapiopen)
				dspo->func_gxresume();
		#if defined(DEVICE_WINMOBILE)
			if (dspo->ddraw)
			{
				dspo->ddraw->SetCooperativeLevel((HWND)dspo->winhandle, DDSCL_NORMAL);
				dspo->dfrontbuf->Restore();
			}
		#endif
	#elif defined(DEVICE_SYMBIAN) && (DEVICE_SYMBIAN >= 610)
		if (dspo->dsa)
			dspo->dsa->StartDSA();
	#elif defined(DEVICE_X11)
		if (dspo->xv_port)// restart Xv
		{
			ecNativetype *wnd = (ecNativetype *)dspo->winhandle;
			XvUngrabPort((Display *)wnd[0], dspo->xv_port, CurrentTime);
			XSync((Display *)wnd[0], False);
			XvGrabPort((Display *)wnd[0], dspo->xv_port, CurrentTime);
		}
	#endif
}

//Set palette entries
void ecDspSetPalette(ECOREOBJ dspobj, unsigned long *colors)
{
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
		if (dspo->ddraw)
		{
			if (dspo->dpalette)
			{
				PALETTEENTRY pal[256];
				for (unsigned short palctr = 0; palctr < 256; palctr++)
				{
					pal[palctr].peRed   = (unsigned char)((colors[palctr] & 0xFF0000) >> 16);
					pal[palctr].peGreen = (unsigned char)((colors[palctr] & 0xFF00) >> 8);
					pal[palctr].peBlue  = (unsigned char)(colors[palctr] & 0xFF);
					pal[palctr].peFlags = 0;
				}
				dspo->dpalette->SetEntries(0, 0, 256, pal);
				dspo->dfrontbuf->SetPalette(dspo->dpalette);
			}
		}
	#endif
}

//Return amount of video memory
//Errcode: E_OK, E_UNSUPPORTED, E_ERROR
ecErr ecDspGetVideoMemory(ECOREOBJ dspobj, unsigned long *vmem)
{
	#if defined(DEVICE_WIN32)
		EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
		if (dspo->ddraw)
		{
			DWORD dwmemtotal;
			DWORD dwmemfree;
			HRESULT hr;
			#if defined(DEVICE_WINMOBILE)
				DDSCAPS ddscaps;
				ecMemSet(&ddscaps, 0, sizeof(ddscaps));
				ddscaps.dwCaps = DDSCAPS_VIDEOMEMORY;
				hr = dspo->ddraw->GetAvailableVidMem(&ddscaps, &dwmemtotal, &dwmemfree);
			#else
				DDCAPS ddcaps, ddhelcaps;
				ecMemSet(&ddcaps, 0, sizeof(ddcaps));
				ecMemSet(&ddhelcaps, 0, sizeof(ddcaps));
				ddcaps.dwSize = sizeof(DDCAPS);
				ddhelcaps.dwSize = sizeof(DDCAPS);
				hr = dspo->ddraw->GetCaps(&ddcaps, &ddhelcaps);
				dwmemtotal = ddcaps.dwVidMemTotal;
				dwmemfree = ddcaps.dwVidMemFree;
			#endif
			if (FAILED(hr))
			{
				if (hr == DDERR_NODIRECTDRAWHW || hr == DDERR_UNSUPPORTED)
					return(1);//E_UNSUPPORTED
				return(2);//E_ERROR
			}
			vmem[0] = dwmemtotal;
			vmem[1] = dwmemfree;
			return(0);//E_OK
		}
		else
			return(2);//E_ERROR
	#endif
	return(1);//E_UNSUPPORTED
}

//Create native surface
//Errcode: E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR
ECOREOBJ ecDspCreateNativeSurface(ECOREOBJ dspobj, ecErr &errcode, bool hardware, bool backbuffer, long width, long height)
{
	errcode = 0;
	EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
	EC_SURFACEOBJ *surfo = new EC_SURFACEOBJ;
	if (surfo)
	{
		surfo->surf = NULL;
		#if defined(DEVICE_WIN32)
			errcode = 2;
			surfo->bmpdc = NULL;
			surfo->hbitmap = NULL;
			if (dspo->dll_ddraw)
			{
				if (dspo->ddraw)
				{
					DDSURFACEDESC ddesc;
					HRESULT hr;
					errcode = 0;
					ecMemSet(&ddesc, 0, sizeof(ddesc));
					ddesc.dwSize = sizeof(ddesc);
					ddesc.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
					if (hardware)
						ddesc.ddsCaps.dwCaps = DDSCAPS_VIDEOMEMORY;
					else
						ddesc.ddsCaps.dwCaps = DDSCAPS_SYSTEMMEMORY;
					ddesc.dwWidth = width;
					ddesc.dwHeight = height;
					hr = dspo->ddraw->CreateSurface(&ddesc, &surfo->surf, NULL);
					if (FAILED(hr) || surfo->surf == NULL)
						errcode = 1;
				}
			}
			if (dspo->gdidc)
			{
				surfo->bmpdc = CreateCompatibleDC(NULL);
				if (surfo->bmpdc)
				{
					BITMAPINFO bmpinfo;
					ZeroMemory(&bmpinfo, sizeof(BITMAPINFO));
					bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
					bmpinfo.bmiHeader.biWidth = width;
					bmpinfo.bmiHeader.biHeight = -1 * (long)height;
					bmpinfo.bmiHeader.biPlanes = 1;
					bmpinfo.bmiHeader.biBitCount = GetDeviceCaps(dspo->gdiwindc, BITSPIXEL);
					surfo->hbmppitch = (width * bmpinfo.bmiHeader.biBitCount + 7) / 8;
					surfo->hbitmap = CreateDIBSection(surfo->bmpdc, (BITMAPINFO *)&bmpinfo, DIB_RGB_COLORS, (void **)&surfo->hbmppix, NULL, 0);
					if (surfo->hbitmap == NULL)
					{
						DeleteDC(surfo->bmpdc);
						errcode = 1;
					}
					errcode = 0;
				}
				else
					errcode = 1;
			}
		#elif defined(DEVICE_SYMBIAN)
			bool createok = false;
			if (dspo->dsa)
				createok = true;
			if (dspo->bitgc)
				createok = true;
			if (createok)
			{
				surfo->surf = new CFbsBitmap;
				if (surfo->surf)
				{
					TDisplayMode createmode = dspo->dispmode;
					if (createmode == EColor16MA)
						createmode = (TDisplayMode)EColor16MU;
					if (surfo->surf->Create(TSize(width, height), createmode) == KErrNone)
					{
						surfo->surflock = new TBitmapUtil(surfo->surf);
						if (surfo->surflock == NULL)
							errcode = 1;//E_NOMEMORY
					}
					else
						errcode = 3;//E_ERROR
				}
				else
					errcode = 1;//E_NOMEMORY
			}
			else
				errcode = 2;
		#elif defined(DEVICE_LINUX)
			#if defined(DEVICE_ANDROID)
				surfo->surf = NULL;
				if (dspo->backbuffer)
				{
				    
				}
			#endif
			#if defined(DEVICE_X11)
				surfo->overlay = NULL;
				surfo->surf = NULL;
				if (dspo->winhandle)
				{
					if (dspo->framebuf_ptr == 0)
					{
						ecNativetype *wnd = (ecNativetype *)dspo->winhandle;
						long xpitch = dspo->bitdepth / 8;
						surfo->lpitch = width * xpitch;
						if (surfo->lpitch % 4 != 0)
							surfo->lpitch += 4 - surfo->lpitch % 4;
						surfo->membuffer = ecMemAlloc(height * surfo->lpitch);
						if (surfo->membuffer)
						{
							long screennumber = DefaultScreen((Display *)wnd[0]);
							Visual *vis = DefaultVisual((Display *)wnd[0], screennumber);
							if (dspo->xvimg_overlay && backbuffer)
							{
								surfo->overlay = XvCreateImage((Display *)wnd[0], dspo->xv_port, dspo->xv_formatid, NULL, width, height);
								if (surfo->overlay)
								{
									surfo->overlay->data = (char*) surfo->membuffer;
								}
							}
							if (!surfo->overlay)
							{
								surfo->surf = XCreateImage((Display *)wnd[0], vis, DefaultDepth((Display *)wnd[0], screennumber), ZPixmap, 0, (char *)surfo->membuffer, width, height, 32, surfo->lpitch);
								if (surfo->surf == NULL)
								{
									ecMemFree(surfo->membuffer);
									errcode = 1;//E_NOMEMORY <- fixme
								}
							}
							surfo->membufsize = (height << 16) | width;
							errcode = 0;//E_OK
						}
						else
							errcode = 1;//E_NOMEMORY
					}
					else
						errcode = 2;//E_UNSUPPORTED
				}
				else
					errcode = 3;//E_ERROR
			#endif
		#elif defined(DEVICE_MAC)
			if (dspo->winhandle)
			{
				ecNativetype *wnd = (ecNativetype *)dspo->winhandle;
				long xpitch = dspo->bitdepth / 8;
				surfo->lpitch = width * xpitch;
				if (surfo->lpitch % 4 != 0)
					surfo->lpitch += 4 - surfo->lpitch % 4;
				surfo->membuffer = ecMemAlloc(height * surfo->lpitch);
				if (surfo->membuffer)
				{
					MacAllocSurface(wnd, surfo->surf, surfo->membuffer, width, height, surfo->lpitch, dspo->bitdepth);
					errcode = 0;//E_OK
				}
				else
					errcode = 1;//E_NOMEMORY
			}
			else
				errcode = 3;//E_ERROR
		#elif defined(DEVICE_OPENKODE)
			if (dspo->windowsurface != EGL_NO_SURFACE)
			{
				if (backbuffer)
				{
					EGLint buffertype = 0xFEFA;
					ecNativetype *winhandlearr = (ecNativetype *)dspo->winhandle;
					eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_RENDER_BUFFER, &buffertype);
					if (buffertype == EGL_BACK_BUFFER)
					{
						surfo->surf = dspo->windowsurface;
						eglQuerySurface(winhandlearr[0], surfo->surf, EGL_WIDTH, (EGLint *)&surfo->width);
						eglQuerySurface(winhandlearr[0], surfo->surf, EGL_HEIGHT, (EGLint *)&surfo->height);
					}
					else
						errcode = 2;//E_UNSUPPORTED
				}
				else
					errcode = 2;//E_UNSUPPORTED
			}
			else
				errcode = 4;//E_NOENTRY
		#else
			errcode = 2;//E_UNSUPPORTED
		#endif
		if (errcode)
		{
			ecDspDestroyNativeSurface(dspo, surfo);
			surfo = NULL;
		}
	}
	else
		errcode = 1;
	return(surfo);
}

//Destroy native surface
void ecDspDestroyNativeSurface(ECOREOBJ dspobj, ECOREOBJ surfobj)
{
	EC_SURFACEOBJ *surfo = (EC_SURFACEOBJ *)surfobj;
	#if defined(DEVICE_WIN32)
		EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
		if (dspo)
		{
			if (dspo->ddraw)
			{
				if (surfo->surf)
					surfo->surf->Release();
			}
			if (surfo->bmpdc)
			{
				if (surfo->hbitmap)
				{
					DeleteObject(surfo->hbitmap);
					surfo->hbitmap = NULL;
				}
				DeleteDC(surfo->bmpdc);
				surfo->bmpdc = NULL;
			}
		}
	#elif defined(DEVICE_SYMBIAN)
		if (surfo->surf)
		{
			if (surfo->surflock)
				delete surfo->surflock;
			delete surfo->surf;
		}
	#elif defined(DEVICE_X11)
		EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
		if (dspo)
			if (dspo->winhandle)
			{
				if (surfo->surf)
					XDestroyImage(surfo->surf);
				if (surfo->overlay)
					XFree(surfo->overlay);
			}
	#elif defined(DEVICE_MAC)
		EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
		if (dspo)
			if (dspo->winhandle)
				if (surfo->surf)
				{
					MacDeallocSurface(surfo->surf);
					ecMemFree(surfo->membuffer);
				}
	#endif
	delete surfo;
}

//Lock the native surface for data access
void *ecDspLockNativeSurface(ECOREOBJ dspobj, ECOREOBJ surfobj, long *lpitch)
{
	EC_SURFACEOBJ *surfo = (EC_SURFACEOBJ *)surfobj;
	#if defined(DEVICE_WIN32)
		if (surfo->hbitmap)
		{
			*lpitch = surfo->hbmppitch;
			return(surfo->hbmppix);
		}
		else if (surfo->surf)
		{
			HRESULT hr;
			DDSURFACEDESC ddesc;
			ddesc.dwSize = sizeof(ddesc);
			hr = surfo->surf->Lock(NULL, &ddesc, DDLOCK_WAIT, NULL);
			if (FAILED(hr))
			{
				if (hr == DDERR_SURFACELOST)
					surfo->surf->Restore();
				return(NULL);
			}
			*lpitch = ddesc.lPitch;
			return(ddesc.lpSurface);
		}
	#elif defined(DEVICE_SYMBIAN) && (DEVICE_SYMBIAN >= 610)
		if (surfo->surflock)
		{
			TSize surfsize = surfo->surf->SizeInPixels();
			TPoint position;
			position.iX = 0;
			position.iY = 0;
			surfo->surflock->Begin(position);
			*lpitch = surfo->surf->ScanLineLength(surfsize.iWidth, surfo->surf->DisplayMode());
			return((unsigned char *)surfo->surf->DataAddress());
		}
	#elif defined(DEVICE_X11)
		*lpitch = surfo->lpitch;
		return(surfo->membuffer);
	#elif defined(DEVICE_MAC)
		*lpitch = surfo->lpitch;
		return(surfo->membuffer);
	#elif defined(DEVICE_OPENKODE)
		EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
		ecNativetype *winhandlearr = (ecNativetype *)dspo->winhandle;
		EGLint lockptr;
		if (dspo->locksurface(winhandlearr[0], surfo->surf, KD_NULL) != EGL_FALSE)
		{
			EGLint height, width;//origin, bpp
			bool lockfail = false;
			eglQuerySurface(winhandlearr[0], surfo->surf, EGL_WIDTH, &width);
			eglQuerySurface(winhandlearr[0], surfo->surf, EGL_HEIGHT, &height);
			//eglQuerySurface(winhandlearr[0], surfo->surf, EGL_BITMAP_ORIGIN_KHR, &origin);
			if (width < surfo->width || height < surfo->height)
				lockfail = true;
			if (eglQuerySurface(winhandlearr[0], surfo->surf, EGL_BITMAP_PITCH_KHR, (EGLint *)lpitch) == EGL_FALSE)
				lockfail = true;
			if (eglQuerySurface(winhandlearr[0], surfo->surf, EGL_BITMAP_POINTER_KHR, &lockptr) == EGL_FALSE)
				lockfail = true;
			if (*lpitch == 0 || lockfail)
			{
				ecDspUnLockNativeSurface(dspobj, surfobj);
				return(NULL);
			}
			//eglQuerySurface(winhandlearr[0], surfo->surf, EGL_BITMAP_PIXEL_SIZE_KHR, &bpp);
			//if (origin == EGL_LOWER_LEFT_KHR)
			//{
			//	lockptr += *lpitch * (height - 1);
			//	*lpitch = -*lpitch;
			//}
			return((unsigned char *)lockptr);
		}
	#endif
	return(NULL);
}

//Unlock native surface
void ecDspUnLockNativeSurface(ECOREOBJ dspobj, ECOREOBJ surfobj)
{
	EC_SURFACEOBJ *surfo = (EC_SURFACEOBJ *)surfobj;
	#if defined(DEVICE_WIN32)
		if (surfo->surf)
			surfo->surf->Unlock(NULL);
	#elif defined(DEVICE_SYMBIAN) && (DEVICE_SYMBIAN >= 610)
		if (surfo->surflock)
			surfo->surflock->End();
	#elif defined(DEVICE_OPENKODE)
		EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
		ecNativetype *winhandlearr = (ecNativetype *)dspo->winhandle;
		dspo->unlocksurface(winhandlearr[0], surfo->surf);
	#endif
}

//Set colorkey to native surface
void ecDspNativeSetColorkey(ECOREOBJ dspobj, ECOREOBJ surfdst, unsigned long color)
{
	#if defined(DEVICE_WIN32)
		EC_SURFACEOBJ *surfd = (EC_SURFACEOBJ *)surfdst;
		if (surfd->surf)
		{
			DDCOLORKEY ddck;
			ddck.dwColorSpaceLowValue  = color;
			ddck.dwColorSpaceHighValue = color;
			surfd->surf->SetColorKey(DDCKEY_SRCBLT, &ddck);
		}
	#endif
}

//Native fillrect for a surface
bool ecDspNativeFillRect(ECOREOBJ dspobj, ECOREOBJ surfdst, long *rc, unsigned long color)
{
	#if defined(DEVICE_WIN32)
		EC_SURFACEOBJ *surfd = (EC_SURFACEOBJ *)surfdst;
		if (surfd->surf)
		{
			DDBLTFX ddbltfx;
			RECT drc;
			SetRect(&drc, rc[0], rc[1], rc[2], rc[3]);
			ecMemSet(&ddbltfx, 0, sizeof(DDBLTFX));
			ddbltfx.dwSize = sizeof(DDBLTFX);
			ddbltfx.dwFillColor = color;
			if (!FAILED(surfd->surf->Blt(&drc, NULL, NULL, DDBLT_COLORFILL, &ddbltfx)))
				return(true);
		}
	#endif
	return(false);
}

//Native bltfast between surfaces
//Flags: 1 = colorkey
bool ecDspNativeBltFast(ECOREOBJ dspobj, ECOREOBJ surfdst, ECOREOBJ surfsrc, long *pos, long *rc, unsigned long flags)
{
	#if defined(DEVICE_WIN32)
		EC_SURFACEOBJ *surfd = (EC_SURFACEOBJ *)surfdst;
		EC_SURFACEOBJ *surfs = (EC_SURFACEOBJ *)surfsrc;
		if (surfd->surf && surfs->surf)
		{
			RECT src;
			SetRect(&src, rc[0], rc[1], rc[2], rc[3]);
			#if defined(DEVICE_WINMOBILE)
				RECT drc;
				SetRect(&drc, pos[0], pos[1], pos[0] + rc[2] - rc[0], pos[1] + rc[3] - rc[1]);
				if (flags & 1) //EFX_COLORKEY
				{
					if (!FAILED(surfd->surf->Blt(&drc, surfs->surf, &src, DDBLT_KEYSRC, NULL)))
						return(true);
				}
				else
				{
					if (!FAILED(surfd->surf->Blt(&drc, surfs->surf, &src, 0, NULL)))
						return(true);
				}
			#else
				if (flags & 1) //EFX_COLORKEY
				{
					if (!FAILED(surfd->surf->BltFast(pos[0], pos[1], surfs->surf, &src, DDBLTFAST_SRCCOLORKEY)))
						return(true);
				}
				else
				{
					if (!FAILED(surfd->surf->BltFast(pos[0], pos[1], surfs->surf, &src, 0)))
						return(true);
				}
			#endif
		}
	#endif
	return(false);
}

//Native blt between surfaces
//Flags: 1 = colorkey, 256 = flip horizontal, 512 = flip vertical
bool ecDspNativeBlt(ECOREOBJ dspobj, ECOREOBJ surfdst, ECOREOBJ surfsrc, long *pos, long *rc, long *cliprc, long *scale, unsigned long rotate, unsigned long flags)
{
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		EC_SURFACEOBJ *surfd = (EC_SURFACEOBJ *)surfdst;
		EC_SURFACEOBJ *surfs = (EC_SURFACEOBJ *)surfsrc;
		if (surfd->surf && surfs->surf)
		{
			if (rotate % 4096 != 0)
				return(false);
			DDBLTFX bltfx;
			RECT src, drc;
			unsigned long bltflags = 0;
			SetRect(&src, rc[0], rc[1], rc[2], rc[3]);
			SetRect(&drc, pos[0] - (rc[2] - rc[0]) * scale[0] / 131072, pos[1] - (rc[3] - rc[1]) * scale[1] / 65536, 0, 0);
			drc.right = drc.left + (rc[2] - rc[0]) * scale[0] / 65536;
			drc.bottom = drc.top + (rc[3] - rc[1]) * scale[1] / 65536;
			if (drc.left >= cliprc[0] && drc.right <= cliprc[2] && drc.top >= cliprc[1] && drc.bottom <= cliprc[3])
			{
				ecMemSet(&bltfx, 0, sizeof(DDBLTFX));
				bltfx.dwSize = sizeof(DDBLTFX);
				bltfx.dwDDFX = 0;
				if (flags & 256) //EFLIP_HORIZONTAL
					bltfx.dwDDFX += DDBLTFX_MIRRORLEFTRIGHT;
				if (flags & 512) //EFLIP_VERTICAL
					bltfx.dwDDFX += DDBLTFX_MIRRORUPDOWN;
				if (bltfx.dwDDFX)
					bltflags += DDBLT_DDFX;
				if (flags & 1) //EFX_COLORKEY
					bltflags += DDBLT_KEYSRC;
				if (!FAILED(surfd->surf->Blt(&drc, surfs->surf, &src, bltflags, &bltfx)))
					return(true);
			}
		}
	#endif
	return(false);
}

//Get the native type of the surface
ecNativetype ecDspNativeSurfaceType(ECOREOBJ surfobj)
{
	EC_SURFACEOBJ *surfo = (EC_SURFACEOBJ *)surfobj;
	#if defined(DEVICE_WIN32)
		if (surfo->hbitmap)
			return((ecNativetype)surfo->hbitmap);
	#endif
	if (surfo->surf)
		return((ecNativetype)surfo->surf);
	return(NULL);
}

//Returns if the display mode handles orientation itself
bool ecDspInfHandleOrientation(ECOREOBJ dspobj)
{
	EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
	#if defined(DEVICE_WIN32)
		if (dspo->ddraw || dspo->gdidc)
			return(true);
		#if defined(DEVICE_WINCE)
			if (dspo->extescapeopen && dspo->rotatedextescape)
				return(true);
		#endif
	#elif defined(DEVICE_SYMBIAN)
		if (dspo->dsa != NULL || dspo->bitgc != NULL)
			return(true);
	#elif defined(DEVICE_OPENKODE)
		if (dspo->windowsurface != EGL_NO_SURFACE)
			return(true);
	#elif defined (DEVICE_ANDROID)
		return (true);
	#endif
	return(false);
}

//Returns native display resolution
void ecDspInfNativeResolution(ECOREOBJ dspobj, long *resdata)
{
	resdata[0] = 0;
	resdata[1] = 0;
	#if defined(DEVICE_WIN32)
		#if defined(DEVICE_WINCE)
			RawFrameBufferInfo rfbi;
			HDC hdc = NULL;
			resdata[0] = 0;
			resdata[1] = 0;
			hdc = GetDC(NULL);
			if (hdc != NULL)
			{
				if (ExtEscape(hdc, GETRAWFRAMEBUFFER, 0, NULL, sizeof(RawFrameBufferInfo), (char *)&rfbi) > 0)
				{
					resdata[0] = rfbi.cxPixels;
					resdata[1] = rfbi.cyPixels;
				}
				ReleaseDC(NULL, hdc);
			}
			if (resdata[0] + resdata[1] == 0)
			{
				DEVMODE_IMP_X devMode;
				devMode.dmSize = sizeof(devMode);
				if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode) == TRUE)
				{
					resdata[0] = devMode.dmPelsWidth;
					resdata[1] = devMode.dmPelsHeight;
				}
				else
				{
					resdata[0] = GetSystemMetrics(SM_CXSCREEN);
					resdata[1] = GetSystemMetrics(SM_CYSCREEN);
				}
			}
		#else
			resdata[0] = GetSystemMetrics(SM_CXSCREEN);
			resdata[1] = GetSystemMetrics(SM_CYSCREEN);
		#endif
	#elif defined(DEVICE_SYMBIAN)
		TInt tmp;
		TPixelsAndRotation sizerotation;
		CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation(sizerotation);
		switch(sizerotation.iRotation)
		{
			case CFbsBitGc::EGraphicsOrientationNormal:
			case CFbsBitGc::EGraphicsOrientationRotated180:
				break;
			case CFbsBitGc::EGraphicsOrientationRotated90:
			case CFbsBitGc::EGraphicsOrientationRotated270:
				tmp = sizerotation.iPixelSize.iWidth;
				sizerotation.iPixelSize.iWidth = sizerotation.iPixelSize.iHeight;
				sizerotation.iPixelSize.iHeight = tmp;
				break;
		}
		resdata[0] = sizerotation.iPixelSize.iWidth;
		resdata[1] = sizerotation.iPixelSize.iHeight;
	#elif defined(DEVICE_LINUX)
		EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
		if (dspo->framebuf_ptr)
		{
			struct fb_var_screeninfo screeninfo;
			ioctl(dspo->framebuf_ptr, FBIOGET_VSCREENINFO, &screeninfo);
			resdata[0] = screeninfo.xres;
			resdata[1] = screeninfo.yres;
		}
		else
		{
			resdata[0] = dspo->framebufsize & 65535;
			resdata[1] = dspo->framebufsize >> 16;
		}
	#elif defined(DEVICE_MAC)
		EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
		MacGetNativeResolution((ECOREOBJ)dspo->winhandle, resdata[0], resdata[1]);
	#elif defined(DEVICE_OPENKODE)
		EC_DISPLAYOBJ *dspo = (EC_DISPLAYOBJ *)dspobj;
		ecNativetype *winhandlearr = (ecNativetype *)dspo->winhandle;
		eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_WIDTH, (EGLint *)&resdata[0]);
		eglQuerySurface(winhandlearr[0], dspo->windowsurface, EGL_HEIGHT, (EGLint *)&resdata[1]);
	#endif
}

//Get ui orientation
unsigned long ecDspInfGetOrientation(ecNativetype winhandle)
{
	#if defined(DEVICE_WINCE)
		DEVMODE_IMP_X DevMode;
		ecMemSet(&DevMode, 0, sizeof(DevMode));
		DevMode.dmSize   = sizeof(DevMode);
		DevMode.dmFields = DM_DISPLAYORIENTATION;
		if (ChangeDisplaySettingsEx(NULL, &DevMode, NULL, CDS_TEST, NULL) == DISP_CHANGE_SUCCESSFUL)
		{
			OSVERSIONINFO verinfo;
			bool compensateorientation = true;
			unsigned long neworientation = DevMode.dmDisplayOrientation;
			ecMemSet(&verinfo, 0, sizeof(OSVERSIONINFO));
			verinfo.dwOSVersionInfoSize = sizeof(verinfo);
			if (GetVersionEx(&verinfo) == TRUE)
				if (verinfo.dwMajorVersion >= 6 || (verinfo.dwMajorVersion == 5 && verinfo.dwMinorVersion >= 2))
					compensateorientation = false;
			if (compensateorientation && GetSystemMetrics(SM_CXSCREEN) != GetSystemMetrics(SM_CYSCREEN))
			{
				long resdata[2];
				ecDspInfNativeResolution(NULL, resdata);
				if (resdata[0] != resdata[1])
				{
					if ((neworientation == DMDO_90 || neworientation == DMDO_270) && resdata[0] == GetSystemMetrics(SM_CXSCREEN) && resdata[1] == GetSystemMetrics(SM_CYSCREEN))
					{
						if (neworientation == DMDO_90)
							neworientation = DMDO_0;
						else
							neworientation = DMDO_180;
					}
					if ((neworientation == DMDO_0 || neworientation == DMDO_180) && resdata[0] == GetSystemMetrics(SM_CYSCREEN) && resdata[1] == GetSystemMetrics(SM_CXSCREEN))
					{
						if (neworientation == DMDO_0)
							neworientation = DMDO_270;
						else
							neworientation = DMDO_90;
					}
				}
			}
			if (neworientation == DMDO_90)
				neworientation = 270;
			else if (neworientation == DMDO_180)
				neworientation = 180;
			else if (neworientation == DMDO_270)
				neworientation = 90;
			else
				neworientation = 0;
			return(neworientation);
		}
	#endif
	return(0);
}
