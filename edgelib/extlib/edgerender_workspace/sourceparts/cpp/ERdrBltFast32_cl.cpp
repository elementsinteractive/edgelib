#include "defs.h"

bool ERdrBltFast32_cl(BLTFAST_PARAMS)
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
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned EINT32)
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

