:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# EstablishConnection


```c++
ERESULT EstablishConnection(void)
```

## Parameters
None.

## Return value
`E_OK`: the connection is established 
`E_NOTCONNECTED`: The connection couldn't be established 
`E_ERROR`: a general error that doesn't fit in other categories

## Description
Prepares a TCP/IP connection by starting a connection to a WiFi access point, GPRS or UMTS. If this function isn't called before searching for a host, the connection module will call this function internally.

## Version changes
- EDGELIB 2.70: Added this function to the SDK

