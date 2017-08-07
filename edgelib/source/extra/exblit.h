// exblit.h
// Optimized filter blitters for rotate/scale or variable shapes
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Helper functions
///////////////////////////////////////////////////////////////////

//Return if the value is a power 2 size (for optimized routines)
bool ExRsBlt_IsPower2(long v)
{
	long sv = 2;
	while (sv < 8192)
	{
		if (v == sv)
			return(true);
		else if (v < sv)
			return(false);
		sv <<= 1;
	}
	return(false);
}

///////////////////////////////////////////////////////////////////
// Rotate/Scale and variable shape blitters                      //
///////////////////////////////////////////////////////////////////

//Blit: optimized default function, handling all filters
void ExRsBltGenericPlus(unsigned char *dst, unsigned char *src, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned long virtwidth, unsigned char videomode, long flags, COLORREF colorkey, long property1, long property2)
{
	bool combinedalpha = false;
	long xctr, yctr, w, bc = 256 - property1;
	unsigned long op_andu = srcwidth, op_andv = srcheight, op_shl = 0;
	while (op_andu > 1)
	{
		op_andu /= 2;
		op_shl++;
	}
	op_andu = srcwidth - 1;
	op_andv = srcheight - 1;
	if ((flags & EFX_OPACITY) && (flags & (EFX_SUB | EFX_ALPHA)))
	{
		flags -= EFX_OPACITY;
		combinedalpha = true;
	}
	if (videomode == EDSP_TRUE12)
	{
		property1 >>= 4;
		bc >>= 4;
	}
	else if (videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
	{
		property1 >>= 3;
		bc >>= 3;
	}
	for (yctr = ytop; yctr <= ybottom; yctr++)
	{
		unsigned long sc = 0;
		long su = 0, sv = 0, eu = 0, ev = 0;
		if (xmax[yctr] >= 0)
		{
			long x1, x2, delta_x, fix;
			x1 = ledge[yctr]; x2 = redge[yctr];
			delta_x = ((x2 - x1) >> 1) + 1;
			su = ledge[dstheight + yctr];
			eu = ((redge[dstheight + yctr] - su) << 9) / delta_x;
			sv = ledge[(dstheight << 1) + yctr];
			ev = ((redge[(dstheight << 1) + yctr] - sv) << 9) / delta_x;
			fix = 1024 - (x1 & 1023);
			su += (eu * fix) >> 10;
			sv += (ev * fix) >> 10;
			if ((x1 >> 10) < clipleft)
			{
				su += (clipleft - (x1 >> 10)) * eu;
				sv += (clipleft - (x1 >> 10)) * ev;
			}
			w = xmax[yctr] + 1 - xmin[yctr];
		}
		else
			w = -1;
		if (bitwidth == 8)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr])];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				*pdd = psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *pdd = (unsigned short *)&dst[((yctr * dstwidth) + xmin[yctr]) * 2];
			unsigned short *psd = (unsigned short *)src;
			if (videomode == EDSP_TRUE12)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sc = psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 1911, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					su += eu;
					sv += ev;
				}
			}
			else if (videomode == EDSP_TRUE15)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sc = psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 992, 31744, 0, 5, 10, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 15855, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 31775, 992, 5, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 31775, 992, 5, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 992, 31744, 32, 1024, 32768);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 992, 31744);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					su += eu;
					sv += ev;
				}
			}
			else if (videomode == EDSP_TRUE16)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sc = psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 31727, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					su += eu;
					sv += ev;
				}
			}
		}
		else if (bitwidth == 24 || bitwidth == 32)
		{
			unsigned long dc = 0;
			unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[((yctr * dstwidth) + xmin[yctr]) * 4];
			unsigned EINT32 *psd32 = (unsigned EINT32 *)src;
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr]) * 3];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				if (bitwidth == 24)
				{
					ClassEMemory::Copy(&sc, &psd[((((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)) * 3], 3);
					ClassEMemory::Copy(&dc, pdd, 3);
				}
				else
				{
					sc = psd32[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)];
					dc = *pdd32;
				}
				if (!(flags & EFX_COLORKEY) || sc != colorkey)
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, property2, unsigned long)
					else if (flags & EFX_COLORFILL)
						sc = property2;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, property2);
					if (combinedalpha)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, property1);
					if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY(dc, sc, 0xFF00FF, 0xFF00, 8, bc, property1);
					else if (flags & EFX_ALPHA)
						sc = EBCODE_MACRO_ADDALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
					else if (flags & EFX_SUB)
						sc = EBCODE_MACRO_SUBALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000);
					if (bitwidth == 24)
						ClassEMemory::Copy(pdd, &sc, 3);
					else
						*pdd32 = sc;
				}
				pdd += 3;
				pdd32++;
				su += eu;
				sv += ev;
			}
		}
	}
}

//Blit: default function, handling all filters
void ExRsBltGeneric(unsigned char *dst, unsigned char *src, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned long virtwidth, unsigned char videomode, long flags, COLORREF colorkey, long property1, long property2)
{
	if (ExRsBlt_IsPower2(virtwidth) && ExRsBlt_IsPower2(srcheight)){ ExRsBltGenericPlus(dst, src, xmin, xmax, ledge, redge, dstwidth, dstheight, srcwidth, srcheight, ytop, ybottom, clipleft, bitwidth, virtwidth, videomode, flags, colorkey, property1, property2); return; }
	bool combinedalpha = false;
	long xctr, yctr, w, bc = 256 - property1;
	if ((flags & EFX_OPACITY) && (flags & (EFX_SUB | EFX_ALPHA)))
	{
		flags -= EFX_OPACITY;
		combinedalpha = true;
	}
	if (videomode == EDSP_TRUE12)
	{
		property1 >>= 4;
		bc >>= 4;
	}
	else if (videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
	{
		property1 >>= 3;
		bc >>= 3;
	}
	for (yctr = ytop; yctr <= ybottom; yctr++)
	{
		unsigned long sc = 0;
		long su = 0, sv = 0, eu = 0, ev = 0;
		if (xmax[yctr] >= 0)
		{
			long x1, x2, delta_x, fix;
			x1 = ledge[yctr]; x2 = redge[yctr];
			delta_x = ((x2 - x1) >> 1) + 1;
			su = ledge[dstheight + yctr];
			eu = ((redge[dstheight + yctr] - su) << 9) / delta_x;
			sv = ledge[(dstheight << 1) + yctr];
			ev = ((redge[(dstheight << 1) + yctr] - sv) << 9) / delta_x;
			fix = 1024 - (x1 & 1023);
			su += (eu * fix) >> 10;
			sv += (ev * fix) >> 10;
			if ((x1 >> 10) < clipleft)
			{
				su += (clipleft - (x1 >> 10)) * eu;
				sv += (clipleft - (x1 >> 10)) * ev;
			}
			w = xmax[yctr] + 1 - xmin[yctr];
		}
		else
			w = -1;
		if (bitwidth == 8)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr])];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				*pdd = psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *pdd = (unsigned short *)&dst[((yctr * dstwidth) + xmin[yctr]) * 2];
			unsigned short *psd = (unsigned short *)src;
			if (videomode == EDSP_TRUE12)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sc = psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 1911, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					su += eu;
					sv += ev;
				}
			}
			else if (videomode == EDSP_TRUE15)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sc = psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 992, 31744, 0, 5, 10, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 15855, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 31775, 992, 5, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 31775, 992, 5, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 992, 31744, 32, 1024, 32768);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 992, 31744);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					su += eu;
					sv += ev;
				}
			}
			else if (videomode == EDSP_TRUE16)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sc = psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 31727, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					su += eu;
					sv += ev;
				}
			}
		}
		else if (bitwidth == 24 || bitwidth == 32)
		{
			unsigned long dc = 0;
			unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[((yctr * dstwidth) + xmin[yctr]) * 4];
			unsigned EINT32 *psd32 = (unsigned EINT32 *)src;
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr]) * 3];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				if (bitwidth == 24)
				{
					ClassEMemory::Copy(&sc, &psd[((((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)) * 3], 3);
					ClassEMemory::Copy(&dc, pdd, 3);
				}
				else
				{
					sc = psd32[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)];
					dc = *pdd32;
				}
				if (!(flags & EFX_COLORKEY) || sc != colorkey)
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, property2, unsigned long)
					else if (flags & EFX_COLORFILL)
						sc = property2;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, property2);
					if (combinedalpha)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, property1);
					if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY(dc, sc, 0xFF00FF, 0xFF00, 8, bc, property1);
					else if (flags & EFX_ALPHA)
						sc = EBCODE_MACRO_ADDALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
					else if (flags & EFX_SUB)
						sc = EBCODE_MACRO_SUBALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000);
					if (bitwidth == 24)
						ClassEMemory::Copy(pdd, &sc, 3);
					else
						*pdd32 = sc;
				}
				pdd += 3;
				pdd32++;
				su += eu;
				sv += ev;
			}
		}
	}
}

