:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PackGetFileInfo


```c++
bool PackGetFileInfo(EPK_FILEINFO *fileinfo, unsigned long id, bool getlocaltime = true) 
bool PackGetFileInfo(EPK_FILEINFO *fileinfo, const char *filename, bool getlocaltime = true) 
bool PackGetFileInfo(EPK_FILEINFO *fileinfo, const WCHAR *filename, bool getlocaltime = true)
```

## Parameters
### *fileinfo (out)
A pointer to a [pack file info structure](classefile_structures.md) containing general file information.

### id (in)
The file ID to request information about.

### *filename (in)
The filename to search for.

### getlocaltime (in)
When set to true, converts time values in the info structure to the active timezone and daylight saving time.

## Return value
Return true if the file information is read in, otherwise return false.

## Description
Read general information about a file within a pack.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function 
- EDGELIB 2.60: Added the ability to search by filename and added the getlocaltime parameter.

