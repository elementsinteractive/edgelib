:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Release History

## Version 4.02 (2012-05-31)
* Replaced deprecated iOS calls.

## Version 4.01 (2011-08-25)
* Fixed several issues related to Android building
* Updated EDGELIB Builder to beta 17: added automatic configuration reloading

## Version 4.00 (2011-07-29)
* Added support for Android
* Added support for Maemo 5
* Added support for Antix Game Player
* Added support for Mac OS X desktop
* Added hardware camera support
* Added OpenGL ES 2.0 support
* Added emulation layer to let the backbuffer surface draw to OpenGL
* Added accelerated versions for several primitives which were previously only available with software rendering
* Added stencil buffer support for iPhone, Maemo 5 and Windows desktop
* Added notifications for lost OpenGL contexts
* Fixed a rendering problem in DrawOval
* Fixed DrawOval and FillOval to properly match documentation
* Fixed input handling for iPhone retina display
* Implemented orientation changes with OpenGL
* Fixed the effects of ChangeOrientation on iOS
* Updated EDGELIB Builder to beta 16: select EDGELIB versions on a per-project basis

## Version 3.989 (2012-05-31)
* Replaced deprecated iOS calls.

## Version 3.988 (2010-10-20)
* Fix in Cleanup so that it can be called from inside an networking event

## Version 3.987 (2010-08-20)
* Fix in SetFontDefaultChar so it doesn't take special characters like newline and control characters into account
* Added additional StrReplace overloaded functions
* Added support for iPhone for reading battery status and level
* Added support for iPhone 4 high resolution
* Increased verbosity of GetDeviceName on iOS devices
* Respect maxfps while minimized
* Added support for background operation on Symbian S60 platforms
* Renamed "GetDatasize" to "GetDataSize", added alias for backwards compatibility
* Added option to look in read-only paths for file functions
* Added new random function using the Mersenne twister algorithm
* Improved key release handling in input class
* Fixed rendering issues on iPhone 4
* Updated EDGELIB Builder to beta 15: Increased amount of characters in output window

## Version 3.986 (2010-04-19)
* Bugfix in XML nodes, now it returns the correct value in CountChildNodes and stability is improved
* Faster socket cleanup, improves shutdown of iPhone apps
* Added default font character to display when a character can't be found in the font character set
* Improved const correctness in parameters of several functions
* Removed all "terminate" symbols, including a symbol with this name in the JPEG decoder
* Fixed memory leak in PowerVR textures
* Improved compatibility of Surface::WritePNG

## Version 3.985 (2010-02-02)
* Added functions to get the world and perspective matrices to ClassEDisplay
* Using allkeys alternative to GAPI (to support newer Windows Mobile devices)
* Added support for xv overlays (used if supported by the Linux distribution)
* Improved handling of input release events after resetting state
* Improved const correctness in some functions throughout the EDGELIB API
* Bugfix for rgba surfaces, now they are colored correctly after being rotated by a display change event
* Bugfix in doubleclick event handling for Win32
* Fixed division by zero in ClassEMath::InvTan2
* Fixed bug in XML parser when having empty content in some cases
* Bugfix for iPhone, now the PowerVR loader (pvrtc) can be used properly without linking issues
* Minor bugfix in timezone function for Win32

## Version 3.98 provisional (2009-11-24)
* Hotfix removal of non-public API terminate in iPhone build (used by ClassEdge::Quit)
* Integrated device and simulator library binaries

## Version 3.97 (2009-07-31)
* Improved threading model for iPhone applications making them more stable
* Added PVRTC support through a loader plugin (disabled by default)
* Added support for font kerning, the possibility to specify spacing between certain font characters combinations (including wildcard support)
* Added clipper support to display class, including OpenGL
* Improved filtering, SetShading works with more ClassEDisplay functions for OpenGL and internal rendering
* Add support to get language and locale on iPhone devices
* Improved support for application status events, such as minimize, restore and terminate (for iPhone)
* Fixed XML parser for iPhone, Symbian and Linux platforms
* Fixed right issues on creating folders (iPhone simulator only)
* Fixed ClassEConsole::MsgWindow on iPhone for Unicode strings

