:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Getting started with Linux desktop

## Required tools and SDKs
For Linux desktop you can develop with an IDE such as Eclipse, use makefiles or use a shell script to compile your source. In this sample we use a shell script. EDGELIB requires X11 libraries which needs to be installed on the Linux desktop. For this sample we assume EDGELIB is installed on the desktop folder.

If you want to create a Linux build from a Windows desktop system, you need to use a virtual machine to emulate Linux. You can get VMWare player from [www.vmware.com](http://www.vmware.com). Also download a Linux image from VMWare (we recommend Ubuntu).

## Installation
To install the X11 libraries, open the terminal and give the following command:

```c++
apt-get install xserver-xorg-dev
```

## Create a helloworld example
We're going to create a working sample for Linux desktop. Create `helloworld.cpp` and paste the following code inside the editor window:

_helloworld.cpp (88 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Code/helloworld.cpp
// One of the EDGELIB tutorial samples for Linux desktop
//
// Copyright (c) 2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Show "hello world" on the screen and exit by tapping the screen or
// hitting a key
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
    public:
        ClassMain(void);
        ~ClassMain(void);
        ERESULT OnDisplayConfig(EDISPLAYCONFIG *config);
        ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
        void OnButtonDown(unsigned long bnr, EBUTTONLIST *blist);
        void OnStylusDown(POINT pnt);
};
 
 
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
 
//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
    ClassEStd::StrCpy(config->caption, "Hello World!");
    return(E_OK);
}
 
//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
    display->buffer.DrawFont(0, 0, &display->fontinternal, "Hello World!");
    return(E_OK);
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

This sample code has been written specifically for Linux desktop. A multi-platform version can be found at the multi-platform section.

## Creating a shell script
We're going to create a shell script file which compiles and helloworld.cpp source file and links it into an executable. Create a build.sh file and paste the following code:

_build.sh (12 lines)_
```c++
echo Compiling...
echo helloworld.cpp
g++ -O3 -I ../../../include -D__LINUX__ -c ../code/helloworld.cpp -o helloworld.o
 
echo
 
echo Linking...
g++ helloworld.o --whole-archive -ledge -ledgerender -lz -lX11 --no-whole-archive -L $HOME/Desktop/edge/lib/linux-x86 -o helloworld
 
echo
 
echo helloworld
```

## Getting started tutorials
* [EDGELIB installation](tutorials_gettingstarted_edgeinstallation.md)
* [Getting started with Apple iOS](gettingstarted_iphone.md)
* [Getting started with Google Android](gettingstarted_android.md)
* [Getting started with Symbian](gettingstarted_symbian.md)
* [Getting started with Windows Mobile](gettingstarted_windowsmobile.md)
* [Getting started with Maemo 5](gettingstarted_maemo5.md)
* [Getting started with Antix Game Player](gettingstarted_antix.md)
* [Getting started with Windows desktop](gettingstarted_desktop.md)
* **Getting started with Linux desktop**
* [Getting started with Mac OS X](gettingstarted_macosx.md)
* [Getting started with GP2X](gettingstarted_gp2x.md)
* [Getting started with Gizmondo](gettingstarted_gizmondo.md)
* [Multi-platform considerations](gettingstarted_multiplatform.md)

