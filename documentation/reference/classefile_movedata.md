:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# MoveData


```c++
bool MoveData(unsigned long dst, unsigned long src, unsigned long msize, unsigned long blocksize = 32768)
```

## Parameters
### dst (in)
The position in the file to copy to.

### src (in)
The position in the file to copy from.

### msize (in)
The amount of bytes to move.

### blocksize (in)
Move in blocks of the specified size. Bigger blocks need more memory, but the move operation will be faster.

## Return value
Returns `true` if the data has been successfully moved, otherwise `false`.

## Description
Move data within an opened file. Overlapped data will be moved correctly.

## Version changes
- EDGELIB 2.60: Added this function to the SDK

