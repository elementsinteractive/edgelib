:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ClassECamera

This module allows access to the camera's present on the system. It allows you to set the input resolution and format, and set up events for captured frames.

Camera access is supported on the following platforms:

* Apple iOS 4.0
* Symbian S60 3rd edition and later
* Windows Mobile 5 and 6 (*)
* Maemo 5
* Windows desktop
* Linux desktop

(*) When DirectShow drivers are present on the system. There is a third party [compatibility matrix](http://studierstube.icg.tu-graz.ac.at/handheld_ar/camera_phones.php), but it may be out of date.

Due to the amount of extra dependencies, the camera code is not included in edge.lib on Windows desktop and Windows Mobile platforms, but in a separate edgecamera.dll. Copy this file to the same directory as the application binary if you want to have camera support.

## Class members
* [ClassECamera](classecamera_classecamera.md)
* [FrameToSurface](classecamera_frametosurface.md)
* [FrameToSurfaceAspect](classecamera_frametosurfaceaspect.md)
* [GetCameraCount](classecamera_getcameracount.md)
* [GetCameraIndex](classecamera_getcameraindex.md)
* [GetCameraOrientation](classecamera_getcameraorientation.md)
* [GetCameraState](classecamera_getcamerastate.md)
* [GetCurrentMode](classecamera_getcurrentmode.md)
* [GetCurrentModeIndex](classecamera_getcurrentmodeindex.md)
* [GetModeCount](classecamera_getmodecount.md)
* [GetModeInformation](classecamera_getmodeinformation.md)
* [RequestCamera](classecamera_requestcamera.md)
* [RequestResolution](classecamera_requestresolution.md)
* [RequestStart](classecamera_requeststart.md)
* [RequestStop](classecamera_requeststop.md)
* [Structures](classecamera_structures.md)
* [Definitions](classecamera_definitions.md)

## Version changes
- EDGELIB 4.00: Added this class to the SDK

