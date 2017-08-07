// exblitfast.h
// Fast optimized filter blitters
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// BltFast                                                       //
///////////////////////////////////////////////////////////////////

//Definitions
#define BLTFAST_PARAMS        long w, long h, unsigned char *dst, unsigned char *src, long dstpitch, long srcpitch, unsigned long colorkey, unsigned long colorize, unsigned long opacity
#define BLTFAST_PARAMS_PASS   w, h, dst, src, dstpitch, srcpitch, colorkey, colorize, opacity

//8-bit entry
bool ERdrBltFast8(BLTFAST_PARAMS, void **erdr_ptr, unsigned long flags)
{
	bool (**ERdrBltFast_ptr)(BLTFAST_PARAMS) = (bool(**)(BLTFAST_PARAMS))(erdr_ptr);
	if (ERdrBltFast_ptr)
	{
		flags &= 255;
		if (ERdrBltFast_ptr[flags] != NULL)
			if (ERdrBltFast_ptr[flags](BLTFAST_PARAMS_PASS))
				return(true);
		if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
			flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
		if (ERdrBltFast_ptr[flags] != NULL)
			if (ERdrBltFast_ptr[flags](BLTFAST_PARAMS_PASS))
				return(true);
	}
	else if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
		flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (!(flags & EFX_COLORKEY) || sc != colorkey)
			{
				if (flags & EFX_COLORIZE)
					sc = colorize;
				if (flags & EFX_ALPHA)
				{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				}
				else if (flags & EFX_SUB)
				{
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				}
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

//12-bit entry
bool ERdrBltFast12(BLTFAST_PARAMS, void **erdr_ptr, unsigned long flags)
{
	bool (**ERdrBltFast_ptr)(BLTFAST_PARAMS) = (bool(**)(BLTFAST_PARAMS))(erdr_ptr);
	if (ERdrBltFast_ptr)
	{
		flags &= 255;
		if (ERdrBltFast_ptr[flags] != NULL)
			if (ERdrBltFast_ptr[flags](BLTFAST_PARAMS_PASS))
				return(true);
		if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
			flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
		if (ERdrBltFast_ptr[flags] != NULL)
			if (ERdrBltFast_ptr[flags](BLTFAST_PARAMS_PASS))
				return(true);
	}
	else if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
		flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned EINT32 *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned EINT32 *)&pdd[1];
	else
		pdd32 = (unsigned EINT32 *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned EINT32 *)&psd[1];
	else
		psd32 = (unsigned EINT32 *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (!(flags & EFX_COLORKEY) || sc != colorkey)
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					else if (flags & EFX_COLORFILL)
						sc = colorize;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					if (flags & (EFX_SUB | EFX_ALPHA))
					{
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					}
					else if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (!(flags & EFX_COLORKEY) || sc != colorkey)
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					else if (flags & EFX_COLORFILL)
						sc = colorize;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					if (flags & (EFX_SUB | EFX_ALPHA))
					{
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					}
					else if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long dblck = colorkey | (colorkey << 16), sc_org = 0, sc_mod, dc;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				dc = (pdd[1] << 16) + pdd[0];
				if (!(flags & EFX_COLORKEY) || (sc_org = sc ^ dblck))
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
					{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
					}
					else if (flags & EFX_COLORFILL)
						sc = colorize;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
					if (flags & (EFX_SUB | EFX_ALPHA))
					{
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
						if (flags & EFX_ALPHA)
						{
							sc_mod = EBCODE_MACRO_ADDALPHA((dc >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(dc, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						}
						else if (flags & EFX_SUB)
						{
							sc_mod = EBCODE_MACRO_SUBALPHA((dc >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(dc, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						}
					}
					else if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY_DBL(dc, sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
					if (flags & EFX_COLORKEY)
					{
						if (sc_org & 0xFFFF)
							*pdd++ = (unsigned short)sc;
						else
							pdd++;
						if (sc_org >> 16)
							*pdd++ = (unsigned short)(sc >> 16);
						else
							pdd++;
					}
					else
					{
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
					}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				dc = *pdd32;
				if (!(flags & EFX_COLORKEY) || (sc_org = sc ^ dblck))
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
					{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
					}
					else if (flags & EFX_COLORFILL)
						sc = colorize;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
					if (flags & (EFX_SUB | EFX_ALPHA))
					{
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
						if (flags & EFX_ALPHA)
						{
							sc_mod = EBCODE_MACRO_ADDALPHA((dc >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(dc, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						}
						else if (flags & EFX_SUB)
						{
							sc_mod = EBCODE_MACRO_SUBALPHA((dc >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(dc, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						}
					}
					else if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY_DBL(dc, sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
					if (flags & EFX_COLORKEY)
					{
						if (sc_org & 0xFFFF)
							*pdd32 = (*pdd32 & 0xFFFF0000) | (sc & 0xFFFF);
						if (sc_org >> 16)
							*pdd32 = (*pdd32 & 0xFFFF) | (sc & 0xFFFF0000);
					}
					else
						*pdd32 = sc;
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

//16-bit entry
bool ERdrBltFast16(BLTFAST_PARAMS, void **erdr_ptr, unsigned long flags)
{
	bool (**ERdrBltFast_ptr)(BLTFAST_PARAMS) = (bool(**)(BLTFAST_PARAMS))(erdr_ptr);
	if (ERdrBltFast_ptr)
	{
		flags &= 255;
		if (ERdrBltFast_ptr[flags] != NULL)
			if (ERdrBltFast_ptr[flags](BLTFAST_PARAMS_PASS))
				return(true);
		if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
			flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
		if (ERdrBltFast_ptr[flags] != NULL)
			if (ERdrBltFast_ptr[flags](BLTFAST_PARAMS_PASS))
				return(true);
	}
	else if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
		flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned EINT32 *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned EINT32 *)&pdd[1];
	else
		pdd32 = (unsigned EINT32 *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned EINT32 *)&psd[1];
	else
		psd32 = (unsigned EINT32 *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (!(flags & EFX_COLORKEY) || sc != colorkey)
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
					else if (flags & EFX_COLORFILL)
						sc = colorize;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
					if (flags & (EFX_SUB | EFX_ALPHA))
					{
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					}
					else if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (!(flags & EFX_COLORKEY) || sc != colorkey)
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
					else if (flags & EFX_COLORFILL)
						sc = colorize;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
					if (flags & (EFX_SUB | EFX_ALPHA))
					{
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					}
					else if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long dblck = colorkey | (colorkey << 16), sc_org = 0, sc_mod, dc;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				dc = (pdd[1] << 16) + pdd[0];
				if (!(flags & EFX_COLORKEY) || (sc_org = sc ^ dblck))
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
					{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
					}
					else if (flags & EFX_COLORFILL)
						sc = colorize;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
					if (flags & (EFX_SUB | EFX_ALPHA))
					{
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
						if (flags & EFX_ALPHA)
						{
							sc_mod = EBCODE_MACRO_ADDALPHA((dc >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(dc, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						}
						else if (flags & EFX_SUB)
						{
							sc_mod = EBCODE_MACRO_SUBALPHA((dc >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(dc, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						}
					}
					else if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY_DBL(dc, sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
					if (flags & EFX_COLORKEY)
					{
						if (sc_org & 0xFFFF)
							*pdd++ = (unsigned short)sc;
						else
							pdd++;
						if (sc_org >> 16)
							*pdd++ = (unsigned short)(sc >> 16);
						else
							pdd++;
					}
					else
					{
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
					}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				dc = *pdd32;
				if (!(flags & EFX_COLORKEY) || (sc_org = sc ^ dblck))
				{
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
					{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
					}
					else if (flags & EFX_COLORFILL)
						sc = colorize;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
					if (flags & (EFX_SUB | EFX_ALPHA))
					{
						if (flags & EFX_OPACITY)
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
						if (flags & EFX_ALPHA)
						{
							sc_mod = EBCODE_MACRO_ADDALPHA((dc >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(dc, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						}
						else if (flags & EFX_SUB)
						{
							sc_mod = EBCODE_MACRO_SUBALPHA((dc >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(dc, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						}
					}
					else if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_OPACITY_DBL(dc, sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
					if (flags & EFX_COLORKEY)
					{
						if (sc_org & 0xFFFF)
							*pdd32 = (*pdd32 & 0xFFFF0000) | (sc & 0xFFFF);
						if (sc_org >> 16)
							*pdd32 = (*pdd32 & 0xFFFF) | (sc & 0xFFFF0000);
					}
					else
						*pdd32 = sc;
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

//32-bit entry
bool ERdrBltFast32(BLTFAST_PARAMS, void **erdr_ptr, unsigned long flags)
{
	bool (**ERdrBltFast_ptr)(BLTFAST_PARAMS) = (bool(**)(BLTFAST_PARAMS))(erdr_ptr);
	if (ERdrBltFast_ptr)
	{
		flags &= 255;
		if (ERdrBltFast_ptr[flags] != NULL)
			if (ERdrBltFast_ptr[flags](BLTFAST_PARAMS_PASS))
				return(true);
		if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
			flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
		if (ERdrBltFast_ptr[flags] != NULL)
			if (ERdrBltFast_ptr[flags](BLTFAST_PARAMS_PASS))
				return(true);
	}
	else if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
		flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
	long xctr;
	unsigned long sc;
	unsigned EINT32 *pdd = (unsigned EINT32 *)dst;
	unsigned EINT32 *psd = (unsigned EINT32 *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (!(flags & EFX_COLORKEY) || sc != colorkey)
			{
				if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
				else if (flags & EFX_COLORFILL)
					sc = colorize;
				else if (flags & EFX_TINT)
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
				if (flags & (EFX_SUB | EFX_ALPHA))
				{
					if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
					if (flags & EFX_ALPHA)
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
					else if (flags & EFX_SUB)
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				}
				else if (flags & EFX_OPACITY)
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

///////////////////////////////////////////////////////////////////
// BltFlip                                                       //
///////////////////////////////////////////////////////////////////

//Definitions
#define BLTFLIP_PARAMS        long xadd, long yadd, long w, long h, unsigned char *dst, unsigned char *src, long dstpitch, long srcpitch, unsigned long colorkey, unsigned long colorize, unsigned long opacity
#define BLTFLIP_PARAMS_PASS   xadd, yadd, w, h, dst, src, dstpitch, srcpitch, colorkey, colorize, opacity

//8-bit entry
bool ERdrBltFlip8(BLTFLIP_PARAMS, void **erdr_ptr, unsigned long flags)
{
	bool (**ERdrBltFlip_ptr)(BLTFLIP_PARAMS) = (bool(**)(BLTFLIP_PARAMS))(erdr_ptr);
	if (ERdrBltFlip_ptr)
	{
		flags &= 255;
		if (ERdrBltFlip_ptr[flags] != NULL)
			if (ERdrBltFlip_ptr[flags](BLTFLIP_PARAMS_PASS))
				return(true);
		if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
			flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
		if (ERdrBltFlip_ptr[flags] != NULL)
			if (ERdrBltFlip_ptr[flags](BLTFLIP_PARAMS_PASS))
				return(true);
	}
	else if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
		flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (!(flags & EFX_COLORKEY) || sc != colorkey)
			{
				if (flags & EFX_COLORIZE)
					sc = colorize;
				if (flags & EFX_ALPHA)
				{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				}
				else if (flags & EFX_SUB)
				{
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				}
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

//12-bit entry
bool ERdrBltFlip12(BLTFLIP_PARAMS, void **erdr_ptr, unsigned long flags)
{
	bool (**ERdrBltFlip_ptr)(BLTFLIP_PARAMS) = (bool(**)(BLTFLIP_PARAMS))(erdr_ptr);
	if (ERdrBltFlip_ptr)
	{
		flags &= 255;
		if (ERdrBltFlip_ptr[flags] != NULL)
			if (ERdrBltFlip_ptr[flags](BLTFLIP_PARAMS_PASS))
				return(true);
		if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
			flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
		if (ERdrBltFlip_ptr[flags] != NULL)
			if (ERdrBltFlip_ptr[flags](BLTFLIP_PARAMS_PASS))
				return(true);
	}
	else if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
		flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	opacity >>= 4;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (!(flags & EFX_COLORKEY) || sc != colorkey)
			{
				if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
				else if (flags & EFX_COLORFILL)
					sc = colorize;
				else if (flags & EFX_TINT)
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
				if (flags & (EFX_SUB | EFX_ALPHA))
				{
					if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
					if (flags & EFX_ALPHA)
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					else if (flags & EFX_SUB)
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				}
				else if (flags & EFX_OPACITY)
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

//16-bit entry
bool ERdrBltFlip16(BLTFLIP_PARAMS, void **erdr_ptr, unsigned long flags)
{
	bool (**ERdrBltFlip_ptr)(BLTFLIP_PARAMS) = (bool(**)(BLTFLIP_PARAMS))(erdr_ptr);
	if (ERdrBltFlip_ptr)
	{
		flags &= 255;
		if (ERdrBltFlip_ptr[flags] != NULL)
			if (ERdrBltFlip_ptr[flags](BLTFLIP_PARAMS_PASS))
				return(true);
		if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
			flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
		if (ERdrBltFlip_ptr[flags] != NULL)
			if (ERdrBltFlip_ptr[flags](BLTFLIP_PARAMS_PASS))
				return(true);
	}
	else if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
		flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	opacity >>= 3;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (!(flags & EFX_COLORKEY) || sc != colorkey)
			{
				if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
				else if (flags & EFX_COLORFILL)
					sc = colorize;
				else if (flags & EFX_TINT)
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
				if (flags & (EFX_SUB | EFX_ALPHA))
				{
					if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
					if (flags & EFX_ALPHA)
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					else if (flags & EFX_SUB)
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				}
				else if (flags & EFX_OPACITY)
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

//32-bit entry
bool ERdrBltFlip32(BLTFLIP_PARAMS, void **erdr_ptr, unsigned long flags)
{
	bool (**ERdrBltFlip_ptr)(BLTFLIP_PARAMS) = (bool(**)(BLTFLIP_PARAMS))(erdr_ptr);
	if (ERdrBltFlip_ptr)
	{
		flags &= 255;
		if (ERdrBltFlip_ptr[flags] != NULL)
			if (ERdrBltFlip_ptr[flags](BLTFLIP_PARAMS_PASS))
				return(true);
		if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
			flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
		if (ERdrBltFlip_ptr[flags] != NULL)
			if (ERdrBltFlip_ptr[flags](BLTFLIP_PARAMS_PASS))
				return(true);
	}
	else if (flags & (EFX_OPACITY_OVH | EFX_OPACITY_OVL))
		flags = (flags & (EFX_COLORFILTER - EFX_OPACITY_OVH - EFX_OPACITY_OVL)) | EFX_OPACITY;
	long xctr;
	unsigned long sc;
	unsigned EINT32 *pdd = (unsigned EINT32 *)dst;
	unsigned EINT32 *psd = (unsigned EINT32 *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (!(flags & EFX_COLORKEY) || sc != colorkey)
			{
				if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
				else if (flags & EFX_COLORFILL)
					sc = colorize;
				else if (flags & EFX_TINT)
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
				if (flags & (EFX_SUB | EFX_ALPHA))
				{
					if (flags & EFX_OPACITY)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
					if (flags & EFX_ALPHA)
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
					else if (flags & EFX_SUB)
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				}
				else if (flags & EFX_OPACITY)
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

///////////////////////////////////////////////////////////////////
// Quick functions                                               //
///////////////////////////////////////////////////////////////////

void inline SxMiscColorConvertR(unsigned short *d, unsigned short *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned short m1, unsigned short m2, unsigned short m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = ((s[xc] >> o1) & m1) | ((s[xc] >> o2) & m2) | ((s[xc] >> o3) & m3);
		s += sw; d += dw;
	}
}

void inline SxMiscColorConvertR(unsigned long *d, unsigned short *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned long m1, unsigned long m2, unsigned long m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = ((s[xc] >> o1) & m1) | ((s[xc] >> o2) & m2) | ((s[xc] >> o3) & m3);
		s += sw; d += dw;
	}
}

void inline SxMiscColorConvertR(unsigned short *d, unsigned long *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned long m1, unsigned long m2, unsigned long m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = (unsigned short)(((s[xc] >> o1) & m1) | ((s[xc] >> o2) & m2) | ((s[xc] >> o3) & m3));
		s += sw; d += dw;
	}
}

void inline SxMiscColorConvertL(unsigned short *d, unsigned short *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned short m1, unsigned short m2, unsigned short m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = ((s[xc] << o1) & m1) | ((s[xc] << o2) & m2) | ((s[xc] << o3) & m3);
		s += sw; d += dw;
	}
}

void inline SxMiscColorConvertL(unsigned long *d, unsigned short *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned long m1, unsigned long m2, unsigned long m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = ((s[xc] << o1) & m1) | ((s[xc] << o2) & m2) | ((s[xc] << o3) & m3);
		s += sw; d += dw;
	}
}

void inline SxMiscColorConvertL(unsigned short *d, unsigned long *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned long m1, unsigned long m2, unsigned long m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = (unsigned short)(((s[xc] << o1) & m1) | ((s[xc] << o2) & m2) | ((s[xc] << o3) & m3));
		s += sw; d += dw;
	}
}

void inline SxMiscColorConvertR_lL(unsigned short *d, unsigned short *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned short m1, unsigned short m2, unsigned short m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = ((s[xc] >> o1) & m1) | ((s[xc] >> o2) & m2) | ((s[xc] << o3) & m3);
		s += sw; d += dw;
	}
}

void inline SxMiscColorConvertR_lL(unsigned long *d, unsigned short *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned long m1, unsigned long m2, unsigned long m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = ((s[xc] >> o1) & m1) | ((s[xc] >> o2) & m2) | ((s[xc] << o3) & m3);
		s += sw; d += dw;
	}
}

void inline SxMiscColorConvertR_lL(unsigned short *d, unsigned long *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned long m1, unsigned long m2, unsigned long m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = (unsigned short)(((s[xc] >> o1) & m1) | ((s[xc] >> o2) & m2) | ((s[xc] << o3) & m3));
		s += sw; d += dw;
	}
}

void inline SxMiscColorConvertL_fR(unsigned short *d, unsigned short *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned short m1, unsigned short m2, unsigned short m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = ((s[xc] >> o1) & m1) | ((s[xc] << o2) & m2) | ((s[xc] << o3) & m3);
		s += sw; d += dw;
	}
}

void inline SxMiscColorConvertL_fR(unsigned long *d, unsigned short *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned long m1, unsigned long m2, unsigned long m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = ((s[xc] >> o1) & m1) | ((s[xc] << o2) & m2) | ((s[xc] << o3) & m3);
		s += sw; d += dw;
	}
}

void inline SxMiscColorConvertL_fR(unsigned short *d, unsigned long *s, unsigned long dw, unsigned long sw, long cw, long ch, unsigned char o1, unsigned char o2, unsigned char o3, unsigned long m1, unsigned long m2, unsigned long m3)
{
	for (long yc = 0; yc < ch; yc++)
	{
		for (long xc = 0; xc < cw; xc++) d[xc] = (unsigned short)(((s[xc] >> o1) & m1) | ((s[xc] << o2) & m2) | ((s[xc] << o3) & m3));
		s += sw; d += dw;
	}
}

///////////////////////////////////////////////////////////////////
// RGBA blitters                                                 //
///////////////////////////////////////////////////////////////////

//Fast RGBA blit: default function, handling all filters (optimized for read-only surfaces)
void ExBltRGBAGenericPlus(long x, long y, long yctr, RECT *rc, unsigned char *dst, unsigned char *src, unsigned long dstwidth, unsigned long srcwidth, unsigned char videomode, long flags, long property1, long property2)
{
	unsigned EINT32 *psd = (unsigned EINT32 *)&src[(yctr * srcwidth + rc->left) * 4];
	unsigned long xctr, w = rc->right - rc->left;
	if (videomode == EDSP_INDEXED8)
	{
		unsigned char *pdd = &dst[(y * dstwidth + x)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				if (psd[xctr] >> 24)
					pdd[xctr] = (unsigned char)psd[xctr];
			pdd += dstwidth; psd += srcwidth; yctr++;
		}
	}
	else if (videomode == EDSP_TRUE12 || videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
	{
		unsigned short *pdd = (unsigned short *)&dst[(y * dstwidth + x) * 2];
		unsigned long sc;
		unsigned char sopac;
		if (videomode == EDSP_TRUE12)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sopac = (unsigned char)(15 - (psd[xctr] >> 28));
					if (sopac)
					{
						sc = psd[xctr] & 0xFFFFFF;
						sc = (((((sc & 3840) << 4) / (sopac + 1)) & 3840) | ((((sc & 240) << 4) / (sopac + 1)) & 240) | ((((sc & 15) << 4) / (sopac + 1)) & 15));
					}
					else
						sc = 0;
					if (flags & EFX_OPACITY)
						sopac = (unsigned char)((sopac * property1) >> 8);
					if (sopac)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 1911, property2);
						if (flags & (EFX_ALPHA | EFX_SUB))
						{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, sopac);
							if (flags & EFX_ALPHA)
								sc = EBCODE_MACRO_ADDALPHA(pdd[xctr], sc, 15, 240, 3840, 16, 256, 4096);
							else if (flags & EFX_SUB)
								sc = EBCODE_MACRO_SUBALPHA(pdd[xctr], sc, 15, 240, 3840);
						}
						else if (sopac < 15)
							sc = EBCODE_MACRO_OPACITY(pdd[xctr], sc, 3855, 240, 4, (16 - sopac), sopac);
						pdd[xctr] = (unsigned short)sc;
					}
				}
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
		else if (videomode == EDSP_TRUE15)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sopac = (unsigned char)(31 - (psd[xctr] >> 27));
					if (sopac)
					{
						sc = psd[xctr] & 0xFFFFFF;
						sc = (((((sc & 31744) << 5) / (sopac + 1)) & 31744) | ((((sc & 992) << 5) / (sopac + 1)) & 992) | ((((sc & 31) << 5) / (sopac + 1)) & 31));
					}
					else
						sc = 0;
					if (flags & EFX_OPACITY)
						sopac = (unsigned char)((sopac * property1) >> 8);
					if (sopac)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 992, 31744, 0, 5, 10, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 15855, property2);
						if (flags & (EFX_ALPHA | EFX_SUB))
						{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 31775, 992, 5, sopac);
							if (flags & EFX_ALPHA)
								sc = EBCODE_MACRO_ADDALPHA(pdd[xctr], sc, 31, 992, 31744, 32, 1024, 32768);
							else if (flags & EFX_SUB)
								sc = EBCODE_MACRO_SUBALPHA(pdd[xctr], sc, 31, 992, 31744);
						}
						else if (sopac < 31)
							sc = EBCODE_MACRO_OPACITY(pdd[xctr], sc, 31775, 992, 5, (32 - sopac), sopac);
						pdd[xctr] = (unsigned short)sc;
					}
				}
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
		else if (videomode == EDSP_TRUE16)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sopac = (unsigned char)(63 - (psd[xctr] >> 26));
					if (sopac)
					{
						sc = psd[xctr] & 0xFFFFFF;
						sc = (((((sc & 63488) << 6) / (sopac + 1)) & 63488) | ((((sc & 2016) << 6) / (sopac + 1)) & 2016) | ((((sc & 31) << 6) / (sopac + 1)) & 31));
					}
					else
						sc = 0;
					if (flags & EFX_OPACITY)
						sopac = (unsigned char)((sopac * property1) >> 8);
					if (sopac)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 31727, property2);
						if (flags & (EFX_ALPHA | EFX_SUB))
						{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 6, sopac);
							if (flags & EFX_ALPHA)
								sc = EBCODE_MACRO_ADDALPHA(pdd[xctr], sc, 31, 2016, 63488, 32, 2048, 65536);
							else if (flags & EFX_SUB)
								sc = EBCODE_MACRO_SUBALPHA(pdd[xctr], sc, 31, 2016, 63488);
						}
						else if (sopac < 63)
							sc = EBCODE_MACRO_OPACITY(pdd[xctr], sc, 63519, 2016, 6, (64 - sopac), sopac);
						pdd[xctr] = (unsigned short)sc;
					}
				}
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
	}
	else if (videomode == EDSP_TRUE24 || videomode == EDSP_TRUE24_INV || videomode == EDSP_TRUE32 || videomode == EDSP_TRUE32_INV)
	{
		unsigned long sc, dc;
		unsigned char sopac;
		unsigned char *pdd8 = &dst[(y * dstwidth + x) * 3];
		unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[(y * dstwidth + x) * 4];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sopac = (unsigned char)(255 - (psd[xctr] >> 24));
				if (sopac)
				{
					sc = psd[xctr] & 0xFFFFFF;
					sc = (((((sc & 0xFF0000) << 8) / (sopac + 1)) & 0xFF0000) | ((((sc & 0xFF00) << 8) / (sopac + 1)) & 0xFF00) | ((((sc & 0xFF) << 8) / (sopac + 1)) & 0xFF));
				}
				else
					sc = 0;
				if (flags & EFX_OPACITY)
					sopac = (unsigned char)((sopac * property1) >> 8);
				if (sopac)
				{
					if (videomode == EDSP_TRUE24 || videomode == EDSP_TRUE24_INV)
						dc = (unsigned long)(pdd8[xctr * 3] | (pdd8[xctr * 3 + 1] << 8) | (pdd8[xctr * 3 + 2] << 16));
					else
						dc = pdd32[xctr];
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, property2, unsigned long)
					else if (flags & EFX_COLORFILL)
						sc = property2;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, property2);
					if (flags & (EFX_ALPHA | EFX_SUB))
					{
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, sopac);
						if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000);
					}
					else if (sopac < 255)
						sc = EBCODE_MACRO_OPACITY(dc, sc, 0xFF00FF, 0xFF00, 8, (256 - sopac), sopac);
					if (videomode == EDSP_TRUE24)
					{
						pdd8[xctr * 3]     = (unsigned char)(sc & 0xFF);
						pdd8[xctr * 3 + 1] = (unsigned char)((sc & 0xFF00) >> 8);
						pdd8[xctr * 3 + 2] = (unsigned char)((sc & 0xFF0000) >> 16);
					}
					else
						pdd32[xctr] = sc;
				}
			}
			pdd32 += dstwidth; pdd8 += dstwidth * 3; psd += srcwidth; yctr++;
		}
	}
}

//Fast RGBA blit: default function, handling all filters
void ExBltRGBAGeneric(long x, long y, long yctr, RECT *rc, unsigned char *dst, unsigned char *src, unsigned long dstwidth, unsigned long srcwidth, unsigned char videomode, long flags, long property1, long property2)
{
	unsigned EINT32 *psd = (unsigned EINT32 *)&src[(yctr * srcwidth + rc->left) * 4];
	unsigned long xctr, w = rc->right - rc->left;
	if (videomode == EDSP_INDEXED8)
	{
		unsigned char *pdd = &dst[(y * dstwidth + x)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				if (psd[xctr] >> 24)
					pdd[xctr] = (unsigned char)psd[xctr];
			pdd += dstwidth; psd += srcwidth; yctr++;
		}
	}
	else if (videomode == EDSP_TRUE12 || videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
	{
		unsigned short *pdd = (unsigned short *)&dst[(y * dstwidth + x) * 2];
		unsigned long sc;
		unsigned char sopac;
		if (videomode == EDSP_TRUE12)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sc = psd[xctr] & 0xFFFFFF;
					sopac = (unsigned char)(psd[xctr] >> 28);
					if (flags & EFX_OPACITY)
						sopac = (unsigned char)((sopac * property1) >> 8);
					if (sopac)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 1911, property2);
						if (flags & (EFX_ALPHA | EFX_SUB))
						{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, sopac);
							if (flags & EFX_ALPHA)
								sc = EBCODE_MACRO_ADDALPHA(pdd[xctr], sc, 15, 240, 3840, 16, 256, 4096);
							else if (flags & EFX_SUB)
								sc = EBCODE_MACRO_SUBALPHA(pdd[xctr], sc, 15, 240, 3840);
						}
						else if (sopac < 15)
							sc = EBCODE_MACRO_OPACITY(pdd[xctr], sc, 3855, 240, 4, (16 - sopac), sopac);
						pdd[xctr] = (unsigned short)sc;
					}
				}
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
		else if (videomode == EDSP_TRUE15)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sc = psd[xctr] & 0xFFFFFF;
					sopac = (unsigned char)(psd[xctr] >> 27);
					if (flags & EFX_OPACITY)
						sopac = (unsigned char)((sopac * property1) >> 8);
					if (sopac)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 992, 31744, 0, 5, 10, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 15855, property2);
						if (flags & (EFX_ALPHA | EFX_SUB))
						{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 31775, 992, 5, sopac);
							if (flags & EFX_ALPHA)
								sc = EBCODE_MACRO_ADDALPHA(pdd[xctr], sc, 31, 992, 31744, 32, 1024, 32768);
							else if (flags & EFX_SUB)
								sc = EBCODE_MACRO_SUBALPHA(pdd[xctr], sc, 31, 992, 31744);
						}
						else if (sopac < 31)
							sc = EBCODE_MACRO_OPACITY(pdd[xctr], sc, 31775, 992, 5, (32 - sopac), sopac);
						pdd[xctr] = (unsigned short)sc;
					}
				}
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
		else if (videomode == EDSP_TRUE16)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					sc = psd[xctr] & 0xFFFFFF;
					sopac = (unsigned char)(psd[xctr] >> 26);
					if (flags & EFX_OPACITY)
						sopac = (unsigned char)((sopac * property1) >> 8);
					if (sopac)
					{
						if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
							EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, property2, unsigned short)
						else if (flags & EFX_COLORFILL)
							sc = property2;
						else if (flags & EFX_TINT)
							sc = EBCODE_MACRO_TINT(sc, 31727, property2);
						if (flags & (EFX_ALPHA | EFX_SUB))
						{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 6, sopac);
							if (flags & EFX_ALPHA)
								sc = EBCODE_MACRO_ADDALPHA(pdd[xctr], sc, 31, 2016, 63488, 32, 2048, 65536);
							else if (flags & EFX_SUB)
								sc = EBCODE_MACRO_SUBALPHA(pdd[xctr], sc, 31, 2016, 63488);
						}
						else if (sopac < 63)
							sc = EBCODE_MACRO_OPACITY(pdd[xctr], sc, 63519, 2016, 6, (64 - sopac), sopac);
						pdd[xctr] = (unsigned short)sc;
					}
				}
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
	}
	else if (videomode == EDSP_TRUE24 || videomode == EDSP_TRUE24_INV || videomode == EDSP_TRUE32 || videomode == EDSP_TRUE32_INV)
	{
		unsigned long sc, dc;
		unsigned char sopac;
		unsigned char *pdd8 = &dst[(y * dstwidth + x) * 3];
		unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[(y * dstwidth + x) * 4];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = psd[xctr] & 0xFFFFFF;
				sopac = (unsigned char)(psd[xctr] >> 24);
				if (flags & EFX_OPACITY)
					sopac = (unsigned char)((sopac * property1) >> 8);
				if (sopac)
				{
					if (videomode == EDSP_TRUE24)
						dc = (unsigned long)(pdd8[xctr * 3] | (pdd8[xctr * 3 + 1] << 8) | (pdd8[xctr * 3 + 2] << 16));
					else
						dc = pdd32[xctr];
					if ((flags & EFX_COLORIZE) == EFX_COLORIZE)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, property2, unsigned long)
					else if (flags & EFX_COLORFILL)
						sc = property2;
					else if (flags & EFX_TINT)
						sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, property2);
					if (flags & (EFX_ALPHA | EFX_SUB))
					{
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, sopac);
						if (flags & EFX_ALPHA)
							sc = EBCODE_MACRO_ADDALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
						else if (flags & EFX_SUB)
							sc = EBCODE_MACRO_SUBALPHA(dc, sc, 0xFF, 0xFF00, 0xFF0000);
					}
					else if (sopac < 255)
						sc = EBCODE_MACRO_OPACITY(dc, sc, 0xFF00FF, 0xFF00, 8, (256 - sopac), sopac);
					if (videomode == EDSP_TRUE24 || videomode == EDSP_TRUE24_INV)
					{
						pdd8[xctr * 3]     = (unsigned char)(sc & 0xFF);
						pdd8[xctr * 3 + 1] = (unsigned char)((sc & 0xFF00) >> 8);
						pdd8[xctr * 3 + 2] = (unsigned char)((sc & 0xFF0000) >> 16);
					}
					else
						pdd32[xctr] = sc;
				}
			}
			pdd32 += dstwidth; pdd8 += dstwidth * 3; psd += srcwidth; yctr++;
		}
	}
}

//Fast RGBA blit: no filters (optimized for read-only surfaces)
void ExBltRGBAPlus(long x, long y, long yctr, RECT *rc, unsigned char *dst, unsigned char *src, unsigned long dstwidth, unsigned long srcwidth, unsigned char videomode)
{
	unsigned EINT32 *psd = (unsigned EINT32 *)&src[(yctr * srcwidth + rc->left) * 4];
	long xctr, w = rc->right - rc->left;
	if (videomode == EDSP_INDEXED8)
	{
		unsigned char *pdd = &dst[(y * dstwidth + x)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				if (psd[xctr] >> 24 != 0xFF)
					pdd[xctr] = (unsigned char)psd[xctr];
			pdd += dstwidth; psd += srcwidth; yctr++;
		}
	}
	else if (videomode == EDSP_TRUE12 || videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
	{
		unsigned short *pdd = (unsigned short *)&dst[(y * dstwidth + x) * 2];
		if (videomode == EDSP_TRUE12)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
					if (!(psd[xctr] >> 24))
						pdd[xctr] = (unsigned short)(psd[xctr] & 0xFFFFFF);
					else if (psd[xctr] >> 24 != 0xFF)
						pdd[xctr] = (unsigned short)EBCODE_MACRO_HPACITY(pdd[xctr], psd[xctr], 3855, 240, 4, (psd[xctr] >> 28));
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
		else if (videomode == EDSP_TRUE15)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
					if (!(psd[xctr] >> 24))
						pdd[xctr] = (unsigned short)(psd[xctr] & 0xFFFFFF);
					else if (psd[xctr] >> 24 != 0xFF)
						pdd[xctr] = (unsigned short)EBCODE_MACRO_HPACITY(pdd[xctr], psd[xctr], 31775, 992, 5, (psd[xctr] >> 27));
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
		else if (videomode == EDSP_TRUE16)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
					if (!(psd[xctr] >> 24))
						pdd[xctr] = (unsigned short)(psd[xctr] & 0xFFFFFF);
					else if (psd[xctr] >> 24 != 0xFF)
						pdd[xctr] = (unsigned short)EBCODE_MACRO_HPACITY(pdd[xctr], psd[xctr], 63519, 2016, 6, (psd[xctr] >> 26));
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
	}
	else if (videomode == EDSP_TRUE24 || videomode == EDSP_TRUE24_INV)
	{
		unsigned long tc;
		unsigned char *pdd = &dst[(y * dstwidth + x) * 3];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				if (psd[xctr] >> 24 != 0xFF)
				{
					tc = (unsigned long)(pdd[xctr * 3] | (pdd[xctr * 3 + 1] << 8) | (pdd[xctr * 3 + 2] << 16));
					if (psd[xctr] & 0xFF000000)
						tc = EBCODE_MACRO_HPACITY(tc, psd[xctr], 0xFF00FF, 0xFF00, 8, (psd[xctr] >> 24));
					else
						tc = psd[xctr] & 0xFFFFFF;
					pdd[xctr * 3]     = (unsigned char)(tc & 0xFF);
					pdd[xctr * 3 + 1] = (unsigned char)((tc & 0xFF00) >> 8);
					pdd[xctr * 3 + 2] = (unsigned char)((tc & 0xFF0000) >> 16);
				}
			pdd += dstwidth * 3; psd += srcwidth; yctr++;
		}
	}
	else if (videomode == EDSP_TRUE32 || videomode == EDSP_TRUE32_INV)
	{
		unsigned EINT32 *pdd = (unsigned EINT32 *)&dst[(y * dstwidth + x) * 4];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				if (!(psd[xctr] >> 24))
					pdd[xctr] = psd[xctr] & 0xFFFFFF;
				else if (psd[xctr] >> 24 != 0xFF)
					pdd[xctr] = EBCODE_MACRO_HPACITY(pdd[xctr], (psd[xctr] & 0xFFFFFF), 0xFF00FF, 0xFF00, 8, (psd[xctr] >> 24));
			pdd += dstwidth; psd += srcwidth; yctr++;
		}
	}
}

//Fast RGBA blit: no filters
void ExBltRGBA(long x, long y, long yctr, RECT *rc, unsigned char *dst, unsigned char *src, unsigned long dstwidth, unsigned long srcwidth, unsigned char videomode)
{
	unsigned EINT32 *psd = (unsigned EINT32 *)&src[(yctr * srcwidth + rc->left) * 4];
	long xctr, w = rc->right - rc->left;
	if (videomode == EDSP_INDEXED8)
	{
		unsigned char *pdd = &dst[(y * dstwidth + x)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				if (psd[xctr] >> 24)
					pdd[xctr] = (unsigned char)(psd[xctr] & 255);
			pdd += dstwidth; psd += srcwidth; yctr++;
		}
	}
	else if (videomode == EDSP_TRUE12 || videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
	{
		unsigned short *pdd = (unsigned short *)&dst[(y * dstwidth + x) * 2];
		if (videomode == EDSP_TRUE12)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
					if ((unsigned)(psd[xctr] & 0xFF000000) == 0xFF000000)
						pdd[xctr] = (unsigned short)(psd[xctr] & 0xFFFFFF);
					else if (psd[xctr] >> 24)
						pdd[xctr] = (unsigned short)EBCODE_MACRO_OPACITY(pdd[xctr], psd[xctr], 3855, 240, 4, (16 - (psd[xctr] >> 28)), (psd[xctr] >> 28));
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
		else if (videomode == EDSP_TRUE15)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
					if ((unsigned)(psd[xctr] & 0xFF000000) == 0xFF000000)
						pdd[xctr] = (unsigned short)(psd[xctr] & 0xFFFFFF);
					else if (psd[xctr] >> 24)
						pdd[xctr] = (unsigned short)EBCODE_MACRO_OPACITY(pdd[xctr], psd[xctr], 31775, 992, 5, (32 - (psd[xctr] >> 27)), (psd[xctr] >> 27));
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
		else if (videomode == EDSP_TRUE16)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
					if ((unsigned)(psd[xctr] & 0xFF000000) == 0xFF000000)
						pdd[xctr] = (unsigned short)(psd[xctr] & 0xFFFFFF);
					else if (psd[xctr] >> 24)
						pdd[xctr] = (unsigned short)EBCODE_MACRO_OPACITY(pdd[xctr], psd[xctr], 63519, 2016, 6, (64 - (psd[xctr] >> 26)), (psd[xctr] >> 26));
				pdd += dstwidth; psd += srcwidth; yctr++;
			}
		}
	}
	else if (videomode == EDSP_TRUE24 || videomode == EDSP_TRUE24_INV)
	{
		unsigned long tc;
		unsigned char *pdd = &dst[(y * dstwidth + x) * 3];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				if (psd[xctr] >> 24)
				{
					tc = (unsigned long)(pdd[xctr * 3] | (pdd[xctr * 3 + 1] << 8) | (pdd[xctr * 3 + 2] << 16));
					if ((unsigned)(psd[xctr] & 0xFF000000) == 0xFF000000)
						tc = EBCODE_MACRO_OPACITY(tc, psd[xctr], 0xFF00FF, 0xFF00, 8, (256 - (psd[xctr] >> 24)), (psd[xctr] >> 24));
					else
						tc = psd[xctr] & 0xFFFFFF;
					pdd[xctr * 3]     = (unsigned char)(tc & 0xFF);
					pdd[xctr * 3 + 1] = (unsigned char)((tc & 0xFF00) >> 8);
					pdd[xctr * 3 + 2] = (unsigned char)((tc & 0xFF0000) >> 16);
				}
			pdd += dstwidth * 3; psd += srcwidth; yctr++;
		}
	}
	else if (videomode == EDSP_TRUE32 || videomode == EDSP_TRUE32_INV)
	{
		unsigned EINT32 *pdd = (unsigned EINT32 *)&dst[(y * dstwidth + x) * 4];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				if ((unsigned)(psd[xctr] & 0xFF000000) == 0xFF000000)
					pdd[xctr] = psd[xctr] & 0xFFFFFF;
				else if (psd[xctr] >> 24)
					pdd[xctr] = EBCODE_MACRO_OPACITY(pdd[xctr], psd[xctr], 0xFF00FF, 0xFF00, 8, (256 - (psd[xctr] >> 24)), (psd[xctr] >> 24));
			pdd += dstwidth; psd += srcwidth; yctr++;
		}
	}
}