//Blit: gradient and dithering
void ExRsBltGradientDith(unsigned char *dst, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned char videomode)
{
	unsigned EINT32 *pdd32;
	unsigned short *pdd16;
	unsigned char *pdd8;
	long xctr, yctr, w;
	for (yctr = ytop; yctr < ybottom + 1; yctr++)
	{
		long rd = 0, gd = 0, bd = 0, r = 0, g = 0, b = 0, dithr, dithg, dithb;
		if (xmax[yctr] >= 0)
		{
			long x1, x2, delta_x, fix;
			x1 = ledge[yctr]; x2 = redge[yctr];
			delta_x = ((x2 - x1) >> 1) + 1;
			r  = ledge[dstheight + yctr];
			rd = ((redge[dstheight + yctr] - r) << 9) / delta_x;
			g  = ledge[(dstheight << 1) + yctr];
			gd = ((redge[(dstheight << 1) + yctr] - g) << 9) / delta_x;
			b  = ledge[(dstheight * 3) + yctr];
			bd = ((redge[(dstheight * 3) + yctr] - b) << 9) / delta_x;
			fix = 1024 - (x1 & 1023);
			r += (rd * fix) >> 10;
			g += (gd * fix) >> 10;
			b += (bd * fix) >> 10;
			if ((x1 >> 10) < clipleft)
			{
				r += (clipleft - (x1 >> 10)) * rd;
				g += (clipleft - (x1 >> 10)) * gd;
				b += (clipleft - (x1 >> 10)) * bd;
			}
			r <<= 6; g <<= 6; b <<= 6; rd <<= 6; gd <<= 6; bd <<= 6;
			w = xmax[yctr] + 1 - xmin[yctr];
		}
		else
			w = -1;
		pdd8 = &dst[(yctr * dstwidth + xmin[yctr]) * (bitwidth >> 3)];
		pdd16 = (unsigned short *)&dst[(yctr * dstwidth + xmin[yctr]) * (bitwidth >> 3)];
		pdd32 = (unsigned EINT32 *)&dst[(yctr * dstwidth + xmin[yctr]) * (bitwidth >> 3)];
		if (bitwidth == 8)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				pdd8[xctr] = (unsigned char)((r >> 16) | (g >> 16) | (b >> 16));
				r += rd;
				g += gd;
				b += bd;
			}
		}
		else if (bitwidth == 16)
		{
			if (videomode == EDSP_TRUE12)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					if (r < 0xF00000)
						dithr = (r & 0xFF0000) + (edithertable16[(((yctr + xctr) & 15) << 4) + ((r >> 16) & 15)] << 16);
					else
						dithr = r;
					if (g < 0xF00000)
						dithg = (g & 0xFF0000) + (edithertable16[(((yctr + xctr) & 15) << 4) + ((g >> 16) & 15)] << 16);
					else
						dithg = g;
					if (b < 0xF00000)
						dithb = (b & 0xFF0000) + (edithertable16[(((yctr + xctr) & 15) << 4) + ((b >> 16) & 15)] << 16);
					else
						dithb = b;
					pdd16[xctr] = (unsigned short)(((dithr >> 12) & 3840) | ((dithg >> 16) & 240) | (dithb >> 20));
					r += rd;
					g += gd;
					b += bd;
				}
			}
			else if (videomode == EDSP_TRUE15)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					if (r < 0xF80000)
						dithr = (r & 0xFF0000) + (edithertable8[(((yctr + xctr) & 7) << 3) + ((r >> 16) & 7)] << 16);
					else
						dithr = r;
					if (g < 0xF80000)
						dithg = (g & 0xFF0000) + (edithertable8[(((yctr + xctr) & 7) << 3) + ((g >> 16) & 7)] << 16);
					else
						dithg = g;
					if (b < 0xF80000)
						dithb = (b & 0xFF0000) + (edithertable8[(((yctr + xctr) & 7) << 3) + ((b >> 16) & 7)] << 16);
					else
						dithb = b;
					pdd16[xctr] = (unsigned short)(((dithr >> 9) & 31744) | ((dithg >> 14) & 992) | (dithb >> 19));
					r += rd;
					g += gd;
					b += bd;
				}
			}
			else
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					if (r < 0xF80000)
						dithr = (r & 0xFF0000) + (edithertable8[(((yctr + xctr) & 7) << 3) + ((r >> 16) & 7)] << 16);
					else
						dithr = r;
					if (g < 0xFB0000)
						dithg = (g & 0xFF0000) + (edithertable4[(((yctr + xctr) & 3) << 2) + ((g >> 16) & 3)] << 16);
					else
						dithg = g;
					if (b < 0xF80000)
						dithb = (b & 0xFF0000) + (edithertable8[(((yctr + xctr) & 7) << 3) + ((b >> 16) & 7)] << 16);
					else
						dithb = b;
					pdd16[xctr] = (unsigned short)(((dithr >> 8) & 63488) | ((dithg >> 13) & 2016) | (dithb >> 19));
					r += rd;
					g += gd;
					b += bd;
				}
			}
		}
		else if (bitwidth == 24)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				pdd8[xctr * 3]     = (unsigned char)(b >> 16);
				pdd8[xctr * 3 + 1] = (unsigned char)(g >> 16);
				pdd8[xctr * 3 + 2] = (unsigned char)(r >> 16);
				r += rd;
				g += gd;
				b += bd;
			}
		}
		else if (bitwidth == 32)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				pdd32[xctr] = (r & 0xFF0000) | ((g >> 8) & 0xFF00) | (b >> 16);
				r += rd;
				g += gd;
				b += bd;
			}
		}
	}
}

