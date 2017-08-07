:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Creating the framework

## Preparing ClassMain
The main class of this tutorial will be based on the helloworld sample of the getting started tutorial. An event will be added to fetch tiltcontrol data as well as two variables to save the tilt state of the device. Also, the emulator will be disabled in the EDGELIB configuration:

_tiltcontrol.cpp (108 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Code/tiltcontrol.cpp
// One of the EDGELIB tutorial samples for Windows Mobile
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Demonstrates the use of the TiltCONTROL device
/////////////////////////////////////////////////////////////////////
 
 
/////////////////////////////////////////////////////////////////////
// Include and link the library                                    //
/////////////////////////////////////////////////////////////////////
 
//Include EDGELIB
#include "edgemain.h"
 
//Link the EDGELIB static library
#pragma comment(lib, "edge.lib")
 
//Include internal resources
#include "resource.h"
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////
 
//The main class
class ClassMain : public ClassEdge
{
    private:
        long tiltx;
        long tilty;
    public:
        ClassMain(void);
        ~ClassMain(void);
        ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
        ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
        ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
        void OnDeviceTilt(long x, long y);
        void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
        void OnStylusDown(POINT pnt);
};
 
 
/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////
 
//ClassMain: constructor
ClassMain::ClassMain(void)
{
    tiltx = 0;
    tilty = 0;
}
 
//ClassMain: destructor
ClassMain::~ClassMain(void)
{
}
 
//Configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
    config->flags |= EFL_NOTILTEMU;
    return(E_OK);
}
 
//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
    ClassEStd::StrCpy(config->caption, "TiltCONTROL game");
    config->icon = IDI_MAIN;
    return(E_OK);
}
 
//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
    return(E_OK);
}
 
//Callback: Called 10 times per second for the state of the TiltCONTROL device
void ClassMain::OnDeviceTilt(long x, long y)
{
    tiltx = x;
    tilty = y;
}
 
//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
    Quit();
}
 
//Callback: Called when the user points the stylus down or clicks the left mouse button
void ClassMain::OnStylusDown(POINT pnt)
{
    Quit();
}
 
 
/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////
 
ClassEdge *EdgeMain(EDGESTARTUP *data){ return(new ClassMain); }
```

The positions of tilt will be set after receiving the OnDeviceTilt() event which will be used in OnNextFrame() later.

## Check the state of the tilt device
Before starting the application, we should check if the TiltCONTROL device is connected properly. This will be done in the new OnInit() event:

_tiltcontrol.cpp (4 lines)_
```c++
        ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
        ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
        ERESULT OnInit(ENATIVETYPE instance);
        ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
```

_tiltcontrol.cpp (11 lines)_
```c++
//Callback: Called before opening the application
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
    if (ecd.input->GetTiltData(&ecd) != E_OK)
    {
        SetErrorMsg("TiltCONTROL device not found or not connected.");
        return(E_ERROR);
 
    }
    return(E_OK);
}
```

And show the x/y position of tilt on the screen:

_tiltcontrol.cpp (7 lines)_
```c++
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
    display->buffer.FillRect(NULL, ECOLOR_BLACK);
    display->buffer.DrawFont(0, 0, &display->fontinternal, tiltx);
    display->buffer.DrawFont(30, 0, &display->fontinternal, tilty);
    return(E_OK);
}
```

This is a sample on how to read the state of the tilt device and show it onscreen. Go to the next chapter to create a simple game.

## Chapters
* **Creating the framework**
* [Making a simple game](tutorials_tiltcontrol_simplegame.md)

