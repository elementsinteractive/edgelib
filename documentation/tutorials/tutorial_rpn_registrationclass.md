:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Using the EDGELIB registration class

## Retrieving device data
The EDGELIB registration class contains a set of functions for retrieving device data. A common device ID for Windows Mobile is the device owner name. Registration keys for Symbian are based on the IMEI code of the phone. These ID's are unique and prevent piracy (an unlock key for one device, usually doesn't work on another). There is also a function available to convert a device ID into a non-recognizable number. When no device ID is available, this converted number will be random.

Here is an example on how to read a device ID, and how to convert it to a number:

_Code (5 lines)_
```c++
ClassERegister reg;
char regid[128];
char encryptid[128];
reg.GetRegistrationId(regid);
reg.EncryptId(encryptid, regid);
```

## Calculating RPN with EDGELIB
After obtaining a device ID, the register class is able to calculate an unlock key through RPN. If everything went correctly, this unlock key should be the same as if it were calculated through a reseller website. Here is an example how to calculate an unlock key using RPN:

_Code (3 lines)_
```c++
ClassERegister reg;
unsigned short key;
key = reg.CalculateKey("i c + key + 10 *", "Dev");
```

## Important note
Passing an RPN string as an argument to a function directly (i.e. `Calculate("i c + key + 10 *")`) is not recommended, because this string will be compiled directly into the program executable (anyone can view this file and search for the RPN string). Because this RPN string is essential for calculating unlock keys, it will be a bad idea to let others rip this string. Defining this string with `#define RPN_STRING "i c + key + 10 *"` and passing it a function doesn't solve this problem either (because compiler just replaces this definition before compiling).

A good way to solve this problem is to generate this string dynamically within the program. One method is to cut the string into multiple pieces, and concatenate it into one string, then passing it to the RPN calculator. An even better method is to write a function to encrypt and decrypt these strings into non-recognizable data.

## Chapters
* [RPN Explained](tutorial_rpn_explained.md)
* **Using the EDGELIB registration class**

