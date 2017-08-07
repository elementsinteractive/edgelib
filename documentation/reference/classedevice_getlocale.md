:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetLocale


```c++
char *GetLocale(char *locale)
```

## Parameters
### *locale (out)
This is a pointer to a string that holds the locale of the device.

## Return value
The locale, the same as the parameter *locale.

## Description
Get the current locale setting of the device. Currently only works on iPhone and iPod.

## Version changes
- EDGELIB 3.97: Added this function to the SDK

