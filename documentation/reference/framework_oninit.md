:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnInit


```c++
ERESULT OnInit(ENATIVETYPE instance)
```

## Parameters
### instance (out)
This is the program instance for Windows and Windows Mobile based applications and can be used to load resource data. Non-Windows applications can ignore this parameter.

## Return value
Return `E_OK` when successful. Return `E_ERROR` to shutdown EDGELIB.

## Description
This is a callback to initialize the application, load game configuration or resource data within a Windows-based executable.

## Version changes
- EDGELIB 3.10: Changed EINSTANCE to ENATIVETYPE

