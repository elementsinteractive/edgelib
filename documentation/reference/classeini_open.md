:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Open


```c++
bool Open(const char *name, unsigned char flags = EFAF_READ) 
bool Open(const WCHAR *name, unsigned char flags = EFAF_READ)
```

## Parameters
### *name (in)
The name of the file to open.

### flags (in)
[Flags](classefile_definitions.md) for opening the INI file.

## Return value
Return true if the file exists and has been successfully opened, otherwise return false.

## Description
Open an INI file. When the file isn't needed anymore, [close](classeini_close.md) it.

## Version changes
- EDGELIB 2.90: Changed the flags parameter, added Unicode overloaded function 
- EDGELIB 2.60: Changed the flags parameter 
- EDGELIB 2.50: Added this function to the SDK

