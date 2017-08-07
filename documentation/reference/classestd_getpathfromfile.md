:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetFolderName


```c++
char static *GetFolderName(char *fullname) 
WCHAR static *GetFolderName(WCHAR *fullname)
```

## Parameters
### *fullname (in/out)
A full filename including path.

## Return value
The same pointer as the parameter.

## Description
Gets the folder component of the full filename.

## Version changes
- EDGELIB 2.90: Added a Unicode overloaded function 
- EDGELIB 2.50: Added this function to the SDK

