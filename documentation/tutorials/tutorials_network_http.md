:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Using HTTP requests

## Sending requests to a webserver
EDGELIB also contains useful utility functions for setting up an HTTP request and receiving/interpreting the response. There are two ways for sending an HTTP request. The GET method which sends the request parameters within the URL, and the POST method which sends the parameters separately. POST is able to send more data and is even capable of uploading entire files. After initializing `ClassEConnect`, a request can be sent immediately without the need to connect to a host.

Here is an example on how to send an HTTP request through a simple GET and POST method, the parameters are parsed automatically:

_Code (5 lines)_
```c++
if (ecd.net[0]->Init(ECONN_SOCKET, this) == E_OK)
{
    ecd.net[0]->SendHTTPGet("www.test.com/testpage.php?ac=info");
    ecd.net[0]->SendHTTPPost("www.test.com/testpage.php?ac=info");
}
```

## Using the HTTP request class
When a post request is a bit more complicated, the above method will be tedious. To solve this, there is an HTTP request class containing information about the request and a number of parameters. The following example will send the same GET and POST request using the request class:

_Code (7 lines)_
```c++
if (ecd.net[0]->Init(ECONN_SOCKET, this) == E_OK)
{
    EHTTPRequest request("www.test.com/testpage.php");
    request.AddParam("ac", "info");
    ecd.net[0]->SendHTTPGet(&request);
    ecd.net[0]->SendHTTPPost(&request);
}
```

## Sending files through a POST request
It's possible to send large amounts of data and files through a POST multipart request. This will be done through the `SendHTTPPostMultipart` method and takes the HTTP request class as a parameter. Add files or data to this request class before sending the request. The following example adds a file to the request class and sends it to a webserver through the POST multipart method:

_Code (6 lines)_
```c++
if (ecd.net[0]->Init(ECONN_SOCKET, this) == E_OK)
{
    EHTTPRequest request("www.test.com/testpage.php");
    request.AddFile("file1", "C:\\post.txt");
    ecd.net[0]->SendHTTPPostMultipart(&request);
}
```

## Using HTTP stacks
When receiving a response from the webserver, it will be a good idea to use an HTTP stack for interpreting the response header and receiving the response body. First allocate the stack and send the request, then fill the stack with all data received from the webserver. When the connection closes, the response header and body can be retrieved from the stack. Here is an example on how to use the HTTP stack:

_Code (2 lines)_
```c++
private:
    EHTTPStack stack;
```

_Code (6 lines)_
```c++
if (ecd.net[0]->Init(ECONN_SOCKET, this) == E_OK)
{
    EHTTPRequest request("www.test.com/testpage.php?ac=info");
    stack.Alloc(32768);
    ecd.net[0]->SendHTTPGet(&request);
}
```

_Code (28 lines)_
```c++
void ClassMain::OnNetMessage(unsigned char connection, long id, long msg, long bufsize, unsigned char *buffer)
{
    EHTTPRESPONSE_INFO info;
    switch(msg)
    {
        case ENET_RECEIVEDATA:
            stack.Push(buffer, bufsize);
            break;
        case ENET_CLOSEDSOCKET:
            if (stack.GetResponseInfo(&info))
            {
                if (stack.BodySize() > 0)
                {
                    unsigned char *bodydata = NULL;
                    unsigned long bodydatasize;
                    bodydata = (unsigned char *)ClassEMemory::Alloc(stack.BodySize());
                    if (bodydata != NULL)
                    {
                        stack.GetBody(bodydata, bodydatasize);
                        //read and react on body data
                        ClassEMemory::DeAlloc(bodydata);
                    }
                }
            }
            stack.Clear();
            break;
    }
}
```

## Chapters
* [Connections and protocols](tutorials_network_protocols.md)
* [Sending and receiving data](tutorials_network_data.md)
* **Using HTTP requests**

