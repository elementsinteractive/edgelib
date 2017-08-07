:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PackDeleteFile


```c++
ERESULT PackDeleteFile(unsigned long id) 
ERESULT PackDeleteFile(const char *filename) 
ERESULT PackDeleteFile(const WCHAR *filename)
```

## Parameters
### id (in)
The ID of the file to delete.

### *filename (in)
The name of the file to delete.

## Return value
`E_OK`: the file has been deleted 
`E_NOENTRY`: the pack access isn't authorized 
`E_NOTEXISTS`: the file that should be deleted is not found 
`E_NOMEMORY`: there is not enough memory for this operation 
`E_ERROR`: a general error not fitting into other categories

## Description
Delete a file from the pack.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function 
- EDGELIB 2.60: Added the ability to search by filename 
- EDGELIB 2.51: Changed the return value datatype to ERESULT

