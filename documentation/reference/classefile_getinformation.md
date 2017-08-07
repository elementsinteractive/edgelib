:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetInformation


```c++
bool GetInformation(E_FILEINFO *fileinfo, const char *filename, bool getlocaltime = true) 
bool GetInformation(E_FILEINFO *fileinfo, const WCHAR *filename = NULL, bool getlocaltime = true)
```

## Parameters
### *fileinfo (out)
A pointer to a [`E_FILEINFO` structure](classefile_structures.md) containing file information.

### *filename (in)
The name of the file or folder to retrieve attributes from.

### getlocaltime (in)
When set to true, the date values will be adjusted for the local timezone and daylight saving time.

## Return value
Returns true if the information has been received. In case of a failure, returns false.

## Description
Gets more information about a file or folder. If the filename isn't specified or `NULL`, it will read the attributes of the current open file.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified, renamed GetInformationC to GetInformation 
- EDGELIB 2.90: Added Unicode overloaded function, renamed non-Unicode version to GetInformationC 
- EDGELIB 2.60: Added the getlocaltime parameter

