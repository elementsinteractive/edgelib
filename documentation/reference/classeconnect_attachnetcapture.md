:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# AttachNetCapture


```c++
void AttachNetCapture(ClassENetCapture *netcapture)
```

## Parameters
### *netcapture (in)
A class derived from ClassENetCapture.

## Return value
None.

## Description
Attach a net capture derived class to the network module. All messages will be sent to the attached class instead of the EDGELIB framework.

## Example
This example demonstrates how to write a network capture class:

```c++
class CaptureSample : public ClassENetCapture 
{ 
    protected: 
        void OnNetMessage(long id, long msg, long bufsize, unsigned char *buffer); 
    public: 
        CaptureSample(void); 
        ~CaptureSample(void); 
};//A sample network capture class 
 
//Class constructor and destructor 
CaptureSample::CaptureSample(void){ } 
CaptureSample::~CaptureSample(void){ } 
 
//Network message callback 
void CaptureSample::OnNetMessage(long id, long msg, long bufsize, unsigned char *buffer) 
{ 
    //handle messages here 
}
```

## Version changes
- EDGELIB 2.70: Added this function to the SDK

