// exfont.h
// Core functionality for font rendering
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Easy getchar function for text/unicode transparency
long inline FontDraw_GetChr(const unsigned char *text, long pos, bool useunicode)
{
	if (useunicode)
		return(text[pos * 2 + 1] << 8 | text[pos * 2]);
	return(text[pos]);
}

//Easy setchar function for text/unicode transparency
void inline FontDraw_SetChr(unsigned char *text, long pos, long chr, bool useunicode)
{
	if (useunicode)
	{
		text[pos * 2] = (unsigned char)(chr & 0xFF);
		text[pos * 2 + 1] = (unsigned char)((chr >> 8) & 0xFF);
	}
	else
		text[pos] = (unsigned char)chr;
}

//Easy strlen function for text/unicode transparency
unsigned long inline FontDraw_StrLen(const unsigned char *text, bool useunicode)
{
	if (useunicode)
		return(ClassEStd::StrLen((const WCHAR *)text));
	return(ClassEStd::StrLen((const char *)text));
}

//Get converted keycode (for optional character set)
long inline FontDraw_ConvertChr(long chr, const WCHAR *fcset, const short *fontrc, long defaultchar)
{
	long result;
	if (fcset)
	{
		result = ClassEStd::StrFind(fcset, (WCHAR)chr);
		if (result < 0 && chr >= 32)
			result = defaultchar;
	}
	else
	{
		result = chr - fontrc[2];
		if ((result < 0 || result >= fontrc[3]) && chr >= 32)
			result = defaultchar;
	}
	return(result);
}

//Calculate font character width
long FontDraw_GetCharWidth(long chridx, const short *fontrc)
{
	if (chridx >= 0 && chridx < fontrc[3])
		return(fontrc[4 + chridx * 4 + 2] - fontrc[4 + chridx * 4]);
	return(0);
}

//Calculate kerning based on character width, configured padding and optional kerning list
long FontDraw_GetKerning(long chridx, long chr, long chrnext, const short *fontrc, const short *fontkerning)
{
	long kerning = 0;
	if (chridx >= 0 && chridx < fontrc[3])
		kerning = fontrc[4 + chridx * 4 + 2] - fontrc[4 + chridx * 4] + fontrc[0];
	if (fontkerning)
	{
		long kerningidx;
		for (kerningidx = 0; kerningidx < fontkerning[1]; kerningidx++)
			if ((fontkerning[2 + kerningidx * 3] == EKERNING_WILDCARD || fontkerning[2 + kerningidx * 3] == chr) && (fontkerning[2 + kerningidx * 3 + 1] == EKERNING_WILDCARD || fontkerning[2 + kerningidx * 3 + 1] == chrnext))
				kerning += fontkerning[2 + kerningidx * 3 + 2];
	}
	return(kerning);
}

//Calculate kerning based on character width, configured padding and optional kerning list
long FontDraw_LocateSpace(const unsigned char *text, long startpos, long stoppos, bool useunicode)
{
	while(startpos >= stoppos)
	{
		if (FontDraw_GetChr(text, startpos, useunicode) == ' ')
			return(startpos);
		startpos--;
	}
	return(-1);
}

