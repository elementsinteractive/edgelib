// esurf_2dbase.cpp
// 2D Base
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "estdc.h"
#include "eloader.h"
#include "ememory.h"
#include "edisplay.h"
#include "econsole.h"

#include "extra/verinfo.h"

///////////////////////////////////////////////////////////////////
// Internal includes                                             //
///////////////////////////////////////////////////////////////////

#include "extra/excoll.h" //Colission hit testing
#include "extra/exfonthdr.h"
#include "extra/exfont.h" //Font drawing

///////////////////////////////////////////////////////////////////
// E2DSurfaceBase: private                                       //
///////////////////////////////////////////////////////////////////

//Clip the rectangle to prevent offbound memory access or user defined clip rectangle
bool E2DSurfaceBase::ClipRect(RECT *rc)
{
	if (rc == NULL)
		return(false);
	if (rc->left >= rc->right)
		return(false);
	if (rc->top >= rc->bottom)
		return(false);
	if (rc->right <= cliprc.left)
		return(false);
	if (rc->bottom <= cliprc.top)
		return(false);
	if (rc->left >= cliprc.right)
		return(false);
	if (rc->top >= cliprc.bottom)
		return(false);
	if (rc->left < cliprc.left)
		rc->left = cliprc.left;
	if (rc->top < cliprc.top)
		rc->top = cliprc.top;
	if (rc->right > cliprc.right)
		rc->right = cliprc.right;
	if (rc->bottom > cliprc.bottom)
		rc->bottom = cliprc.bottom;
	if (rc->left >= rc->right)
		return(false);
	if (rc->top >= rc->bottom)
		return(false);
	return(true);
}

//Prepare the rectangle for a blit to prevent offbound memory access, update position accordingly
bool E2DSurfaceBase::PrepareBlitRect(long &x, long &y, RECT *rc, long rcwidth, long rcheight, long dwidth, long dheight)
{
	if (rc == NULL)
		return(false);
	if (x >= cliprc.right)
		return(false);
	if (y >= cliprc.bottom)
		return(false);
	if (rc->left >= rc->right)
		return(false);
	if (rc->top >= rc->bottom)
		return(false);
	if (rc->right < 0)
		return(false);
	if (rc->bottom < 0)
		return(false);
	if (rc->left >= rcwidth)
		return(false);
	if (rc->top >= rcheight)
		return(false);
	if (rc->left < 0)
	{
		x -= rc->left;
		rc->left = 0;
	}
	if (rc->top < 0)
	{
		y -= rc->top;
		rc->top = 0;
	}
	if (x < cliprc.left)
	{
		rc->left -= x - cliprc.left;
		x = cliprc.left;
	}
	if (y < cliprc.top)
	{
		rc->top -= y - cliprc.top;
		y = cliprc.top;
	}
	if (rc->right > rcwidth)
		rc->right = rcwidth;
	if (rc->bottom > rcheight)
		rc->bottom = rcheight;
	if (x + (rc->right - rc->left) > cliprc.right)
		rc->right = rc->left + cliprc.right - x;
	if (y + (rc->bottom - rc->top) > cliprc.bottom)
		rc->bottom = rc->top + cliprc.bottom - y;
	if (rc->left >= rc->right)
		return(false);
	if (rc->top >= rc->bottom)
		return(false);
	return(true);
}

