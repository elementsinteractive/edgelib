// code/main.h
// Contains the framework class
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
// Structures                                                      //
/////////////////////////////////////////////////////////////////////

//Game configuration
typedef struct
{
	bool autoshot;
	unsigned char startlevel;
}GAMECFG;

//Program shared data
typedef struct
{
	unsigned long module;
	unsigned long modulemsg;
	bool forceminimize;
	E2DSurface font;
	E2DSurface titlebg;
	E2DSurface gamebg;
	E2DSurface sheet;
	GAMECFG cfg;
	ECD *ecd;
}SHARED_DATA;


/////////////////////////////////////////////////////////////////////
// Module definitions & messages                                   //
/////////////////////////////////////////////////////////////////////

#define MODULE_LOADER  0
#define MODULE_MENU    1
#define MODULE_LEVEL   2
#define MAX_MODULES    3
#define MODULE_CLEANUP MAX_MODULES

#define MM_NONE        0
#define MM_INIT        1
#define MM_FROMMODULE  32768
#define MM_CUSTOM      65536


/////////////////////////////////////////////////////////////////////
// Module includes                                                 //
/////////////////////////////////////////////////////////////////////

#include "module.h"
#include "loader.h"
#include "menu.h"
#include "level.h"


/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

//The main class
class ClassMain : public ClassEdge
{
	private:
		SHARED_DATA shared;
		ClassModule *cmodule[MAX_MODULES];
		bool InitModules(void);
		void LoadCfg(void);
		void SaveCfg(void);
	public:
		ClassMain(void);
		~ClassMain(void);
		ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnInit(ENATIVETYPE instance);
		ERESULT OnMinimize(void);
		ERESULT OnRestore(void);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
		ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		void OnExit(ENATIVETYPE instance);
};


/////////////////////////////////////////////////////////////////////
// ClassMain: private                                              //
/////////////////////////////////////////////////////////////////////

//Allocate and initialize each module
bool ClassMain::InitModules(void)
{
	unsigned char ctr;
	cmodule[MODULE_LOADER] = new ClassLoader;
	cmodule[MODULE_MENU]   = new ClassMenu;
	cmodule[MODULE_LEVEL]  = new ClassLevel;
	for (ctr = 0; ctr < MAX_MODULES; ctr++)
	{
		if (cmodule[ctr] != NULL)
			cmodule[ctr]->LinkShared(&shared);
		else
			return(false);
	}
	return(true);
}

//Load the game configuration file
void ClassMain::LoadCfg(void)
{
	ClassEFile ef;
	shared.cfg.autoshot = true;
	shared.cfg.startlevel = 1;
	if (ef.Open("game.cfg", EFOF_READ))
	{
		ef.Read(&shared.cfg, sizeof(GAMECFG));
		ef.Close();
	}
}

//Save the game configuration file
void ClassMain::SaveCfg(void)
{
	ClassEFile ef;
	if (ef.New("game.cfg"))
	{
		ef.Write(&shared.cfg, sizeof(GAMECFG));
		ef.Close();
	}
}


/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////

//ClassMain: constructor
ClassMain::ClassMain(void)
{
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_MODULES; ctr++)
		cmodule[ctr] = NULL;
	shared.module = MODULE_LOADER;
	shared.modulemsg = MM_INIT;
	shared.forceminimize = false;
}

//ClassMain: destructor
ClassMain::~ClassMain(void)
{
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_MODULES; ctr++)
		if (cmodule[ctr] != NULL)
			delete cmodule[ctr];
}

//Configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
	config->ffwdkey         = 'A';                //A key for fast forwarding (multiplies framerate by 2.5)
	config->slomokey        = 'S';                //A key for slow motion     (halves framerate)
	config->maxfps          = 60;                 //Limit the maximum frames per second by 60
	config->flags           = EFL_NOTEARING;      //Desktop PC: Enable VSync to prevent tearing in displaying frames
	config->jogtiltarrow    = true;               //Emulate the UIQ jog as left/right arrows
	return(E_OK);
}

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	ClassEStd::StrCpy(config->caption, "Blastar");
	config->icon            = IDI_MAIN;           //Application icon
	config->enginestatus    = true;               //Desktop PC: Enable the status bar and status information icon
	config->engineconsole   = false;              //Desktop PC: Disable the EDGELIB console
	config->width           = XRES;               //Desktop PC: Custom display width
	config->height          = YRES;               //Desktop PC: Custom display height
	
	//Determine screen orientation
	if (XRES < YRES)
		config->orientation = DOR_PORTRAIT;
	else if (XRES > YRES)
		config->orientation = DOR_LANDSCAPE;
	else
		config->orientation = DOR_STARTUP;
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	shared.ecd = &ecd;
	if (!InitModules())
	{
		SetErrorMsg("Error: Module init");
		return(E_ERROR);
	}
    LoadCfg();
	return(E_OK);
}

//Callback: Called when the program minimizes
ERESULT ClassMain::OnMinimize(void)
{
	if (shared.module < MAX_MODULES)
		if (cmodule[shared.module] != NULL)
			cmodule[shared.module]->OnMinimize();
	return(E_OK);
}

//Callback: Called when the program restores back from a minimized state
ERESULT ClassMain::OnRestore(void)
{
	if (shared.module < MAX_MODULES)
		if (cmodule[shared.module] != NULL)
			cmodule[shared.module]->OnRestore();
	return(E_OK);
}

//Callback: Called when the display mode has changed, use this to initialize your surfaces, etc.
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
	return(E_OK);
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	if (shared.module < MAX_MODULES)
		if (cmodule[shared.module] != NULL)
			cmodule[shared.module]->OnNextFrame(&display->buffer, &shared.module, &shared.modulemsg, timedelta);
	if (shared.module == MODULE_CLEANUP)
		Quit();
	if (shared.forceminimize)
	{
		shared.forceminimize = false;
		Minimize();
	}
	return(E_OK);
}

//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
	SaveCfg();
}
