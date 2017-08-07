:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# The project framework

## Before you start programming
When building a project with EDGELIB, you need to set up a framework to communicate with EDGELIB. This framework will be based on the getting started tutorial. It will be expanded with three additional classes that act as a framework on their own: the menu class, the in-game level class and the loader class. This will be a modular approach on development which has several advantages. It improves manageability of the code, and it will improve game development with multiple programmers (each programmer will be able to work on his own module).

Each module will have an instance to a shared data structure, and communicating between modules will be done through messages. The main framework will keep track on which module is currently activated, it acts as a manager. Each module may also request an action from the main framework, such as minimizing the game.

## The basic framework
After setting up the projects described in the getting started tutorial, it's time to update the source code. The new source code will expand the basic framework with additional callback functions which will be used in our game. The main framework will be moved to a new file main.h, while the source file (blastar.cpp) will contain the entry function and include source code files.

_blastar.cpp (32 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Code/blastar.cpp
// One of the Edge tutorial samples
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// A simple shoot-em-up game
/////////////////////////////////////////////////////////////////////
 
/////////////////////////////////////////////////////////////////////
// Include and link the library                                    //
/////////////////////////////////////////////////////////////////////
 
//Include EDGELIB
#include "edgemain.h"
 
//Link the EDGELIB static library
#pragma comment(lib, "edge.lib")
 
//Platform dependent definitions
#include "def_platf.h"
 
//Include the framework class
#include "main.h"
 
 
/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////
 
ClassEdge *EdgeMain(EDGESTARTUP *data){ return(new ClassMain); }
```

_main.h (107 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Code/main.h
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Contains the framework class
/////////////////////////////////////////////////////////////////////
 
//Include internal resources for native Win32 builds
#if defined(DEVICE_WIN32)
#include "resource.h"
#else
#define IDI_MAIN 0
#endif
 
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////
 
//The main class
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
        ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta);
        void OnExit(ENATIVETYPE instance);
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
 
//Configure framework
ERESULT ClassMain::OnFrameworkConfig(EFRAMEWORKCONFIG *config)
{
    config->ffwdkey         = 'A';                //A key for fast forwarding (multiplies framerate by 2.5)
    config->slomokey        = 'S';                //A key for slow motion     (halves framerate)
    config->maxfps          = 60;                 //Limit the maximum frames per second by 60
    config->flags           = EFL_NOTEARING;      //Desktop PC: Enable VSync to prevent tearing in displaying frames
    config->jogtiltarrow    = true;               //Emulate the UIQ jog as left/right arrows
    return(E_OK);
}
 
//Configure display
ERESULT ClassMain::OnDisplayConfig(EDISPLAYCONFIG *config)
{
    ClassEStd::StrCpy(config->caption, "Blastar");
    config->icon            = IDI_MAIN;           //Application icon
    config->enginestatus    = true;               //Desktop PC: Enable the status bar and status information icon
    config->engineconsole   = false;              //Desktop PC: Disable the EDGELIB console
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
 
//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
    return(E_OK);
}
 
//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
}
```

## Project workspaces
This project will be built for mobile devices using the EDGELIB Builder tool and using Microsoft Visual Studio for Windows Desktop. Delete workspaces so only the workspace_pc (or workspace_pc_vc2005 when using Visual Studio 2005) and workspace_eide remain. The pc workspace needs to be recreated and name it Blastar. The EDGELIB builder tool workspace can be renamed easily. Rename helloworld.epj to blastar.epj and open the project file. Go to project -> properties and change Hello World into Blastar. The application UID will be 0x10205DA4.

## The game data structure
When programming a game, there will be a need to save data. This data will be saved in a main structure that will be shared throughout the program. For now we're going to add a game configuration structure, and a shared data structure. These structures will expand as development continues.

_main.h (24 lines)_
```c++
// Contains the framework class
/////////////////////////////////////////////////////////////////////
 
/////////////////////////////////////////////////////////////////////
// Structures                                                      //
/////////////////////////////////////////////////////////////////////
 
//Game configuration
typedef struct
{
    bool autoshot;
    unsigned char startlevel;
}GAMECFG;
 
//Program shared data
typedef struct
{
    GAMECFG cfg;
    ECD *ecd;
}SHARED_DATA;
 
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                // 
```

The game configuration structure contains the options that were described in the previous chapter, the shared data structure contains the game configuration, and an instance of ECD for communicating with EDGELIB. The shared data structure will be a private variable for our main framework. Also add 2 new function prototypes:

_main.h (7 lines)_
```c++
class ClassMain : public ClassEdge
{
    private:
        SHARED_DATA shared;
        void LoadCfg(void);
        void SaveCfg(void);
    public:
```

The game configuration will be loaded when the game starts and saved when the game quits. The implementation of LoadCfg() and SaveCfg():

_main.h (35 lines)_
```c++
        void OnExit(ENATIVETYPE instance);
};
 
 
/////////////////////////////////////////////////////////////////////
// ClassMain: private                                              //
/////////////////////////////////////////////////////////////////////
 
//Load the game configuration file
void ClassMain::LoadCfg(void)
{
    ClassEFile ef;
    shared.cfg.autoshot = true;
    shared.cfg.startlevel = 1;
    if (ef.Open("game.cfg", EFOF_READ))
    {
        ef.Read(&shared.cfg, sizeof(GAMECFG));
        ef.Close();
    }
}
 
//Save the game configuration file
void ClassMain::SaveCfg(void)
{
    ClassEFile ef;
    if (ef.New("game.cfg"))
    {
        ef.Write(&shared.cfg, sizeof(GAMECFG));
        ef.Close();
    }
}
 
 
/////////////////////////////////////////////////////////////////////
// ClassMain: public                                               // 
```

The function sets the game configuration to default values in case the file can't be loaded (this will always happen when the game is being loaded for the first time). Calling this function and initializing the shared data structure is done within OnInit():

_main.h (7 lines)_
```c++
//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
    shared.ecd = &ecd;
    LoadCfg();
    return(E_OK);
}
```

The configuration also needs to be saved. This function will be called when the game quits:

_main.h (5 lines)_
```c++
//Callback: Called when the program exits
void ClassMain::OnExit(ENATIVETYPE instance)
{
    SaveCfg();
}
```

## Adding modules
Each module will be based on a generic class and enherit functionality from this class. With this you can use a trick to access these modules easier and improves readability of the code. It will also be easier to add new modules this way. This will be the generic module class, add a new file module.h:

_module.h (46 lines)_
```c++
// Copyright (c) 2006-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Contains the module base class
/////////////////////////////////////////////////////////////////////
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////
 
class ClassModule
{
    protected:
        SHARED_DATA *shared;
        unsigned long modulestate;
    public:
        ClassModule(void);
        virtual ~ClassModule(void);
        void LinkShared(SHARED_DATA *pshared);
        void virtual OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta);
        void virtual OnMinimize(void);
        void virtual OnRestore(void);
};
 
 
/////////////////////////////////////////////////////////////////////
// ClassModule::Virtual methods                                    //
/////////////////////////////////////////////////////////////////////
 
void ClassModule::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta){ }
void ClassModule::OnMinimize(void){ }
void ClassModule::OnRestore(void) { }
ClassModule::~ClassModule(void)   { }
 
/////////////////////////////////////////////////////////////////////
// ClassModule::Public                                             //
/////////////////////////////////////////////////////////////////////
 
//ClassModule: constructor
ClassModule::ClassModule(void){ }
 
//Link the main shared data structure
void ClassModule::LinkShared(SHARED_DATA *pshared)
{
    shared = pshared;
}
```

It contains a function to save the shared data structure inside each module (which will be done at startup), and several wrapper functions for a number of events. The next step is to create derivations of the class, our 3 modules. Add these header files with basic module implementations:

_loader.h (40 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Code/loader.h
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Module: loading game data on startup
/////////////////////////////////////////////////////////////////////
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////
 
class ClassLoader : public ClassModule
{
    public:
        ClassLoader(void);
        ~ClassLoader(void);
        void OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta);
};
 
 
/////////////////////////////////////////////////////////////////////
// ClassLoader::Public                                             //
/////////////////////////////////////////////////////////////////////
 
//ClassLoader: Constructor
ClassLoader::ClassLoader(void)
{
}
 
//ClassLoader: Destructor
ClassLoader::~ClassLoader(void)
{
}
 
//Module game loop
void ClassLoader::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta)
{
}
```

_menu.h (40 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Code/menu.h
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Module: the menu system
/////////////////////////////////////////////////////////////////////
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////
 
class ClassMenu : public ClassModule
{
    public:
        ClassMenu(void);
        ~ClassMenu(void);
        void OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta);
};
 
 
/////////////////////////////////////////////////////////////////////
// ClassMenu::Public                                               //
/////////////////////////////////////////////////////////////////////
 
//ClassMenu: Constructor
ClassMenu::ClassMenu(void)
{
}
 
//ClassMenu: Destructor
ClassMenu::~ClassMenu(void)
{
}
 
//Module game loop
void ClassMenu::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta)
{
}
```

