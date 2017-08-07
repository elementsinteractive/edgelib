// exrect.h
// Optimized filter rectangle fillers
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Fill rects                                                    //
///////////////////////////////////////////////////////////////////

//Fast opacity fill: mix with destination pixels (optimized 50%)
void ExRctOpacPlus(RECT *rc, unsigned char *dst, unsigned long dstwidth, unsigned char bitwidth, unsigned char videomode, COLORREF col, unsigned long value)
{
	long xctr, yctr = rc->top;
	unsigned short w = (unsigned short)(rc->right - rc->left);
	unsigned char opshl = 0;
	value = 256 - value;
	if (value == 128)                     opshl = 1;
	else if (value == 64 || value == 192) opshl = 2;
	else if (value == 32 || value == 224) opshl = 3;
	else if (value == 16 || value == 240) opshl = 4;
	if (bitwidth == 8)
	{
		unsigned char *pdd8 = &dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				pdd8[xctr] = (unsigned char)col;
			pdd8 += dstwidth;
			yctr++;
		}
	}
	else if (bitwidth == 16)
	{
		unsigned long opand = 0, mdp = (rc->top * dstwidth + rc->left), opdbland, wend, wdbl;
		unsigned short *pdd16 = (unsigned short *)&dst[mdp * 2];
		unsigned EINT32 *pdd32;
		if (videomode == EDSP_TRUE12)
		{
			if (opshl == 1)      opand = 1911;
			else if (opshl == 2) opand = 819;
			else if (opshl == 3) opand = 273;
			else if (opshl == 4) opand = 0;
		}
		else if (videomode == EDSP_TRUE15)
		{
			if (opshl == 1)      opand = 15855;
			else if (opshl == 2) opand = 7399;
			else if (opshl == 3) opand = 3171;
			else if (opshl == 4) opand = 1057;
		}
		else if (videomode == EDSP_TRUE16)
		{
			if (opshl == 1)      opand = 31727;
			else if (opshl == 2) opand = 14823;
			else if (opshl == 3) opand = 6371;
			else if (opshl == 4) opand = 2145;
		}
		opdbland = opand | (opand << 16);
		if (value > 128)
		{
			while(yctr < rc->bottom)
			{
				pdd32 = (unsigned EINT32 *)&dst[(mdp + mdp % 2) * 2];
				wend = (mdp + w) % 2;
				wdbl = (w - wend) / 2;
				if (mdp % 2 == 1)
					pdd16[0] = (unsigned short)EBCODE_MACRO_HPACITY_O(pdd16[0], col, opshl, opand);
				for (xctr = 0; xctr < (long)wdbl; xctr++)
					pdd32[xctr] = EBCODE_MACRO_HPACITY_O(pdd32[xctr], (col | (col << 16)), opshl, opdbland);
				if (wend == 1)
					pdd16[w - 1] = (unsigned short)EBCODE_MACRO_HPACITY_O(pdd16[w - 1], col, opshl, opand);
				mdp += dstwidth; pdd16 += dstwidth; yctr++;
			}
		}
		else
		{
			while(yctr < rc->bottom)
			{
				pdd32 = (unsigned EINT32 *)&dst[(mdp + mdp % 2) * 2];
				wend = (mdp + w) % 2;
				wdbl = (w - wend) / 2;
				if (mdp % 2 == 1)
					pdd16[0] = (unsigned short)EBCODE_MACRO_HPACITY_O2(pdd16[0], col, opshl, opand);
				for (xctr = 0; xctr < (long)wdbl; xctr++)
					pdd32[xctr] = EBCODE_MACRO_HPACITY_O2(pdd32[xctr], (col | (col << 16)), opshl, opdbland);
				if (wend == 1)
					pdd16[w - 1] = (unsigned short)EBCODE_MACRO_HPACITY_O2(pdd16[w - 1], col, opshl, opand);
				mdp += dstwidth; pdd16 += dstwidth; yctr++;
			}
		}
	}
	else if (bitwidth == 24)
	{
		unsigned long tc, opand = 0;
		unsigned char *pdd8 = &dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		if (opshl == 1)      opand = 0x7F7F7F;
		else if (opshl == 2) opand = 0x3F3F3F;
		else if (opshl == 3) opand = 0x1F1F1F;
		else if (opshl == 4) opand = 0x0F0F0F;
		if (value > 128)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					tc = (unsigned long)(pdd8[xctr * 3] | (pdd8[xctr * 3 + 1] << 8) | (pdd8[xctr * 3 + 2] << 16));
					tc = EBCODE_MACRO_HPACITY_O(tc, col, opshl, opand);
					pdd8[xctr * 3]     = (unsigned char)(tc & 0xFF);
					pdd8[xctr * 3 + 1] = (unsigned char)((tc & 0xFF00) >> 8);
					pdd8[xctr * 3 + 2] = (unsigned char)((tc & 0xFF0000) >> 16);
				}
				pdd8 += dstwidth * 3; yctr++;
			}
		}
		else
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
				{
					tc = (unsigned long)(pdd8[xctr * 3] | (pdd8[xctr * 3 + 1] << 8) | (pdd8[xctr * 3 + 2] << 16));
					tc = EBCODE_MACRO_HPACITY_O2(tc, col, opshl, opand);
					pdd8[xctr * 3]     = (unsigned char)(tc & 0xFF);
					pdd8[xctr * 3 + 1] = (unsigned char)((tc & 0xFF00) >> 8);
					pdd8[xctr * 3 + 2] = (unsigned char)((tc & 0xFF0000) >> 16);
				}
				pdd8 += dstwidth * 3; yctr++;
			}
		}
	}
	else if (bitwidth == 32)
	{
		unsigned long opand = 0;
		unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		if (opshl == 1)      opand = 0x7F7F7F;
		else if (opshl == 2) opand = 0x3F3F3F;
		else if (opshl == 3) opand = 0x1F1F1F;
		else if (opshl == 4) opand = 0x0F0F0F;
		if (value > 128)
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
					pdd32[xctr] = EBCODE_MACRO_HPACITY_O(pdd32[xctr], col, opshl, opand);
				pdd32 += dstwidth; yctr++;
			}
		}
		else
		{
			while(yctr < rc->bottom)
			{
				for (xctr = 0; xctr < w; xctr++)
					pdd32[xctr] = EBCODE_MACRO_HPACITY_O2(pdd32[xctr], col, opshl, opand);
				pdd32 += dstwidth; yctr++;
			}
		}
	}
}

