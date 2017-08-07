:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RequestPing


```c++
void RequestPing(unsigned char connection = 0)
```

## Parameters
### connection (in)
The connection to send the request to. The server connection uses 0, clients start from 1.

## Return value
None.

## Description
Send a ping request inside an EDGELIB network package to a client.

