// code/blastar.cpp
// One of the Edge tutorial samples
//
// A simple shoot-em-up game
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

//Platform dependent definitions
#include "def_platf.h"

//Include the framework class
#include "main.h"


/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////

ClassEdge *EdgeMain(EDGESTARTUP *data){ return(new ClassMain); }
