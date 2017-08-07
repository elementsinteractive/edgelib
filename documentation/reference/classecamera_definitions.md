:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Definitions

## Camera locations
```c++
ELOC_REAR     // The camera is on the backside of the device, the common location for recording. 
ELOC_FRONT    // The camera is on the screen side of the device, the common location for video calls. 
ELOC_UNKNOWN  // The position is unknown or can be changed.  
```

The camera locations are currently provided on most Series 60 devices, and on some Windows Mobile devices.

## Camera states
```c++
ECS_INVALID    // The camera is not operational 
ECS_READY      // The camera is ready for use 
ECS_RECORDING  // The camera is currently recording  
```

The current states for cameras.

## Camera frame rotation
```c++
ECO_XFLIP     // The camera data is mirrored across the X axis 
ECO_YFLIP     // The camera data is mirrored across the Y axis 
ECO_SWAPAXIS  // The axis of the camera are swapped  
```

Flags for camera image orientation relative to the screen. You can use this to rotate the camera into its correct position.

## Camera video formats
```c++
EDSP_YUV12    // Y plane followed by a U plane and a V plane at half dimensions 
EDSP_YVU12    // Y plane followed by a V plane and a U plane at half dimensions 
EDSP_YUV420   // four Y bytes followed by an U and a V byte 
EDSP_UYVY     // one U/V pair per two horizontal Y values 
EDSP_YVYU     // one U/V pair per two horizontal Y values 
EDSP_YUYV     // one U/V pair per two horizontal Y values 
EDSP_YUV444   // Y U and V bytes per pixel 
EDSP_YUV444s  // Y bitmap followed by UV bitmap  
```

Some of the camera-specific formats. In addition, the [standard buffer formats](classedisplay_definitions.md) can be used

## Version changes
- EDGELIB 4.00: Added initial definitions to SDK

