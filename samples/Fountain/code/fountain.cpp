// code/fountain.cpp
// One of the Edge samples
// Load a sprite from a file and create a spritesheet dynamically
// draw animated graphics with math functions. Also shows how to use
// fixed point data.
//
// Shows a particle fountain with sprites and blend effects
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Include and link the library                                    //
/////////////////////////////////////////////////////////////////////

//Include Edge
#include "edgemain.h"

//Link the Edge static library
#pragma comment(lib, "edge.lib")

//Contains ClassMain, the application framework
#include "main.h"

/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////

ClassEdge *EdgeMain(EDGESTARTUP *data){ return(new ClassMain); }
