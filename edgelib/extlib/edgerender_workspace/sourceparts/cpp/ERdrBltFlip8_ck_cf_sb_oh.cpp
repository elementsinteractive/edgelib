#include "defs.h"

bool ERdrBltFlip8_ck_cf_sb_oh(BLTFLIP_PARAMS)
{
	EINT32 xctr;
	unsigned EINT32 sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

