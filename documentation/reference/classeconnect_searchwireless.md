:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# SearchWireless


```c++
unsigned long SearchWireless(unsigned long numdevices = 16, bool searchservices = false)
```

## Parameters
### numdevices (in)
The maximum amount of devices to search for.

### searchservices (in)
When enabled, search for services on all detected devices.

## Return value
Returns a positive value if the search has been started successfully.

## Description
Search for wireless devices with the bluetooth protocol. The wrapper class is notified through the [OnNetMessage](framework_onnetmessage.md) callback function if a new device or service has been found.

When searchservices is turned off, it only returns device id's with no port information. When searchservices is turned on it will return service id's and which port it's being advertised on. It will also search for an EDGELIB service with a matching UID and returns that as a device id with the correct port information.

## Version changes
- EDGELIB 3.30: Changed functionality of the searchservices parameter, also this function works asynchronously for Windows Mobile devices now 
- EDGELIB 3.10: Enable searching for Bluetooth services 
- EDGELIB 2.80: Set parameter numdevices to default value 16

