:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Send


```c++
void Send(const unsigned char *buffer, long buflen, unsigned char connection = 0)
```

## Parameters
### *buffer (in)
The buffer containing network data.

### buflen (in)
The amount of data inside the buffer in bytes.

### connection (in)
The connection to send to. The server connection uses 0, clients start from 1.

## Return value
None.

## Description
Send data over the network encapsulated in EDGELIB packages. When receiving data from these packages, use the [network data stack](enetstack.md).

