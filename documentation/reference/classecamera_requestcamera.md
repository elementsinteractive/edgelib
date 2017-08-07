:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RequestCamera


```c++
ERESULT RequestCamera(int camera);
```

## Parameters
### camera (in)
The installed camera to use. You can specify a camera by [number](classecamera_getcameracount.md), or you can specify a [camera location](classecamera_definitions.md).

## Return value
An ERESULT value describing the status known up to that point.

## Description
Binds the class to the specified camera object. It will release a camera object selected by a previous request. Camera devices are often asynchronous, and the status will be sent to [OnCameraChange](framework_oncamerachange.md), either during the function call, or later as part of an event. If the success is known at the time of the call, the function will return it, otherwise E_OK is returned.

When a camera location is specified, the function makes a best-effort attempt to select a camera with the desired property. If there is no such camera, or the information is unknown, a camera is guessed instead. The actual properties can be retrieved after the OnCameraChange callback.

## Version changes
- EDGELIB 4.00: Added this function

