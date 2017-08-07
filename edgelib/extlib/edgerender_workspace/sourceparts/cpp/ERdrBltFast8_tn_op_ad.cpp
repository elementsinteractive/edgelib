#include "defs.h"

bool ERdrBltFast8_tn_op_ad(BLTFAST_PARAMS)
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
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

