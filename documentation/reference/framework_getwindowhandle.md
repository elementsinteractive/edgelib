:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetWindowHandle


```c++
ENATIVETYPE GetWindowHandle(void)
```

## Parameters
None.

## Return value
This is the current window handle of the framework, this can be useful for Windows or Symbian based applications.

## Description
Use this function to retrieve the current window handle. For Win32 build the return value can be cast to HWND, for Symbian builds it can be cast to RWindow *.

## Version changes
- EDGELIB 3.10: Replacement of GetWnd, added support for Symbian