## Version 3.96 (2009-03-03)
* Improved support for Symbian S60 devices with a new display mode (including the Nokia 5800)
* Added clear function to ClassEDisplay, works for both OpenGL and non-OpenGL modes
* Added integer concatenation to the EString class
* It's now also possible to pass color filter flags (such as EFX_COLORKEY) to the ClassEDisplay::SetShading function for blitting, for OpenGL only the EFX_COLORIZE, EFX_OPACITY and EFX_ADD work
* Added sound looping to the OpenAL wrapper
* Improved speed of the EDGELIB Packer, now it's back to the original speed again (before version 2.20)
* Improved stability of XML class
* Fixed issues relating to the PNG loader
* Fixed a bug regarding uploading surfaces and using wrapper functions such as BltFast, now it binds the texture correctly
* Fixed a slowdown in the file API reading the size of an open file
* Minor fix in the OpenAL sound wrapper, now it works better when closing down the sound system
* Bugfix for ClassEInput::GetClickPressCount, now it doesn't return a positive value when pressing a key
* Returning a value other than E_OK in the OnNextFrame callback, will quit the application
* Fixed touch bugs for the iPhone and iPod, now it works better when the device is running slower (for example, when loading resources)
* Fixed bug in multitouch implementation on iPhone's running firmware >= 2.2

## Version 3.952 (2008-12-23)
* Another ZLib fix, now it works correctly for all platforms

## Version 3.951 (2008-12-16)
* Fixed linking ZLib libraries for more platforms

## Version 3.95 (2008-12-02)
* Improved Apple iPhone and iPod Touch support
* Added multi touch support for Apple iPhone and iPod Touch
* Added vibration/force feedback, currently only works for Apple iPhone
* Added an OpenAL wrapper for ClassESound, currently only works with sound effects. Using OpenAL is currently recommended for Apple iPhone devices
* Improved device information reading from the Apple iPhone, most functionality from ClassEDevice (like GetOSName and GetSerialNumber) is implemented
* MsgWindow also works on AppleiPhone and iPod Touch devices now
* Improved handling of click event coordinates in the input class
* Bugfix for the ZLib libraries in the beta, now they link properly
* Updated the font maker tool with extra settings to pad characters and make multirow font images

## Version 3.90 beta (2008-10-24)
* Added Apple iPhone and iPod Touch support
* Added support for GDI display mode for Windows devices, useful for making windowed applications
* Added a z-axis to the tilt events and functions
* Speed up loading of images
* Added faster performing zlib libraries for Windows Mobile, Symbian S60 3rd edition, UIQ 3.x, Windows desktop and GP2X
* Increased performance of OpenGL texture uploader
* Extended StrUpper and StrLower string functions to also change the case of accented letters like ë and Ç
* Updated EDGELIB Builder to beta 14 increasing the amount of project files and adding a stop build option
* Fixed a memory leak in E2DSurfaceRGBA, it happened when creating surfaces with extra flags such as EST_DITHER and EST_READONLY
* Fixed a bug with E2DSurface, now it is more stable when reloading and reuploading the same texture two or more times
* Small bugfix in the E2DSurface BltFx function, the E2DBLTFX parameter isn't modified inside the function anymore

## Version 3.80 (2008-09-17)
* Added perspective correct texture mapping, it's possible to switch between affine and perspective correct texture mapping through a simple function
* Added a new 3D perspective calculation method for the internal renderer, it is similar to OpenGL
* Expanded functionality to change the 3D perspective
* Added scale/rotate blitting to the display class, making it possible to use this in OpenGL as well
* Added node parsing to the XML class, making it possible to access nodes like the XML-RPC class
* Added a function to manually set a matrix to a 3D surface
* Added a property parameter to the SetShading function of ClassEDisplay. This can be used for the internal software renderer
* The optimized texture mapper can also be used now for non-square textures, as long as both width and height are a power of 2
* Removed vertical flipping of texture coordinates for the internal software renderer, they should be identical for OpenGL and the internal renderer now
* Bugfix for BltFx, now it works correctly on pre-rotated surfaces
* Bugfix for PrepareFontWrap, it works correctly now with Unicode strings

## Version 3.70 (2008-05-29)
* Added support for Linux desktop (XWindows/Xlib), now you can also build native applications for Linux desktop
* Increased performance of the native image loaders more than 25%
* EDGELIB now handles resolution and color depth changing correctly on Windows desktop machines
* Created a new version of the EDGELIB Strip Maker tool adding support for images with an alpha channel
* Added an option to capture mouse move events even if the mouse cursor is moved outside of the window
* Bugfix for E3DSurface: now it doesn't crash anymore when using a global 3D surface
* Minor fixes in the EString class
* Updated EDGELIB Builder to beta 12 fixing minor issues with Symbian capabilities

