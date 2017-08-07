:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Rand


```c++
unsigned long Rand() 
unsigned long Rand(long min, long max)
```

## Parameters
### min (in)
The minimum return value.

### max (in)
The maximum return value.

## Return value
A random value between 0 and 4294967295 or specified by the minimum and maximum value.

## Description
Returns a random value.

## Version changes
- EDGELIB 3.10: Changed parameters to signed, to enable generating negative random numbers 
- EDGELIB 2.95: Added this function to the SDK