//Blit: gradient
void ExRsBltGradient(unsigned char *dst, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned char videomode)
{
	unsigned EINT32 *pdd32;
	unsigned short *pdd16;
	unsigned char *pdd8;
	long xctr, yctr, w;
	for (yctr = ytop; yctr < ybottom + 1; yctr++)
	{
		long rd = 0, gd = 0, bd = 0, r = 0, g = 0, b = 0;
		if (xmax[yctr] >= 0)
		{
			long x1, x2, delta_x, fix;
			x1 = ledge[yctr]; x2 = redge[yctr];
			delta_x = ((x2 - x1) >> 1) + 1;
			r  = ledge[dstheight + yctr];
			rd = ((redge[dstheight + yctr] - r) << 9) / delta_x;
			g  = ledge[(dstheight << 1) + yctr];
			gd = ((redge[(dstheight << 1) + yctr] - g) << 9) / delta_x;
			b  = ledge[(dstheight * 3) + yctr];
			bd = ((redge[(dstheight * 3) + yctr] - b) << 9) / delta_x;
			fix = 1024 - (x1 & 1023);
			r += (rd * fix) >> 10;
			g += (gd * fix) >> 10;
			b += (bd * fix) >> 10;
			if ((x1 >> 10) < clipleft)
			{
				r += (clipleft - (x1 >> 10)) * rd;
				g += (clipleft - (x1 >> 10)) * gd;
				b += (clipleft - (x1 >> 10)) * bd;
			}
			r <<= 6; g <<= 6; b <<= 6; rd <<= 6; gd <<= 6; bd <<= 6;
			w = xmax[yctr] + 1 - xmin[yctr];
		}
		else
			w = -1;
		pdd8 = &dst[(yctr * dstwidth + xmin[yctr]) * (bitwidth >> 3)];
		pdd16 = (unsigned short *)&dst[(yctr * dstwidth + xmin[yctr]) * (bitwidth >> 3)];
		pdd32 = (unsigned EINT32 *)&dst[(yctr * dstwidth + xmin[yctr]) * (bitwidth >> 3)];
		if (bitwidth == 8)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				pdd8[xctr] = (unsigned char)((r >> 16) | (g >> 16) | (b >> 16));
				r += rd;
				g += gd;
				b += bd;
			}
		}
		else if (bitwidth == 16)
		{
			if (videomode == EDSP_TRUE12)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					pdd16[xctr] = (unsigned short)(((r >> 12) & 3840) | ((g >> 16) & 240) | (b >> 20));
					r += rd;
					g += gd;
					b += bd;
				}
			}
			else if (videomode == EDSP_TRUE15)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					pdd16[xctr] = (unsigned short)(((r >> 9) & 31744) | ((g >> 14) & 992) | (b >> 19));
					r += rd;
					g += gd;
					b += bd;
				}
			}
			else
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					pdd16[xctr] = (unsigned short)(((r >> 8) & 63488) | ((g >> 13) & 2016) | (b >> 19));
					r += rd;
					g += gd;
					b += bd;
				}
			}
		}
		else if (bitwidth == 24)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				pdd8[xctr * 3]     = (unsigned char)(b >> 16);
				pdd8[xctr * 3 + 1] = (unsigned char)(g >> 16);
				pdd8[xctr * 3 + 2] = (unsigned char)(r >> 16);
				r += rd;
				g += gd;
				b += bd;
			}
		}
		else if (bitwidth == 32)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				pdd32[xctr] = (r & 0xFF0000) | ((g >> 8) & 0xFF00) | (b >> 16);
				r += rd;
				g += gd;
				b += bd;
			}
		}
	}
}

