/////////////////////////////////////////////////////////////////////
// Code/main.h
//
// Copyright (c)2004-2017 Elements Interactive B.V.
//
// Contains the main wrapper class
/////////////////////////////////////////////////////////////////////

//Include internal resources for native Win32 builds
#if defined(DEVICE_WIN32)
	#include "resource.h"
#endif

/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

#include <cstdio>

//The main class
class ClassMain : public ClassEdge
{
	public:
		ClassMain(void);
		~ClassMain(void);
		ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnInit(ENATIVETYPE instance);
		ERESULT OnMinimize(void);
		ERESULT OnRestore(void);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
		void OnDisplayChange(ClassEDisplay *display);
		ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		void OnExit(ENATIVETYPE instance);
		void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
		void OnButtonUp(unsigned long bnr, EBUTTONLIST *blist);
		void OnWheel(long direction);
		void OnStylusUp(POINT pnt);
		void OnStylusDown(POINT pnt);
		void OnStylusDouble(POINT pnt);
		void OnStylusMove(POINT pnt);
		void OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer);
	
		E2DSurface testimage;
};
