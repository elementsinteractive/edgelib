:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# TimestampToDateTime


```c++
void static TimestampToDateTime(E_DATETIME *datetime, ETYPE_TIMESTAMP timestamp)
```

## Parameters
### *datetime (out)
A [datetime](ref_globalstructures.md) structure which will hold the converted value.

### timestamp (in)
A timestamp value.

## Return value
None.

## Description
Converts a timestamp value to a datetime structure.

## Version changes
- EDGELIB 2.70: Changed E_TIMESTAMP to ETYPE_TIMESTAMP 
- EDGELIB 2.60: Added this function to the SDK

