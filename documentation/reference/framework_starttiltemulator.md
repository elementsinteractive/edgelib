:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# StartTiltEmulator


```c++
void StartTiltEmulator(short centeranglex = 0, short centerangley = 675, bool emulatestylus = true, unsigned long stylussensitivity = 7000, bool emulatedpad = false, unsigned long dpadsensitivity = 3000)
```

## Parameters
### centeranglex, centerangley (in)
The angle to use as a center tilt position (default 0, 675).

### emulatestylus (in)
Enable or disable stylus emulation (default enabled).

### stylussensitivity (in)
The amount of sensitivity when emulating the stylus (default 7000).

### emulatedpad (in)
Enable or disable directional pad emulation (default disabled).

### dpadsensitivity (in)
The directional pad sensitivity, the amount of tilt before the key will be emulated (default 3000).

## Return value
None.

## Description
Enable or disable the TiltCONTROL emulator. This will emulate events from the tilt device as stylus events or directional pad events. Stylus emulation is enabled by default.

## Version changes
- EDGELIB 2.40: Added this function to the SDK

