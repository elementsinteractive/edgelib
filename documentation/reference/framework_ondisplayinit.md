:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnDisplayInit


```c++
ERESULT OnDisplayInit(ENATIVETYPE instance, ClassEDisplay *display)
```

## Parameters
### instance (out)
The program instance for Windows and Windows Mobile based applications. This can be used to load surfaces from program resources. Non-Windows applications can ignore this parameter.

### *display (out)
A pointer to the [`ClassEDisplay` module](classedisplay.md).

## Return value
Return E_OK when everything went correctly. Return `E_ERROR` to shutdown EDGELIB.

## Description
This is a callback when the display interface is available for exclusive access. Use it to load and initialize surfaces.

## Version changes
- EDGELIB 3.10: Changed EINSTANCE to ENATIVETYPE

