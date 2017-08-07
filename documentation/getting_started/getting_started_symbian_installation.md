:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Installation

## Required tools and SDKs
For Symbian there are several target platforms, each needing their own SDK. Available Symbian devices may run one of the following Symbian platforms: S60 (previously known as Series 60), Series 80, Series 90 or UIQ. There is only one device available running Series 90 which is the Nokia 7710. These SDKs will install a compiler and several tools. These tools require [ActivePerl](http://www.activestate.com/Products/ActivePerl/?mp=1), but not all SDKs include this in the installation.

Devices running Symbian are mostly backwards compatible. For example, a device running Series 60 second edition is able to run an application built for Series 60 first edition. The exception to this rule are the recently released Symbian 9.1 devices running S60 third edition or UIQ 3.0. This new Symbian version has introduced many changes which require a separate build. When installing multiple SDKs of the same Symbian platform, it's recommended to install in the following order: from oldest to newest. It may also be required to download an additional SDK, for example the Series 60 2nd edition FP3 SDK for creating a separate build supporting high resolution screens. Note that not all available SDKs have a matching build of EDGELIB. Check the list below for the recommended SDKs to install.

There are several methods for building Symbian applications. These include using an IDE such as Microsoft Visual Studio or MetroWerks CodeWarrior, building through the commandline or using the EDGELIB Builder (only available for some Symbian SDKs at the moment). When building for the more recent SDKs, you can also use the [Carbide](http://www.forum.nokia.com/main/resources/tools_and_sdks/carbide/index.html) toolkit.

* [Series 60 SDK download center](http://www.forum.nokia.com/info/sw.nokia.com/id/4a7149a5-95a5-4726-913a-3c6f21eb65a5/S60-SDK-0616-3.0-mr.html)
* [Series 80 SDK download center](http://www.forum.nokia.com/info/sw.nokia.com/id/9fe9cd82-c8a2-4597-a426-3871b7f92da1/Series_80_Platform_2_0_SDK_for_Symbian_OS.html)
* [Nokia 7710 SDK download center (Series 90)](http://www.forum.nokia.com/info/sw.nokia.com/id/6d77aa41-4c0d-41eb-b43f-a716cf2eb508.html)
* [UIQ SDK download center](http://developer.sonyericsson.com/site/global/docstools/symbian/p_symbian.jsp)
* [Series 60 SDK downloads from NewLC](http://www.newlc.com/Nokia-Series-60-SDK.html)

## Installing SDKs
We recommend downloading and installing the following SDKs to support a broad range of Symbian devices:

* Series 60 first edition supporting CodeWarrior
* S60 third edition
* Series 80 second edition
* Nokia 7710 SDK supporting CodeWarrior
* UIQ 2.1 SDK
* UIQ 3.0 SDK

A Symbian SDK needs to be installed on the same disk where the projects can be found. An SDK usually installs itself in the folder `C:\symbian`. Unfortunately, the Series 60 first edition SDK supporting CodeWarrior isn't available anymore from forum nokia. However, it can still be downloaded from NewLC.

After the SDKs have been installed, copy the EDGELIB library files from `c:\projects\edge\lib` into the library folder of each SDK. When using the EDGELIB Builder, this doesn't need to be done for EDGELIB Builder supported SDKs.

`\epoc32\release\armi\urel` (for most SDKs) 
`\epoc32\release\armv5\urel` (for the S60 3rd edition and UIQ 3.0 SDKs, Symbian 9.1) 
`\epoc32\release\winscw\udeb` (for emulator builds)

## Chapters
* **Installation**
* [Commandline project](gettingstarted_symbian_cmdproject.md)
* [The C++ source file](gettingstarted_symbian_sourcefile.md)
* [Symbian installations](gettingstarted_symbian_sisfiles.md)
* [Symbian security](gettingstarted_symbian_security.md)
* [Application UID](gettingstarted_symbian_appuid.md)

