:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Symbian installations

## Package files
To install applications on a Symbian device, we need to create a .sis Symbian installation file. This file is usually sent to the device through Bluetooth, and will be installed on the device itself. These files can be created with the `makesis` utility which is part of every Symbian SDK. This utility needs a package file containing installation information and the files that need to be installed. Take a look at this template file for SDKs targetting a Symbian before version 9:

_helloworld.pkg (5 lines)_
```c++
#{"Hello World"},(0x10205D9D),1,0,0 
(0x101F6F88), 0, 0, 0, {"Series60ProductID"} 
"\Symbian\6.1\Series60\Epoc32\Data\z\system\apps\helloworld\helloworld.rsc"-"!:\system\apps\helloworld\helloworld.rsc" 
"\Symbian\6.1\Series60\epoc32\data\z\system\apps\helloworld\helloworld.aif"-"!:\system\apps\helloworld\helloworld.aif"
"\Symbian\6.1\Series60\Epoc32\Release\armi\urel\helloworld.app"-"!:\system\apps\helloworld\helloworld.app"
```

The first line contains the name of the application, its UID and version information in the following order: major, minor, build number. The second line contains the UID of the target platform and the lines after that indicate which files should be included and where they should be installed. These are the platform UID's you can use (for Series 80 first edition, this line should be removed):

`0x101F6F88` (S60) 
`0x101F7961` (S60 3rd edition) 
`0x101F8ED2` (S80 second edition) 
`0x101FBE05` (Nokia 7710) 
`0x101F617B` (UIQ 2.0) 
`0x101F6300` (UIQ 3.0)

When targetting Symbian 9+ you can use this template file:

_helloworld.pkg (13 lines)_
```c++
&EN
 
#{"helloworld"},(0xF0205D9D),1,0,0
 
%{"Vendor-EN"}
:"Vendor"
 
[0x101F7961], 0, 0, 0, {"S60ProductID"}
 
"\Symbian\9.1\S60_3rd\epoc32\release\gcce\urel\helloworld.exe"-"!:\sys\bin\helloworld.exe"
"\Symbian\9.1\S60_3rd\Epoc32\Data\z\resource\apps\helloworld.rsc"-"!:\resource\apps\helloworld.rsc"
"\Symbian\9.1\S60_3rd\Epoc32\Data\z\private\10003a3f\import\apps\helloworld_reg.rsc"-"!:\private\10003a3f\import\apps\helloworld_reg.rsc"
"helloworld_aif.mbm"-"!:\resource\apps\helloworld_aif.mbm"
```

When the application doesn't use icons, the helloworld.aif or helloworld_aif.mbm files can be removed from the file list.

## Signing installation files
The installation files can be signed with a developer, self-signed or publisher certificate. More information about Symbian Signed can be read in the [security chapter](getting_started_symbian_security.md). A self-signed certificate can be created with the following command:

```c++
makekeys -cert -password 12345 -len 2048 -dname "CN=name OU=department OR=company CO=US EM=address@host.com" test.key test.cer
```

To sign the installation file use the following command:


```c++
signsis helloworld.sis helloworld_signed.sis test.cer test.key 12345
```

## Chapters
* [Installation](getting_started_symbian_installation.md)
* [Commandline project](getting_started_symbian_cmdproject.md)
* [The C++ source file](getting_started_symbian_sourcefile.md)
* **Symbian installations**
* [Symbian security](getting_started_symbian_security.md)
* [Application UID](getting_started_symbian_appuid.md)

