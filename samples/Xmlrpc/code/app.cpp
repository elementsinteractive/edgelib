// code/app.cpp
// One of the EDGELIB samples
//
// Loads a formatted XML-RPC file and shows a list of nodes
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Include and link
/////////////////////////////////////////////////////////////////////

//Include EDGELIB
#include "edgemain.h"

//Link the EDGELIB static library
#pragma comment(lib, "edge.lib")

//Include internal resources for native Win32 builds
#if defined(DEVICE_WIN32)
#include "resource.h"
#endif

//Include framework
#include "main.h"


/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////

ClassEdge *EdgeMain(EDGESTARTUP *data){ return(new ClassMain); }
