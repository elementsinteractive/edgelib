:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetKeyPressCount


```c++
unsigned long GetKeyPressCount(unsigned char action, bool includerepeat = true)
```

## Parameters
### *action (in)
The mapped key action to check.

### includerepeat (in)
Include repeat key strokes.

## Return value
The number of key strokes.

## Description
Check input events and return the number of key strokes for the specified action.

## Version changes
- EDGELIB 3.00: Added this function to the SDK

