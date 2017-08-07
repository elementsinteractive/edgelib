:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# RespondPing


```c++
void RespondPing(unsigned char connection = 0)
```

## Parameters
### connection (in)
The connection to send the response to. The server connection uses 0, clients start from 1.

## Return value
None.

## Description
This function can be used to respond when a ping has been requested through the connection.

