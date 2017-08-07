:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Migrating from EDGELIB version 3.00 to 3.10

## API changes
EDGELIB 3.10 introduces moderate changes to the framework, configuration and some data types.

## Framework configuration
The biggest change is how the framework is configured. The method OnCreate has been replaced by OnFrameworkConfig and OnDisplayConfig. Also, some settings of the configuration has been removed, renamed or replaced. Below is a list of configuration options that have been changed:

**config->orientation** 
This setting has been changed from config->rotation to config->orientation. Definitions have been changed, for example ROT_90DEG is now DOR_90DEG. Also the functionality of DOR_AUTO is changed, it will rotate the application if the device orientation is changed while the application runs. Use DOR_STARTUP when the application should only take the orientation at startup, and not rotate while it runs.

**config->fullscreen** 
This setting will now affect Windows Mobile applications because EDGELIB supports windowed mode. Set this to true for all mobile devices if your application should run fullscreen.

**config->openmethod** 
This option replaces config->resolution and config->flipmethod. It allows to set the preferred method to open the display. It's set to DOM_DEFAULT to open the recommended display mode, but can be configured. For Windows Mobile it can be set to DOM_DIRECTDRAW, DOM_FRAMEBUFFER or DOM_GAPI. For Symbian it can be set to DOM_DSA, DOM_GDI or DOM_FRAMEBUFFER. When config->resolution has been set to ERS_EMULATELOWRES, you need to set config->openmethod to DOM_GAPI. DOM_GAPI will only be used when the application is built for Windows Mobile 2002 and 2003 and the resource HI_RES_AWARE isn't present in resource.rc. When config->flipmethod has been set to DOM_DIRECTCOPY, config->openmethod should be set to DOM_FRAMEBUFFER. This is only supported from Symbian 6.0 to Symbian 8.1.

**config->netlogging** 
This is a new feature, it enables logging of network events in the console.

**config->lockkeys, config->keysounds** 
These options were automatically enabled in previous EDGELIB releases, now they are configurable. It enables the use of shortcut keys in your application and turns off the sounds when keys are pressed on phone devices.

**config->caption, config->icon** 
These options are moved from EdgeMain, set the title and icon of your application in OnDisplayConfig.

**config->dialogbox** 
In previous EDGELIB releases this was a parameter to ClassEdge::Start, now it is an option which can be configured in OnDisplayConfig.

**config->menu** 
This is a new feature, it allows the use of menus in Windows Desktop and Windows Mobile applications. It should be set to the resource ID from resource.h.

**config->width, config->height** 
These are renamed options. Before they were config->videowidth and config->videoheight.

**config->surfacedefault** 
This is set to EST_SYSMEMORY by default. Using native surfaces seem to have a negative performance hit on many devices.

**config->surfprerotate** 
This is a new feature. When set to false, it will not pre-rotate surfaces. The pitches of the backbuffer and other surfaces will always be the same, regardless of orientation. Applications have better performance when this is set to true. This options needs to be set to true for Symbian 9+ devices.

## EdgeMain and ClassEdge changes
There are some small changes in the EdgeMain function and the ClassEdge constructor.

```c++
ClassEdge *EdgeMain(EDGESTARTUP *data) 
{ 
    return(new ClassMain); 
}
```

The MAINRET definition is replaced by ClassEdge *, also the EDGEDATA structure is replaced by EDGESTARTUP. This new structure is similar to EDGEDATA, it contains the commandline and returnvalue members. The caption, instance and icon members have been removed. There is no need to call ClassEdge::Start, just return an instance of the object derived from ClassEdge.

The other change is in the ClassEdge constructor. It takes a void parameter and it isn't needed to pass the EDGEDATA structure anymore.

## Data types
The EINSTANCE and EWND data types have been replaced by ENATIVETYPE. To cast this to an instance handle or window use the following code:

```c++
#if defined(DEVICE_WIN32) 
    HWND hwnd = (HWND)winhandle; 
    HINSTANCE hinstance = (HINSTANCE)instance; 
#elif defined(DEVICE_SYMBIAN) 
    RWindow *window = (RWindow *)winhandle; 
#endif  
```

ENATIVETYPE is also used for other values, like getting the native (Symbian bitmap or DirectDraw) surfaces from E2DSurface.

## Native event handler
For previous EDGELIB applications it was possible to handle native events through OnMessage. This function has been renamed to OnNativeEvent and it takes four ENATIVETYPE parameters. A new feature is that it is now possible to use this for Symbian. Here is an example on how to cast the parameters to the correct data types:

```c++
bool ClassMain::OnNativeEvent(ENATIVETYPE winhandle, ENATIVETYPE msg, ENATIVETYPE param1, ENATIVETYPE param2) 
{ 
    #if defined(DEVICE_WIN32) 
        HWND hwnd = (HWND)winhandle; 
        UINT wmsg = (UINT)msg; 
        WPARAM wparam = (WPARAM)param1; 
        LPARAM lparam = (LPARAM)param2; 
    #elif defined(DEVICE_SYMBIAN) 
        RWindow *window = (RWindow *)winhandle; 
        TWsEvent *aEvent = (TWsEvent *)msg; 
    #endif 
}
```

## Device orientation
It's now possible to rotate the application at run-time (so called "hot rotate"). When orientation is set to DOR_AUTO, it will keep the orientation of the operating system and may change while the application runs. EDGELIB will rotate all internal surfaces automatically, it may only be needed to recalculate the screen interface. Also, the backbuffer must be redrawn.

When the orientation is changed real-time, the OnDisplayChange method is called. This is a new callback in the ClassEdge framework.

## Libc usage
For Symbian and Win32 builds, EDGELIB isn't dependent on libc anymore. If you would like to use libc, you need to manually include one of its headers (like string.h, or math.h). When using libc on Symbian, you need to add the following function to the framework destructor to prevent a memory leak:

```c++
CloseSTDLIB();
```

## Chapters
* **Migrating from EDGELIB version 3.00 to 3.10**
* [Migrating from EDGELIB version 2.xx to 3.00](migration_edgelib300.md)

