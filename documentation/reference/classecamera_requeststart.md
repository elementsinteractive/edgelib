:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RequestStart


```c++
ERESULT RequestStart();
```

## Parameters
None

## Return value
An ERESULT value describing the status known up to that point.

## Description
Requests the camera driver to start sending frames. Camera devices are often asynchronous, and the status will be sent to [OnCameraChange](framework_oncamerachange.md), either during the function call, or later as part of an event. If the success is known at the time of the call, the function will return it, otherwise E_OK is returned.

## Version changes
- EDGELIB 4.00: Added this function

