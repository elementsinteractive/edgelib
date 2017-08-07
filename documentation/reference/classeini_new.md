:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# New


```c++
bool New(const char *name) 
bool New(const WCHAR *name)
```

## Parameters
### *name (in)
The name of the file to create.

## Return value
Return true if the file has been created and successfully opened, otherwise return false.

## Description
Create and open a new INI file. When the file isn't needed anymore, [close](classeini_close.md) it.

## Version changes
- EDGELIB 2.90: Removed obsolete flags parameter, added a Unicode overloaded function 
- EDGELIB 2.50: Added this function to the SDK

