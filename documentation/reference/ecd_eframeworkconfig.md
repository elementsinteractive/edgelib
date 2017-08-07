:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# EFRAMEWORKCONFIG


```c++
unsigned long ffwdkey 
unsigned long slomokey 
unsigned long maxfps 
unsigned long flags 
unsigned char numconnections 
bool smartfps 
bool focusminimize 
bool jogtiltarrow 
bool focussuspend 
bool backlighton 
bool netlogging 
bool lockkeys 
bool keysounds
```

## Members
### ffwdkey
The code of the button or key that should fast forward program processing. The timedelta parameter in the game loop is influenced when the button is held. When the game processes are based on this parameter, the developer is able to drastically increase development and testing speed. 
Default setting: disabled (0)

### slomokey
The code of the button or key that should decrease program processing speed. The timedelta parameter in the game loop is influenced when the button is held. When the game processes are based on this parameter, the developer is able to analyze animations with more detail. 
Default setting: disabled (0)

### maxfps
This setting can be used to limit the maximum amount of frames per second. Use this to lock the framerate, or set it a reasonable level to conserve the battery life of mobile devices. 
Default setting: 60 FPS

### flags
Extra [flag options](ecd_definitions.md) that should be passed to EDGELIB. 
Default setting: none (0)

### numconnections
The amount of network connections that will be used simultaneously with ClassEConnect. The maximum number of connections is 16. 
Default setting: 1

### smartfps
This enables smart framerate lock managing. It is used to decrease the amount of fluctuations in the framerate and is highly recommended for mobile phone applications. 
Default setting: true

### focusminimize
If the focus is lost, the program will minimize when this option is enabled. It is recommended to turn this on for mobile applications, and to turn it off for desktop applications. 
Default setting: true for mobile devices, false for Windows desktop

### jogtiltarrow
Jog movement is treated as arrow movement for Symbian UIQ devices. 
Default setting: true

### focussuspend
Suspend the application when focus is lost. 
Default setting: true

### backlighton
Keep the backlight on. Supported by Symbian devices and Windows Mobile devices. 
Default setting: true

### netlogging
Enables logging of network events in the EDGELIB console. 
Default setting: false

### lockkeys
Locks keys from the operating system, so they can be used in your EDGELIB application. 
Default setting: true

### keysounds
When set to false, disable keyclick sounds from phone devices. 
Default setting: false

## Version changes
- EDGELIB 3.10: This structure is split from EDGECONFIG, added netlogging, lockkeys and keysounds options 
- EDGELIB 2.95: Added backlighton configuration option