## Version 3.60 (2008-03-25)
* Added support for more tilt devices: EDGELIB now supports the Accelerometer found in the Nokia 5500, N93, N95 8GB and N82
* Tilt device emulation is now disabled by default to prevent issues with devices where the tilt device can't be removed or deactivated, use EFL_TILTEMULATOR to enable it again
* Added the ability to blit and draw fonts using RGBA surfaces through ClassEDisplay
* WorldRotate should now work identical for OpenGL and the internal renderer
* Changed internal 3D structures to be in fixed point for all implementations
* Added several Symbian model names based on their Device ID from Nokia, Sony-Ericsson and Motorola
* Updated EDGELIB Builder to beta 11 fixing a bug when using an absolute path in source files
* Bugfix for RGBA texture uploading, now it should work correctly
* Bugfix for SetFlipRect, now it should work properly on Mobile devices
* Bugfix in the RPN calculator, it now parses the 'i' symbol correctly for Device ID's larger than 10 digits
* Bugfix for backlight control on some Windows Mobile devices

## Version 3.50 (2008-01-09)
* Added inverted sine and cosine functions
* Added inverted tangent functions to get the angle based on object distances
* Bugfixes in the XML and XML-RPC classes, they are now accessible again
* Bugfix in ClassEInput::SetRepeatRates, it should work correctly now

## Version 3.40 (2007-12-19)
* Added support for font drawing with RGBA surfaces
* Added support for GPH's touchscreen-enabled GP2X F-200 handheld by adding stylus events
* Added a new tool for creating font images easily from a TrueType font file, also supporting RGBA
* Improved XML and XML-RPC parser to handle Unicode files and ampersand character
* Bugfixes in the XML-RPC parser, now it handles all tags correctly
* Fixed multiple stylus up events bug on Windows desktop builds
* WriteBMP and WritePNG are now able to write 8-bit indexed color files if the source surface is 8-bit
* Fixed starting EDGELIB from Dialog Boxes for Windows desktop

## Version 3.30 (2007-11-27)
* Added lightweight and easy to use XML parser
* Added XML-RPC parser and node objects
* Added basic string class
* Added basic data classes for data array management
* Completely reworked internal network code adding new features and making it more flexible
* SearchWireless works asynchronously for Windows Mobile now, also changed the behavior of the searchservices parameter
* Added a new efficient EDGELIB 3D (.e3d) file format
* Fixed the Milkshape 3D (.ms3d) loader for devices running Symbian versions below 9.x
* Added forward compatibility for Bluetooth on Windows Mobile, there is no need to create separate Windows Mobile 2002 and 2003 builds now
* Added function to get the position of the softkey labels
* Added Gizmondo key names
* Added a function to reallocate memory
* Added the ability to use the bottom right key (escape) of the Nokia 7710, changed key mappings accordingly
* PrepareFontWrap now removes trailing spaces before newlines
* Updated the EDGELIB Builder to beta 10 fixing some threading issues, platform INI files for Visual Studio 2005 and some changes for Symbian UID handling
* Fixed orientation issues for Nokia E90 and Nokia N93 devices
* Bugfix for Windows Mobile Smartphone: the home key now minimizes the application correctly
* Bugfix for some Pocket PC phone devices: the power off button now suspends the application properly
* Bugfix for some Pocket PC phone devices: the application doesn't use processor cycles when using the power (suspend) button
* Added device ID for Nokia E90 and Nokia N93 handsets
* Added forward compatibility for backlight control for Windows Mobile, backlight control supports more devices
* Bugfix for Windows Mobile: stylus input works correctly again after minimizing in full screen applications
* Bugfix for full screen Windows desktop: the application doesn't crash after shutdown anymore
* Bugfix for GradientRect, now it works correctly on rotated displays
* Small bugfix for Windows desktop: it isn't possible to click the 'i' button of the EDGELIB status bar two times anymore

