// code/main.h
// Contains the main wrapper class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Include internal resources for native Win32 builds
#if defined(DEVICE_WIN32)
#include "resource.h"
#else
#define IDI_MAIN 0
#endif

/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

//The main class
class ClassMain : public ClassEdge
{
	public:
		ClassMain(void);
		virtual ~ClassMain(void);
		ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnInit(ENATIVETYPE instance);
		ERESULT OnMinimize(void);
		ERESULT OnRestore(void);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
        void OnCameraChange(ClassECamera *, ERESULT);
        void OnCameraFrame(ClassECamera *, const E_CAMERAFRAME *);
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

    protected:
    
        const char * FormatToString(int format);
        
        E2DSurface cambuffer;
        ClassECamera * mycamera;
        
        void MenuAction(int);

        bool menu;
        int menuindex;
        int menumax;
        int winheight;

        bool discard;

        bool error;
        const char * errormsg;
        char errbuffer[256];
};


