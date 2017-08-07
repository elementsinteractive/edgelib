/////////////////////////////////////////////////////////////////////
// code/filedemo.cpp
// One of the Edge samples
// Handling of files: reading graphics from Edge packs, read and
// show the content of the program folder and write the list to a
// new file
//
// Shows a list of files in the program folder and write the list to
// log.txt
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
