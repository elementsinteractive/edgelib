:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetIMEI


```c++
char *GetIMEI(char *imeicode, bool includedashes = false) 
WCHAR *GetIMEI(WCHAR *imeicode, bool includedashes = false)
```

## Parameters
### *imeicode (out)
This is a pointer to a string that holds the IMEI code of the device.

### includedashes (in)
When set to true, it formats the IMEI code with dashes in the following format: 012345-01-012345-9

## Return value
The IMEI code of the device, the same as the parameter *imeicode.

## Description
Retrieve the IMEI code of phone devices. Other devices return an empty string.

## See also
* [GetOwnerName](classedevice_getownername.md)
* [GetSerialNumber](classedevice_getserialnumber.md)

## Version changes
- EDGELIB 3.00: Added an optional parameter for alternative IMEI code formatting 
- EDGELIB 2.80: Added Unicode overloaded function 
- EDGELIB 2.50: Moved this function from ClassERegister to ClassEDevice

