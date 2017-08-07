#include "defs.h"

bool ERdrBltFlip8_tn_ad_oh(BLTFLIP_PARAMS)
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
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