//Prepare the rectangle for a blit to prevent offbound memory access, update position accordingly
bool E2DSurfaceBase::PrepareBlitFlipRect(long &x, long &y, RECT *rc, long rcwidth, long rcheight, long dwidth, long dheight, unsigned short rotation, unsigned char flipflags)
{
	if (rc == NULL)
		return(false);
	if (x >= cliprc.right)
		return(false);
	if (y >= cliprc.bottom)
		return(false);
	if (rc->left >= rc->right)
		return(false);
	if (rc->top >= rc->bottom)
		return(false);
	if (rc->right < 0)
		return(false);
	if (rc->bottom < 0)
		return(false);
	if (rc->left >= rcwidth)
		return(false);
	if (rc->top >= rcheight)
		return(false);
	if (rc->left < 0)
	{
		if ((rotation == 2048 && (flipflags & EFLIP_HORIZONTAL)) || (rotation == 0 && !(flipflags & EFLIP_HORIZONTAL)))
			x -= rc->left;
		else if ((rotation == 3072 && (flipflags & EFLIP_HORIZONTAL)) || (rotation == 1024 && !(flipflags & EFLIP_HORIZONTAL)))
			y -= rc->left;
		rc->left = 0;
	}
	if (rc->top < 0)
	{
		if ((rotation == 2048 && (flipflags & EFLIP_VERTICAL)) || (rotation == 0 && !(flipflags & EFLIP_VERTICAL)))
			y -= rc->top;
		else if ((rotation == 3072 && !(flipflags & EFLIP_VERTICAL)) || (rotation == 1024 && (flipflags & EFLIP_VERTICAL)))
			x -= rc->top;
		rc->top = 0;
	}
	if (x < cliprc.left)
	{
		if ((rotation == 2048 && (flipflags & EFLIP_HORIZONTAL)) || (rotation == 0 && !(flipflags & EFLIP_HORIZONTAL)))
			rc->left -= x - cliprc.left;
		else if ((rotation == 2048 && !(flipflags & EFLIP_HORIZONTAL)) || (rotation == 0 && (flipflags & EFLIP_HORIZONTAL)))
			rc->right += x - cliprc.left;
		else if ((rotation == 3072 && !(flipflags & EFLIP_VERTICAL)) || (rotation == 1024 && (flipflags & EFLIP_VERTICAL)))
			rc->top -= x - cliprc.left;
		else if ((rotation == 3072 && (flipflags & EFLIP_VERTICAL)) || (rotation == 1024 && !(flipflags & EFLIP_VERTICAL)))
			rc->bottom += x - cliprc.left;
		x = cliprc.left;
	}
	if (y < cliprc.top)
	{
		if ((rotation == 2048 && (flipflags & EFLIP_VERTICAL)) || (rotation == 0 && !(flipflags & EFLIP_VERTICAL)))
			rc->top -= y - cliprc.top;
		else if ((rotation == 2048 && !(flipflags & EFLIP_VERTICAL)) || (rotation == 0 && (flipflags & EFLIP_VERTICAL)))
			rc->bottom += y - cliprc.top;
		else if ((rotation == 3072 && (flipflags & EFLIP_HORIZONTAL)) || (rotation == 1024 && !(flipflags & EFLIP_HORIZONTAL)))
			rc->left -= y - cliprc.top;
		else if ((rotation == 3072 && !(flipflags & EFLIP_HORIZONTAL)) || (rotation == 1024 && (flipflags & EFLIP_HORIZONTAL)))
			rc->right += y - cliprc.top;
		y = cliprc.top;
	}
	if (rc->right > rcwidth)
	{
		if ((rotation == 2048 && !(flipflags & EFLIP_HORIZONTAL)) || (rotation == 0 && (flipflags & EFLIP_HORIZONTAL)))
			x -= rcwidth - rc->right;
		else if ((rotation == 3072 && !(flipflags & EFLIP_HORIZONTAL)) || (rotation == 1024 && (flipflags & EFLIP_HORIZONTAL)))
			y -= rcwidth - rc->right;
		rc->right = rcwidth;
	}
	if (rc->bottom > rcheight)
	{
		if ((rotation == 2048 && !(flipflags & EFLIP_VERTICAL)) || (rotation == 0 && (flipflags & EFLIP_VERTICAL)))
			y -= rcheight - rc->bottom;
		else if ((rotation == 3072 && (flipflags & EFLIP_VERTICAL)) || (rotation == 1024 && !(flipflags & EFLIP_VERTICAL)))
			x -= rcheight - rc->bottom;
		rc->bottom = rcheight;
	}
	if (rotation % 2048 == 0)
	{
		if (x + (rc->right - rc->left) > cliprc.right)
		{
			if ((rotation == 2048 && !(flipflags & EFLIP_HORIZONTAL)) || (rotation == 0 && (flipflags & EFLIP_HORIZONTAL)))
				rc->left = rc->right + x - cliprc.right;
			else
				rc->right = rc->left + cliprc.right - x;
		}
		if (y + (rc->bottom - rc->top) > cliprc.bottom)
		{
			if ((rotation == 2048 && !(flipflags & EFLIP_VERTICAL)) || (rotation == 0 && (flipflags & EFLIP_VERTICAL)))
				rc->top = rc->bottom + y - cliprc.bottom;
			else
				rc->bottom = rc->top + cliprc.bottom - y;
		}
	}
	else
	{
		if (x + (rc->bottom - rc->top) > cliprc.right)
		{
			if ((rotation == 3072 && (flipflags & EFLIP_VERTICAL)) || (rotation == 1024 && !(flipflags & EFLIP_VERTICAL)))
				rc->top = rc->bottom + x - cliprc.right;
			else
				rc->bottom = rc->top + cliprc.right - x;
		}
		if (y + (rc->right - rc->left) > cliprc.bottom)
		{
			if ((rotation == 3072 && !(flipflags & EFLIP_HORIZONTAL)) || (rotation == 1024 && (flipflags & EFLIP_HORIZONTAL)))
				rc->left = rc->right + y - cliprc.bottom;
			else
				rc->right = rc->left + cliprc.bottom - y;
		}
	}
	if (rc->left >= rc->right)
		return(false);
	if (rc->top >= rc->bottom)
		return(false);
	return(true);
}

