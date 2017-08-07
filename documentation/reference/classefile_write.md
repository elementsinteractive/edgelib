:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Write


```c++
bool Write(const void *src, unsigned long wsize)
```

## Parameters
### *src (in)
A pointer to the memory to write the data to.

### wsize (in)
The amount of bytes to write to the file.

## Return value
Returns `true` if the data has been successfully written, otherwise `false`.

## Description
Write data to a file.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified

