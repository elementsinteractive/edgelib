{literal}
bool ERdrBltFast32{/literal}{$funcext}{literal}(BLTFAST_PARAMS)
{
	long xctr;
	unsigned long sc;
	unsigned long *pdd = (unsigned long *)dst;
	unsigned long *psd = (unsigned long *)src;
	{/literal}{if $oh}{literal}
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
	{/literal}{elseif $ol}{literal}
		unsigned char opshl = 0;
		unsigned long opand = 0;
		if (opacity == 64)      opshl = 2;
		else if (opacity == 32) opshl = 3;
		else if (opacity == 16) opshl = 4;
		if (opacity == 64)      opand = 0x3F3F3F;
		else if (opacity == 32) opand = 0x1F1F1F;
		else if (opacity == 16) opand = 0x0F0F0F;
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
					EBCODE_MACRO_COLORIZE(sc, sc, 0xFF, 0xFF00, 0xFF0000, 0, 8, 16, 8, colorize, unsigned long)
				{/literal}{elseif $cf}{literal}
					sc = colorize;
				{/literal}{elseif $tn}{literal}
					sc = EBCODE_MACRO_TINT(sc, 0x7F7F7F, colorize);
				{/literal}{/if}{literal}
				{/literal}{if $sb || $ad}{literal}
					{/literal}{if $op}{literal}
						sc = EBCODE_MACRO_HPACITY(sc, 0, 0xFF00FF, 0xFF00, 8, opacity);
					{/literal}{elseif $oh}{literal}
						sc = EBCODE_MACRO_HPACITY_O(sc, 0, opshl, opand);
					{/literal}{elseif $ol}{literal}
						sc = EBCODE_MACRO_HPACITY_O2(sc, 0, opshl, opand);
					{/literal}{/if}{literal}
					{/literal}{if $ad}{literal}
						sc = EBCODE_MACRO_ADDALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000, 0xF00, 0xF0000, 0xF000000);
					{/literal}{elseif $sb}{literal}
						sc = EBCODE_MACRO_SUBALPHA(*pdd, sc, 0xFF, 0xFF00, 0xFF0000);
					{/literal}{/if}{literal}
				{/literal}{elseif $op}{literal}
					sc = EBCODE_MACRO_OPACITY(*pdd, sc, 0xFF00FF, 0xFF00, 8, (256 - opacity), opacity);
				{/literal}{elseif $oh}{literal}
					sc = EBCODE_MACRO_OPACITY_O(*pdd, sc, opshl, opand);
				{/literal}{elseif $ol}{literal}
					sc = EBCODE_MACRO_OPACITY_O2(*pdd, sc, opshl, opand);
				{/literal}{/if}{literal}
				*pdd = sc;
			{/literal}{if $ck}{literal}
			}
			{/literal}{/if}{literal}
			pdd++; psd++;
		}
		pdd += dstpitch / 4 - w; psd += srcpitch / 4 - w;
		h--;
	}
	return(true);
}
{/literal}