## Version 3.20 (2007-09-26)
* Added support for the NVIDIA OpenGL ES SDK
* Added support for native Windows Mobile 6 (Windows Mobile Standard and Professional SDK)
* Added support for the Gizmondo device running Windows CE
* The "hang up" button closes applications on Symbian S60 3rd edition devices like it should
* Applications will close more quickly when using the 'C' key in the task list on Symbian
* Added forward compatibility to Windows Mobile 2002 builds, they now work correctly on rotated displays in Windows Mobile 2003+ devices
* Improved key mappings for Symbian UIQ devices
* Added the ability to read the serial number of GP2X devices with later firmware version (2.00+)
* Merged pluginrasteroid.lib and pluginpowervr.lib libraries for Windows Mobile into plugin1-0.lib for ease of use
* Improved input event handling on Windows Mobile
* Improved OpenGL (ES) handling after changing orientation on Windows Mobile devices
* Fixed various screen refresh bugs on Windows Mobile on certain events (for example, the phonecall notification window)
* Focus change events are now handled properly for fullscreen DirectDraw applications on Windows Mobile
* Fixed orientation bugs on square screen devices
* Fixed a bug which made the program instable when using global surface objects
* Fixed a bug which crashed the program when using more than 32 surfaces simultaneously
* Fixed a bug related to surface resizing, it works correctly now
* Improved stability while using ClassESound in combination with Hekkus Sound System
* Bugfix for Windows Mobile, the process is properly destroyed when closing the application forcefully
* Bugfix for DrawFont, it displays Unicode strings correctly now
* Bugfix for reading file size of an open file, it now flushes file buffers before reading its size
* Added documentation for Resize (RGB and RGBA) to the reference

## Version 3.11 (2007-08-24)
* Fixed a major bug with Symbian, applications built for Symbian 6.0 to 8.1 should be able to start again
* Made E2DSurface object more stack friendly, this results in more stability for Symbian 6.0 to 8.1 builds
* Fixed a bug with handling EDGELIB Packer resource files and vendor authorization in EDGELIB and the EDGELIB packer

## Version 3.10 (2007-08-20)
* Completely renewed EDGELIB's internal framework which allows more flexibility and new features (see below)
* Added windowed mode for Windows Mobile enabling the title bar and the bottom menu (if present)
* Added the ability to rotate the display run-time (hot rotate)
* Added the ability for the application to read the device settings and take the current orientation of the device
* Expanded the SearchWireless API, it's now able to return Bluetooth services on all found devices
* Fixed a large number of input issues for Windows Mobile regarding device orientation
* DirectDraw loads dynamically now, the DirectX SDK isn't needed anymore to develop with EDGELIB
* Added dynamic loading of GAPI, your Windows Mobile application is allowed to run on devices without gx.dll
* Added downscaling with optional resampling for RGB and RGBA surfaces (Resize)
* Added writing of surfaces to PNG
* Improved logging of network events in the EDGELIB console
* Improved support for backlight control on Windows Mobile
* Improved accuracy of Symbian timing
* Added console button to the status bar to toggle the console window on Windows desktop
* Added CRC checksum calculation
* Added UTF-8 string conversion functions
* Minor change to the random generator, added the possibility to generate negative numbers
* Bugfix in blitting indexed 8-bit surfaces
* Bugfix in the Sleep() function for Symbian devices which caused unexpected results in certain cases
* Renamed Edge IDE to EDGELIB Builder and Edge Packer to EDGELIB Packer
* Added a configuration dialog to the EDGELIB Builder tool for setting the EDGELIB installation path and vendor settings (updated to beta 8)
* Added device ID for the Nokia N95 handset

## Version 3.00 (2007-04-06)
* Added OpenGL ES and Open GL support, read the OpenGL tutorial on how to get started
* Added 3D model animation
* Added multiple editions of the SDK: Lite, Standard and Full
* Major changes to the existing 3D interfaces, please read the migration tutorial
* Moderate changes to the font system, please read the migration tutorial for detailed information
* Added support for loading Milkshape 3D models, including full animations
* Updated Edge IDE to beta 7 adding user defined commands for generating executables, libraries and setups, project-defined platform selection, a link command in the build menu and a few minor fixes
* Added additional ClassEInput functions to get the number of key and click events
* Added a function 2D and 3D surfaces to check if they have been created
* Edge now uses high-resolution timers for the framework and timer modules in Windows Desktop builds
* Added matrix math to the math class
* IMEI codes are returned in raw format by default, dashes can still be included with an optional parameter
* Improved mouse handling on windowed Windows desktop builds
* Fixed a critical bug for Symbian, minimizing doesn't crash the application anymore when receiving network data
* Fixed directional pad input for Samsung landscape smartphone devices
* Fixed an input bug on Windows Mobile, now the keys are properly locked again after resuming an application
* Fixed bugs in wrapped font drawing, it should use the specified width correctly now
* Bugfix for ClassESound::MusicPlaying, now it returns the correct value
* Bugfix for ClassEFile::OpenFolder, now it opens correctly on all devices
* Added safety checks for StrCpy, StrCat, StrLen and StrTrim definitions to remove error messages in ATL projects

