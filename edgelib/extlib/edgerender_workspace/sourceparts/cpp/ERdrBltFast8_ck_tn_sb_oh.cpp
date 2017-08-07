#include "defs.h"

bool ERdrBltFast8_ck_tn_sb_oh(BLTFAST_PARAMS)
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
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

