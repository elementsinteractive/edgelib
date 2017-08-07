:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetOSName


```c++
void static GetOSName(char *osname) 
void static GetOSName(WCHAR *osname)
```

## Parameters
### *osname (out)
A string that contains the operating system name.

## Return value
None.

## Description
This function can be used to get the operating system name. Use it to write code for specific operating systems. It may include one of the following names:

* Symbian S60
* Symbian Series 60
* Symbian Series 80
* Symbian Series 90
* Symbian UIQ
* Symbian
* Windows Mobile Pocket PC
* Windows Mobile Smartphone
* Windows Mobile
* Windows 2003
* Windows XP
* Windows 2000
* Windows NT
* Windows ME
* Windows 98
* Windows 95
* Linux
* Unknown

## Version changes
- EDGELIB 2.90: Added this function to the SDK

