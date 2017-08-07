:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Symbian security

## Symbian Signed
To prevent malicious software from spreading, Symbian has introduced [Symbian Signed](http://www.symbiansigned.com). This is a procedure for certifying applications to make sure the application can be trusted. By default, most Symbian devices prevent an unsigned application from installing. Usually this can be turned off by changing a setting in the control panel. For devices running Symbian 9+ signing is mandatory, but most applications can use a self-signed certificate. However, if the device only allows signed applications to install, a self-signed certificate is still refused by the installer.

To get your application Symbian Signed it needs to be sent to a testing house which will run a number of basic tests on the application to ensure it's stable, behaves as expected and doesn't contain virusses or any other programming code which might damage the device. Submitting an application for testing requires a certain fee. If the test fails, it needs to be resubmitted. To reduce these costs, it is recommended to test the application extensively before submitting it to a testing house. A document describing the test criteria can be found on the Symbian Signed website. Before an application can be submitted for testing, the developer needs a publisher ID, which is a unique number identifying the developer. This is used for creating a certificate for signing the file before it's tested. This can be requested for a fee, and needs to be renewed every year.

When an application is in development, it doesn't need to be signed after each internal build. It can be signed with a self-signed certificate, or a developer certificate. A developer certificate only works on one device, but grants more permissions. A developer certificate can be requested through Symbian Signed.

## Capabilities
Since Symbian 9, an application can set certain capabilities. For a Bluetooth enabled multi-player game, it needs to set the LocalServices capability. If this capability isn't defined, the application will still run, but connection requests through Bluetooth will be denied. When a self-signed application is installed on a device which enables all installations, it will list the capabilities an application will use. Only the basic set of capabilities can be used with a self-signed certificate, other capabilities require signing from Symbian Signed or signing from a phone manufacturer.

Here is a list of capabilities:

### Basic capabilities
* LocalServices
* UserEnvironment
* NetworkServices
* Location
* ReadUserData
* WriteUserData

### Extended capabilities
* ReadDeviceData
* WriteDeviceData
* SWEvent
* ProtSrv
* SurroundingsDD
* TrustedUI
* PowerMgmt

### Phone manufacturer approved
* DRM
* MultimediaDD
* Network Control
* TCB
* AllFiles
* CommDD
* DiskAdmin

To develop and test an application with the more advanced capabilities, you can use a developer certificate.

## Chapters
* [Installation](gettingstarted_symbian_installation.md)
* [Commandline project](gettingstarted_symbian_cmdproject.md)
* [The C++ source file](gettingstarted_symbian_sourcefile.md)
* [Symbian installations](gettingstarted_symbian_sisfiles.md)
* **Symbian security**
* [Application UID](gettingstarted_symbian_appuid.md)

