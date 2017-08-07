:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnNextFrame


```c++
ERESULT OnNextFrame(ClassEDisplay *display, unsigned long timedelta)
```

## Parameters
### *display (out)
A pointer to the [ClassEDisplay module](classedisplay.md). Use it to access its buffer member variable for graphics programming.

### timedelta (out)
The time that has elapsed since the last call to this function. Use it to move game entities based on time rather than fixed framerate.

## Return value
Return `E_OK` when everything went correctly. Return `E_ERROR` to shutdown EDGELIB.

## Description
This callback function is called multiple times (depending on the framerate) per second and is the actual game loop.

