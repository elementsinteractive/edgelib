:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Platform definitions

## Screen layouts
Because the supported platforms have various screen layouts and resolutions, we need to have separated definitions for each platform. For some devices the game will have a landscape orientation except for phone devices.

First we need to adjust the EDGELIB configuration within the `OnDisplayConfig()` function to enable landscape or portrait mode depending on the resolution:

_main.h (13 lines)_
```c++
    config->engineconsole   = false;              //Desktop PC: Disable the EDGELIB console
    config->width           = XRES;               //Desktop PC: Custom display width
    config->height          = YRES;               //Desktop PC: Custom display height
    
    //Determine screen orientation
    if (XRES < YRES)
        config->orientation = DOR_PORTRAIT;
    else if (XRES > YRES)
        config->orientation = DOR_LANDSCAPE;
    else
        config->orientation = DOR_STARTUP;
 
    return(E_OK);
```

XRES and YRES are platform dependent definitions and will be explained below.

## Platform dependent definitions
The best way to create platform dependent definitions is to create a header file within each workspace folder. The header file included by the compiler will be the one from the active workspace, this will enable you to create definitions for each workspace separately. First let's create a definition file for workspace_pc:

_def_platf.h (14 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// Workspace_pc/def_platf.h
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Contains specific definitions for this platform
/////////////////////////////////////////////////////////////////////
 
#define XRES          640
#define YRES          480
 
#define MOVE_SPEED      8
#define DIM_SPRITE      8 
```

This will define the screen resolution and a definition for resolution independent sprite animation as well as a sprite size modifier. After adding this file, we need to include it:

_blastar.cpp (8 lines)_
```c++
//Link the EDGELIB static library
#pragma comment(lib, "edge.lib")
 
//Platform dependent definitions
#include "def_platf.h"
 
//Include the main wrapper class
#include "main.h" 
```

After running the sample, the desktop version should have a bigger screen in the correct orientation.

## Definitions for other platforms
These will be the files for the other platforms. For this project, the UIQ platform will be similar to a Pocket PC, but with less vertical space.

_def_platf.h (14 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// workspace_eide_320x240/def_platf.h
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Contains specific definitions for this platform
/////////////////////////////////////////////////////////////////////
 
#define XRES          320
#define YRES          240
 
#define MOVE_SPEED      4
#define DIM_SPRITE      4 
```

_def_platf.h (14 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// workspace_eide_176x208/def_platf.h
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Contains specific definitions for this platform
/////////////////////////////////////////////////////////////////////
 
#define XRES          176
#define YRES          208
 
#define MOVE_SPEED      3
#define DIM_SPRITE      3 
```

_def_platf.h (14 lines)_
```c++
/////////////////////////////////////////////////////////////////////
// workspace_eide_320x208/def_platf.h
//
// Copyright (c) 2006-2017 Elements Interactive B.V.
// http://www.edgelib.com
//
// Contains specific definitions for this platform
/////////////////////////////////////////////////////////////////////
 
#define XRES          320
#define YRES          208
 
#define MOVE_SPEED      4
#define DIM_SPRITE      4 
```

When looking at the `MOVE_SPEED` definition for every platform, you will notice that this value will decrease when the resolution decreases. Objects will move faster when there is more screen space.

## Chapters
* [Designing a game](tutorials_blastar_design.md)
* [The project framework](tutorials_blastar_framework.md)
* [Creating graphics](tutorials_blastar_graphics.md)
* [The loader module](tutorials_blastar_loader.md)
* **Platform definitions**
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

