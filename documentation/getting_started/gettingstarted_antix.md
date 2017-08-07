:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Getting started with Antix Game Player

## Required tools and SDKs
Before you can target the Antix Game Player ("Antix") platform, you need to go through a [registration procedure](https://developer.antixlabs.com/). Once you have been approved, you can download the Antix GDK. This SDK contains all headers and libraries, as well as the various tools needed to build applications. Run the GDK installer to install the contents of the SDK to its default location. To build for this platform, use gcc through the command line or use the EDGELIB Builder tool. We recommend using the EDGELIB Builder tool for building Antix applications. A tutorial on how to set up the EDGELIB Builder tool [is available](tutorials_edgeide.html).

## Creating a .cpp source code file
Create a new folder: `C:\projects\helloworld` which will contain all project files. Also create a new folder: `C:\projects\helloworld\code` to contain the source code. Create a new helloworld.cpp file and edit it with a text editor. Paste the following source code sample and save the source file:

_helloworld.cpp (72 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Code/helloworld.cpp
// One of the EDGELIB tutorial samples for Antix
//
// Copyright (c) 2011-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Show "hello world" on the screen and exit by hitting a key
/////////////////////////////////////////////////////////////////////
 
/////////////////////////////////////////////////////////////////////
// Include and link the library                                    //
/////////////////////////////////////////////////////////////////////
 
//Include EDGELIB
#include "edgemain.h"
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////
 
//The main class
class ClassMain : public ClassEdge
{
    public:
        ClassMain(void);
        ~ClassMain(void);
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

## Set up an Antix EDGELIB Builder project
Open the EDGELIB Builder and select project -> new. Choose a location to save your project. Create a new folder and save it in `c:\projects\helloworld\workspace_antix`. Click once on "untitled" in the project tree to rename it to Hello World, the name of the project. Then add helloworld.cpp to the source files folder in the project tree. Right click on this folder and select "add files to folder". Browse to helloworld.cpp and open it to add it to the project tree. Save the project file, choose "Antix" as the target platform and click build -> rebuild all.

## Signing builds
EDGELIB Builder can create signed Antix components. For this process to work, you will need to sign in to Antix and request a signing package. Put a copy of the downloaded file in tools/edgebuilder/resources/ under the name namestub_keys.zip. The corresponding password can be entered in tools/edgebuilder/platforms/antix.ini

## Getting started tutorials
* [EDGELIB installation](tutorials_getting_started_edgeinstallation.md)
* [Getting started with Apple iOS](getting_started_iphone.md)
* [Getting started with Google Android](getting_started_android.md)
* [Getting started with Symbian](getting_started_symbian.md)
* [Getting started with Windows Mobile](getting_started_windowsmobile.md)
* [Getting started with Maemo 5](getting_started_maemo5.md)
* **Getting started with Antix Game Player**
* [Getting started with Windows desktop](getting_started_desktop.md)
* [Getting started with Linux desktop](getting_started_linux.md)
* [Getting started with Mac OS X](getting_started_macosx.md)
* [Getting started with GP2X](getting_started_gp2x.md)
* [Getting started with Gizmondo](getting_started_gizmondo.md)
* [Multi-platform considerations](getting_started_multiplatform.md)

