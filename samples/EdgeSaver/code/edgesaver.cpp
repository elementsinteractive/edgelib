// code/edgesaver.cpp
// One of the Edge samples
// Uses advanced techniques by accessing the backbuffer directly
// The final executable may be renamed to .scr to use it as a
// Windows Desktop screensaver
//
// Shows 5 different screensaver animations
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Include and link the library                                    //
/////////////////////////////////////////////////////////////////////

//No optimized blitters needed
#define EDGERENDER_LITE

//Include Edge
#include "edgemain.h"

//Link the Edge static library
#pragma comment(lib, "edge.lib")

//Include internal resources for native Win32 builds
#if defined(DEVICE_WIN32)
#include "resource.h"
#else
#define IDI_MAIN 0
#endif

/////////////////////////////////////////////////////////////////////
// Global definitions                                              //
/////////////////////////////////////////////////////////////////////

#define MAX_STATES          5
#define MAX_PALETTES        3

/////////////////////////////////////////////////////////////////////
// Other includes                                                  //
/////////////////////////////////////////////////////////////////////

//Include settings and framework
#include "settings.h"
#include "main.h"

/////////////////////////////////////////////////////////////////////
// Edge entry point                                                //
/////////////////////////////////////////////////////////////////////

ClassEdge *EdgeMain(EDGESTARTUP *data)
{
	if (ClassEStd::StrLen(data->commandline))
	{
		if (data->commandline[0] == '/')
		{
			if (data->commandline[1] == 'c')
			{
				#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
					HWND fromwnd = (HWND)ClassEStd::StrToInt(&data->commandline[3]);
					DialogBox((HINSTANCE)data->instance, MAKEINTRESOURCE(IDD_MAIN), fromwnd, WinProc);
				#endif
			}
			if (data->commandline[1] != 's')
				return(NULL);
		}
	}
	return(new ClassMain);
}
