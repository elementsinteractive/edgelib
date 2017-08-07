:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetRegistrationId


```c++
char *GetRegistrationId(char *registrationid) 
WCHAR *GetRegistrationId(WCHAR *registrationid)
```

## Parameters
### *registrationid (out)
This is a pointer to a string that holds the registration ID of the device.

## Return value
The registration ID, the same as the parameter *registrationid.

## Description
Retrieve a registration ID which can be used for registration protection of software. The return value is unique for every device. The GP2X returns a serial number (where possible), Symbian devices return the IMEI code and Windows Mobile devices return the owner name of the device.

## Version changes
- EDGELIB 2.80: Added a unicode overloaded function 
- EDGELIB 2.50: Added this function to the SDK

