:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# MD5File


```c++
void static MD5File(char *result, const char *filename) 
void static MD5File(char *result, ClassEFile *ef)
```

## Parameters
### *result (out)
The string containing the result of the MD5 algorithm in hex.

### *filename (in)
The name of the file to calculate the MD5 hash from.

### *ef (in)
A pointer to an open [ClassEFile object](classefile.md) to calculate the MD5 hash from.

## Return value
None.

## Description
Calculates the MD5 hash from the specified file.

