#include "defs.h"

bool ERdrBltFast12_cl_op_ad(BLTFAST_PARAMS)
{
	EINT32 xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned EINT32 *pdd32, *psd32;
	if ((EINTPTR)(pdd) & 2)
		pdd32 = (unsigned EINT32 *)&pdd[1];
	else
		pdd32 = (unsigned EINT32 *)pdd;
	if ((EINTPTR)(psd) & 2)
		psd32 = (unsigned EINT32 *)&psd[1];
	else
		psd32 = (unsigned EINT32 *)psd;
	EINT32 yctr;
	unsigned EINT32 sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((EINTPTR)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((EINTPTR)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned EINT32 sc_mod;
	colorize |= (colorize << 16);
	if ((EINTPTR)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned EINT32)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned EINT32)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
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
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned EINT32)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned EINT32)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

