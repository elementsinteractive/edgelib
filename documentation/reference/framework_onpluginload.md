:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# OnPluginLoad


```c++
ERESULT OnPluginLoad(ClassEDisplay *display)
```

## Parameters
### *display (out)
A pointer to the [`ClassEDisplay` module](classedisplay.md).

## Return value
Return `E_OK` when everything went correctly. Return `E_ERROR` to shutdown EDGELIB.

## Description
This is a callback for loading plugins, such as surface loaders. By default all built-in plugins are loaded, but this behaviour can be changed by defining `EDGE_NODEFAULTPLUGINS`. The size of the final executable can be decreased when unneeded plugins are not loaded.

## Example
Here is an example on how to register all surface loaders:

```c++
ERESULT ClassMain::OnPluginLoad(ClassEDisplay *display) 
{ 
    ELoader_BMP *bmp = new ELoader_BMP; 
    if (bmp == NULL) 
        return(E_NOMEMORY); 
    if (!display->RegisterLoader(bmp)) 
    { 
        delete bmp; 
        return(E_ERROR); 
    } 
    ELoader_GIF *gif = new ELoader_GIF; 
    if (gif == NULL) 
        return(E_NOMEMORY); 
    if (!display->RegisterLoader(gif)) 
    { 
        delete gif; 
        return(E_ERROR); 
    } 
    ELoader_PNG *png = new ELoader_PNG; 
    if (png == NULL) 
        return(E_NOMEMORY); 
    if (!display->RegisterLoader(png)) 
    { 
        delete png; 
        return(E_ERROR); 
    } 
    ELoader_JPG *jpg = new ELoader_JPG; 
    if (jpg == NULL) 
        return(E_NOMEMORY); 
    if (!display->RegisterLoader(jpg)) 
    { 
        delete jpg; 
        return(E_ERROR); 
    } 
    ELoader_TGA *tga = new ELoader_TGA; 
    if (tga == NULL) 
        return(E_NOMEMORY); 
    if (!display->RegisterLoader(tga)) 
    { 
        delete tga; 
        return(E_ERROR); 
    } 
    ELoader_3DS *l3ds = new ELoader_3DS; 
    if (l3ds == NULL) 
        return(E_NOMEMORY); 
    if (!display->RegisterLoader(l3ds)) 
    { 
        delete l3ds; 
        return(E_ERROR); 
    } 
    ELoader_MS3D *ms3d = new ELoader_MS3D; 
    if (ms3d == NULL) 
        return(E_NOMEMORY); 
    if (!display->RegisterLoader(ms3d)) 
    { 
        delete ms3d; 
        return(E_ERROR); 
    } 
    return(E_OK); 
}
```

The following example shows how to decrease your application size by only registering the PNG loader:


```c++
ERESULT ClassMain::OnPluginLoad(ClassEDisplay *display) 
{ 
    ELoader_PNG *png = new ELoader_PNG; 
    if (png == NULL) 
        return(E_NOMEMORY); 
    if (!display->RegisterLoader(png)) 
    { 
        delete png; 
        return(E_ERROR); 
    } 
    return(E_OK); 
}
```

## Version changes
- EDGELIB 2.80: Added this function to the SDK

