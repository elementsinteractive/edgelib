// ccamera.cpp
// Edgecore: provides camera API
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED

#include "estdc.h"
#include "esound.h"
#include "eloader.h"
#include "edisplay.h"
#include "einput.h"
#include "ememory.h"
#include "evtimer.h"
#include "efile.h"
#include "emath.h"
#include "eini.h"
#include "ereg.h"
#include "econsole.h"
#include "edevice.h"
#include "edgeframe.h"
#include "ecamera.h"
#include "edgecore.h"

#if defined(DEVICE_SERIES60) && (DEVICE_SYMBIAN >= 900)
/* fixme: exclude Series60-1.ed and early Series60-2.ed. They do not have camera support
          Only 3rd+ and the later 2.ed have it*/
    #include "extapi/symbiancam.h"

//#elif defined(DEVICE_WINMOBILE)

/* fixme: may cause build environment dependencies due to the direct linkage to DirectShow classes */
//    #include "core/extapi/pocketpccam.h"

//#elif defined(DEVICE_WINDOWS) && defined(DEVICE_DESKTOP)

/* fixme: requires non-free VS */
//    #include "core/extapi/pocketpccam.h"

#elif defined(DEVICE_WIN32)
	#if defined(EDGE_WINCAMLIB)    
		#include "extapi/pocketpccam.h"
	#else
		#include "extapi/windowscam.h"
	#endif

#elif defined(DEVICE_IPHONE) && !defined(DEVICE_IPHONEEMU)
	
    /* implementation in iphone4camera.mm */

#elif defined(DEVICE_LINUX)

    // Linux & linux embedded (maemo)
    #include "extapi/linuxcam.h"

#else

    ECOREOBJ ecCamCreate(ECOREOBJ framework, ECOREOBJ frontend)
    {
        return NULL;
    }
    
    void ecCamDelete(ECOREOBJ camera)
    {
    }
    
    int ecCamDeviceCount(void)
    {
        return 0;
    }
    
    unsigned long ecCamRequestCamera(ECOREOBJ camera, int index)
    {
        return E_UNSUPPORTED;
    }

    unsigned long ecCamStartCapture(ECOREOBJ camera)
    {
        return E_ERROR;
    }

    unsigned long ecCamStopCapture(ECOREOBJ camera)
    {
        return E_ERROR;
    }

    int ecCamGetResolutions(ECOREOBJ camera)
    {
        return 0;
    }

    void ecCamGetResolution(ECOREOBJ camera, int index, ECOREOBJ mode)
    {
    }

    unsigned long ecCamSetResolution(ECOREOBJ camera, int index)
    {
        return E_ERROR;
    }
    
    int ecCamGetOrientation(ECOREOBJ camera)
    {
        return ELOC_UNKNOWN;
    }

    int ecCamGetState(ECOREOBJ camera)
    {
        return ECS_INVALID;
    }

    int ecCamGetCurrentResolution(ECOREOBJ camera)
    {
        return 0;
    }

	int ecCamGetIndex(ECOREOBJ camera)
	{	
		return 0;
	}


#endif
