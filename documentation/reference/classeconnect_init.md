:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Init


```c++
ERESULT Init(unsigned short protocol, ClassEdge *pedge)
```

## Parameters
### protocol (in)
The [network protocol](classeconnect_definitions.md) to use.

### *pedge (in)
A pointer to the [framework class](framework.md).

## Return value
E_OK: the connection class has been initialized successfully 
E_INVALIDPARAM: the pointer to the wrapper class is invalid 
E_UNSUPPORTED: the requested protocol isn't supported 
E_ERROR: a general error that doesn't fit in other categories

## Description
Initialize the connection using the desired protocol.

