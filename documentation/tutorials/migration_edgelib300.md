:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Migrating from EDGELIB version 2.xx to 3.00

## API changes
EDGELIB 3.00 introduces moderate changes to the font system and major changes in the 3D renderer.

## Font surfaces
The most important change is that the font surface is created in a different way. When using the new system, first set font properties (for example, the number of used characters), and then call BuildFont() for it to be used. Here is a small sample for creating a fixed width and variable width font:

```c++
E2DSurface fixedfont, variablefont; 
if (display->CreateSurface(&fixedfont, "fixedfont.bmp") == E_OK) 
{ 
    fixedfont.SetFontCharacters('A', 'z'); 
    fixedfont.SetFontPadding(1, 0); 
    fixedfont.BuildFont(false, ECOLOR_FUCHSIA); 
} 
if (display->CreateSurface(&fixedfont, "variablefont.bmp") == E_OK) 
{ 
    variablefont.SetFontCharacterSet('ABCDEFG'); 
    variablefont.BuildFont(true, ECOLOR_FUCHSIA); 
}
```

Only SetFontPadding() can be called after building the font, the other functions should be called once. E2DSurface::BuildTableVwf() is deprecated and replaced by BuildFont() with the first parameter set to true.

A new addition to the font system are fonts with multiple rows. These can be specified using E2DSurface::SetFontSheetLayout(). More information is available [here](../reference/e2dsurface_setfontsheetlayout.md).

Other changes include the following:

* BuildFont() should always be called, otherwise the font functions won't be available
* By default, font characters 32 to 127 are used instead of 0 to 127

## 3D renderer
With the addition of an OpenGL renderer, the EDGELIB internal 3D renderer also made some changes. Binding light sources and textures are handled through the display class. Also, the internal renderer will support up to 8 light sources. The following sample loads a 3D model with a texture and renders it to the backbuffer:

_Code (6 lines)_
```c++
E2DSurface texture;
E3DSurface model;
display->CreateSurface(&model, "edgelogo.3ds");
if (display->CreateSurface(&texture, "texture.bmp") == E_OK)
    display->BindTexture(&texture);
display->buffer.Render(0, 0, &model, E3D_TEXTURE);
```

The following example sets 2 light sources and renders with gouraud shading:

_Code (5 lines)_
```c++
display->SetLightSource(0, -4096, 4096, -4096, ECOLOR_RED);
display->UseLight(0);
display->SetLightSource(1, 4096, 4096, -4096, ECOLOR_GREEN);
display->UseLight(1);
display->buffer.Render(0, 0, &model, E3D_GOURAUD);
```

Other changes include the following:

* Lighting, colors and texture coordinates are linked to vertices instead of polygons
* The y-axis points up, as in the real-life coordinate system
* Vertices in polygons should be assigned counter-clockwise
* Texture coordinates have higher precision, they go from 0 to 65536 instead of 0 to 256
* Model transformation (translate, rotate and scale) uses matrix math, call these functions in reversed order
* Render sizes and perspective have changed
* Changed model size in the .3ds loader, added a parameter to [CreateSurface](../reference/classedisplay_createsurface.md) to give more control over the size of the model

## Chapters
* [Migrating from EDGELIB version 3.00 to 3.10](migration_edgelib310.md)
* **Migrating from EDGELIB version 2.xx to 3.00**