//Blit: optimized without filters
void ExRsBltPlus(unsigned char *dst, unsigned char *src, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned char videomode)
{
	long xctr, yctr, w;
	unsigned long op_andu = srcwidth, op_andv = srcheight, op_shl = 0;
	while (op_andu > 1)
	{
		op_andu /= 2;
		op_shl++;
	}
	op_andu = srcwidth - 1;
	op_andv = srcheight - 1;
	for (yctr = ytop; yctr <= ybottom; yctr++)
	{
		long su = 0, sv = 0, eu = 0, ev = 0;
		if (xmax[yctr] >= 0)
		{
			long x1, x2, delta_x, fix;
			x1 = ledge[yctr]; x2 = redge[yctr];
			delta_x = ((x2 - x1) >> 1) + 1;
			su = ledge[dstheight + yctr];
			eu = ((redge[dstheight + yctr] - su) << 9) / delta_x;
			sv = ledge[(dstheight << 1) + yctr];
			ev = ((redge[(dstheight << 1) + yctr] - sv) << 9) / delta_x;
			fix = 1024 - (x1 & 1023);
			su += (eu * fix) >> 10;
			sv += (ev * fix) >> 10;
			if ((x1 >> 10) < clipleft)
			{
				su += (clipleft - (x1 >> 10)) * eu;
				sv += (clipleft - (x1 >> 10)) * ev;
			}
			w = xmax[yctr] + 1 - xmin[yctr];
		}
		else
			w = -1;
		if (bitwidth == 8)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr])];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				*pdd = psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *pdd = (unsigned short *)&dst[((yctr * dstwidth) + xmin[yctr]) * 2];
			unsigned short *psd = (unsigned short *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				*pdd = psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 24)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr]) * 3];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				ClassEMemory::Copy(pdd, &psd[((((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)) * 3], 3);
				pdd += 3;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 32)
		{
			unsigned EINT32 *pdd = (unsigned EINT32 *)&dst[((yctr * dstwidth) + xmin[yctr]) * 4];
			unsigned EINT32 *psd = (unsigned EINT32 *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				*pdd = psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
	}
}

//Blit: no filters
void ExRsBlt(unsigned char *dst, unsigned char *src, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned long virtwidth, unsigned char videomode)
{
	if (ExRsBlt_IsPower2(virtwidth) && ExRsBlt_IsPower2(srcheight)){ ExRsBltPlus(dst, src, xmin, xmax, ledge, redge, dstwidth, dstheight, srcwidth, srcheight, ytop, ybottom, clipleft, bitwidth, videomode); return; }
	long xctr, yctr, w;
	for (yctr = ytop; yctr <= ybottom; yctr++)
	{
		long su = 0, sv = 0, eu = 0, ev = 0;
		if (xmax[yctr] >= 0)
		{
			long x1, x2, delta_x, fix;
			x1 = ledge[yctr]; x2 = redge[yctr];
			delta_x = ((x2 - x1) >> 1) + 1;
			su = ledge[dstheight + yctr];
			eu = ((redge[dstheight + yctr] - su) << 9) / delta_x;
			sv = ledge[(dstheight << 1) + yctr];
			ev = ((redge[(dstheight << 1) + yctr] - sv) << 9) / delta_x;
			fix = 1024 - (x1 & 1023);
			su += (eu * fix) >> 10;
			sv += (ev * fix) >> 10;
			if ((x1 >> 10) < clipleft)
			{
				su += (clipleft - (x1 >> 10)) * eu;
				sv += (clipleft - (x1 >> 10)) * ev;
			}
			w = xmax[yctr] + 1 - xmin[yctr];
		}
		else
			w = -1;
		if (bitwidth == 8)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr])];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				*pdd = psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *pdd = (unsigned short *)&dst[((yctr * dstwidth) + xmin[yctr]) * 2];
			unsigned short *psd = (unsigned short *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				*pdd = psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 24)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr]) * 3];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				ClassEMemory::Copy(pdd, &psd[((((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)) * 3], 3);
				pdd += 3;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 32)
		{
			unsigned EINT32 *pdd = (unsigned EINT32 *)&dst[((yctr * dstwidth) + xmin[yctr]) * 4];
			unsigned EINT32 *psd = (unsigned EINT32 *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				*pdd = psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
	}
}

//Blit: colorkey (optimized in certain cases)
void ExRsBltCkPlus(unsigned char *dst, unsigned char *src, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned char videomode, unsigned long colorkey)
{
	long xctr, yctr, w;
	unsigned long op_andu = srcwidth, op_andv = srcheight, op_shl = 0;
	while (op_andu > 1)
	{
		op_andu /= 2;
		op_shl++;
	}
	op_andu = srcwidth - 1;
	op_andv = srcheight - 1;
	for (yctr = ytop; yctr < ybottom + 1; yctr++)
	{
		long su = 0, sv = 0, eu = 0, ev = 0;
		if (xmax[yctr] >= 0)
		{
			long x1, x2, delta_x, fix;
			x1 = ledge[yctr]; x2 = redge[yctr];
			delta_x = ((x2 - x1) >> 1) + 1;
			su = ledge[dstheight + yctr];
			eu = ((redge[dstheight + yctr] - su) << 9) / delta_x;
			sv = ledge[(dstheight << 1) + yctr];
			ev = ((redge[(dstheight << 1) + yctr] - sv) << 9) / delta_x;
			fix = 1024 - (x1 & 1023);
			su += (eu * fix) >> 10;
			sv += (ev * fix) >> 10;
			if ((x1 >> 10) < clipleft)
			{
				su += (clipleft - (x1 >> 10)) * eu;
				sv += (clipleft - (x1 >> 10)) * ev;
			}
			w = xmax[yctr] + 1 - xmin[yctr];
		}
		else
			w = -1;
		if (bitwidth == 8)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr])];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				if (psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)] != colorkey)
					*pdd = psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *pdd = (unsigned short *)&dst[((yctr * dstwidth) + xmin[yctr]) * 2];
			unsigned short *psd = (unsigned short *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				if (psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)] != colorkey)
					*pdd = psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 24)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr]) * 3];
			unsigned char *psd = (unsigned char *)src;
			unsigned long tc;
			for (xctr = 0; xctr < w; xctr++)
			{
				ClassEMemory::Copy(&tc, &psd[((((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)) * 3], 3);
				if (tc != colorkey)
					ClassEMemory::Copy(pdd, &tc, 3);
				pdd += 3;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 32)
		{
			unsigned EINT32 *pdd = (unsigned EINT32 *)&dst[((yctr * dstwidth) + xmin[yctr]) * 4];
			unsigned EINT32 *psd = (unsigned EINT32 *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				if (psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)] != colorkey)
					*pdd = psd[(((sv >> 10) & op_andv) << op_shl) + ((su >> 10) & op_andu)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
	}
}

//Blit: colorkey
void ExRsBltCk(unsigned char *dst, unsigned char *src, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned long virtwidth, unsigned char videomode, unsigned long colorkey)
{
	if (ExRsBlt_IsPower2(virtwidth) && ExRsBlt_IsPower2(srcheight)){ ExRsBltCkPlus(dst, src, xmin, xmax, ledge, redge, dstwidth, dstheight, srcwidth, srcheight, ytop, ybottom, clipleft, bitwidth, videomode, colorkey); return; }
	long xctr, yctr, w;
	for (yctr = ytop; yctr < ybottom + 1; yctr++)
	{
		long su = 0, sv = 0, eu = 0, ev = 0;
		if (xmax[yctr] >= 0)
		{
			long x1, x2, delta_x, fix;
			x1 = ledge[yctr]; x2 = redge[yctr];
			delta_x = ((x2 - x1) >> 1) + 1;
			su = ledge[dstheight + yctr];
			eu = ((redge[dstheight + yctr] - su) << 9) / delta_x;
			sv = ledge[(dstheight << 1) + yctr];
			ev = ((redge[(dstheight << 1) + yctr] - sv) << 9) / delta_x;
			fix = 1024 - (x1 & 1023);
			su += (eu * fix) >> 10;
			sv += (ev * fix) >> 10;
			if ((x1 >> 10) < clipleft)
			{
				su += (clipleft - (x1 >> 10)) * eu;
				sv += (clipleft - (x1 >> 10)) * ev;
			}
			w = xmax[yctr] + 1 - xmin[yctr];
		}
		else
			w = -1;
		if (bitwidth == 8)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr])];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				if (psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)] != colorkey)
					*pdd = psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *pdd = (unsigned short *)&dst[((yctr * dstwidth) + xmin[yctr]) * 2];
			unsigned short *psd = (unsigned short *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				if (psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)] != colorkey)
					*pdd = psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 24)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr]) * 3];
			unsigned char *psd = (unsigned char *)src;
			unsigned long tc;
			for (xctr = 0; xctr < w; xctr++)
			{
				ClassEMemory::Copy(&tc, &psd[((((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)) * 3], 3);
				if (tc != colorkey)
					ClassEMemory::Copy(pdd, &tc, 3);
				pdd += 3;
				su += eu;
				sv += ev;
			}
		}
		else if (bitwidth == 32)
		{
			unsigned EINT32 *pdd = (unsigned EINT32 *)&dst[((yctr * dstwidth) + xmin[yctr]) * 4];
			unsigned EINT32 *psd = (unsigned EINT32 *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				if (psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)] != colorkey)
					*pdd = psd[(((sv >> 10) % srcheight) * srcwidth) + ((su >> 10) % virtwidth)];
				pdd++;
				su += eu;
				sv += ev;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
// Rotate/Scale and variable shape blitters (perspective correct)//
///////////////////////////////////////////////////////////////////

//Blit: default function, handling all filters (optimized in certain cases)
void ExRsPerspBltGenericPlus(unsigned char *dst, unsigned char *src, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned long virtwidth, unsigned char videomode, long flags, COLORREF colorkey, long property1, long property2)
{
	bool combinedalpha = false;
	long xctr, yctr, w, bc = 256 - property1;
	unsigned long op_andu = srcwidth, op_andv = srcheight, op_shl = 0;
	while (op_andu > 1)
	{
		op_andu /= 2;
		op_shl++;
	}
	op_andu = srcwidth - 1;
	op_andv = srcheight - 1;
	if ((flags & EFX_OPACITY) && (flags & (EFX_SUB | EFX_ALPHA)))
	{
		flags -= EFX_OPACITY;
		combinedalpha = true;
	}
	if (videomode == EDSP_TRUE12)
	{
		property1 >>= 4;
		bc >>= 4;
	}
	else if (videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
	{
		property1 >>= 3;
		bc >>= 3;
	}
	for (yctr = ytop; yctr <= ybottom; yctr++)
	{
		if (xmax[yctr] < 0)
			continue;
		unsigned long sc = 0;
		long dxi  = ((redge[yctr] - ledge[yctr]) >> 10) + 1;
		long uoz = ledge[dstheight + yctr];
		long voz = ledge[dstheight * 2 + yctr];
		long ooz = ledge[dstheight * 3 + yctr];
		long duozdx = (redge[dstheight + yctr] - uoz) / dxi;
		long dvozdx = (redge[dstheight * 2 + yctr] - voz) / dxi;
		long doozdx = (redge[dstheight * 3 + yctr] - ooz) / dxi;
		if ((ledge[yctr] >> 10) < xmin[yctr])
		{
			long pixdiff = xmin[yctr] - (ledge[yctr] >> 10);
			uoz += duozdx * pixdiff;
			voz += dvozdx * pixdiff;
			ooz += doozdx * pixdiff;
		}
		w = xmax[yctr] + 1 - xmin[yctr];
		if (bitwidth == 8)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr])];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				long z = 0x7FFFFFFF / ooz;
				long u = (((uoz / 8) * (z / 8)) >> 20) & op_andu;
				long v = (((voz / 8) * (z / 8)) >> 20) & op_andv;
				*pdd = psd[(v << op_shl) + u];
				pdd++;
				uoz += duozdx;
				voz += dvozdx;
				ooz += doozdx;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *pdd = (unsigned short *)&dst[((yctr * dstwidth) + xmin[yctr]) * 2];
			unsigned short *psd = (unsigned short *)src;
			if (videomode == EDSP_TRUE12)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) & op_andu;
					long v = (((voz / 8) * (z / 8)) >> 20) & op_andv;
					sc = psd[(v << op_shl) + u];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 1911, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
			else if (videomode == EDSP_TRUE15)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) & op_andu;
					long v = (((voz / 8) * (z / 8)) >> 20) & op_andv;
					sc = psd[(v << op_shl) + u];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 992, 31744, 0, 5, 10, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 15855, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 31775, 992, 5, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 31775, 992, 5, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 992, 31744, 32, 1024, 32768);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 992, 31744);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
			else if (videomode == EDSP_TRUE16)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) & op_andu;
					long v = (((voz / 8) * (z / 8)) >> 20) & op_andv;
					sc = psd[(v << op_shl) + u];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 31727, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
		}
		else if (bitwidth == 24 || bitwidth == 32)
		{
			unsigned long dc = 0;
			unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[((yctr * dstwidth) + xmin[yctr]) * 4];
			unsigned EINT32 *psd32 = (unsigned EINT32 *)src;
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr]) * 3];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				long z = 0x7FFFFFFF / ooz;
				long u = (((uoz / 8) * (z / 8)) >> 20) & op_andu;
				long v = (((voz / 8) * (z / 8)) >> 20) & op_andv;
				if (bitwidth == 24)
				{
					ClassEMemory::Copy(&sc, &psd[(v << op_shl) * 3 + u * 3], 3);
					ClassEMemory::Copy(&dc, pdd, 3);
				}
				else
				{
					sc = psd32[(v << op_shl) + u];
					dc = *pdd32;
				}
				if (!(flags & EFX_COLORKEY) || sc != colorkey)
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, property2, unsigned long)
					else if (flags & EFX_COLORFILL)
						sc = property2;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, property2);
					if (combinedalpha)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, property1);
					if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY(dc, sc, 0xFF00FF, 0xFF00, 8, bc, property1);
					else if (flags & EFX_ALPHA)
						sc = EBCODE_MACRO_ADDALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
					else if (flags & EFX_SUB)
						sc = EBCODE_MACRO_SUBALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000);
					if (bitwidth == 24)
						ClassEMemory::Copy(pdd, &sc, 3);
					else
						*pdd32 = sc;
				}
				pdd += 3;
				pdd32++;
				uoz += duozdx;
				voz += dvozdx;
				ooz += doozdx;
			}
		}
	}
}