//Retrieve the width of a font text
unsigned long FontDraw_GetFontWidth(const unsigned char *text, unsigned long wrapwidth, const WCHAR *fcset, const short *fontrc, const short *fontkerning, long defaultchar, bool useunicode, bool stopnewline, long maxcount)
{
	if (fontrc == NULL)
		return(0);
	unsigned long ctr, ctr2, maxwidth = 0, srcpos = 0, len = FontDraw_StrLen(text, useunicode);
	long kchr, kchrnext, curwidth = 0, lastwidth = 0;
	for (ctr = 0; ctr < len; ctr++)
	{
		//Break out when stop position is reached
		if (maxcount >= 0 && (long)ctr >= maxcount)
			break;

		//Fetch characters
		kchr = FontDraw_GetChr(text, ctr, useunicode);
		kchrnext = FontDraw_GetChr(text, ctr + 1, useunicode);

		//Check for manual newline
		if ((kchr == '\r' && kchrnext != '\n') || kchr == '\n')
		{
			if (maxwidth < (unsigned long)lastwidth)
				maxwidth = lastwidth;
			srcpos = ctr;
			curwidth = 0;
			if (stopnewline)
				break;
		}

		//Calculate width and wrapping
		if (kchr != '\r' && kchr != '\n')
		{
			lastwidth = curwidth + FontDraw_GetCharWidth(FontDraw_ConvertChr(kchr, fcset, fontrc, defaultchar), fontrc);
			curwidth += FontDraw_GetKerning(FontDraw_ConvertChr(kchr, fcset, fontrc, defaultchar), kchr, kchrnext, fontrc, fontkerning);
			if (wrapwidth > 0)
				if (curwidth + FontDraw_GetCharWidth(FontDraw_ConvertChr(kchrnext, fcset, fontrc, defaultchar), fontrc) > (long)wrapwidth)
				{
					long kchrcheck, kchrchecknext, kchridx;
					long spacepos = FontDraw_LocateSpace(text, ctr + 1, srcpos + 1, useunicode);
					long spacepos_end = spacepos;
					if (spacepos < 0)
					{
						spacepos = ctr;
						spacepos_end = spacepos + 1;
					}
					lastwidth = 0;
					for (ctr2 = srcpos; ctr2 < (unsigned long)spacepos_end; ctr2++)
					{
						kchrcheck = FontDraw_GetChr(text, ctr2, useunicode);
						kchrchecknext = FontDraw_GetChr(text, ctr2 + 1, useunicode);
						kchridx = FontDraw_ConvertChr(kchrcheck, fcset, fontrc, defaultchar);
						if (kchridx >= 0 && kchridx < fontrc[3])
						{
							unsigned long realwidth = lastwidth + FontDraw_GetCharWidth(kchridx, fontrc);
							if (maxwidth < realwidth)
								maxwidth = realwidth;
						}
						lastwidth += FontDraw_GetKerning(kchridx, kchrcheck, kchrchecknext, fontrc, fontkerning);
					}
					srcpos = spacepos + 1;
					curwidth = 0;
					ctr = spacepos;
				}
		}
	}
	if (maxwidth < (unsigned long)lastwidth)
		maxwidth = lastwidth;
	return(maxwidth);
}

//Retrieve the height of a font text
unsigned long FontDraw_GetFontHeight(const unsigned char *text, unsigned long wrapwidth, const WCHAR *fcset, const short *fontrc, const short *fontkerning, long defaultchar, bool useunicode)
{
	if (fontrc == NULL)
		return(0);
	if (text == NULL)
		return(fontrc[7] - fontrc[5]);
	unsigned long ctr, lines = 1, srcpos = 0, len = FontDraw_StrLen(text, useunicode);
	long kchr, kchrnext, curwidth = 0;
	for (ctr = 0; ctr < len; ctr++)
	{
		kchr = FontDraw_GetChr(text, ctr, useunicode);
		kchrnext = FontDraw_GetChr(text, ctr + 1, useunicode);

		//Check for manual newline
		if ((kchr == '\r' && kchrnext != '\n') || kchr == '\n')
		{
			srcpos = ctr;
			curwidth = 0;
			lines++;
		}

		//Calculate width and wrapping
		if (kchr != '\r' && kchr != '\n')
		{
			curwidth += FontDraw_GetKerning(FontDraw_ConvertChr(kchr, fcset, fontrc, defaultchar), kchr, kchrnext, fontrc, fontkerning);
			if (wrapwidth > 0)
				if (curwidth + FontDraw_GetCharWidth(FontDraw_ConvertChr(kchrnext, fcset, fontrc, defaultchar), fontrc) > (long)wrapwidth)
				{
					long spacepos = FontDraw_LocateSpace(text, ctr + 1, srcpos + 1, useunicode);
					long spacepos_end = spacepos;
					if (spacepos < 0)
					{
						spacepos = ctr;
						spacepos_end = spacepos + 1;
					}
					srcpos = spacepos + 1;
					curwidth = 0;
					ctr = spacepos;
					lines++;
				}
		}
	}
	return(lines * (fontrc[7] - fontrc[5]) + (lines - 1) * fontrc[1]);
}

