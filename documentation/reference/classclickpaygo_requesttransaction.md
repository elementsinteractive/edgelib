:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RequestTransaction (deprecated)


```c++
void RequestTransaction(char *username, char *password, char *itemdesc, ETYPE_MONEY itemprice, char *itemcurrency, bool debug = false)
```

## Parameters
### *username (in)
A string containing the username of the customer.

### *password (in)
A string containing the password of the customer.

### *itemdesc (in)
A string containing the description of the item.

### itemprice (in)
The VAT excluded price of the item.

### *itemcurrency (in)
A string containing the currency of the item. The following currencies are supported: "USD", "GBP" and "EUR".

### debug (in)
Enables debugging while using the ClickPayGo API.

## Return value
None.

## Description
Request a transaction. The credits deducted from the user account can be higher than the given price if VAT is applied. It's recommended to display the VAT included price to the customer which can be retrieved by calling [RequestGrossPrice](classclickpaygo_requestgrossprice.md). When the request is complete, it is sent in the form of a ENET_CPGTRANSACTION message to the [EDGELIB framework](framework_onnetmessage.md).

## Version changes
- EDGELIB 4.00: Removed from SDK 
- EDGELIB 2.70: Added this function to the SDK

