// code/main.h
// The main program wrapper class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

#ifdef DEVICE_WIN32
	#include "resource.h"             //Window app resources
#endif

class ClassMain : public ClassEdge
{
	public:
		ClassMain(void);
		~ClassMain(void);
		ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnInit(ENATIVETYPE instance);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
		ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		void OnExit(ENATIVETYPE instance);
};//The main class

/////////////////////////////////////////////////////////////////////
// ClassMain::Public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: Constructor
ClassMain::ClassMain(void)
{
}

//ClassMain: Destructor
ClassMain::~ClassMain(void)
{
}

//Callback: framework configuration
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
	config->flags           = 0;                    //No extra flag options
	config->focusminimize   = false;                //Don't minimize the application when the focus is lost
	#if defined(DEVICE_GP2X)
		config->smartfps = false;                   //No smartfps needed for GP2X
	#else
		config->smartfps = true;                    //Enable smartfps, needed for most phone devices to prevent 100% processor usage
	#endif
	config->ffwdkey         = 0;                    //No fastforward key
	config->slomokey        = 0;                    //No slow motion key
	config->maxfps          = 0;                    //No frame limiter
	#ifndef DEVICE_DESKTOP
		config->focusminimize   = true;             //Minimize the application on lost focus for mobile devices
	#else
		config->focussuspend    = false;
	#endif
	#ifdef DEVELOPMENT
		config->ffwdkey         = 'A';              //A key for fast forwarding (multiplies framerate by 2.5)
		config->slomokey        = 'S';              //A key for slow motion     (halves framerate)
		config->maxfps          = 60;               //Limit the maximum frames per second by 60 (to save processor heat)
	#else
		config->maxfps          = 60;               //Limit the maximum frames per second by 60
	#endif
	return(E_OK);
}

//Callback: display configuration
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	#if defined(IDI_MAIN)
		config->icon = IDI_MAIN;
	#endif
	ClassEStd::StrCpy(config->caption, "MultiTouch");
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	return(E_OK);
}

//Callback: Called when the display mode has changed
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
	return(E_OK);
}

//Callback: Called when a new frame can be processed
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	COLORREF color[] = {ECOLOR_RED, ECOLOR_GREEN, ECOLOR_BLUE, ECOLOR_YELLOW, ECOLOR_WHITE};
	
	//Clear the screen
	display->buffer.FillRect(NULL, ECOLOR_BLACK);
	
	//Check the state of the five touches
	unsigned long ctr;
	for (ctr = 0; ctr < 5; ctr++)
	{
		if (ecd.input->ClickHold(ctr + 1))
		{
			POINT pnt;
			RECT rc;
			
			ecd.input->GetClickPosition(pnt.x, pnt.y, ctr + 1);
			SetRect(&rc, pnt.x - 40, pnt.y - 40, pnt.x + 40, pnt.y + 40);
			display->buffer.FillRect(&rc, color[ctr]);
		}
	}
	
	return(E_OK);
}

//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
}