//Blit: default function, handling all filters
void ExRsPerspBltGeneric(unsigned char *dst, unsigned char *src, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned long virtwidth, unsigned char videomode, long flags, COLORREF colorkey, long property1, long property2)
{
	if (ExRsBlt_IsPower2(virtwidth) && ExRsBlt_IsPower2(srcheight)){ ExRsPerspBltGenericPlus(dst, src, xmin, xmax, ledge, redge, dstwidth, dstheight, srcwidth, srcheight, ytop, ybottom, clipleft, bitwidth, virtwidth, videomode, flags, colorkey, property1, property2); return; }
	bool combinedalpha = false;
	long xctr, yctr, w, bc = 256 - property1;
	if ((flags & EFX_OPACITY) && (flags & (EFX_SUB | EFX_ALPHA)))
	{
		flags -= EFX_OPACITY;
		combinedalpha = true;
	}
	if (videomode == EDSP_TRUE12)
	{
		property1 >>= 4;
		bc >>= 4;
	}
	else if (videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
	{
		property1 >>= 3;
		bc >>= 3;
	}
	for (yctr = ytop; yctr <= ybottom; yctr++)
	{
		if (xmax[yctr] < 0)
			continue;
		unsigned long sc = 0;
		long dxi  = ((redge[yctr] - ledge[yctr]) >> 10) + 1;
		long uoz = ledge[dstheight + yctr];
		long voz = ledge[dstheight * 2 + yctr];
		long ooz = ledge[dstheight * 3 + yctr];
		long duozdx = (redge[dstheight + yctr] - uoz) / dxi;
		long dvozdx = (redge[dstheight * 2 + yctr] - voz) / dxi;
		long doozdx = (redge[dstheight * 3 + yctr] - ooz) / dxi;
		if ((ledge[yctr] >> 10) < xmin[yctr])
		{
			long pixdiff = xmin[yctr] - (ledge[yctr] >> 10);
			uoz += duozdx * pixdiff;
			voz += dvozdx * pixdiff;
			ooz += doozdx * pixdiff;
		}
		w = xmax[yctr] + 1 - xmin[yctr];
		if (bitwidth == 8)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr])];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				long z = 0x7FFFFFFF / ooz;
				long u = (((uoz / 8) * (z / 8)) >> 20) % srcwidth;
				long v = (((voz / 8) * (z / 8)) >> 20) % srcheight;
				*pdd = psd[v * srcwidth + u];
				pdd++;
				uoz += duozdx;
				voz += dvozdx;
				ooz += doozdx;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *pdd = (unsigned short *)&dst[((yctr * dstwidth) + xmin[yctr]) * 2];
			unsigned short *psd = (unsigned short *)src;
			if (videomode == EDSP_TRUE12)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) % srcwidth;
					long v = (((voz / 8) * (z / 8)) >> 20) % srcheight;
					sc = psd[v * srcwidth + u];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 1911, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
			else if (videomode == EDSP_TRUE15)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) % srcwidth;
					long v = (((voz / 8) * (z / 8)) >> 20) % srcheight;
					sc = psd[v * srcwidth + u];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 992, 31744, 0, 5, 10, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 15855, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 31775, 992, 5, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 31775, 992, 5, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 992, 31744, 32, 1024, 32768);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 992, 31744);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
			else if (videomode == EDSP_TRUE16)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) % srcwidth;
					long v = (((voz / 8) * (z / 8)) >> 20) % srcheight;
					sc = psd[v * srcwidth + u];
					if (!(flags & EFX_COLORKEY) || sc != colorkey)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 31727, property2);
						if (combinedalpha)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, property1);
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, bc, property1);
						else if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
						*pdd = (unsigned short)sc;
					}
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
		}
		else if (bitwidth == 24 || bitwidth == 32)
		{
			unsigned long dc = 0;
			unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[((yctr * dstwidth) + xmin[yctr]) * 4];
			unsigned EINT32 *psd32 = (unsigned EINT32 *)src;
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr]) * 3];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				long z = 0x7FFFFFFF / ooz;
				long u = (((uoz / 8) * (z / 8)) >> 20) % srcwidth;
				long v = (((voz / 8) * (z / 8)) >> 20) % srcheight;
				if (bitwidth == 24)
				{
					ClassEMemory::Copy(&sc, &psd[v * srcwidth * 3 + u * 3], 3);
					ClassEMemory::Copy(&dc, pdd, 3);
				}
				else
				{
					sc = psd32[v * srcwidth + u];
					dc = *pdd32;
				}
				if (!(flags & EFX_COLORKEY) || sc != colorkey)
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, property2, unsigned long)
					else if (flags & EFX_COLORFILL)
						sc = property2;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, property2);
					if (combinedalpha)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, property1);
					if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY(dc, sc, 0xFF00FF, 0xFF00, 8, bc, property1);
					else if (flags & EFX_ALPHA)
						sc = EBCODE_MACRO_ADDALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
					else if (flags & EFX_SUB)
						sc = EBCODE_MACRO_SUBALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000);
					if (bitwidth == 24)
						ClassEMemory::Copy(pdd, &sc, 3);
					else
						*pdd32 = sc;
				}
				pdd += 3;
				pdd32++;
				uoz += duozdx;
				voz += dvozdx;
				ooz += doozdx;
			}
		}
	}
}

