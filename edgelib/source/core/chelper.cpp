// chelper.cpp
// Edgecore: provides helper functions
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Device include
#include "edgedevice.h"

//Core include
#include "edgecore.h"

///////////////////////////////////////////////////////////////////
// Internal definitions and structures
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Helper API
///////////////////////////////////////////////////////////////////

//Safe mapping of error result
long ecExMapError(long idx, const long *errarray, long numarray)
{
	if (idx + 1 >= numarray)
		return(errarray[0]);
	return(errarray[idx + 1]);
}

//Scan and convert filename
void ecExScanFilename(ecUnistring fname, const ecUnistring pfname)
{
	unsigned long tfctr, tflen = ecStrLen(pfname);
	for (tfctr = 0; tfctr < tflen; tfctr++)
	{
		if (pfname[tfctr] == '\\' || pfname[tfctr] == '/')
		{
			#if defined(DEVICE_LINUX) || defined(DEVICE_MAC) || defined(DEVICE_OPENKODE)
				fname[tfctr] = '/';
			#else
				fname[tfctr] = '\\';
			#endif
		}
		else
			fname[tfctr] = pfname[tfctr];
	}
	fname[tflen] = 0;
}

//Create new log file
ECOREOBJ ecExCreateLogFile(void)
{
	ecUnichar fpath[512];
	ecErr dummy;
	ecFGetBinaryPath(fpath);
	ecStrCat(fpath, "diagnostics.txt");
	return(ecFOpen(dummy, fpath, 4));
}

//Log text
void ecExWriteLog(ECOREOBJ f, const char *txt, bool newline)
{
	ecFWrite(f, txt, ecStrLen(txt));
	if (newline)
	{
		char newline[] = "\r\n";
		ecFWrite(f, newline, 2);
	}
}

//Log text
void ecExWriteLog(ECOREOBJ f, int v, bool newline)
{
	char vstr[12];
	ecIntToStr(vstr, v);
	ecFWrite(f, vstr, ecStrLen(vstr));
	if (newline)
	{
		char newline[] = "\r\n";
		ecFWrite(f, newline, 2);
	}
}

//Return the length of a multibyte string
long ecStrLen(const char *str, unsigned long max_str)
{
	unsigned long ctr = 0;
	while (str[ctr] != 0 && ctr < max_str)
		ctr++;
	return(ctr);
}

//Return the length of a wide character
long ecStrLen(const ecUnistring str, unsigned long max_str)
{
	unsigned long ctr = 0;
	while (str[ctr] != 0 && ctr < max_str)
		ctr++;
	return(ctr);
}

//Copy a multibyte string
void ecStrCpy(char *dst, const char *src, long maxlen)
{
	long ctr, len = maxlen;
	if (len < 0)
		len = 16384;
	for (ctr = 0; ctr < len; ctr++)
	{
		if (src[ctr] == 0 || ctr == len - 1)
		{
			dst[ctr] = 0;
			break;
		}
		else
			dst[ctr] = src[ctr];
	}
}

//Convert a wide character string to a multibyte string
void ecStrCpy(char *dst, const ecUnistring src, long maxlen)
{
	long ctr, len = maxlen;
	if (len < 0)
		len = 16384;
	for (ctr = 0; ctr < len; ctr++)
	{
		if (src[ctr] == 0 || ctr == len - 1)
		{
			dst[ctr] = 0;
			break;
		}
		else
			dst[ctr] = (char)src[ctr];
	}
}

//Convert a multibyte string to a wide character string
void ecStrCpy(ecUnistring dst, const char *src, long maxlen)
{
	long ctr, len = maxlen;
	if (len < 0)
		len = 16384;
	for (ctr = 0; ctr < len; ctr++)
	{
		if (src[ctr] == 0 || ctr == len - 1)
		{
			dst[ctr] = 0;
			break;
		}
		else
			dst[ctr] = src[ctr];
	}
}

//Copy wide character strings
void ecStrCpy(ecUnistring dst, const ecUnistring src, long maxlen)
{
	long ctr, len = maxlen;
	if (len < 0)
		len = 16384;
	for (ctr = 0; ctr < len; ctr++)
	{
		if (src[ctr] == 0 || ctr == len - 1)
		{
			dst[ctr] = 0;
			break;
		}
		else
			dst[ctr] = src[ctr];
	}
}

