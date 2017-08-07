:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# AllocList


```c++
void *AllocList(unsigned long memsize)
```

## Parameters
### memsize (in)
The amount of memory to allocate.

## Return value
Return a pointer to the allocated chunk of memory. It returns NULL if allocation has failed.

## Description
Allocate memory dynamically. Always [deallocate](classememory_dealloclist.md) memory when it isn't needed anymore. Use the list function to log the amount of memory in use. Use matching allocate and deallocation functions: alloc/dealloc and alloclist/dealloclist.

