:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ValidateTransaction (deprecated)


```c++
void ValidateTransaction(bool debug = false)
```

## Parameters
### debug (in)
Enables debugging while using the ClickPayGo API.

## Return value
None.

## Description
Request a validation of a transaction. When the request is complete, it is sent in the form of a ENET_CPGVALIDATED message to the [EDGELIB framework](framework_onnetmessage.md).

## Version changes
- EDGELIB 4.00: Removed from SDK 
- EDGELIB 2.70: Added this function to the SDK

