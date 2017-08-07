:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RequestVATPercentage (deprecated)


```c++
void RequestVATPercentage(char *username, bool debug = false)
```

## Parameters
### *username (in)
A string containing the username of the customer.

### debug (in)
Enables debugging while using the ClickPayGo API.

## Return value
None.

## Description
Request the current VAT percentage. This percentage can vary depending on the location of the customer. When the request is complete, it is sent in the form of a ENET_CPGVATPERC message to the [Edge framework](framework_onnetmessage.md).

## Version changes
- EDGELIB 4.00: Removed from SDK 
- EDGELIB 2.70: Added this function to the SDK

