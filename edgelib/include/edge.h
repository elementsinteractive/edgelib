// edge.h
// The multi-platform game development library supprting many
// mobile and desktop platforms.
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition
///////////////////////////////////////////////////////////////////

#ifndef EDGE_INCLUDED //Make sure the library isn't included multiple times
#define EDGE_INCLUDED

///////////////////////////////////////////////////////////////////
// Includes
///////////////////////////////////////////////////////////////////

//SDK and device definitions
#include "edgesdkdef.h"
#include "edgedevice.h"

//EDGELIB definitions
#include "edgedef.h"

///////////////////////////////////////////////////////////////////
// Engine module includes
///////////////////////////////////////////////////////////////////

#include "eraw.h"       //Raw constant data
#include "eloader.h"    //Loader base class + included loaders
#include "estdc.h"      //The standard class
#include "ereg.h"       //The registration module
#include "emath.h"      //Fixed point math module
#include "ememory.h"    //The memory manager module
#include "efile.h"      //The file wrapper module
#include "eini.h"       //The ini-file module
#include "evtimer.h"    //The virtual timer module
#include "edisplay.h"   //The display & graphics module
#include "esound.h"     //The sound wrapper module
#include "einput.h"     //The input module
#include "econnect.h"   //The connection module
#include "edevice.h"    //The device module
#include "econsole.h"   //The console module
#include "ecamera.h"    //The camera module
#include "edgeframe.h"  //The EDGELIB framework

///////////////////////////////////////////////////////////////////
// Extended API includes
///////////////////////////////////////////////////////////////////

#include "extapi/netapi.h"   //Extended network API's
#include "extapi/dataapi.h"  //Extended data API's
#include "extapi/xmlapi.h"   //Extended XML API's

#endif
