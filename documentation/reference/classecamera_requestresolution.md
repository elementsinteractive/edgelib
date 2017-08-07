:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RequestResolution


```c++
ERESULT RequestResolution(int mode); 
ERESULT RequestResolution(E_CAMERAMODE & info);
```

## Parameters
### mode (in)
The mode index to select

### info (in)
A [mode description](classecamera_structures.md) to match

## Return value
An ERESULT value describing the status known up to that point.

## Description
Sets the mode to the specified index, or sets a mode that matches the input as close as possible.  
The actual properties can be retrieved after the [OnCameraChange](framework_oncamerachange.md) callback.

## Version changes
- EDGELIB 4.00: Added this function