///////////////////////////////////////////////////////////////////
// Color conversion blitters (device independent)                //
///////////////////////////////////////////////////////////////////

//Quick color conversion between 2 surfaces
void ExBltColorConvert(unsigned char *dst, unsigned char *src, long dstwidth, long srcwidth, unsigned char dstvideo, unsigned char srcvideo, long copywidth, long copyheight, COLORREF *paldata, unsigned long *realpal)
{
	//Source and destination are the same: memcopy
	if (srcvideo == dstvideo)
	{
		signed char bytewidth = 1;
		if (srcvideo == EDSP_TRUE12 || srcvideo == EDSP_TRUE15 || srcvideo == EDSP_TRUE16)
			bytewidth = 2;
		else if (srcvideo == EDSP_TRUE24 || srcvideo == EDSP_TRUE24_INV)
			bytewidth = 3;
		else if (srcvideo == EDSP_TRUE32 || srcvideo == EDSP_TRUE32_INV)
			bytewidth = 4;
		for (long yc = 0; yc < copyheight; yc++)
		{
			ClassEMemory::Copy(dst, src, copywidth * bytewidth);
			src += srcwidth * bytewidth;
			dst += dstwidth * bytewidth;
		}
		return;
	}

	//Set up palette for source (palette) to true color conversion
	if (realpal[256] != dstvideo && srcvideo == EDSP_INDEXED8 && dstvideo != EDSP_INDEXED8)
	{
		for (long palc = 0; palc < 256; palc++)
		{
			if (dstvideo == EDSP_TRUE24 || dstvideo == EDSP_TRUE32)
				realpal[palc] = E_RGBX(E_GETB(paldata[palc]), E_GETG(paldata[palc]), E_GETR(paldata[palc]));
			else if (dstvideo == EDSP_TRUE12)
				realpal[palc] = (((paldata[palc] & 0xFF) << 4) & 3840) | (((paldata[palc] & 0xFF00) >> 8) & 240) | ((paldata[palc] & 0xFF0000) >> 20);
			else if (dstvideo == EDSP_TRUE15)
				realpal[palc] = (((paldata[palc] & 0xFF) << 7) & 31744) | (((paldata[palc] & 0xFF00) >> 6) & 992) | ((paldata[palc] & 0xFF0000) >> 19);
			else if (dstvideo == EDSP_TRUE16)
				realpal[palc] = (((paldata[palc] & 0xFF) << 8) & 63488) | (((paldata[palc] & 0xFF00) >> 5) & 2016) | ((paldata[palc] & 0xFF0000) >> 19);
			else if (dstvideo == EDSP_TRUE24_INV || dstvideo == EDSP_TRUE32_INV)
				realpal[palc] = paldata[palc];
		}
		realpal[256] = dstvideo;
	}

	//Palette to true color blit
	if (srcvideo == EDSP_INDEXED8)
	{
		if ((dstvideo == EDSP_TRUE12 || dstvideo == EDSP_TRUE15 || dstvideo == EDSP_TRUE16))
		{
			unsigned short *dstp = (unsigned short *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = (unsigned short)realpal[src[xc]];
				src += srcwidth; dstp += dstwidth;
			}
			return;
		}
		else if (dstvideo == EDSP_TRUE24 || dstvideo == EDSP_TRUE24_INV)
		{
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
				{
					dst[xc * 3]     = (unsigned char)(realpal[src[xc]] & 0xFF);
					dst[xc * 3 + 1] = (unsigned char)((realpal[src[xc]] >> 8) & 0xFF);
					dst[xc * 3 + 2] = (unsigned char)((realpal[src[xc]] >> 16) & 0xFF);
				}
				src += srcwidth; dst += dstwidth * 3;
			}
		}
		else if (dstvideo == EDSP_TRUE32 || dstvideo == EDSP_TRUE32_INV)
		{
			unsigned EINT32 *dstp = (unsigned EINT32 *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = realpal[src[xc]];
				src += srcwidth; dstp += dstwidth;
			}
		}
		return;
	}

	//24-bit inversion
	if ((srcvideo == EDSP_TRUE24 || srcvideo == EDSP_TRUE24_INV) && (dstvideo == EDSP_TRUE24 || dstvideo == EDSP_TRUE24_INV))
	{
		for (long yc = 0; yc < copyheight; yc++)
		{
			for (long xc = 0; xc < copywidth; xc++)
			{
				dst[xc * 3 + 2] = src[xc * 3];
				dst[xc * 3 + 1] = src[xc * 3 + 1];
				dst[xc * 3] = src[xc * 3 + 2];
			}
			src += srcwidth * 3; dst += dstwidth * 3;
		}
		return;
	}

	//32-bit inversion
	if ((srcvideo == EDSP_TRUE32 || srcvideo == EDSP_TRUE32_INV) && (dstvideo == EDSP_TRUE32 || dstvideo == EDSP_TRUE32_INV))
	{
		unsigned EINT32 *dstp = (unsigned EINT32 *)dst;
		unsigned EINT32 *srcp = (unsigned EINT32 *)src;
		for (long yc = 0; yc < copyheight; yc++)
		{
			for (long xc = 0; xc < copywidth; xc++)
				dstp[xc] = ((srcp[xc] & 0xFF) << 16) | (srcp[xc] & 0xFF00FF00) | ((srcp[xc] & 0xFF0000) >> 16);
			srcp += srcwidth; dstp += dstwidth;
		}
		return;
	}

	//Others
	if (dstvideo == EDSP_TRUE12)
	{
		if (srcvideo == EDSP_TRUE24)
		{
			unsigned short *dstp = (unsigned short *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = (src[xc * 3] >> 4) | (src[xc * 3 + 1] & 240) | ((src[xc * 3 + 2] << 4) & 3840);
				src += srcwidth * 3; dstp += dstwidth;
			}
		}
		else if (srcvideo == EDSP_TRUE24_INV)
		{
			unsigned short *dstp = (unsigned short *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = (src[xc * 3 + 2] >> 4) | (src[xc * 3 + 1] & 240) | ((src[xc * 3] << 4) & 3840);
				src += srcwidth * 3; dstp += dstwidth;
			}
		}
		else if (srcvideo == EDSP_TRUE15)
			SxMiscColorConvertR((unsigned short *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 1, 2, 3, 15, 240, 3840);
		else if (srcvideo == EDSP_TRUE16)
			SxMiscColorConvertR((unsigned short *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 1, 3, 4, 15, 240, 3840);
		else if (srcvideo == EDSP_TRUE32)
			SxMiscColorConvertR((unsigned short *)dst, (unsigned long *)src, dstwidth, srcwidth, copywidth, copyheight, 4, 8, 12, 15, 240, 3840);
		else if (srcvideo == EDSP_TRUE32_INV)
			SxMiscColorConvertR_lL((unsigned short *)dst, (unsigned long *)src, dstwidth, srcwidth, copywidth, copyheight, 20, 8, 4, 15, 240, 3840);
	}
	else if (dstvideo == EDSP_TRUE15)
	{
		if (srcvideo == EDSP_TRUE24)
		{
			unsigned short *dstp = (unsigned short *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = (src[xc * 3] >> 3) | ((src[xc * 3 + 1] << 2) & 992) | ((src[xc * 3 + 2] << 7) & 31744);
				src += srcwidth * 3; dstp += dstwidth;
			}
		}
		else if (srcvideo == EDSP_TRUE24_INV)
		{
			unsigned short *dstp = (unsigned short *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = (src[xc * 3 + 2] >> 3) | ((src[xc * 3 + 1] << 2) & 992) | ((src[xc * 3] << 7) & 31744);
				src += srcwidth * 3; dstp += dstwidth;
			}
		}
		else if (srcvideo == EDSP_TRUE12)
			SxMiscColorConvertL((unsigned short *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 1, 2, 3, 31, 992, 31744);
		else if (srcvideo == EDSP_TRUE16)
			SxMiscColorConvertR((unsigned short *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 0, 0, 1, 31, 0, 32736);
		else if (srcvideo == EDSP_TRUE32)
			SxMiscColorConvertR((unsigned short *)dst, (unsigned long *)src, dstwidth, srcwidth, copywidth, copyheight, 3, 6, 9, 31, 992, 31744);
		else if (srcvideo == EDSP_TRUE32_INV)
			SxMiscColorConvertR_lL((unsigned short *)dst, (unsigned long *)src, dstwidth, srcwidth, copywidth, copyheight, 19, 6, 7, 31, 992, 31744);
	}
	else if (dstvideo == EDSP_TRUE16)
	{
		if (srcvideo == EDSP_TRUE24)
		{
			unsigned short *dstp = (unsigned short *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = (src[xc * 3] >> 3) | ((src[xc * 3 + 1] << 3) & 2016) | ((src[xc * 3 + 2] << 8) & 63488);
				src += srcwidth * 3; dstp += dstwidth;
			}
		}
		else if (srcvideo == EDSP_TRUE24_INV)
		{
			unsigned short *dstp = (unsigned short *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = (src[xc * 3] >> 3) | ((src[xc * 3 + 1] << 3) & 2016) | ((src[xc * 3 + 2] << 8) & 63488);
				src += srcwidth * 3; dstp += dstwidth;
			}
		}
		else if (srcvideo == EDSP_TRUE12)
			SxMiscColorConvertL((unsigned short *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 1, 3, 4, 31, 2016, 63488);
		else if (srcvideo == EDSP_TRUE15)
			SxMiscColorConvertL((unsigned short *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 0, 0, 1, 31, 0, 65504);
		else if (srcvideo == EDSP_TRUE32)
			SxMiscColorConvertR((unsigned short *)dst, (unsigned long *)src, dstwidth, srcwidth, copywidth, copyheight, 3, 5, 8, 31, 2016, 63488);
		else if (srcvideo == EDSP_TRUE32_INV)
			SxMiscColorConvertR_lL((unsigned short *)dst, (unsigned long *)src, dstwidth, srcwidth, copywidth, copyheight, 19, 5, 8, 31, 2016, 63488);
	}
	else if (dstvideo == EDSP_TRUE24)
	{
		if (srcvideo == EDSP_TRUE12)
		{
			unsigned short *srcp = (unsigned short *)src;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
				{
					dst[xc * 3] = srcp[xc] << 4;
					dst[xc * 3 + 1] = srcp[xc] & 240;
					dst[xc * 3 + 2] = (srcp[xc] >> 4) & 240;
				}
				srcp += srcwidth; dst += dstwidth * 3;
			}
		}
		else if (srcvideo == EDSP_TRUE15)
		{
			unsigned short *srcp = (unsigned short *)src;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
				{
					dst[xc * 3] = srcp[xc] << 3;
					dst[xc * 3 + 1] = (srcp[xc] >> 2) & 248;
					dst[xc * 3 + 2] = (srcp[xc] >> 7) & 248;
				}
				srcp += srcwidth; dst += dstwidth * 3;
			}
		}
		else if (srcvideo == EDSP_TRUE16)
		{
			unsigned short *srcp = (unsigned short *)src;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
				{
					dst[xc * 3] = srcp[xc] << 3;
					dst[xc * 3 + 1] = (srcp[xc] >> 3) & 252;
					dst[xc * 3 + 2] = (srcp[xc] >> 8) & 248;
				}
				srcp += srcwidth; dst += dstwidth * 3;
			}
		}
		else if (srcvideo == EDSP_TRUE32)
		{
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
				{
					dst[xc * 3] = src[xc * 4];
					dst[xc * 3 + 1] = src[xc * 4 + 1];
					dst[xc * 3 + 2] = src[xc * 4 + 2];
				}
				src += srcwidth * 4; dst += dstwidth * 3;
			}
		}
		else if (srcvideo == EDSP_TRUE32_INV)
		{
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
				{
					dst[xc * 3] = src[xc * 4 + 2];
					dst[xc * 3 + 1] = src[xc * 4 + 1];
					dst[xc * 3 + 2] = src[xc * 4];
				}
				src += srcwidth * 4; dst += dstwidth * 3;
			}
		}
	}
	else if (dstvideo == EDSP_TRUE24_INV)
	{
		if (srcvideo == EDSP_TRUE12)
		{
			unsigned short *srcp = (unsigned short *)src;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
				{
					dst[xc * 3 + 2] = srcp[xc] << 4;
					dst[xc * 3 + 1] = srcp[xc] & 240;
					dst[xc * 3] = (srcp[xc] >> 4) & 240;
				}
				srcp += srcwidth; dst += dstwidth * 3;
			}
		}
		else if (srcvideo == EDSP_TRUE15)
		{
			unsigned short *srcp = (unsigned short *)src;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
				{
					dst[xc * 3 + 2] = srcp[xc] << 3;
					dst[xc * 3 + 1] = (srcp[xc] >> 2) & 248;
					dst[xc * 3] = (srcp[xc] >> 7) & 248;
				}
				srcp += srcwidth; dst += dstwidth * 3;
			}
		}
		else if (srcvideo == EDSP_TRUE16)
		{
			unsigned short *srcp = (unsigned short *)src;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
				{
					dst[xc * 3 + 2] = srcp[xc] << 3;
					dst[xc * 3 + 1] = (srcp[xc] >> 3) & 252;
					dst[xc * 3] = (srcp[xc] >> 8) & 248;
				}
				srcp += srcwidth; dst += dstwidth * 3;
			}
		}
		else if (srcvideo == EDSP_TRUE32)
		{
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
				{
					dst[xc * 3] = src[xc * 4 + 2];
					dst[xc * 3 + 1] = src[xc * 4 + 1];
					dst[xc * 3 + 2] = src[xc * 4];
				}
				src += srcwidth * 4; dst += dstwidth * 3;
			}
		}
		else if (srcvideo == EDSP_TRUE32_INV)
		{
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
				{
					dst[xc * 3] = src[xc * 4];
					dst[xc * 3 + 1] = src[xc * 4 + 1];
					dst[xc * 3 + 2] = src[xc * 4 + 2];
				}
				src += srcwidth * 4; dst += dstwidth * 3;
			}
		}
	}
	else if (dstvideo == EDSP_TRUE32)
	{
		if (srcvideo == EDSP_TRUE24)
		{
			unsigned EINT32 *dstp = (unsigned EINT32 *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = src[xc * 3] | (src[xc * 3 + 1] << 8) | (src[xc * 3 + 2] << 16);
				src += srcwidth * 3; dstp += dstwidth;
			}
		}
		else if (srcvideo == EDSP_TRUE24_INV)
		{
			unsigned EINT32 *dstp = (unsigned EINT32 *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = src[xc * 3 + 2] | (src[xc * 3 + 1] << 8) | (src[xc * 3] << 16);
				src += srcwidth * 3; dstp += dstwidth;
			}
		}
		else if (srcvideo == EDSP_TRUE12)
			SxMiscColorConvertL((unsigned long *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 4, 8, 12, 0xF0, 0xF000, 0xF00000);
		else if (srcvideo == EDSP_TRUE15)
			SxMiscColorConvertL((unsigned long *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 3, 6, 9, 0xF8, 0xF800, 0xF80000);
		else if (srcvideo == EDSP_TRUE16)
			SxMiscColorConvertL((unsigned long *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 3, 5, 8, 0xF8, 0xFC00, 0xF80000);
	}
	else if (dstvideo == EDSP_TRUE32_INV)
	{
		if (srcvideo == EDSP_TRUE24)
		{
			unsigned EINT32 *dstp = (unsigned EINT32 *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = src[xc * 3 + 2] | (src[xc * 3 + 1] << 8) | (src[xc * 3] << 16);
				src += srcwidth * 3; dstp += dstwidth;
			}
		}
		else if (srcvideo == EDSP_TRUE24_INV)
		{
			unsigned EINT32 *dstp = (unsigned EINT32 *)dst;
			for (long yc = 0; yc < copyheight; yc++)
			{
				for (long xc = 0; xc < copywidth; xc++)
					dstp[xc] = src[xc * 3] | (src[xc * 3 + 1] << 8) | (src[xc * 3 + 2] << 16);
				src += srcwidth * 3; dstp += dstwidth;
			}
		}
		else if (srcvideo == EDSP_TRUE12)
			SxMiscColorConvertL_fR((unsigned long *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 4, 8, 20, 0xF0, 0xF000, 0xF00000);
		else if (srcvideo == EDSP_TRUE15)
			SxMiscColorConvertL_fR((unsigned long *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 7, 6, 19, 0xF8, 0xF800, 0xF80000);
		else if (srcvideo == EDSP_TRUE16)
			SxMiscColorConvertL_fR((unsigned long *)dst, (unsigned short *)src, dstwidth, srcwidth, copywidth, copyheight, 8, 5, 19, 0xF8, 0xFC00, 0xF80000);
	}
}
