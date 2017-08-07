:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetTiltData


```c++
ERESULT GetTiltData(ECD *ecd, long *x = NULL, long *y = NULL, long *z = NULL)
```

## Parameters
### *ecd (in)
A pointer to the [ECD](ecd.md) object located in the application framework.

### x, y, z (out)
A pointer to a variable to receive the amount of tilt, ranges between -1024 and 1024.

## Return value
`E_OK`: the tilt device has been connected and data has been received. 
`E_NOTEXISTS`: the tilt device is not found or the driver isn't installed. 
`E_TYPEMISMATCH`: there was an error accessing the device driver. 
`E_NOENTRY`: the tilt device can't use the serial port because it is already in use. 
`E_NOTCONNECTED`: the tilt device is currently not connected.

## Description
This function can be used to poll for the current amount of tilt and the status of the tilt device. When a tilt device is found on Symbian, this function starts returning E_OK after the first call to OnNextFrame.

## Version changes
- EDGELIB 3.90: Added a z-axis parameter to this function 
- EDGELIB 2.70: Moved this function to ClassEInput 
- EDGELIB 2.40: Added this function to the SDK

