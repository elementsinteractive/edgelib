{literal}
bool ERdrBltFast12{/literal}{$funcext}{literal}(BLTFAST_PARAMS)
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
	{/literal}{if $oh}{literal}
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 128)      opshl = 1;
		else if (opacity == 192) opshl = 2;
		else if (opacity == 224) opshl = 3;
		if (opacity == 128)      opand = 1911;
		else if (opacity == 192) opand = 819;
		else if (opacity == 224) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	{/literal}{elseif $ol}{literal}
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		if (opacity == 64)      opand = 819;
		else if (opacity == 32) opand = 273;
		unsigned long opdbland = opand | (opand << 16);
	{/literal}{/if}{literal}
	{/literal}{if $op}{literal}
		opacity >>= 4;
	{/literal}{/if}{literal}
	if (w)
	{
		if ((long)(psd) & 2)
		{
			orgpdd = pdd, orgpsd = psd;
			for (yctr = 0; yctr < h; yctr++)
			{
				sc = *psd;
				{/literal}{if $ck}{literal}
				if (sc != colorkey)
				{
				{/literal}{/if}{literal}
					{/literal}{if $cl}{literal}
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					{/literal}{elseif $cf}{literal}
						sc = colorize;
					{/literal}{elseif $tn}{literal}
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					{/literal}{/if}{literal}
					{/literal}{if $sb || $ad}{literal}
						{/literal}{if $op}{literal}
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						{/literal}{elseif $oh}{literal}
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						{/literal}{elseif $ol}{literal}
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						{/literal}{/if}{literal}
						{/literal}{if $ad}{literal}
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
						{/literal}{elseif $sb}{literal}
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
						{/literal}{/if}{literal}
					{/literal}{elseif $op}{literal}
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					{/literal}{elseif $oh}{literal}
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					{/literal}{elseif $ol}{literal}
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					{/literal}{/if}{literal}
					*pdd = (unsigned short)sc;
				{/literal}{if $ck}{literal}
				}
				{/literal}{/if}{literal}
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
				{/literal}{if $ck}{literal}
				if (sc != colorkey)
				{
				{/literal}{/if}{literal}
					{/literal}{if $cl}{literal}
						EBCODE_MACRO_COLORIZE(sc, sc, 15, 240, 3840, 0, 4, 8, 4, colorize, unsigned short)
					{/literal}{elseif $cf}{literal}
						sc = colorize;
					{/literal}{elseif $tn}{literal}
						sc = EBCODE_MACRO_TINT(sc, 1911, colorize);
					{/literal}{/if}{literal}
					{/literal}{if $sb || $ad}{literal}
						{/literal}{if $op}{literal}
							sc = EBCODE_MACRO_HPACITY(sc, 0, 3855, 240, 4, opacity);
						{/literal}{elseif $oh}{literal}
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
						{/literal}{elseif $ol}{literal}
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
						{/literal}{/if}{literal}
						{/literal}{if $ad}{literal}
							sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 15, 240, 3840, 16, 256, 4096);
						{/literal}{elseif $sb}{literal}
							sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 15, 240, 3840);
						{/literal}{/if}{literal}
					{/literal}{elseif $op}{literal}
						sc = EBCODE_MACRO_OPACITY(*pdd, sc, 3855, 240, 4, (16 - opacity), opacity);
					{/literal}{elseif $oh}{literal}
						sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
					{/literal}{elseif $ol}{literal}
						sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
					{/literal}{/if}{literal}
					*pdd = (unsigned short)sc;
				{/literal}{if $ck}{literal}
				}
				{/literal}{/if}{literal}
				pdd += dstpitch / 2; psd += srcpitch / 2;
			}
			pdd = orgpdd; psd = orgpsd; w--;
		}
		w /= 2;
	}
	{/literal}{if $ck}{literal}
	unsigned long sc_org;
	colorkey |= (colorkey << 16);
	{/literal}{/if}{literal}
	{/literal}{if $ad || $sb || $cl}{literal}
	unsigned long sc_mod;
	{/literal}{/if}{literal}
	colorize |= (colorize << 16);
	if ((long)(pdd) & 2)
	{
		while (h)
		{
			for (xctr = 0; xctr < w; xctr++)
			{
				sc = *psd32;
				{/literal}{if $ck}{literal}
				if ((sc_org = sc ^ colorkey))
				{
				{/literal}{/if}{literal}
					{/literal}{if $cl}{literal}
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
					{/literal}{elseif $cf}{literal}
						sc = colorize;
					{/literal}{elseif $tn}{literal}
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
					{/literal}{/if}{literal}
					{/literal}{if $sb || $ad}{literal}
						{/literal}{if $op}{literal}
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
						{/literal}{elseif $oh}{literal}
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
						{/literal}{elseif $ol}{literal}
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
						{/literal}{/if}{literal}
						{/literal}{if $ad}{literal}
							sc_mod = EBCODE_MACRO_ADDALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						{/literal}{elseif $sb}{literal}
							sc_mod = EBCODE_MACRO_SUBALPHA(pdd[1], (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(pdd[0], sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						{/literal}{/if}{literal}
					{/literal}{elseif $op}{literal}
						sc = EBCODE_MACRO_OPACITY_DBL(((pdd[1] << 16) + pdd[0]), sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
					{/literal}{elseif $oh}{literal}
						sc = EBCODE_MACRO_OPACITY_O(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
					{/literal}{elseif $ol}{literal}
						sc = EBCODE_MACRO_OPACITY_O2(((pdd[1] << 16) + pdd[0]), sc, opshl, opdbland);
					{/literal}{/if}{literal}
					{/literal}{if $ck}{literal}
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
					{/literal}{else}{literal}
						*pdd++ = (unsigned short)sc;
						*pdd++ = (unsigned short)(sc >> 16);
					{/literal}{/if}{literal}
				{/literal}{if $ck}{literal}
				}
				else
					pdd += 2;
				{/literal}{/if}{literal}
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
				{/literal}{if $ck}{literal}
				if ((sc_org = sc ^ colorkey))
				{
				{/literal}{/if}{literal}
					{/literal}{if $cl}{literal}
						EBCODE_MACRO_COLORIZE(sc_mod, (sc >> 16), 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						EBCODE_MACRO_COLORIZE(sc, sc, 0xF, 0xF0, 0xF00, 0, 4, 8, 4, colorize, unsigned long)
						sc = (sc & 0xFFFF) | (sc_mod << 16);
					{/literal}{elseif $cf}{literal}
						sc = colorize;
					{/literal}{elseif $tn}{literal}
						sc = EBCODE_MACRO_TINT(sc, 0x07770777, colorize);
					{/literal}{/if}{literal}
					{/literal}{if $sb || $ad}{literal}
						{/literal}{if $op}{literal}
							sc = EBCODE_MACRO_HPACITY_DBL(sc, 0, 0x0F0F00F0, 0x00F00F0F, 4, opacity);
						{/literal}{elseif $oh}{literal}
							sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opdbland);
						{/literal}{elseif $ol}{literal}
							sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opdbland);
						{/literal}{/if}{literal}
						{/literal}{if $ad}{literal}
							sc_mod = EBCODE_MACRO_ADDALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = EBCODE_MACRO_ADDALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00, 0x10, 0x100, 0x1000);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						{/literal}{elseif $sb}{literal}
							sc_mod = EBCODE_MACRO_SUBALPHA((*pdd32 >> 16), (sc >> 16), 0xF, 0xF0, 0xF00);
							sc = EBCODE_MACRO_SUBALPHA(*pdd32, sc, 0xF, 0xF0, 0xF00);
							sc = (sc & 0xFFFF) | (sc_mod << 16);
						{/literal}{/if}{literal}
					{/literal}{elseif $op}{literal}
						sc = EBCODE_MACRO_OPACITY_DBL(*pdd32, sc, 0x0F0F00F0, 0x00F00F0F, 4, (16 - opacity), opacity);
					{/literal}{elseif $oh}{literal}
						sc = EBCODE_MACRO_OPACITY_O(*pdd32, sc, opshl, opdbland);
					{/literal}{elseif $ol}{literal}
						sc = EBCODE_MACRO_OPACITY_O2(*pdd32, sc, opshl, opdbland);
					{/literal}{/if}{literal}
					{/literal}{if $ck}{literal}
						if ((sc_org & 0xFFFF) && (sc_org & 0xFFFF0000))
							*pdd32 = sc;
						else if (sc_org & 0xFFFF)
							((unsigned short *)(pdd32))[0] = (unsigned short)(sc & 0xFFFF);
						else
							((unsigned short *)(pdd32))[1] = (unsigned short)(sc >> 16);
					{/literal}{else}{literal}
						*pdd32 = sc;
					{/literal}{/if}{literal}
				{/literal}{if $ck}{literal}
				}
				{/literal}{/if}{literal}
				pdd32++; psd32++;
			}
			pdd32 += dstpitch / 4 - w; psd32 += srcpitch / 4 - w;
			h--;
		}
	}
	return(true);
}
{/literal}
