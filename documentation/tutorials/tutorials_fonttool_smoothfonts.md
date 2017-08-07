:warning: _This file was converted from the EDGELIB 4.02 documentation from 2012 and is included for historic purposes. The documentation is not maintained anymore: information is outdated and external links might be broken._

# Smooth fonts

## How it works
It's also possible to create font files to draw colored fonts with a smooth black edge. The way to do it is to create two font files, one for the black stroke and one for the colored font. The font with the black stroke will be drawn with the `EFX_SUB` filter, then the actual font will be drawn over it. The pictures below give a visual presentation how it works.

![A font character](images/edgefont_character.png "A font character")
![A font mask character](images/edgefont_character-mask.png "A font mask character")
![Background image](images/edgefont_background.png "Background image")
![A font mask character blitted with EFX_SUB](images/edgefont_blitmask.png "A font mask character blitted with EFX_SUB")
![A smooth font character](images/edgefont_blitfont.png "A smooth font character")

## Preparing font files
It's possible to create these font characters easily with the font tool. The provided photoshop file can be used as a template. It contains a font with the stroke effect which can be turned on and off. The masked font will be saved with the stroke enabled, the normal font with the stroke effect disabled. This will create two font files where the position of the font characters will match. Below is a sample of these files.

![Font file sample](images/edgefont_font.png "Font file sample")
![Font mask file sample](images/edgefont_fontmask.png "Font mask file sample")

After these files are ready, use the EDGELIB Font tool to organize them into font files usable by EDGELIB. This font is a special case, because they need to be used together. The characters in the font file without the stroke effect need to have the same reserved font width as the characters in the file with the stroke effect. This can be done by processing these two font files at the same time when using the EDGELIB Font tool.

When selecting a file in the file open dialog box, first select the file without the stroke effect and then select the file with the stroke effect. The font file with characters that take the most amount of space should be selected last (it will be listed first in the filename text field of the open file dialog box).

After processing these files, the result will look like this:

![The font file sample (processed)](images/edgefont_font-target.png "The font file sample (processed)")
![The font mask file sample (processed)](images/edgefont_fontmask-target.png "The font mask file sample (processed)")

## Code sample
After creating these font files, they can be used to draw smooth font. Load these files onto two surfaces and build the variable width font table for both font surfaces. Then draw a text sample, first by drawing the mask with the `EFX_SUB` color filter, then draw the normal font over it (no color filter is needed). Here is a small code sample:

_Code (12 lines)_
```c++
E2DSurface font;
E2DSurface fontmask;
if (display->CreateSurface(&font, "font1_frontmask_target.bmp") != E_OK)
    return(E_ERROR);
if (display->CreateSurface(&fontmask, "font1_backmask_target.bmp") != E_OK)
    return(E_ERROR);
font.SetFontCharacters(0, 127);
font.BuildFont(true, ECOLOR_FUCHSIA);
fontmask.SetFontCharacters(0, 127);
fontmask.BuildFont(true, ECOLOR_FUCHSIA);
display->buffer.DrawFont(0, 0, &fontmask, "Smooth font text sample", EFX_COLORKEY | EFX_SUB);
display->buffer.DrawFont(0, 0, &font, "Smooth font text sample", EFX_COLORKEY);
```

![Smooth font result](images/edgefont_code-result.png "Smooth font result")

## Chapters
* [Creating fonts](tutorials_fonttool_creatingfonts.md)
* **Smooth fonts**