//Fast opacity fill: mix with destination pixels
void ExRctOpac(RECT *rc, unsigned char *dst, unsigned long dstwidth, unsigned char bitwidth, unsigned char videomode, COLORREF col, unsigned long value)
{
	if (value == 16 || value == 32 || value == 64 || value == 128 || value == 192 || value == 224 || value == 240){	ExRctOpacPlus(rc, dst, dstwidth, bitwidth, videomode, col, value); return; }
	long xctr, yctr = rc->top, w = rc->right - rc->left;
	value = 256 - value;
	if (bitwidth == 8)
	{
		unsigned char *pdd8 = &dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				pdd8[xctr] = (unsigned char)col;
			pdd8 += dstwidth;
			yctr++;
		}
	}
	else if (bitwidth == 16)
	{
		unsigned long mdp = (rc->top * dstwidth + rc->left), wend, wdbl;
		unsigned short *pdd16 = (unsigned short *)&dst[mdp * 2];
		unsigned EINT32 *pdd32;
		if (videomode == EDSP_TRUE12)
			value >>= 4;
		else
			value >>= 3;
		while(yctr < rc->bottom)
		{
			pdd32 = (unsigned EINT32 *)&dst[(mdp + mdp % 2) * 2];
			wend = (mdp + w) % 2;
			wdbl = (w - wend) / 2;
			if (videomode == EDSP_TRUE12)
			{
				if (mdp % 2 == 1)
					pdd16[0] = (unsigned short)EBCODE_MACRO_HPACITY(pdd16[0], col, 3855, 240, 4, value);
				for (xctr = 0; xctr < (long)wdbl; xctr++)
					pdd32[xctr] = EBCODE_MACRO_HPACITY_DBL(pdd32[xctr], (col | (col << 16)), 0x0F0F00F0, 0x00F00F0F, 4, value);
				if (wend == 1)
					pdd16[w - 1] = (unsigned short)EBCODE_MACRO_HPACITY(pdd16[w - 1], col, 3855, 240, 4, value);
			}
			else if (videomode == EDSP_TRUE15)
			{
				if (mdp % 2 == 1)
					pdd16[0] = (unsigned short)EBCODE_MACRO_HPACITY(pdd16[0], col, 31775, 992, 5, value);
				for (xctr = 0; xctr < (long)wdbl; xctr++)
					pdd32[xctr] = EBCODE_MACRO_HPACITY_DBL(pdd32[xctr], (col | (col << 16)), 0x7C1F03E0, 0x03E07C1F, 5, value);
				if (wend == 1)
					pdd16[w - 1] = (unsigned short)EBCODE_MACRO_HPACITY(pdd16[w - 1], col, 31775, 992, 5, value);
			}
			else
			{
				if (mdp % 2 == 1)
					pdd16[0] = (unsigned short)EBCODE_MACRO_HPACITY(pdd16[0], col, 63519, 2016, 5, value);
				for (xctr = 0; xctr < (long)wdbl; xctr++)
					pdd32[xctr] = EBCODE_MACRO_HPACITY_DBL(pdd32[xctr], (col | (col << 16)), 0xF81F07E0, 0x07E0F81F, 5, value);
				if (wend == 1)
					pdd16[w - 1] = (unsigned short)EBCODE_MACRO_HPACITY(pdd16[w - 1], col, 63519, 2016, 5, value);
			}
			mdp += dstwidth; pdd16 += dstwidth; yctr++;
		}
	}
	else if (bitwidth == 24)
	{
		unsigned long tc;
		unsigned char *pdd8 = &dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				tc = (unsigned long)(pdd8[xctr * 3] | (pdd8[xctr * 3 + 1] << 8) | (pdd8[xctr * 3 + 2] << 16));
				tc = EBCODE_MACRO_HPACITY(tc, col, 0xFF00FF, 0xFF00, 8, value);
				pdd8[xctr * 3]     = (unsigned char)(tc & 0xFF);
				pdd8[xctr * 3 + 1] = (unsigned char)((tc & 0xFF00) >> 8);
				pdd8[xctr * 3 + 2] = (unsigned char)((tc & 0xFF0000) >> 16);
			}
			pdd8 += dstwidth * 3;
			yctr++;
		}
	}
	else if (bitwidth == 32)
	{
		unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				pdd32[xctr] = EBCODE_MACRO_HPACITY(pdd32[xctr], col, 0xFF00FF, 0xFF00, 8, value);
			pdd32 += dstwidth;
			yctr++;
		}
	}
}