//Blit: default function, handling all filters (optimized in certain cases)
void ExRsPerspBltPlus(unsigned char *dst, unsigned char *src, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned long virtwidth, unsigned char videomode)
{
	long xctr, yctr, w;
	unsigned long op_andu = srcwidth, op_andv = srcheight, op_shl = 0;
	while (op_andu > 1)
	{
		op_andu /= 2;
		op_shl++;
	}
	op_andu = srcwidth - 1;
	op_andv = srcheight - 1;
	for (yctr = ytop; yctr <= ybottom; yctr++)
	{
		if (xmax[yctr] < 0)
			continue;
		unsigned long sc = 0;
		long dxi  = ((redge[yctr] - ledge[yctr]) >> 10) + 1;
		long uoz = ledge[dstheight + yctr];
		long voz = ledge[dstheight * 2 + yctr];
		long ooz = ledge[dstheight * 3 + yctr];
		long duozdx = (redge[dstheight + yctr] - uoz) / dxi;
		long dvozdx = (redge[dstheight * 2 + yctr] - voz) / dxi;
		long doozdx = (redge[dstheight * 3 + yctr] - ooz) / dxi;
		if ((ledge[yctr] >> 10) < xmin[yctr])
		{
			long pixdiff = xmin[yctr] - (ledge[yctr] >> 10);
			uoz += duozdx * pixdiff;
			voz += dvozdx * pixdiff;
			ooz += doozdx * pixdiff;
		}
		w = xmax[yctr] + 1 - xmin[yctr];
		if (bitwidth == 8)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr])];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				long z = 0x7FFFFFFF / ooz;
				long u = (((uoz / 8) * (z / 8)) >> 20) & op_andu;
				long v = (((voz / 8) * (z / 8)) >> 20) & op_andv;
				*pdd = psd[(v << op_shl) + u];
				pdd++;
				uoz += duozdx;
				voz += dvozdx;
				ooz += doozdx;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *pdd = (unsigned short *)&dst[((yctr * dstwidth) + xmin[yctr]) * 2];
			unsigned short *psd = (unsigned short *)src;
			if (videomode == EDSP_TRUE12)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) & op_andu;
					long v = (((voz / 8) * (z / 8)) >> 20) & op_andv;
					*pdd = psd[(v << op_shl) + u];
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
			else if (videomode == EDSP_TRUE15)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) & op_andu;
					long v = (((voz / 8) * (z / 8)) >> 20) & op_andv;
					*pdd = psd[(v << op_shl) + u];
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
			else if (videomode == EDSP_TRUE16)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) & op_andu;
					long v = (((voz / 8) * (z / 8)) >> 20) & op_andv;
					*pdd = psd[(v << op_shl) + u];
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
		}
		else if (bitwidth == 24 || bitwidth == 32)
		{
			unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[((yctr * dstwidth) + xmin[yctr]) * 4];
			unsigned EINT32 *psd32 = (unsigned EINT32 *)src;
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr]) * 3];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				long z = 0x7FFFFFFF / ooz;
				long u = (((uoz / 8) * (z / 8)) >> 20) & op_andu;
				long v = (((voz / 8) * (z / 8)) >> 20) & op_andv;
				if (bitwidth == 24)
				{
					ClassEMemory::Copy(&sc, &psd[(v << op_shl) * 3 + u * 3], 3);
					ClassEMemory::Copy(pdd, &sc, 3);
				}
				else
					*pdd32 = psd32[(v << op_shl) + u];
				pdd += 3;
				pdd32++;
				uoz += duozdx;
				voz += dvozdx;
				ooz += doozdx;
			}
		}
	}
}

