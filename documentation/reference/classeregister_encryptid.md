:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# EncryptId


```c++
char *EncryptId(char *newid, const char *id) 
WCHAR *EncryptId(WCHAR *newid, const WCHAR *id)
```

## Parameters
### *newid (out)
This is a pointer to a string that holds the encrypted ID (13 characters including the zero character).

### *id (in)
The ID to encrypt.

## Return value
The encrypted ID, the same as the parameter *newid.

## Description
Calculate a new ID based on another ID. This can be used to encrypt a unique ID into something else.

## Version changes
- EDGELIB 2.80: Added Unicode overloaded function

