:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Memory management

## Basic memory functions
EDGELIB offers several functions to manipulate memory. Fill memory with a list of the same value, copy memory from source to destination and compare two buffers of memory.

Overlap checking to ensure the data is being copied correctly was added to EDGELIB in the 2.60 release.


## Dynamic memory allocation and deallocation
Memory can be allocated and deallocated dynamically through the memory module. Memory should be allocated dynamically when trying to store large amounts of game data.

As of EDGELIB 3.30 it's also possible to reallocate memory to expand or shrink an allocated memory block and keeping its data intact.


## Memory compression
Memory can be compressed by using two methods. Compression can be done using Zlib or using the internal EDGELIB compression algorithm.