_level.h (40 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Code/level.h
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Module: in-game action
/////////////////////////////////////////////////////////////////////
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                //
/////////////////////////////////////////////////////////////////////
 
class ClassLevel : public ClassModule
{
    public:
        ClassLevel(void);
        ~ClassLevel(void);
        void OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta);
};
 
 
/////////////////////////////////////////////////////////////////////
// ClassLevel::Public                                              //
/////////////////////////////////////////////////////////////////////
 
//ClassLevel: Constructor
ClassLevel::ClassLevel(void)
{
}
 
//ClassLevel: Destructor
ClassLevel::~ClassLevel(void)
{
}
 
//Module game loop
void ClassLevel::OnNextFrame(E2DSurface *buffer, unsigned long *module, unsigned long *modulemsg, unsigned long timedelta)
{
}
```

After adding these files to the project, add a definition for each module and include header files:

_main.h (32 lines)_
```c++
    ECD *ecd;
}SHARED_DATA;
 
 
/////////////////////////////////////////////////////////////////////
// Module definitions & messages                                   //
/////////////////////////////////////////////////////////////////////
 
#define MODULE_LOADER  0
#define MODULE_MENU    1
#define MODULE_LEVEL   2
#define MAX_MODULES    3
#define MODULE_CLEANUP MAX_MODULES
 
