:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetResponseInfo


```c++
bool GetResponseInfo(EHTTPRESPONSE_INFO *info) 
bool GetResponseInfo(char *rawinfo)
```

## Parameters
### *info (out)
A pointer to an [`EHTTPRESPONSE_INFO` structure](classeconnect_structures.md) containing more information about the HTTP response.

### *rawinfo (out)
Receive HTTP response information in a raw format.

## Return value
Returns true if the information can be found within the stack, otherwise returns false.

## Description
Receive more information about the HTTP response from the webserver.

