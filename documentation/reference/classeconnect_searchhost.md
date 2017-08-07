:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SearchHost


```c++
bool SearchHost(const char *address, int port)
```

## Parameters
### *address (in)
The TCP/IP host name, IP address (formatted as 192.16.0.4) or Bluetooth address (formatted as 00:00:64:ac:d4:f9).

### port (in)
The TCP/IP port to connect to.

## Return value
Return true if the search request has been successful, otherwise return false.

## Description
Search for a host. The framework class is notified through the [OnNetMessage](framework_onnetmessage.md) callback function when the host is found.

Pass NULL as the address parameter to set up a server.

