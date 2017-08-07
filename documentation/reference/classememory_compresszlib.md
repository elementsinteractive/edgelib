:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# CompressZLib


```c++
bool CompressZLib(void *dst, unsigned long &dst_size, const void *src, unsigned long src_size)
```

## Parameters
### *dst (out)
The destination memory that holds the compressed data.

### &dst_size (out)
The size of the compressed memory.

### *src (in)
The source memory to compress.

### src_size (in)
The size of the source memory block.

## Return value
Returns true if compression has been successful, return false if compression failed or when the compressed size is bigger than the source.

## Description
Compress memory using ZLib. Allocate memory for compressed data that is the same amount as the source memory + 10% and 12 bytes.

