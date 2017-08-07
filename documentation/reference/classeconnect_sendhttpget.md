:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SendHTTPGet


```c++
bool SendHTTPGet(const char *url, bool autoencode = false) 
bool SendHTTPGet(EHTTPRequest *request)
```

## Parameters
### *url (in)
The server to send the request to.

### *request (in)
A pointer to an [EHTTPRequest object](ehttprequest.md) containing details about the HTTP request.

### autoencode (in)
Set this to `true` to automatically encode all URL parameters.

## Return value
Returns true if the host is found, otherwise returns false.

## Description
Send an HTTP request using GET to a webserver. Parameters are encoded in the 'query string' part of the URL. This method automatically connects to the webserver, it isn't needed to call the `SearchHost` function.

## Version changes
- EDGELIB 3.70: Added return value

