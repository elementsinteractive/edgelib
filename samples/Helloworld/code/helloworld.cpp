/////////////////////////////////////////////////////////////////////
// code/helloworld.cpp
// One of the EDGELIB samples
// Set up a basic application framework using EDGELIB
//
// Show "hello world" on the screen and exit by clicking/tapping the
// window or hitting a key
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Include and link the library                                    //
/////////////////////////////////////////////////////////////////////

//Include EDGELIB
#include "edgemain.h"

//Link the EDGELIB static library
#pragma comment(lib, "edge.lib")

//Contains ClassMain, the application framework
#include "main.h"

/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////

ClassEdge *EdgeMain(EDGESTARTUP *data){ return(new ClassMain); }
