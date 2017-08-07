// emod_std.cpp
// Standard class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////
// 
// This software contains a C++/object oriented translation and modification of MD5 (version 1.02) by Mordechai T. Abzug.
// Translation and modification (c) 1995 by Mordechai T. Abzug
// 
// Copyright 1991-1992 RSA Data Security, Inc.
// The MD5 algorithm is defined in RFC 1321. This implementation is derived from the reference C code in RFC 1321 which is covered by the following copyright statement:
// 
// Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All rights reserved.
// License to copy and use this software is granted provided that it is identified as the ``RSA Data Security, Inc. MD5 Message-Digest Algorithm'' in all material mentioning or referencing this software or this function.
// 
// License is also granted to make and use derivative works provided that such works are identified as "derived from the RSA Data Security, Inc. MD5 Message-Digest Algorithm'' in all material mentioning or referencing the derived work.
// 
// RSA Data Security, Inc. makes no representations concerning either the merchantability of this software or the suitability of this software for any particular purpose. It is provided ``as is'' without express or implied warranty of any kind.
// 
// These notices must be retained in any copies of any part of this documentation and/or software.
// 
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "estdc.h"
#include "ememory.h"
#include "efile.h"

#include "extra/verinfo.h"

#define MD5S11   7
#define MD5S12  12
#define MD5S13  17
#define MD5S14  22
#define MD5S21   5
#define MD5S22   9
#define MD5S23  14
#define MD5S24  20
#define MD5S31   4
#define MD5S32  11
#define MD5S33  16
#define MD5S34  23
#define MD5S41   6
#define MD5S42  10
#define MD5S43  15
#define MD5S44  21
//MD5 definitions

/////////////////////////////////////////////////////////////////////
// ClassEStd::Pure functions                                       //
/////////////////////////////////////////////////////////////////////

//ROTATE_LEFT rotates x left n bits.
unsigned int inline ClassEStd_MD5RotateLeft(unsigned long x, unsigned long n){ return((x << n) | (x >> (32 - n))); }

// F, G, H and I are basic MD5 functions.
unsigned int inline ClassEStd_MD5F(unsigned long x, unsigned long y, unsigned long z){ return((x & y) | (~x & z)); }
unsigned int inline ClassEStd_MD5G(unsigned long x, unsigned long y, unsigned long z){ return((x & z) | (y & ~z)); }
unsigned int inline ClassEStd_MD5H(unsigned long x, unsigned long y, unsigned long z){ return(x ^ y ^ z); }
unsigned int inline ClassEStd_MD5I(unsigned long x, unsigned long y, unsigned long z){ return(y ^ (x | ~z)); }

// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4. Rotation is separate from addition to prevent recomputation.
void inline ClassEStd_MD5FF(unsigned long& a, unsigned long b, unsigned long c, unsigned long d, unsigned long x, unsigned long s, unsigned long ac){ a += ClassEStd_MD5F(b, c, d) + x + ac; a = ClassEStd_MD5RotateLeft(a, s) + b; }
void inline ClassEStd_MD5GG(unsigned long& a, unsigned long b, unsigned long c, unsigned long d, unsigned long x, unsigned long s, unsigned long ac){	a += ClassEStd_MD5G(b, c, d) + x + ac; a = ClassEStd_MD5RotateLeft(a, s) + b; }
void inline ClassEStd_MD5HH(unsigned long& a, unsigned long b, unsigned long c, unsigned long d, unsigned long x, unsigned long s, unsigned long ac){	a += ClassEStd_MD5H(b, c, d) + x + ac; a = ClassEStd_MD5RotateLeft(a, s) + b; }
void inline ClassEStd_MD5II(unsigned long& a, unsigned long b, unsigned long c, unsigned long d, unsigned long x, unsigned long s, unsigned long ac){	a += ClassEStd_MD5I(b, c, d) + x + ac; a = ClassEStd_MD5RotateLeft(a, s) + b; }

//Encodes input (unsigned long) into output (unsigned char). Assumes len is a multiple of 4.
void ClassEStd_MD5Encode(unsigned char *output, unsigned long *input, unsigned long len)
{
	unsigned int i, j;
	for (i = 0, j = 0; j < len; i++, j += 4)
	{
		output[j]     = (unsigned char)( input[i] & 0xFF);
		output[j + 1] = (unsigned char)((input[i] >> 8) & 0xFF);
		output[j + 2] = (unsigned char)((input[i] >> 16) & 0xFF);
		output[j + 3] = (unsigned char)((input[i] >> 24) & 0xFF);
	}
}

//Decodes input (unsigned char) into output (unsigned long). Assumes len is a multiple of 4.
void ClassEStd_MD5Decode(unsigned long *output, unsigned char *input, unsigned long len)
{
	unsigned int i, j;
	for (i = 0, j = 0; j < len; i++, j += 4)
		output[i] = ((unsigned long)input[j]) | (((unsigned long)input[j + 1]) << 8) | (((unsigned long)input[j + 2]) << 16) | (((unsigned long)input[j + 3]) << 24);
}

