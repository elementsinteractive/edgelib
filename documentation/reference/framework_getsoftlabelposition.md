:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetSoftlabelPosition


```c++
unsigned char GetSoftlabelPosition(unsigned char *firstkeyposition = NULL)
```

## Parameters
### *firstkeyposition (out)
The position of the first softkey label relative to the position of the second softkey label.

## Return value
The position of the softkey labels, please check the [position definitions](ref_definitions.md) for the possible return values.

## Description
Use this function to receive the position of the softkey labels. This is useful for orientation aware applications.

## Version changes
- EDGELIB 3.30: Added this function to the EDGELIB SDK

