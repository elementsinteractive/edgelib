#include "defs.h"

bool ERdrBltFast8_ck(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cf(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_tn(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_cl(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cl(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cf_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_tn_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_cl_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cl_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cf_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_tn_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_cl_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_cl_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cf_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_tn_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_cl_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_cl_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_cf_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_tn_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_cl_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cl_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_cf_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_tn_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_cl_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cl_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cf_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_tn_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_cl_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cl_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cf_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_tn_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_cl_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_cl_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_cf_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_tn_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_cl_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cl_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cf_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_tn_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_cl_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cl_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cf_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_tn_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_cl_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_cl_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_ck_cf_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_tn_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_tn_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast8_cl_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd++;
		}
		pdd += dstpitch - w; psd += srcpitch - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast8_ck_cl_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFast12_ck(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 4;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cf_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_tn_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_tn_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_cl_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast12_ck_cl_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0xF81F07E0, 0x07E0F81F, 5, (32 - opacity), opacity);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		opacity >>= 3;
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0xF81F07E0, 0x07E0F81F, 5, opacity);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800, 0x20, 0x800, 0x10000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cf_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = colorize;
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_tn_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_tn_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						sc = EBCODE_MACRO_TINT(sc, 0x7BEF7BEF, colorize);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_cl_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						*pdd32 = sc;
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast16_ck_cl_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	unsigned long *pdd32, *psd32;
	if ((long)(pdd) & 2)
		pdd32 = (unsigned long *)&pdd[1];
	else
		pdd32 = (unsigned long *)pdd;
	if ((long)(psd) & 2)
		psd32 = (unsigned long *)&psd[1];
	else
		psd32 = (unsigned long *)psd;
	long yctr;
	unsigned long sc;
	unsigned short *orgpdd, *orgpsd;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd + 1; psd = orgpsd + 1; w--;
		}
		if (((long)(psd) + w * 2) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			pdd += w - 1; psd += w - 1;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				if (sc != colorkey)
				{
						EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					*pdd = (unsigned short)sc;
				}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	unsigned long sc_mod;
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
						{
							*pdd++ = (unsigned short)sc;
							*pdd++ = (unsigned short)(sc >> 16);
						}
						else if (sc_org & 0xFFFF)
						{
							*pdd++ = (unsigned short)sc;
							pdd++;
						}
						else
						{
							pdd++;
							*pdd++ = (unsigned short)(sc >> 16);
						}
				}
				else
					pdd += 2;
				psd32++;
			}
			pdd += dstpitch / 2 - w * 2; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	else
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				if ((sc_org = sc ^ colorkey))
				{
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0x1F, 0x7E0, 0xF800, 0, 6, 11, 5, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0x1F, 0x7E0, 0xF800);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0x1F, 0x7E0, 0xF800);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
				}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}

