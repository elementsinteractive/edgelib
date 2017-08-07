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
// Definitions                                                     //
/////////////////////////////////////////////////////////////////////

#define PS_PREPARE     0
#define PS_OPENFOLDER  1
#define PS_READFOLDER  2
#define PS_DONE        3 //Program states

#define MAX_LIST      20 //Maximum amount of files to list


/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////

//The main class
class ClassMain : public ClassEdge
{
	private:
		ClassEFile ef;
		E2DSurface font;
		#if defined(SDK_EDITION_FULL)
			E2DSurfaceRGBA pip;
		#else
			E2DSurface pip;
		#endif
		char listedfile[MAX_LIST][MAX_EFILESTR];
		unsigned char programstate;
		unsigned char listnr;
	public:
		ClassMain(void);
		~ClassMain(void);
		ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
		ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
		ERESULT OnInit(ENATIVETYPE instance);
		ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
		ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
		void OnExit(ENATIVETYPE instance);
		void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
		void OnStylusDown(POINT pnt);
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
	config->maxfps = 20; //Limit the maximum frames per second by 20
	return(E_OK);
}

//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
	ClassEStd::StrCpy(config->caption, "FileDemo");
	config->icon            = IDI_MAIN;           //Application icon
	config->width           = 240;                //Desktop PC: Custom display width
	config->height          = 320;                //Desktop PC: Custom display height
	config->enginestatus    = true;               //Desktop PC: Enable the status bar and status information icon
	config->engineconsole   = false;              //Desktop PC: Disable the edge console
	return(E_OK);
}

//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
	listnr = 0;
	programstate = PS_PREPARE;
	return(E_OK);
}

//Callback: Called when the display mode has changed, use this to initialize your surfaces, etc.
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
	//Open an Edge package and create 2 surfaces from the graphics within
	unsigned char *file_data;
	unsigned long file_datasize;
	bool success = true;
	if (ef.Open("res.epk", EFOF_READ | EFOF_READONLYPATH))
	{
		//Every Edge pack needs to be authorized with a unique code depending on the vendor signature
		//(The pack used in the sample has no vendor signature and can be authorized with code 0)
		if (ef.PackAuthorize(0))
		{
			//Load a fixed font
			if (ef.PackGetFileData(file_data, file_datasize, "font_fixed.bmp") == E_OK)
			{
				if (display->CreateSurface(&font, file_data, file_datasize) == E_OK)
				{
					font.SetFontCharacters(0, 127);
					font.BuildFont(false, ECOLOR_FUCHSIA);
				}
				else
					success = false;
				ClassEMemory::DeAlloc(file_data);
			}
			else
				success = false;

			//Load a bullet picture for the list
			if (ef.PackGetFileData(file_data, file_datasize, "pip.png") == E_OK)
			{
				if (display->CreateSurface(&pip, file_data, file_datasize) == E_OK)
				{
					#if !defined(SDK_EDITION_FULL)
						pip.AutoSetColorKey();
					#endif
				}
				else
					success = false;
				ClassEMemory::DeAlloc(file_data);
			}
			else
				success = false;
		}
		else
			success = false;
		ef.Close();
		if (!success)
		{
			SetErrorMsg("Couldn't load res.epk");
			return(E_ERROR);
		}
	}
	else
		return(E_ERROR);
	return(E_OK);
}

//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
	if (programstate == PS_PREPARE) //Prepare for folder reading, show a message on the screen
	{
		programstate = PS_OPENFOLDER;
		display->buffer.FillRect(NULL, ECOLOR_NAVY);
		display->buffer.DrawLine(0, 15, display->buffer.GetWidth(), 15, ECOLOR_WHITE);
		display->buffer.DrawFont(2, 2, &font, "Reading program folder...");
	}
	else if (programstate == PS_OPENFOLDER) //Open the program folder for reading
	{
		programstate = PS_READFOLDER;
		if (!ef.OpenFolder(ecd.dat->currentpath))
		{
			#if defined(SDK_EDITION_FULL)
				display->buffer.BltFast(2, 18 + listnr * 12, &pip, NULL);
			#else
				display->buffer.BltFast(2, 18 + listnr * 12, &pip, NULL, EFX_COLORKEY);
			#endif
			display->buffer.DrawFont(16, 18 + listnr * 12, &font, "Unable to open folder");
			listnr++;
			return(E_ERROR);
		}
	}
	else if (programstate == PS_READFOLDER) //Read a filename from the folder, keep reading until there is no more
	{
		char filename[MAX_EFILESTR];
		if (ef.ReadFolder(filename) && listnr < MAX_LIST)
		{
			ClassEStd::StrCpy(listedfile[listnr], filename);
			#if defined(SDK_EDITION_FULL)
				display->buffer.BltFast(2, 18 + listnr * 12, &pip, NULL);
			#else
				display->buffer.BltFast(2, 18 + listnr * 12, &pip, NULL, EFX_COLORKEY);
			#endif
			display->buffer.DrawFont(16, 18 + listnr * 12, &font, filename);
			listnr++;
		}
		else
		{
			RECT rc;
			ef.CloseFolder();
			SetRect(&rc, 0, 0, display->buffer.GetWidth(), 15);
			display->buffer.FillRect(&rc, ECOLOR_NAVY);
			display->buffer.DrawFont(2, 2, &font, "Done (press key to quit)");
			programstate = PS_DONE;
		}
	}
	return(E_OK);
}

//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
	unsigned long ctr;
	if (ef.New("log.txt"))
	{
		for (ctr = 0; ctr < listnr; ctr++)
		{
			ef.Write(listedfile[ctr], ClassEStd::StrLen(listedfile[ctr]));
			ef.Write((void *)"\r\n", 2);
		}
		ef.Close();
	}
}

//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
	Quit();
}

//Callback: Called when the user points the stylus down or clicks the left mouse button
void ClassMain::OnStylusDown(POINT pnt)
{
	Quit();
}
