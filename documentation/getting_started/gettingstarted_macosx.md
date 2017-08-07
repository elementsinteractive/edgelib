:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Getting started with Mac OS X desktop

## Downloading XCode
Obtain XCode 4 from the Apple Developer website. This will require you to register an Apple ID. You can also use your existing Apple ID if you have one. After logging in you can download XCode 4 to your Mac device. The SDK comes with the XCode programming environment, Mac OS X headers and libraries, and iPhone headers and libraries in one package. When installing make sure that you have the Mac OS X SDK selected

## Creating a sample project
Start XCode, and create a new project. Under Mac OS X, Application, select Cocoa Application and click next. Supply HelloWorld as the name, and nl.edgelib.samples as the company identifier. You can replace these for your own project. 
Once you have the project created, remove the following files from the project: main.m (Appname)AppDelegate.h, (Appname)AppDelegate.m, and MainWindow.xib. Open (AppName)-Info.plist and clear the "Main nib file base name" property. Add a new file, select C++ File under the Mac OS X, C and C++ tab, click next, then save as Main. XCode will add a .h and .cpp file to the project. 

Replace the contents of main.h and main.cpp with the following:

_main.h (35 lines)_
```c++
//
//  Main.h
//  HelloWorld
//  Created by Marcel Sondaar on 7/26/11.
//
// Copyright (c) 2017 Elements Interactive B.V.
// http://www.edgelib.com
//
 
#include "edge.h"
 
class ClassMain : public ClassEdge
{
public:
    ClassMain(void);
    ~ClassMain(void);
    ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config);
    ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
    ERESULT OnInit(ENATIVETYPE instance);
    ERESULT OnMinimize(void);
    ERESULT OnRestore(void);
    ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display);
    void OnDisplayChange(ClassEDisplay *display);
    ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
    void OnExit(ENATIVETYPE instance);
    void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
    void OnButtonUp(unsigned long bnr, EBUTTONLIST *blist);
    void OnWheel(long direction);
    void OnStylusUp(POINT pnt);
    void OnStylusDown(POINT pnt);
    void OnStylusDouble(POINT pnt);
    void OnStylusMove(POINT pnt);
    void OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer);
};
/
```

