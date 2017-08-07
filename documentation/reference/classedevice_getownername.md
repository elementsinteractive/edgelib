:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetOwnerName


```c++
char *GetOwnerName(char *owner) 
WCHAR *GetOwnerName(WCHAR *owner)
```

## Parameters
### *owner (out)
This is a pointer to a string that holds the owner name of the device.

## Return value
The owner name, the same as the parameter *owner.

## Description
Retrieve the owner ID or owner name of the device. This function only works on Windows Mobile devices and Windows desktop. Other operating systems return an empty string.

## See also
* [GetIMEI](classedevice_getimei.md)
* [GetSerialNumber](classedevice_getserialnumber.md)

## Version changes
- EDGELIB 2.80: Added Unicode overloaded function 
- EDGELIB 2.50: Moved this function from ClassERegister

