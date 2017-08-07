:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetSerialNumber


```c++
char *GetSerialNumber(char *serialid) 
WCHAR *GetSerialNumber(WCHAR *serialid)
```

## Parameters
### *serialid (out)
This is a pointer to a string that holds the unique serial number of the device.

## Return value
The serial number of the device, the same as the parameter *serialid.

## Description
Retrieve an serial number or otherwise unique identifier of the device. Identifiers can be retrieved on GP2X (8 characters),  Android, and iPhone.

## See also
* [GetOwnerName](classedevice_getownername.md)
* [GetIMEI](classedevice_getimei.md)

## Notes
GP2X handhelds upgraded to firmware 2.0 won't report the serial number.

## Version changes
- EDGELIB 4.02: Replaced iPhone ID to conform to AppStore standards 
- EDGELIB 2.80: Added a Unicode overloaded function 
- EDGELIB 2.50: Added this function to the SDK

