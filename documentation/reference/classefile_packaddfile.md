:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PackAddFile


```c++
ERESULT PackAddFile(const char *filename, bool overwrite = false, unsigned char compression = ECMP_ZLIB) 
ERESULT PackAddFile(const WCHAR *filename, bool overwrite = false, unsigned char compression = ECMP_ZLIB)
```

## Parameters
### *filename (in)
The name of the file to add.

### overwrite (in)
Set this to true if the file must be overwritten.

### compression (in)
A [compression method](classefile_definitions.md) to use when adding a file to the pack.

## Return value
`E_OK`: the file has been added. 
`E_NOENTRY`: the pack access isn't authorized. 
`E_TOOBIG`: the pack has too many files. 
`E_ALREADYFOUND`: the file is already found inside the pack. 
`E_NOTEXISTS`: the file that should be added is not found. 
`E_NODATA`: there is no data to add to the pack. 
`E_NOMEMORY`: there is not enough memory for this operation.

## Description
Add a file to an Edge pack.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function 
- EDGELIB 2.51: Changed the return value datatype to ERESULT

