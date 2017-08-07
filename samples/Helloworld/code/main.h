/////////////////////////////////////////////////////////////////////
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
};


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassMain::ClassMain(void)
{
}

//ClassMain: destructor
ClassMain::~ClassMain(void)
{
}

//Configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
	config->ffwdkey         = 'A';                //A key for fast forwarding (multiplies framerate by 2.5)
	config->slomokey        = 'S';                //A key for slow motion     (halves framerate)
	config->maxfps          = 60;                 //Limit the maximum frames per second by 60
	config->flags           = 0;                  //No extra flag options
	config->numconnections  = 1;                  //Keep 1 active network connection
	#if defined(DEVICE_GP2X)
		config->smartfps    = false;              //GP2X: No smartfps needed
	#else
		config->smartfps    = true;               //Other devices: Enable smart fps lock managing
	#endif
	#if defined(DEVICE_DESKTOP)
		config->focusminimize = false;            //Desktop PC: Don't minimize the application when the focus is lost
	#else
		config->focusminimize = true;             //Mobile devices: Minimize the application when the focus is lost
	#endif
	config->jogtiltarrow    = true;               //Emulate the UIQ jog as left/right arrows
	config->focussuspend    = true;               //Desktop PC: suspend the application when the window focus is lost
	config->backlighton     = true;               //Keep backlight on
	config->netlogging      = true;               //Enable logging of network events in the console
	config->lockkeys        = true;               //Lock keys for use in this application
	config->keysounds       = false;              //Disable key 'click' sounds for smartphones
	return(E_OK);
}

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	ClassEStd::StrCpy(config->caption, "Hello World!");
	config->icon            = IDI_MAIN;           //Application icon
	config->menu            = 0;                  //Application contains no Windows Menu
	config->dialogbox       = 0;                  //Don't start from dialogbox (Desktop only)
	config->width           = 240;                //Desktop PC: Custom display width
	config->height          = 320;                //Desktop PC: Custom display height
	config->videotype       = EDSP_DEFAULT;       //Choose the default display mode
	config->emulvideotype   = EDSP_DEFAULT;       //No color depth emulation
	config->orientation     = DOR_AUTO;           //Change orientation to match the settings of the device
	config->openmethod      = DOM_DEFAULT;        //Take recommended (fastest) display open method
	config->surfacedefault  = EST_SYSMEMORY;      //Create surfaces in system memory by default, don't use DirectDraw or Symbian bitmap surfaces
	#if defined(DEVICE_DESKTOP)
		config->fullscreen  = false;              //Windowed application for Desktop
		config->fullscreenmouse = true;           //Desktop PC: Enable the mouse cursor for fullscreen applications
	#else
		config->fullscreen  = true;               //Fullscreen application for mobile devices
	#endif
	config->dspnative       = true;               //If possible, try to use native blitters (like DirectDraw)
	config->enginestatus    = true;               //Desktop PC: Enable the status bar and status information icon
	config->engineconsole   = true;               //Desktop PC: Enable the edge console
	config->surfprerotate   = true;               //Use pre-rotated surfaces to increase speed in other orientations
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	return(E_OK);
}

//Callback: Called when the program minimizes
ERESULT ClassMain::OnMinimize(void)
{
	return(E_OK);
}

//Callback: Called when the program restores back from a minimized state
ERESULT ClassMain::OnRestore(void)
{
	return(E_OK);
}

//Callback: Called when the display mode has changed, use this to initialize your surfaces, etc.
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
	return(E_OK);
}

//Callback: Called when the display has been changed (because of a change in orientation)
void ClassMain::OnDisplayChange(ClassEDisplay *display)
{
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	display->buffer.DrawFont(2, 2, &display->fontinternal, "Hello World!");
	display->buffer.DrawFont(2, display->buffer.GetHeight() - 2, &display->fontinternal, "Button/Stylus: Quit", EFX_COLORKEY | EFO_VBOTTOM);
	return(E_OK);
}

//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
}

//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
	Quit();
}

//Callback: Called when the user released a key or button
void ClassMain::OnButtonUp(unsigned long bnr, EBUTTONLIST *blist)
{
}

//Callback: Called when the user scrolls a wheel device
void ClassMain::OnWheel(long direction)
{
}

//Callback: Called when the user released the stylus or mouse button
void ClassMain::OnStylusUp(POINT pnt)
{
}

//Callback: Called when the user points the stylus down or clicks the left mouse button
void ClassMain::OnStylusDown(POINT pnt)
{
	Quit();
}

//Callback: Called when the user points the stylus 2 times down quickly or double clicks the left mouse button
void ClassMain::OnStylusDouble(POINT pnt)
{
}

//Callback: Called when the user moves the stylus while pressed down, or moves the mouse cursor
void ClassMain::OnStylusMove(POINT pnt)
{
}

//Callback: Called when a network event occurs
void ClassMain::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
{
}
