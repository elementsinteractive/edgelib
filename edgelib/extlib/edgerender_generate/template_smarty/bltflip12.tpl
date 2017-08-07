{literal}
bool ERdrBltFlip12{/literal}{$funcext}{literal}(BLTFLIP_PARAMS)
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
			pdd++; psd += xadd;
		}
		pdd += dstpitch / 2 - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}
{/literal}
