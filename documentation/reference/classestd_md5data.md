:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# MD5Data


```c++
void static MD5Data(char *result, const void *buffer, unsigned long buflen)
```

## Parameters
### *result (out)
The string containing the result of the MD5 algorithm in hex.

### *buffer (in)
A buffer to calculate the MD5 hash from.

### buflen (in)
The size of the buffer.

## Return value
None.

## Description
Calculates the MD5 hash from the specified data buffer.

