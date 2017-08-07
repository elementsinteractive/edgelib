:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnFrameworkConfig


```c++
ERESULT OnFrameworkConfig(EFRAMEWORKCONFIG *config)
```

## Parameters
### *config (out)
A pointer to an [EFRAMEWORKCONFIG structure](ecd_eframeworkconfig.md). Use it to configure the framework EDGELIB.

## Return value
Return `E_OK` when everything went correctly. Return `E_ERROR` to shutdown EDGELIB.

## Description
This is a callback for configuring EDGELIB and is called just before the engine starts.

## Version changes
- EDGELIB 3.10: Added this function to the SDK