bool ERdrBltFast32_ck(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_tn(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cl(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_tn_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cl_op(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_tn_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cl_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
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

bool ERdrBltFast32_ck_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
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

bool ERdrBltFast32_ck_tn_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
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

bool ERdrBltFast32_ck_cl_op_ad(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_tn_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cl_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
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
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_tn_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cl_op_sb(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_tn_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cl_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_tn_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cl_ad_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_tn_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cl_sb_ol(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_tn_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cl_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_tn_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cl_ad_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cf_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_tn_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_tn_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_cl_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFast32_ck_cl_sb_oh(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_tn(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_tn(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cl(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_tn_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_tn_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cl_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_tn_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_tn_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_cl_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_tn_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_tn_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_cl_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_tn_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_tn_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cl_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_tn_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_tn_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cl_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_tn_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_tn_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cl_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_tn_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_tn_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_cl_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_tn_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_tn_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cl_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_tn_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_tn_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cl_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_tn_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_tn_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_ck_cl_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				*pdd = (unsigned char)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cf_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip8_tn_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_tn_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_cl_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned char *pdd = dst;
	unsigned char *psd = src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				*pdd = (unsigned char)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip8_ck_cl_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cf(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_tn(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_cl(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 4;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cf_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					sc = colorize;
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_tn_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 4;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_cl_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 4;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
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

bool ERdrBltFlip12_ck_cf_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
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

bool ERdrBltFlip12_tn_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
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

bool ERdrBltFlip12_cl_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
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

bool ERdrBltFlip12_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 4;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip12_ck_cf_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					sc = colorize;
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

bool ERdrBltFlip12_tn_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 4;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
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

bool ERdrBltFlip12_cl_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 4;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip12_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cf_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_tn_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_cl_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 4;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
						sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cf_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_tn_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 4;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_cl_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 4;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cf_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_tn_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_cl_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
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

bool ERdrBltFlip12_ck_cf_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
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

bool ERdrBltFlip12_tn_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
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

bool ERdrBltFlip12_cl_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
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

bool ERdrBltFlip12_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cf_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_tn_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_cl_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cf_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_tn_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_cl_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
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

bool ERdrBltFlip12_ck_cf_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
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

bool ERdrBltFlip12_tn_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
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

bool ERdrBltFlip12_cl_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
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

bool ERdrBltFlip12_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cf_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_tn_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_tn_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_cl_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip12_ck_cl_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cf(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_tn(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_cl(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 3;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip16_ck_cf_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					sc = colorize;
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

bool ERdrBltFlip16_tn_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 3;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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

bool ERdrBltFlip16_cl_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 3;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
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

bool ERdrBltFlip16_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cf_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_tn_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_cl_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 3;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cf_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_tn_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 3;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_cl_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 3;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cf_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_tn_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_cl_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 3;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cf_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_tn_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 3;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_cl_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		opacity >>= 3;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
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
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cf_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_tn_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_cl_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cf_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_tn_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_cl_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cf_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_tn_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_cl_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cf_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_tn_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_cl_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cf_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_tn_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_cl_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cf_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_tn_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_tn_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_cl_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip16_ck_cl_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 31727;
		else if (opacity == 192) opand = 14823;
		else if (opacity == 224) opand = 6371;
		else if (opacity == 240) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
				*pdd = (unsigned short)sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cf(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_tn(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_tn(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_cl(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cf_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_tn_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
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

bool ERdrBltFlip32_ck_tn_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_cl_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl_op(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cf_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_tn_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_tn_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_cl_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cf_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_tn_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_tn_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_cl_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl_op_ad(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
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

bool ERdrBltFlip32_ck_cf_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
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

bool ERdrBltFlip32_tn_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_tn_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
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

bool ERdrBltFlip32_cl_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
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

bool ERdrBltFlip32_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
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

bool ERdrBltFlip32_ck_cf_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
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

bool ERdrBltFlip32_tn_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_tn_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
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

bool ERdrBltFlip32_cl_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl_op_sb(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
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

bool ERdrBltFlip32_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cf_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_tn_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_tn_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_cl_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cf_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_tn_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_tn_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_cl_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl_ad_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
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

bool ERdrBltFlip32_ck_cf_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
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

bool ERdrBltFlip32_tn_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_tn_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
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

bool ERdrBltFlip32_cl_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl_sb_ol(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
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

bool ERdrBltFlip32_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cf_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_tn_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_tn_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_cl_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cf_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_tn_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_tn_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_cl_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl_ad_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				*pdd = sc;
			}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
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

bool ERdrBltFlip32_ck_cf_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = colorize;
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
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

bool ERdrBltFlip32_tn_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_tn_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
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

bool ERdrBltFlip32_cl_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
				*pdd = sc;
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 4 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}

bool ERdrBltFlip32_ck_cl_sb_oh(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		else if (opacity == 240) opshl = 4;
		if (opacity == 128)      opand = 0x7F7F7F;
		else if (opacity == 192) opand = 0x3F3F3F;
		else if (opacity == 224) opand = 0x1F1F1F;
		else if (opacity == 240) opand = 0x0F0F0F;
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			if (sc != colorkey)
			{
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
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

int edgerender_getlibid(void)
{
	return(1000);
}




