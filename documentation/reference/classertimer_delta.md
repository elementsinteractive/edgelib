:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Delta


```c++
unsigned long Delta(unsigned long newtime, unsigned long oldtime)
```

## Parameters
### newtime (in)
A new time value in milliseconds.

### oldtime (in)
The previous time value in milliseconds.

## Return value
The difference between the two time parameters.

## Description
Calculate the difference in time. The calculation will also succeed when the `newtime` value has overflown its datatype.

