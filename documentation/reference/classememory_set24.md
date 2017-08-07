:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Set24


```c++
void Set24(void *dest, unsigned long value, unsigned long count)
```

## Parameters
### *dest (out)
The destination memory to set.

### value (in)
The value to set the memory with.

### count (in)
The amount of memory that should be set with the specified value.

## Return value
None.

## Description
Set a large amount of memory with the same values. The count parameter indicates the number of values to set, rather than bytes. For example, if the memory should be set with 128 values, the memory will be filled with 384 bytes.

