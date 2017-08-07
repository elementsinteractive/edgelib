#include "defs.h"

bool ERdrBltFast32_tn_op_ad(BLTFAST_PARAMS)
{
	EINT32 xctr;
	unsigned EINT32 sc;
	unsigned EINT32 *pdd = (unsigned EINT32 *)dst;
	unsigned EINT32 *psd = (unsigned EINT32 *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

