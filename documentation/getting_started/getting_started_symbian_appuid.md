:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Application UID

## What is a UID?
A UID is a unique application identification number used for identifying applications. It is a 32-bit number which is used for maintaining application installations, security checking and tells the ownership of certain files. Usually, an application uses a developer UID during development and receives its final UID when it is released. Request a UID through [Symbian Signed](http://www.symbiansigned.com).

## Development UID's
For devices running before Symbian 9, it can use a UID from 0x01000000 to 0x0FFFFFFF. From Symbian 9 and onwards, these range from 0xE0000000 to 0xEFFFFFFF. If an application for Symbian 8 is in development, and the developer likes to test it for Symbian 9 devices as well, the first number after the 'x' can be changed from 0 to E.

## UID ranges
From Symbian 9, the list of UID's is separated into several categories. UID's from 0x00000000 to 0x6FFFFFFF are for protected applications where signing through Symbian Signed is mandatory.

Here is a list of categories:

* 0x00000000 - 0x0FFFFFFF: Development use only
* 0x10000000 - 0x1FFFFFFF: Legacy UID allocations
* 0x20000000 - 0x2FFFFFFF: V9 protected UID allocations
* 0x30000000 - 0x6FFFFFFF: Reserved
* 0x70000000 - 0x7FFFFFFF: Vendor ID's
* 0x80000000 - 0x9FFFFFFF: Reserved
* 0xA0000000 - 0xAFFFFFFF: V9 unprotected UID allocations
* 0xB0000000 - 0xDFFFFFFF: Reserved
* 0xE0000000 - 0xEFFFFFFF: Development use only
* 0xF0000000 - 0xFFFFFFFF: Legacy UID compatibility range

Legacy UID's are used for applications built for devices before Symbian 9. When building for Symbian 9, change the UID. For example, allocated UID 0x10205D9D for Symbian 6.1 will be 0xF0205D9D for Symbian 9 and development UID 0x00205D9D for Symbian 6.1 will be 0xE0205D9D for Symbian 9. Unprotected UID 0xA0205D9D can be changed to 0x10205D9D for builds targetting devices before Symbian 9.

## Chapters
* [Installation](gettingstarted_symbian_installation.md)
* [Commandline project](gettingstarted_symbian_cmdproject.md)
* [The C++ source file](gettingstarted_symbian_sourcefile.md)
* [Symbian installations](gettingstarted_symbian_sisfiles.md)
* [Symbian security](gettingstarted_symbian_security.md)
* **Application UID**

