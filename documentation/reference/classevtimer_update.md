:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Update


```c++
void Update(void) 
void Update(unsigned long ntime)
```

## Parameters
### ntime (in)
The current system time tick count.

## Return value
None.

## Description
Update the internal timer value and calculate the difference in time. When no argument is given, it will retrieve the time tick count internally.

