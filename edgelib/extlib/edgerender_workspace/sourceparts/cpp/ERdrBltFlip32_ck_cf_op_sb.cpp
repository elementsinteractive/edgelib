#include "defs.h"

bool ERdrBltFlip32_ck_cf_op_sb(BLTFLIP_PARAMS)
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
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