//MD5 basic transformation. Transforms state based on block.
void ClassEStd_MD5Transform(unsigned char block[64], unsigned long *state)
{
	unsigned long a = state[0], b = state[1], c = state[2], d = state[3], x[16];
	ClassEStd_MD5Decode(x, block, 64);
	ClassEStd_MD5FF(a, b, c, d, x[ 0], MD5S11, 0xd76aa478); /* 1 */
	ClassEStd_MD5FF(d, a, b, c, x[ 1], MD5S12, 0xe8c7b756); /* 2 */
	ClassEStd_MD5FF(c, d, a, b, x[ 2], MD5S13, 0x242070db); /* 3 */
	ClassEStd_MD5FF(b, c, d, a, x[ 3], MD5S14, 0xc1bdceee); /* 4 */
	ClassEStd_MD5FF(a, b, c, d, x[ 4], MD5S11, 0xf57c0faf); /* 5 */
	ClassEStd_MD5FF(d, a, b, c, x[ 5], MD5S12, 0x4787c62a); /* 6 */
	ClassEStd_MD5FF(c, d, a, b, x[ 6], MD5S13, 0xa8304613); /* 7 */
	ClassEStd_MD5FF(b, c, d, a, x[ 7], MD5S14, 0xfd469501); /* 8 */
	ClassEStd_MD5FF(a, b, c, d, x[ 8], MD5S11, 0x698098d8); /* 9 */
	ClassEStd_MD5FF(d, a, b, c, x[ 9], MD5S12, 0x8b44f7af); /* 10 */
	ClassEStd_MD5FF(c, d, a, b, x[10], MD5S13, 0xffff5bb1); /* 11 */
	ClassEStd_MD5FF(b, c, d, a, x[11], MD5S14, 0x895cd7be); /* 12 */
	ClassEStd_MD5FF(a, b, c, d, x[12], MD5S11, 0x6b901122); /* 13 */
	ClassEStd_MD5FF(d, a, b, c, x[13], MD5S12, 0xfd987193); /* 14 */
	ClassEStd_MD5FF(c, d, a, b, x[14], MD5S13, 0xa679438e); /* 15 */
	ClassEStd_MD5FF(b, c, d, a, x[15], MD5S14, 0x49b40821); /* 16 */
	ClassEStd_MD5GG(a, b, c, d, x[ 1], MD5S21, 0xf61e2562); /* 17 */
	ClassEStd_MD5GG(d, a, b, c, x[ 6], MD5S22, 0xc040b340); /* 18 */
	ClassEStd_MD5GG(c, d, a, b, x[11], MD5S23, 0x265e5a51); /* 19 */
	ClassEStd_MD5GG(b, c, d, a, x[ 0], MD5S24, 0xe9b6c7aa); /* 20 */
	ClassEStd_MD5GG(a, b, c, d, x[ 5], MD5S21, 0xd62f105d); /* 21 */
	ClassEStd_MD5GG(d, a, b, c, x[10], MD5S22,  0x2441453); /* 22 */
	ClassEStd_MD5GG(c, d, a, b, x[15], MD5S23, 0xd8a1e681); /* 23 */
	ClassEStd_MD5GG(b, c, d, a, x[ 4], MD5S24, 0xe7d3fbc8); /* 24 */
	ClassEStd_MD5GG(a, b, c, d, x[ 9], MD5S21, 0x21e1cde6); /* 25 */
	ClassEStd_MD5GG(d, a, b, c, x[14], MD5S22, 0xc33707d6); /* 26 */
	ClassEStd_MD5GG(c, d, a, b, x[ 3], MD5S23, 0xf4d50d87); /* 27 */
	ClassEStd_MD5GG(b, c, d, a, x[ 8], MD5S24, 0x455a14ed); /* 28 */
	ClassEStd_MD5GG(a, b, c, d, x[13], MD5S21, 0xa9e3e905); /* 29 */
	ClassEStd_MD5GG(d, a, b, c, x[ 2], MD5S22, 0xfcefa3f8); /* 30 */
	ClassEStd_MD5GG(c, d, a, b, x[ 7], MD5S23, 0x676f02d9); /* 31 */
	ClassEStd_MD5GG(b, c, d, a, x[12], MD5S24, 0x8d2a4c8a); /* 32 */
	ClassEStd_MD5HH(a, b, c, d, x[ 5], MD5S31, 0xfffa3942); /* 33 */
	ClassEStd_MD5HH(d, a, b, c, x[ 8], MD5S32, 0x8771f681); /* 34 */
	ClassEStd_MD5HH(c, d, a, b, x[11], MD5S33, 0x6d9d6122); /* 35 */
	ClassEStd_MD5HH(b, c, d, a, x[14], MD5S34, 0xfde5380c); /* 36 */
	ClassEStd_MD5HH(a, b, c, d, x[ 1], MD5S31, 0xa4beea44); /* 37 */
	ClassEStd_MD5HH(d, a, b, c, x[ 4], MD5S32, 0x4bdecfa9); /* 38 */
	ClassEStd_MD5HH(c, d, a, b, x[ 7], MD5S33, 0xf6bb4b60); /* 39 */
	ClassEStd_MD5HH(b, c, d, a, x[10], MD5S34, 0xbebfbc70); /* 40 */
	ClassEStd_MD5HH(a, b, c, d, x[13], MD5S31, 0x289b7ec6); /* 41 */
	ClassEStd_MD5HH(d, a, b, c, x[ 0], MD5S32, 0xeaa127fa); /* 42 */
	ClassEStd_MD5HH(c, d, a, b, x[ 3], MD5S33, 0xd4ef3085); /* 43 */
	ClassEStd_MD5HH(b, c, d, a, x[ 6], MD5S34,  0x4881d05); /* 44 */
	ClassEStd_MD5HH(a, b, c, d, x[ 9], MD5S31, 0xd9d4d039); /* 45 */
	ClassEStd_MD5HH(d, a, b, c, x[12], MD5S32, 0xe6db99e5); /* 46 */
	ClassEStd_MD5HH(c, d, a, b, x[15], MD5S33, 0x1fa27cf8); /* 47 */
	ClassEStd_MD5HH(b, c, d, a, x[ 2], MD5S34, 0xc4ac5665); /* 48 */
	ClassEStd_MD5II(a, b, c, d, x[ 0], MD5S41, 0xf4292244); /* 49 */
	ClassEStd_MD5II(d, a, b, c, x[ 7], MD5S42, 0x432aff97); /* 50 */
	ClassEStd_MD5II(c, d, a, b, x[14], MD5S43, 0xab9423a7); /* 51 */
	ClassEStd_MD5II(b, c, d, a, x[ 5], MD5S44, 0xfc93a039); /* 52 */
	ClassEStd_MD5II(a, b, c, d, x[12], MD5S41, 0x655b59c3); /* 53 */
	ClassEStd_MD5II(d, a, b, c, x[ 3], MD5S42, 0x8f0ccc92); /* 54 */
	ClassEStd_MD5II(c, d, a, b, x[10], MD5S43, 0xffeff47d); /* 55 */
	ClassEStd_MD5II(b, c, d, a, x[ 1], MD5S44, 0x85845dd1); /* 56 */
	ClassEStd_MD5II(a, b, c, d, x[ 8], MD5S41, 0x6fa87e4f); /* 57 */
	ClassEStd_MD5II(d, a, b, c, x[15], MD5S42, 0xfe2ce6e0); /* 58 */
	ClassEStd_MD5II(c, d, a, b, x[ 6], MD5S43, 0xa3014314); /* 59 */
	ClassEStd_MD5II(b, c, d, a, x[13], MD5S44, 0x4e0811a1); /* 60 */
	ClassEStd_MD5II(a, b, c, d, x[ 4], MD5S41, 0xf7537e82); /* 61 */
	ClassEStd_MD5II(d, a, b, c, x[11], MD5S42, 0xbd3af235); /* 62 */
	ClassEStd_MD5II(c, d, a, b, x[ 2], MD5S43, 0x2ad7d2bb); /* 63 */
	ClassEStd_MD5II(b, c, d, a, x[ 9], MD5S44, 0xeb86d391); /* 64 */
	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;
	ClassEMemory::Set((unsigned char *)x, 0, sizeof(x));// Zeroize sensitive information.
}

