:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Getting started with Maemo 5

## Required tools and SDKs
To get started, Download and install the [Maemo SDK](http://www.forum.nokia.com/Library/Tools_and_downloads/Other/Maemo/) and follow the instructions. In this example we will also use [ESBox](http://maemo.org/development/documentation/ide_integration/) as the IDE.

### Set up a build environment
To create a new project using ESBox: 
- Select File -> New -> Project from the menu. 
- Select C++ project, then maemo 5 helloworld. 
- Select the relevant targets. 
- Fill out the other details where needed, then create the project. 
in src/Makefile.am you will have to modify the LDADD and CXXFLAGS lines to look like the following for device targets:

_Code (2 lines)_
```c++
appname_LDADD: $(LIBHILDONMM_LIBS) $(LIBOSSO_LIBS) -ledge -ledgerender -lconic -lXv -L /path/to/edgelib/dist/lib/maemo-5_dev/ 
appname_CXXFLAGS = $(LIBHILDONMM_CFLAGS) $(LIBOSSO_CFLAGS) -DHELLOWORLD_LOCALEDIR=\"${HELLOWORLD_LOCALEDIR}\"  -D__MAEMO__ -I /path/to/edgelib/dist/include 
```

and the following for simulator targets:

_Code (2 lines)_
```c++
appname_LDADD: $(LIBHILDONMM_LIBS) $(LIBOSSO_LIBS) -ledge -ledgerender -lXv -L /path/to/edgelib/dist/lib/maemo-5_sim/ 
appname_CXXFLAGS = $(LIBHILDONMM_CFLAGS) $(LIBOSSO_CFLAGS) -DHELLOWORLD_LOCALEDIR=\"${HELLOWORLD_LOCALEDIR}\"  -D__MAEMO__ -D__MAEMO_SIM__  -I /path/to/edgelib/dist/include 
```

If you add or remove files, you will also have to update it in src/Makefile.am

### Build a sample application
To turn the default helloworld in an EDGELIB application, remove mywindow.h and mywindow.cpp, and replace main.cpp with the following code:

_main.cpp (76 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Code/helloworld.cpp
// One of the EDGELIB tutorial samples for Maemo
//
// Copyright (c) 2009-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Show "hello world" on the screen and exit by hitting a key
/////////////////////////////////////////////////////////////////////
 
#include "config.h"
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

Then update Makefile.am and remove the references to mywindow. After you save the files, the project will automatically build.