//Concatenate a multibyte string
void ecStrCat(char *dst, const char *src)
{
	ecStrCpy(&dst[ecStrLen(dst)], src);
}

//Concatenate a multibyte string with a wide character string
void ecStrCat(char *dst, const ecUnistring src)
{
	ecStrCpy(&dst[ecStrLen(dst)], src);
}

//Concatenate a wide character string with a multibyte string
void ecStrCat(ecUnistring dst, const char *src)
{
	ecStrCpy(&dst[ecStrLen(dst)], src);
}

//Concatenate a wide character string
void ecStrCat(ecUnistring dst, const ecUnistring src)
{
	ecStrCpy(&dst[ecStrLen(dst)], src);
}

//Convert wide character string to multibyte UTF8 string
void ecStrToUtf8(char *dst, const ecUnistring src, long maxlen)
{
	unsigned long ctr, pos = 0, slen = ecStrLen(src);
	for (ctr = 0; ctr < slen; ctr++)
	{
		if (maxlen >= 0)
		{
			if (src[ctr] < 0x80 && pos + 1 >= (unsigned long)maxlen)
				break;
			if (src[ctr] >= 0x80 && src[ctr] < 0x800 && pos + 2 >= (unsigned long)maxlen)
				break;
			if (src[ctr] >= 0x800 /*&& src[ctr] < 0x10000*/ && pos + 3 >= (unsigned long)maxlen)
				break;
			//if (src[ctr] >= 0x10000 && src[ctr] < 0x110000 && pos + 4 >= (unsigned long)maxlen)
			//	break;
		}
		if (src[ctr] < 0x80)
		{
			dst[pos++] = (unsigned char)(src[ctr] & 0xFF);
		}
		else if (src[ctr] < 0x800)
		{
			dst[pos++] = (unsigned char)(0xC0 | ((src[ctr] & 0x7C0) >> 6));
			dst[pos++] = (unsigned char)(0x80 | ((src[ctr] & 0x3F)));
		}
		else/* if (src[ctr] < 0x10000)*/
		{
			dst[pos++] = (unsigned char)(0xE0 | ((src[ctr] & 0xF000) >> 12));
			dst[pos++] = (unsigned char)(0x80 | ((src[ctr] & 0xFC0) >> 6));
			dst[pos++] = (unsigned char)(0x80 | ((src[ctr] & 0x3F)));
		}
		//else if (src[ctr] < 0x110000)
		//{
		//	dst[pos++] = (unsigned char)(0xF0 | ((src[ctr] & 0x1C0000) >> 18));
		//	dst[pos++] = (unsigned char)(0x80 | ((src[ctr] & 0x3F000) >> 12));
		//	dst[pos++] = (unsigned char)(0x80 | ((src[ctr] & 0xFC0) >> 6));
		//	dst[pos++] = (unsigned char)(0x80 | ((src[ctr] & 0x3F)));
		//}
	}
	dst[pos] = 0;
}

//Convert multibyte UTF8 string to wide character string
void ecUtf8ToStr(ecUnistring dst, const char *src, long maxlen)
{
	unsigned long ctr, pos = 0, slen = ecStrLen(src);
	for (ctr = 0; ctr < slen; ctr++)
	{
		if (pos + 1 >= (unsigned long)maxlen && maxlen >= 0)
			break;
		dst[pos] = 0;
		if ((src[ctr] & 0xF0) == 0xF0)
		{
			dst[pos] += (src[ctr++] & 0x7) << 18;
			dst[pos] += (src[ctr++] & 0x3F) << 12;
			dst[pos] += (src[ctr++] & 0x3F) << 6;
			dst[pos++] += src[ctr] & 0x3F;
		}
		else if ((src[ctr] & 0xE0) == 0xE0)
		{
			dst[pos] += (src[ctr++] & 0xF) << 12;
			dst[pos] += (src[ctr++] & 0x3F) << 6;
			dst[pos++] += src[ctr] & 0x3F;
		}
		else if ((src[ctr] & 0xC0) == 0xC0)
		{
			dst[pos] += (src[ctr++] & 0x1F) << 6;
			dst[pos++] += src[ctr] & 0x3F;
		}
		else
			dst[pos++] = src[ctr];
	}
	dst[pos] = 0;
}

