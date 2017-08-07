// esurf_2d.cpp
// 2D Surface
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eraw.h"
#include "eloader.h"
#include "estdc.h"
#include "emath.h"
#include "ememory.h"
#include "efile.h"
#include "edisplay.h"
#include "econsole.h"

#include "core/edgecore.h"
#include "extra/verinfo.h"

///////////////////////////////////////////////////////////////////
// Internal includes                                             //
///////////////////////////////////////////////////////////////////

#include "extra/exmacro.h"
#include "extra/exrect.h"
#include "extra/exfonthdr.h"
#include "extra/exblit.h"
#include "extra/exblitfast.h" //Optimized drawing routines

//Edgerender types
#define ERDRF_BLTFAST              0
#define ERDRF_BLTFLIP              1

//Other definitions
#define E_EMULDIVBYTESC            6
#define MAX_PALETTE              256

//Structure containing userdata for font draw callback
typedef struct
{
	E2DSurfaceBase *source;
	E2DSurface *dest;
	unsigned long flags;
	unsigned long property;
	bool srcrgba;
}FONTDRAW_USERDATA_SURF2D;

//Prototype
void ClassEDisplay_PixelCopy(void *dst, void *src, long dhpitch, long dvpitch, long shpitch, long svpitch, unsigned long cw, unsigned long ch, unsigned long dw, unsigned long dh, unsigned long sw, unsigned long sh, unsigned char bitdepth);
bool EdgeGL_Use(void);

///////////////////////////////////////////////////////////////////
// E2DSurface: pure functions                                    //
///////////////////////////////////////////////////////////////////

