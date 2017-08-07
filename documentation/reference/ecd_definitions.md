:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Definitions

## EDGELIB flag options
```c++
EFL_NONE            //No flags 
EFL_MANUALFLIP      //Flip each frame manually 
EFL_GETSHADOWKEYS   //Disable the filtering of shadow keys 
EFL_NOTEARING       //Synchronize flip with the refresh rate of the display 
EFL_NOINTERNALFONT  //Add this flag when not using EDGELIB's internal font 
EFL_SCREENCAPTURE   //Capture the screen inside the backbuffer on startup 
EFL_TILTEMULATOR    //Enable tilt device emulation at startup 
EFL_RUNMINIMIZED    //Allow running in the background 
EFL_MORECLICKMOVE   //Get clickdevice movement events when outside of the window  
```

Flags passed to the EDGELIB configuration before the application starts. EFL_NOTEARING is currently only available for Windows desktop. Shadow keys are specific keycodes that are sent in combination with the hardware buttons on some devices. Minimized running expects focussuspend to be disabled as well.

## Open methods
```c++
DOM_DEFAULT     //Take the fastest and stable display method 
DOM_FRAMEBUFFER //Try to open the raw framebuffer directly 
DOM_GDI         //Use the Symbian bitmap graphics driver interface, or GDI for Windows 
DOM_DIRECTDRAW  //Use DirectDraw 
DOM_DSA         //Use DirectScreenAccess for Symbian 
DOM_GAPI        //Use GAPI on Windows Mobile  
```

We recommend using DOM_DEFAULT in most cases. For Windows Mobile you may need to use GAPI to stretch the screen on high resolution (VGA) devices. If the display can't be opened in the preferred method, EDGELIB tries to open the display using the other methods. The DOM_FRAMEBUFFER method uses an undocumented API in Symbian and only works on devices running Symbian 6.0 to 8.1. It may not work on all devices. It has been tested successfullt on the following devices:

* Nokia 7710
* Nokia N-Gage
* Sony-Ericsson P900

## Version changes
- EDGELIB 3.70: Added EFL_MORECLICKMOVE 
- EDGELIB 3.60: Changed EFL_NOTILTEMU into EFL_TILTEMULATOR, tilt device emulation is now disabled by default 
- EDGELIB 3.10: Removed EFL_NOSYMBIANDSA, removed resolution and flip method options, added open methods 
- EDGELIB 2.70: Added EFL_NOSYMBIANDSA 
- EDGELIB 2.40: Added EFL_NOTILTEMU and enabled manual flipping in the evaluation version 
- EDGELIB 2.10: Added EFL_SCREENCAPTURE, ERS_DEVICEDEFAULT, ERS_EMULATELOWRES, ERS_REQUIREHIGHRES, EFM_DIRECTCOPY and EFM_BITMAP

