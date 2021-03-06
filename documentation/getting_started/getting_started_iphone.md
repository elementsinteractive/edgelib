:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Getting started with Apple iOS

This tutorial will get you started to develop for Apple iOS devices, including iPhone, iPod touch and iPad. It contains the following chapters:

* [Required hardware and software](gettingstarted_mac_hardsoftware.md)
* [Creating an EDGELIB project](gettingstarted_mac_project.md)

## Important
When installing applications on the iPhone device, it automatically puts your files in a read-only folder. A read-write folder is available to create new files such as the application configuration. This will require you to manage two paths, instead of only one path used on most other systems. When accessing files, it uses the read-write folder automatically. To read files from the read-only folder, an additional parameter is needed in the EDGELIB function calls. For example, in `ClassEFile::Open` specify the `EFOF_READONLYPATH` flag and in CreateSurface, use the `EST_READONLYPATH` flag.

When using PNG images in your iPhone application, graphics quality can be greatly decreased. This is because the device converts the file while installing your application. You can workaround this behavior by using EDGELIB packages (.epk files) or rename the files to use a different extension.

## Download tutorial project
Here is a [sample that can be downloaded](files/helloworld_iphone.zip) which contains the result of this tutorial. Remember to set the build target to simulator before building.

## Getting started tutorials
* [EDGELIB installation](tutorials_gettingstarted_edgeinstallation.md)
* **Getting started with Apple iOS**
* [Getting started with Google Android](gettingstarted_android.md)
* [Getting started with Symbian](gettingstarted_symbian.md)
* [Getting started with Windows Mobile](gettingstarted_windowsmobile.md)
* [Getting started with Maemo 5](gettingstarted_maemo5.md)
* [Getting started with Antix Game Player](gettingstarted_antix.md)
* [Getting started with Windows desktop](gettingstarted_desktop.md)
* [Getting started with Linux desktop](gettingstarted_linux.md)
* [Getting started with Mac OS X](gettingstarted_macosx.md)
* [Getting started with GP2X](gettingstarted_gp2x.md)
* [Getting started with Gizmondo](gettingstarted_gizmondo.md)
* [Multi-platform considerations](gettingstarted_multiplatform.md)