//Blit: default function, handling all filters
void ExRsPerspBlt(unsigned char *dst, unsigned char *src, short *xmin, short *xmax, EINT32 *ledge, EINT32 *redge, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, long ytop, long ybottom, long clipleft, unsigned char bitwidth, unsigned long virtwidth, unsigned char videomode)
{
	if (ExRsBlt_IsPower2(virtwidth) && ExRsBlt_IsPower2(srcheight)){ ExRsPerspBltPlus(dst, src, xmin, xmax, ledge, redge, dstwidth, dstheight, srcwidth, srcheight, ytop, ybottom, clipleft, bitwidth, virtwidth, videomode); return; }
	long xctr, yctr, w;
	for (yctr = ytop; yctr <= ybottom; yctr++)
	{
		if (xmax[yctr] < 0)
			continue;
		unsigned long sc = 0;
		long dxi  = ((redge[yctr] - ledge[yctr]) >> 10) + 1;
		long uoz = ledge[dstheight + yctr];
		long voz = ledge[dstheight * 2 + yctr];
		long ooz = ledge[dstheight * 3 + yctr];
		long duozdx = (redge[dstheight + yctr] - uoz) / dxi;
		long dvozdx = (redge[dstheight * 2 + yctr] - voz) / dxi;
		long doozdx = (redge[dstheight * 3 + yctr] - ooz) / dxi;
		if ((ledge[yctr] >> 10) < xmin[yctr])
		{
			long pixdiff = xmin[yctr] - (ledge[yctr] >> 10);
			uoz += duozdx * pixdiff;
			voz += dvozdx * pixdiff;
			ooz += doozdx * pixdiff;
		}
		w = xmax[yctr] + 1 - xmin[yctr];
		if (bitwidth == 8)
		{
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr])];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				long z = 0x7FFFFFFF / ooz;
				long u = (((uoz / 8) * (z / 8)) >> 20) % srcwidth;
				long v = (((voz / 8) * (z / 8)) >> 20) % srcheight;
				*pdd = psd[v * srcwidth + u];
				pdd++;
				uoz += duozdx;
				voz += dvozdx;
				ooz += doozdx;
			}
		}
		else if (bitwidth == 16)
		{
			unsigned short *pdd = (unsigned short *)&dst[((yctr * dstwidth) + xmin[yctr]) * 2];
			unsigned short *psd = (unsigned short *)src;
			if (videomode == EDSP_TRUE12)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) % srcwidth;
					long v = (((voz / 8) * (z / 8)) >> 20) % srcheight;
					*pdd = psd[v * srcwidth + u];
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
			else if (videomode == EDSP_TRUE15)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) % srcwidth;
					long v = (((voz / 8) * (z / 8)) >> 20) % srcheight;
					*pdd = psd[v * srcwidth + u];
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
			else if (videomode == EDSP_TRUE16)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					long z = 0x7FFFFFFF / ooz;
					long u = (((uoz / 8) * (z / 8)) >> 20) % srcwidth;
					long v = (((voz / 8) * (z / 8)) >> 20) % srcheight;
					*pdd = psd[v * srcwidth + u];
					pdd++;
					uoz += duozdx;
					voz += dvozdx;
					ooz += doozdx;
				}
			}
		}
		else if (bitwidth == 24 || bitwidth == 32)
		{
			unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[((yctr * dstwidth) + xmin[yctr]) * 4];
			unsigned EINT32 *psd32 = (unsigned EINT32 *)src;
			unsigned char *pdd = (unsigned char *)&dst[((yctr * dstwidth) + xmin[yctr]) * 3];
			unsigned char *psd = (unsigned char *)src;
			for (xctr = 0; xctr < w; xctr++)
			{
				long z = 0x7FFFFFFF / ooz;
				long u = (((uoz / 8) * (z / 8)) >> 20) % srcwidth;
				long v = (((voz / 8) * (z / 8)) >> 20) % srcheight;
				if (bitwidth == 24)
				{
					ClassEMemory::Copy(&sc, &psd[v * srcwidth * 3 + u * 3], 3);
					ClassEMemory::Copy(pdd, &sc, 3);
				}
				else
					*pdd32 = psd32[v * srcwidth + u];
				pdd += 3;
				pdd32++;
				uoz += duozdx;
				voz += dvozdx;
				ooz += doozdx;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
// Landscape blitters                                            //
///////////////////////////////////////////////////////////////////

//Landscape blit: optimized for horizontal scanlines
void ExLndNormalPlus(RECT *rc, unsigned char *dst, unsigned char *src, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, EINT32 *ledge, unsigned char bitwidth)
{
	unsigned long op_and = srcwidth, op_sh = 0, width = rc->right - rc->left;
	long yctr, xctr, yadd, xadd, ypos, xpos;
	while (op_and > 1)
	{
		op_and /= 2;
		op_sh++;
	}
	op_and = srcwidth - 1;
	if (bitwidth == 8)
	{
		unsigned char *pdd = dst, *psd = src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->top;
		while(yctr < rc->bottom)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) & op_and) << op_sh) | ((xpos >> 12) & op_and)];
				xpos += xadd;
				ypos += yadd;
				pdd++;
			}
			pdd += dstwidth - width;
			yctr++;
		}
	}
	else if (bitwidth == 16)
	{
		unsigned short *pdd = (unsigned short *)dst, *psd = (unsigned short *)src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->top;
		while(yctr < rc->bottom)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) & op_and) << op_sh) | ((xpos >> 12) & op_and)];
				xpos += xadd;
				ypos += yadd;
				pdd++;
			}
			pdd += dstwidth - width;
			yctr++;
		}
	}
	else if (bitwidth == 24)
	{
		unsigned char *pdd = dst, *psd = src;
		pdd += rc->top * dstwidth * 3 + rc->left * 3;
		yctr = rc->top;
		while(yctr < rc->bottom)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				ClassEMemory::Copy(pdd, &psd[((((ypos >> 12) & op_and) << op_sh) | ((xpos >> 12) & op_and)) * 3], 3);
				xpos += xadd;
				ypos += yadd;
				pdd += 3;
			}
			pdd += (dstwidth - width) * 3;
			yctr++;
		}
	}
	else if (bitwidth == 32)
	{
		unsigned EINT32 *pdd = (unsigned EINT32 *)dst, *psd = (unsigned EINT32 *)src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->top;
		while(yctr < rc->bottom)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) & op_and) << op_sh) | ((xpos >> 12) & op_and)];
				xpos += xadd;
				ypos += yadd;
				pdd++;
			}
			pdd += dstwidth - width;
			yctr++;
		}
	}
}