//Return if the character is a space separator
bool inline E2DSurface_IsWhiteSpace(WCHAR chr)
{
	return(chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r' || chr == '\f' || chr == '\x0b');
}

//Callback function for font character writing
ERESULT E2DSurface_DrawFontFunc(long x, long y, const short *srcrc, void *userdata)
{
	FONTDRAW_USERDATA_SURF2D *info = (FONTDRAW_USERDATA_SURF2D *)userdata;
	RECT rc;
	SetRect(&rc, srcrc[0], srcrc[1], srcrc[2], srcrc[3]);
	if (info->srcrgba)
	{
		E2DSurfaceRGBA *surfrgba = (E2DSurfaceRGBA *)info->source;
		return(info->dest->BltFast(x, y, surfrgba, &rc, info->flags, info->property));
	}
	E2DSurface *surf = (E2DSurface *)info->source;
	return(info->dest->BltFast(x, y, surf, &rc, info->flags, info->property));
}

//Fixed point divide, check values and use proper shifting
long inline E2DSurface_FixedDivAutoPrec(long sv, long dv, const long prec)
{
	bool negsv = (sv < 0), negdv = (dv < 0);
	long shiftval, bitpos = E_EMULDIVBYTESC * 8 - 1;
	unsigned long ctr, eval = 0, result = 0;
	unsigned char svbits[E_EMULDIVBYTESC];
	if (negsv)
		sv = -sv;
	if (negdv)
		dv = -dv;
	for (ctr = 0; ctr < E_EMULDIVBYTESC; ctr++)
	{
		shiftval = (prec - (long)ctr * 8);
		if (shiftval < -31 || shiftval > 31)
			svbits[ctr] = 0;
		else if (shiftval < 0)
			svbits[ctr] = (unsigned char)(sv >> -shiftval);
		else
			svbits[ctr] = (unsigned char)(sv << shiftval);
	}
	while (bitpos >= 0)
	{
		eval = (eval << 1) | ((svbits[bitpos / 8] & (1 << (bitpos % 8))) >> (bitpos % 8));
		result <<= 1;
		if (eval >= (unsigned long)dv)
		{
			result |= 1;
			eval -= dv;
		}
		bitpos--;
	}
	shiftval = result;
	if (negsv ^ negdv)
		shiftval = -shiftval;
	return(shiftval);
}

///////////////////////////////////////////////////////////////////
// E2DSurface: private                                           //
///////////////////////////////////////////////////////////////////

//Convert a color reference to a native color
void inline E2DSurface::ConvertColor(COLORREF &col, unsigned char vmode)
{
	if (vmode != EDSP_INDEXED8 && vmode != EDSP_TRUE24_INV && vmode != EDSP_TRUE32_INV)
	{
		col = ((col & 0xFF0000) >> 16) | (col & 0xFF00) | ((col & 0xFF) << 16);
		if (vmode == EDSP_TRUE12)
			col = (((col & 0xFF0000) >> 12) & 3840) | (((col & 0xFF00) >> 8) & 240) | ((col & 0xFF) >> 4);
		else if (vmode == EDSP_TRUE15)
			col = (((col & 0xFF0000) >> 9) & 31744) | (((col & 0xFF00) >> 6) & 992) | ((col & 0xFF) >> 3);
		else if (vmode == EDSP_TRUE16)
			col = (((col & 0xFF0000) >> 8) & 63488) | (((col & 0xFF00) >> 5) & 2016) | ((col & 0xFF) >> 3);
	}
}

//Revert a color reference from a native color
void inline E2DSurface::RevertColor(COLORREF &col, unsigned char vmode)
{
	if (vmode != EDSP_INDEXED8 && vmode != EDSP_TRUE24_INV && vmode != EDSP_TRUE32_INV)
	{
		if (vmode == EDSP_TRUE12)
			col = ((col & 3840) << 12) | ((col & 240) << 8) | ((col & 15) << 4);
		else if (vmode == EDSP_TRUE15)
			col = ((col & 31744) << 9) | ((col & 992) << 6) | ((col & 31) << 3);
		else if (vmode == EDSP_TRUE16)
			col = ((col & 63488) << 8) | ((col & 2016) << 5) | ((col & 31) << 3);
		col = ((col & 0xFF0000) >> 16) | (col & 0xFF00) | ((col & 0xFF) << 16);
	}
}

//Dither a high bit color value depending on the index (for example: index = x + y)
void inline E2DSurface::DitherColor(COLORREF &col, unsigned long index)
{
	if (videomode == EDSP_TRUE12)
	{
		if ((col & 0xFF0000) < 0xF00000)
			col = (col & 0x00FFFF) | ((col & 0xF00000) + (edithertable16[((index & 15) << 4) + ((col >> 16) & 15)] << 16));
		if ((col & 0xFF00) < 0xF000)
			col = (col & 0xFF00FF) | ((col & 0xF000) + (edithertable16[((index & 15) << 4) + ((col >> 8) & 15)] << 8));
		if ((col & 0xFF) < 0xF0)
			col = (col & 0xFFFF00) | ((col & 0xF0) + (edithertable16[((index & 15) << 4) + (col & 15)]));
	}
	else if (videomode == EDSP_TRUE15)
	{
		if ((col & 0xFF0000) < 0xF80000)
			col = (col & 0x00FFFF) | ((col & 0xF80000) + (edithertable8[((index & 7) << 3) + ((col >> 16) & 7)] << 16));
		if ((col & 0xFF00) < 0xF800)
			col = (col & 0xFF00FF) | ((col & 0xF800) + (edithertable8[((index & 7) << 3) + ((col >> 8) & 7)] << 8));
		if ((col & 0xFF) < 0xF8)
			col = (col & 0xFFFF00) | ((col & 0xF8) + (edithertable8[((index & 7) << 3) + (col & 7)]));
	}
	else if (videomode == EDSP_TRUE16)
	{
		if ((col & 0xFF0000) < 0xF80000)
			col = (col & 0x00FFFF) | ((col & 0xF80000) + (edithertable8[((index & 7) << 3) + ((col >> 16) & 7)] << 16));
		if ((col & 0xFF00) < 0xFB00)
			col = (col & 0xFF00FF) | ((col & 0xFB00) + (edithertable4[((index & 3) << 2) + ((col >> 8) & 3)] << 8));
		if ((col & 0xFF) < 0xF8)
			col = (col & 0xFFFF00) | ((col & 0xF8) + (edithertable8[((index & 7) << 3) + (col & 7)]));
	}
}

//Check the color filter flags and properties for rectangle filling
bool inline E2DSurface::CheckFilterFill(COLORREF &col, long &flags, unsigned long &property)
{
	unsigned long opacprop = property;
	flags &= EFX_COLORFILTER;
	if (flags & EFX_COLORIZE)
		opacprop >>= 24;
	if ((flags & (EFX_SUB | EFX_ALPHA)) == (EFX_SUB | EFX_ALPHA))
		flags -= EFX_SUB | EFX_ALPHA;
	if (flags & EFX_COLORIZE)
	{
		if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
			EBCODE_MACRO_COLORIZE(col, col, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, (property & 0xFFFFFF), unsigned long)
		else if (flags & EFX_COLORFILL)
			col = property & 0xFFFFFF;
		else if (flags & EFX_TINT)
			col = EBCODE_MACRO_TINT(col, 0x7F7F7F, ((property >> 1) & 0x7F7F7F));//((col >> 1) & 0x7F7F7F) + ((property >> 1) & 0x7F7F7F);
		flags &= EFX_COLORFILTER - EFX_COLORIZE;
		property >>= 24;
	}
	if ((flags & EFX_OPACITY) && (flags & (EFX_SUB | EFX_ALPHA)))
	{
		col = EBCODE_MACRO_HPACITY(col, 0, 0xFF00FF, 0x00FF00, 8, opacprop);
		flags -= EFX_OPACITY;
	}
	if (flags & EFX_OPACITY)
	{
		if (opacprop > 254)
		{
			flags -= EFX_OPACITY;
			opacprop = 0;
		}
		else
		{
			if (videomode == EDSP_TRUE12)
				opacprop -= opacprop & 15;
			else if (videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
				opacprop -= opacprop & 7;
			if (opacprop <= 0)
				return(false);
		}
		if (flags & EFX_COLORIZE)
			property = (property & 0xFFFFFF) | (opacprop << 24);
	}
	return(true);
}

//Check the color filter flags and properties for rectangle filling
bool inline E2DSurface::CheckFilterBlt(long &flags, unsigned long &property)
{
	unsigned long opacprop = property;
	flags &= EFX_COLORFILTER;
	if (flags & EFX_COLORIZE)
		opacprop >>= 24;
	if ((flags & (EFX_SUB | EFX_ALPHA)) == (EFX_SUB | EFX_ALPHA))
		flags -= EFX_SUB | EFX_ALPHA;
	if (flags & EFX_OPACITY)
	{
		if (opacprop > 254)
		{
			flags -= EFX_OPACITY;
			opacprop = 0;
		}
		else
		{
			if (videomode == EDSP_TRUE12)
				opacprop -= opacprop & 15;
			else if (videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
				opacprop -= opacprop & 7;
			if (opacprop <= 0)
				return(false);
		}
		if (flags & EFX_COLORIZE)
			property = (property & 0xFFFFFF) | (opacprop << 24);
	}
	return(true);
}

//Check the color filter flags and properties for rectangle filling
bool inline E2DSurface::CheckFilterBlt(long &flags, unsigned long &colorize, unsigned char &opacity, unsigned long property)
{
	colorize = 0;
	flags &= EFX_COLORFILTER;
	if (flags & EFX_COLORIZE)
		opacity = (unsigned char)(property >> 24);
	else
		opacity = (unsigned char)property;
	if ((flags & (EFX_SUB | EFX_ALPHA)) == (EFX_SUB | EFX_ALPHA))
		flags -= EFX_SUB | EFX_ALPHA;
	if (flags & EFX_OPACITY)
	{
		if (opacity > 254)
		{
			flags -= EFX_OPACITY;
			opacity = 0;
		}
		else
		{
			if (videomode == EDSP_TRUE12)
				opacity &= 0xF0;
			else if (videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
				opacity &= 0xF8;
			if (opacity <= 0)
				return(false);
		}
	}
	if (flags & EFX_COLORIZE)
		colorize = (property & 0xFFFFFF);
	return(true);
}

//Scan a polygon line to determine the fill edges
void E2DSurface::ScanPolyLine(long x1, long y1, long x2, long y2)
{
	if (y1 == y2)
		return;
	if (y2 < y1)
	{
		long tmp;
		tmp = y1; y1 = y2; y2 = tmp;
		tmp = x2; x2 = x1; x1 = tmp;
	}
	long yctr;
	long delta_y = ((y2 - y1) >> 1) + 1;
	long delta_x = ((x2 - x1) << 9) / delta_y;
	int fix = 1024 - (y1 & 1023);
	x1 += (delta_x * fix) >> 10;
	for (yctr = (y1 >> 10); yctr < (y2 >> 10); yctr++)
	{
		if (yctr >= cliprc.top && yctr < cliprc.bottom)
		{
			if ((x1 >> 10) < xmin[yctr]) 
				xmin[yctr] = (short)(x1 >> 10);
			if ((x1 >> 10) - 1 > xmax[yctr]) 
				xmax[yctr] = (short)((x1 >> 10) - 1);
		}
		x1 += delta_x;
	}
}

//Scan polygon edges for texture mapping
void E2DSurface::ScanPolyLine(long x1, long y1, long x2, long y2, long u1, long v1, long u2, long v2)
{
	if (y1 == y2)
		return;
	if (y2 < y1)
	{
		long tmp;
		tmp = y1; y1 = y2; y2 = tmp;
		tmp = x2; x2 = x1; x1 = tmp;
		tmp = u1; u1 = u2; u2 = tmp;
		tmp = v1; v1 = v2; v2 = tmp;
	}
	long yctr;
	long delta_y = ((y2 - y1) >> 1) + 1;
	long delta_x = ((x2 - x1) << 9) / delta_y;
	long delta_u = ((u2 - u1) << 11) / delta_y;
	long delta_v = ((v2 - v1) << 11) / delta_y;
	int fix = 1024 - (y1 & 1023);
	u1 <<= 2;
	v1 <<= 2;
	u1 += (delta_u * fix) >> 10;
	v1 += (delta_v * fix) >> 10;
	x1 += (delta_x * fix) >> 10;
	for (yctr = (y1 >> 10); yctr < (y2 >> 10); yctr++)
	{
		if (yctr >= cliprc.top && yctr < cliprc.bottom)
		{
			if (x1 < ledge[yctr]) 
			{
				xmin[yctr] = (short)(x1 >> 10);
				ledge[yctr] = x1;
				ledge[sheight + yctr] = u1;
				ledge[(sheight << 1) + yctr] = v1;
			}
			if (x1 > redge[yctr]) 
			{
				xmax[yctr] = (short)((x1 >> 10) - 1);
				redge[yctr] = x1;
				redge[sheight + yctr] = u1;
				redge[(sheight << 1) + yctr] = v1;
			}
		}
		x1 += delta_x;
		u1 += delta_u, v1 += delta_v;
	}
}

//Scan polygon edges for perspective correct texture mapping
void E2DSurface::ScanPolyLinePz(long x1, long y1, long z1, long x2, long y2, long z2, long u1, long v1, long u2, long v2)
{
	if (y1 == y2)
		return;
	if (y2 < y1)
	{
		long tmp;
		tmp = x2; x2 = x1; x1 = tmp;
		tmp = y1; y1 = y2; y2 = tmp;
		tmp = z1; z1 = z2; z2 = tmp;
		tmp = u1; u1 = u2; u2 = tmp;
		tmp = v1; v1 = v2; v2 = tmp;
	}
	u1 >>= 2;
	u2 >>= 2;
	v1 >>= 2;
	v2 >>= 2;
	long dx     = x2 - x1;
	long dy     = y2 - y1;
	long idy    = (dy >> 10) + 1;
	long dxdy   = dx / idy;
	long ooz1   = 0x7fffffff / z1;
	long ooz2   = 0x7fffffff / z2;
	long doozdy = (ooz2 - ooz1) / idy;
	long u1oz1  = E2DSurface_FixedDivAutoPrec(u1, z1, 20);
	long u2oz2  = E2DSurface_FixedDivAutoPrec(u2, z2, 20);
	long v1oz1  = E2DSurface_FixedDivAutoPrec(v1, z1, 20);
	long v2oz2  = E2DSurface_FixedDivAutoPrec(v2, z2, 20);
	long duozdy = (u2oz2 - u1oz1) / idy;
	long dvozdy = (v2oz2 - v1oz1) / idy;
	for (long yctr = (y1 >> 10), ye = y2 >> 10; yctr <= ye; yctr++)
	{
		if (yctr >= cliprc.top && yctr < cliprc.bottom)
		{
			if (x1 < ledge[yctr])
			{
				xmin[yctr] = (short)(x1 >> 10);
				ledge[yctr] = x1;
				ledge[sheight + yctr] = u1oz1;
				ledge[(sheight << 1) + yctr] = v1oz1;
				ledge[(sheight * 3) + yctr] = ooz1;
			}
			if (x1 > redge[yctr])
			{
				xmax[yctr] = (short)(x1 >> 10);
				redge[yctr] = x1;
				redge[sheight + yctr] = u1oz1;
				redge[(sheight << 1) + yctr] = v1oz1;
				redge[(sheight * 3) + yctr] = ooz1;
			}
		}
		x1    += dxdy;
		ooz1  += doozdy;
		u1oz1 += duozdy,
		v1oz1 += dvozdy;
	}
}

//Scan polygon edges for gouraud shading
void E2DSurface::ScanPolyLine(long x1, long y1, long x2, long y2, long r1, long g1, long b1, long r2, long g2, long b2)
{
	if (y1 == y2)
		return;
	if (y2 < y1)
	{
		long tmp;
		tmp = y1; y1 = y2; y2 = tmp;
		tmp = x2; x2 = x1; x1 = tmp;
		tmp = r1; r1 = r2; r2 = tmp;
		tmp = g1; g1 = g2; g2 = tmp;
		tmp = b1; b1 = b2; b2 = tmp;
	}
	long yctr;
	long delta_y = ((y2 - y1) >> 1) + 1;
	long delta_x = ((x2 - x1) << 9) / delta_y;
	long delta_r = ((r2 - r1) << 9) / delta_y;
	long delta_g = ((g2 - g1) << 9) / delta_y;
	long delta_b = ((b2 - b1) << 9) / delta_y;
	int fix = 1024 - (y1 & 1023);
	r1 += (delta_r * fix) >> 10;
	g1 += (delta_g * fix) >> 10;
	b1 += (delta_b * fix) >> 10;
	x1 += (delta_x * fix) >> 10;
	for (yctr = (y1 >> 10); yctr < (y2 >> 10); yctr++)
	{
		if (yctr >= cliprc.top && yctr < cliprc.bottom)
		{
			if (x1 < ledge[yctr]) 
			{
				xmin[yctr] = (short)(x1 >> 10);
				ledge[yctr] = x1;
				ledge[sheight + yctr] = r1;
				ledge[(sheight << 1) + yctr] = g1;
				ledge[(sheight * 3) + yctr] = b1;
			}
			if (x1 > redge[yctr]) 
			{
				xmax[yctr] = (short)((x1 >> 10) - 1);
				redge[yctr] = x1;
				redge[sheight + yctr] = r1;
				redge[(sheight << 1) + yctr] = g1;
				redge[(sheight * 3) + yctr] = b1;
			}
		}
		x1 += delta_x;
		r1 += delta_r, g1 += delta_g, b1 += delta_b;
	}
}

//Get a pixel from non-virual position x, y without error checking
COLORREF E2DSurface::GetPixelRaw(long x, long y, unsigned char *memptr)
{
	COLORREF col = 0;
	if (bitwidth == 8)
		col = memptr[y * vpitch + x];
	else if (bitwidth == 16)
		col = *(unsigned short *)(&memptr[y * vpitch + x * 2]);
	else if (bitwidth == 24)
		ClassEMemory::Copy(&col, &memptr[y * vpitch + x * 3], 3);
	else if (bitwidth == 32)
		col = *(unsigned EINT32 *)(&memptr[y * vpitch + x * 4]);
	RevertColor(col, videomode);
	return(col);
}

//Put a pixel at non-virual position x, y without error checking
void E2DSurface::PutPixelRaw(long x, long y, COLORREF col, long flags, unsigned long property, unsigned char *memptr)
{
	if (!CheckFilterFill(col, flags, property))
		return;
	if (flags & EFX_OPACITY)
	{
		COLORREF tc;
		unsigned long bc;
		bc = 256 - property;
		tc = GetPixelRaw(x, y, memptr);
		tc = (((((tc & 0xFF0000) * bc) >> 8) & 0xFF0000) | ((((tc & 0xFF00) * bc) >> 8) & 0xFF00) | (((tc & 0xFF) * bc) >> 8));
		col = tc + (((((col & 0xFF0000) * property) >> 8) & 0xFF0000) | ((((col & 0xFF00) * property) >> 8) & 0xFF00) | (((col & 0xFF) * property) >> 8));
	}
	else if (flags & EFX_ALPHA)
	{
		COLORREF tc;
		tc = GetPixelRaw(x, y, memptr);
		if (((tc & 0xFF)     + (col & 0xFF))     & 0x100)     tc |= 0xFF;     else tc += col & 0xFF;
		if (((tc & 0xFF00)   + (col & 0xFF00))   & 0x10000)   tc |= 0xFF00;   else tc += col & 0xFF00;
		if (((tc & 0xFF0000) + (col & 0xFF0000)) & 0x1000000) tc |= 0xFF0000; else tc += col & 0xFF0000;
		col = tc;
	}
	else if (flags & EFX_SUB)
	{
		COLORREF tc;
		tc = GetPixelRaw(x, y, memptr);
		if ((col & 0xFF)     > (tc & 0xFF))     tc &= 0xFFFF00; else tc -= col & 0xFF;
		if ((col & 0xFF00)   > (tc & 0xFF00))   tc &= 0xFF00FF; else tc -= col & 0xFF00;
		if ((col & 0xFF0000) > (tc & 0xFF0000)) tc &= 0xFFFF;   else tc -= col & 0xFF0000;
		col = tc;
	}
	ConvertColor(col, videomode);
	if (bitwidth == 8)
		memptr[y * vpitch + x] = (unsigned char)col;
	else if (bitwidth == 16)
		*(unsigned short *)(&memptr[y * vpitch + x * 2]) = (unsigned short)col;
	else if (bitwidth == 24)
		ClassEMemory::Copy(&memptr[y * vpitch + x * 3], &col, 3);
	else if (bitwidth == 32)
		*(unsigned EINT32 *)(&memptr[y * vpitch + x * 4]) = col;
}

//Private getpixel function without virtual positioning
COLORREF E2DSurface::GetPixelV(long x, long y)
{
	if (x < 0 || y < 0 || x >= (long)virtualwidth || y >= (long)sheight)
		return(RGB(0, 0, 0));
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return(RGB(0, 0, 0));
	COLORREF col = GetPixelRaw(x, y, memptr);
	Unlock();
	return(col);
}

//Private rectangle fill function for using virtual positioning or not
void E2DSurface::FillRectV(RECT *rc, COLORREF col, long flags, unsigned long property, bool pconv)
{
	if (currenttype % 16 == EST_NONE)
		return;
	long colorflags = flags;
	if (!CheckFilterFill(col, colorflags, property))
		return;
	flags = flags - (flags & EFX_COLORFILTER) + (colorflags & EFX_COLORFILTER);
	RECT nrc;
	if (rc != NULL)
	{
		ClassEMemory::Copy(&nrc, rc, sizeof(RECT));
		if (pconv)
			ecd->dsp->VirtualRect(&nrc, virtualwidth, sheight);
		if (!ClipRect(&nrc))
		{
			Unlock();
			return;
		}
	}
	else
		SetRect(&nrc, cliprc.left, cliprc.top, cliprc.right, cliprc.bottom);
	if (colorflags & EFX_OPACITY)
		col = (((((col & 0xFF0000) * property) >> 8) & 0xFF0000) | ((((col & 0xFF00) * property) >> 8) & 0xFF00) | (((col & 0xFF) * property) >> 8));
	ConvertColor(col, videomode);
	if (!(colorflags & (EFX_OPACITY | EFX_ALPHA | EFX_SUB)))
		colorflags = 0;
	if (nativesurf != NULL && locked == 0)
		if ((ecd->dispcfg->dspnative && !(flags & E2D_PREVENTNATIVE)) || (flags & E2D_ALLOWNATIVE))
		{
			long n_rect[] = { nrc.left, nrc.top, nrc.right, nrc.bottom };
			if (colorflags == 0)
				if (ecDspNativeFillRect(ecd->dsp->objdsp, nativesurf, n_rect, col))
					return;
		}
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return;
	if (colorflags == 0)
	{
		long yctr;
		if (nrc.left == 0 && nrc.right == vpitch / (signed char)(bitwidth / 8))
		{
			if (bitwidth == 8)
				ClassEMemory::Set(&memptr[nrc.top * vpitch], (unsigned char)col, swidth * (nrc.bottom - nrc.top));
			else if (bitwidth == 16)
				ecd->mem->Set16(&memptr[nrc.top * vpitch], col, swidth * (nrc.bottom - nrc.top));
			else if (bitwidth == 24)
				ecd->mem->Set24(&memptr[nrc.top * vpitch], col, swidth * (nrc.bottom - nrc.top));
			else if (bitwidth == 32)
				ecd->mem->Set32(&memptr[nrc.top * vpitch], col, swidth * (nrc.bottom - nrc.top));
		}
		else if (nrc.left > 0 || nrc.right < (long)swidth)
		{
			yctr = nrc.top;
			if (bitwidth == 8)
				while(yctr < nrc.bottom){ ClassEMemory::Set(&memptr[yctr * vpitch + nrc.left], (unsigned char)col, nrc.right - nrc.left); yctr++; }
			else if (bitwidth == 16)
				while(yctr < nrc.bottom){ ecd->mem->Set16(&memptr[yctr * vpitch + nrc.left * 2], col, nrc.right - nrc.left); yctr++; }
			else if (bitwidth == 24)
				while(yctr < nrc.bottom){ ecd->mem->Set24(&memptr[yctr * vpitch + nrc.left * 3], col, nrc.right - nrc.left); yctr++; }
			else if (bitwidth == 32)
				while(yctr < nrc.bottom){ ecd->mem->Set32(&memptr[yctr * vpitch + nrc.left * 4], col, nrc.right - nrc.left); yctr++; }
		}
		Unlock();
		return;
	}
	if (colorflags & EFX_OPACITY)
		ExRctOpac(&nrc, memptr, swidth, bitwidth, videomode, col, property);
	else if (colorflags & EFX_ALPHA)
		ExRctAlpha(&nrc, memptr, swidth, bitwidth, videomode, col);
	else if (colorflags & EFX_SUB)
		ExRctSub(&nrc, memptr, swidth, bitwidth, videomode, col);
	Unlock();
}

//Private pixel row function for using virtual positioning or not
void E2DSurface::PutPixelRowV(long x, long y, unsigned long width, COLORREF col, long flags, unsigned long property, bool pconv)
{
	if (x < 0)
	{
		width += x;
		x = 0;
	}
	RECT rc;
	SetRect(&rc, x, y, x + width, y + 1);
	FillRectV(&rc, col, flags, property, pconv);
}

//Private function to fill a polygon with a texture
void E2DSurface::TexturePolyV(long x1, long y1, long u1, long v1, long x2, long y2, long u2, long v2, long x3, long y3, long u3, long v3, E2DSurface *source, long flags, unsigned long property)
{
	if (!this->CheckFilterBlt(flags, property))
		return;
	unsigned char *memptr1 = Lock(), *memptr2 = source->Lock();
	if (memptr1 == NULL || memptr2 == NULL)
	{
		if (memptr1 != NULL)
			Unlock();
		if (memptr2 != NULL)
			source->Unlock();
		return;
	}
	long xp[3] = { x1, x2, x3 };
	long yp[3] = { y1, y2, y3 };
	long xctr, xl, xr, yt, yb;
	for (xctr = 0; xctr < 3; xctr++)
		ecd->dsp->VirtualPos(xp[xctr], yp[xctr], virtualwidth << 10, sheight << 10);
	ecd->dsp->VirtualPos(u1, v1, source->virtualwidth << 10, source->sheight << 10);
	ecd->dsp->VirtualPos(u2, v2, source->virtualwidth << 10, source->sheight << 10);
	ecd->dsp->VirtualPos(u3, v3, source->virtualwidth << 10, source->sheight << 10);
	xl = xp[0], xr = xp[0], yt = yp[0], yb = yp[0];
	for (xctr = 1; xctr < 3; xctr++)
	{
		if (xp[xctr] < xl)
			xl = xp[xctr];
		if (xp[xctr] > xr)
			xr = xp[xctr];
		if (yp[xctr] < yt)
			yt = yp[xctr];
		if (yp[xctr] > yb)
			yb = yp[xctr];
	}
	xl >>= 10;
	xr = (xr >> 10) - 1;
	yt >>= 10;
	yb = (yb >> 10) - 1;
	if (yb < 0 || xr < 0 || yt >= (long)sheight || xl >= (long)virtualwidth)
	{
		Unlock();
		source->Unlock();
		return;
	}
	if (yt < cliprc.top)
		yt = cliprc.top;
	if (yb >= (long)cliprc.bottom)
		yb = cliprc.bottom - 1;
	ecd->mem->Set32(&ledge[yt], virtualwidth << 10, yb - yt + 1);
	ecd->mem->Set32(&redge[yt], -1           << 10, yb - yt + 1);
	ecd->mem->Set16(&xmin[yt],  virtualwidth,   yb - yt + 1);
	ecd->mem->Set16(&xmax[yt],  (unsigned)(-1), yb - yt + 1);
	ScanPolyLine(xp[0], yp[0], xp[1], yp[1], u1, v1, u2, v2);
	ScanPolyLine(xp[1], yp[1], xp[2], yp[2], u2, v2, u3, v3);
	ScanPolyLine(xp[2], yp[2], xp[0], yp[0], u3, v3, u1, v1);
	for (xctr = yt; xctr <= yb; xctr++)
	{
		if (xmin[xctr] < cliprc.right && xmax[xctr] >= cliprc.left)
		{
			if (xmin[xctr] < cliprc.left)
				xmin[xctr] = (short)cliprc.left;
			if (xmax[xctr] >= cliprc.right)
				xmax[xctr] = (short)(cliprc.right - 1);
		}
		else
			xmax[xctr] = -1;
	}
	if (flags & EFX_COLOREFFECT)
	{
		if (flags & EFX_COLORIZE)
		{
			COLORREF col;
			if ((flags & EFX_COLORIZE) == EFX_TINT)
				col = (property >> 1) & 0x7F7F7F;
			else
				col = property & 0xFFFFFF;
			ConvertColor(col, videomode);
			ExRsBltGeneric(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode, flags, source->colorkey, property >> 24, col);
		}
		else
			ExRsBltGeneric(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode, flags, source->colorkey, property, 0);
	}
	else if (flags & EFX_COLORKEY)
		ExRsBltCk(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode, source->colorkey);
	else
		ExRsBlt(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode);
	Unlock();
	source->Unlock();
}

//Texture polygon (perspective correct)
void E2DSurface::TexturePolyV(long x1, long y1, long w1, long u1, long v1, long x2, long y2, long w2, long u2, long v2, long x3, long y3, long w3, long u3, long v3, E2DSurface *source, long flags, unsigned long property)
{
	if (!this->CheckFilterBlt(flags, property))
		return;
	unsigned char *memptr1 = Lock(), *memptr2 = source->Lock();
	if (memptr1 == NULL || memptr2 == NULL)
	{
		if (memptr1 != NULL)
			Unlock();
		if (memptr2 != NULL)
			source->Unlock();
		return;
	}
	long xp[3] = { x1, x2, x3 };
	long yp[3] = { y1, y2, y3 };
	long xctr, xl, xr, yt, yb;
	for (xctr = 0; xctr < 3; xctr++)
		ecd->dsp->VirtualPos(xp[xctr], yp[xctr], virtualwidth << 10, sheight << 10);
	ecd->dsp->VirtualPos(u1, v1, source->virtualwidth << 10, source->sheight << 10);
	ecd->dsp->VirtualPos(u2, v2, source->virtualwidth << 10, source->sheight << 10);
	ecd->dsp->VirtualPos(u3, v3, source->virtualwidth << 10, source->sheight << 10);
	xl = xp[0], xr = xp[0], yt = yp[0], yb = yp[0];
	for (xctr = 1; xctr < 3; xctr++)
	{
		if (xp[xctr] < xl)
			xl = xp[xctr];
		if (xp[xctr] > xr)
			xr = xp[xctr];
		if (yp[xctr] < yt)
			yt = yp[xctr];
		if (yp[xctr] > yb)
			yb = yp[xctr];
	}
	xl >>= 10;
	xr = (xr >> 10) - 1;
	yt >>= 10;
	yb = (yb >> 10) - 1;
	if (yb < 0 || xr < 0 || yt >= (long)sheight || xl >= (long)virtualwidth)
	{
		Unlock();
		source->Unlock();
		return;
	}
	if (yt < cliprc.top)
		yt = cliprc.top;
	if (yb >= (long)cliprc.bottom)
		yb = cliprc.bottom - 1;
	ecd->mem->Set32(&ledge[yt], virtualwidth << 10, yb - yt + 1);
	ecd->mem->Set32(&redge[yt], -1           << 10, yb - yt + 1);
	ecd->mem->Set16(&xmin[yt],  virtualwidth,   yb - yt + 1);
	ecd->mem->Set16(&xmax[yt],  (unsigned)(-1), yb - yt + 1);
	ScanPolyLinePz(xp[0], yp[0], w1, xp[1], yp[1], w2, u1, v1, u2, v2);
	ScanPolyLinePz(xp[1], yp[1], w2, xp[2], yp[2], w3, u2, v2, u3, v3);
	ScanPolyLinePz(xp[2], yp[2], w3, xp[0], yp[0], w1, u3, v3, u1, v1);
	for (xctr = yt; xctr <= yb; xctr++)
	{
		if (xmin[xctr] < cliprc.right && xmax[xctr] >= cliprc.left)
		{
			if (xmin[xctr] < cliprc.left)
				xmin[xctr] = (short)cliprc.left;
			if (xmax[xctr] >= cliprc.right)
				xmax[xctr] = (short)(cliprc.right - 1);
		}
		else
			xmax[xctr] = -1;
	}
	if (flags/* & EFX_COLOREFFECT*/)
	{
		if (flags & EFX_COLORIZE)
		{
			COLORREF col;
			if ((flags & EFX_COLORIZE) == EFX_TINT)
				col = (property >> 1) & 0x7F7F7F;
			else
				col = property & 0xFFFFFF;
			ConvertColor(col, videomode);
			ExRsPerspBltGeneric(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode, flags, source->colorkey, property >> 24, col);
		}
		else
			ExRsPerspBltGeneric(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode, flags, source->colorkey, property, 0);
	}
	//else if (flags & EFX_COLORKEY)
	//	ExRsBltCk(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode, source->colorkey);
	else
		ExRsPerspBlt(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode);
	Unlock();
	source->Unlock();
}

//Private function to fill a polygon with a solid color + filters
void E2DSurface::FillPolyV(long x1, long y1, long x2, long y2, long x3, long y3, COLORREF col, long flags, unsigned long property)
{
	if (Lock() == NULL)
		return;
	long xp[3] = { x1, x2, x3 };
	long yp[3] = { y1, y2, y3 };
	long yctr, xl, xr, yt, yb;
	for (yctr = 0; yctr < 3; yctr++)
		ecd->dsp->VirtualPos(xp[yctr], yp[yctr], virtualwidth << 10, sheight << 10);
	xl = xp[0], xr = xp[0], yt = yp[0], yb = yp[0];
	for (yctr = 1; yctr < 3; yctr++)
	{
		if (xp[yctr] < xl)
			xl = xp[yctr];
		if (xp[yctr] > xr)
			xr = xp[yctr];
		if (yp[yctr] < yt)
			yt = yp[yctr];
		if (yp[yctr] > yb)
			yb = yp[yctr];
	}
	xl >>= 10;
	xr = (xr >> 10) - 1;
	yt >>= 10;
	yb = (yb >> 10) - 1;
	if (yb < 0 || xr < 0 || yt >= (long)sheight || xl >= (long)virtualwidth)
	{
		Unlock();
		return;
	}
	if (yt < cliprc.top)
		yt = cliprc.top;
	if (yb >= (long)cliprc.bottom)
		yb = cliprc.bottom - 1;
	ecd->mem->Set16(&xmin[yt], virtualwidth,   yb - yt + 1);
	ecd->mem->Set16(&xmax[yt], (unsigned)(-1), yb - yt + 1);
	for (yctr = 0; yctr < 3; yctr++)
		ScanPolyLine(xp[yctr], yp[yctr], xp[(yctr + 1) % 3], yp[(yctr + 1) % 3]);
	for (yctr = yt; yctr <= yb; yctr++)
	{
		if (xmin[yctr] < cliprc.right && xmax[yctr] >= cliprc.left)
		{
			if (xmin[yctr] < cliprc.left)
				xmin[yctr] = (short)cliprc.left;
			if (xmax[yctr] >= cliprc.right)
				xmax[yctr] = (short)(cliprc.right - 1);
		}
		else
			xmax[yctr] = -1;
	}
	for (yctr = yt; yctr < yb + 1; yctr++)
		if (xmax[yctr] >= 0)
			PutPixelRowV(xmin[yctr], yctr, xmax[yctr] - xmin[yctr] + 1, col, flags, property, false);
	Unlock();
}

//Private function to fill a gradient polygon
void E2DSurface::GradientPolyV(long x1, long y1, COLORREF col1, long x2, long y2, COLORREF col2, long x3, long y3, COLORREF col3, long flags)
{
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return;
	long xp[3] = { x1, x2, x3 };
	long yp[3] = { y1, y2, y3 };
	long yctr, xl, xr, yt, yb;
	for (yctr = 0; yctr < 3; yctr++)
		ecd->dsp->VirtualPos(xp[yctr], yp[yctr], virtualwidth << 10, sheight << 10);
	xl = xp[0], xr = xp[0], yt = yp[0], yb = yp[0];
	for (yctr = 1; yctr < 3; yctr++)
	{
		if (xp[yctr] < xl)
			xl = xp[yctr];
		if (xp[yctr] > xr)
			xr = xp[yctr];
		if (yp[yctr] < yt)
			yt = yp[yctr];
		if (yp[yctr] > yb)
			yb = yp[yctr];
	}
	xl >>= 10;
	xr = (xr >> 10) - 1;
	yt >>= 10;
	yb = (yb >> 10) - 1;
	if (yb < 0 || xr < 0 || yt >= (long)sheight || xl >= (long)virtualwidth)
	{
		Unlock();
		return;
	}
	if (yt < cliprc.top)
		yt = cliprc.top;
	if (yb >= (long)cliprc.bottom)
		yb = cliprc.bottom - 1;
	ecd->mem->Set32(&ledge[yt], virtualwidth << 10, yb - yt + 1);
	ecd->mem->Set32(&redge[yt], -1           << 10, yb - yt + 1);
	ecd->mem->Set16(&xmin[yt],  virtualwidth,   yb - yt + 1);
	ecd->mem->Set16(&xmax[yt],  (unsigned)(-1), yb - yt + 1);
	ScanPolyLine(xp[0], yp[0], xp[1], yp[1], (col1 & 0xFF) << 10, (col1 & 0xFF00) << 2, (col1 & 0xFF0000) >> 6, (col2 & 0xFF) << 10, (col2 & 0xFF00) << 2, (col2 & 0xFF0000) >> 6);
	ScanPolyLine(xp[1], yp[1], xp[2], yp[2], (col2 & 0xFF) << 10, (col2 & 0xFF00) << 2, (col2 & 0xFF0000) >> 6, (col3 & 0xFF) << 10, (col3 & 0xFF00) << 2, (col3 & 0xFF0000) >> 6);
	ScanPolyLine(xp[2], yp[2], xp[0], yp[0], (col3 & 0xFF) << 10, (col3 & 0xFF00) << 2, (col3 & 0xFF0000) >> 6, (col1 & 0xFF) << 10, (col1 & 0xFF00) << 2, (col1 & 0xFF0000) >> 6);
	for (yctr = yt; yctr <= yb; yctr++)
	{
		if (xmin[yctr] < cliprc.right && xmax[yctr] >= cliprc.left)
		{
			if (xmin[yctr] < cliprc.left)
				xmin[yctr] = (short)cliprc.left;
			if (xmax[yctr] >= cliprc.right)
				xmax[yctr] = (short)(cliprc.right - 1);
		}
		else
			xmax[yctr] = -1;
	}
	if (flags & EFX_DITHER)
		ExRsBltGradientDith(memptr, xmin, xmax, ledge, redge, swidth, sheight, yt, yb, cliprc.left, bitwidth, videomode);
	else
		ExRsBltGradient(memptr, xmin, xmax, ledge, redge, swidth, sheight, yt, yb, cliprc.left, bitwidth, videomode);
	Unlock();
}

//Blit one surface to another with a possibility to rotate 90, 180 and 270 degrees and flip
ERESULT E2DSurface::BltFlip(long x, long y, E2DSurface *source, RECT *rc, unsigned short rotation, unsigned char flipflags, long flags, unsigned long property)
{
	if (rotation % 1024 != 0)
		return(E_INVALIDPARAM);
	long colorflags = flags;
	COLORREF colorize;
	unsigned char opacity;
	if (!this->CheckFilterBlt(colorflags, colorize, opacity, property))
		return(E_NOACTION);
	dirty = true;
	rotation %= 4096;
	flags = flags - (flags & EFX_COLORFILTER) + (colorflags & EFX_COLORFILTER);
	if ((colorflags & (EFX_COLORIZE | EFX_COLORKEY)) == EFX_COLORFILL)
	{
		RECT rrc;
		if (rc == NULL)
		{
			if (rotation % 2048 == 0)
				SetRect(&rrc, x, y, x + source->GetWidth(), y + source->GetHeight());
			else
				SetRect(&rrc, x, y, x + source->GetHeight(), y + source->GetWidth());
		}
		else
		{
			if (rotation % 2048 == 0)
				SetRect(&rrc, x, y, x + rc->right - rc->left, y + rc->bottom - rc->top);
			else
				SetRect(&rrc, x, y, x + rc->bottom - rc->top, y + rc->right - rc->left);
		}
		FillRect(&rrc, colorize, flags - EFX_COLORFILL, opacity);
		return(E_OK);
	}
	ERESULT result;
	RECT nrc;
	result = GetBltFlipPositions(x, y, &nrc, rc, source, rotation, flipflags);
	if (result != E_OK)
		return(result);
	long w = nrc.right - nrc.left, h = nrc.bottom - nrc.top;
	unsigned char *memptr1 = Lock(), *memptr2 = source->Lock();
	if (memptr1 == NULL || memptr2 == NULL)
	{
		if (memptr1 != NULL)
			Unlock();
		if (memptr2 != NULL)
			source->Unlock();
		return(E_LOCKFAILED);
	}
	if (videomode != source->videomode)
		return(E_UNSUPPORTED);
	long xadd = bitwidth / 8, yadd = source->vpitch;
	memptr1 = &memptr1[y * vpitch + x * bitwidth / 8];
	memptr2 = &memptr2[nrc.top * source->vpitch + nrc.left * bitwidth / 8];
	if (rotation % 2048 != 0)
	{
		w = nrc.bottom - nrc.top;
		h = nrc.right - nrc.left;
	}
	if (rotation == 1024)
	{
		xadd = -((long)(source->vpitch));
		yadd = bitwidth / 8;
		memptr2 -= xadd * (w - 1);
	}
	else if (rotation == 2048)
	{
		xadd = -((long)(bitwidth / 8));
		yadd = -((long)(source->vpitch));
		memptr2 -= xadd * (w - 1) + yadd * (h - 1);
	}
	else if (rotation == 3072)
	{
		xadd = source->vpitch;
		yadd = -((long)(bitwidth / 8));
		memptr2 -= yadd * (h - 1);
	}
	if (flipflags & EFLIP_HORIZONTAL)
	{
		if (rotation % 2048 == 0)
		{
			xadd = -xadd;
			memptr2 -= xadd * (w - 1);
		}
		else
		{
			yadd = -yadd;
			memptr2 -= yadd * (h - 1);
		}
	}
	if (flipflags & EFLIP_VERTICAL)
	{
		if (rotation % 2048 == 0)
		{
			yadd = -yadd;
			memptr2 -= yadd * (h - 1);
		}
		else
		{
			xadd = -xadd;
			memptr2 -= xadd * (w - 1);
		}
	}
	xadd /= bitwidth / 8; yadd /= bitwidth / 8;
	if (colorflags & EFX_OPACITY)
	{
		if (opacity == 128 || opacity == 192 || opacity == 224 || opacity == 240)
			colorflags = ((colorflags & (EFX_COLORFILTER - EFX_OPACITY)) | EFX_OPACITY_OVH);
		else if (opacity == 64 || opacity == 32 || opacity == 16)
			colorflags = ((colorflags & (EFX_COLORFILTER - EFX_OPACITY)) | EFX_OPACITY_OVL);
	}
	if ((flags & EFX_COLORIZE) == EFX_TINT && videomode != EDSP_INDEXED8)
		colorize = (colorize >> 1) & 0x7F7F7F;
	ConvertColor(colorize, videomode);
	if (videomode == EDSP_INDEXED8)
		ERdrBltFlip8(xadd, yadd, w, h, memptr1, memptr2, swidth * bitwidth / 8, source->swidth * bitwidth / 8, source->colorkey, colorize, opacity, &ecd->dsp->edgerender[ERDRF_BLTFLIP * 256], colorflags);
	else if (videomode == EDSP_TRUE12)
		ERdrBltFlip12(xadd, yadd, w, h, memptr1, memptr2, swidth * bitwidth / 8, source->swidth * bitwidth / 8, source->colorkey, colorize, opacity, &ecd->dsp->edgerender[ERDRF_BLTFLIP * 256], colorflags);
	else if (videomode == EDSP_TRUE16)
		ERdrBltFlip16(xadd, yadd, w, h, memptr1, memptr2, swidth * bitwidth / 8, source->swidth * bitwidth / 8, source->colorkey, colorize, opacity, &ecd->dsp->edgerender[ERDRF_BLTFLIP * 256], colorflags);
	else if (videomode == EDSP_TRUE32 || videomode == EDSP_TRUE32_INV)
		ERdrBltFlip32(xadd, yadd, w, h, memptr1, memptr2, swidth * bitwidth / 8, source->swidth * bitwidth / 8, source->colorkey, colorize, opacity, &ecd->dsp->edgerender[ERDRF_BLTFLIP * 256], colorflags);
	Unlock();
	source->Unlock();
	return(E_OK);
}

//Link the display module to the surface
void E2DSurface::LinkECD(ECD *pecd){ ecd = pecd; }

//Load callback: Create surface
ERESULT E2DSurface::LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata)
{
	ERESULT er;
	if (ldrdata->compressedformat)
	{
		Free();
		texformat = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(ldrdata->compressedformat) + 1);
		if (texformat)
			ClassEStd::StrCpy(texformat, ldrdata->compressedformat);
		if (ldrdata->width > 0 && ldrdata->height > 0)
		{
			swidth = virtualwidth = ldrdata->width;
			sheight = ldrdata->height;
			gdata = (unsigned char *)ecd->mem->AllocList(ldrdata->streamsize);
			if (gdata)
			{
				currenttype = EST_COMPRESSEDTEX;
				if (!ecd->dsp->HandleSurfList(this))
				{
					Free();
					return(E_NOMEMORY);
				}
				return(E_OK);
			}
		}
		return(E_ERROR);
	}
	er = Create(ldrdata->width, ldrdata->height, ldrdata->nativedisplaymode, ldrdata->createflags);
	if (er == E_OK)
		SetColorKey(E_RGBX(0, 0, 0));
	return(er);
}

//Load callback: Palette stream data
ERESULT E2DSurface::LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata)
{
	if (ldrdata->paletteindex >= MAX_PALETTE)
		return(E_NOACTION);
	unsigned long tocopy = ldrdata->streamsize / sizeof(COLORREF);
	if (ldrdata->paletteindex + tocopy > MAX_PALETTE)
		tocopy = MAX_PALETTE - ldrdata->paletteindex;
	ClassEMemory::Copy(&palette[ldrdata->paletteindex], ldrdata->streamdata, tocopy * sizeof(COLORREF));
	ldrdata->paletteindex += tocopy;
	FULL_FREE(indexlookup);
	return(E_OK);
}

//Load callback: Pixel stream data
ERESULT E2DSurface::LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata)
{
	if (currenttype == EST_COMPRESSEDTEX)
	{
		ClassEMemory::Copy(&gdata[ldrdata->pixelindex], ldrdata->streamdata, ldrdata->streamsize);
		ldrdata->pixelindex += ldrdata->streamsize;
		return(E_OK);
	}
	E2DSURFACEINFO info;
	unsigned char *memptr = Lock(&info);
	memptr += info.bufferoffset;
	if (memptr != NULL)
	{
		COLORREF col, a, altblack = E_RGBX(1, 1, 1);
		unsigned long ctr;
		if (videomode == EDSP_INDEXED8)
		{
			if (ldrdata->createflags & EST_MATCHPALETTE)
			{
				if (ldrdata->usepalette && indexlookup == NULL)
				{
					indexlookup = (unsigned char *)ClassEMemory::Alloc(256);
					if (indexlookup == NULL)
					{
						Unlock();
						return(E_NOMEMORY);
					}
					ecd->dsp->BuildPaletteLookupIndex(indexlookup, palette);
				}
				else if (!ldrdata->usepalette)
					if (!ecd->dsp->BuildPaletteLookup())
					{
						Unlock();
						return(E_ERROR);
					}
			}
			altblack = 0;
		}
		else if (videomode == EDSP_TRUE12)
			altblack = E_RGBX(16, 16, 16);
		else if (videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
			altblack = E_RGBX(8, 8, 8);
		if (ldrdata->usepalette)
		{
			unsigned char *rptr8 = (unsigned char *)ldrdata->streamdata;
			for (ctr = 0; ctr < ldrdata->streamsize; ctr++)
			{
				long putx = ldrdata->pixelindex % ldrdata->width, puty = ldrdata->pixelindex / ldrdata->width;
				if (videomode == EDSP_INDEXED8)
				{
					col = (*rptr8++);
					if (E_GETA(palette[col]) < 255)
						col = 0;
					if (ldrdata->createflags & EST_MATCHPALETTE)
						col = indexlookup[col];
				}
				else
				{
					col = palette[(*rptr8++)];
					a = E_GETA(col);
					col = E_GETRGB(col);
					if (ldrdata->createflags & EST_DITHER)
						DitherColor(col, putx + puty);
					if (a < 255)
						col = 0;
					else if (col == 0 && ldrdata->usetransparency)
						col = altblack;
				}
				ConvertColor(col, videomode);
				if (bitwidth == 8)
					memptr[puty * info.ypitch + putx * info.xpitch] = (unsigned char)col;
				else if (bitwidth == 16)
					*(unsigned short *)(&memptr[puty * info.ypitch + putx * info.xpitch]) = (unsigned short)col;
				else if (bitwidth == 24)
					ClassEMemory::Copy(&memptr[puty * info.ypitch + putx * info.xpitch], &col, 3);
				else if (bitwidth == 32)
					*(unsigned EINT32 *)(&memptr[puty * info.ypitch + putx * info.xpitch]) = col;
				ldrdata->pixelindex++;
			}
		}
		else
		{
			unsigned EINT32 *rptr32 = (unsigned EINT32 *)ldrdata->streamdata;
			for (ctr = 0; ctr < ldrdata->streamsize / 4; ctr++)
			{
				long putx = ldrdata->pixelindex % ldrdata->width, puty = ldrdata->pixelindex / ldrdata->width;
				if (videomode == EDSP_INDEXED8)
				{
					col = (*rptr32++);
					if (E_GETA(col) == 255)
					{
						if (ldrdata->createflags & EST_MATCHPALETTE)
						{
							col = ((col >> 9) & 31744) | ((col >> 6) & 992) | ((col >> 3) & 31);
							col = ecd->dsp->palettelookup[col];
						}
					}
					else
						col = 0;
				}
				else
				{
					col = (*rptr32++);
					a = E_GETA(col);
					col = E_GETRGB(col);
					if (ldrdata->createflags & EST_DITHER)
						DitherColor(col, putx + puty);
					if (a < 255)
						col = 0;
					else if (col == 0 && ldrdata->usetransparency)
						col = altblack;
				}
				ConvertColor(col, videomode);
				if (bitwidth == 8)
					memptr[puty * info.ypitch + putx * info.xpitch] = (unsigned char)col;
				else if (bitwidth == 16)
					*(unsigned short *)(&memptr[puty * info.ypitch + putx * info.xpitch]) = (unsigned short)col;
				else if (bitwidth == 24)
					ClassEMemory::Copy(&memptr[puty * info.ypitch + putx * info.xpitch], &col, 3);
				else if (bitwidth == 32)
					*(unsigned EINT32 *)(&memptr[puty * info.ypitch + putx * info.xpitch]) = col;
				ldrdata->pixelindex++;
			}
		}
		return(E_OK);
	}
	return(E_ERROR);
}

//Create a new surface from the display class
ERESULT E2DSurface::Create(unsigned long width, unsigned long height, unsigned char passmode, unsigned long usetype)
{
	if (usetype % 16 == EST_DEFAULT)
		usetype = usetype / 16 * 16 + ecd->dispcfg->surfacedefault;
	if (usetype % 16 > EST_HARDWARE)
		return(E_INVALIDPARAM);
	if (width == 0 || height == 0)
		return(E_INVALIDPARAM);
	ERESULT er;
	if (currenttype % 16 != EST_NONE && currenttype % 16 != EST_TEXTURE)
	{
		if (usetype == createtype && width == GetWidth() && height == GetHeight())
		{
			Clear();
			return(E_OK);
		}
		else
			Free();
	}
	createtype = usetype;
	if (currenttype % 16 != EST_NONE)
		Free();
	if (usetype % 16 == EST_NONE)
		return(E_OK);
	if (palette == NULL)
	{
		palette = (COLORREF *)ClassEMemory::Alloc(MAX_PALETTE * sizeof(COLORREF));
		if (palette == NULL)
			return(E_NOMEMORY);
	}
	if (((usetype & EST_DSPTYPE) >> 8) != EST_DEFAULT)
		videomode = (unsigned char)((usetype & EST_DSPTYPE) >> 8);
	else
		videomode = passmode;
	if (videomode != ecd->dsp->realvideomode)
		usetype = usetype / 16 * 16 + EST_SYSMEMORY;
	if (videomode == EDSP_INDEXED8)
		bitwidth = 8;
	else if (videomode == EDSP_TRUE12 || videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
		bitwidth = 16;
	else if (videomode == EDSP_TRUE24 || videomode == EDSP_TRUE24_INV)
		bitwidth = 24;
	else if (videomode == EDSP_TRUE32 || videomode == EDSP_TRUE32_INV)
		bitwidth = 32;
	else
		return(E_ERROR);
	if (ecd->dsp->switchxy)
	{
		swidth  = height;
		sheight = width;
	}
	else
	{
		swidth  = width;
		sheight = height;
	}
	virtualwidth = swidth;
	if (swidth % 4 != 0)
		swidth = swidth / 4 * 4 + 4;
	if (ecd->dsp->gldriver != NULL)
		usetype = usetype / 16 * 16 + EST_SYSMEMORY;
	if (usetype % 16 == EST_SYSTEMAPI || usetype % 16 == EST_HARDWARE)
	{
		ecErr errcode = 0;
		bool isbackbuffer = false;
		if (ecd->dsp->realvideomode != ecd->dsp->videomode)
			isbackbuffer = (this == &ecd->dsp->exbuffer);
		else
			isbackbuffer = (this == &ecd->dsp->buffer);
		#if defined(DEVICE_WIN32)
			if ((ecd->dsp->rotation != ecd->dsp->sysorientation && !ecd->dsp->prerotation) || (ecd->dsp->realxpitch != ecd->dsp->dispinfo.bitwidth / 8 || ecd->dsp->realypitch <= 0) || (ecd->dsp->buffer.currenttype % 16 != EST_SYSTEMAPI && ecd->dsp->buffer.IsCreated()))
				errcode = 1;
		#endif
		if (errcode == 0)
			nativesurf = ecDspCreateNativeSurface(ecd->dsp->objdsp, errcode, usetype % 16 == EST_HARDWARE, isbackbuffer, swidth, sheight);
		if (nativesurf == NULL)
			usetype = usetype / 16 * 16 + EST_SYSMEMORY;
	}
	if (usetype % 16 == EST_SYSMEMORY)
	{
		gdata = (unsigned char *)ecd->mem->AllocList(swidth * sheight * (bitwidth >> 3));
		if (gdata == NULL)
			return(E_NOMEMORY);
	}
	else if (usetype % 16 != EST_SYSTEMAPI && usetype % 16 != EST_HARDWARE)
		return(E_UNSUPPORTED);
	vpitch = swidth * (signed char)(bitwidth / 8);
	er = Init();
	if (er != E_OK)
		Free();
	currenttype = (unsigned char)(usetype % 16);
	Clear();
	if (!ecd->dsp->HandleSurfList(this))
	{
		Free();
		return(E_NOMEMORY);
	}
	return(er);
}

//Create a new surface by loading an image from memory
ERESULT E2DSurface::LoadImage(void *imgmem, unsigned long memsize, unsigned char passmode, unsigned long usetype)
{
	if (usetype % 16 > EST_HARDWARE)
		return(E_INVALIDPARAM);
	ERESULT er = E_UNSUPPORTED;
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_GRAPHICSLOADERS; ctr++)
		if (ecd->dsp->graphicsloader[ctr] != NULL)
		{
			er = ecd->dsp->graphicsloader[ctr]->Start(this, (unsigned char *)imgmem, memsize, passmode, usetype);
			if (er != E_UNSUPPORTED && er != E_NOFEATURE)
				break;
			if (er == E_OK)
				break;
		}
	if (locked)
	{
		locked = 1;
		Unlock();
	}
	return(er);
}

//Create a new surface by loading an image
ERESULT E2DSurface::LoadImage(WCHAR *imgfile, unsigned char passmode, unsigned long usetype)
{
	ClassEFile iml;
	ERESULT er;
	unsigned char *fdata = NULL;
	unsigned long fsize;
	unsigned char opentype = EFOF_READ | EFOF_NOEPKCHECK;
	if (usetype & EST_READONLYPATH)
		opentype |= EFOF_READONLYPATH;
	if (!iml.Open(imgfile, opentype))
		return(E_NOTEXISTS);
	fsize = iml.Size();
	if (fsize == 0)
	{
		iml.Close();
		return(E_NODATA);
	}
	fdata = (unsigned char *)ecd->mem->AllocList(fsize);
	if (fdata == NULL)
	{
		iml.Close();
		return(E_NOMEMORY);
	}
	iml.Read(fdata, fsize);
	iml.Close();
	er = LoadImage(fdata, fsize, passmode, usetype);
	FULL_FREE_L(fdata);
	return(er);
}

//Initializer after creating a surface in any way
ERESULT E2DSurface::Init(void)
{
	unsigned long esize = sheight;
	if (swidth > sheight && ecd->dsp->switchxy)
		esize = swidth;
	ClassEMemory::Set(palette, 0, MAX_PALETTE * sizeof(COLORREF));
	SetRect(&cliprc, 0, 0, virtualwidth, sheight);
	fontinfo.charstart = 32;
	fontinfo.charend = 127;
	fontinfo.xspacing = 0;
	fontinfo.yspacing = 0;
	fontinfo.numrows = 1;
	fontinfo.rowchars = 128;
	fontinfo.defaultchar = -1;
	FULL_FREE_L(cbitmask);
	FULL_FREE_L(fontrc);
	FULL_FREE(fontkerning);
	FULL_FREE_L(xmax);
	FULL_FREE_L(xmin);
	FULL_FREE_L(ledge);
	FULL_FREE_L(redge);
	xmin = (short *)ecd->mem->AllocList(esize * sizeof(short));
	if (xmin == NULL)
		return(E_NOMEMORY);
	xmax = (short *)ecd->mem->AllocList(esize * sizeof(short));
	if (xmax == NULL)
	{
		FULL_FREE_L(xmin);
		return(E_NOMEMORY);
	}
	ledge = (EINT32 *)ecd->mem->AllocList(esize * sizeof(EINT32) * 4);
	if (ledge == NULL)
	{
		FULL_FREE_L(xmax);
		FULL_FREE_L(xmin);
		return(E_NOMEMORY);
	}
	redge = (EINT32 *)ecd->mem->AllocList(esize * sizeof(EINT32) * 4);
	if (redge == NULL)
	{
		FULL_FREE_L(ledge);
		FULL_FREE_L(xmax);
		FULL_FREE_L(xmin);
		return(E_NOMEMORY);
	}
	return(E_OK);
}

//Handle orientation changes
void E2DSurface::OnChangeOrientation(long *olddata)
{
	bool newxy = ecd->dsp->switchxy;
	long newoh = ecd->dsp->orienth, newov = ecd->dsp->orientv;
	ecd->dsp->orienth = olddata[0];
	ecd->dsp->orientv = olddata[1];
	if (olddata[2])
		ecd->dsp->switchxy = true;
	else
		ecd->dsp->switchxy = false;
	if (ecd->dsp->orienth == newoh && ecd->dsp->orientv == newov && ecd->dsp->switchxy == newxy)
		return;
	E2DSURFACEINFO curinfo;
	unsigned char *curptr = Lock(&curinfo);
	ecd->dsp->orienth = newoh;
	ecd->dsp->orientv = newov;
	ecd->dsp->switchxy = newxy;
	if (curptr)
	{
		E2DSurface *tmpnew;
		bool tocopy = false;
		tmpnew = new E2DSurface;
		if (tmpnew == NULL)
		{
			Free();
			return;
		}
		Unlock();
		if (ecd->dsp->CreateSurface(tmpnew, curinfo.width, curinfo.height, currenttype % 16 | (videomode << 8)) == E_OK)
		{
			curptr = Lock();
			if (curptr)
			{
				E2DSURFACEINFO newinfo;
				unsigned char *newptr = tmpnew->Lock(&newinfo);
				if (newptr)
				{
					ClassEDisplay_PixelCopy(newptr + newinfo.bufferoffset, curptr + curinfo.bufferoffset, newinfo.xpitch, newinfo.ypitch, curinfo.xpitch, curinfo.ypitch, curinfo.width, curinfo.height, newinfo.realwidth, newinfo.realheight, curinfo.realwidth, curinfo.realheight, curinfo.bitwidth);
					tmpnew->Unlock();
				}
				tmpnew->SetColorKey(GetColorKey());
				ClassEMemory::Copy(&tmpnew->fontinfo, &fontinfo, sizeof(fontinfo));
				if (fontcharset)
					tmpnew->SetFontCharacterSet(fontcharset);
				if (fontkerning)
				{
					tmpnew->fontkerning = (short *)ClassEMemory::Alloc((fontkerning[0] * 3 + 2) * sizeof(short));
					if (tmpnew->fontkerning)
						ClassEMemory::Copy(tmpnew->fontkerning, fontkerning, (fontkerning[0] * 3 + 2) * sizeof(short));
				}
				if (fontrc)
				{
					unsigned long fontrc_size = (fontrc[3] * 4 + 4) * sizeof(short);
					tmpnew->fontrc = (short *)ecd->mem->AllocList(fontrc_size);
					ClassEMemory::Copy(tmpnew->fontrc, fontrc, fontrc_size);
				}
				tocopy = true;
			}
		}
		unsigned long ctr, thisnr = 0xFFFFFFFF;
		if (tocopy)
			for (ctr = 0; ctr < ecd->dsp->surflistsize; ctr++)
				if (ecd->dsp->surflist[ctr] == this)
				{
					thisnr = ctr;
					break;
				}
		Unlock();
		Free();
		if (thisnr == 0xFFFFFFFF)
		{
			tmpnew->Free();
			delete tmpnew;
			return;
		}
		if (tocopy)
		{
			unsigned long ctr;
			ClassEMemory::Copy(this, tmpnew, sizeof(E2DSurface));
			for (ctr = 0; ctr < ecd->dsp->surflistsize; ctr++)
				if (ecd->dsp->surflist[ctr] == tmpnew)
				{
					ecd->dsp->surflist[ctr] = NULL;
					break;
				}
			ecd->dsp->surflist[thisnr] = this;
		}
		int *tptr = (int *)tmpnew;
		delete tptr;
	}
	else
		Free();
}

///////////////////////////////////////////////////////////////////
// E2DSurface: public                                            //
///////////////////////////////////////////////////////////////////

//Retrieve a surface type depending on the target operating system API
ENATIVETYPE E2DSurface::GetSysApiSurface(void)
{
	if (nativesurf)
		return((ENATIVETYPE)ecDspNativeSurfaceType(nativesurf));
	return(0);
}

//E2DSurface: constructor
E2DSurface::E2DSurface(void)
{
	currenttype = EST_NONE;
	palette     = NULL;
	ecd         = NULL;
	xmin        = NULL;
	xmax        = NULL;
	ledge       = NULL;
	redge       = NULL;
	fontcharset = NULL;
	fontrc      = NULL;
	fontkerning = NULL;
	cbitmask    = NULL;
	gdata       = NULL;
	indexlookup = NULL;
	nativesurf  = NULL;
	texformat   = NULL;
	exthandle = -1;
	locked    = 0;
	dirty = true;
    autorepair = false;
    reload_alpha = false;
    reload_convert = false;
}

//E2DSurface: destructor
E2DSurface::~E2DSurface(void)
{
	Free();
}

//Lock this surface & return its pointer of the topleft corner pixel, retrieve surface information if needed
unsigned char *E2DSurface::Lock(E2DSURFACEINFO *surfaceinfo)
{
	if (currenttype % 16 == EST_NONE || currenttype % 16 == EST_TEXTURE)
		return(NULL);
	if (locked == 0)
	{
		if (currenttype % 16 == EST_SYSTEMAPI || currenttype % 16 == EST_HARDWARE)
		{
			long lpitch;
			unsigned char *lockdata = (unsigned char *)ecDspLockNativeSurface(ecd->dsp->objdsp, nativesurf, &lpitch);
			if (lockdata)
			{
				swidth = ClassEMath::Abs(lpitch) / (bitwidth / 8);
				vpitch = lpitch;
				gdata = lockdata;
			}
			else
				return(NULL);
		}
	}
	if (gdata != NULL)
	{
		if (surfaceinfo != NULL)
			GetInfo(surfaceinfo);
		locked++;
	}
	dirty = true;
	return(gdata);
}

//Unlock this surface
void E2DSurface::Unlock(void)
{
	if (locked > 0)
	{
		locked--;
		if (locked == 0)
			if (currenttype % 16 == EST_SYSTEMAPI || currenttype % 16 == EST_HARDWARE)
				ecDspUnLockNativeSurface(ecd->dsp->objdsp, nativesurf);
	}
}

//Free resourced used by the surface
void E2DSurface::Free(void)
{
	if (currenttype == EST_NONE)
		return;
	if (locked > 0)
	{
		locked = 1;
		Unlock();
	}
	if (ecd != NULL)
	{
		if (exthandle != -1)
		{
			ecd->dsp->ReleaseTextureHandle(exthandle);
			exthandle = -1;
		}
		if (currenttype % 16 == EST_SYSMEMORY || currenttype % 16 == EST_COMPRESSEDTEX)
		{
			FULL_FREE_L(gdata);
		}
		else
			gdata = NULL;
		FULL_FREE_L(fontcharset);
		FULL_FREE_L(cbitmask);
		FULL_FREE_L(fontrc);
		FULL_FREE(fontkerning);
		FULL_FREE_L(xmax);
		FULL_FREE_L(xmin);
		FULL_FREE_L(ledge);
		FULL_FREE_L(redge);
		FULL_FREE(indexlookup);
		FULL_FREE(palette);
		FULL_FREE(texformat);
		if (nativesurf)
		{
			ecDspDestroyNativeSurface(ecd->dsp->objdsp, nativesurf);
			nativesurf = NULL;
		}
		ecd->dsp->HandleSurfList(this, false);
	}
	currenttype = EST_NONE;
	locked = 0;
    autorepair = false;
}

//Clear surface information
void E2DSurface::Clear(COLORREF col)
{
	FillRect(NULL, col);
	FULL_FREE_L(fontrc);
	FULL_FREE(fontkerning);
	FULL_FREE_L(cbitmask);
    autorepair = false;
}

void E2DSurface::Reload(ClassEDisplay *dsp)
{
    dsp->UploadTexture(this, reload_alpha, false, reload_convert); //reload_alpha, false, reload_convert);
}

//Build a bit mask for pixel collision testing
void E2DSurface::BuildTableMask(unsigned char brightness)
{
	#if (EDITION_SDK < 500)
		return;
	#endif
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return;
	COLORREF cmpck = GetColorKey();
	unsigned long col, xctr, yctr, bitm = 0, mwidth = (virtualwidth + 31) / 32 * 32;
	FULL_FREE_L(cbitmask);
	cbitmask = (unsigned long *)ecd->mem->AllocList(mwidth / 32 * 4 * sheight);
	if (cbitmask == NULL)
	{
		Unlock();
		return;
	}
	ClassEMemory::Set(cbitmask, 0, mwidth / 32 * 4 * sheight);
	for (yctr = 0; yctr < sheight; yctr++)
		for (xctr = 0; xctr < mwidth; xctr++)
		{
			col = GetPixelV(xctr, yctr);
			if (col != cmpck && (((col >> 16) & 0xFF) + ((col >> 8) & 0xFF) + (col & 0xFF)) / 3 >= brightness)
				bitm |= 1 << (31 - (xctr & 31));
			if (xctr % 32 == 31)
			{
				cbitmask[yctr * mwidth / 32 + xctr / 32] = bitm;
				bitm = 0;
			}
		}
	Unlock();
}

//Write the content of the surface to a bitmap file
void E2DSurface::WriteBMP(char *filename, RECT *rc)
{
	USTR_CHECK_BEGIN(filename);
	WriteBMP(ustr_filename, rc);
	USTR_CHECK_END(filename);
}

//Write the content of the surface to a bitmap file
void E2DSurface::WriteBMP(WCHAR *filename, RECT *rc)
{
	RECT copyrc;
	ClassEFile ef;
	if (rc != NULL)
		ClassEMemory::Copy(&copyrc, rc, sizeof(RECT));
	else
		SetRect(&copyrc, 0, 0, GetWidth(), GetHeight());
	if (Lock() == NULL)
		return;
	if (!ef.New(filename))
	{
		Unlock();
		return;
	}
	ELoader_BMP ldr_bmp;
	unsigned long width = copyrc.right - copyrc.left, height = copyrc.bottom - copyrc.top;
	ldr_bmp.WriteHeader(&ef, width, height, bitwidth);
	ldr_bmp.WritePixelData(&ef, this, &copyrc, ecd->dsp->videopalette);
	ef.Close();
	Unlock();
}

//Write the content of the surface to a PNG file
void E2DSurface::WritePNG(char *filename, RECT *rc)
{
	USTR_CHECK_BEGIN(filename);
	WritePNG(ustr_filename, rc);
	USTR_CHECK_END(filename);
}

//Write the content of the surface to a PNG file
void E2DSurface::WritePNG(WCHAR *filename, RECT *rc)
{
	RECT copyrc;
	ClassEFile ef;
	if (rc != NULL)
		ClassEMemory::Copy(&copyrc, rc, sizeof(RECT));
	else
		SetRect(&copyrc, 0, 0, GetWidth(), GetHeight());
	if (Lock() == NULL)
		return;
	if (!ef.New(filename))
	{
		Unlock();
		return;
	}
	ELoader_PNG ldr_png;
	unsigned long width = copyrc.right - copyrc.left, height = copyrc.bottom - copyrc.top;
	ldr_png.WriteHeader(&ef, width, height, bitwidth);
	ldr_png.WritePixelData(&ef, this, &copyrc, bitwidth, ecd->dsp->videopalette);
	ef.Close();
	Unlock();
}

//Retrieve the current color key for this surface
unsigned long E2DSurface::GetColorKey(void)
{
	unsigned long ck = colorkey;
	RevertColor(ck, videomode);
	return(ck);
}

//Retrieve more information about the surface
bool E2DSurface::GetInfo(E2DSURFACEINFO *surfaceinfo)
{
	if (surfaceinfo != NULL)
	{
		long ofsx = 0, ofsy = 0;
		surfaceinfo->width       = GetWidth();
		surfaceinfo->height      = GetHeight();
		surfaceinfo->realwidth   = virtualwidth;
		surfaceinfo->realheight  = sheight;
		surfaceinfo->pixelformat = videomode;
		surfaceinfo->realpitch   = vpitch;
		surfaceinfo->bitwidth    = bitwidth;
		if (ecd->dsp->switchxy)
		{
			surfaceinfo->xpitch = surfaceinfo->realpitch * ecd->dsp->orientv;
			surfaceinfo->ypitch = surfaceinfo->bitwidth / 8 * ecd->dsp->orienth;
		}
		else
		{
			surfaceinfo->xpitch = surfaceinfo->bitwidth / 8 * ecd->dsp->orienth;
			surfaceinfo->ypitch = surfaceinfo->realpitch * ecd->dsp->orientv;
		}
		ecd->dsp->VirtualPos(ofsx, ofsy, virtualwidth, sheight);
		surfaceinfo->bufferoffset = ofsy * surfaceinfo->realpitch + ofsx * bitwidth / 8;
	}
	else
		return(false);
	return(true);
}

//Convert a coordinate on the landscape to a screen coodinate
bool E2DSurface::GetLandscapePoint(RECT *viewport, long pitch, long mapx, long mapy, unsigned short turn, short horizon, unsigned short scale, long objx, long objy, E2DSurface *source, EVECTOR3D *vector)
{
	RECT nrc;
	bool lineswitch = false, scanswitch = false;
	long yctr, width, height, div;
	long tmpx, tmpy, orgzadd, zadd, curdistance = 0;
	objx -= mapx;
	objy = mapy - objy;
	tmpx = objx;
	tmpy = objy;
	objx = (long)(tmpx * (ecd->math->Cos(turn) >> 6)) - (tmpy * (ecd->math->Sin(turn) >> 6));
	objy = (long)(tmpx * (ecd->math->Sin(turn) >> 6)) + (tmpy * (ecd->math->Cos(turn) >> 6));
	objx >>= 2;
	objy >>= 2;
	vector->x = 0;
	vector->y = 0;
	vector->z = objy;
	if (viewport == NULL)
	{
		nrc.left   = 0;
		nrc.top    = 0;
		nrc.right  = GetWidth();
		nrc.bottom = GetHeight();
	}
	else
		ClassEMemory::Copy(&nrc, viewport, sizeof(RECT));
	ecd->dsp->VirtualRotation(turn);
	ecd->dsp->VirtualRect(&nrc, virtualwidth, sheight);
	ecd->dsp->VirtualPos(mapx, mapy, source->virtualwidth * 16, source->sheight * 16);
	if (!ClipRect(&nrc))
		return(false);
	ecd->dsp->ReVirtualRect(&nrc, virtualwidth, sheight);
	if (horizon < 0)
		lineswitch = true;
	width = nrc.right - nrc.left;
	height = ecd->dsp->rendersize;
	zadd = (65536 << 4) / (long)scale;
	orgzadd = zadd;
	yctr = nrc.bottom;
	div = ((height << 10) - (nrc.bottom - yctr) * ClassEMath::Abs(horizon));
	if (div > 0)
	{
		zadd = ((orgzadd * height) << 8) / div;
		zadd = ((zadd    * height) << 8) / div;
	}
	curdistance = pitch * zadd;
	if (curdistance > vector->z)
		scanswitch = true;
	while((!scanswitch && yctr > nrc.top) || (scanswitch && yctr < nrc.bottom + (nrc.bottom - nrc.top)))
	{
		if (scanswitch)
			yctr++;
		else
			yctr--;
		div = ((height << 10) - (nrc.bottom - yctr) * ClassEMath::Abs(horizon));
		if (div <= 0)
			return(false);
		zadd = ((orgzadd * height) << 8) / div;
		zadd = ((zadd    * height) << 8) / div;
		zadd = (((zadd >> 16) * (ClassEMath::Abs(horizon) + 256)) << 8) + (((zadd & 65535) * (ClassEMath::Abs(horizon) + 256)) >> 8);
		if (scanswitch)
			curdistance -= zadd;
		else
			curdistance += zadd;
		if ((!scanswitch && curdistance >= vector->z && curdistance - zadd <= vector->z) || (scanswitch && curdistance <= vector->z && curdistance + zadd >= vector->z))
		{
			long newzadd = ((orgzadd * height) << 8) / div;
			if (lineswitch)
				vector->y = nrc.top + nrc.bottom - 1 - yctr;
			else
				vector->y = yctr;
			if (newzadd > 0)
				vector->x = nrc.right - (nrc.right - nrc.left) / 2 + (objx / newzadd);
			else
				return(false);
			return(true);
		}
	}
	return(false);
}

//Retrieve the clip rectangle
void E2DSurface::GetClipper(RECT *rc)
{
	if (rc != NULL && ecd != NULL)
	{
		ClassEMemory::Copy(rc, &cliprc, sizeof(RECT));
		ecd->dsp->ReVirtualRect(rc, virtualwidth, sheight);
	}
}

//Set the colorkey value for transparency
void E2DSurface::SetColorKey(COLORREF col)
{
	colorkey = col;
	ConvertColor(colorkey, videomode);
	if (nativesurf)
		ecDspNativeSetColorkey(ecd->dsp->objdsp, nativesurf, colorkey);
}

//Set the colorkey automatically based on the given pixel position color
void E2DSurface::AutoSetColorKey(long x, long y){ SetColorKey(GetPixel(x, y)); }

//Set a smaller clipper rectangle
void E2DSurface::SetClipper(RECT *rc)
{
	if (currenttype == EST_NONE)
		return;
	if (rc != NULL)
	{
		ClassEMemory::Copy(&cliprc, rc, sizeof(RECT));
		ecd->dsp->VirtualRect(&cliprc, virtualwidth, sheight);
		if (cliprc.left < 0)
			cliprc.left = 0;
		if (cliprc.right > (long)virtualwidth)
			cliprc.right = virtualwidth;
		if (cliprc.top < 0)
			cliprc.top = 0;
		if (cliprc.bottom > (long)sheight)
			cliprc.bottom = sheight;
	}
	else
		SetRect(&cliprc, 0, 0, virtualwidth, sheight);
}

//Build the font sheet
void E2DSurface::BuildFont(bool variablewidth, COLORREF fontcolorkey, long autoreorder)
{
	FULL_FREE_L(fontrc);
	FULL_FREE(fontkerning);
	unsigned long sheetwidth = GetWidth(), sheetheight = GetHeight();
	unsigned long numchars = fontinfo.charend - fontinfo.charstart + 1;
	if (fontcharset)
		numchars = ClassEStd::StrLen(fontcharset);
	if (numchars == 0)
		return;
	if (fontinfo.numrows == 0)
		fontinfo.numrows = 1;
	short *fontrcptr;
	long ctr, fixedwidth = 0, fixedheight = 0;
	if (fontinfo.numrows > 0)
		fixedheight = sheetheight / fontinfo.numrows;
	if (fontinfo.numrows == 1)
		fontinfo.rowchars = numchars;
	fontrc = (short *)ecd->mem->AllocList((numchars * 4 + 4) * sizeof(short));
	if (fontrc == NULL)
		return;
	ClassEMemory::Set(fontrc, 0, (numchars * 4 + 4) * sizeof(short));
	fontrc[0] = (short)fontinfo.xspacing;
	fontrc[1] = (short)fontinfo.yspacing;
	fontrc[2] = fontinfo.charstart;
	fontrc[3] = (short)numchars;
	fontrcptr = &fontrc[4];
	SetColorKey(fontcolorkey);
	fontcolorkey = GetColorKey();
	if (variablewidth)
	{
		bool onnewline = true;
		unsigned long currow = 0;
		for (ctr = 0; ctr < (long)numchars; ctr++)
		{
			if (onnewline)
				fontrcptr[0] = 0;
			else
				fontrcptr[0] = fontrcptr[-2];
			while(GetPixel(fontrcptr[0], currow * fixedheight) == fontcolorkey && fontrcptr[0] < (short)sheetwidth)
				fontrcptr[0]++;
			fontrcptr[2] = fontrcptr[0];
			while(GetPixel(fontrcptr[2], currow * fixedheight) != fontcolorkey && fontrcptr[2] < (short)sheetwidth)
				fontrcptr[2]++;
			if (fontrcptr[0] == fontrcptr[2])
			{
				onnewline = true;
				currow++;
				if (currow * fixedheight >= sheetheight)
					break;
				ctr--;
				continue;
			}
			else
			{
				onnewline = false;
				fontrcptr[1] = (short)(currow * fixedheight + 2);
				fontrcptr[3] = (short)(fontrcptr[1] + fixedheight - 2);
				fontrcptr += 4;
			}
		}
	}
	else
	{
		if (fontinfo.rowchars <= 0)
			fontinfo.rowchars = numchars / fontinfo.numrows;
		if (fontinfo.rowchars == 0)
			return;
		fixedwidth = sheetwidth / fontinfo.rowchars;
		for (ctr = 0; ctr < (long)numchars; ctr++)
		{
			if (ctr / fontinfo.rowchars * fixedheight >= (long)sheetheight)
				break;
			fontrcptr[0] = (short)(ctr % fontinfo.rowchars * fixedwidth);
			fontrcptr[1] = (short)(ctr / fontinfo.rowchars * fixedheight);
			fontrcptr[2] = (short)(fontrcptr[0] + fixedwidth);
			fontrcptr[3] = (short)(fontrcptr[1] + fixedheight);
			fontrcptr += 4;
		}
	}

	if (autoreorder == EFR_ALWAYS || (autoreorder == EFR_DEFAULT && EdgeGL_Use()))
	{
		long fitwidth = 1024;
		if ((unsigned)(fitwidth * fitwidth * 3 / 16) > GetWidth() * GetHeight()) fitwidth >>= 1; // if smaller square can contain font + 33%, shrink
		if ((unsigned)(fitwidth * fitwidth * 3 / 16) > GetWidth() * GetHeight()) fitwidth >>= 1;

		unsigned long * newmap = (unsigned long *) ecd->mem->Alloc(fitwidth*fitwidth*sizeof(long));
		short * newfontrc = (short *)ecd->mem->AllocList((numchars * 4 + 4) * sizeof(short));
		if (newmap && newfontrc)
		{
			newfontrc[0] = fontrc[0];
			newfontrc[1] = fontrc[1];
			newfontrc[2] = fontrc[2];
			newfontrc[3] = fontrc[3];

			short * oldrcptr = &(fontrc[4]);
			short * newrcptr = &(newfontrc[4]);

			int rowstart = 0;
			int rowh = 0;
			int rowx = 0;

			unsigned int i;
			for (i = 0; i < numchars; i++)
			{
				int charw = oldrcptr[2] - oldrcptr[0];
				int charh = oldrcptr[3] - oldrcptr[1];
				if (charh > rowh)
				{
					rowh = charh;
					if (rowstart + rowh >= fitwidth) break;
				}
				if (rowx + charw >= fitwidth)
				{
					rowstart += rowh + 1;
					rowh = charh;
					rowx = 0;
					if (rowstart + rowh >= fitwidth) break;
				}
				newrcptr[0] = rowx;
				newrcptr[1] = rowstart;
				newrcptr[2] = rowx + charw;
				newrcptr[3] = rowstart + charh;

				for (int y = 0; y <= charh; y++)
				{	
					for (int x = 0; x <= charw; x++)
					{							
						newmap[(x + rowx) + fitwidth*(y+ rowstart)] = GetPixel(oldrcptr[0] + x, oldrcptr[1] + y);
					}
				}
				newrcptr = &(newrcptr[4]);
				oldrcptr = &(oldrcptr[4]);

				rowx += charw + 1;
			}
			if (i == numchars)
			{
				// update original texture
				rowstart += rowh + 1;
				Create(fitwidth, rowstart, videomode, EST_DEFAULT);
				for (int y = 0; y < rowstart; y++)
				{
					for (int x = 0; x < fitwidth; x++)
					{
						this->PutPixel(x, y, newmap[x + fitwidth*y] & 0xffffff, (unsigned char)(newmap[x + fitwidth*y] >> 24));
					}
				}
				FULL_FREE_L(fontrc);				
				fontrc = newfontrc;
			} 
			else 
			{
				FULL_FREE_L(newfontrc);
			}
			ecd->mem->DeAlloc(newmap);
		}
		else
		{
			if (newfontrc) FULL_FREE_L(newfontrc);
			if (newmap) ecd->mem->DeAlloc(newmap);
		}

	}
}

//Replace one color with another
bool E2DSurface::ReplaceColor(COLORREF dst, COLORREF src, bool exclude)
{
	ConvertColor(dst, videomode);
	ConvertColor(src, videomode);
	long xc, yc, w = cliprc.right - cliprc.left;
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return(false);
	if (exclude)
	{
		if (bitwidth == 8)
		{
			unsigned char *psd = &memptr[cliprc.top * vpitch + cliprc.left];
			for (yc = cliprc.top; yc < cliprc.bottom; yc++)
			{
				for (xc = 0; xc < w; xc++)
					if (psd[xc] != src)
						psd[xc] = (unsigned char)dst;
				psd += vpitch;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *psd = (unsigned short *)&memptr[cliprc.top * vpitch + cliprc.left * 2];
			for (yc = cliprc.top; yc < cliprc.bottom; yc++)
			{
				for (xc = 0; xc < w; xc++)
					if (psd[xc] != src)
						psd[xc] = (unsigned short)dst;
				psd += vpitch / 2;
			}
		}
		else if (bitwidth == 24)
		{
			unsigned long tc = 0;
			unsigned char *psd = &memptr[cliprc.top * vpitch + cliprc.left * 3];
			for (yc = cliprc.top; yc < cliprc.bottom; yc++)
			{
				for (xc = 0; xc < w; xc++)
				{
					ClassEMemory::Copy(&tc, &psd[xc * 3], 3);
					if (tc != src)
						ClassEMemory::Copy(&psd[xc * 3], &dst, 3);
				}
				psd += vpitch;
			}
		}
		else if (bitwidth == 32)
		{
			unsigned EINT32 *psd = (unsigned EINT32 *)&memptr[cliprc.top * vpitch + cliprc.left * 4];
			for (yc = cliprc.top; yc < cliprc.bottom; yc++)
			{
				for (xc = 0; xc < w; xc++)
					if (psd[xc] != src)
						psd[xc] = dst;
				psd += vpitch / 4;
			}
		}
	}
	else
	{
		if (bitwidth == 8)
		{
			unsigned char *psd = &memptr[cliprc.top * vpitch + cliprc.left];
			for (yc = cliprc.top; yc < cliprc.bottom; yc++)
			{
				for (xc = 0; xc < w; xc++)
					if (psd[xc] == src)
						psd[xc] = (unsigned char)dst;
				psd += vpitch;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *psd = (unsigned short *)&memptr[cliprc.top * vpitch + cliprc.left * 2];
			for (yc = cliprc.top; yc < cliprc.bottom; yc++)
			{
				for (xc = 0; xc < w; xc++)
					if (psd[xc] == src)
						psd[xc] = (unsigned short)dst;
				psd += vpitch / 2;
			}
		}
		else if (bitwidth == 24)
		{
			unsigned long tc = 0;
			unsigned char *psd = &memptr[cliprc.top * vpitch + cliprc.left * 3];
			for (yc = cliprc.top; yc < cliprc.bottom; yc++)
			{
				for (xc = 0; xc < w; xc++)
				{
					ClassEMemory::Copy(&tc, &psd[xc * 3], 3);
					if (tc == src)
						ClassEMemory::Copy(&psd[xc * 3], &dst, 3);
				}
				psd += vpitch;
			}
		}
		else if (bitwidth == 32)
		{
			unsigned EINT32 *psd = (unsigned EINT32 *)&memptr[cliprc.top * vpitch + cliprc.left * 4];
			for (yc = cliprc.top; yc < cliprc.bottom; yc++)
			{
				for (xc = 0; xc < w; xc++)
					if (psd[xc] == src)
						psd[xc] = dst;
				psd += vpitch / 4;
			}
		}
	}
	Unlock();
	return(true);
}

//Change size of the surface
void E2DSurface::Resize(unsigned long width, unsigned long height, bool usefilter)
{
	#if (EDITION_SDK < 500)
		return;
	#endif
	if (height == 0 && width > 0)
		height = GetHeight() * width / GetWidth();
	else if (width == 0 && height > 0)
		width = GetWidth() * height / GetHeight();
	if (width == GetWidth() && height == GetHeight())
		return;
	if (width > GetWidth() || height > GetHeight())
		usefilter = false;
	E2DSurface old;
	if (ecd->dsp->CreateSurface(&old, GetWidth(), GetHeight(), EST_SYSMEMORY | (videomode << 8)) == E_OK)
	{
		old.BltFast(0, 0, this, NULL);
		if (Create(width, height, videomode, currenttype % 16) == E_OK)
		{
			/*
			E2DSURFACEINFO info1, info2;
			unsigned char *memptr1, *memptr2;
			memptr1 = old.Lock(&info1);
			memptr2 = Lock(&info2);
			if (memptr1 != NULL && memptr2 != NULL)
			{
				long xc, yc;
				if (bitwidth == 32)
				{
					unsigned long *smem = (unsigned long *)memptr1;
					long u, v, uadd, vadd, vidx, vidx2, ublend, vblend;
					info1.realpitch /= sizeof(unsigned long);
					uadd = (info1.realwidth << 16) / info2.realwidth;
					vadd = (info1.realheight << 16) / info2.realheight;
					ublend = (uadd & 0xFFFF) >> 8;
					vblend = (vadd & 0xFFFF) >> 8;


					v = 0;
					for (yc = 0; yc < info2.realheight; yc++)
					{
						unsigned long *dmem = (unsigned long *)&memptr2[yc * info2.realpitch];
						vidx = (v >> 16) * info1.realpitch;
						if ((v >> 16) < info1.realheight - 1)
							vidx2 = vidx + info1.realpitch;
						else
							vidx2 = vidx;
						u = 0;
						for (xc = 0; xc < info2.realwidth; xc++)
						{
							long u1, u2;
							unsigned long r, g, b;
							COLORREF c1, c2, c3, c4;
							u1 = (u >> 16);
							if (u1 < info1.realwidth - 1)
								u2 = u1 + 1;
							else
								u2 = u1;
							c1 = smem[vidx + u1];
							c2 = smem[vidx + u2];
							c3 = smem[vidx2 + u1];
							c4 = smem[vidx2 + u2];

							r = (EBCODE_MACRO_BILINEAR(0xFF00, c1 >> 8, c2 >> 8, c3 >> 8, c4 >> 8, ublend, vblend) >> 8) & 0xFF0000;
							g = (EBCODE_MACRO_BILINEAR(0xFF00, c1, c2, c3, c4, ublend, vblend) >> 16) & 0xFF00;
							b = (EBCODE_MACRO_BILINEAR(0xFF, c1, c2, c3, c4, ublend, vblend) >> 16) & 0xFF;

							dmem[xc] = r | g | b;
							u += uadd;
						}
						v += vadd;
					}
				}
			}
			else
			{
				if (memptr1 != NULL)
					old.Unlock();
				if (memptr2 != NULL)
					Unlock();
			}*/
			unsigned long x, y;
			if (usefilter)
			{
				unsigned long *utable = NULL, *vtable = NULL;
				utable = (unsigned long *)ClassEMemory::Alloc((width + 1) * sizeof(unsigned long));
				vtable = (unsigned long *)ClassEMemory::Alloc((height + 1) * sizeof(unsigned long));
				if (utable != NULL && vtable != NULL)
				{
					for (x = 0; x < width; x++)
						utable[x] = x * old.GetWidth() * 256 / width;
					utable[width] = old.GetWidth() * 256;
					for (y = 0; y < height; y++)
						vtable[y] = y * old.GetHeight() * 256 / height;
					vtable[height] = old.GetHeight() * 256;
					for (y = 0; y < height; y++)
					{
						for (x = 0; x < width; x++)
						{
							unsigned long vr = 0, vg = 0, vb = 0;
							unsigned long v, nextv;
							v = vtable[y];
							while(v < vtable[y + 1])
							{
								unsigned long ur = 0, ug = 0, ub = 0;
								unsigned long u, nextu, lineworth = 256;
								if (v + 255 >= vtable[y + 1])
								{
									lineworth = vtable[y + 1] & 255;
									nextv = vtable[y + 1];
								}
								else if (v & 255)
								{
									lineworth = 256 - (v & 255);
									nextv = (v & 0xFFFFFF00) + 256;
								}
								else
									nextv = v + 256;
								u = utable[x];
								while (u < utable[x + 1])
								{
									COLORREF col;
									unsigned long pixworth = 256;
									if (u + 255 >= utable[x + 1])
									{
										pixworth = utable[x + 1] & 255;
										nextu = utable[x + 1];
									}
									else if (u & 255)
									{
										pixworth = 256 - (u & 255);
										nextu = (u & 0xFFFFFF00) + 256;
									}
									else
										nextu = u + 256;
									col = old.GetPixel(u >> 8, v >> 8);
									ur += E_GETR(col) * pixworth;
									ug += E_GETG(col) * pixworth;
									ub += E_GETB(col) * pixworth;
									u = nextu;
								}
								ur /= utable[x + 1] - utable[x];
								ug /= utable[x + 1] - utable[x];
								ub /= utable[x + 1] - utable[x];
								vr += ur * lineworth;
								vg += ug * lineworth;
								vb += ub * lineworth;
								v = nextv;
							}
							vr /= vtable[y + 1] - vtable[y];
							vg /= vtable[y + 1] - vtable[y];
							vb /= vtable[y + 1] - vtable[y];
							PutPixel(x, y, E_RGBX(vr, vg, vb));
						}
					}
				}
				ClassEMemory::DeAlloc(utable);
				ClassEMemory::DeAlloc(vtable);
			}
			else
			{
				for (y = 0; y < height; y++)
					for (x = 0; x < width; x++)
						PutPixel(x, y, old.GetPixel(x * old.GetWidth() / width, y * old.GetHeight() / height));
			}
		}
		old.Free();
	}
}

//Font surfaces: draw text using various effects
void E2DSurface::DrawFont(long x, long y, E2DSurface *source, const char *text, long flags, unsigned long property)
{
	if (currenttype % 16 == EST_NONE || source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	dirty = true;
	bool edgeinternal = true;
	if (nativesurf != NULL && locked == 0 && source->nativesurf != NULL && source->locked == 0)
		if ((ecd->dispcfg->dspnative && !(flags & E2D_PREVENTNATIVE)) || (flags & E2D_ALLOWNATIVE))
			if ((flags & (EFX_COLORFILTER - EFX_COLORKEY)) == 0)
				edgeinternal = false;
	if (edgeinternal)
	{
		if (Lock() == NULL)
			return;
		if (source->Lock() == NULL)
		{
			Unlock();
			return;
		}
	}
	FONTDRAW_USERDATA_SURF2D info;
	info.dest = this;
	info.source = source;
	info.srcrgba = false;
	info.flags = flags;
	info.property = property;
	FontDraw_Draw(E2DSurface_DrawFontFunc, x, y, (const unsigned char *)text, 0, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, false, &info);
	dirty = true;
	if (edgeinternal)
	{
		Unlock();
		source->Unlock();
	}
}

//Font surfaces: draw text using various effects
void E2DSurface::DrawFont(long x, long y, E2DSurface *source, const WCHAR *text, long flags, unsigned long property)
{
	if (currenttype % 16 == EST_NONE || source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	bool edgeinternal = true;
	if (nativesurf != NULL && locked == 0 && source->nativesurf != NULL && source->locked == 0)
		if ((ecd->dispcfg->dspnative && !(flags & E2D_PREVENTNATIVE)) || (flags & E2D_ALLOWNATIVE))
			if ((flags & (EFX_COLORFILTER - EFX_COLORKEY)) == 0)
				edgeinternal = false;
	if (edgeinternal)
	{
		if (Lock() == NULL)
			return;
		if (source->Lock() == NULL)
		{
			Unlock();
			return;
		}
	}	
	FONTDRAW_USERDATA_SURF2D info;
	info.dest = this;
	info.source = source;
	info.srcrgba = false;
	info.flags = flags;
	info.property = property;
	FontDraw_Draw(E2DSurface_DrawFontFunc, x, y, (const unsigned char *)text, 0, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, true, &info);
	dirty = true;
	if (edgeinternal)
	{
		Unlock();
		source->Unlock();
	}
}

//Font surfaces: draw a value using various effects
void E2DSurface::DrawFont(long x, long y, E2DSurface *source, long value, long flags, unsigned long property)
{
	char valuestr[32];
	if (value < 0)
	{
		valuestr[0] = '-';
		value = ClassEMath::Abs(value);
		ClassEStd::IntToStr(&valuestr[1], value, 10);
	}
	else
		ClassEStd::IntToStr(valuestr, value, 10);
	DrawFont(x, y, source, valuestr, flags, property);
}

//Font surfaces: draw text using various effects
void E2DSurface::DrawFont(long x, long y, E2DSurfaceRGBA *source, const char *text, long flags, unsigned long property)
{
	if (currenttype % 16 == EST_NONE || source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	if (Lock() == NULL)
		return;
	if (source->Lock() == NULL)
	{
		Unlock();
		return;
	}
	FONTDRAW_USERDATA_SURF2D info;
	info.dest = this;
	info.source = source;
	info.srcrgba = true;
	info.flags = flags;
	info.property = property;
	FontDraw_Draw(E2DSurface_DrawFontFunc, x, y, (const unsigned char *)text, 0, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, false, &info);
	dirty = true;
	Unlock();
	source->Unlock();
}

//Font surfaces: draw text using various effects
void E2DSurface::DrawFont(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, long flags, unsigned long property)
{
	if (currenttype % 16 == EST_NONE || source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	if (Lock() == NULL)
		return;
	if (source->Lock() == NULL)
	{
		Unlock();
		return;
	}
	FONTDRAW_USERDATA_SURF2D info;
	info.dest = this;
	info.source = source;
	info.srcrgba = true;
	info.flags = flags;
	info.property = property;
	FontDraw_Draw(E2DSurface_DrawFontFunc, x, y, (const unsigned char *)text, 0, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, true, &info);
	dirty = true;
	Unlock();
	source->Unlock();
}

//Font surfaces: draw a value using various effects
void E2DSurface::DrawFont(long x, long y, E2DSurfaceRGBA *source, long value, long flags, unsigned long property)
{
	char valuestr[32];
	if (value < 0)
	{
		valuestr[0] = '-';
		value = ClassEMath::Abs(value);
		ClassEStd::IntToStr(&valuestr[1], value, 10);
	}
	else
		ClassEStd::IntToStr(valuestr, value, 10);
	DrawFont(x, y, source, valuestr, flags, property);
}

//Draw font wrapped to the given width
void E2DSurface::DrawFontWrap(long x, long y, E2DSurface *source, const char *text, unsigned long width, long flags, unsigned long property)
{
	if (currenttype % 16 == EST_NONE || source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	bool edgeinternal = true;
	if (nativesurf != NULL && locked == 0 && source->nativesurf != NULL && source->locked == 0)
		if ((ecd->dispcfg->dspnative && !(flags & E2D_PREVENTNATIVE)) || (flags & E2D_ALLOWNATIVE))
			if ((flags & (EFX_COLORFILTER - EFX_COLORKEY)) == 0)
				edgeinternal = false;
	if (edgeinternal)
	{
		if (Lock() == NULL)
			return;
		if (source->Lock() == NULL)
		{
			Unlock();
			return;
		}
	}
	FONTDRAW_USERDATA_SURF2D info;
	info.dest = this;
	info.source = source;
	info.srcrgba = false;
	info.flags = flags;
	info.property = property;
	FontDraw_Draw(E2DSurface_DrawFontFunc, x, y, (const unsigned char *)text, width, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, false, &info);
	dirty = true;
	if (edgeinternal)
	{
		Unlock();
		source->Unlock();
	}
}

//Draw font wrapped to the given width
void E2DSurface::DrawFontWrap(long x, long y, E2DSurface *source, const WCHAR *text, unsigned long width, long flags, unsigned long property)
{
	if (currenttype % 16 == EST_NONE || source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	bool edgeinternal = true;
	if (nativesurf != NULL && locked == 0 && source->nativesurf != NULL && source->locked == 0)
		if ((ecd->dispcfg->dspnative && !(flags & E2D_PREVENTNATIVE)) || (flags & E2D_ALLOWNATIVE))
			if ((flags & (EFX_COLORFILTER - EFX_COLORKEY)) == 0)
				edgeinternal = false;
	if (edgeinternal)
	{
		if (Lock() == NULL)
			return;
		if (source->Lock() == NULL)
		{
			Unlock();
			return;
		}
	}
	FONTDRAW_USERDATA_SURF2D info;
	info.dest = this;
	info.source = source;
	info.srcrgba = false;
	info.flags = flags;
	info.property = property;
	FontDraw_Draw(E2DSurface_DrawFontFunc, x, y, (const unsigned char *)text, width, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, true, &info);
	dirty = true;
	if (edgeinternal)
	{
		Unlock();
		source->Unlock();
	}
}

//Draw font wrapped to the given width
void E2DSurface::DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const char *text, unsigned long width, long flags, unsigned long property)
{
	if (currenttype % 16 == EST_NONE || source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	if (Lock() == NULL)
		return;
	if (source->Lock() == NULL)
	{
		Unlock();
		return;
	}
	FONTDRAW_USERDATA_SURF2D info;
	info.dest = this;
	info.source = source;
	info.srcrgba = true;
	info.flags = flags;
	info.property = property;
	FontDraw_Draw(E2DSurface_DrawFontFunc, x, y, (const unsigned char *)text, width, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, false, &info);
	dirty = true;
	Unlock();
	source->Unlock();
}

//Draw font wrapped to the given width
void E2DSurface::DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, unsigned long width, long flags, unsigned long property)
{
	if (currenttype % 16 == EST_NONE || source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	if (Lock() == NULL)
		return;
	if (source->Lock() == NULL)
	{
		Unlock();
		return;
	}
	FONTDRAW_USERDATA_SURF2D info;
	info.dest = this;
	info.source = source;
	info.srcrgba = true;
	info.flags = flags;
	info.property = property;
	FontDraw_Draw(E2DSurface_DrawFontFunc, x, y, (const unsigned char *)text, width, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, true, &info);
	dirty = true;
	Unlock();
	source->Unlock();
}

//Retrieve a pixel from the specified position
COLORREF E2DSurface::GetPixel(long x, long y)
{
	ecd->dsp->VirtualPos(x, y, virtualwidth, sheight);
	return(GetPixelV(x, y));
}

//Put a pixel on the specified position using various effects
void E2DSurface::PutPixel(long x, long y, COLORREF col, long flags, unsigned long property)
{
	ecd->dsp->VirtualPos(x, y, virtualwidth, sheight);
	if (x < cliprc.left || y < cliprc.top || x >= cliprc.right || y >= cliprc.bottom)
		return;
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return;
	PutPixelRaw(x, y, col, flags, property, memptr);
	Unlock();
}

//Put a row of pixels using various effects
void E2DSurface::PutPixelRow(long x, long y, unsigned long width, COLORREF col, long flags, unsigned long property)
{
	PutPixelRowV(x, y, width, col, flags, property, true);
}

//Draw a line using various effects
void E2DSurface::DrawLine(long x1, long y1, long x2, long y2, COLORREF col, long flags, unsigned long property)
{
	if (y1 == y2)
	{
		if (x2 < x1)
			PutPixelRow(x2, y1, x1 - x2 + 1, col, flags, property);
		else
			PutPixelRow(x1, y1, x2 - x1 + 1, col, flags, property);
		return;
	}
	if (Lock() == NULL)
		return;
	long dx, dy, x_inc, y_inc, error, index, tx, ty, temp;
	if (y1 > y2)
	{
		temp = x1; x1 = x2;	x2 = temp;
		temp = y1; y1 = y2;	y2 = temp;
	}
	error = 0;
	dx = x2 - x1;
	dy = y2 - y1;
	tx = x1;
	ty = y1;
	if (dx < 0)
	{
		x_inc = -1;
		dx = -dx;
	}
	else
		x_inc = 1;
	if (dy < 0)
	{
		y_inc = -1;
		dy = -dy;
	}
	else
		y_inc = 1;
	if (dx > dy)
	{
		for (index = 0; index <= dx; index++)
		{
			PutPixel(tx, ty, col, flags, property);
			error += dy;
			if (error >= dx)
			{
				error -= dx;
				ty += y_inc;
			}
			tx += x_inc;
		}
	}
	else
	{
		for (index = 0; index <= dy; index++)
		{
			PutPixel(tx, ty, col, flags, property);
			error += dx;
			if (error >= dy)
			{
				error -= dy;
				tx += x_inc;
			}
			ty += y_inc;
		}
	}
	Unlock();
}

//Draw an oval or circle using various effects
void E2DSurface::DrawOval(long x, long y, long xradius, long yradius, COLORREF col, long flags, unsigned long property)
{
	if (Lock() == NULL)
		return;
	long yctr, sinv, cosv, prevcosv, prevsinv = -1, rsinv = -1;	
	yradius = ClassEMath::Abs(yradius);
	xradius = ClassEMath::Abs(xradius);
	prevcosv = 999999;
	for (yctr = 0; yctr < 1024; yctr++)
	{
		cosv = (ecd->math->Cos(yctr) * yradius) >> 16;
		if (prevcosv > cosv)
		{
			if (prevcosv == 999999)
			{
				prevsinv = -1;
				prevcosv = cosv;
			}
			else
			{
				sinv = (ecd->math->Sin(yctr) * xradius) >> 16;
				if (prevsinv == sinv)
					rsinv = prevsinv;
				else if (prevsinv < sinv)
					rsinv = prevsinv + 1;
				else if (prevsinv > sinv)
					rsinv = prevsinv - 1;
				DrawLine(x - sinv, y - cosv, x - rsinv, y - cosv, col, flags, property);
				DrawLine(x + sinv, y - cosv, x + rsinv, y - cosv, col, flags, property);
				if (cosv != 0)
				{
					DrawLine(x - sinv, y + cosv, x - rsinv, y + cosv, col, flags, property);
					DrawLine(x + sinv, y + cosv, x + rsinv, y + cosv, col, flags, property);
				}
				prevsinv = sinv;
				prevcosv = cosv;
			}
		}
	}
	Unlock();
}

//Draw a rectangle using various effects
void E2DSurface::DrawRect(RECT *rc, COLORREF col, long flags, unsigned long property)
{
	if (rc == NULL)
		return;
	if (Lock() == NULL)
		return;
	DrawLine(rc->left, rc->top, rc->right - 1, rc->top, col, flags, property);
	if (rc->bottom - 1 != rc->top)
		DrawLine(rc->left, rc->bottom - 1, rc->right - 1, rc->bottom - 1, col, flags, property);
	if (ClassEMath::Abs(rc->bottom - 1 - rc->top) >= 2)
	{
		DrawLine(rc->left, rc->top + 1, rc->left, rc->bottom - 2, col, flags, property);
		DrawLine(rc->right - 1, rc->top + 1, rc->right - 1, rc->bottom - 2, col, flags, property);
	}
	Unlock();
}

//Fill a polygon with a texture
void E2DSurface::TexturePoly(long x1, long y1, long u1, long v1, long x2, long y2, long u2, long v2, long x3, long y3, long u3, long v3, E2DSurface *source, long flags, unsigned long property)
{
	this->TexturePolyV(x1 << 10, y1 << 10, u1 << 10, v1 << 10, x2 << 10, y2 << 10, u2 << 10, v2 << 10, x3 << 10, y3 << 10, u3 << 10, v3 << 10, source, flags, property);
}

//Fill a polygon with a texture (perspective correct)
void E2DSurface::TexturePoly(long x1, long y1, long w1, long u1, long v1, long x2, long y2, long w2, long u2, long v2, long x3, long y3, long w3, long u3, long v3, E2DSurface *source, long flags, unsigned long property)
{
	this->TexturePolyV(x1 << 10, y1 << 10, w1 << 10, u1 << 10, v1 << 10, x2 << 10, y2 << 10, w2 << 10, u2 << 10, v2 << 10, x3 << 10, y3 << 10, w3 << 10, u3 << 10, v3 << 10, source, flags, property);
}

//Fill a polygon using various effects
void E2DSurface::FillPoly(long x1, long y1, long x2, long y2, long x3, long y3, COLORREF col, long flags, unsigned long property)
{
	this->FillPolyV(x1 << 10, y1 << 10, x2 << 10, y2 << 10, x3 << 10, y3 << 10, col, flags, property);
}

//Fill a gradient polygon
void E2DSurface::GradientPoly(long x1, long y1, COLORREF col1, long x2, long y2, COLORREF col2, long x3, long y3, COLORREF col3, long flags)
{
	this->GradientPolyV(x1 << 10, y1 << 10, col1, x2 << 10, y2 << 10, col2, x3 << 10, y3 << 10, col3, flags);
}

//Fill an oval or circle using various effects
void E2DSurface::FillOval(long x, long y, long xradius, long yradius, COLORREF col, long flags, unsigned long property)
{
	if (Lock() == NULL)
		return;
	long yctr, sinv, cosv, prevcosv;	
	yradius = ClassEMath::Abs(yradius);
	xradius = ClassEMath::Abs(xradius);
	prevcosv = 999999;
	for (yctr = 0; yctr < 1024; yctr++)
	{
		cosv = (ecd->math->Cos(yctr) * yradius) >> 16;
		if (prevcosv > cosv)
		{
			if (prevcosv != 999999)
			{
				sinv = (ecd->math->Sin(yctr) * xradius) >> 16;
				DrawLine(x - sinv, y - cosv, x + sinv, y - cosv, col, flags, property);
				if (cosv != 0)
					DrawLine(x - sinv, y + cosv, x + sinv, y + cosv, col, flags, property);
			}
			prevcosv = cosv;
		}
	}
	Unlock();
}

//Fill a rectangle using various effects
void E2DSurface::FillRect(RECT *rc, COLORREF col, long flags, unsigned long property)
{
	FillRectV(rc, col, flags, property, true);
}

//Fill a rectangle with a gradient
void E2DSurface::GradientRect(RECT *rc, COLORREF topleft, COLORREF topright, COLORREF bottomleft, COLORREF bottomright, long flags)
{
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return;
	RECT nrc;
	if (rc != NULL)
	{
		ClassEMemory::Copy(&nrc, rc, sizeof(RECT));
		ecd->dsp->VirtualRect(&nrc, virtualwidth, sheight);
		if (!ClipRect(&nrc))
		{
			Unlock();
			return;
		}
		ecd->dsp->ReVirtualRect(&nrc, virtualwidth, sheight);
	}
	else
	{
		SetRect(&nrc, 0, 0, virtualwidth, sheight);
		ecd->dsp->ReVirtualRect(&nrc, virtualwidth, sheight);
	}
	GradientPoly(nrc.left, nrc.top, topleft, nrc.right, nrc.top, topright, nrc.left, nrc.bottom, bottomleft, flags);
	GradientPoly(nrc.right, nrc.top, topright, nrc.right, nrc.bottom, bottomright, nrc.left, nrc.bottom, bottomleft, flags);
	Unlock();
}

//Horizontal gradient
void E2DSurface::GradientRectH(RECT *rc, COLORREF left, COLORREF right, long flags){ GradientRect(rc, left, right, left, right, flags); }

//Vertical gradient
void E2DSurface::GradientRectV(RECT *rc, COLORREF top, COLORREF bottom, long flags){ GradientRect(rc, top, top, bottom, bottom, flags); }

//Make a fast blit
ERESULT E2DSurface::BltFast(long x, long y, E2DSurface *source, RECT *rc, long flags, unsigned long property)
{
	long colorflags = flags;
	COLORREF colorize;
	unsigned char opacity;
	if (!this->CheckFilterBlt(colorflags, colorize, opacity, property))
		return(E_NOACTION);
	dirty = true;
	flags = flags - (flags & EFX_COLORFILTER) + (colorflags & EFX_COLORFILTER);
	if ((colorflags & (EFX_COLORIZE | EFX_COLORKEY)) == EFX_COLORFILL)
	{
		RECT rrc;
		if (rc != NULL)
			SetRect(&rrc, x, y, x + rc->right - rc->left, y + rc->bottom - rc->top);
		else
			SetRect(&rrc, x, y, x + source->GetWidth(), y + source->GetHeight());
		FillRect(&rrc, colorize, flags - EFX_COLORFILL, opacity);
		return(E_OK);
	}
	ERESULT result;
	RECT nrc;
	result = GetBltFastPositions(x, y, &nrc, rc, source);
	if (result != E_OK)
		return(result);
	if (nativesurf != NULL && locked == 0 && source->nativesurf != NULL && source->locked == 0)
		if ((ecd->dispcfg->dspnative && !(flags & E2D_PREVENTNATIVE)) || (flags & E2D_ALLOWNATIVE))
		{
			long n_pos[] = { x, y };
			long n_rect[] = { nrc.left, nrc.top, nrc.right, nrc.bottom };
			unsigned long n_flags = 0;
			if (colorflags & EFX_COLORKEY)
				n_flags |= 1;
			if ((colorflags & (EFX_COLORFILTER - EFX_COLORKEY)) == 0)
				if (ecDspNativeBltFast(ecd->dsp->objdsp, nativesurf, source->nativesurf, n_pos, n_rect, n_flags))
					return(E_OK);
		}
	unsigned char *memptr1 = Lock(), *memptr2 = source->Lock();
	if (memptr1 == NULL || memptr2 == NULL)
	{
		if (memptr1 != NULL)
			Unlock();
		if (memptr2 != NULL)
			source->Unlock();
		return(E_LOCKFAILED);
	}
	if (videomode != source->videomode)
	{
		if (colorflags != 0)
		{
			COLORREF ck = source->GetColorKey();
			for (long yc = 0; yc < nrc.bottom - nrc.top; yc++) for (long xc = 0; xc < nrc.right - nrc.left; xc++)
			{
				COLORREF col = source->GetPixelRaw(nrc.left + xc, nrc.top + yc, memptr2);
				if ((!(colorflags & EFX_COLORKEY)) || col != ck)
				{
					if (source->videomode == EDSP_INDEXED8)
						col = ecd->dsp->videopalette[col];
					PutPixelRaw(x + xc, y + yc, col, flags, property, memptr1);
				}
			}
		}
		else
			ExBltColorConvert(&memptr1[y * vpitch + x * (bitwidth >> 3)], &memptr2[nrc.top * source->vpitch + nrc.left * (source->bitwidth >> 3)], swidth, source->swidth, videomode, source->videomode, nrc.right - nrc.left, nrc.bottom - nrc.top, ecd->dsp->videopalette, ecd->dsp->quickpalette);
		Unlock();
		source->Unlock();
		return(E_OK);
	}
	if (colorflags == 0)
	{
		long yctr = nrc.top;
		if (nrc.right - nrc.left != vpitch || nrc.right - nrc.left != source->vpitch)
		{
			while(yctr < nrc.bottom)
			{
				ClassEMemory::Copy(&memptr1[y * vpitch + x * (bitwidth >> 3)], &memptr2[yctr * source->vpitch + nrc.left * (bitwidth >> 3)], (nrc.right - nrc.left) * (bitwidth >> 3));
				yctr++;
				y++;
			}
		}
		else
			ClassEMemory::Copy(&memptr1[y * vpitch + x * (bitwidth >> 3)], &memptr2[yctr * source->vpitch + nrc.left * (bitwidth >> 3)], swidth * (nrc.bottom - nrc.top) * (bitwidth >> 3));
		Unlock();
		source->Unlock();
		return(E_OK);
	}
	memptr1 = &memptr1[y * vpitch + x * bitwidth / 8];
	memptr2 = &memptr2[nrc.top * source->vpitch + nrc.left * bitwidth / 8];
	if (colorflags & EFX_OPACITY)
	{
		if (opacity == 128 || opacity == 192 || opacity == 224 || opacity == 240)
			colorflags = ((colorflags & (EFX_COLORFILTER - EFX_OPACITY)) | EFX_OPACITY_OVH);
		else if (opacity == 64 || opacity == 32 || opacity == 16)
			colorflags = ((colorflags & (EFX_COLORFILTER - EFX_OPACITY)) | EFX_OPACITY_OVL);
	}
	if ((flags & EFX_COLORIZE) == EFX_TINT && videomode != EDSP_INDEXED8)
		colorize = (colorize >> 1) & 0x7F7F7F;
	ConvertColor(colorize, videomode);
	if (videomode == EDSP_INDEXED8)
		ERdrBltFast8(nrc.right - nrc.left, nrc.bottom - nrc.top, memptr1, memptr2, swidth * bitwidth / 8, source->swidth * bitwidth / 8, source->colorkey, colorize, opacity, &ecd->dsp->edgerender[ERDRF_BLTFAST * 256], colorflags);
	else if (videomode == EDSP_TRUE12)
		ERdrBltFast12(nrc.right - nrc.left, nrc.bottom - nrc.top, memptr1, memptr2, swidth * bitwidth / 8, source->swidth * bitwidth / 8, source->colorkey, colorize, opacity, &ecd->dsp->edgerender[ERDRF_BLTFAST * 256], colorflags);
	else if (videomode == EDSP_TRUE16)
		ERdrBltFast16(nrc.right - nrc.left, nrc.bottom - nrc.top, memptr1, memptr2, swidth * bitwidth / 8, source->swidth * bitwidth / 8, source->colorkey, colorize, opacity, &ecd->dsp->edgerender[ERDRF_BLTFAST * 256], colorflags);
	else if (videomode == EDSP_TRUE32 || videomode == EDSP_TRUE32_INV)
		ERdrBltFast32(nrc.right - nrc.left, nrc.bottom - nrc.top, memptr1, memptr2, swidth * bitwidth / 8, source->swidth * bitwidth / 8, source->colorkey, colorize, opacity, &ecd->dsp->edgerender[ERDRF_BLTFAST * 256], colorflags);
	Unlock();
	source->Unlock();
	return(E_OK);
}

//Make a fast blit with an RGBA source surface
ERESULT E2DSurface::BltFast(long x, long y, E2DSurfaceRGBA *source, RECT *rc, long flags, unsigned long property)
{
	long colorflags = flags;
	if (!this->CheckFilterBlt(colorflags, property))
		return(E_NOACTION);
	flags = flags - (flags & EFX_COLORFILTER) + (colorflags & EFX_COLORFILTER);
	RECT nrc;
	ERESULT result;
	result = GetBltFastPositions(x, y, &nrc, rc, source);
	if (result != E_OK)
		return(result);
	dirty = true;
	unsigned char *memptr1 = Lock(), *memptr2 = source->Lock();
	if (memptr1 == NULL || memptr2 == NULL)
	{
		if (memptr1 != NULL)
			Unlock();
		if (memptr2 != NULL)
			source->Unlock();
		return(E_LOCKFAILED);
	}
	if (videomode != source->videomode)
	{
		unsigned long opacprop = property;
		if (colorflags & EFX_COLORIZE)
		{
			opacprop >>= 24;
			property &= 0xFFFFFF;
		}
		for (long yc = 0; yc < nrc.bottom - nrc.top; yc++) for (long xc = 0; xc < nrc.right - nrc.left; xc++)
		{
			COLORREF col;
			unsigned char alpha;
			source->GetPixelV(nrc.left + xc, nrc.top + yc, col, alpha);
			if (colorflags & EFX_OPACITY)
				alpha = (unsigned char)((alpha * opacprop) >> 8);
			if (alpha)
			{
				if (colorflags & EFX_COLORIZE)
					PutPixelRaw(x + xc, y + yc, col, flags | EFX_OPACITY, property | (alpha << 24), memptr1);
				else
					PutPixelRaw(x + xc, y + yc, col, flags | EFX_OPACITY, alpha, memptr1);
			}
		}
		Unlock();
		source->Unlock();
		return(E_OK);
	}
	if (colorflags & EFX_COLOREFFECT)
	{
		COLORREF prop2 = 0;
		if (colorflags & EFX_COLORIZE)
		{
			if ((colorflags & EFX_COLORIZE) == EFX_TINT)
				prop2 = (property >> 1) & 0x7F7F7F;
			else
				prop2 = property & 0xFFFFFF;
			ConvertColor(prop2, videomode);
			property >>= 24;
		}
		if (source->currenttype & EST_READONLY)
			ExBltRGBAGenericPlus(x, y, nrc.top, &nrc, memptr1, memptr2, swidth, source->swidth, videomode, flags, property, prop2);
		else
			ExBltRGBAGeneric(x, y, nrc.top, &nrc, memptr1, memptr2, swidth, source->swidth, videomode, flags, property, prop2);
	}
	else
	{
		if (source->currenttype & EST_READONLY)
			ExBltRGBAPlus(x, y, nrc.top, &nrc, memptr1, memptr2, swidth, source->swidth, videomode);
		else
			ExBltRGBA(x, y, nrc.top, &nrc, memptr1, memptr2, swidth, source->swidth, videomode);
	}
	Unlock();
	source->Unlock();
	return(E_OK);
}

//Make a fast blit using a custom pixel shader
ERESULT E2DSurface::BltPixelShader(long x, long y, E2DSurface *source, RECT *rc, EPIXELSHADER, long flags, unsigned long property)
{
	#if (EDITION_SDK < 1000)
		return(E_NOFEATURE);
	#endif
	if (source->videomode != videomode)
		return(E_TYPEMISMATCH);
	RECT nrc;
	ERESULT result;
	result = GetBltFastPositions(x, y, &nrc, rc, source);
	if (result != E_OK)
		return(result);
	dirty = true;
	unsigned char *memptr1 = Lock(), *memptr2 = source->Lock();
	if (memptr1 == NULL || memptr2 == NULL)
	{
		if (memptr1 != NULL)
			Unlock();
		if (memptr2 != NULL)
			source->Unlock();
		return(E_LOCKFAILED);
	}
	if (flags & EFX_COLORKEY)
		MemBltColorkey(shader, x, y, memptr1, memptr2, &nrc, swidth, source->swidth, videomode, property, source->colorkey);
	else
		MemBlt(shader, x, y, memptr1, memptr2, &nrc, swidth, source->swidth, videomode, property);
	Unlock();
	source->Unlock();
	return(E_OK);
}

//Blit one surface to another with a possibility to rotate and scale
ERESULT E2DSurface::Blt(long center_x, long center_y, E2DSurface *source, RECT *rc, unsigned short rotation, unsigned long scale, unsigned char flipflags, long flags, unsigned long property)
{
	E2DBLTFX tmpfx;
	tmpfx.flags = flags;
	tmpfx.flipflags = flipflags;
	tmpfx.property = property;
	tmpfx.rotation = rotation;
	tmpfx.xscale = scale;
	tmpfx.yscale = scale;
	return(BltFx(center_x, center_y, source, rc, &tmpfx));
}

//Blit with several effects
ERESULT E2DSurface::BltFx(long center_x, long center_y, E2DSurface *source, RECT *rc, E2DBLTFX *bltfx)
{
	dirty = true;
	E2DBLTFX tmpfx;
	E2DBLTFX *usefx = &tmpfx;
	long bltflipx = 0, bltflipy = 0;
	if (bltfx == NULL)
	{
		tmpfx.flags = EFX_NONE;
		tmpfx.flipflags = EFLIP_NONE;
		tmpfx.property = 0;
		tmpfx.rotation = 0;
		tmpfx.xscale = 65536;
		tmpfx.yscale = 65536;
	}
	else
		ClassEMemory::Copy(&tmpfx, bltfx, sizeof(E2DBLTFX));
	if (usefx->flipflags == EFLIP_NONE && usefx->rotation == 0 && usefx->xscale == 65536 && usefx->yscale == 65536)
	{
		if (rc == NULL)
			return(BltFast(center_x - source->GetWidth() / 2, center_y - source->GetHeight() / 2, source, rc, usefx->flags, usefx->property));
		return(BltFast(center_x - (rc->right - rc->left) / 2, center_y - (rc->bottom - rc->top) / 2, source, rc, usefx->flags, usefx->property));
	}
	if (usefx->rotation % 1024 == 0 && usefx->xscale == 65536 && usefx->yscale == 65536)
	{
		if (rc == NULL)
		{
			if (usefx->rotation % 2048 == 0)
			{
				bltflipx = center_x - source->GetWidth() / 2;
				bltflipy = center_y - source->GetHeight() / 2;
			}
			else
			{
				bltflipx = center_x - source->GetHeight() / 2;
				bltflipy = center_y - source->GetWidth() / 2;
			}
		}
		else
		{
			if (usefx->rotation % 2048 == 0)
			{
				bltflipx = center_x - (rc->right - rc->left) / 2;
				bltflipy = center_y - (rc->bottom - rc->top) / 2;
			}
			else
			{
				bltflipx = center_x - (rc->bottom - rc->top) / 2;
				bltflipy = center_y - (rc->right - rc->left) / 2;
			}
		}
	}
	long colorflags = usefx->flags;
	if (!this->CheckFilterBlt(colorflags, usefx->property))
		return(E_NOACTION);
	usefx->flags = usefx->flags - (usefx->flags & EFX_COLORFILTER) + (colorflags & EFX_COLORFILTER);
	if (source->videomode != videomode)
		return(E_TYPEMISMATCH);
	if (nativesurf != NULL && locked == 0 && source->nativesurf != NULL && source->locked == 0)
		if ((ecd->dispcfg->dspnative && !(usefx->flags & E2D_PREVENTNATIVE)) || (usefx->flags & E2D_ALLOWNATIVE))
		{
			RECT t_rc;
			if (rc == NULL)
				SetRect(&t_rc, 0, 0, source->GetWidth(), source->GetHeight());
			else
				ClassEMemory::Copy(&t_rc, rc, sizeof(RECT));
			long n_pos[] = { center_x, center_y };
			long n_rect[] = { t_rc.left, t_rc.top, t_rc.right, t_rc.bottom };
			long n_clip[] = { cliprc.left, cliprc.top, cliprc.right, cliprc.bottom };
			long n_scale[] = { usefx->xscale, usefx->yscale };
			unsigned long n_flags = 0;
			if (colorflags & EFX_COLORKEY)
				n_flags |= 1;
			if (usefx->flipflags & EFLIP_HORIZONTAL)
				n_flags |= 256;
			if (usefx->flipflags & EFLIP_VERTICAL)
				n_flags |= 512;
			if ((colorflags & (EFX_COLORFILTER - EFX_COLORKEY)) == 0)
				if (ecDspNativeBlt(ecd->dsp->objdsp, nativesurf, source->nativesurf, n_pos, n_rect, n_clip, n_scale, usefx->rotation, n_flags))
					return(E_OK);
		}
	if (usefx->rotation % 1024 == 0 && usefx->xscale == 65536 && usefx->yscale == 65536)
		return(BltFlip(bltflipx, bltflipy, source, rc, (unsigned short)usefx->rotation, usefx->flipflags, usefx->flags, usefx->property));
	unsigned char *memptr1 = Lock(), *memptr2 = source->Lock();
	if (memptr1 == NULL || memptr2 == NULL)
	{
		if (memptr1 != NULL)
			Unlock();
		if (memptr2 != NULL)
			source->Unlock();
		return(E_LOCKFAILED);
	}
	RECT nrc;
	if (rc == NULL)
	{
		nrc.left   = 0;
		nrc.top    = 0;
		nrc.right  = source->GetWidth();
		nrc.bottom = source->GetHeight();
	}
	else
		ClassEMemory::Copy(&nrc, rc, sizeof(RECT));
	long srcwidth, srcheight;
	if ((nrc.right - nrc.left) % 2 == 1)
		center_x++;
	if ((nrc.bottom - nrc.top) % 2 == 1)
		center_y++;
	ecd->dsp->VirtualRect(&nrc, source->virtualwidth, source->sheight);
	ecd->dsp->VirtualPos(center_x, center_y, virtualwidth, sheight);
	if (ecd->dsp->switchxy)
	{
		long xyfx_tmp = usefx->flipflags;
		usefx->flipflags = 0;
		if (xyfx_tmp & EFLIP_HORIZONTAL)
			usefx->flipflags |= EFLIP_VERTICAL;
		if (xyfx_tmp & EFLIP_VERTICAL)
			usefx->flipflags |= EFLIP_HORIZONTAL;
		xyfx_tmp = usefx->xscale;
		usefx->xscale = usefx->yscale;
		usefx->yscale = xyfx_tmp;
	}
	srcwidth  = nrc.right - nrc.left;
	srcheight = nrc.bottom - nrc.top;
	if (!source->ClipRect(&nrc))
	{
		Unlock();
		source->Unlock();
		return(E_INVALIDPARAM);
	}
	EINT32 px[4], py[4];
	EINT32 tmpx, tmpy, yt = sheight << 10, yb = -1 << 10;
	unsigned char ctr;
	px[1] = (srcwidth * (long)usefx->xscale) >> 15;
	px[2] = px[1];
	px[0] = px[1] - ((srcwidth * (long)usefx->xscale) >> 14);
	px[3] = px[0];
	py[2] = (srcheight * (long)usefx->yscale) >> 15;
	py[3] = py[2];
	py[0] = py[2] - ((srcheight * (long)usefx->yscale) >> 14);
	py[1] = py[0];
	for (ctr = 0; ctr < 4; ctr++)
	{
		if (usefx->flipflags & EFLIP_HORIZONTAL)
			px[ctr] = -px[ctr];
		if (usefx->flipflags & EFLIP_VERTICAL)
			py[ctr] = -py[ctr];
		tmpx = px[ctr];
		tmpy = py[ctr];
		px[ctr] = (center_x << 10) + ((tmpx * ecd->math->Cos(usefx->rotation)) >> 8) - ((tmpy * ecd->math->Sin(usefx->rotation)) >> 8);
		py[ctr] = (center_y << 10) + ((tmpx * ecd->math->Sin(usefx->rotation)) >> 8) + ((tmpy * ecd->math->Cos(usefx->rotation)) >> 8);
		if (yt > py[ctr])
			yt = py[ctr];
		if (yb < py[ctr])
			yb = py[ctr];
	}
	yt >>= 10;
	yb = (yb >> 10) - 1;
	if (yt < cliprc.top)
		yt = cliprc.top;
	if (yb >= cliprc.bottom)
		yb = cliprc.bottom - 1;
	if (yb < yt)
	{
		Unlock();
		source->Unlock();
		return(E_INVALIDPARAM);
		
	}
	ecd->mem->Set32(&ledge[yt], virtualwidth << 10, yb - yt + 1);
	ecd->mem->Set32(&redge[yt], -1           << 10, yb - yt + 1);
	ecd->mem->Set16(&xmin[yt],  virtualwidth,   yb - yt + 1);
	ecd->mem->Set16(&xmax[yt],  (unsigned)(-1), yb - yt + 1);
	ScanPolyLine(px[0], py[0], px[1], py[1], nrc.left  << 8, nrc.top    << 8, nrc.right << 8, nrc.top    << 8);
	ScanPolyLine(px[3], py[3], px[2], py[2], nrc.left  << 8, nrc.bottom << 8, nrc.right << 8, nrc.bottom << 8);
	ScanPolyLine(px[1], py[1], px[2], py[2], nrc.right << 8, nrc.top    << 8, nrc.right << 8, nrc.bottom << 8);
	ScanPolyLine(px[0], py[0], px[3], py[3], nrc.left  << 8, nrc.top    << 8, nrc.left  << 8, nrc.bottom << 8);
	for (tmpy = yt; tmpy <= yb; tmpy++)
	{
		if (xmin[tmpy] < cliprc.right && xmax[tmpy] >= cliprc.left)
		{
			if (xmin[tmpy] < cliprc.left)
				xmin[tmpy] = (short)cliprc.left;
			if (xmax[tmpy] >= cliprc.right)
				xmax[tmpy] = (short)(cliprc.right - 1);
		}
		else
			xmax[tmpy] = -1;
	}
	if (colorflags & EFX_COLOREFFECT)
	{
		if (colorflags & EFX_COLORIZE)
		{
			COLORREF col;
			if ((colorflags & EFX_COLORIZE) == EFX_TINT)
				col = (usefx->property >> 1) & 0x7F7F7F;
			else
				col = usefx->property & 0xFFFFFF;
			ConvertColor(col, videomode);
			ExRsBltGeneric(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode, usefx->flags, source->colorkey, usefx->property >> 24, col);
		}
		else
			ExRsBltGeneric(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode, usefx->flags, source->colorkey, usefx->property, 0);
	}
	else if (colorflags & EFX_COLORKEY)
		ExRsBltCk(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode, source->colorkey);
	else
		ExRsBlt(memptr1, memptr2, xmin, xmax, ledge, redge, swidth, sheight, source->swidth, source->sheight, yt, yb, cliprc.left, bitwidth, source->virtualwidth, videomode);
	Unlock();
	source->Unlock();
	return(E_OK);
}

//Stretch blitter
ERESULT E2DSurface::BltStretch(RECT *dstrc, E2DSurface *source, RECT *srcrc, unsigned char flipflags, long flags, unsigned long property)
{
	E2DBLTFX bltfx;
	RECT drc, src;
	long xdec = 0, ydec = 0;
	dirty = true;
	if (dstrc != NULL)
		SetRect(&drc, dstrc->left, dstrc->top, dstrc->right, dstrc->bottom);
	else
		SetRect(&drc, 0, 0, GetWidth(), GetHeight());
	if (srcrc != NULL)
		SetRect(&src, srcrc->left, srcrc->top, srcrc->right, srcrc->bottom);
	else
		SetRect(&src, 0, 0, source->GetWidth(), source->GetHeight());
	if ((src.right - src.left) % 2 == 1)
		xdec = 1;
	if ((src.bottom - src.top) % 2 == 1)
		ydec = 1;
	bltfx.xscale = ((drc.right - drc.left) * 65536 + 16383) / (src.right - src.left);
	bltfx.yscale = ((drc.bottom - drc.top) * 65536 + 16383) / (src.bottom - src.top);
	bltfx.rotation = 0;
	bltfx.flipflags = flipflags;
	bltfx.flags = flags;
	bltfx.property = property;
	return(BltFx(drc.left + (drc.right - drc.left + 1) / 2 - xdec, drc.top + (drc.bottom - drc.top + 1) / 2 - ydec, source, &src, &bltfx));
}

//Render a 3D surface using various effects
void E2DSurface::Render(long x, long y, E3DSurface *source, unsigned long flags, unsigned long property)
{
	if (source->allocpoly == 0 || source->allocvertex == 0 || source->drawlist == NULL)
		return;
	if (Lock() == NULL)
		return;
	bool calctex = ((flags & E3D_TEXTURE) && source->lofs_texel != 0xFFFFFFFF);
	bool cull = !((flags & (E3D_SHADINGFILTER - E3D_FLAT)) == E3D_WIREFRAME && !calctex);
	bool calcsmoothcol = ((flags & E3D_GOURAUD) == E3D_GOURAUD);
	long *drawarray;
	unsigned long drawpoly = source->PrepareRender((((flags & E3D_GOURAUD) || (flags & E3D_FLAT)) && source->lofs_normal != 0xFFFFFFFF), !calctex, calcsmoothcol, calctex, cull);
	unsigned long pctr, vtxstride = 9 * sizeof(long);
	x = (x << 10) + (GetWidth() << 9);
	y = (y << 10) + (GetHeight() << 9);
	if (calctex)
	{
		vtxstride += 6 * sizeof(long);
		for (pctr = 0; pctr < drawpoly; pctr++)
		{
			drawarray = (long *)&source->drawlist[source->sortindexlist[pctr * 2] * vtxstride];
			if (ecd->dsp->perspectivemapping)
				TexturePolyV(x + (drawarray[0] >> 10), y - (drawarray[1] >> 10), drawarray[2], drawarray[9], drawarray[10], x + (drawarray[3] >> 10), y - (drawarray[4] >> 10), drawarray[5], drawarray[11], drawarray[12], x + (drawarray[6] >> 10), y - (drawarray[7] >> 10), drawarray[8], drawarray[13], drawarray[14], (E2DSurface *)ecd->dsp->usetex, flags, property);
			else
				TexturePolyV(x + (drawarray[0] >> 10), y - (drawarray[1] >> 10), drawarray[9], drawarray[10], x + (drawarray[3] >> 10), y - (drawarray[4] >> 10), drawarray[11], drawarray[12], x + (drawarray[6] >> 10), y - (drawarray[7] >> 10), drawarray[13], drawarray[14], (E2DSurface *)ecd->dsp->usetex, flags, property);
		}
	}
	else if (calcsmoothcol)
	{
		vtxstride += 3 * sizeof(long);
		for (pctr = 0; pctr < drawpoly; pctr++)
		{
			drawarray = (long *)&source->drawlist[source->sortindexlist[pctr * 2] * vtxstride];
			GradientPolyV(x + (drawarray[0] >> 10), y - (drawarray[1] >> 10), drawarray[9], x + (drawarray[3] >> 10), y - (drawarray[4] >> 10), drawarray[10], x + (drawarray[6] >> 10), y - (drawarray[7] >> 10), drawarray[11], flags);
		}
	}
	else
	{
		vtxstride += sizeof(long);
		if (flags & E3D_WIREFRAME)
		{
			x >>= 10;
			y >>= 10;
			for (pctr = 0; pctr < drawpoly; pctr++)
			{
				drawarray = (long *)&source->drawlist[source->sortindexlist[pctr * 2] * vtxstride];
				DrawLine(x + (drawarray[0] >> 20), y - (drawarray[1] >> 20), x + (drawarray[3] >> 20), y - (drawarray[4] >> 20), drawarray[9], flags, property);
				DrawLine(x + (drawarray[3] >> 20), y - (drawarray[4] >> 20), x + (drawarray[6] >> 20), y - (drawarray[7] >> 20), drawarray[9], flags, property);
				DrawLine(x + (drawarray[6] >> 20), y - (drawarray[7] >> 20), x + (drawarray[0] >> 20), y - (drawarray[1] >> 20), drawarray[9], flags, property);
			}
		}
		else
		{
			for (pctr = 0; pctr < drawpoly; pctr++)
			{
				drawarray = (long *)&source->drawlist[source->sortindexlist[pctr * 2] * vtxstride];
				FillPolyV(x + (drawarray[0] >> 10), y - (drawarray[1] >> 10), x + (drawarray[3] >> 10), y - (drawarray[4] >> 10), x + (drawarray[6] >> 10), y - (drawarray[7] >> 10), drawarray[9], flags, property);
			}
		}
	}
	Unlock();
}

//Draw a mode7 landscape
void E2DSurface::Landscape(RECT *viewport, long pitch, long mapx, long mapy, unsigned short turn, short horizon, unsigned short scale, E2DSurface *source)
{
	if (source->videomode != videomode)
		return;
	unsigned char *memptr1 = Lock(), *memptr2 = source->Lock();
	if (memptr1 == NULL || memptr2 == NULL)
	{
		if (memptr1 != NULL)
			Unlock();
		if (memptr2 != NULL)
			source->Unlock();
		return;
	}
	RECT nrc;
	if (viewport == NULL)
	{
		nrc.left   = 0;
		nrc.top    = 0;
		nrc.right  = GetWidth();
		nrc.bottom = GetHeight();
	}
	else
		ClassEMemory::Copy(&nrc, viewport, sizeof(RECT));
	ecd->dsp->VirtualRotation(turn);
	ecd->dsp->VirtualRect(&nrc, virtualwidth, sheight);
	ecd->dsp->VirtualPos(mapx, mapy, source->virtualwidth * 16, source->sheight * 16);
	if (!ClipRect(&nrc))
	{
		Unlock();
		source->Unlock();
		return;
	}
	bool lineswitch = false;
	long yctr, width, height, xadd, yadd, zxadd, zyadd, xpos, ypos, orgxadd, orgyadd, div;
	if (ecd->dsp->switchxy)
	{
		if (horizon * ecd->dsp->orienth < 0)
			lineswitch = true;
	}
	else
	{
		if (horizon * ecd->dsp->orientv < 0)
			lineswitch = true;
	}
	if (ecd->dsp->switchxy)
		width = nrc.bottom - nrc.top;
	else
		width = nrc.right - nrc.left;
	height = ecd->dsp->rendersize;
	xadd = (ecd->math->Cos(turn) << 4) / (long)scale;
	yadd = (ecd->math->Sin(turn) << 4) / (long)scale;
	orgxadd = xadd;
	orgyadd = yadd;
	zxadd = yadd;
	zyadd = -xadd;
	if (ecd->dsp->switchxy)
		yctr = nrc.right;
	else
		yctr = nrc.bottom;
	if (ecd->dsp->switchxy)
		div = ((height << 10) - (nrc.right - yctr) * ClassEMath::Abs(horizon));
	else
		div = ((height << 10) - (nrc.bottom - yctr) * ClassEMath::Abs(horizon));
	if (div > 0)
	{
		xadd  = ((orgxadd * height) << 8) / div;
		yadd  = ((orgyadd * height) << 8) / div;
		zxadd = ((yadd    * height) << 8) / div;
		zyadd = ((-xadd   * height) << 8) / div;
	}
	xpos  = (long)(mapx * 256) + pitch * zxadd;
	ypos  = (long)(mapy * 256) + pitch * zyadd;
	while((ecd->dsp->switchxy && yctr > nrc.left) || (!ecd->dsp->switchxy && yctr > nrc.top))
	{
		yctr--;
		xpos -= xadd * (width >> 1);
		ypos -= yadd * (width >> 1);
		if (lineswitch)
		{
			if (ecd->dsp->switchxy)
			{
				ledge[(nrc.right - 1 - yctr + nrc.left) * 4]     = xpos;
				ledge[(nrc.right - 1 - yctr + nrc.left) * 4 + 1] = ypos;
				ledge[(nrc.right - 1 - yctr + nrc.left) * 4 + 2] = xadd;
				ledge[(nrc.right - 1 - yctr + nrc.left) * 4 + 3] = yadd;
			}
			else
			{
				ledge[(nrc.bottom - 1 - yctr + nrc.top) * 4]     = xpos;
				ledge[(nrc.bottom - 1 - yctr + nrc.top) * 4 + 1] = ypos;
				ledge[(nrc.bottom - 1 - yctr + nrc.top) * 4 + 2] = xadd;
				ledge[(nrc.bottom - 1 - yctr + nrc.top) * 4 + 3] = yadd;
			}
		}
		else
		{
			ledge[yctr * 4] = xpos;
			ledge[yctr * 4 + 1] = ypos;
			ledge[yctr * 4 + 2] = xadd;
			ledge[yctr * 4 + 3] = yadd;
		}
		if (ecd->dsp->switchxy)
			div = ((height << 10) - (nrc.right - yctr) * ClassEMath::Abs(horizon));
		else
			div = ((height << 10) - (nrc.bottom - yctr) * ClassEMath::Abs(horizon));
		if (div <= 0)
		{
			if (ecd->dsp->switchxy)
			{
				if (lineswitch)
					nrc.right = nrc.right - yctr;
				else
					nrc.left = yctr;
			}
			else
			{
				if (lineswitch)
					nrc.bottom = nrc.bottom - yctr;
				else
					nrc.top = yctr;
			}
			break;
		}
		xpos += xadd * (width >> 1);
		ypos += yadd * (width >> 1);
		xadd  = ((orgxadd * height) << 8) / div;
		yadd  = ((orgyadd * height) << 8) / div;
		zxadd = ((yadd    * height) << 8) / div;
		zyadd = ((-xadd   * height) << 8) / div;
		xpos += ((zxadd >> 16) * (ClassEMath::Abs(horizon) + 256)) << 8;
		ypos += ((zyadd >> 16) * (ClassEMath::Abs(horizon) + 256)) << 8;
		xpos += ((zxadd & 65535) * (ClassEMath::Abs(horizon) + 256)) >> 8;
		ypos += ((zyadd & 65535) * (ClassEMath::Abs(horizon) + 256)) >> 8;
	}
	if (ecd->dsp->switchxy && ecd->dsp->orientv < 0)
		for (yctr = nrc.left; yctr < nrc.right; yctr++)
		{
			ledge[yctr * 4]     += ledge[yctr * 4 + 2] * (width - 1);
			ledge[yctr * 4 + 1] += ledge[yctr * 4 + 3] * (width - 1);
			ledge[yctr * 4 + 2] = -ledge[yctr * 4 + 2];
			ledge[yctr * 4 + 3] = -ledge[yctr * 4 + 3];
		}
	if (!ecd->dsp->switchxy && ecd->dsp->orienth < 0)
		for (yctr = nrc.top; yctr < nrc.bottom; yctr++)
		{
			ledge[yctr * 4]     += ledge[yctr * 4 + 2] * (width - 1);
			ledge[yctr * 4 + 1] += ledge[yctr * 4 + 3] * (width - 1);
			ledge[yctr * 4 + 2] = -ledge[yctr * 4 + 2];
			ledge[yctr * 4 + 3] = -ledge[yctr * 4 + 3];
		}
	if (viewport != NULL)
	{
		ClassEMemory::Copy(viewport, &nrc, sizeof(RECT));
		ecd->dsp->ReVirtualRect(viewport, virtualwidth, sheight);
	}
	if (ecd->dsp->switchxy)
		ExLndSide(&nrc, memptr1, memptr2, swidth, sheight, source->swidth, source->sheight, ledge, bitwidth, source->virtualwidth);
	else
		ExLndNormal(&nrc, memptr1, memptr2, swidth, sheight, source->swidth, source->sheight, ledge, bitwidth, source->virtualwidth);
	Unlock();
	source->Unlock();
}

bool E2DSurface::DirtyFlush()
{
	bool olddirty = dirty;
	dirty = false;
	return olddirty;
}
