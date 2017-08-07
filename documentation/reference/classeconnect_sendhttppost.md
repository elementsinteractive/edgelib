:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SendHTTPPost


```c++
bool SendHTTPPost(const char *url, bool autoencode = false) 
bool SendHTTPPost(EHTTPRequest *request)
```

## Parameters
### *url (in)
The URL containing information (host, port, parameters) about the request.

### *request (in)
A pointer to an [EHTTPRequest object](ehttprequest.md) containing details about the HTTP request.

### autoencode (in)
Set this to `true` to automatically encode all URL parameters.

## Return value
Returns true if the host is found, otherwise returns false.

## Description
Send an HTTP request using POST to a webserver. Data will be encoded as "application/x-www-form-urlencoded". This method automatically connects to the webserver, it isn't needed to call the `SearchHost` function. This POST method is suited for small amounts of data (with sizes not larger than 4 kilobyte).

## Version changes
- EDGELIB 3.70: Added return value

