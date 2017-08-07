#include "defs.h"

bool ERdrBltFast16_ck_tn_ad_oh(BLTFAST_PARAMS)
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
		unsigned char opshl = 0;
		unsigned EINT32 opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned EINT32 opdbland = opand | (opand << 16);
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
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
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
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
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
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
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
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
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

