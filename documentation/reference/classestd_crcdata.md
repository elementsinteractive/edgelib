:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# CRCData


```c++
unsigned long static CRCData(const void *buffer, unsigned long buflen)
```

## Parameters
### *buffer (in)
A buffer to calculate the CRC checksum from.

### buflen (in)
The size of the buffer.

## Return value
A 32-bit CRC checksum.

## Description
Calculates the 32-bit CRC checksum from the specified data buffer.

## Version changes
- EDGELIB 3.10: Added this function to the SDK

