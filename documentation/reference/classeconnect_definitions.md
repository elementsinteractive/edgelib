:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Definitions

## Network protocols
```c++
ECONN_NONE       //No protocol specified 
ECONN_SOCKET     //Uses TCP/IP sockets for internet and WiFi connections 
ECONN_BLUETOOTH  //Uses the Bluetooth RFCOMM protocol  
```

Different network protocols for setting up servers, connect to clients and send/receive data.

## Message types
```c++
ENET_SERVERFOUND   //A server has been found, buffer = server address in ASCII 
ENET_RUNNINGSERVER //The server is running, buffer = server address in ASCII 
ENET_ADDCLIENT     //A client has been added, buffer = client address in ASCII 
ENET_CLOSEDSOCKET  //A client socket has been closed 
ENET_RECEIVEDATA   //Data has been received, buffer = received data 
ENET_DEVICEFOUND   //A device has been found, buffer = EBLUETOOTH_DEVICEID structure 
ENET_SERVICEFOUND  //A service has been found, buffer = EBLUETOOTH_SERVICEID structure 
ENET_ENDSEARCH     //The search for devices is complete 
ENET_USER          //User defined network messages  
```

Messages that are passed to the framework when a network event occurs. There is no certainty the ENET_ENDSEARCH message is received, it is recommended to add a 'stop search' button to the search interface. Up to 128 custom network messages can be defined by adding ENET_USER. Extended network APIs which uses their own message definitions are: [ClickPayGo](classclickpaygo_netmessage.md)

## HTTP Request connection types
```c++
EHTTP_CONNECTION_NONE      //Connection not specified 
EHTTP_CONNECTION_KEEPALIVE //Keep the connection alive 
EHTTP_CONNECTION_CLOSE     //Close the connection after receiving data  
```

Extra connection parameter specified when using [EHTTPRequest](ehttprequest.md). It changes the connection behaviour of the webserver.

## Version changes
- EDGELIB 3.10: Added `ENET_SERVICEFOUND` 
- EDGELIB 2.80: Added `EHTTP_CONNECTION_NONE`, `EHTTP_CONNECTION_KEEPALIVE` and `EHTTP_CONNECTION_CLOSE` 
- EDGELIB 2.70: Added `ENET_USER`

