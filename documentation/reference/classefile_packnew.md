:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# PackNew


```c++
ERESULT PackNew(const char *filename, const char *vendor) 
ERESULT PackNew(const WCHAR *filename, const WCHAR *vendor)
```

## Parameters
### *filename (in)
The name of the pack to create.

### *vendor (in)
A string containing a four character code. A unique code is handed out to registered EDGELIB licensees.

## Return value
`E_OK`: the pack has been created. 
`E_ALREADYFOUND`: the file already exists. 
`E_ERROR`: error creating file.

## Description
Create a new pack file. The vendor ID "FREE" is available for every developer.

## Version changes
- EDGELIB 3.10: Added const keyword for parameters that aren't modified 
- EDGELIB 2.90: Added Unicode overloaded function 
- EDGELIB 2.51: Changed the return value to ERESULT

