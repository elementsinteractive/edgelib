#include "defs.h"

bool ERdrBltFast32_ck_tn_ol(BLTFAST_PARAMS)
{
	EINT32 xctr;
	unsigned EINT32 sc;
	unsigned EINT32 *pdd = (unsigned EINT32 *)dst;
	unsigned EINT32 *psd = (unsigned EINT32 *)src;
		unsigned char opshl = 0;
		unsigned EINT32 opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

