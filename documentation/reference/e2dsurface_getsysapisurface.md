:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetSysApiSurface


```c++
ENATIVETYPE GetSysApiSurface(void)
```

## Parameters
None.

## Return value
A native surface type. If it's equal to NULL, the object doesn't have a native surface.

## Description
When the E2DSurface object is created with the EST_SYSTEMAPI or EST_HARDWARE flag, this function may return a native surface type. It can be cast to LPDIRECTDRAWSURFACE (Win32) or CFbsBitmap * (Symbian).

## Version changes
- EDGELIB 3.10: Changes return value to ENATIVETYPE 
- EDGELIB 2.20: Added this method for Windows Mobile 5 and Windows desktop

