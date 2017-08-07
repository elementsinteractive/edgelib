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
	private:
		long pinchDistPrevious;
		long zoom;
		E2DSurface logo;
		bool pinchActive;
	public:
		ClassMain(void);
		~ClassMain(void);
		ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnInit(ENATIVETYPE instance);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
		void OnDisplayChange(ClassEDisplay *display);
		ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		void OnExit(ENATIVETYPE instance);
};


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassMain::ClassMain(void)
{
	pinchActive = false;
	zoom = 42000;
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
	ClassEStd::StrCpy(config->caption, "Pinch demo!");
	config->icon            = IDI_MAIN;           //Application icon
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	return(E_OK);
}

//Callback: Called when the display mode has changed, use this to initialize your surfaces, etc.
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
	//Enable blending for OpenGL (ES)
	#if defined(EGL_USEGL)
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
	#endif
	
	//Load the EDGELIB logo
	if (display->CreateSurface(&logo, "edgelib.png", EST_READONLYPATH) != E_OK)
		return(E_ERROR);
	display->UploadTexture(&logo);
	
	//use smooth scaling
	#if defined(EGL_USEGL)
		#if defined(EGL_USEGLES)
			glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		#else
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);			
		#endif
	#endif
	
	return(E_OK);
}

//Callback: Called when the display has been changed (because of a change in orientation)
void ClassMain::OnDisplayChange(ClassEDisplay *display)
{
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	//Check for pinch
	if (ecd.input->ClickHold(EIC_TOUCH1) && ecd.input->ClickHold(EIC_TOUCH2))
	{
		long pinchDist = 0;
		long x1, y1, x2, y2;
		ecd.input->GetClickPosition(x1, y1, EIC_TOUCH1);
		ecd.input->GetClickPosition(x2, y2, EIC_TOUCH2);
		
		//Calculcate pinch distance and zoom
		pinchDist = ClassEMath::Abs(x1 - x2) + ClassEMath::Abs(y1 - y2);
		if (!pinchActive)
			pinchActive = true;
		else
			zoom += (pinchDist - pinchDistPrevious) * 50;
	
		//Limit zoom distance
		if (zoom < 2000)
			zoom = 2000;
		else if (zoom > 100000)
			zoom = 100000;
		
		//Replace current previous distance with current distance
		pinchDistPrevious = pinchDist;
	}
	else
		pinchActive = false;
		
	//Fill the screen with a nice blue color
	#if defined(EGL_USEGL)
		#if defined(EGL_USEGLES)
			glClearColorx(0, 0, 0, 65536);
		#else
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		#endif
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	#else
		display->buffer.FillRect(NULL, E_RGBX(255, 255, 255));
	#endif

	//Render the EDGELIB logo scaled
	display->SetShading(E3D_TEXTURE);
	display->Blt(display->GetWidth() / 2, display->GetHeight() / 2, &logo, NULL, 0, zoom);
	return(E_OK);
}

//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
}
