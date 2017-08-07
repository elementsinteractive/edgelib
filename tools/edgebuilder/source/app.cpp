// app.cpp
// Main application file: opens Edge projects for easy compilation
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Todo:
//+ Environment variabelen kunnen zien/toevoegen
//. Libjes toevoegen

//SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, project.loadfolder);

//Link Edge
#include "edgemain.h"
#pragma comment(lib, "edge.lib")

//Windows headers
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "shfolder.lib")
#include <commdlg.h>
#include <shellapi.h>
#include <shlobj.h>
#include <direct.h>
#include <commctrl.h>
#include <shfolder.h>

//Resource definitions
#include "resource.h"

//Definitions
#include "def_main.h"

//Program headers
#include "filetree.h"
#include "env.h"
#include "compiler.h"
#include "main.h"

//Entry point
ClassEdge *EdgeMain(EDGESTARTUP *data)
{
	main.OpenDialog(data);
	return(NULL);
}
