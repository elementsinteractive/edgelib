:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RequestStop


```c++
ERESULT RequestStop();
```

## Parameters
None

## Return value
An ERESULT value describing the status known up to that point.

## Description
Requests the camera driver to stop sending frames. Some frames may arrive after the call completes, but before [OnCameraChange](framework_oncamerachange.md) is called.

## Version changes
- EDGELIB 4.00: Added this function

