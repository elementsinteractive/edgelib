:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Getting started with Gizmondo

## Required tools and SDKs
When targeting the Gizmondo device, you need Kingcdr's KGSDK for developing homebrew. The SDK contains Embedded Visual Studio 4.0, and specific headers and libraries for Gizmondo. The SDK can be obtained at the [Gizmondo forums](http://www.gizmondoforums.com). When building with this SDK, your Gizmondo application is only able to run on Gizmondo developer units or Gizmondo devices patched with [Service Pack B](http://www.takingthingsapart.org/index.php?option=com_content&task=view&id=32&Itemid=38) to run homebrew.

After downloading the SDK, unpack it to a temporary location. Install Embedded Visual studio 4.0 including service pack 4. After that, install the SDK for EVC++ which will install the header and library files for Gizmondo. To build for this device use the EDGELIB Builder tool. A tutorial on how to set up the EDGELIB Builder tool [ is available](tutorials_edgeide.md).

## Creating a .cpp source code file
Create a new folder: `C:\projects\helloworld` which will contain all project files. Also create a new folder: `C:\projects\helloworld\code` to contain the source code. Create a new helloworld.cpp file and edit it with a text editor. Paste the following source code sample and save the sourcefile:

_helloworld.cpp (89 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Code/helloworld.cpp
// One of the EDGELIB tutorial samples for Gizmondo
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
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
    config->icon = IDI_MAIN;
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

## Set up a Gizmondo EDGELIB Builder project
Open the EDGELIB Builder and select project -> new. Choose a location to save your project. Create a new folder and save it in `c:\projects\helloworld\workspace_gizmondo`. Click once on "untitled" in the project tree to rename it to Hello World, the name of the project. Then add helloworld.cpp to the source files folder in the project tree. Right click on this folder and select "add files to folder". Browse to helloworld.cpp and open it to add it to the project tree. Save the project file, choose "Gizmondo" as the target platform and click build -> rebuild all. The folder `c:\projects\helloworld\workspace_gizmondo\release\gizmondo` contains an .exe file. This file can be copied to an SD Card with a launcher program to run homebrew through the Gizmondo menu. For more information on how to run homebrew from an SD Card, check the [launcher guide](http://www.gizmondoforums.com/forums/index.php?showtopic=8222&hl=launcher).

## Download tutorial project
Here is a [sample that can be downloaded](files/helloworld_gizmondo.zip) which contains the result of this tutorial.

## Getting started tutorials
* [EDGELIB installation](tutorials_gettingstarted_edgeinstallation.md)
* [Getting started with Apple iOS](gettingstarted_iphone.md)
* [Getting started with Google Android](gettingstarted_android.md)
* [Getting started with Symbian](gettingstarted_symbian.md)
* [Getting started with Windows Mobile](gettingstarted_windowsmobile.md)
* [Getting started with Maemo 5](gettingstarted_maemo5.md)
* [Getting started with Antix Game Player](gettingstarted_antix.md)
* [Getting started with Windows desktop](gettingstarted_desktop.md)
* [Getting started with Linux desktop](gettingstarted_linux.md)
* [Getting started with Mac OS X](gettingstarted_macosx.md)
* [Getting started with GP2X](gettingstarted_gp2x.md)
* **Getting started with Gizmondo**
* [Multi-platform considerations](gettingstarted_multiplatform.md)

