:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OpenGL (ES) implementations

## Supported implementation libraries
Because OpenGL and OpenGL ES are only specifications, you need an implementation library to use software or hardware accelerated 3D rendering. For Windows Desktop, an OpenGL implementation is provided by the display driver. The OpenGL headers and libraries are delivered with the compiler. From Symbian version 8.0 and onwards, OpenGL implementations are installed on each device taking advantage of hardware or software rendering depending if hardware acceleration hardware is present. The headers and libraries are provided by the Symbian SDK.

For Symbian versions below 8.0 and Windows Mobile, you need to download a separate OpenGL ES implementation library. These libraries also contain an emulation library to emulate OpenGL ES on Windows Desktop systems.

## Downloading OpenGL ES implementation libraries
To use hardware acceleration we recommend [PowerVR](http://www.imgtec.com/powervr/Insider/) or the [NVIDIA GoForce SDK](http://developer.nvidia.com). These libraries don't support software rendering. To use OpenGL ES software rendering, you can download [Rasteroid](http://www.hybrid.fi/main/download/tools.php).

The PowerVR and Rasteroid implementation libraries are free for commercial use. The NVIDIA GoForce SDK can be obtained by contacting NVIDIA.

To use PowerVR on devices with an Intel 2700g hardware accelerator, you need to download the [Intel 2700g SDK](http://downloadfinder.intel.com/scripts-df-external/detail_desc.aspx?ProductID=2236&DwnldID=9492&agr=N), install the SDK and copy the OpenGL ES library into the PowerVR SDK.

## Installing OpenGL ES implementation libraries
After download a library, we recommend installing them near your EDGELIB SDK installation, like `c:\projects\powervr` or `c:\projects\rasteroid`. They install OpenGL ES headers, libraries and samples.

When using an implementation library with EDGELIB, set up include and library paths, include the OpenGL header files and link the implementation library.

## Differences between OpenGL and OpenGL ES
The main difference between OpenGL and OpenGL ES is that OpenGL ES lacks a number of features present in OpenGL. For example, glBegin and glEnd are removed as well as some other functions.

Another difference is that OpenGL ES supports rendering using fixed point instead of floating point. Floating point OpenGL ES libraries are available, but will not work on some devices (like the hardware accelerated Dell Axim x50v). Fixed point OpenGL ES implementations are referred as the common-lite profile, while the floating point implementations are referred as the common profile.

We recommend using fixed point where possible. This will improve performance because floating point math is emulated by software on mobile devices.

## Using OpenGL (ES) in EDGELIB
By default, EDGELIB opens the display for software blitting and rendering. To open the display in OpenGL, you need to add a definition before including edgemain.h:

```c++
//Use OpenGL 
#define EGL_USEGL 
 
//Include the EDGELIB SDK 
#include "edgemain.h"  
```

It's also needed to link an OpenGL wrapper library. These are available for each platform in the EDGELIB SDK library folder. Available wrapper libraries depend on the target platform.

## Chapters
* **OpenGL (ES) implementations**
* [3D helper functions](tutorials_3d_helperfunctions.md)
* [Advanced 3D functions](tutorials_3d_advanced.md)

