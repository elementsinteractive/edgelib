:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetBatteryLevel


```c++
unsigned char static GetBatteryLevel(void)
```

## Parameters
None.

## Return value
Returns the amount of energy left in the battery in a number between 0 (empty) and 255 (full). If reading of the battery is not supported by the device, the functions always returns 255.

## Description
Read the amount of energy left in the battery.

## Note
Check the battery status first using [GetBatteryStatus](classedevice_getbatterystatus.md). Currently support for iPhone, Windows Mobile, Symbian and GP2X.

## See also
[GetBatteryStatus](classedevice_getbatterystatus.md)

## Version changes
EDGELIB 3.987: Added support for iPhone 
- EDGELIB 2.50: Added this function to the SDK

