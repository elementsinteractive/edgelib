:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Connections and protocols

## About network connections
EDGELIB features a class to set up a client-server connection between two or more devices. It's also possible to connect to a webserver to request data. There are currently two protocols supported: TCP/IP and Bluetooth. When two devices connect, a server with the same protocol must be present on one of these devices (for example, a TCP/IP client isn't able to connect with a Bluetooth server).

It's also possible to have multiple simultaneous connections. A gameserver with connected clients on connection 1 is able to retrieve data from a webserver on connection 2. This tutorial will describe how to connect, send and receive data between two devices.

## Starting a server
When starting up a connection a device can be a client or a server. This example shows how to set up a listening server using TCP/IP or Bluetooth. Before setting up a connection, a port is needed to listen on. For TCP/IP a port can be a value higher than 4000 (available to developers). For Bluetooth the port can be zero, EDGELIB will read an available Bluetooth port to listen on from the device. The following example connects using Bluetooth or TCP/IP:

_Code (23 lines)_
```c++
bool Connect(bool usebluetooth)
{
    if (usebluetooth)
    {
        if (ecd.net[0]->Init(ECONN_BLUETOOTH, this) == E_OK)
        {
            if (!ecd.net[0]->SearchHost(NULL, 0))
                return(false);
        }
        else
            return(false);
    }
    else
    {
        if (ecd.net[0]->Init(ECONN_SOCKET, this) == E_OK)
        {
            if (!ecd.net[0]->SearchHost(NULL, 4000))
                return(false);
        }
        else
            return(false);
    }
}
```

When the server has been set up, a message is posted to the network message callback function:

_Code (8 lines)_
```c++
//Callback: Called when a network event occurs
void ClassMain::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
{
    if (msg == ENET_RUNNINGSERVER)
    {
        //The server is running
    }
}
```

The buffer contains the IP address of the device itself. The buffer size indicates the length of the string.

## Connecting a client
When a server is started, a client is able to connect with it using the same port number. A string will contain the address to connect to. It should be the same value the server receives in a buffer when receiving `ENET_RUNNINGSERVER`. Here is an example on how to connect to a server:

_Code (23 lines)_
```c++
bool Connect(bool usebluetooth)
{
    if (usebluetooth)
    {
        if (ecd.net[0]->Init(ECONN_BLUETOOTH, this) == E_OK)
        {
            if (!ecd.net[0]->SearchHost("00:00:54:1F:8D:24", 4))
                return(false);
        }
        else
            return(false);
    }
    else
    {
        if (ecd.net[0]->Init(ECONN_SOCKET, this) == E_OK)
        {
            if (!ecd.net[0]->SearchHost("194.16.0.5", 4000))
                return(false);
        }
        else
            return(false);
    }
}
```

When the client has been connected, a message is posted to the network message callback function:

_Code (8 lines)_
```c++
//Callback: Called when a network event occurs
void ClassMain::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
{
    if (msg == ENET_SERVERFOUND)
    {
        //The client has been connected
    }
}
```

The buffer contains the IP address of the server. The buffer size indicates the length of the string.

## Other network events
While a server is started or a client connected, messages about the status of the active connection can be received. A server will receive a message after a client has been connected (the IP address can be read from the buffer), and both server and client will receive a message when the connection has been closed or interrupted:

_Code (12 lines)_
```c++
//Callback: Called when a network event occurs
void ClassMain::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
{
    if (msg == ENET_ADDCLIENT)
    {
        //A client has been added to the server
    }
    else if (msg == ENET_CLOSEDSOCKET)
    {
        //Closed connection
    }
}
```

## Searching for devices
Bluetooth has the capability to search for devices around 10 or 100 meters of a device (the distance depends on the power of the receiver). EDGELIB is able to initiate a device search and will receive events once devices are found. This search process can only be interrupted for Symbian devices. After all devices are found, a message will be received telling that the search has ended. An example on how to initiate a Bluetooth device search to search for a maximum of eight devices:

_Code (2 lines)_
```c++
if (ecd.net[0]->Init(ECONN_BLUETOOTH, this) == E_OK)
    ecd.net[0]->SearchWireless(8);
```

These events will be received during a device search:

_Code (14 lines)_
```c++
//Callback: Called when a network event occurs
void ClassMain::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
{
    EBLUETOOTH_DEVICEID *deviceinfo;
    if (msg == ENET_DEVICEFOUND)
    {
        deviceinfo = (EBLUETOOTH_DEVICEID *)buffer;
        //A bluetooth device has been found
    }
    else if (msg == ENET_ENDSEARCH)
    {
        //Bluetooth searching has ended
    }
}
```

The device searcher only finds Bluetooth devices running an EDGELIB server with the same UID. The exception are connections between Symbian 9.1 and older Symbian devices. Then it only looks at the last 7 hexadecimal numbers of the UID (for example, a client with UID `0x10205d9d` is able to find a server with UID `0xf0205d9d`). Connect to the server address and port located in the `EBLUETOOTH_DEVICEID` structure.

## Chapters
* **Connections and protocols**
* [Sending and receiving data](tutorials_network_data.md)
* [Using HTTP requests](tutorials_network_http.md)

