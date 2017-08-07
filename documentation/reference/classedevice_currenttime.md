:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# CurrentTime


```c++
void static CurrentTime(unsigned char &hour, unsigned char &minute, unsigned char &second, bool islocal = true)
```

## Parameters
### &hour (out)
The current hour.

### &minute (out)
The current minute.

### &second (out)
The current second.

### islocal (in)
When set to true, the time values will be adjusted for the local timezone and daylight saving time.

## Return value
None.

## Description
This function can be used to retrieve the current time on the device.

## Version changes
- EDGELIB 2.60: Added the islocal parameter 
- EDGELIB 2.20: Made this function static

