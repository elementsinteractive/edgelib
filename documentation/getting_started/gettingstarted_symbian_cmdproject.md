:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Commandline project

## Creating workspace folders
When building through the commandline, a project workspace will contain a build file, a project file, resource files and a batchfile to ease the building process. Create a workspace folder for each Symbian platform:

`C:\projects\helloworld\workspace_s60` 
`C:\projects\helloworld\workspace_s80` 
`C:\projects\helloworld\workspace_s90` 
`C:\projects\helloworld\workspace_uiq`

## The build file
The build file contain a reference to the project file, and the supported target architectures (such as a device build, or an emulator build). Add bld.inf to each workspace folder:

_bld.inf (6 lines)_
```c++
PRJ_MMPFILES
helloworld.mmp
 
PRJ_PLATFORMS
GCCE
WINSCW
```

The PRJ_PLATFORMS section only needs to be added for the Symbian 9+ SDKs, such as S60 third edition and UIQ 3.0. The following project platforms can be used: GCCE, WINS and/or WINSCW.

## The project file
The project file contains general build information such as the name of the application and unique identification number. It contains source files, header files, resource files, libraries and additional include folders for headers and libraries. It can also contain additional information such as vendor information and capabilities. Add helloworld.mmp to each workspace folder:

_helloworld.mmp (4 lines)_
```c++
TARGET helloworld.app
TARGETTYPE app
TARGETPATH \system\apps\helloworld
UID 0x100039CE 0x10205D9D
```

Since Symbian 9, the target type has been changed from .app to .exe. When building with these SDKs, change `helloworld.app` into `helloworld.exe`. Change TARGETTYPE to exe as well. Also remove targetpath and add `EPOCSTACKSIZE 0x8000`.

The UID is a unique number to identify your application. The first UID (0x100039CE) is a generic Symbian UID and will always be the same. The second UID (0x10205D9D) will be different for each application. In this sample we use the generic EDGELIB UID. For more information about UID's, check the [application UID chapter](getting_started_symbian_appuid.md). For Symbian 9, also change the UID from 0x10205D9D into 0xF0205D9D.

Now append helloworld.mmp with a macro definition to pass to the compiler. It is needed to identify the Symbian target platform. Add `MACRO xxx` where xxx will be one of the following: SERIES60, SERIES80, SERIES90 or SYMBIANUIQ. Also specify the application UID as a macro. Add `MACRO APP_UID=0x10205D9D` or `MACRO APP_UID=0xF0205D9D` depending on the Symbian SDK.

Finally add the c++ source files, header paths and libraries to the project file:

_helloworld.mmp (32 lines)_
```c++
SOURCEPATH . 
SOURCE ..\code\helloworld.cpp
 
USERINCLUDE . 
USERINCLUDE ..\code 
USERINCLUDE ..\..\edge\include 
  
SYSTEMINCLUDE \epoc32\include 
SYSTEMINCLUDE \epoc32\include\libc 
  
STATICLIBRARY edge.lib 
STATICLIBRARY edgerender.lib 
 
LIBRARY euser.lib 
LIBRARY apparc.lib 
LIBRARY cone.lib 
LIBRARY eikcore.lib 
LIBRARY eikcoctl.lib 
LIBRARY ws32.lib 
LIBRARY efsrv.lib 
LIBRARY etel.lib 
LIBRARY apgrfx.lib 
LIBRARY fbscli.lib 
LIBRARY bitgdi.lib 
LIBRARY esock.lib 
LIBRARY insock.lib 
LIBRARY bluetooth.lib 
LIBRARY btextnotifiers.lib 
LIBRARY btmanclient.lib 
LIBRARY sdpagent.lib 
LIBRARY sdpdatabase.lib 
LIBRARY hal.lib
```

A few libraries need to be added depending on the Symbian SDK: 
`STATICLIBRARY zlib.lib` (All SDKs except Symbian 9+) 
`LIBRARY estlib.lib` (All device builds) 
`LIBRARY avkon.lib` (All S60 SDKs) 
`LIBRARY qikctl.lib` (UIQ 2.1) 
`LIBRARY qikcore.lib` (UIQ 3.0) 
`LIBRARY quiconfigclient.lib` (UIQ 3.0) 
`LIBRARY etel3rdparty.lib` (Symbian 8+ SDKs, Series 90) 
`LIBRARY ezlib.lib` (Symbian 9+ SDKs) 
`LIBRARY plpvariant.lib` (S60 except S60 3rd edition, Series 80, UIQ 2.1)

For Symbian 9+ builds you also need to specify a language code, vendor ID and application capabilities. Add the following code to helloworld.mmp:

_helloworld.mmp (4 lines)_
```c++
LANG SC
 
VENDORID      0
CAPABILITY    NONE
```

The language code SC is undefined and will be used by default. When not using a vendor ID, it will be set to 0. Read the [security chapter](getting_started_symbian_security.md) for more information about capabilities. When using capabilities they can be specified as: CAPABILITY LocalServices NetworkServices

## Resources
Each Symbian application requires at least one resource file, otherwise it won't be able to start. For devices before Symbian 9 create this resource file:

_helloworld.rss (10 lines)_
```c++
NAME HEWO
 
#include <eikon.rh>
#include <eikcore.rsg>
 
RESOURCE RSS_SIGNATURE { }
 
RESOURCE TBUF { buf=""; }
 
RESOURCE EIK_APP_INFO{ }
```

For devices with Symbian 9 and later create 2 resource files:

