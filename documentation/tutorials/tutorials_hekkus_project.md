:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Create a multi-platform project with Hekkus

## Prepare project workspaces/solutions
This project will be based on the helloworld multi-platform code sample from the [getting started tutorial](tutorials_gettingstarted_edgeinstallation.md).

First include and link Hekkus with the project. You can add Hekkus for the following Windows-based workspaces/solutions by going to the project properties and adding additional include directories and library paths:

### Microsoft Visual C++ (all versions) include directories
`.,..\..\edge\include,..\..\hekkus` (Windows desktop and Windows Mobile)

### Microsoft Visual C++ 6.0 library paths
`..\..\edge\lib\pc-x86,..\..\hekkus\lib\xp` (Windows desktop)

### Microsoft Embedded Visual C++ library paths
`..\..\edge\lib\ppc-arm,..\..\hekkus\lib\ppc-arm` (Pocket PC) 
`..\..\edge\lib\ppc2003-arm,..\..\hekkus\lib\ppc-arm` (Pocket PC 2003) 
`..\..\edge\lib\smartphone-arm,..\..\hekkus\lib\ppc-arm` (Smartphone) 
`..\..\edge\lib\smartphone2003-arm,..\..\hekkus\lib\ppc-arm` (Smartphone 2003)

### Microsoft Visual C++ 2005 library paths
`..\..\edge\lib\pc-x86_vc2005,..\..\hekkus\lib\xp-unicode` (Windows desktop) 
`..\..\edge\lib\ppc2003-arm_vc2005,..\..\hekkus\lib\ppc-arm` (Pocket PC 2003) 
`..\..\edge\lib\ppc2005-arm,..\..\hekkus\lib\ppc-arm` (Pocket PC 2005) 
`..\..\edge\lib\smartphone2003-arm_vc2005,..\..\hekkus\lib\ppc-arm` (Smartphone 2003) 
`..\..\edge\lib\smartphone2005-arm,..\..\hekkus\lib\ppc-arm` (Smartphone 2005)

Then edit the installation definition file in the setup folder:

_install.inf (25 lines)_
```c++
[DefaultInstall]
CopyFiles   = ExeFiles,SndFiles
CEShortcuts = Startmenu
 
 
[SourceDisksNames.ARMPPC]
1 = ,"ExeFiles",,..\workspace_ppc2003\ARMV4Rel
2 = ,"SndFiles",,..\res
 
 
[SourceDisksFiles]
Helloworld.exe = 1
hss.dll = 1
sound.wav = 2
music.mod = 2
 
 
[ExeFiles]
"Helloworld.exe"
"hss.dll"
 
 
[SndFiles]
"sound.wav"
"music.mod"
```

After that, edit the Symbian project files (for all Symbian platforms) to include Hekkus:

_helloworld.mmp (3 lines)_
```c++
USERINCLUDE   ..\..\edge\include
USERINCLUDE   ..\..\hekkus
USERINCLUDE   ..\..\hekkus\include
```

_helloworld.mmp (2 lines)_
```c++
LIBRARY hss.lib
LIBRARY euser.lib
```

_helloworld.pkg (4 lines)_
```c++
"helloworld.app"-"!:\system\apps\helloworld\helloworld.app"
"hss.dll"-"!:\system\apps\helloworld\hss.dll"
"..\res\sound.wav"-"!:\system\apps\hekkus_sample\sound.wav"
"..\res\music.mod"-"!:\system\apps\hekkus_sample\music.mod"
```

As you can see, the setup file install.inf and the Symbian package file contain two extra files: sound.wav and music.mod. 
Download [sound.wav](files/sound.wav) and [music.mod](files/music.mod) and copy them into the res folder of the project. Copy them to the release folder of desktop pc workspaces too, for quick testing.

Finally copy the matching Hekkus .DLL files for each platform workspace/solution you may have:

* `hekkus\dll\xp\hss.dll` (Windows Desktop)
* `hekkus\dll\xp-unicode\hss.dll` (Windows Desktop, Visual C++ 2005)
* `hekkus\dll\ppc-arm\hss.dll` (Windows Mobile)
* `hekkus\dll\symbian-armi\hss.dll` (All Symbian device before Symbian 9)
* `hekkus\dll\symbian_v9-gcce\hss.dll` (All Symbian 9+ devices)

## Include and link Hekkus
To use the Hekkus Sound System wrapper, define `ESOUND_HEKKUS` before including edgemain.h:

_helloworld.cpp (8 lines)_
```c++
//Use the ClassESound Hekkus wrapper
#define ESOUND_HEKKUS
 
//Include EDGELIB
#include "edgemain.h"
 
//Link the Edge static library
#pragma comment(lib, "edge.lib") 
```

## Chapters
* [How to use Hekkus within EDGELIB](tutorials_hekkus_usinghekkus.md)
* **Create a multi-platform project with Hekkus**
* [Playback sound and music](tutorials_hekkus_playback.md)

