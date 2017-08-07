:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# FloatToFixed


```c++
long static FloatToFixed(unsigned long value, int fixedprecision = 16)
```

## Parameters
### value (in)
A float value read as an int.

### fixedprecision (in)
The amount of bits to use for fixed point data precision.

## Return value
The converted value.

## Description
This function is useful for devices that doesn't natively support floating point data types, but the application needs to read files containing data represented in floating point. Read them as a 32-bit integer value, and convert them using this function.

## Version changes
- EDGELIB 2.50: Added this function to the SDK

