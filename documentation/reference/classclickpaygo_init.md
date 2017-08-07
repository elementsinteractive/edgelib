:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Init (deprecated)


```c++
void Init(ClassEdge *pedge, ClassEConnect *net, char *licensekey, char *secret)
```

## Parameters
### *pedge (in)
A pointer to the [framework class](framework.md).

### *net (in)
The pointer to the [network class](classeconnect.md).

### *licensekey (in)
The ClickPayGo licensekey, a 32 character string.

### *secret (in)
A secret string for the license.

## Return value
None.

## Description
Initialize and attach the ClickPayGo interface class to the network module. Call this function once before using this class.

## Version changes
- EDGELIB 4.00: Removed from SDK 
- EDGELIB 2.70: Added this function to the SDK