//Get the bltfast destination position and source rectangle with bounds checking
ERESULT E2DSurfaceBase::GetBltFastPositions(long &x, long &y, RECT *drct, RECT *srct, E2DSurfaceBase *source)
{
	if (currenttype % 16 == EST_NONE || source->currenttype % 16 == EST_NONE)
		return(E_ERROR);
	if (srct == NULL)
		SetRect(drct, 0, 0, source->GetWidth(), source->GetHeight());
	else
		ClassEMemory::Copy(drct, srct, sizeof(RECT));
	ecd->dsp->VirtualRectPos(x, y, drct, virtualwidth, sheight, source->virtualwidth, source->sheight);
	if (!PrepareBlitRect(x, y, drct, source->virtualwidth, source->sheight, virtualwidth, sheight))
		return(E_INVALIDPARAM);
	return(E_OK);
}

//Get the bltflip destination position and source rectangle with bounds checking
ERESULT E2DSurfaceBase::GetBltFlipPositions(long &x, long &y, RECT *drct, RECT *srct, E2DSurfaceBase *source, unsigned short rotation, unsigned char &flipflags)
{
	if (currenttype % 16 == EST_NONE || source->currenttype % 16 == EST_NONE)
		return(E_ERROR);
	if (srct == NULL)
		SetRect(drct, 0, 0, source->GetWidth(), source->GetHeight());
	else
		ClassEMemory::Copy(drct, srct, sizeof(RECT));
	ecd->dsp->VirtualRectPos(x, y, drct, virtualwidth, sheight, source->virtualwidth, source->sheight, rotation);
	if (ecd->dsp->switchxy)
	{
		unsigned char tmpflip = flipflags;
		flipflags = 0;
		if (tmpflip & EFLIP_HORIZONTAL)
			flipflags |= EFLIP_VERTICAL;
		if (tmpflip & EFLIP_VERTICAL)
			flipflags |= EFLIP_HORIZONTAL;
	}
	if (!PrepareBlitFlipRect(x, y, drct, source->virtualwidth, source->sheight, virtualwidth, sheight, rotation, flipflags))
		return(E_INVALIDPARAM);
	return(E_OK);
}

//Handle orientation change
void E2DSurfaceBase::OnChangeOrientation(long *olddata)
{
}

///////////////////////////////////////////////////////////////////
// E2DSurfaceBase: public                                        //
///////////////////////////////////////////////////////////////////

//E2DSurfaceBase: constructor
E2DSurfaceBase::E2DSurfaceBase(void)
{
}

//E2DSurfaceBase: destructor
E2DSurfaceBase::~E2DSurfaceBase(void)
{
}

//Free surface data
void E2DSurfaceBase::Free(void)
{
}

//Return if the surface is created
bool E2DSurfaceBase::IsCreated(void)
{
	if (currenttype % 16 == EST_NONE)
		return(false);
	return(true);
}

//Retrieve the surface width
unsigned long E2DSurfaceBase::GetWidth(void)
{
	if (currenttype % 16 == EST_NONE)
		return(0);
	if (ecd->dsp->switchxy)
		return(sheight);
	return(virtualwidth);
}

//Retrieve the surface height
unsigned long E2DSurfaceBase::GetHeight(void)
{
	if (currenttype % 16 == EST_NONE)
		return(0);
	if (ecd->dsp->switchxy)
		return(virtualwidth);
	return(sheight);
}

//Get loaded image palette data
void E2DSurfaceBase::GetPalette(COLORREF *palettedata, unsigned char index, unsigned long entries)
{
	unsigned short palctr;
	if (index + entries > 256)
		entries = 256 - index;
	for (palctr = index; palctr < index + entries; palctr++)
		palettedata[palctr - index] = palette[palctr];
}

