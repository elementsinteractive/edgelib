#include "defs.h"

bool ERdrBltFlip12_ck_cl_op_ad(BLTFLIP_PARAMS)
{
	EINT32 xctr;
	unsigned EINT32 sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 4;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