## Version 2.95 (2006-12-13)
* Added a new sound wrapper class to ease implementation of external sound engines
* Added font drawing with automatic word wrapping
* Added font wrap preparation functions as an alternative to direct wrapped font drawing
* Added functionality to the framework for recording and playing back events useful for debugging and reproducing bugs
* Updated Edge IDE to beta 6 adding support for Windows Mobile 2005 and Series 80 first edition as well as improved Symbian UID handling between Symbian 9.0 and Symbian versions below 9.0
* Expanded ClassEMath with random number generator functions and a function to get absolute values
* Added backlight control option to the Edge configuration
* Added new rotation types for forcing portrait or landscape mode in the Edge configuration
* Improved handling of GetChar and DIAM_KBTYPEKEY on Windows Mobile Smartphones
* ClassEdge::SetErrorMsg now also works for Symbian
* Minor input related fixes
* Fixed loading images with black colors in images with transparency disabled
* Several bugfixes relating to indexed 8-bit videomode, palettes and E_GETR, E_GETG and E_GETB macro's, colors should now be shown correctly
* Bugfix: Series 80 second edition devices now have Bluetooth correctly implemented again
* Bugfix: mouse cursor is now correctly shown after returning from an error on Windows desktop fullscreen applications
* Bugfix: quoted comment characters ; and # are interpreted correctly now in ClassEIni 

## Version 2.90 (2006-10-31)
* Optimized Blt() in the following cases: rotation values of 0, 1024, 2048 and 3072 (0, 90, 180 and 270 degrees) and a scale value of 65536 (100%)
* Added optimized filter combination routines (dubbed "blitlets") which can be specified using a plug-in based system for BltFast() and optimized Blt()
* Improved Bluetooth support for Symbian, Windows Mobile and Windows desktop. Devices must be connected through the port member of EBLUETOOTH_DEVICEID
* SearchWireless searches for devices running a Bluetooth server with the same application UID
* Fixed Bluetooth connection issues on Symbian 9.1 devices
* Added Unicode support to ClassEFile and ClassEIni for opening files
* Added Unicode support for loading surfaces and E2DSurface::WriteBmp
* Emulate different display modes with a simple configuration option
* Increased support for 8-bit indexed display mode and color conversion
* Added methods for display palette management
* Added a function to set the font layout of a E2DSurface without knowing its ASCII codes
* Removed dependencies to libc in ClassEFile for Windows Mobile and Windows Desktop
* Updated Edge IDE to beta 5, fixing generation of icons for Windows Mobile 2002 and UIQ 2.1, adding Symbian 9 capabilities, version information and building of .SIS installation files
* Updated Edge Font tool to version 2.00 being able to scan up to 256 characters and being able to set a tolerance option for scanning
* Added support for native loading of JPEG images, including progressive JPEG images
* Added support for native loading of Targa (TGA) images, including alpha channel
* Expanded ClassEInput with functions for retrieving key names for key scan codes
* Added the possibility to get the palette loaded from E2DSurface images
* Added a function to retrieve the name of the operating system
* Increased precision of the framerate limiter
* Added a menu confirm mapping to the jog for UIQ 3 devices
* Bugfixes in font surfaces for non-Unicode drawing and using more than 128 characters
* Small bugfixes for ClassEIni, increased efficiency of parsing using OpenData()
* Bugfix for stylus events in Symbian builds, increased reactivity
* Bugfixes for creating INI files
* Bugfixes for loading transparent GIF and PNG image files
* Bugfix for RGBA surface loading using the EST_READONLY flag
* Bugfixes for receiving device information in ClassEDevice and ClassERegister
* Bugfix for recreating files on Symbian through ClassEFile
* Bugfix for writing files to SD cards on GP2X devices running firmware version 2.0
* Bugfix for reading the GP2X serial number