_helloworld.rss (20 lines)_
```c++
NAME HEWO
 
#include <eikon.rh>
#include <appinfo.rh>
 
RESOURCE RSS_SIGNATURE { }
 
RESOURCE TBUF { buf=""; }
 
RESOURCE EIK_APP_INFO{ }
 
RESOURCE LOCALISABLE_APP_INFO exe_localisable_app_info
{
    short_caption = "Helloworld";
    caption_and_icon = CAPTION_AND_ICON_INFO
    {
        caption = "Helloworld";
        number_of_icons = 0;
    };
}
```

_helloworld_reg.rss (14 lines)_
```c++
#include <AppInfo.rh>
#include <helloworld.rsg>
 
UID2 KUidAppRegistrationResourceFile
UID3 0xF0205D9D
 
RESOURCE APP_REGISTRATION_INFO
{
    app_file = "helloworld";
    localisable_resource_file = "\\resource\\apps\\helloworld";
    localisable_resource_id = EXE_LOCALISABLE_APP_INFO;
    embeddability=KAppNotEmbeddable;
    newfile=KAppDoesNotSupportNewFile;
}
```

Resources also need to be added to the helloworld.mmp project file. When targetting Symbian 9+ you need to add the following section:

_helloworld.mmp (12 lines)_
```c++
START RESOURCE helloworld.rss
HEADER
TARGETPATH    \resource\apps
END
 
START RESOURCE helloworld_reg.rss
#ifdef WINSCW
TARGETPATH    \private\10003a3f\apps
#else
TARGETPATH    \private\10003a3f\import\apps
#endif
END
```

For other builds you can add one line of code: `RESOURCE helloworld.rss`.

## Creating icons
Create a new folder `C:\projects\helloworld\res\aif`. This folder contains additional resources for this application. Create two icon bitmap files inside this folder: `icon.bmp` and `iconmask.bmp`. The first one contains the icon itself, the second contains an alpha mask to blend the icon with (white has no opacity, black has full opacity). The recommended icon size depends on the target platform. For Series 60 we recommend an icon of 24x24 pixels, for Series 80 and Series 90 we recommend 25x20 or 64x50 and for UIQ we recommend an icon of 20x20 pixels. Read below on how to attach an icon to your application.

## AIF resource (before Symbian 9)
It's possible to add resource information, such as the program title and an icon. Before Symbian 9 an .aif resource file is used. To create the .aif file include a new line of code at the bottom of `helloworld.mmp`: 
`AIF helloworld.aif ..\res\aif aif.rss c12 icon.bmp iconmask.bmp`

Add this file to the `C:\projects\helloworld\res\aif` folder:

_aif.rss (11 lines)_
```c++
#include <aiftool.rh>
 
RESOURCE AIF_DATA
{
    app_uid = 0x10205D9D;
    caption_list=
    {
        CAPTION { code = ELangOther; caption = "Hello World"; }
    };
    num_icons = 1;
}
```

## Adding icons (Symbian 9+)
For Symbian 9+ expand the following section in helloworld.rss:

_helloworld.rss (10 lines)_
```c++
RESOURCE LOCALISABLE_APP_INFO exe_localisable_app_info
{
    short_caption = "Helloworld";
    caption_and_icon = CAPTION_AND_ICON_INFO
    {
        caption = "Helloworld";
        number_of_icons = 1;
        icon_file = "\\resource\\apps\\helloworld_aif.mbm";
    };
}
```

Then create an .mbm file with the following command: `bmconv helloworld_aif.mbm /c12..\res\aif\icon.bmp /c12..\res\aif\iconmask.bmp`.

## Creating a batchfile
To ease the building process, we will create a batch file. It should contain the following:

_build.bat (10 lines)_
```c++
set OLDPATH=%PATH%
set OLDEPOC=%EPOCROOT%
set PATH=%symbianroot%\6.1\Shared\EPOC32\gcc\bin\;%symbianroot%\6.1\Shared\Epoc32\Tools\;%symbianroot%\6.1\Series60\Epoc32\Tools\;%path%;C:\Program Files\Microsoft Visual Studio\VC98\Bin;C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin
set EPOCROOT=\Symbian\6.1\Series60\
devices -setdefault @Series60_1_2_CW:com.Nokia.Series60_1_2_CW
call bldmake bldfiles
call abld build armi urel
makesis %1.pkg
set PATH=%OLDPATH%
set EPOCROOT=%OLDEPOC%
```

Set path and epocroot is only needed for the Series 60 and Series 80 first edition SDKs. These can be removed for all other SDKs.

Devices is used to change the active Symbian SDK. Get a list with the following command: `devices`.

`call bldmake bldfiles` prepares the building process and `call abld build armi urel` compiles the program. When targetting Symbian 9.1 you need to call `call abld build gcce urel` and for emulator builds call `call abld build winscw udeb`.

Makesis reads the package file and creates helloworld.sis. Read the [Symbian installation chapter](getting_started_symbian_sisfiles.md) for more information.

The batchfile can be executed from the commandline with this command: `call build helloworld`. Make sure that the current folder is the same as the batchfile before executing this command.

## Chapters
* [Installation](getting_started_symbian_installation.md)
* **Commandline project**
* [The C++ source file](getting_started_symbian_sourcefile.md)
* [Symbian installations](getting_started_symbian_sisfiles.md)
* [Symbian security](getting_started_symbian_security.md)
* [Application UID](getting_started_symbian_appuid.md)

