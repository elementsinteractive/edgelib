#include "defs.h"

bool ERdrBltFlip32_tn_op(BLTFLIP_PARAMS)
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
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

