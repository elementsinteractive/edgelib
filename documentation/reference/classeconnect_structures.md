:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Structures

## EBLUETOOTH_DEVICEID
```c++
struct 
{ 
    char name[64] 
    char address[18]; 
    unsigned char port 
}EBLUETOOTH_DEVICEID
```

This contains general information about a Bluetooth device. When the device has no name, the name string is empty.

## EBLUETOOTH_SERVICEID
```c++
struct 
{ 
    char devicename[64] 
    char servicename[64] 
    char address[18]; 
    unsigned char port 
}EBLUETOOTH_SERVICEID
```

A Bluetooth Device ID including the name of a found bluetooth service.

## EHTTPREQUEST_INFO
```c++
struct 
{ 
    char host[512] 
    char path[512] 
    char referer[1024] 
    char useragent[128] 
    char username[128] 
    char password[128] 
    unsigned char connection 
    int port 
}EHTTPREQUEST_INFO
```

The HTTP request information contains member variables defining the information to send with an HTTP request.

## EHTTPRESPONSE_INFO
```c++
struct 
{ 
    char contentencoding[16] 
    char contenttype[128] 
    char server[256] 
    char location[1024] 
    char date[48] 
    long contentlength 
    unsigned short status 
}EHTTPRESPONSE_INFO
```

The HTTP header contains the information received from a webserver, after sending a request.

## Version changes
- EDGELIB 3.30: Changed bluetooth addresses to ascii strings 
- EDGELIB 2.90: Added EBLUETOOTH_SERVICEID 
- EDGELIB 2.90: Added port attribute to EBLUETOOTH_DEVICEID 
- EDGELIB 2.80: Added connection attribute to EHTTPREQUEST_INFO