//Prepare font wrapped to the given width
void FontDraw_PrepareFontWrap(unsigned char *dst, const unsigned char *src, unsigned long wrapwidth, const WCHAR *fcset, const short *fontrc, const short *fontkerning, long defaultchar, bool useunicode)
{
	if (fontrc == NULL || wrapwidth == 0)
		return;
	unsigned long ctr, dstpos = 0, srcpos = 0, len = FontDraw_StrLen(src, useunicode);
	long kchr, kchrnext, curwidth = 0;
	unsigned char bytewidth = 1;
	if (useunicode)
		bytewidth = 2;
	FontDraw_SetChr(dst, 0, 0, useunicode);
	for (ctr = 0; ctr < len; ctr++)
	{
		kchr = FontDraw_GetChr(src, ctr, useunicode);
		kchrnext = FontDraw_GetChr(src, ctr + 1, useunicode);

		//Check for manual newline
		if ((kchr == '\r' && kchrnext != '\n') || kchr == '\n')
		{
			ClassEMemory::Copy(&dst[dstpos * bytewidth], &src[srcpos * bytewidth], (ctr - srcpos) * bytewidth);
			dstpos += ctr - srcpos;
			srcpos = ctr;
			FontDraw_SetChr(dst, dstpos, 0, useunicode);
			curwidth = 0;
		}

		//Calculate width and wrapping
		if (kchr != '\r' && kchr != '\n')
		{
			curwidth += FontDraw_GetKerning(FontDraw_ConvertChr(kchr, fcset, fontrc, defaultchar), kchr, kchrnext, fontrc, fontkerning);
			if (curwidth + FontDraw_GetCharWidth(FontDraw_ConvertChr(kchrnext, fcset, fontrc, defaultchar), fontrc) > (long)wrapwidth)
			{
				long spacepos = FontDraw_LocateSpace(src, ctr + 1, srcpos + 1, useunicode);
				long spacepos_end = spacepos;
				if (spacepos < 0)
				{
					spacepos = ctr;
					spacepos_end = spacepos + 1;
				}
				ClassEMemory::Copy(&dst[dstpos * bytewidth], &src[srcpos * bytewidth], (spacepos_end - srcpos) * bytewidth);
				dstpos += spacepos_end - srcpos;
				srcpos = spacepos + 1;
				FontDraw_SetChr(dst, dstpos++, '\n', useunicode);
				FontDraw_SetChr(dst, dstpos, 0, useunicode);
				curwidth = 0;
				ctr = spacepos;
			}
		}
	}

	//Cat remaining characters
	if (srcpos < len)
	{
		ClassEMemory::Copy(&dst[dstpos * bytewidth], &src[srcpos * bytewidth], (len - srcpos) * bytewidth);
		dstpos += len - srcpos;
		FontDraw_SetChr(dst, dstpos, 0, useunicode);
	}
}

