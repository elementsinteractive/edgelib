:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# ReleaseConnection


```c++
void ReleaseConnection(void)
```

## Parameters
None.

## Return value
None.

## Description
Releases a previously [established](classeconnect_establishconnection.md) TCP/IP connection. If this function isn't called before cleaning up, the connection module will call this function internally.

## Version changes
- EDGELIB 2.70: Added this function to the SDK

