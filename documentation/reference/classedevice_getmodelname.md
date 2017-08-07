:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetModelName


```c++
void static GetModelName(char *modelname) 
void static GetModelName(WCHAR *modelname)
```

## Parameters
### *modelname (out)
A string that contains the device model name.

## Return value
None.

## Description
This function can be used to get a device model name. Use it to write code for specific devices.

## Example
_Code (2 lines)_
```c++
char modelname[MAX_ESTR]; 
ClassEDevice::GetModelName(modelname);
```

**Symbian Series 60/S60** 
`0x10005e33`: Nokia 92xx 
`0x10005f62`: Symbian emulator 
`0x101f3ee3`: Nokia 6620 
`0x101f408b`: Sony-Ericsson P800 
`0x101f466a`: Nokia 3650/3660 
`0x101f4fc3`: Nokia 7650 
`0x101f8c19`: Nokia N-Gage 
`0x101f8ddb`: Nokia 9500 
`0x101f9071`: Siemens SX1 
`0x101fa031`: Sendo X 
`0x101fb2b1`: Nokia N-Gage QD 
`0x101fb3dd`: Nokia 6600 
`0x101fb3f3`: Nokia 7610 
`0x101fb3f4`: Nokia 6260 
`0x101fbb55`: Nokia 6630 
`0x101fbe09`: Nokia 7710 
`0x10200f97`: Nokia 3230 
`0x10200f98`: Nokia N90 
`0x10200f99`: Nokia 6680 
`0x10200f9a`: Nokia N70 
`0x10200f9b`: Nokia 6682 
`0x10200f9c`: Nokia 6681 
`0x200005f8`: Nokia 3250 
`0x200005f9`: Nokia N80 
`0x200005fa`: Nokia N92 
`0x200005fb`: Nokia N73 
`0x200005fc`: Nokia N91 
`0x200005fe`: Nokia N75 
`0x200005ff`: Nokia N71 
`0x20000600`: Nokia N93 
`0x20000602`: Nokia 5500 
`0x20000604`: Nokia E65 
`0x20000605`: Nokia N93i 
`0x20000606`: Nokia 6290 
`0x2000060b`: Nokia N95 
`0x20001856`: Nokia E60 
`0x20001857`: Nokia E70 
`0x20001858`: Nokia E61 
`0x20002495`: Nokia E50 
`0x20002496`: Nokia E90 
`0x20002498`: Nokia E51 
`0x20002d7b`: Nokia 6110 Navigator 
`0x20002d7c`: Nokia 5700 
`0x20002d7e`: Nokia 6120 Classic 
`0x20002d7f`: Nokia E61i 
`0x20002d83`: Nokia N81 
`0x20002d84`: Nokia N95 8GB 
`0x20002d85`: Nokia N82 
 
**Symbian UIQ** 
`0x101f6b26`: Motorola A9xx 
`0x101fb2ae`: Sony-Ericsson P900 
`0x10200ac6`: Sony-Ericsson P910 
`0x1020e285`: Sony-Ericsson P990i 
`0x1027400d`: Motorola RIZR Z8 
`0x10274bf9`: Sony-Ericsson M600i 
`0x10274bfa`: Sony-Ericsson W950i 
`0x20002e69`: Sony-Ericsson P1i 
`0x20002e6a`: Sony-Ericsson W960i

## Version changes
EDGELIB 3.987: On iPhone, the hardware revision is now included in the returned string. 
- EDGELIB 2.80: Removed the modelid parameter, it isn't required anymore. Added more Symbian model names. Added Unicode overloaded function.