//Retrieve the width of a font text
unsigned long E2DSurfaceBase::GetFontWidth(char *text)
{
	if (fontrc == NULL)
		return(0);
	return(FontDraw_GetFontWidth((const unsigned char *)text, 0, fontcharset, fontrc, fontkerning, fontinfo.defaultchar, false));
}

//Retrieve the width of a font text
unsigned long E2DSurfaceBase::GetFontWidth(WCHAR *text)
{
	if (fontrc == NULL)
		return(0);
	return(FontDraw_GetFontWidth((const unsigned char *)text, 0, fontcharset, fontrc, fontkerning, fontinfo.defaultchar, true));
}

//Retrieve the height of a font text
unsigned long E2DSurfaceBase::GetFontHeight(char *text)
{
	if (fontrc == NULL)
		return(0);
	return(FontDraw_GetFontHeight((const unsigned char *)text, 0, fontcharset, fontrc, fontkerning, fontinfo.defaultchar, false));
}

//Retrieve the height of a font text
unsigned long E2DSurfaceBase::GetFontHeight(WCHAR *text)
{
	if (fontrc == NULL)
		return(0);
	return(FontDraw_GetFontHeight((const unsigned char *)text, 0, fontcharset, fontrc, fontkerning, fontinfo.defaultchar, true));
}

//Create a wrapped font text
void E2DSurfaceBase::PrepareFontWrap(char *dst, char *src, unsigned long width)
{
	if (fontrc == NULL)
		return;
	FontDraw_PrepareFontWrap((unsigned char *)dst, (const unsigned char *)src, width, fontcharset, fontrc, fontkerning, fontinfo.defaultchar, false);
}

//Create a wrapped font text
void E2DSurfaceBase::PrepareFontWrap(WCHAR *dst, WCHAR *src, unsigned long width)
{
	if (fontrc == NULL)
		return;
	FontDraw_PrepareFontWrap((unsigned char *)dst, (const unsigned char *)src, width, fontcharset, fontrc, fontkerning, fontinfo.defaultchar, true);
}

//Determine if two rectangles intersect, can use pixel precision when a table has been built
bool E2DSurfaceBase::Intersect(long x1, long y1, RECT *rc1, long x2, long y2, RECT *rc2, E2DSurfaceBase *cmp, POINT *pnt)
{
	if (currenttype % 16 == EST_NONE || cmp->currenttype % 16 == EST_NONE)
		return(false);
	RECT nrc[2];
	if (rc1 != NULL)
		ClassEMemory::Copy(&nrc[0], rc1, sizeof(RECT));
	else
		SetRect(&nrc[0], 0, 0, GetWidth(), GetHeight());
	if (rc2 != NULL)
		ClassEMemory::Copy(&nrc[1], rc2, sizeof(RECT));
	else
		SetRect(&nrc[1], 0, 0, cmp->GetWidth(), cmp->GetHeight());
	if (pnt != NULL)
	{
		pnt->x = -1;
		pnt->y = -1;
	}
	ecd->dsp->VirtualRectPos(x1, y1, &nrc[0], ecd->dsp->buffer.GetWidth(), ecd->dsp->buffer.GetHeight(), virtualwidth, sheight);
	ecd->dsp->VirtualRectPos(x2, y2, &nrc[1], ecd->dsp->buffer.GetWidth(), ecd->dsp->buffer.GetHeight(), cmp->virtualwidth, cmp->sheight);
	if (!ClipRect(&nrc[0]))
		return(false);
	if (!cmp->ClipRect(&nrc[1]))
		return(false);
	return(ExMiscIntersect(x1, y1, x2, y2, nrc, cbitmask, cmp->cbitmask, virtualwidth, cmp->virtualwidth, pnt));
}

//Enable font per character padding
void E2DSurfaceBase::SetFontPadding(long horizontal, long vertical)
{
	fontinfo.xspacing = horizontal;
	fontinfo.yspacing = vertical;
	if (fontrc)
	{
		fontrc[0] = (short)horizontal;
		fontrc[1] = (short)vertical;
	}
}

//Set the starting font character in the sheet
void E2DSurfaceBase::SetFontCharacters(WCHAR start, WCHAR end)
{
	fontinfo.charstart = start;
	fontinfo.charend = end;
}

