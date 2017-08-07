:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PackAddFileData


```c++
ERESULT PackAddFileData(const char *filename, unsigned char *data, unsigned long datasize, bool overwrite = false, unsigned char compression = ECMP_ZLIB, ETYPE_TIMESTAMP filedate = EDT_TIMESTAMPNOW) 
ERESULT PackAddFileData(const WCHAR *filename, unsigned char *data, unsigned long datasize, bool overwrite = false, unsigned char compression = ECMP_ZLIB, ETYPE_TIMESTAMP filedate = EDT_TIMESTAMPNOW)
```

## Parameters
### *filename (in)
The name of the file to add.

### *data (in)
A pointer to the data to add to the pack.

### datasize (in)
The size of the data to add to the pack.

### overwrite (in)
Set this to true if the file must be overwritten.

### compression (in)
A [compression method](classefile_definitions.md) to use when adding data to the pack.

### filedate (in)
The date to give to the file.

## Return value
E_OK: the file has been added 
E_NOENTRY: the pack access isn't authorized 
E_TOOBIG: the pack has too many files 
E_ALREADYFOUND: the file is already found inside the pack 
E_NOTEXISTS: the file that should be added is not found 
E_NODATA: there is no data to add to the pack 
E_NOMEMORY: there is not enough memory for this operation

## Description
Add filedata to an EDGELIB resource pack.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function 
- EDGELIB 2.70: Changed E_TIMESTAMP to ETYPE_TIMESTAMP and changed TIMESTAMP_NOW to EDT_TIMESTAMPNOW 
- EDGELIB 2.60: Added the filedate parameter 
- EDGELIB 2.51: Changed the return value datatype to ERESULT

