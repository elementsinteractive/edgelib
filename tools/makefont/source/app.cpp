// app.cpp
// Makefont tool
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Include and link the library                                    //
/////////////////////////////////////////////////////////////////////

//Include resources
#include "resource.h"

//#include <afxwin.h>
//#pragma comment(lib, "uafxcwd.lib")

//Include and link EDGELIB
#pragma comment(lib, "edge.lib")
#include "edgemain.h"

//Contains ClassMain, the application framework
#include "interface.h"

/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////

ClassEdge *EdgeMain(EDGESTARTUP *data){ return(new ClassMain); }
