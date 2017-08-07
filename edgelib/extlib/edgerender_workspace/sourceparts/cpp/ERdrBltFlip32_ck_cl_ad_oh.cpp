#include "defs.h"

bool ERdrBltFlip32_ck_cl_ad_oh(BLTFLIP_PARAMS)
{
	EINT32 xctr;
	unsigned EINT32 sc;
	unsigned EINT32 *pdd = (unsigned EINT32 *)dst;
	unsigned EINT32 *psd = (unsigned EINT32 *)src;
		unsigned char opshl = 0;
		unsigned EINT32 opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned EINT32)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

