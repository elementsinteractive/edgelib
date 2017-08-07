:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# TimestampNow


```c++
ETYPE_TIMESTAMP static TimestampNow(bool islocal = true)
```

## Parameters
### islocal (in)
When set to true, the timestamp value will be adjusted for the local timezone and daylight saving time.

## Return value
A timestamp value of the current date and time.

## Description
This function can be used to retrieve the current date and time on the device in a timestamp value.

## Version changes
- EDGELIB 2.70: Changed E_TIMESTAMP to ETYPE_TIMESTAMP 
- EDGELIB 2.60: Added this function to the SDK

