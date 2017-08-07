// excoll.h
// Collision hit testing
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Intersect testing (also with bit masks for fast 32-pixel at once testing)
// fixme: bit-aware prototype
bool ExMiscIntersect(long x1, long y1, long x2, long y2, RECT *nrc, unsigned long *bitmask1, unsigned long *bitmask2, unsigned long swidth1, unsigned long swidth2, POINT *pnt)
{
	unsigned long w, h;
	if (x1 + (nrc[0].right - nrc[0].left) <= x2 || x2 + (nrc[1].right - nrc[1].left) <= x1 || y1 + (nrc[0].bottom - nrc[0].top) <= y2 || y2 + (nrc[1].bottom - nrc[1].top) <= y1)
		return(false);
	w = nrc[0].right - nrc[0].left;
	h = nrc[0].bottom - nrc[0].top;
	if (x1 < x2)
	{
		nrc[0].left += x2 - x1;
		w -= x2 - x1;
	}
	else if (x2 < x1)
		nrc[1].left += x1 - x2;
	if (y1 < y2)
	{
		nrc[0].top += y2 - y1;
		h -= y2 - y1;
	}
	else if (y2 < y1)
		nrc[1].top += y1 - y2;
	if ((long)w > nrc[1].right - nrc[1].left)
		w = nrc[1].right - nrc[1].left;
	if ((long)h > nrc[1].bottom - nrc[1].top)
		h = nrc[1].bottom - nrc[1].top;
	if (bitmask1 != NULL && bitmask2 != NULL)
	{
		unsigned long *bitsheet1, *bitsheet2;
		unsigned long cmp1, cmp2, sw1, sw2;
		sw1 = (swidth1 + 31) / 32;
		sw2 = (swidth2 + 31) / 32;
		bitsheet1 = &bitmask1[nrc[0].top * sw1 + nrc[0].left / 32];
		bitsheet2 = &bitmask2[nrc[1].top * sw2 + nrc[1].left / 32];
		if (pnt != NULL)
		{
			pnt->x = x1 + nrc[0].left;
			pnt->y = y1 + nrc[0].top;
		}
		x1 = nrc[0].left & 31;
		x2 = nrc[1].left & 31;
		if (w > 32)
		{
			long todo, xctr;
			while(h > 0)
			{
				xctr = 0;
				todo = w;
				while(todo > 0)
				{
					cmp1 = (*bitsheet1 << x1) | (bitsheet1[1] >> (31 - x1) >> 1);
					cmp2 = (*bitsheet2 << x2) | (bitsheet2[1] >> (31 - x2) >> 1);
					if (todo < 32)
					{
						if ((cmp1 & cmp2) >> (32 - todo))
						{
							if (pnt != NULL)
							{
								unsigned long pmask = 0x80000000;
								pnt->x += xctr * 32;
								while (pmask > 0)
								{
									if ((cmp1 & cmp2) & pmask)
										return(true);
									else
										pnt->x++;
									pmask >>= 1;
								}
							}
							return(true);
						}
					}
					else if (cmp1 & cmp2)
					{
						if (pnt != NULL)
						{
							unsigned long pmask = 0x80000000;
							pnt->x += xctr * 32;
							while (pmask > 0)
							{
								if ((cmp1 & cmp2) & pmask)
									return(true);
								else
									pnt->x++;
								pmask >>= 1;
							}
						}
						return(true);
					}
					todo -= 32;
					xctr++;
					bitsheet1++;
					bitsheet2++;
				}
				bitsheet1 += sw1 - xctr;
				bitsheet2 += sw2 - xctr;
				if (pnt != NULL)
					pnt->y++;
				h--;
			}
		}
		else
		{
			w = 32 - w;
			while(h > 0)
			{
				cmp1 = (*bitsheet1 << x1) | (bitsheet1[1] >> (31 - x1) >> 1);
				cmp2 = (*bitsheet2 << x2) | (bitsheet2[1] >> (31 - x2) >> 1);
				if ((cmp1 & cmp2) >> w)
				{
					if (pnt != NULL)
					{
						unsigned long pmask = 0x80000000;
						while (pmask > 0)
						{
							if ((cmp1 & cmp2) & pmask)
								return(true);
							else
								pnt->x++;
							pmask >>= 1;
						}
					}
					return(true);
				}
				bitsheet1 += sw1;
				bitsheet2 += sw2;
				if (pnt != NULL)
					pnt->y++;
				h--;
			}
		}
		if (pnt != NULL)
		{
			pnt->x = -1;
			pnt->y = -1;
		}
		return(false);
	}
	if (pnt != NULL)
	{
		pnt->x = x1 + nrc[0].left;
		pnt->y = y1 + nrc[0].top;
	}
	return(true);
}
