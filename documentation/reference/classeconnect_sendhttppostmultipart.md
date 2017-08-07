:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SendHTTPPostMultipart


```c++
bool SendHTTPPostMultipart(EHTTPRequest *request)
```

## Parameters
### *request (in)
A pointer to an [EHTTPRequest object](ehttprequest.md) containing details about the HTTP request.

## Return value
Returns true if the host is found, otherwise returns false.

## Description
Send an HTTP request using POST multipart to a webserver. Data will be encoded as "multipart/form-data". This method automatically connects to the webserver, it isn't needed to call the `SearchHost` function. This POST method is suited for files and large amounts of data for uploading to a webserver.

## Version changes
- EDGELIB 3.70: Added return value

