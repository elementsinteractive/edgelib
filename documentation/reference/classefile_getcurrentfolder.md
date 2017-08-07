:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetCurrentFolder


```c++
char *GetCurrentFolderC(void) 
WCHAR *GetCurrentFolder(void)
```

## Parameters
None.

## Return value
A pointer to the internal string of the current working folder.

## Description
Returns a pointer to an internally stored string containing the current working folder of a single file object.

## Version changes
- EDGELIB 2.90: Added Unicode overloaded function, renamed non-unicode version to GetCurrentFolderC 
- EDGELIB 2.60: Added this function to the SDK

