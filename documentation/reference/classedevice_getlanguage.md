:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# GetLanguage


```c++
char *GetLanguage(char *language)
```

## Parameters
### *language (out)
This is a pointer to a string that holds the language of the device.

## Return value
The language, the same as the parameter *language.

## Description
Get the current set language of the device. Currently only works on iPhone and iPod. 
The string return takes the form of a two-character language code, optionally followed by a dash and a two-character country code when a regional variant of the language is used. E.g.:

* **en-gb** - British English
* **en-us** - American English
* **ja** - Japanese
* **fr** - French
* **fr-ca** - Canadian French
* **nl** - Dutch

## Version changes
- EDGELIB 3.97: Added this function to the SDK