//Fast opacity fill: add pixels to destination
void ExRctAlpha(RECT *rc, unsigned char *dst, unsigned long dstwidth, unsigned char bitwidth, unsigned char videomode, COLORREF col)
{
	long xctr, yctr = rc->top, w = rc->right - rc->left;
	if (bitwidth == 8)
	{
		unsigned char *pdd8 = &dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				pdd8[xctr] = (unsigned char)col;
			pdd8 += dstwidth;
			yctr++;
		}
	}
	else if (bitwidth == 16)
	{
		unsigned short *pdd16 = (unsigned short *)&dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			if (videomode == EDSP_TRUE12)
			{
				for (xctr = 0; xctr < w; xctr++)
					pdd16[xctr] = (unsigned short)EBCODE_MACRO_ADDALPHA(pdd16[xctr], col, 15, 240, 3840, 16, 256, 4096);
			}
			else if (videomode == EDSP_TRUE15)
			{
				for (xctr = 0; xctr < w; xctr++)
					pdd16[xctr] = (unsigned short)EBCODE_MACRO_ADDALPHA(pdd16[xctr], col, 31, 992, 31744, 32, 1024, 32768);
			}
			else
			{
				for (xctr = 0; xctr < w; xctr++)
					pdd16[xctr] = (unsigned short)EBCODE_MACRO_ADDALPHA(pdd16[xctr], col, 31, 2016, 63488, 32, 2048, 65536);
			}
			pdd16 += dstwidth;
			yctr++;
		}
	}
	else if (bitwidth == 24)
	{
		unsigned long tc;
		unsigned char *pdd8 = &dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				tc = (unsigned long)(pdd8[xctr * 3] | (pdd8[xctr * 3 + 1] << 8) | (pdd8[xctr * 3 + 2] << 16));
				tc = EBCODE_MACRO_ADDALPHA(tc, col, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
				pdd8[xctr * 3]     = (unsigned char)(tc & 0xFF);
				pdd8[xctr * 3 + 1] = (unsigned char)((tc & 0xFF00) >> 8);
				pdd8[xctr * 3 + 2] = (unsigned char)((tc & 0xFF0000) >> 16);
			}
			pdd8 += dstwidth * 3;
			yctr++;
		}
	}
	else if (bitwidth == 32)
	{
		unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				pdd32[xctr] = EBCODE_MACRO_ADDALPHA(pdd32[xctr], col, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
			pdd32 += dstwidth;
			yctr++;
		}
	}
}