## Version 2.80 (2006-09-06)
* Major improvements for Symbian 9.1: applications should now run correctly on devices running UIQ 3.0
* Other Symbian 9.1 bugfixes, Edge now works correctly on the Nokia E61 phone (landscape QVGA screen)
* Updated Edge IDE to beta 4 fixing Symbian 9 platforms, adding icon resource support and adding support for Symbian Series 80, Series 90 and Windows Mobile 2003 for Smartphone and Pocket PC
* Updated Edge Packer to 2.10 with the ability to choose the compression method as well as fixing a bug with adding read-only files
* Added functions to ClassEFile to generate a file index for Edge packages, this can drastically speed up reading a large number of files from Edge packages
* Increased speed of BMP, GIF and PNG loaders. Additionally, less memory is used when loading PNG images
* Added support for loading monochrome and 4-bit BMP bitmap images
* Changed the surface loaders to a plugin based system, it's possible to exclude unwanted loaders when they are not needed
* Added an optimized zlib.lib for Symbian 9 device builds
* Added more Nokia and Sony Ericsson device ID's
* Removed the modelid parameter from ClassEDevice::GetModelName
* Added overloaded functions for StrCat for concatening integer values
* Increased the size of the INI parse buffer and added a function to adjust it manually
* Removed the default "Connection: Keep-Alive" header in HTTP requests (fixes an issue with Apache 2.0 webservers), now configurable through EHTTPRequest
* Fixed a small bug with loading PNG's with a single transparency index
* Set a default parameter for ClassEConnect::SearchWireless

## Version 2.71 (2006-08-08)
* Added an internal optimization, Blt() will call BltFast() when no effects are needed
* Fixed a timing issue when limiting fps by using the maxfps setting in the Edge configuration
* Added devices (Nokia 9210 and Nokia E60) when calling ClassEDevice::GetModelName

## Version 2.70 (2006-07-21)
* Added Chillingo's ClickPayGo m-payment API
* Major bugfixes and improvements for Symbian 9.1, applications should now work correctly on devices running Symbian 9.1
* Added a new module ClassEInput, useful for all sorts of input handling
* Added default key mappings for all supported platforms
* Updated the Edge IDE tool to beta 3 adding additional platforms and commandline support
* Symbian now uses the Direct Screen Access (DSA) framework for accessing the display, this should solve a screen refresh bug after minimalization
* Added functions for establishing/releasing TCP/IP connections to the internet for Windows Mobile and Symbian
* Added opening and reading of INI data through memory
* Added functions to ClassEStd changing strings to lower and upper case
* Added the ETYPE_MONEY datatype and macro's for money transactions
* Fixed several TCP/IP and HTTP connection issues
* Fixed a few small issues related to blitting on DirectX devices
* Additional smaller changes (renaming of E_TIMESTAMP to ETYPE_TIMESTAMP and other minor changes)
* Added a function to ClassEMath for converting fixed point values to float (FixedToFloat) 

## Version 2.60 (2006-06-12)
* Added Symbian 9.1 support, libraries which can be linked in combination with the Series 60 3rd edition SDK and the UIQ 3.0 SDK
* Added Symbian 9.1 libraries for the emulator
* Updated the Edge Packer tool to version 2.00 with many improvements and additions, like commandline support
* Updated the Edge IDE tool to beta 2 adding support for the Series 60 first edition and UIQ 2.1 SDK's
* Major update to the Edge Packer file format and updated functions from ClassEFile, keeping the new functions backwards compatible
* Expanded ClassEDevice with more time related functions such as: TimestampNow and LocalTimeOffset
* Added E_TIMESTAMP values and conversion functions in ClassEStd for converting between E_DATETIME and E_TIMESTAMP
* Added functions to ClassEFile to set and retrieve the current working folder of a single object
* Added a MoveData function to ClassEFile
* Added more functions to ClassEIni for writing integer values, comments and newlines. Added searching for keys and values and resetting the cursor
* Speedup for ClassEMemory::Copy and added overlap checking to ensure the data is being copied correctly
* Speedup for ClassEMemory::Set variations, also added the possibilty to set using unaligned pointers
* Added == and != operators to the ClassERegister::CalculateKey RPN parser
* Optimized 3D surfaces when transforming and rendering with unused polygons
* Controls now adapt if the screen rotation changes while the application runs (for devices with a rotatable display)
* Added new global rectangle functions for checking rectangle intersection and if a point is inside a rectangle
* Changed the UIQ framework, you need to add qikctl.lib to the library list for UIQ 2.1 builds
* Fixed various 3D lighting issues and fixed a minor texture mapping issue when loading from .3ds files, added an option to set the brightness of the lightsource
* Fixed a bug causing the start menu not being updated when debugging Windows Mobile applications
* Fixed a bug when getting the current battery level for applications compiled with the Symbian 8+ SDK's
* Fixed a minor bug related to stylus input from applications running in QVGA on a VGA Windows Mobile 5 device
* Fixed a few memory leaks in the Symbian build

