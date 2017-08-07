:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SetSpeed


```c++
void SetSpeed(unsigned long nspeed)
```

## Parameters
### nspeed (in)
The new timer speed. Values smaller than 1000 makes the timer slower, values bigger than 1000 makes it faster.

## Return value
None.

## Description
Set the internal speed of the virtual timer. Retrieving differences in time will be influenced by this parameter.