//Set a font character set
void E2DSurfaceBase::SetFontCharacterSet(char *set)
{
	if (set != NULL)
	{
		fontcharset = (WCHAR *)ecd->mem->AllocList((ClassEStd::StrLen(set) + 1) * sizeof(WCHAR));
		if (fontcharset != NULL)
			ClassEStd::StrCpy(fontcharset, set);
	}
	else
		FULL_FREE_L(fontcharset);
}

//Set a font character set
void E2DSurfaceBase::SetFontCharacterSet(WCHAR *set)
{
	if (set != NULL)
	{
		fontcharset = (WCHAR *)ecd->mem->AllocList((ClassEStd::StrLen(set) + 1) * sizeof(WCHAR));
		if (fontcharset != NULL)
			ClassEStd::StrCpy(fontcharset, set);
	}
	else
		FULL_FREE_L(fontcharset);
}

//Change the font sheet layout to support multiple rows
void E2DSurfaceBase::SetFontSheetLayout(unsigned char numrows, short rowchars)
{
	fontinfo.rowchars = rowchars;
	fontinfo.numrows = numrows;
}

//Set font spacing between certain characters
void E2DSurfaceBase::SetFontKerning(WCHAR charleft, WCHAR charright, long kerning)
{
	if (fontkerning == NULL)
	{
		fontkerning = (short *)ClassEMemory::Alloc((32 * 3 + 2) * sizeof(short));
		if (fontkerning)
		{
			fontkerning[0] = 32;
			fontkerning[1] = 0;
		}
	}
	if (fontkerning)
	{
		unsigned long ctr;
		for (ctr = 0; ctr < (unsigned long)fontkerning[1]; ctr++)
			if (fontkerning[2 + ctr * 3] == charleft && fontkerning[2 + ctr * 3 + 1] == charright)
			{
				if (kerning == 0)
				{
					fontkerning[2 + ctr * 3] = 0;
					fontkerning[2 + ctr * 3 + 1] = 0;
				}
				else
					fontkerning[2 + ctr * 3 + 2] = (short)kerning;
				return;
			}
		for (ctr = 0; ctr < (unsigned long)fontkerning[0]; ctr++)
			if (ctr >= (unsigned long)fontkerning[1] || (fontkerning[2 + ctr * 3] == 0 && fontkerning[2 + ctr * 3 + 1] == 0))
			{
				if (kerning != 0)
				{
					fontkerning[2 + ctr * 3] = charleft;
					fontkerning[2 + ctr * 3 + 1] = charright;
					fontkerning[2 + ctr * 3 + 2] = (short)kerning;
					if (ctr + 1 > (unsigned long)fontkerning[1])
						fontkerning[1] = (short)(ctr + 1);
				}
				return;
			}
		unsigned long newcount = fontkerning[0] + 32;
		short *newkerning = (short *)ClassEMemory::ReAlloc(fontkerning, (newcount * 3 + 2) * sizeof(short));
		if (newkerning)
		{
			fontkerning = newkerning;
			fontkerning[0] = (short)newcount;
			for (ctr = 0; ctr < (unsigned long)fontkerning[0]; ctr++)
				if (ctr >= (unsigned long)fontkerning[1] || (fontkerning[2 + ctr * 3] == 0 && fontkerning[2 + ctr * 3 + 1] == 0))
				{
					if (kerning != 0)
					{
						fontkerning[2 + ctr * 3] = charleft;
						fontkerning[2 + ctr * 3 + 1] = charright;
						fontkerning[2 + ctr * 3 + 2] = (short)kerning;
						if (ctr + 1 > (unsigned long)fontkerning[1])
							fontkerning[1] = (short)(ctr + 1);
					}
					return;
				}
		}
	}
}

//Set default character to draw characters not present in fontset
void E2DSurfaceBase::SetFontDefaultChar(long index)
{
	fontinfo.defaultchar = index;
}


void E2DSurfaceBase::Reload(ClassEDisplay *dsp)
{
    // to be performed by subclasses
}

///////////////////////////////////////////////////////////////////
// E2DSurfaceBase: virtual loader callbacks                      //
///////////////////////////////////////////////////////////////////

ERESULT E2DSurfaceBase::LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata){ return(E_UNSUPPORTED); }
ERESULT E2DSurfaceBase::LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata){ return(E_UNSUPPORTED); }
ERESULT E2DSurfaceBase::LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata){ return(E_UNSUPPORTED); }
