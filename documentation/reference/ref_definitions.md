:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Global definitions

## General
```c++
MAX_ESTR       //Amount of characters to reserve for generic strings 
MAX_EFILESTR   //Amount of characters to reserve for filenames 
MAX_CMDLINE    //Amount of characters to reserve for the commandline  
```

General purpose definitions which can be used throughout the application.


```c++
EPOS_TOP 
EPOS_BOTTOM 
EPOS_LEFT 
EPOS_RIGHT
```

Position definitions. These are used to indicate the position of the softkey labels for example.

## EDGELIB information
```c++
E_ENGINENAME   //The name of the engine, "EDGELIB" 
E_VERSION      //The current version of the engine (string) 
E_VERSION_NR   //The current version of the engine (value)  
```

These definitions contain the name of the engine and its version within a string, the current version of EDGELIB. The numeric value uses a specific format. For example, EDGELIB 3.10 is defined as E_VERSION_NR = 310.

## Device operating systems
```c++
DEVICE_WIN32           //All Windows based devices, desktop and mobile 
DEVICE_WINDOWS         //Windows 9x, NT, 2000, XP and Vista 
DEVICE_WINCE           //Windows CE based devices 
DEVICE_WINMOBILE       //Windows Mobile based devices 
DEVICE_WINMOBILE_NG    //Windows Mobile devices from 2003 and onwards 
DEVICE_SYMBIAN         //Symbian based devices 
DEVICE_LINUX           //Linux based devices 
DEVICE_LINUXEMBEDDED   //Linux based mobile devices 
DEVICE_MAC             //Mac devices from Apple 
DEVICE_IPHONE          //The iPhone and iPod devices 
DEVICE_IPHONEEMU       //The iPhone simulator 
DEVICE_ANTIX           //The Antix Game Player  
```

These define the target operating system, use them to write OS specific code. DEVICE_SYMBIAN and DEVICE_WINMOBILE contain a version number. This version number is formatted, for example if DEVICE_SYMBIAN is 910 it is Symbian 9.10.

## Devices models
```c++
DEVICE_DESKTOP      //Desktop based devices 
DEVICE_POCKETPC     //Windows Mobile Pocket PC 
DEVICE_SMARTPHONE   //Windows Mobile Smartphone 
DEVICE_SERIES60     //Symbian Series60/S60 Smartphone 
DEVICE_SERIES80     //Symbian Series80 Communicator 
DEVICE_SERIES90     //Symbian Series90 (Nokia 7710) 
DEVICE_S60          //Symbian Series60/S60 Smartphone 
DEVICE_S80          //Symbian Series80 Communicator 
DEVICE_S90          //Symbian Series90 (Nokia 7710) 
DEVICE_UIQ          //Symbian UIQ Smartphone 
DEVICE_NOKIA7710    //Symbian Series90 (Nokia 7710) 
DEVICE_GP2X         //The GP2X personal entertainment device 
DEVICE_MAEMO          //The Maemo platform 
DEVICE_MAEMOSIM       //The Maemo simulator 
DEVICE_ANDROID        //Android  
```

These define the target device. Use it to write platform specific code.

## Devices capabilities
```c++
DEVICE_DIRECTX        //Devices containing the DirectX APIs 
DEVICE_X11             //Systems that run on an X server  
```

These define the capabilities of the target device.

## Native type
```c++
ENATIVETYPE //A generic data type  
```

This data type is used for any data type that is defined by a specific device SDK (like window handles). Use typecasting to convert these.

## Datatypes
```c++
ETYPE_TIMESTAMP //A date/time value, the amount of seconds passed since 1 januari 1970 
ETYPE_MONEY     //A data type used for money transactions: amount * 100 + cents  
```

Custom datatypes defined by EDGELIB.

## Other definitions
```c++
EDT_TIMESTAMPNOW  //The current time stamp 
EDGE_SAFE_CONTEXT   // boolean indicating if an OpenGL context may be lost  
```

Miscellaneous global definitions.

## Error return values
```c++
E_OK 
E_ERROR 
E_NOMEMORY 
E_UNSUPPORTED 
E_INVALIDPARAM 
E_NOENTRY 
E_TOOBIG 
E_NOTEXISTS 
E_NODATA 
E_ALREADYFOUND 
E_NOACTION 
E_TYPEMISMATCH 
E_LOCKFAILED 
E_NOTCONNECTED 
E_NOFEATURE
```

With these error codes the developer is able to know the reason of a function error. E_NOFEATURE is returned if the functionality is not present in the current version and edition of the EDGELIB SDK. An edition upgrade may be needed.

## Version changes
- EDGELIB 4.00: Added DEVICE_MAEMO, DEVICE_MAEMOSIM, DEVICE_ANDROID, DEVICE_X11, DEVICE_ANTIX and EDGE_SAFE_CONTEXT 
- EDGELIB 3.90: Added DEVICE_MAC, DEVICE_IPHONE and DEVICE_IPHONEEMU 
- EDGELIB 3.30: Added position definitions 
- EDGELIB 3.10: Added `E_VERSION_NR` and `ENATIVETYPE` 
- EDGELIB 3.00: Added `E_NOFEATURE` 
- EDGELIB 2.70: Added `ETYPE_MONEY`, renamed `E_TIMESTAMP` to `ETYPE_TIMESTAMP`, renamed `TIMESTAMP_NOW` to `EDT_TIMESTAMPNOW` 
- EDGELIB 2.60: Added `E_TIMESTAMP, TIMESTAMP_NOW` 
- EDGELIB 2.40: Added `E_NOTCONNECTED` 
- EDGELIB 2.20: Added a new definition for DirectX 
- EDGELIB 2.10: Added new definitions for device types, changed `DEVICE_WM2003` to `DEVICE_WINMOBILE_NG`. Added new error return values and general definitions.