//Prepare font wrapped to the given width
void FontDraw_Draw(FUNC_DRAWFONTCHARACTER drawfunc, long x, long y, const unsigned char *text, unsigned long wrapwidth, unsigned long alignment, const WCHAR *fcset, const short *fontrc, const short *fontkerning, long defaultchar, bool useunicode, void *userdata)
{
	if (fontrc == NULL)
		return;
	unsigned long ctr, ctr2, srcpos = 0, len = FontDraw_StrLen(text, useunicode);
	long kchr, kchrnext, kchrdraw, kchrdrawnext, kchridx, onx, ony, curwidth = 0;
	unsigned char bytewidth = 1;
	if (useunicode)
		bytewidth = 2;
	//if (alignment & EFO_HCENTER)
	//	x -= FontDraw_GetFontWidth(text, wrapwidth, fcset, fontrc, fontkerning, useunicode) / 2;
	//if (alignment & EFO_HRIGHT)
	//	x -= FontDraw_GetFontWidth(text, wrapwidth, fcset, fontrc, fontkerning, useunicode);
	if (alignment & EFO_VCENTER)
		y -= FontDraw_GetFontHeight(text, wrapwidth, fcset, fontrc, fontkerning, defaultchar, useunicode) / 2;
	else if (alignment & EFO_VBOTTOM)
		y -= FontDraw_GetFontHeight(text, wrapwidth, fcset, fontrc, fontkerning, defaultchar, useunicode);
	ony = y;
	for (ctr = 0; ctr < len; ctr++)
	{
		kchr = FontDraw_GetChr(text, ctr, useunicode);
		kchrnext = FontDraw_GetChr(text, ctr + 1, useunicode);

		//Check for manual newline
		if ((kchr == '\r' && kchrnext != '\n') || kchr == '\n')
		{
			onx = x;
			if (alignment & EFO_HCENTER)
				onx -= FontDraw_GetFontWidth(&text[srcpos * bytewidth], 0, fcset, fontrc, fontkerning, defaultchar, useunicode, false, ctr - srcpos) / 2;
			else if (alignment & EFO_HRIGHT)
				onx -= FontDraw_GetFontWidth(&text[srcpos * bytewidth], 0, fcset, fontrc, fontkerning, defaultchar, useunicode, false, ctr - srcpos);
			for (ctr2 = srcpos; ctr2 < ctr; ctr2++)
			{
				kchrdraw = FontDraw_GetChr(text, ctr2, useunicode);
				kchrdrawnext = FontDraw_GetChr(text, ctr2 + 1, useunicode);
				kchridx = FontDraw_ConvertChr(kchrdraw, fcset, fontrc, defaultchar);
				if (kchridx >= 0 && kchridx < fontrc[3])
					drawfunc(onx, ony, &fontrc[4 + kchridx * 4], userdata);
				onx += FontDraw_GetKerning(kchridx, kchrdraw, kchrdrawnext, fontrc, fontkerning);
			}
			srcpos = ctr;
			curwidth = 0;
			ony += fontrc[7] - fontrc[5] + fontrc[1];
		}

		//Calculate width and wrapping
		if (kchr != '\r' && kchr != '\n')
		{
			curwidth += FontDraw_GetKerning(FontDraw_ConvertChr(kchr, fcset, fontrc, defaultchar), kchr, kchrnext, fontrc, fontkerning);
			if (wrapwidth > 0)
				if (curwidth + FontDraw_GetCharWidth(FontDraw_ConvertChr(kchrnext, fcset, fontrc, defaultchar), fontrc) > (long)wrapwidth)
				{
					long spacepos = FontDraw_LocateSpace(text, ctr + 1, srcpos + 1, useunicode);
					long spacepos_end = spacepos;
					if (spacepos < 0)
					{
						spacepos = ctr;
						spacepos_end = spacepos + 1;
					}
					onx = x;
					if (alignment & EFO_HCENTER)
						onx -= FontDraw_GetFontWidth(&text[srcpos * bytewidth], 0, fcset, fontrc, fontkerning, defaultchar, useunicode, false, spacepos_end - srcpos) / 2;
					else if (alignment & EFO_HRIGHT)
						onx -= FontDraw_GetFontWidth(&text[srcpos * bytewidth], 0, fcset, fontrc, fontkerning, defaultchar, useunicode, false, spacepos_end - srcpos);
					for (ctr2 = srcpos; ctr2 < (unsigned long)spacepos_end; ctr2++)
					{
						kchrdraw = FontDraw_GetChr(text, ctr2, useunicode);
						kchrdrawnext = FontDraw_GetChr(text, ctr2 + 1, useunicode);
						kchridx = FontDraw_ConvertChr(kchrdraw, fcset, fontrc, defaultchar);
						if (kchridx >= 0 && kchridx < fontrc[3])
							drawfunc(onx, ony, &fontrc[4 + kchridx * 4], userdata);
						onx += FontDraw_GetKerning(kchridx, kchrdraw, kchrdrawnext, fontrc, fontkerning);
					}
					srcpos = spacepos + 1;
					curwidth = 0;
					ctr = spacepos;
					ony += fontrc[7] - fontrc[5] + fontrc[1];
				}
		}
	}

	//Draw remaining characters
	if (srcpos < len)
	{
		onx = x;
		if (alignment & EFO_HCENTER)
			onx -= FontDraw_GetFontWidth(&text[srcpos * bytewidth], 0, fcset, fontrc, fontkerning, defaultchar, useunicode, false, len - srcpos) / 2;
		else if (alignment & EFO_HRIGHT)
			onx -= FontDraw_GetFontWidth(&text[srcpos * bytewidth], 0, fcset, fontrc, fontkerning, defaultchar, useunicode, false, len - srcpos);
		for (ctr2 = srcpos; ctr2 < (unsigned long)len; ctr2++)
		{
			kchrdraw = FontDraw_GetChr(text, ctr2, useunicode);
			kchrdrawnext = FontDraw_GetChr(text, ctr2 + 1, useunicode);
			kchridx = FontDraw_ConvertChr(kchrdraw, fcset, fontrc, defaultchar);
			if (kchridx >= 0 && kchridx < fontrc[3])
				drawfunc(onx, ony, &fontrc[4 + kchridx * 4], userdata);
			onx += FontDraw_GetKerning(kchridx, kchrdraw, kchrdrawnext, fontrc, fontkerning);
		}
	}
}