//MD5 has calculation update
void ClassEStd_MD5Update(const void *buffer, unsigned long buflen, unsigned long *count, unsigned long *state, unsigned char *md5buffer)
{
	unsigned long input_index, buffer_index, buffer_space;
	buffer_index = (unsigned int)((count[0] >> 3) & 0x3F);
	if ((count[0] += ((unsigned long)buflen << 3)) < ((unsigned long)buflen << 3))
		count[1]++;
	count[1] += ((unsigned long)buflen >> 29);
	buffer_space = 64 - buffer_index;
	if (buflen >= buffer_space)
	{
		ClassEMemory::Copy(md5buffer + buffer_index, buffer, buffer_space);
		ClassEStd_MD5Transform(md5buffer, state);
		for (input_index = buffer_space; input_index + 63 < buflen; input_index += 64)
			ClassEStd_MD5Transform((unsigned char *)(buffer) + input_index, state);
		buffer_index = 0;
	}
	else
		input_index = 0;
	ClassEMemory::Copy(md5buffer + buffer_index, (unsigned char *)(buffer) + input_index, buflen - input_index);
}

//Get the result based on the md5 buffer
void ClassEStd_MD5Finalize(char *result, unsigned char *md5buffer, unsigned long *count, unsigned long *state)
{
	unsigned char PADDING[64] =
	{
		0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
	unsigned char digest[16], bits[8];
	unsigned int index, padLen;
	ClassEStd_MD5Encode(bits, count, 8);
	index = (unsigned long)((count[0] >> 3) & 0x3f);
	padLen = (index < 56) ? (56 - index) : (120 - index);
	ClassEStd_MD5Update(PADDING, padLen, count, state, md5buffer);
	ClassEStd_MD5Update(bits, 8, count, state, md5buffer);
	ClassEStd_MD5Encode(digest, state, 16);
	ClassEMemory::Set(md5buffer, 0, sizeof(*md5buffer));
	for (index = 0; index < 16; index++)
	{
		result[index * 2] = digest[index] / 16 + '0';
		result[index * 2 + 1] = digest[index] % 16 + '0';
		result[index * 2 + 2] = 0;
	}
	for (index = 0; index < 32; index++)
		if (result[index] > '9')
			result[index] = result[index] - '9' - 1 + 'a';
}

//Replace an occurrence in a string
void ClassEStd_StrNew(char *src, const char *replace, long srclen)
{
	long ctr, replacelen = ClassEStd::StrLen(replace), totallen = ClassEStd::StrLen(src);
	if (replacelen <= srclen)
	{
		ClassEMemory::Copy(src, replace, replacelen);
		if (replacelen < srclen)
			for (ctr = replacelen; ctr < totallen + 1 - (srclen - replacelen); ctr++)
				src[ctr] = src[ctr + (srclen - replacelen)];
	}
	else
	{
		for (ctr = totallen + (replacelen - srclen); ctr >= replacelen; ctr--)
			src[ctr] = src[ctr - (replacelen - srclen)];
		ClassEMemory::Copy(src, replace, replacelen);
	}
}

//Replace an occurrence in a string
void ClassEStd_StrNew(WCHAR *src, const WCHAR *replace, long srclen)
{
	long ctr, replacelen = ClassEStd::StrLen(replace), totallen = ClassEStd::StrLen(src);
	if (replacelen <= srclen)
	{
		ClassEMemory::Copy(src, replace, replacelen * sizeof(WCHAR));
		if (replacelen < srclen)
			for (ctr = replacelen; ctr < totallen + 1 - (srclen - replacelen); ctr++)
				src[ctr] = src[ctr + (srclen - replacelen)];
	}
	else
	{
		for (ctr = totallen + (replacelen - srclen); ctr >= replacelen; ctr--)
			src[ctr] = src[ctr - (replacelen - srclen)];
		ClassEMemory::Copy(src, replace, replacelen * sizeof(WCHAR));
	}
}

//Replace an occurrence in a string
void ClassEStd_StrNew(WCHAR *src, const char *replace, long srclen)
{
	long ctr, replacelen = ClassEStd::StrLen(replace), totallen = ClassEStd::StrLen(src);
	if (replacelen <= srclen)
	{
		ClassEMemory::Copy(src, replace, replacelen * sizeof(WCHAR));
		if (replacelen < srclen)
			for (ctr = replacelen; ctr < totallen + 1 - (srclen - replacelen); ctr++)
				src[ctr] = src[ctr + (srclen - replacelen)];
	}
	else
	{
		for (ctr = totallen + (replacelen - srclen); ctr >= replacelen; ctr--)
			src[ctr] = src[ctr - (replacelen - srclen)];
		ClassEMemory::Copy(src, replace, replacelen * sizeof(WCHAR));
	}
}

//Create a lookup table 
void ClassEStd_MakeCRCTable(unsigned long *table)
{
	unsigned long c;
	int n, k;
	for (n = 0; n < 256; n++)
	{
		c = (unsigned long)n;
		for (k = 0; k < 8; k++)
		{
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
				c = c >> 1;
		}
		table[n] = c;
	}
}

//Update CRC checksum
unsigned long ClassEStd_UpdateCRC(unsigned long *table, unsigned long crc, unsigned char *buf, int len)
{
	unsigned long c = crc;
	int n;
	for (n = 0; n < len; n++)
		c = table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
	return(c);
}

/////////////////////////////////////////////////////////////////////
// ClassEStd::Private                                              //
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// ClassEStd::Public                                               //
/////////////////////////////////////////////////////////////////////

//Return if the character is a space separator
bool ClassEStd::IsWhiteSpace(WCHAR chr)
{
	return(chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r' || chr == '\f' || chr == '\x0b');
}

//Copy multibyte strings
void ClassEStd::StrCpy(char *dst, const char *src, long maxlen)
{
	long ctr;
	if (maxlen < 0)
		maxlen = 0xFFFFFF;
	for (ctr = 0; ctr < maxlen; ctr++)
	{
		if (src[ctr] == 0 || ctr == maxlen - 1)
		{
			dst[ctr] = 0;
			break;
		}
		else
			dst[ctr] = src[ctr];
	}
}

//Convert a multibyte string to a wide character string
void ClassEStd::StrCpy(WCHAR *dst, const char *src, long maxlen)
{
	long ctr;
	if (maxlen < 0)
		maxlen = 0xFFFFFF;
	for (ctr = 0; ctr < maxlen; ctr++)
	{
		if (src[ctr] == 0 || ctr == maxlen - 1)
		{
			dst[ctr] = 0;
			break;
		}
		else
		{
			unsigned char src_u = src[ctr];
			dst[ctr] = src_u;
		}
	}
}

//Convert a wide character string to a multibyte string
void ClassEStd::StrCpy(char *dst, const WCHAR *src, long maxlen)
{
	long ctr;
	if (maxlen < 0)
		maxlen = 0xFFFFFF;
	for (ctr = 0; ctr < maxlen; ctr++)
	{
		if (src[ctr] == 0 || ctr == maxlen - 1)
		{
			dst[ctr] = 0;
			break;
		}
		else
			dst[ctr] = (char)src[ctr];
	}
}

//Copy wide character strings
void ClassEStd::StrCpy(WCHAR *dst, const WCHAR *src, long maxlen)
{
	long ctr;
	if (maxlen < 0)
		maxlen = 0xFFFFFF;
	for (ctr = 0; ctr < maxlen; ctr++)
	{
		if (src[ctr] == 0 || ctr == maxlen - 1)
		{
			dst[ctr] = 0;
			break;
		}
		else
			dst[ctr] = src[ctr];
	}
}

//Concatenate a multibyte string
void ClassEStd::StrCat(char *dst, const char *src)
{
	StrCpy(&dst[StrLen(dst)], src);
}

//Concatenate a wide character string with a multibyte string
void ClassEStd::StrCat(WCHAR *dst, const char *src)
{
	StrCpy(&dst[StrLen(dst)], src);
}

//Concatenate a multibyte string with a wide character string
void ClassEStd::StrCat(char *dst, const WCHAR *src)
{
	StrCpy(&dst[StrLen(dst)], src);
}

//Concatenate a wide character string
void ClassEStd::StrCat(WCHAR *dst, const WCHAR *src)
{
	StrCpy(&dst[StrLen(dst)], src);
}

//Concatenate a multibyte string with an integer value
void ClassEStd::StrCat(char *dst, int i, unsigned char base, bool signedvalue)
{
	IntToStr(&dst[StrLen(dst)], i, base, signedvalue);
}

//Concatenate a wide character string with an integer value
void ClassEStd::StrCat(WCHAR *dst, int i, unsigned char base, bool signedvalue)
{
	IntToStr(&dst[StrLen(dst)], i, base, signedvalue);
}

//Return the length of a multibyte string
long ClassEStd::StrLen(const char *str, long max_str)
{
	unsigned long ctr = 0;
	while (str[ctr] != 0 && (ctr < (unsigned long)max_str || max_str < 0))
		ctr++;
	return(ctr);
}

//Return the length of a wide character
long ClassEStd::StrLen(const WCHAR *str, long max_str)
{
	unsigned long ctr = 0;
	while (str[ctr] != 0 && (ctr < (unsigned long)max_str || max_str < 0))
		ctr++;
	return(ctr);
}

//Compares 2 multibyte strings
bool ClassEStd::StrEqual(const char *str1, const char *str2, bool casesensitive)
{
	unsigned long ctr, len = StrLen(str1), tlen = StrLen(str2);
	if (tlen != len)
		return(false);
	for (ctr = 0; ctr < len; ctr++)
		if (!casesensitive)
		{
			unsigned long v1 = str1[ctr], v2 = str2[ctr];
			if (v1 >= 'a' && v1 <= 'z')
				v1 -= 32;
			if (v2 >= 'a' && v2 <= 'z')
				v2 -= 32;
			if (v1 != v2)
				return(false);
		}
		else if (str1[ctr] != str2[ctr])
			return(false);
	return(true);
}

//Conpares a multibyte string with a wide character string
bool ClassEStd::StrEqual(const WCHAR *str1, const char *str2, bool casesensitive)
{
	unsigned long ctr, len = StrLen(str1), tlen = StrLen(str2);
	if (tlen != len)
		return(false);
	for (ctr = 0; ctr < len; ctr++)
		if (!casesensitive)
		{
			unsigned long v1 = str1[ctr], v2 = str2[ctr];
			if (v1 >= 'a' && v1 <= 'z')
				v1 -= 32;
			if (v2 >= 'a' && v2 <= 'z')
				v2 -= 32;
			if (v1 != v2)
				return(false);
		}
		else if (str1[ctr] != str2[ctr])
			return(false);
	return(true);
}

//Conpares a multibyte string with a wide character string
bool ClassEStd::StrEqual(const char *str1, const WCHAR *str2, bool casesensitive)
{
	unsigned long ctr, len = StrLen(str1), tlen = StrLen(str2);
	if (tlen != len)
		return(false);
	for (ctr = 0; ctr < len; ctr++)
		if (!casesensitive)
		{
			unsigned long v1 = str1[ctr], v2 = str2[ctr];
			if (v1 >= 'a' && v1 <= 'z')
				v1 -= 32;
			if (v2 >= 'a' && v2 <= 'z')
				v2 -= 32;
			if (v1 != v2)
				return(false);
		}
		else if (str1[ctr] != str2[ctr])
			return(false);
	return(true);
}

//Conpares 2 wide character strings
bool ClassEStd::StrEqual(const WCHAR *str1, const WCHAR *str2, bool casesensitive)
{
	unsigned long ctr, len = StrLen(str1), tlen = StrLen(str2);
	if (tlen != len)
		return(false);
	for (ctr = 0; ctr < len; ctr++)
		if (!casesensitive)
		{
			unsigned long v1 = str1[ctr], v2 = str2[ctr];
			if (v1 >= 'a' && v1 <= 'z')
				v1 -= 32;
			if (v2 >= 'a' && v2 <= 'z')
				v2 -= 32;
			if (v1 != v2)
				return(false);
		}
		else if (str1[ctr] != str2[ctr])
			return(false);
	return(true);
}

//Convert an integer value to a multibyte string
void ClassEStd::IntToStr(char *dst, int i, unsigned char base, bool signedvalue)
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
void ClassEStd::IntToStr(WCHAR *dst, int i, unsigned char base, bool signedvalue)
{
	char tmpstr[128];
	IntToStr(tmpstr, i, base, signedvalue);
	StrCpy(dst, tmpstr);
}

//Convert a multibyte string to an integer value
int ClassEStd::StrToInt(char *str, unsigned char base)
{
	if (base < 2 || base > 36)
		return(0);
	int rval = 0, valmul = 1;
	long slen = StrLen(str), ctr;
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
int ClassEStd::StrToInt(WCHAR *str, unsigned char base)
{
	if (base < 2 || base > 36)
		return(0);
	int rval = 0, valmul = 1;
	long slen = StrLen(str), ctr;
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

//Convert wide character string to multibyte UTF8 string
void ClassEStd::StrToUtf8(char *dst, const WCHAR *src, long maxlen)
{
	unsigned long ctr, pos = 0, slen = StrLen(src);
	for (ctr = 0; ctr < slen; ctr++)
	{
		//int vlue = src[ctr];
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
void ClassEStd::Utf8ToStr(WCHAR *dst, const char *src, long maxlen)
{
	unsigned long ctr, pos = 0, slen = StrLen(src);
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

//Find a character in a multibyte string
long ClassEStd::StrFind(const char *str, char character)
{
	long ctr = 0;
	while(str[ctr] != 0)
	{
		if (str[ctr] == character)
			return(ctr);
		ctr++;
	}
	return(-1);
}

//Find a character in a wide character string
long ClassEStd::StrFind(const WCHAR *str, WCHAR character)
{
	long ctr = 0;
	while(str[ctr] != 0)
	{
		if (str[ctr] == character)
			return(ctr);
		ctr++;
	}
	return(-1);
}

//Find a multibyte string in a multibyte string
long ClassEStd::StrFind(const char *str, const char *findstr, bool casesensitive)
{
	unsigned long v1, v2;
	long slen = StrLen(str), flen = StrLen(findstr), ctr, ctr2;
	if (flen == 0)
		return(-1);
	if (flen > slen)
		return(-1);
	for (ctr = 0; ctr < slen; ctr++)
	{
		v1 = str[ctr]; v2 = findstr[0];
		if (!casesensitive)
		{
			if (v1 >= 'a' && v1 <= 'z')
				v1 -= 32;
			if (v2 >= 'a' && v2 <= 'z')
				v2 -= 32;
		}
		if (v1 == v2)
		{
			for (ctr2 = 0; ctr2 < flen; ctr2++)
			{
				if (ctr + ctr2 >= slen)
					return(-1);
				v1 = str[ctr + ctr2]; v2 = findstr[ctr2];
				if (!casesensitive)
				{
					if (v1 >= 'a' && v1 <= 'z')
						v1 -= 32;
					if (v2 >= 'a' && v2 <= 'z')
						v2 -= 32;
				}
				if (v1 != v2)
					break;
				if (ctr2 == flen - 1)
					return(ctr);
			}
		}
	}
	return(-1);
}

//Find a multibyte string in a wide character string
long ClassEStd::StrFind(const WCHAR *str, const char *findstr, bool casesensitive)
{
	unsigned long v1, v2;
	long slen = StrLen(str), flen = StrLen(findstr), ctr, ctr2;
	if (flen == 0)
		return(-1);
	if (flen > slen)
		return(-1);
	for (ctr = 0; ctr < slen; ctr++)
	{
		v1 = str[ctr]; v2 = findstr[0];
		if (!casesensitive)
		{
			if (v1 >= 'a' && v1 <= 'z')
				v1 -= 32;
			if (v2 >= 'a' && v2 <= 'z')
				v2 -= 32;
		}
		if (v1 == v2)
		{
			for (ctr2 = 0; ctr2 < flen; ctr2++)
			{
				if (ctr + ctr2 >= slen)
					return(-1);
				v1 = str[ctr + ctr2]; v2 = findstr[ctr2];
				if (!casesensitive)
				{
					if (v1 >= 'a' && v1 <= 'z')
						v1 -= 32;
					if (v2 >= 'a' && v2 <= 'z')
						v2 -= 32;
				}
				if (v1 != v2)
					break;
				if (ctr2 == flen - 1)
					return(ctr);
			}
		}
	}
	return(-1);
}

//Find a wide character string in a multibyte string
long ClassEStd::StrFind(const char *str, const WCHAR *findstr, bool casesensitive)
{
	unsigned long v1, v2;
	long slen = StrLen(str), flen = StrLen(findstr), ctr, ctr2;
	if (flen == 0)
		return(-1);
	if (flen > slen)
		return(-1);
	for (ctr = 0; ctr < slen; ctr++)
	{
		v1 = str[ctr]; v2 = findstr[0];
		if (!casesensitive)
		{
			if (v1 >= 'a' && v1 <= 'z')
				v1 -= 32;
			if (v2 >= 'a' && v2 <= 'z')
				v2 -= 32;
		}
		if (v1 == v2)
		{
			for (ctr2 = 0; ctr2 < flen; ctr2++)
			{
				if (ctr + ctr2 >= slen)
					return(-1);
				v1 = str[ctr + ctr2]; v2 = findstr[ctr2];
				if (!casesensitive)
				{
					if (v1 >= 'a' && v1 <= 'z')
						v1 -= 32;
					if (v2 >= 'a' && v2 <= 'z')
						v2 -= 32;
				}
				if (v1 != v2)
					break;
				if (ctr2 == flen - 1)
					return(ctr);
			}
		}
	}
	return(-1);
}

//Find a wide character string in a wide character string
long ClassEStd::StrFind(const WCHAR *str, const WCHAR *findstr, bool casesensitive)
{
	unsigned long v1, v2;
	long slen = StrLen(str), flen = StrLen(findstr), ctr, ctr2;
	if (flen == 0)
		return(-1);
	if (flen > slen)
		return(-1);
	for (ctr = 0; ctr < slen; ctr++)
	{
		v1 = str[ctr]; v2 = findstr[0];
		if (!casesensitive)
		{
			if (v1 >= 'a' && v1 <= 'z')
				v1 -= 32;
			if (v2 >= 'a' && v2 <= 'z')
				v2 -= 32;
		}
		if (v1 == v2)
		{
			for (ctr2 = 0; ctr2 < flen; ctr2++)
			{
				if (ctr + ctr2 >= slen)
					return(-1);
				v1 = str[ctr + ctr2]; v2 = findstr[ctr2];
				if (!casesensitive)
				{
					if (v1 >= 'a' && v1 <= 'z')
						v1 -= 32;
					if (v2 >= 'a' && v2 <= 'z')
						v2 -= 32;
				}
				if (v1 != v2)
					break;
				if (ctr2 == flen - 1)
					return(ctr);
			}
		}
	}
	return(-1);
}

//Left trim a multibyte string of whitespace
char *ClassEStd::StrTrimLeft(char *str)
{
	unsigned long ctr, numspace = 0, len = StrLen(str);
	while (IsWhiteSpace(str[numspace]))
		numspace++;
	if (numspace)
		for (ctr = 0; ctr < len - numspace + 1; ctr++)
			str[ctr] = str[ctr + numspace];
	return(str);
}

//Left trim a wide character string of whitespace
WCHAR *ClassEStd::StrTrimLeft(WCHAR *str)
{
	unsigned long ctr, numspace = 0, len = StrLen(str);
	while (IsWhiteSpace(str[numspace]))
		numspace++;
	if (numspace)
		for (ctr = 0; ctr < len - numspace + 1; ctr++)
			str[ctr] = str[ctr + numspace];
	return(str);
}

//Right trim a multibyte string of whitespace
char *ClassEStd::StrTrimRight(char *str)
{
	long ctr, len = StrLen(str);
	for (ctr = len - 1; ctr >= 0; ctr--)
		if (IsWhiteSpace(str[ctr]))
			str[ctr] = 0;
		else
			break;
	return(str);
}

//Right trim a wide character string of whitespace
WCHAR *ClassEStd::StrTrimRight(WCHAR *str)
{
	long ctr, len = StrLen(str);
	for (ctr = len - 1; ctr >= 0; ctr--)
		if (IsWhiteSpace(str[ctr]))
			str[ctr] = 0;
		else
			break;
	return(str);
}

//Trim a multibyte string on both sides
char *ClassEStd::StrTrim(char *str)
{
	return(StrTrimRight(StrTrimLeft(str)));
}

//Trim a wide character string on both sides
WCHAR *ClassEStd::StrTrim(WCHAR *str)
{
	return(StrTrimRight(StrTrimLeft(str)));
}

//Replace multiple occurrences in a multibyte string
int ClassEStd::StrReplace(char *src, const char *search, const char *replace, bool casesensitive, int max_replace)
{
	if (max_replace == -1)
		max_replace = 0xFFFFFFF;
	int cur_replace = 0;
	long str_pos, searchlen = StrLen(search), replacelen = StrLen(replace);
	while (cur_replace < max_replace)
	{
		str_pos = StrFind(src, search, casesensitive);
		if (str_pos < 0)
			break;
		ClassEStd_StrNew(&src[str_pos], replace, searchlen);
		src = &src[str_pos + replacelen];
		cur_replace++;
	}
	return(cur_replace);
}

//Replace multiple occurrences in a wide character string
int ClassEStd::StrReplace(WCHAR *src, const WCHAR *search, const WCHAR *replace, bool casesensitive, int max_replace)
{
	if (max_replace == -1)
		max_replace = 0xFFFFFFF;
	int cur_replace = 0;
	long str_pos, searchlen = StrLen(search), replacelen = StrLen(replace);
	while (cur_replace < max_replace)
	{
		str_pos = StrFind(src, search, casesensitive);
		if (str_pos < 0)
			break;
		ClassEStd_StrNew(&src[str_pos], replace, searchlen);
		src = &src[str_pos + replacelen];
		cur_replace++;
	}
	return(cur_replace);
}

//Replace multiple occurrences in a wide character string
int ClassEStd::StrReplace(WCHAR *src, const char *search, const WCHAR *replace, bool casesensitive, int max_replace)
{
	if (max_replace == -1)
		max_replace = 0xFFFFFFF;
	int cur_replace = 0;
	long str_pos, searchlen = StrLen(search), replacelen = StrLen(replace);
	while (cur_replace < max_replace)
	{
		str_pos = StrFind(src, search, casesensitive);
		if (str_pos < 0)
			break;
		ClassEStd_StrNew(&src[str_pos], replace, searchlen);
		src = &src[str_pos + replacelen];
		cur_replace++;
	}
	return(cur_replace);
}

//Replace multiple occurrences in a wide character string
int ClassEStd::StrReplace(WCHAR *src, const char *search, const char *replace, bool casesensitive, int max_replace)
{
	if (max_replace == -1)
		max_replace = 0xFFFFFFF;
	int cur_replace = 0;
	long str_pos, searchlen = StrLen(search), replacelen = StrLen(replace);
	while (cur_replace < max_replace)
	{
		str_pos = StrFind(src, search, casesensitive);
		if (str_pos < 0)
			break;
		ClassEStd_StrNew(&src[str_pos], replace, searchlen);
		src = &src[str_pos + replacelen];
		cur_replace++;
	}
	return(cur_replace);
}

//Replace multiple occurrences in a wide character string
int ClassEStd::StrReplace(WCHAR *src, const WCHAR *search, const char *replace, bool casesensitive, int max_replace)
{
	if (max_replace == -1)
		max_replace = 0xFFFFFFF;
	int cur_replace = 0;
	long str_pos, searchlen = StrLen(search), replacelen = StrLen(replace);
	while (cur_replace < max_replace)
	{
		str_pos = StrFind(src, search, casesensitive);
		if (str_pos < 0)
			break;
		ClassEStd_StrNew(&src[str_pos], replace, searchlen);
		src = &src[str_pos + replacelen];
		cur_replace++;
	}
	return(cur_replace);
}

//Uppercase multibyte string
char *ClassEStd::StrUpper(char *str)
{
	char ext_low[32];
	char ext_up[32];
	unsigned long ctr, len = StrLen(str);
	StrCpy(ext_low, "‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔÒÚÛÙıˆ¯˘˙˚¸˝˛");
	StrCpy(ext_up, "¿¡¬√ƒ≈∆«»… ÀÃÕŒœ–—“”‘’÷ÿŸ⁄€‹›ﬁ");
	for (ctr = 0; ctr < len; ctr++)
	{
		if (str[ctr] >= 'a' && str[ctr] <= 'z')
			str[ctr] -= 32;
		else
		{
			long pos = StrFind(ext_low, str[ctr]);
			if (pos >= 0)
				str[ctr] = ext_up[pos];
		}
	}
	return(str);
}

//Uppercase wide character string
WCHAR *ClassEStd::StrUpper(WCHAR *str)
{
	WCHAR ext_low[32];
	WCHAR ext_up[32];
	unsigned long ctr, len = StrLen(str);
	StrCpy(ext_low, "‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔÒÚÛÙıˆ¯˘˙˚¸˝˛");
	StrCpy(ext_up, "¿¡¬√ƒ≈∆«»… ÀÃÕŒœ–—“”‘’÷ÿŸ⁄€‹›ﬁ");
	for (ctr = 0; ctr < len; ctr++)
	{
		if (str[ctr] >= 'a' && str[ctr] <= 'z')
			str[ctr] -= 32;
		else
		{
			long pos = StrFind(ext_low, str[ctr]);
			if (pos >= 0)
				str[ctr] = ext_up[pos];
		}
	}
	return(str);
}

//Lowercase multibyte string
char *ClassEStd::StrLower(char *str)
{
	char ext_low[32];
	char ext_up[32];
	unsigned long ctr, len = StrLen(str);
	StrCpy(ext_low, "‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔÒÚÛÙıˆ¯˘˙˚¸˝˛");
	StrCpy(ext_up, "¿¡¬√ƒ≈∆«»… ÀÃÕŒœ–—“”‘’÷ÿŸ⁄€‹›ﬁ");
	for (ctr = 0; ctr < len; ctr++)
	{
		if (str[ctr] >= 'A' && str[ctr] <= 'Z')
			str[ctr] += 32;
		else
		{
			long pos = StrFind(ext_up, str[ctr]);
			if (pos >= 0)
				str[ctr] = ext_low[pos];
		}
	}
	return(str);
}

//Lowercase wide character string
WCHAR *ClassEStd::StrLower(WCHAR *str)
{
	WCHAR ext_low[32];
	WCHAR ext_up[32];
	unsigned long ctr, len = StrLen(str);
	StrCpy(ext_low, "‡·‚„‰ÂÊÁËÈÍÎÏÌÓÔÒÚÛÙıˆ¯˘˙˚¸˝˛");
	StrCpy(ext_up, "¿¡¬√ƒ≈∆«»… ÀÃÕŒœ–—“”‘’÷ÿŸ⁄€‹›ﬁ");
	for (ctr = 0; ctr < len; ctr++)
	{
		if (str[ctr] >= 'A' && str[ctr] <= 'Z')
			str[ctr] += 32;
		else
		{
			long pos = StrFind(ext_up, str[ctr]);
			if (pos >= 0)
				str[ctr] = ext_low[pos];
		}
	}
	return(str);
}

//Calculate the MD5 checksum of data
void ClassEStd::MD5Data(char *result, const void *buffer, unsigned long buflen)
{
	unsigned char md5buffer[64];
	unsigned long count[2], state[4];
	count[0] = 0;
	count[1] = 0;
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
	ClassEStd_MD5Update(buffer, buflen, count, state, md5buffer);
	ClassEStd_MD5Finalize(result, md5buffer, count, state);
}

//Calculate the MD5 checksum of a string
void ClassEStd::MD5Str(char *result, const char *str)
{
	MD5Data(result, str, StrLen(str));
}

//Calculate the MD5 checksum of a string
void ClassEStd::MD5Str(char *result, const WCHAR *str)
{
	MD5Data(result, str, StrLen(str) * sizeof(WCHAR));
}

//Calculate the MD5 checksum of a file handle
void ClassEStd::MD5File(char *result, ClassEFile *ef)
{
	unsigned char md5buffer[64], filebuffer[8192], dummy;
	unsigned long count[2], state[4], ctr, fsize = ef->Size();
	count[0] = 0;
	count[1] = 0;
	state[0] = 0x67452301;
	state[1] = 0xefcdab89;
	state[2] = 0x98badcfe;
	state[3] = 0x10325476;
	if (fsize)
	{
		for (ctr = 0; ctr < fsize / 8192; ctr++)
		{
			ef->Read(filebuffer, 8192);
			ClassEStd_MD5Update(filebuffer, 8192, count, state, md5buffer);
		}
		if (fsize % 8192)
		{
			ef->Read(filebuffer, fsize % 8192);
			ClassEStd_MD5Update(filebuffer, fsize % 8192, count, state, md5buffer);
		}
	}
	else
		ClassEStd_MD5Update(&dummy, 0, count, state, md5buffer);
	ClassEStd_MD5Finalize(result, md5buffer, count, state);
}

//Calculate the MD5 checksum of a file
void ClassEStd::MD5File(char *result, const char *filename)
{
	ClassEFile ef;
	unsigned char dummy;
	if (ef.Open(filename, EFOF_READ))
	{
		MD5File(result, &ef);
		ef.Close();
	}
	else
		MD5Data(result, &dummy, 0);
}

//Calculate the CRC checksum of data
unsigned long ClassEStd::CRCData(const void *buffer, unsigned long buflen)
{
	unsigned long crctable[256];
	ClassEStd_MakeCRCTable(crctable);
	return(ClassEStd_UpdateCRC(crctable, 0xffffffffL, (unsigned char *)buffer, buflen) ^ 0xffffffffL);
}

//Convert datetime structure to timestamp
ETYPE_TIMESTAMP ClassEStd::DateTimeToTimestamp(E_DATETIME *datetime)
{
	unsigned char mday[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 };
	unsigned long ctr, day = 0;
	if (datetime->year < 1970)
		return(0);
	for (ctr = 1970; ctr <= datetime->year; ctr++)
	{
		if (ctr < datetime->year)
			day += 365;
		if (ctr < datetime->year || datetime->month > 2)
			if (ctr % 4 == 0 && (ctr % 100 != 0 || ctr % 400 == 0))
				day++;
	}
	for (ctr = 0; ctr < datetime->month && ctr < 12; ctr++)
		day += mday[ctr];
	if (datetime->day > 0)
		day += datetime->day - 1;
	return(day * 86400 + datetime->hour * 3600 + datetime->minute * 60 + datetime->second);
}

//Convert a timestamp to a datetime structure
void ClassEStd::TimestampToDateTime(E_DATETIME *datetime, ETYPE_TIMESTAMP timestamp)
{
	unsigned char mday[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 };
	unsigned long ctr = 0;
	datetime->hour   = (unsigned char)(timestamp % 86400 / 3600);
	datetime->minute = (unsigned char)(timestamp % 86400 / 60 % 60);
	datetime->second = (unsigned char)(timestamp % 86400 % 60);
	timestamp /= 86400;
	datetime->year = 1970;
	datetime->month = 1;
	datetime->day = 1;
	while(timestamp)
	{
		bool leapyear = false;
		if (ctr % 4 == 0 && (ctr % 100 != 0 || ctr % 400 == 0))
			leapyear = true;
		if ((leapyear && timestamp >= 366) || (!leapyear && timestamp >= 365))
		{
			timestamp -= 365;
			if (leapyear)
				timestamp--;
			datetime->year++;
		}
		else
		{
			bool addmonth = false;
			if (datetime->month < 12)
				if (timestamp >= mday[datetime->month])
					addmonth = true;
			if (addmonth)
			{
				timestamp -= mday[datetime->month];
				datetime->month++;
			}
			else
			{
				datetime->day += (unsigned char)(timestamp);
				break;
			}
		}
		ctr++;
	}
}
