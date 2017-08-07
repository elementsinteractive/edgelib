:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Definitions

## Battery statuses
```c++
BATT_ERROR          // Error while trying to read the status 
BATT_UNSUPPORTED    // Reading the battery is not supported 
BATT_NOTFOUND       // There was no battery found in the current device 
BATT_CHARGING       // The battery is currently charging 
BATT_BATTERYPOWERED // The device is running on battery power 
BATT_ACPOWERED      // The battery is full and the device is externally powered  
```

The status of the device internal battery.

## Version changes
- EDGELIB 2.50: Added battery status definitions

