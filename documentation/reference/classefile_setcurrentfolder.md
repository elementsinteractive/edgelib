:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetCurrentFolder


```c++
void SetCurrentFolder(const char *foldername, unsigned char location = EFOF_DEFAULT) 
void SetCurrentFolder(const WCHAR *foldername, unsigned char location = EFOF_DEFAULT)
```

## Parameters
### *foldername (in)
A string containing the absolute or relative path of a folder.

### location (in)
Select the [location](classefile_definitions.md) from where to open this file. Can be either EFOF_DEFAULT or EFOF_READONLYPATH.

## Return value
None.

## Description
Change the current working folder of a single file object. When a file object is created, the current path will be set to the path of the executable.

## Version changes
EDGELIB 3.987: Added location argument 
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function 
- EDGELIB 2.60: Added this function to the SDK

