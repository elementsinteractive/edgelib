:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Getting started with GP2X

## Required tools and SDKs
When targetting the GP2X platform, you need the [devkitPro GP2X SDK](http://sourceforge.net/project/showfiles.php?group_id=114505&package_id=150769&release_id=372057). The SDK contains all headers and libraries and includes gcc as well, an open source c++ compiler. Run the executable to install the contents of the SDK to c:\devkitGP2X. To build for this platform, use gcc through the commandline or use the EDGELIB Builder tool (formerly Edge IDE). We recommend using the EDGELIB Builder tool for building GP2X applications. A tutorial on how to set up the EDGELIB Builder tool [is available](tutorials_edgeide.html).

## Creating a .cpp source code file
Create a new folder: `C:\projects\helloworld` which will contain all project files. Also create a new folder: `C:\projects\helloworld\code` to contain the source code. Create a new helloworld.cpp file and edit it with a text editor. Paste the following source code sample and save the source file:

_helloworld.cpp (72 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Code/helloworld.cpp
// One of the EDGELIB tutorial samples for GP2X
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
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

## Set up a GP2X EDGELIB Builder project
Open the EDGELIB Builder and select project -> new. Choose a location to save your project. Create a new folder and save it in `c:\projects\helloworld\workspace_gp2x`. Click once on "untitled" in the project tree to rename it to Hello World, the name of the project. Then add helloworld.cpp to the source files folder in the project tree. Right click on this folder and select "add files to folder". Browse to helloworld.cpp and open it to add it to the project tree. Save the project file, choose "GP2X" as the target platform and click build -> rebuild all. The folder `c:\projects\helloworld\workspace_gp2x\release\gp2x` contains a .gpe file. This file can be copied to a GP2X device and can be executed from the menu.

## Using the commandline
The commandline tools can be found in the bin folder of the GP2X SDK. Open a command window in the same folder as the .cpp source file and execute the following commands to compile an object file: 
`set path=%path%;c:\devkitGP2X\bin` 
`arm-linux-g++ -O3 -I c:\projects\edge\include -D__LINUX__ -D__GP2X__ -c helloworld.cpp -o helloworld.obj`

This will link the object file and libraries into the executable file and strips the executable to decrease its filesize: 
`arm-linux-g++ helloworld.obj --whole-archive -static -ledge -ledgerender -lz --no-whole-archive -L c:\projects\edge\lib\gp2x -o helloworld.gpe`

## Notes
At the moment, a GP2X application will hang the device when a floating point datatype is defined, or a floating point calculation is executed. Be careful when using functionality that's not part of the EDGELIB SDK. To add an icon to a GP2X application, create a 32x32 pixel PNG file with the same name as the .gpe executable. It should also be located in the same folder.

## Download tutorial project
Here is a [sample that can be downloaded](files/helloworld_gp2x.zip) which contains the result of this tutorial.

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
* [Getting started with Mac OS X](getting_started_macosx.md)
* **Getting started with GP2X**
* [Getting started with Gizmondo](getting_started_gizmondo.md)
* [Multi-platform considerations](getting_started_multiplatform.md)

