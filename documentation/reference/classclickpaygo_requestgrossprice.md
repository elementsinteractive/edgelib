:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RequestGrossPrice (deprecated)


```c++
void RequestGrossPrice(char *username, ETYPE_MONEY itemprice, bool debug = false)
```

## Parameters
### *username (in)
A string containing the username of the customer.

### itemprice (in)
The VAT excluded price.

### debug (in)
Enables debugging while using the ClickPayGo API.

## Return value
None.

## Description
Request a VAT included price. This price can vary depending on the location of the customer. When the request is complete, it is sent in the form of a `ENET_CPGGROSSPRICE` message to the [EDGELIB framework](framework_onnetmessage.md).

## Version changes
- EDGELIB 4.00: Removed from SDK 
- EDGELIB 2.70: Added this function to the SDK