#define MM_NONE        0
#define MM_INIT        1
#define MM_FROMMODULE  32768
#define MM_CUSTOM      65536
 
 
/////////////////////////////////////////////////////////////////////
// Module includes                                                 //
/////////////////////////////////////////////////////////////////////
 
#include "module.h"
#include "loader.h"
#include "menu.h"
#include "level.h"
 
 
/////////////////////////////////////////////////////////////////////
// Class definition                                                // 
```

There is also a definition to define the number of modules we use. An array of modules and an initialization function will be added to the class definition of ClassMain:

_main.h (5 lines)_
```c++
    private:
        SHARED_DATA shared;
        ClassModule *cmodule[MAX_MODULES];
        bool InitModules(void);
        void LoadCfg(void);
```

The final step is to initialize all modules. InitModules() is created and will be called in OnInit():

_main.h (19 lines)_
```c++
// ClassMain: private                                              //
/////////////////////////////////////////////////////////////////////
 
//Allocate and initialize each module
bool ClassMain::InitModules(void)
{
    unsigned char ctr;
    cmodule[MODULE_LOADER] = new ClassLoader;
    cmodule[MODULE_MENU]   = new ClassMenu;
    cmodule[MODULE_LEVEL]  = new ClassLevel;
    for (ctr = 0; ctr < MAX_MODULES; ctr++)
    {
        if (cmodule[ctr] != NULL)
            cmodule[ctr]->LinkShared(&shared);
        else
            return(false);
    }
    return(true);
}
```

_main.h (12 lines)_
```c++
//Callback: Called before the display mode changes
ERESULT ClassMain::OnInit(ENATIVETYPE instance)
{
    shared.ecd = &ecd;
    if (!InitModules())
    {
        SetErrorMsg("Error: Module init");
        return(E_ERROR);
    }
    LoadCfg();
    return(E_OK);
}
```

Because modules are dynamically allocated, they need to be treated with care. Initialize the module pointers to value NULL in the constructor, and release allocated pointers (not NULL) in the destructor:

_main.h (16 lines)_
```c++
//ClassMain: constructor
ClassMain::ClassMain(void)
{
    unsigned char ctr;
    for (ctr = 0; ctr < MAX_MODULES; ctr++)
        cmodule[ctr] = NULL;
}
 
