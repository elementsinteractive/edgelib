:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# EDGEDATA


```c++
ENATIVETYPE instance 
WCHAR currentpath[MAX_EFILESTR] 
WCHAR readonlypath[MAX_EFILESTR] 
char commandline[MAX_CMDLINE] 
long returnvalue
```

## Members
### instance
The handle to the program instance. Non Windows based applications can ignore it.

### currentpath
The full absolute path where the running application is located. Attach this path to filenames for accessing files.

### readonlypath
Some platforms may install resource files into this path which is read-only. This path can be different than the currentpath variable. Attach this path instead of currentpath to access these installation files.

### commandline
The parameters that have been passed to the program using the command line.

### returnvalue
The return value (exit code) when the application quits, return value 0 is default (no error). Symbian-based applications can ignore it.

## Description
This structure generic application data. Usually it won't be needed for the program to operate.

## Version changes
- EDGELIB 3.90: Added readonlypath member 
- EDGELIB 3.10: Removed caption and icon members 
- EDGELIB 2.90: Changed caption and currentpath members to WCHAR 
- EDGELIB 2.60: Added returnvalue 
- EDGELIB 2.10: Changed the caption variable to a string, it needs to be set by using a string copy

