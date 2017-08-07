:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# EDGESTARTUP


```c++
ENATIVETYPE instance 
char commandline[MAX_CMDLINE] 
long returnvalue
```

## Members
### instance
The handle to the program instance. Non Windows based applications can ignore it.

### commandline
The parameters that have been passed to the program using the command line.

### returnvalue
The return value (exit code) when the application quits, return value 0 is default (no error). Symbian-based applications can ignore it.

## Description
This structure contains data collected at program startup.

## Version changes
- EDGELIB 3.10: Added this structure to the SDK

