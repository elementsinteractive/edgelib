:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnDisplayChange


```c++
void OnDisplayChange(ClassEDisplay *display)
```

## Parameters
### *display (out)
A pointer to the [`ClassEDisplay` module](classedisplay.md).

## Return value
None.

## Description
This is a callback when the display size or orientation has changed, use this to recalculate the screen layout and redraw the backbuffer.

## Version changes
- EDGELIB 3.10: Added this function to the SDK