## Version 2.51 (2006-04-05)
* Fixed a major bug with E2DSurface::GetClipper and rotated display orientations. Rotated applications compiled with the evaluation version will work properly again
* Minor changes in ClassEFile regarding return values

## Version 2.50 (2006-03-31)
* Added support for the Gamepark Holdings GP2X device running Embedded Linux
* Added native support for Symbian Series 60 second edition, feature pack 2 and 3 (Symbian 8.0 and 8.1)
* Added a beta version of the Edge IDE tool to easier build GP2X applications without using the commandline, also added support to build through Visual C++ 6.0
* Added ClassEIni, a new INI file class
* Added GetSystemRotation, a function to get the rotation of the operating system which can be inherited by Edge, added the ROT_AUTO flag
* Added functions to ClassEMath for calculating integer square root (IntSqrt) and converting float values to fixed point (FloatToFixed)
* Added functions to E3DSurface to get the amount of used vertices/polygons and added an alternative to E3DSurface::SetPolygon
* Extended ClassEStd with more string functions (IsWhiteSpace, StrTrimLeft, StrTrimRight, StrTrim and StrReplace)
* Added the support of using relative paths when opening files: The current path will automatically be prepended when a relative path is detected
* Added the possibility in the file class to use both forward slashes and backslashes in file paths
* Added the ability to read the battery status and battery level
* Increased speed of E2DSurface::WriteBmp and added an optional rectangle parameter
* Increased speed of loading and manually creating 3d surfaces
* Moved functions to read the 'owner name' and IMEI number from ClassERegister to ClassEDevice, added GetRegistrationId in ClassERegister
* Fixed various rotation input bugs when Windows Mobile is running in landscape mode
* Fixed a minor bug in loading 8-bit PNG files with an alpha channel
* Fixed a bug that always showed the mousecursor in fullscreen, even when disabled
* Added an overloaded function of GetFPS, returning the major number of fps

## Version 2.40 (2006-02-27)
* Added native support for Pocketmotion's TiltCONTROL device including emulation
* Added support for devices running Symbian Series 80 first edition (Nokia 9210 and 9290)
* Added a BltFx function for advanced effects such as variable x/y scaling
* Added improved support for Nokia N70 and Nokia N90 devices
* Added macros for manipulating color values
* Added StartList/EndList functions to speed up the creation of manual 3D surfaces
* Fixed a small bug in BltFast() with EFX_TINT and EFX_COLORFILL
* Optimized recreation of surfaces when the new size and type is the same as the previous
* Added Clear functions to E2DSurface and E2DSurfaceRGBA
* Added button definition for the 'Menu' key on Symbian devices

## Version 2.30 (2006-01-30)
* Implementation of an improved filter system. Many filters can be combined with a lot of graphics operations
* Surfaces can be created with any supported color depth by passing a parameter to CreateSurface()
* Added color depth conversion when blitting between surfaces with incompatible color depths
* Improved Blt() and texture mapping and fixed the bugs relating to these functions
* Fixed a bug in the E2DSurface::GetInfo() function for rotated surfaces, added a member to calculate the correct pointer
* Improved opacity blending, it should be faster and has no rounding errors when working with low color depths
* Fixed an application hang problem with DrawLine() and PutPixel()
* Fixed crash problems with SetColorKey() and Blt() using DirectDraw
* Fixed minor clipping bug with Blt() using DirectDraw
* Fixed a minor timing issue with Symbian, the framerate is locked correctly now
* Color filter EFX_COLORFILL added

## Version 2.20 (2005-12-28)
* Added support for DirectDraw for Windows Mobile 5 and Windows desktop
* Added a GetInfo() function to 2D surfaces to retrieve detailed information
* Added the possibility to get surface information when locking the surface
* Added new configuration options to configure how Edge handles DirectDraw
* Added an EST_DEFAULT parameter for creating surfaces, this will be used as default instead of EST_SYSMEMORY
* Added a new function to ClassEDevice: GetCurrentDate()
* Added a new function to 2D surfaces: GetClipper()
* Added a GetVideoMemory() function to the display class
* Fixed a slowdown problem for minimized applications
* Improved Rotate/Scale blitting
* Changed the scale formula for blitting, to convert your parameter use this macro: newscale = (((1 << 30) / scale) << 2))

