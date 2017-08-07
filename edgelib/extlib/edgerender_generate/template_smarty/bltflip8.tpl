{literal}
bool ERdrBltFlip8{/literal}{$funcext}{literal}(BLTFLIP_PARAMS)
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
			{/literal}{if $ck}{literal}
			if (sc != colorkey)
			{
			{/literal}{/if}{literal}
				{/literal}{if $cf}{literal}
					sc = colorize;
				{/literal}{/if}{literal}
				{/literal}{if $ad}{literal}
					if (sc + *pdd > 255)
						sc = 255;
					else
						sc += *pdd;
				{/literal}{elseif $sb}{literal}
					if (sc > *pdd)
						sc = 0;
					else
						sc = *pdd - sc;
				{/literal}{/if}{literal}
				*pdd = (unsigned char)sc;
			{/literal}{if $ck}{literal}
			}
			{/literal}{/if}{literal}
			pdd++; psd += xadd;
		}
		pdd += dstpitch - w; psd += yadd - w * xadd;
		h--;
	}
	return(true);
}
{/literal}