_main.cpp (178 lines)_
```c++
//
//  Main.cpp
//  HelloWorld Sample
//
// Copyright (c) 2017 Elements Interactive B.V.
// http://www.edgelib.com
//
 
#include "Main.h"
 
 
#include "edgemain.h"
 
/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////
 
ClassEdge *EdgeMain(EDGESTARTUP *data){ return(new ClassMain); }
 
 
/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               //
/////////////////////////////////////////////////////////////////////
 
//ClassMain: constructor
ClassMain::ClassMain(void)
{
}
 
//ClassMain: destructor
ClassMain::~ClassMain(void)
{
}
 
//Configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
    config->ffwdkey         = 'A';                //A key for fast forwarding (multiplies framerate by 2.5)
    config->slomokey        = 'S';                //A key for slow motion     (halves framerate)
    config->maxfps          = 60;                 //Limit the maximum frames per second by 60
    config->flags           = 0;                  //No extra flag options
    config->numconnections  = 1;                  //Keep 1 active network connection
#if defined(DEVICE_GP2X)
    config->smartfps    = false;              //GP2X: No smartfps needed
#else
    config->smartfps    = true;               //Other devices: Enable smart fps lock managing
#endif
#if defined(DEVICE_DESKTOP)
    config->focusminimize = false;            //Desktop PC: Don't minimize the application when the focus is lost
#else
    config->focusminimize = true;             //Mobile devices: Minimize the application when the focus is lost
#endif
    config->jogtiltarrow    = true;               //Emulate the UIQ jog as left/right arrows
    config->focussuspend    = true;               //Desktop PC: suspend the application when the window focus is lost
    config->backlighton     = true;               //Keep backlight on
    config->netlogging      = true;               //Enable logging of network events in the console
    config->lockkeys        = true;               //Lock keys for use in this application
    config->keysounds       = false;              //Disable key 'click' sounds for smartphones
    
    return(E_OK);
}
 
//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
    ClassEStd::StrCpy(config->caption, "Hello World!");
#if defined(IDI_ICON1)
    config->icon            = IDI_ICON1;           //Application icon
#endif
    config->menu            = 0;                  //Application contains no Windows Menu
    config->dialogbox       = 0;                  //Don't start from dialogbox (Desktop only)
    config->width           = 1024;                //Desktop PC: Custom display width
    config->height          = 768;                //Desktop PC: Custom display height
    config->videotype       = EDSP_DEFAULT;       //Choose the default display mode
    config->emulvideotype   = EDSP_DEFAULT;       //No color depth emulation
    config->orientation     = DOR_AUTO;           //Change orientation to match the settings of the device
    config->openmethod      = DOM_DEFAULT;        //Take recommended (fastest) display open method
    config->surfacedefault  = EST_SYSMEMORY;      //Create surfaces in system memory by default, don't use DirectDraw or Symbian bitmap surfaces
#if defined(DEVICE_DESKTOP)
    config->fullscreen  = false;              //Windowed application for Desktop
    config->fullscreenmouse = true;           //Desktop PC: Enable the mouse cursor for fullscreen applications
#else
    config->fullscreen  = true;               //Fullscreen application for mobile devices
#endif
    config->dspnative       = true;               //If possible, try to use native blitters (like DirectDraw)
    config->enginestatus    = true;               //Desktop PC: Enable the status bar and status information icon
    config->engineconsole   = false;              //Desktop PC: Enable the edge console
    config->surfprerotate   = true;               //Use pre-rotated surfaces to increase speed in other orientations
    config->stencil      = false;          //Try to get a stencil buffer
    config->emulatelowres   = 0;                  //The device is not to emulate an older device
    config->supportedorientations = 0xF;//B;
    return(E_OK);
}
 
//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
    return(E_OK);
}
 
//Callback: Called when the program minimizes
ERESULT ClassMain::OnMinimize(void)
{
    return(E_OK);
}
 
//Callback: Called when the program restores back from a minimized state
ERESULT ClassMain::OnRestore(void)
{
    return(E_OK);
}
 
//Callback: Called when the display mode has changed, use this to initialize your surfaces, etc.
ERESULT ClassMain::OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
{
    return(E_OK);
}
 
//Callback: Called when the display has been changed (because of a change in orientation)
void ClassMain::OnDisplayChange(ClassEDisplay *display)
{
}
 
//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{   
    display->buffer.DrawFont(2, 2, &display->fontinternal, "Hello World!");
    display->buffer.DrawFont(2, display->GetHeight() - 2, &display->fontinternal, "Button/Stylus: Quit", EFX_COLORKEY | EFO_VBOTTOM);
    
    return(E_OK);
}
 
//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
}
 
//Callback: Called when the user pressed a key or button
void ClassMain::OnButtonDown(unsigned long bnr, EBUTTONLIST *blist)
{
    Quit();
}
 
//Callback: Called when the user released a key or button
void ClassMain::OnButtonUp(unsigned long bnr, EBUTTONLIST *blist)
{
}
 
//Callback: Called when the user scrolls a wheel device
void ClassMain::OnWheel(long direction)
{
}
 
//Callback: Called when the user released the stylus or mouse button
void ClassMain::OnStylusUp(POINT pnt)
{
}
 
//Callback: Called when the user points the stylus down or clicks the left mouse button
void ClassMain::OnStylusDown(POINT pnt)
{
    Quit();
}
 
//Callback: Called when the user points the stylus 2 times down quickly or double clicks the left mouse button
void ClassMain::OnStylusDouble(POINT pnt)
{
}
 
//Callback: Called when the user moves the stylus while pressed down, or moves the mouse cursor
void ClassMain::OnStylusMove(POINT pnt)
{
}
 
//Callback: Called when a network event occurs
void ClassMain::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
{
}
```

Next, we must tell XCode to build against EDGELIB. Select the project navigator on the left, click the HelloWorld label. In the main part, go to build settings, select all, and find the Header Search Paths option. Insert here the location of the edgelib header files (/path/to/edgelib/dist/include). Switch to the build phases tab, and expand the Link Binary box. Add libz.dylib, OpenGL.framework from the default list, and click Use Other to locate and add libedge.a and libedgerender.a, which can be found in the edgelib/dist/lib/macos/ folder.

Save all the changes using command+S for each edited file (modified files are shaded grey). You can then press the run button to build and start the application.

## Getting started tutorials
* [EDGELIB installation](tutorials_getting_started_edgeinstallation.md)
* [Getting started with Apple iOS](getting_started_iphone.md)
* [Getting started with Google Android](getting_started_android.md)
* [Getting started with Symbian](getting_started_symbian.md)
* [Getting started with Windows Mobile](getting_started_windowsmobile.md)
* [Getting started with Maemo 5](getting_started_maemo5.md)
* [Getting started with Antix Game Player](getting_started_antix.md)
* [Getting started with Windows desktop](getting_started_desktop.md)
* [Getting started with Linux desktop](getting_started_linux.md)
* **Getting started with Mac OS X**
* [Getting started with GP2X](getting_started_gp2x.md)
* [Getting started with Gizmondo](getting_started_gizmondo.md)
* [Multi-platform considerations](getting_started_multiplatform.md)

