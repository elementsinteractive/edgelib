:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnNetMessage


```c++
void OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
```

## Parameters
### connection (out)
The connection from which the event occurs.

### id (out)
The client from which the event occurs, server events use 0 as an id.

### msg (out)
The [message type](classeconnect_definitions.md) of the event.

### bufsize (out)
The size of the event buffer.

### *buffer (out)
A buffer containing additional event info.

## Return value
None.

## Description
This is a callback when a network event occurs. Use it to read the status of active network connections.

