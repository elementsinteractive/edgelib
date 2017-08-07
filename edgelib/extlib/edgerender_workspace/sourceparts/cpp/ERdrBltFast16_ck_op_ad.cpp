#include "defs.h"

bool ERdrBltFast16_ck_op_ad(BLTFAST_PARAMS)
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
		opacity >>= 3;
	if (w)
	{
		if ((EINTPTR)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
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
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned EINT32 sc_org;
	colorkey |= (colorkey << 16);
	unsigned EINT32 sc_mod;
	colorize |= (colorize << 16);
	if ((EINTPTR)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
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
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