//Landscape blit: normal horizontal scanlines
void ExLndNormal(RECT *rc, unsigned char *dst, unsigned char *src, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, EINT32 *ledge, unsigned char bitwidth, unsigned long virtwidth)
{
	if (srcheight == virtwidth && ExRsBlt_IsPower2(virtwidth)){ ExLndNormalPlus(rc, dst, src, dstwidth, dstheight, srcwidth, srcheight, ledge, bitwidth); return; }
	unsigned long width = rc->right - rc->left;
	long yctr, xctr, yadd, xadd, ypos, xpos;
	if (bitwidth == 8)
	{
		unsigned char *pdd = dst, *psd = src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->top;
		while(yctr < rc->bottom)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) % srcheight) * srcwidth) + ((xpos >> 12) % virtwidth)];
				xpos += xadd;
				ypos += yadd;
				pdd++;
			}
			pdd += dstwidth - width;
			yctr++;
		}
	}
	else if (bitwidth == 16)
	{
		unsigned short *pdd = (unsigned short *)dst, *psd = (unsigned short *)src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->top;
		while(yctr < rc->bottom)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) % srcheight) * srcwidth) + ((xpos >> 12) % virtwidth)];
				xpos += xadd;
				ypos += yadd;
				pdd++;
			}
			pdd += dstwidth - width;
			yctr++;
		}
	}
	else if (bitwidth == 24)
	{
		unsigned char *pdd = dst, *psd = src;
		pdd += rc->top * dstwidth * 3 + rc->left * 3;
		yctr = rc->top;
		while(yctr < rc->bottom)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				ClassEMemory::Copy(pdd, &psd[((((ypos >> 12) % srcheight) * srcwidth) + ((xpos >> 12) % virtwidth)) * 3], 3);
				xpos += xadd;
				ypos += yadd;
				pdd += 3;
			}
			pdd += (dstwidth - width) * 3;
			yctr++;
		}
	}
	else if (bitwidth == 32)
	{
		unsigned EINT32 *pdd = (unsigned EINT32 *)dst, *psd = (unsigned EINT32 *)src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->top;
		while(yctr < rc->bottom)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) % srcheight) * srcwidth) + ((xpos >> 12) % virtwidth)];
				xpos += xadd;
				ypos += yadd;
				pdd++;
			}
			pdd += dstwidth - width;
			yctr++;
		}
	}
}

//Landscape blit: optimized for vertical scanlines
void ExLndSidePlus(RECT *rc, unsigned char *dst, unsigned char *src, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, EINT32 *ledge, unsigned char bitwidth)
{
	unsigned long op_and = srcwidth, op_sh = 0, width = rc->bottom - rc->top;
	long yctr, xctr, yadd, xadd, ypos, xpos;
	while (op_and > 1)
	{
		op_and /= 2;
		op_sh++;
	}
	op_and = srcwidth - 1;
	if (bitwidth == 8)
	{
		unsigned char *pdd = dst, *psd = src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->left;
		while(yctr < rc->right)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) & op_and) << op_sh) | ((xpos >> 12) & op_and)];
				xpos += xadd;
				ypos += yadd;
				pdd += dstwidth;
			}
			pdd -= (dstwidth * (rc->bottom - rc->top)) - 1;
			yctr++;
		}
	}
	else if (bitwidth == 16)
	{
		unsigned short *pdd = (unsigned short *)dst, *psd = (unsigned short *)src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->left;
		while(yctr < rc->right)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) & op_and) << op_sh) | ((xpos >> 12) & op_and)];
				xpos += xadd;
				ypos += yadd;
				pdd += dstwidth;
			}
			pdd -= (dstwidth * (rc->bottom - rc->top)) - 1;
			yctr++;
		}
	}
	else if (bitwidth == 24)
	{
		unsigned char *pdd = dst, *psd = src;
		pdd += rc->top * dstwidth * 3 + rc->left * 3;
		yctr = rc->left;
		while(yctr < rc->right)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				ClassEMemory::Copy(pdd, &psd[((((ypos >> 12) & op_and) << op_sh) | ((xpos >> 12) & op_and)) * 3], 3);
				xpos += xadd;
				ypos += yadd;
				pdd += dstwidth * 3;
			}
			pdd -= ((dstwidth * (rc->bottom - rc->top)) - 1) * 3;
			yctr++;
		}
	}
	else if (bitwidth == 32)
	{
		unsigned EINT32 *pdd = (unsigned EINT32 *)dst, *psd = (unsigned EINT32 *)src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->left;
		while(yctr < rc->right)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) & op_and) << op_sh) | ((xpos >> 12) & op_and)];
				xpos += xadd;
				ypos += yadd;
				pdd += dstwidth;
			}
			pdd -= (dstwidth * (rc->bottom - rc->top)) - 1;
			yctr++;
		}
	}
}

//Landscape blit: vertical scanlines for several display orientations
void ExLndSide(RECT *rc, unsigned char *dst, unsigned char *src, unsigned long dstwidth, unsigned long dstheight, unsigned long srcwidth, unsigned long srcheight, EINT32 *ledge, unsigned char bitwidth, unsigned long virtwidth)
{
	if (srcheight == virtwidth && ExRsBlt_IsPower2(virtwidth)){ ExLndSidePlus(rc, dst, src, dstwidth, dstheight, srcwidth, srcheight, ledge, bitwidth); return; }
	unsigned long width = rc->bottom - rc->top;
	long yctr, xctr, yadd, xadd, ypos, xpos;
	if (bitwidth == 8)
	{
		unsigned char *pdd = dst, *psd = src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->left;
		while(yctr < rc->right)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) % srcheight) * srcwidth) + ((xpos >> 12) % virtwidth)];
				xpos += xadd;
				ypos += yadd;
				pdd += dstwidth;
			}
			pdd -= (dstwidth * (rc->bottom - rc->top)) - 1;
			yctr++;
		}
	}
	else if (bitwidth == 16)
	{
		unsigned short *pdd = (unsigned short *)dst, *psd = (unsigned short *)src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->left;
		while(yctr < rc->right)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) % srcheight) * srcwidth) + ((xpos >> 12) % virtwidth)];
				xpos += xadd;
				ypos += yadd;
				pdd += dstwidth;
			}
			pdd -= (dstwidth * (rc->bottom - rc->top)) - 1;
			yctr++;
		}
	}
	else if (bitwidth == 24)
	{
		unsigned char *pdd = dst, *psd = src;
		pdd += rc->top * dstwidth * 3 + rc->left * 3;
		yctr = rc->left;
		while(yctr < rc->right)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				ClassEMemory::Copy(pdd, &psd[((((ypos >> 12) % srcheight) * srcwidth) + ((xpos >> 12) % virtwidth)) * 3], 3);
				xpos += xadd;
				ypos += yadd;
				pdd += dstwidth * 3;
			}
			pdd -= ((dstwidth * (rc->bottom - rc->top)) - 1) * 3;
			yctr++;
		}
	}
	else if (bitwidth == 32)
	{
		unsigned EINT32 *pdd = (unsigned EINT32 *)dst, *psd = (unsigned EINT32 *)src;
		pdd += rc->top * dstwidth + rc->left;
		yctr = rc->left;
		while(yctr < rc->right)
		{
			xpos = ledge[yctr * 4];
			ypos = ledge[yctr * 4 + 1];
			xadd = ledge[yctr * 4 + 2];
			yadd = ledge[yctr * 4 + 3];
			for (xctr = 0; xctr < (long)width; xctr++)
			{
				*pdd = psd[(((ypos >> 12) % srcheight) * srcwidth) + ((xpos >> 12) % virtwidth)];
				xpos += xadd;
				ypos += yadd;
				pdd += dstwidth;
			}
			pdd -= (dstwidth * (rc->bottom - rc->top)) - 1;
			yctr++;
		}
	}
}