//ClassMain: destructor
ClassMain::~ClassMain(void)
{
    unsigned char ctr;
    for (ctr = 0; ctr < MAX_MODULES; ctr++)
        if (cmodule[ctr] != NULL)
            delete cmodule[ctr];
}
```

The result is that a module can be easily called:

```c++
cmodule[MODULE_LOADER]->OnNextFrame()
```

## Module managing and messages
After setting up the module classes, it's time to add managing functionality to ClassMain. Each module should have an implementation of the OnNextFrame() function, one module will be processed depending on the game state. 2 variables will be added to the shared data structure for tracking modules and module messages:

_main.h (8 lines)_
```c++
//Program shared data
typedef struct
{
    unsigned long module;
    unsigned long modulemsg;
    GAMECFG cfg;
    ECD *ecd;
}SHARED_DATA;
```

These will be set to their defaults in the ClassMain constructor:

_main.h (9 lines)_
```c++
//ClassMain: constructor
ClassMain::ClassMain(void)
{
    unsigned char ctr;
    for (ctr = 0; ctr < MAX_MODULES; ctr++)
        cmodule[ctr] = NULL;
    shared.module = MODULE_LOADER;
    shared.modulemsg = MM_INIT;
}
```

Finally, call the desired module within OnNextFrame of ClassMain:

_main.h (10 lines)_
```c++
//Callback: Called every frame
ERESULT ClassMain::OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
{
    if (shared.module < MAX_MODULES)
        if (cmodule[shared.module] != NULL)
            cmodule[shared.module]->OnNextFrame(&display->buffer, &shared.module, &shared.modulemsg, timedelta);
    if (shared.module == MODULE_CLEANUP)
        Quit();
    return(E_OK);
}
```

Everything should be ready now. Every module can be programmed separately by interpreting messages. For example, try to fill the screen with a different color in each module to see which module is being processed and try changing modules as well.

## Chapters
* [Designing a game](tutorials_blastar_design.md)
* **The project framework**
* [Creating graphics](tutorials_blastar_graphics.md)
* [The loader module](tutorials_blastar_loader.md)
* [Platform definitions](tutorials_blastar_definitions.md)
* [The menu module](tutorials_blastar_menu.md)
* [The level module](tutorials_blastar_level.md)
* [Using sprites](tutorials_blastar_sprites.md)
* [Player control](tutorials_blastar_control.md)
* [Wall of objects](tutorials_blastar_objects.md)
* [Collision detection](tutorials_blastar_collision.md)
* [The boss](tutorials_blastar_boss.md)
* [Refining the game](tutorials_blastar_refining.md)
* [Adding powerups](tutorials_blastar_powerups.md)
* [Special effects](tutorials_blastar_specialfx.md)
* [Completing the game](tutorials_blastar_completing.md)