## Version 2.10 (2005-11-25)
* Added support for new devices: Symbian Series 80 (Nokia 9300 and 9500) and Series 90 (Nokia 7710)
* Added native support for Windows Mobile 5.0
* Added support for Visual Studio 2005 (Windows Mobile and Windows Desktop) and Visual C++ 2005 express edition
* Fixed a problem with projects that contain multiple .cpp files, the .cpp file that contains EdgeMain() needs to include edgemain.h, other .cpp implementations require edge.h
* The UIQ internet and camera shortcut keys can now be used in Edge, without minimizing the application
* Added newline support to the GetFontWidth() function
* Added a new function GetFontHeight() to E2DSurface, including newline support
* Added font alignment flags to the DrawFont() function for horizontal and vertical text alignment
* Added a BackFlip() function to the display class to flip the screen content back to the backbuffer
* Added a screen capture flag to the Edge configuration to capture the screen inside the backbuffer before the Edge application starts
* Added a method to set the flip rectangle, useful if only small parts of the screen needs to be updated (only available in the commercial version)
* Added a new option to the Edge configuration to set the Symbian flip method
* Changed the Edge configuration highresolution option to resolution, adding more options
* Changed the caption variable of the Edge data structure, this needs to be set by using a string copy
* Fixed a bug when drawing blended rectangle outlines
* Fixed a bug with fixed width fonts and using the SetFontOffset() function
* Fixed a bug in the GetPixel() function for RGBA surfaces
* Fixed a bug in the PackAddFile() and PackAddFileData() function in the file class when the overwrite argument was true

## Version 2.00 (2005-10-14)
* First public release!
* Optimized blitter routines up to 25%
* Added custom pixel shader functionality for blitting
* Expanded HTTP functionality
* Added calculation of MD5 hashes
* Added a minimal pixel brightness parameter for building the collision detection mask table (optional)
* Added collision detection for RGBA surfaces
* Added an option to disable the loading of the internal font to save memory
* Added a function to skip a number of unused characters in a font surface
* Expanded the 2D surface intersect function with the optional abily to return the actual hit point of the collision
* Added button definitions for keyboard keys such as: enter, space, escape, shift and control

## Version 1.95 (2005-09-16)
* Added button definitions for two softbuttons and a home key
* Added filtering of shadow keys for Windows Mobile
* Fixed a bug with large rotate/scale blitting
* Expanded file class: deleting/renaming files and folders, creating new folders, reading file attributes and more
* Added a standard class as a unified interface to standard functions

## Version 1.90 (2005-08-12)
* Fixed bugs with rotate/scale blitting and display orientations
* Small 3D render optimizations
* Added Edge support for dialog boxes
* Added gradient fill function
* Added a function to automatically set a color key, based on a pixel color
* Added a dithering option for Gouraud 3D rendering and gradient filling

## Version 1.80 (2005-07-15)
* Fixed bugs with the input system for Windows Mobile 2003 SE
* Fixed a Symbian display bug after minimizing
* Added TCP/IP support for Symbian

## Version 1.70 (2005-06-02)
* Added support for truecolor ("18 bit") Symbian devices (for example Nokia 6680)
* Added wipe functions to the display class
* Added wireframe rendering
* Added blitting with rotate/scale and flip support
* Added a function to render a semi-3D landscape

## Version 1.60 (2005-05-06)
* Fixed a screen refresh bug on Symbian, which occured after a long pause 
* Fixed a stylus input bug with the QVGA version installed on a VGA device 
* Fixed a bug affecting the controls in landscape mode on Windows Mobile 2003 devices 

## Version 1.50 (2005-04-26)
* Added RGBA surfaces
* Added .PNG file support for 2D surfaces
* Added ZLIB compression/decompression support
* Added .GIF file support for 2D surfaces

## Version 1.25 (2005-02-18)
* Added directory reading for Symbian
* Fixed a few bugs with the Symbian file system
* Speed up for 3D rendering
* Added .3DS file support for 3D surfaces
* Added native VGA support for Windows Mobile
* Added a clipper for 2D surfaces

## Version 1.10 (2004-12-22)
* Added Bluetooth network support for Symbian and Windows Mobile
* Added a network connection class using TCP/IP for Windows Mobile
* Added a console class for showing debug messages on Windows desktop
* Added a dithering option when loading graphics
* Added a device class with a method to read the current time
* Reading of directories for Windows Mobile
* Reading IMEI codes for Symbian devices

## Version 1.00 (2004-09-10)
* Added support for Symbian Series 60 and Symbian UIQ
* Added support for basic 3D operations

## Version 0.75 (2004-04-06)
* Initial release supporting Pocket PC, Microsoft Smartphone and Windows desktop