//Convert an integer value to a multibyte string
void ecIntToStr(char *dst, int i, unsigned char base, bool signedvalue)
{
	if (base < 2 || base > 36)
		return;
	long spos = 0, ilen = 0;
	unsigned char result;
	if (signedvalue)
	{
		int stmpi;
		if (i < 0)
		{
			dst[0] = '-';
			spos++;
			i = -i;
		}
		stmpi = i;
		do
		{
			ilen++;
			stmpi /= base;
		}while(stmpi > 0);
		dst[spos + ilen] = 0;
		do
		{
			result = i % base;
			if (result < 10)
				result += '0';
			else
				result += 'A' - 10;
			dst[spos + ilen - 1] = result;
			ilen--;
			i /= base;
		}
		while(i > 0);
	}
	else
	{
		unsigned int utmpi;
		utmpi = i;
		do
		{
			ilen++;
			utmpi /= base;
		}while(utmpi > 0);
		dst[spos + ilen] = 0;
		utmpi = i;
		do
		{
			result = utmpi % base;
			if (result < 10)
				result += '0';
			else
				result += 'A' - 10;
			dst[spos + ilen - 1] = result;
			ilen--;
			utmpi /= base;
		}
		while(utmpi > 0);
	}
}

//Convert an integer value to a wide character string
void ecIntToStr(ecUnistring dst, int i, unsigned char base, bool signedvalue)
{
	if (base < 2 || base > 36)
		return;
	long spos = 0, ilen = 0;
	unsigned char result;
	if (signedvalue)
	{
		int stmpi;
		if (i < 0)
		{
			dst[0] = '-';
			spos++;
			i = -i;
		}
		stmpi = i;
		do
		{
			ilen++;
			stmpi /= base;
		}while(stmpi > 0);
		dst[spos + ilen] = 0;
		do
		{
			result = i % base;
			if (result < 10)
				result += '0';
			else
				result += 'A' - 10;
			dst[spos + ilen - 1] = result;
			ilen--;
			i /= base;
		}
		while(i > 0);
	}
	else
	{
		unsigned int utmpi;
		utmpi = i;
		do
		{
			ilen++;
			utmpi /= base;
		}while(utmpi > 0);
		dst[spos + ilen] = 0;
		utmpi = i;
		do
		{
			result = utmpi % base;
			if (result < 10)
				result += '0';
			else
				result += 'A' - 10;
			dst[spos + ilen - 1] = result;
			ilen--;
			utmpi /= base;
		}
		while(utmpi > 0);
	}
}

//Convert a multibyte string to an integer value
int ecStrToInt(char *str, unsigned char base)
{
	if (base < 2 || base > 36)
		return(0);
	int rval = 0, valmul = 1;
	long slen = ecStrLen(str), ctr;
	unsigned char result;
	for (ctr = 0; ctr < slen; ctr++)
	{
		if (ctr == 0 && str[ctr] == '-')
		{
			valmul = -1;
			continue;
		}
		rval *= base;
		if (str[ctr] >= '0' && str[ctr] <= '9')
			result = str[ctr] - '0';
		else if (str[ctr] >= 'A' && str[ctr] <= 'Z')
			result = str[ctr] - 'A' + 10;
		else if (str[ctr] >= 'a' && str[ctr] <= 'z')
			result = str[ctr] - 'a' + 10;
		else
			return(0);
		if (result >= base)
			return(0);
		rval += result;
	}
	return(rval * valmul);
}

//Convert a wide character string to an integer value
int ecStrToInt(ecUnistring str, unsigned char base)
{
	if (base < 2 || base > 36)
		return(0);
	int rval = 0, valmul = 1;
	long slen = ecStrLen(str), ctr;
	unsigned char result;
	for (ctr = 0; ctr < slen; ctr++)
	{
		if (ctr == 0 && str[ctr] == '-')
		{
			valmul = -1;
			continue;
		}
		rval *= base;
		if (str[ctr] >= '0' && str[ctr] <= '9')
			result = (unsigned char)(str[ctr]) - '0';
		else if (str[ctr] >= 'A' && str[ctr] <= 'Z')
			result = (unsigned char)(str[ctr]) - 'A' + 10;
		else if (str[ctr] >= 'a' && str[ctr] <= 'z')
			result = (unsigned char)(str[ctr]) - 'a' + 10;
		else
			return(0);
		if (result >= base)
			return(0);
		rval += result;
	}
	return(rval * valmul);
}
