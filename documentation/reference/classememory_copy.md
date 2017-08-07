:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Copy


```c++
void static Copy(const void *dest, const void *src, int count, bool checkoverlap = true)
```

## Parameters
### *dest (out)
The destination memory.

### *src (in)
The source memory.

### count (in)
The amount of memory to copy in bytes.

### checkoverlap (in)
Check for overlapped memory, use an alternative copy procedure when needed.

## Return value
None.

## Description
Copy an amount of memory from the source to the destination.

## Version changes
- EDGELIB 2.60: Added checking for overlapped memory

