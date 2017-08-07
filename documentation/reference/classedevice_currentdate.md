:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# CurrentDate


```c++
void static CurrentDate(unsigned long &year, unsigned char &month, unsigned char &day, bool islocal = true)
```

## Parameters
### &year (out)
The current year.

### &month (out)
The current month.

### &day (out)
The current day of the month.

### islocal (in)
When set to true, the date values will be adjusted for the local timezone and daylight saving time.

## Return value
None.

## Description
This function can be used to retrieve the current date on the device.

## Version changes
- EDGELIB 2.60: Added the islocal parameter 
- EDGELIB 2.20: Added this function to the SDK

