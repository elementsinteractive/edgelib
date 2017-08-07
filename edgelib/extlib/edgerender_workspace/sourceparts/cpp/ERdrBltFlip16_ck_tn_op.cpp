#include "defs.h"

bool ERdrBltFlip16_ck_tn_op(BLTFLIP_PARAMS)
{
	EINT32 xctr;
	unsigned EINT32 sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 3;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
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

