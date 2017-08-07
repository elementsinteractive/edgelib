// code/connect.cpp
// One of the Edge samples
// Demonstrates how to set up a network server, or how to connect
// a client to a server (including device searching when using
// bluetooth). Send and receive network data.
//
// Set up a server or connect as a client using TCP/IP or bluetooth.
// Send and receive network messages between client and server.
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

#if !defined(APP_UID)
	#define APP_UID 0x10205DA2
#endif


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
