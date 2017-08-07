:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# FixedToFloat


```c++
unsigned long static FixedToFloat(long value, int fixedprecision = 16)
```

## Parameters
### value (in)
A fixed point value.

### fixedprecision (in)
The amount of bits to use for fixed point data precision.

## Return value
The converted floating point value which can be written as an int.

## Description
This function is useful for devices that doesn't natively support floating point data types, but the application needs to write files or send data containing data represented in floating point. Convert them using this function and write as a 32-bit integer value.

## Version changes
- EDGELIB 2.70: Added this function to the SDK

