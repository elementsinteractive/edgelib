{literal}
bool ERdrBltFlip16{/literal}{$funcext}{literal}(BLTFLIP_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned short *pdd = (unsigned short *)dst;
	unsigned short *psd = (unsigned short *)src;
	{/literal}{if $oh}{literal}
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
	{/literal}{elseif $ol}{literal}
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 14823;
		else if (opacity == 32) opand = 6371;
		else if (opacity == 16) opand = 2145;
		unsigned long opdbland = opand | (opand << 16);
	{/literal}{/if}{literal}
	{/literal}{if $op}{literal}
		opacity >>= 3;
	{/literal}{/if}{literal}
	while (h)
	{
		for (xctr = 0; xctr < w; xctr++)
		{
			sc = *psd;
			{/literal}{if $ck}{literal}
			if (sc != colorkey)
			{
			{/literal}{/if}{literal}
				{/literal}{if $cl}{literal}
					EBCODE_MACRO_COLORIZE(sc, sc, 31, 2016, 63488, 0, 6, 11, 5, colorize, unsigned short)
				{/literal}{elseif $cf}{literal}
					sc = colorize;
				{/literal}{elseif $tn}{literal}
					sc = EBCODE_MACRO_TINT(sc, 31727, colorize);
				{/literal}{/if}{literal}
				{/literal}{if $sb || $ad}{literal}
					{/literal}{if $op}{literal}
						sc = EBCODE_MACRO_HPACITY(sc, 0, 63519, 2016, 5, opacity);
					{/literal}{elseif $oh}{literal}
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
					{/literal}{elseif $ol}{literal}
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
					{/literal}{/if}{literal}
					{/literal}{if $ad}{literal}
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 31, 2016, 63488, 32, 2048, 65536);
					{/literal}{elseif $sb}{literal}
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 31, 2016, 63488);
					{/literal}{/if}{literal}
				{/literal}{elseif $op}{literal}
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 63519, 2016, 5, (32 - opacity), opacity);
				{/literal}{elseif $oh}{literal}
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				{/literal}{elseif $ol}{literal}
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				{/literal}{/if}{literal}
				*pdd = (unsigned short)sc;
			{/literal}{if $ck}{literal}
			}
			{/literal}{/if}{literal}
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}
{/literal}
