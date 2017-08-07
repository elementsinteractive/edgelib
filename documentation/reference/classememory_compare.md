:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Compare


```c++
int Compare(const void *src1, const void *src2, int memsize)
```

## Parameters
### *src1 (in)
A pointer to the first memory block.

### *src2 (in)
A pointer to the second memory block.

### memsize (in)
The amount of memory to compare in bytes.

## Return value
Returns the difference between the two memory blocks. Returns zero when they are equal.

## Description
Compare two memory blocks.

