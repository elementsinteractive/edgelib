:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Networking

## TCP/IP network connections
One of the network protocols EDGELIB supports is TCP/IP. You can use it to connect to any internet host for communication.

A client connection to a server can be made by connecting to an IP address, or a TCP/IP server can be started. When a connection is created, data can be received and sent to and from clients. Use network connections to add multi-player capabilities to your games: EDGELIB notifies the application with events through the framework.

Aditionally, EDGELIB offers separate functions to send HTTP requests and process HTTP results. Use HTTP functionality to dynamically download game content or submit highscores to a webserver.

EDGELIB also supports Bluetooth.


## Bluetooth network connections and device searching
Besides TCP/IP, EDGELIB also supports the Bluetooth network protocol. Bluetooth works similar to TCP/IP, with only one difference. Bluetooth has the capability to search for devices around approximately 10 meters (32 feet). Bluetooth is excellent for creating multiplayer games. It's possible to let the bluetooth hardware search for devices, without interrupting your application.

An EDGELIB Bluetooth server is a service which can be found with most Bluetooth software. It's even possible to connect to this service with Hyperterminal for debugging purposes. An EDGELIB Bluetooth client searches EDGELIB Bluetooth servers running the same application.

For Windows Mobile, EDGELIB currently supports the Microsoft Bluetooth stack.


## Network data stacks
Because network data is sent by separate packages, data stacks are provided by EDGELIB for improved handling of network data. Data stacks are used to store data and give information when a complete network package can be read. There are two stacks, one for regular network data and one for data returned after a HTTP request.

Network data stacks will help reduce the complications that occur when working with network data. It will be easier creating multi-player games with EDGELIB.
