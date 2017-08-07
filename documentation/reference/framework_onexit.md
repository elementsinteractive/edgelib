:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnExit


```c++
void OnExit(ENATIVETYPE instance)
```

## Parameters
### instance (out)
The program instance for Windows and Windows Mobile based applications. This can be used to access the application resources. Non-Windows applications can ignore this parameter.

## Return value
None.

## Description
This is a callback when the application is about to exit. Cleanup resources and save game configuration here.

## Version changes
- EDGELIB 3.10: Changed EINSTANCE to ENATIVETYPE

