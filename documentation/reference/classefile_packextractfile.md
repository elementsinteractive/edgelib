:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PackExtractFile


```c++
ERESULT PackExtractFile(const char *path, unsigned long id, bool overwrite = false) 
ERESULT PackExtractFile(const char *path, const char *filename, bool overwrite = false) 
ERESULT PackExtractFile(const WCHAR *path, unsigned long id, bool overwrite = false) 
ERESULT PackExtractFile(const WCHAR *path, const WCHAR *filename, bool overwrite = false)
```

## Parameters
### *path (in)
The path to extract files to, set to NULL to extract to the current path.

### id (in)
The ID of the file to extract.

### *filename (in)
The name of the file to extract.

### overwrite (in)
Overwrites the destination file if it already exists.

## Return value
`E_OK`: the file has been extracted 
`E_NOENTRY`: the pack access isn't authorized 
`E_NOTEXISTS`: the file that should be extracted is not found 
`E_ALREADYFOUND`: there is already a file with the same filename in the extraction folder 
`E_ERROR`: a general error not fitting into other categories

## Description
Extract a file from the pack.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function 
- EDGELIB 2.60: Added the ability to search by filename 
- EDGELIB 2.51: Changed the return value datatype to ERESULT