//Fast opacity fill: subtract pixels from destination
void ExRctSub(RECT *rc, unsigned char *dst, unsigned long dstwidth, unsigned char bitwidth, unsigned char videomode, COLORREF col)
{
	long xctr, yctr = rc->top, w = rc->right - rc->left;
	if (bitwidth == 8)
	{
		unsigned char *pdd8 = &dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				pdd8[xctr] = (unsigned char)col;
			pdd8 += dstwidth;
			yctr++;
		}
	}
	else if (bitwidth == 16)
	{
		unsigned short *pdd16 = (unsigned short *)&dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			if (videomode == EDSP_TRUE12)
			{
				for (xctr = 0; xctr < w; xctr++)
					pdd16[xctr] = (unsigned short)EBCODE_MACRO_SUBALPHA(pdd16[xctr], col, 15, 240, 3840);
			}
			else if (videomode == EDSP_TRUE15)
			{
				for (xctr = 0; xctr < w; xctr++)
					pdd16[xctr] = (unsigned short)EBCODE_MACRO_SUBALPHA(pdd16[xctr], col, 31, 992, 31744);
			}
			else
			{
				for (xctr = 0; xctr < w; xctr++)
					pdd16[xctr] = (unsigned short)EBCODE_MACRO_SUBALPHA(pdd16[xctr], col, 31, 2016, 63488);
			}
			pdd16 += dstwidth;
			yctr++;
		}
	}
	else if (bitwidth == 24)
	{
		unsigned long tc;
		unsigned char *pdd8 = &dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				tc = (unsigned long)(pdd8[xctr * 3] | (pdd8[xctr * 3 + 1] << 8) | (pdd8[xctr * 3 + 2] << 16));
				tc = EBCODE_MACRO_SUBALPHA(tc, col, 0xFF, 0xFF00, 0xFF0000);
				pdd8[xctr * 3]     = (unsigned char)(tc & 0xFF);
				pdd8[xctr * 3 + 1] = (unsigned char)((tc & 0xFF00) >> 8);
				pdd8[xctr * 3 + 2] = (unsigned char)((tc & 0xFF0000) >> 16);
			}
			pdd8 += dstwidth * 3;
			yctr++;
		}
	}
	else if (bitwidth == 32)
	{
		unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[(rc->top * dstwidth + rc->left) * (bitwidth >> 3)];
		while(yctr < rc->bottom)
		{
			for (xctr = 0; xctr < w; xctr++)
				pdd32[xctr] = EBCODE_MACRO_SUBALPHA(pdd32[xctr], col, 0xFF, 0xFF00, 0xFF0000);
			pdd32 += dstwidth;
			yctr++;
		}
	}
}

