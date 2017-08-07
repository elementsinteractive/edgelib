#include "defs.h"

bool ERdrBltFlip16_tn_sb_oh(BLTFLIP_PARAMS)
{
	EINT32 xctr;
	unsigned EINT32 sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
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
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

