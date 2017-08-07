# EDGELIB SDK
EDGELIB is a [feature-rich framework](documentation/features/README.md) written in C++ and a set of build tools to ease cross-platform development of games and applications.

[Elements Interactive](https://www.elements.nl) maintained EDGELIB actively between 2004 and 2012 and was used in many great mobile games, including Syberia by Tetraedge Games, Rough Cookie's acclaimed Star Defense and Elements' own mobile games.

The SDK is mainly targeted at mobile and handheld platforms, but also allows desktop builds on Linux, Mac OS X and Windows for quick testing. Supported mobile platforms include Windows Mobile/CE, Symbian, iOS, Android, Maemo 5, and various others.

Enjoy! And please let us know at _edgelib [at] elements [dot] nl_ if you find EDGELIB useful!

# License
While Elements Interactive does not maintain, support or commercialize EDGELIB anymore, the code is now provided to the open source community under the [3-clause BSD license](LICENSE).

# Getting started
With the majority of platform SDKs being Windows-only in 2012, the tools assume a Windows environment, but components can be built from the host for which individual SDKs have been targeted.

The [original EDGELIB 4.02 documentation from 2012](documentation/README.md) is bundled with this repository, where you can have a look at the [Getting Started section](documentation/getting_started/README.md) and the [SDK Reference](reference/README.md).

Also the original development project files have been included for convenience. You will still need to manually install the platform SDKs and update the paths. There is a significant chance however that some of the required software or libraries are either no longer available, or not able to run anymore on current configurations.

# Third party libraries
EDGELIB contains the following third-party libraries. Source code of these libraries are included in the [`external`](external) directory.

* The complete Small JPEG Decoder Library v0.93b distribution is included in the EDGELIB SDK and can be found in the directory [`external/jpgd093b`](external/jpgd093b).
* The complete MD5-CC v1.02 distribution is included in the EDGELIB SDK and can be found in the directory [`external/md5-cc-102`](external/md5-cc-102).

## Small JPEG Decoder Library v0.93b
This software contains code based on the Small JPEG Decoder Library v0.93b by Rich Geldreich licensed under LGPL. Copyright (C) 1994-2000 Rich Geldreich.

Portions of this software are based on the work of the Independent JPEG Group. Parts are copyright (C) 1991-1998, Thomas G. Lane.

## MD5-CC v1.02
This software contains a C++/object oriented translation and modification of MD5 (version 1.02) by Mordechai T. Abzug.
Translation and modification (c) 1995 by Mordechai T. Abzug

Copyright 1991-1992 RSA Data Security, Inc.
The MD5 algorithm is defined in RFC 1321. This implementation is derived from the reference C code in RFC 1321 which is covered by the following copyright statement:

Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All rights reserved.
License to copy and use this software is granted provided that it is identified as the ``RSA Data Security, Inc. MD5 Message-Digest Algorithm'' in all material mentioning or referencing this software or this function.

License is also granted to make and use derivative works provided that such works are identified as "derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm'' in all material mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either the merchantability of this software or the suitability of this software for any particular purpose. It is provided ``as is'' without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this documentation and/or software.
