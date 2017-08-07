:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PackGetFileData


```c++
ERESULT PackGetFileData(unsigned char *&ptr, unsigned long &datasize, unsigned long id) 
ERESULT PackGetFileData(unsigned char *&ptr, unsigned long &datasize, const char *filename) 
ERESULT PackGetFileData(unsigned char *&ptr, unsigned long &datasize, const WCHAR *filename)
```

## Parameters
### *&ptr (out)
A pointer that contains the file data, it doesn't need to be allocated. Deallocate the memory when it isn't needed anymore.

### &datasize (out)
The size of the filedata.

### id (in)
The file ID to request data from.

### *filename (in)
The filename to search for.

## Return value
`E_OK`: the data has been read 
`E_NOENTRY`: the pack access isn't authorized 
`E_NOTEXISTS`: the file ID or filename doesn't exist 
`E_NOMEMORY`: there is not enough available memory to read the data 
`E_NODATA`: there is no data to read

## Description
Read file data from a file within a pack.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added overloaded Unicode function 
- EDGELIB 2.60: Added the ability to search by filename. 
- EDGELIB 2.51: Changed the return value datatype to ERESULT

