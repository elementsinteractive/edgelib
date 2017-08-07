:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OpenFolder


```c++
bool OpenFolder(const char *searchfolder, const char *mask, unsigned char location = EFOF_DEFAULT) 
bool OpenFolder(const WCHAR *searchfolder, const WCHAR *mask, unsigned char location = EFOF_DEFAULT)
```

## Parameters
### *searchfolder (in)
The folder to open for searching.

### *mask (in)
The file extension mask. Only search for files of this type. Currently only implemented on Win32-based platforms.

### location (in)
Select the [location](classefile_definitions.md) from where to open this file. Can be either EFOF_DEFAULT or EFOF_READONLYPATH.

## Return value
Return true if the folder has been successfully opened, otherwise return false.

## Description
Opens a folder to read the filenames it contains. After reading filenames, [close](classefile_closefolder.md) it.

## Version changes
EDGELIB 3.987: Added location argument 
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function

