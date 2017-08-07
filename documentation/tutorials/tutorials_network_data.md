:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Sending and receiving data

## Sending data
After establishing a connection, data can be sent between server and client. Data can be sent in packages managed by EDGELIB (a few bytes are padded to the sent data) or the data can be sent raw. Sent data managed by EDGELIB is easier to manage than raw data. This is because raw data may be cut in half while it's being sent (because of the network architecture).

Data is being sent by supplying the network class with a chunk of memory (we recommend using a structure with a message type and data). This data will be received on the other side. Here is an example to send data:

_Code (3 lines)_
```c++
char netmessage[128];
ClassEStd::StrCpy(netmessage, "This is a network message sent through the EDGELIB network connection class.");
ecd.net[0]->Send((unsigned char *)netmessage, strlen(netmessage) + 1);
```

## Receiving data
When data is ready to be received, a network event occurs. When data is received from a webserver, the connection is closed immediately. Raw data may interpreted directly after it has been received. However, data packages received may not be the same size as the packages that were sent. For example, when a client sends a package of 10 bytes and a package of 100 bytes, the server might receive a package of 30 bytes (containing the send data of 10 bytes + 20 bytes of the second package) and a package of 80 bytes. Data is sent in the correct order though.

Because this way of sending data may not be desired, EDGELIB contains functionality to ease this process. Don't send raw data (instead, use the regular `Send()` function) and work with network stacks. When running a server, each client needs a separate network stack for interpreting data packages. When a connection has been made, allocate data for a network stack and send received data to this stack. The stack will inform the program when a full data package is ready to be received. Here is an example on how to regulate network data using a network stack.

_Code (5 lines)_
```c++
class ClassMain : public ClassEdge
{
    private:
        ENetStack netstack;
    public:
```

_Code (20 lines)_
```c++
void ClassMain::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
{
    char testmsg[128];
    switch(msg)
    {
        case ENET_SERVERFOUND:
            netstack.Alloc(1024);
            break;
        case ENET_RECEIVEDATA:
            if (netstack.Push(buffer, bufsize))
            {
                unsigned long peeksize = netstack.Peek();
                if (peeksize == 128)
                {
                    netstack.Pop(testmsg, peeksize);
                }
            }
            break;
    }
}
```

## Chapters
* [Connections and protocols](tutorials_network_protocols.md)
* **Sending and receiving data**
* [Using HTTP requests](tutorials_network_http.md)

