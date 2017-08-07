:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ReAlloc


```c++
void static *ReAlloc(void *ptr, unsigned long memsize)
```

## Parameters
### *ptr (in)
Pointer to a previously allocated block of memory.

### memsize (in)
The new size of the memory block.

## Return value
Return a pointer to the new reallocated chunk of memory. It returns NULL if reallocation has failed.

## Description
Reallocates a block of memory, the new size can be bigger or smaller than the previous size. When the memory block has moved, it returns the pointer to the new memory block, otherwise the return value is the same as the first parameter. When the memory block changes its location, the data will be copied over automatically and the previous memory block will be freed. If there is not enough memory, the function returns NULL and the original pointer is preserved.

## Version changes
- EDGELIB 3.30: Added this function to the EDGELIB SDK

