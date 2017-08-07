// eldr_png.cpp
// PNG image loader
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eloader.h"
#include "emath.h"
#include "ememory.h"
#include "efile.h"
#include "edisplay.h"
#include "econsole.h"

#include "extra/verinfo.h"

///////////////////////////////////////////////////////////////////
// Static data                                                   //
///////////////////////////////////////////////////////////////////

const unsigned char epng_ldr_c1bit[] = {0, 255};
const unsigned char epng_ldr_c2bit[] = {0, 85, 170, 255};
const unsigned char epng_ldr_c4bit[] = {0, 17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255};

///////////////////////////////////////////////////////////////////
// Type definitions                                              //
///////////////////////////////////////////////////////////////////

#define EPNG_SIGNATURE    0x89504E47
#define EPNG_IDENTIFIER   0x0D0A1A0A
#define EPNG_CgBI         0x43674249
#define EPNG_IHDR         0x49484452
#define EPNG_PLTE         0x504C5445
#define EPNG_gAMA         0x67414D41
#define EPNG_cHRM         0x6348524D
#define EPNG_tRNS         0x74524E53
#define EPNG_IDAT         0x49444154
#define EPNG_IEND         0x49454E44 //Chunks

#define EPNG_FLTRNONE              0
#define EPNG_FLTRSUB               1
#define EPNG_FLTRUP                2
#define EPNG_FLTRAVG               3
#define EPNG_FLTRPAETH             4 //Scanline filter types

#define MAX_EPNGPALETTE          256

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

typedef struct
{
	unsigned long sig;
	unsigned long identifier;
	unsigned long datastart;
	unsigned long width;
	unsigned long height;
	unsigned long datablocks;
	unsigned long datatotalsize;
	unsigned long transparentindex;
	unsigned char bitdepth;
	unsigned char compression;
	unsigned char filter;
	unsigned char interlaced;
	bool usecolor;
	bool usealpha;
	bool usepalette;
	bool appleformat;
	unsigned char alphapalette[MAX_EPNGPALETTE];
	COLORREF palettetable[MAX_EPNGPALETTE];
}EPNG_HEADER;
//The PNG header

///////////////////////////////////////////////////////////////////
// Loader: Pure functions                                        //
///////////////////////////////////////////////////////////////////

//Paeth predictor calculator for a specific filter (a = left, b = above, c = upperleft)
unsigned char inline ELoader_PNG_PaethPredictor(unsigned char a, unsigned char b, unsigned char c)
{
	unsigned long p;
	long pa, pb, pc;
	p = a + b - c;
	pa = (long)(p - a);
	pb = (long)(p - b);
	pc = (long)(p - c);
	if (pa < 0)
		pa = -pa;
	if (pb < 0)
		pb = -pb;
	if (pc < 0)
		pc = -pc;
	if (pa <= pb && pa <= pc)
		return(a);
	else if (pb <= pc)
		return(b);
	return(c);
}

//Calculate the size of a scanline
unsigned long ELoader_PNG_Linesize(unsigned long width, unsigned char bitdepth, bool usepalette, bool usecolor, bool usealpha)
{
	if (usepalette)
		return((width * bitdepth + 7) / 8 + 1);
	else if (usecolor)
	{
		if (usealpha)
			return((width * bitdepth * 4 + 7) / 8 + 1);
		else
			return((width * bitdepth * 3 + 7) / 8 + 1);
	}
	else
	{
		if (usealpha)
			return((width * bitdepth * 2 + 7) / 8 + 1);
		else
			return((width * bitdepth + 7) / 8 + 1);
	}
	return(1);
}

//Calculate CRC checksum
void ClassEStd_MakeCRCTable(unsigned long *table);
unsigned long ClassEStd_UpdateCRC(unsigned long *table, unsigned long crc, unsigned char *buf, int len);
unsigned long ELoader_PNG_CalculateCRC(unsigned long *table, unsigned char *buf, int len)
{
	return(ClassEStd_UpdateCRC(table, 0xffffffffL, buf, len) ^ 0xffffffffL);
}

///////////////////////////////////////////////////////////////////
// Loader: Private                                               //
///////////////////////////////////////////////////////////////////

//Revert filtered bytes
void ELoader_PNG::RevertBytes(unsigned char *cmem, unsigned long csize, unsigned long cwidth, unsigned long cheight, unsigned char bpp)
{
	unsigned long yctr, xctr;
	long ydist = cwidth + 1;
	unsigned char filter;
	for (yctr = 0; yctr < cheight; yctr++)
	{
		filter = *cmem;
		cmem++;
		if (filter == EPNG_FLTRSUB)
		{
			for (xctr = 0; xctr < cwidth; xctr++)
			{
				if (xctr >= bpp)
					cmem[xctr] += cmem[xctr - (signed char)(bpp)];
			}
		}
		else if (filter == EPNG_FLTRUP)
		{
			for (xctr = 0; xctr < cwidth; xctr++)
			{
				if (yctr > 0)
					cmem[xctr] += cmem[xctr - ydist];
			}
		}
		else if (filter == EPNG_FLTRAVG)
		{
			for (xctr = 0; xctr < cwidth; xctr++)
			{
				unsigned char exvalue1;
				unsigned char exvalue2;
				if (xctr >= bpp)
					exvalue1 = cmem[xctr - (signed char)(bpp)];
				else
					exvalue1 = 0;
				if (yctr > 0)
					exvalue2 = cmem[xctr - ydist];
				else
					exvalue2 = 0;
				exvalue1 = (exvalue1 + exvalue2) / 2;
				cmem[xctr] += exvalue1;
			}
		}
		else if (filter == EPNG_FLTRPAETH)
		{
			for (xctr = 0; xctr < cwidth; xctr++)
			{
				unsigned char exvalue1;
				unsigned char exvalue2;
				unsigned char exvalue3;
				if (xctr >= bpp)
					exvalue1 = cmem[xctr - (signed char)(bpp)];
				else
					exvalue1 = 0;
				if (yctr > 0)
					exvalue2 = cmem[xctr - ydist];
				else
					exvalue2 = 0;
				if (xctr >= bpp && yctr > 0)
					exvalue3 = cmem[xctr - ydist - (signed char)bpp];
				else
					exvalue3 = 0;
				exvalue1 = ELoader_PNG_PaethPredictor(exvalue1, exvalue2, exvalue3);
				cmem[xctr] += exvalue1;
			}
		}
		cmem += cwidth;
	}
}

//Rasterize the image depending on the color format
void ELoader_PNG::Rasterize(unsigned char *dst, unsigned char *src, unsigned long cwidth, unsigned long cheight, unsigned long scanlinewidth, void *hdrdata, unsigned long yofs)
{
	EPNG_HEADER *header = (EPNG_HEADER *)hdrdata;
	unsigned long xctr, yctr;
	scanlinewidth++;
	if (header->usepalette)
	{
		unsigned long mpos;
		for (yctr = 0; yctr < cheight; yctr++)
		{
			mpos = (yctr + yofs) * scanlinewidth + 1;
			if (header->bitdepth == 1)
			{
				for (xctr = 0; xctr < cwidth; xctr++)
				{
					if (xctr % 8 == 0)
						dst[yctr * cwidth + xctr] = (src[mpos] >> 7);
					else if (xctr % 8 == 1)
						dst[yctr * cwidth + xctr] = (src[mpos] >> 6) & 1;
					else if (xctr % 8 == 2)
						dst[yctr * cwidth + xctr] = (src[mpos] >> 5) & 1;
					else if (xctr % 8 == 3)
						dst[yctr * cwidth + xctr] = (src[mpos] >> 4) & 1;
					else if (xctr % 8 == 4)
						dst[yctr * cwidth + xctr] = (src[mpos] >> 3) & 1;
					else if (xctr % 8 == 5)
						dst[yctr * cwidth + xctr] = (src[mpos] >> 2) & 1;
					else if (xctr % 8 == 6)
						dst[yctr * cwidth + xctr] = (src[mpos] >> 1) & 1;
					else
						dst[yctr * cwidth + xctr] = (src[mpos++] & 1);
				}
			}
			else if (header->bitdepth == 2)
			{
				for (xctr = 0; xctr < cwidth; xctr++)
				{
					if (xctr % 4 == 0)
						dst[yctr * cwidth + xctr] = (src[mpos] >> 6);
					else if (xctr % 4 == 1)
						dst[yctr * cwidth + xctr] = (src[mpos] >> 4) & 3;
					else if (xctr % 4 == 2)
						dst[yctr * cwidth + xctr] = (src[mpos] >> 2) & 3;
					else
						dst[yctr * cwidth + xctr] = (src[mpos++] & 3);
				}
			}
			else if (header->bitdepth == 4)
			{
				for (xctr = 0; xctr < cwidth; xctr++)
				{
					if (xctr % 2 == 0)
						dst[yctr * cwidth + xctr] = src[mpos] >> 4;
					else
						dst[yctr * cwidth + xctr] = src[mpos++] & 15;
				}
			}
			else if (header->bitdepth == 8)
			{
				for (xctr = 0; xctr < cwidth; xctr++)
					dst[yctr * cwidth + xctr] = src[mpos++];
			}
		}
	}
	else if (header->usecolor)
	{
		if (header->bitdepth == 8)
		{
			if (header->usealpha)
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						dst[yctr * cwidth * 4 + xctr * 4] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 4];
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 4 + 1];
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 4 + 2];
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 4 + 3];
					}
			}
			else
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						dst[yctr * cwidth * 4 + xctr * 4] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 3];
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 3 + 1];
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 3 + 2];
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = 255;
					}
			}
		}
		else if (header->bitdepth == 16)
		{
			if (header->usealpha)
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						dst[yctr * cwidth * 4 + xctr * 4] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 8];
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 8 + 2];
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 8 + 4];
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 8 + 6];
					}
			}
			else
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						dst[yctr * cwidth * 4 + xctr * 4] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 6];
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 6 + 2];
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 6 + 4];
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = 255;
					}
			}
		}
		if (header->appleformat)
		{
			unsigned char temp;
			for (yctr = 0; yctr < cheight; yctr++)
				for (xctr = 0; xctr < cwidth; xctr++)
				{
					temp = dst[yctr * cwidth * 4 + xctr * 4];
					dst[yctr * cwidth * 4 + xctr * 4] = dst[yctr * cwidth * 4 + xctr * 4 + 2];
					dst[yctr * cwidth * 4 + xctr * 4 + 2] = temp;
				}
		}
	}
	else
	{
		if (header->bitdepth == 1)
		{
			if (header->usealpha)
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						unsigned char c, a;
						if (xctr % 4 == 0)
						{
							c = src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] >> 7;
							a = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] >> 6) & 1;
						}
						else if (xctr % 4 == 1)
						{
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] >> 5) & 1;
							a = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] >> 4) & 1;
						}
						else if (xctr % 4 == 2)
						{
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] >> 3) & 1;
							a = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] >> 2) & 1;
						}
						else
						{
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] >> 1) & 1;
							a = src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] & 1;
						}
						c = epng_ldr_c1bit[c & 1];
						a = epng_ldr_c1bit[a & 1];
						dst[yctr * cwidth * 4 + xctr * 4] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = a;
					}
			}
			else
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						unsigned char c;
						if (xctr % 8 == 0)
							c = src[(yctr + yofs) * scanlinewidth + 1 + xctr / 8] >> 7;
						else if (xctr % 8 == 1)
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 8] >> 6) & 1;
						else if (xctr % 8 == 2)
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 8] >> 5) & 1;
						else if (xctr % 8 == 3)
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 8] >> 4) & 1;
						else if (xctr % 8 == 4)
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 8] >> 3) & 1;
						else if (xctr % 8 == 5)
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 8] >> 2) & 1;
						else if (xctr % 8 == 6)
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 8] >> 1) & 1;
						else
							c = src[(yctr + yofs) * scanlinewidth + 1 + xctr / 8] & 1;
						c = epng_ldr_c1bit[c & 1];
						dst[yctr * cwidth * 4 + xctr * 4] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = 255;
					}
			}
		}
		else if (header->bitdepth == 2)
		{
			if (header->usealpha)
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						unsigned char c, a;
						if (xctr % 2 == 0)
						{
							c = src[(yctr + yofs) * scanlinewidth + 1 + xctr / 2] >> 6;
							a = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 2] >> 4) & 3;
						}
						else
						{
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 2] >> 2) & 3;
							a = src[(yctr + yofs) * scanlinewidth + 1 + xctr / 2] & 3;
						}
						c = epng_ldr_c2bit[c & 3];
						a = epng_ldr_c2bit[a & 3];
						dst[yctr * cwidth * 4 + xctr * 4] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = a;
					}
			}
			else
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						unsigned char c;
						if (xctr % 4 == 0)
							c = src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] >> 6;
						else if (xctr % 4 == 1)
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] >> 4) & 3;
						else if (xctr % 4 == 2)
							c = (src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] >> 2) & 3;
						else
							c = src[(yctr + yofs) * scanlinewidth + 1 + xctr / 4] & 3;
						c = epng_ldr_c2bit[c & 3];
						dst[yctr * cwidth * 4 + xctr * 4] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = 255;
					}
			}
		}
		else if (header->bitdepth == 4)
		{
			if (header->usealpha)
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						unsigned char c = src[(yctr + yofs) * scanlinewidth + 1 + xctr] >> 4, a = src[(yctr + yofs) * scanlinewidth + 1 + xctr] & 15;
						c = epng_ldr_c4bit[c & 15];
						a = epng_ldr_c4bit[a & 15];
						dst[yctr * cwidth * 4 + xctr * 4] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = a;
					}
			}
			else
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						unsigned char c;
						if (xctr % 2 == 0)
							c = src[(yctr + yofs) * scanlinewidth + 1 + xctr / 2] >> 4;
						else
							c = src[(yctr + yofs) * scanlinewidth + 1 + xctr / 2] & 15;
						c = epng_ldr_c4bit[c & 15];
						dst[yctr * cwidth * 4 + xctr * 4] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = 255;
					}
			}
		}
		else if (header->bitdepth == 8)
		{
			if (header->usealpha)
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						unsigned char c = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 2], a = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 2 + 1];
						dst[yctr * cwidth * 4 + xctr * 4] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = a;
					}
			}
			else
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						unsigned char c = src[(yctr + yofs) * scanlinewidth + 1 + xctr];
						dst[yctr * cwidth * 4 + xctr * 4] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = 255;
					}
			}
		}
		else if (header->bitdepth == 16)
		{
			if (header->usealpha)
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						unsigned char c = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 4], a = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 4 + 2];
						dst[yctr * cwidth * 4 + xctr * 4] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = a;
					}
			}
			else
			{
				for (yctr = 0; yctr < cheight; yctr++)
					for (xctr = 0; xctr < cwidth; xctr++)
					{
						unsigned char c = src[(yctr + yofs) * scanlinewidth + 1 + xctr * 2];
						dst[yctr * cwidth * 4 + xctr * 4] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 1] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 2] = c;
						dst[yctr * cwidth * 4 + xctr * 4 + 3] = 255;
					}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
// Loader: Public                                                //
///////////////////////////////////////////////////////////////////

//Start the loader
ERESULT ELoader_PNG::Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags)
{
	#if (EDITION_SDK < 500)
		return(E_NOFEATURE);
	#else
		ERESULT result = E_OK;
		EPNG_HEADER header;
		ELDR_2DCALLBACKINFO cbinfo;
		unsigned char paldata[768];
		bool segmentreading = true;
		unsigned long ctr, segment_id, segment_size, segment_crc, startpos;
		unsigned char packedfields;
		LinkData(ldata, lsize);
		SetByteOrder(true);
		cbinfo.nativedisplaymode = nativedisplaymode;
		cbinfo.createflags = createflags;
		cbinfo.paletteindex = 0;
		cbinfo.pixelindex = 0;
		cbinfo.usetransparency = false;
		cbinfo.compressedformat = NULL;
		Read32(&header.sig);
		Read32(&header.identifier);
		if (header.sig != EPNG_SIGNATURE || header.identifier != EPNG_IDENTIFIER)
			return(E_UNSUPPORTED);
		header.datablocks = 0;
		header.datatotalsize = 0;
		header.transparentindex = 0xFF000000;
		header.appleformat = false;
		for (ctr = 0; ctr < 256; ctr++)
			header.palettetable[ctr] = E_RGBA(0, 0, 0, 255);
		while (HaveData() && segmentreading)
		{
			startpos = Tell();
			Read32(&segment_size);
			Read32(&segment_id);
			switch(segment_id)
			{
				case EPNG_CgBI:
					header.appleformat = true;
					break;
				case EPNG_IHDR:
					Read32(&header.width);
					Read32(&header.height);
					Read8(&header.bitdepth);
					Read8(&packedfields);
					Read8(&header.compression);
					Read8(&header.filter);
					Read8(&header.interlaced);
					if (packedfields & 1)
						header.usepalette = true;
					else
						header.usepalette = false;
					if (packedfields & 2)
						header.usecolor = true;
					else
						header.usecolor = false;
					if (packedfields & 4)
					{
						cbinfo.usetransparency = true;
						header.usealpha = true;
					}
					else
						header.usealpha = false;
					if (header.width == 0 || header.height == 0)
						return(E_ERROR);
					if (header.compression != 0 || header.filter != 0)
						return(E_UNSUPPORTED);
					cbinfo.width = header.width;
					cbinfo.height = header.height;
					cbinfo.usepalette = header.usepalette;
					result = surface->LdrOnCreate(&cbinfo);
					if (result != E_OK)
						return(result);
					break;
				case EPNG_PLTE:
					if (segment_size % 3 == 0 && segment_size <= 768)
					{
						cbinfo.paletteindex = 0;
						ReadStream(paldata, 768);
						for (ctr = 0; ctr < segment_size / 3; ctr++)
							header.palettetable[ctr] = E_RGBA(paldata[ctr * 3], paldata[ctr * 3 + 1], paldata[ctr * 3 + 2], E_GETA(header.palettetable[ctr]));
						cbinfo.streamdata = header.palettetable;
						cbinfo.streamsize = segment_size / 3 * sizeof(COLORREF);
						surface->LdrOnPaletteStream(&cbinfo);
					}
					else
						return(E_ERROR);
					break;
				case EPNG_tRNS:
					cbinfo.usetransparency = true;
					if (header.usepalette)
					{
						unsigned char palalpha[MAX_EPNGPALETTE];
						ReadStream(palalpha, segment_size);
						for (ctr = 0; ctr < segment_size; ctr++)
							header.palettetable[ctr] = E_RGBA(E_GETR(header.palettetable[ctr]), E_GETG(header.palettetable[ctr]), E_GETB(header.palettetable[ctr]), palalpha[ctr]);
						cbinfo.paletteindex = 0;
						cbinfo.streamdata = header.palettetable;
						cbinfo.streamsize = sizeof(COLORREF) * segment_size;
						surface->LdrOnPaletteStream(&cbinfo);
					}
					else if (header.usecolor)
					{
						unsigned short r, g, b;
						Read16(&r);
						Read16(&g);
						Read16(&b);
						if (header.bitdepth == 16)
						{
							r >>= 8;
							g >>= 8;
							b >>= 8;
						}
						header.transparentindex = E_RGBX(r, g, b);
					}
					else
					{
						unsigned short v;
						Read16(&v);
						if (header.bitdepth == 1)
							v = epng_ldr_c1bit[v & 1];
						else if (header.bitdepth == 2)
							v = epng_ldr_c2bit[v & 3];
						else if (header.bitdepth == 4)
							v = epng_ldr_c4bit[v & 15];
						else if (header.bitdepth == 16)
							v >>= 8;
						header.transparentindex = E_RGBX(v, v, v);
					}
					break;
				case EPNG_gAMA:
					break;
				case EPNG_cHRM:
					break;
				case EPNG_IDAT:
					if (header.datablocks == 0)
						header.datastart = Tell() - 8;
					header.datablocks++;
					header.datatotalsize += segment_size;
					break;
				case EPNG_IEND:
					break;
			}
			SeekSet(startpos + segment_size + 8);
			Read32(&segment_crc);
		}
		if (header.datablocks)
		{
			result = DecodeImage(surface, &cbinfo, &header);
			return(result);
		}
		return(E_ERROR);
	#endif
}

//Decode the image and send the pixel stream
ERESULT ELoader_PNG::DecodeImage(E2DSurfaceBase *surface, ELDR_2DCALLBACKINFO *cbinfo, void *hdrdata)
{
	EPNG_HEADER *header = (EPNG_HEADER *)hdrdata;
	unsigned long dummy, ctr, mctr, src_size, dst_size;
	unsigned char *datamem = NULL, *decmpmem = NULL;
	if (header->interlaced)
	{
		dst_size  = (header->height + 7) / 8 * ELoader_PNG_Linesize((header->width + 7) / 8, header->bitdepth, header->usepalette, header->usecolor, header->usealpha);
		dst_size += (header->height + 7) / 8 * ELoader_PNG_Linesize((header->width + 3) / 8, header->bitdepth, header->usepalette, header->usecolor, header->usealpha);
		dst_size += (header->height + 3) / 8 * ELoader_PNG_Linesize((header->width + 3) / 4, header->bitdepth, header->usepalette, header->usecolor, header->usealpha);
		dst_size += (header->height + 3) / 4 * ELoader_PNG_Linesize((header->width + 1) / 4, header->bitdepth, header->usepalette, header->usecolor, header->usealpha);
		dst_size += (header->height + 1) / 4 * ELoader_PNG_Linesize((header->width + 1) / 2, header->bitdepth, header->usepalette, header->usecolor, header->usealpha);
		dst_size += (header->height + 1) / 2 * ELoader_PNG_Linesize((header->width) / 2, header->bitdepth, header->usepalette, header->usecolor, header->usealpha);
		dst_size += (header->height) / 2 * ELoader_PNG_Linesize((header->width), header->bitdepth, header->usepalette, header->usecolor, header->usealpha);
	}
	else
		dst_size = ELoader_PNG_Linesize(header->width, header->bitdepth, header->usepalette, header->usecolor, header->usealpha) * header->height;
	decmpmem = (unsigned char *)ClassEMemory::Alloc(dst_size);
	if (decmpmem == NULL)
		return(E_NOMEMORY);
	SeekSet(header->datastart);
	Read32(&src_size);
	Read32(&dummy);
	if (header->datablocks > 1)
	{
		unsigned long nptr = 0, dptr = 0;
		datamem = (unsigned char *)ClassEMemory::Alloc(header->datatotalsize);
		if (datamem == NULL)
		{
			ClassEMemory::DeAlloc(decmpmem);
			return(E_NOMEMORY);
		}
		for (mctr = 0; mctr < header->datablocks; mctr++)
		{
			SeekSet(header->datastart + dptr);
			Read32(&src_size);
			Read32(&dummy);
			ReadStream(&datamem[nptr], src_size);
			nptr += src_size;
			dptr += src_size + 12;
		}
		src_size = header->datatotalsize;
	}
	else
		datamem = &lmem[lptr];
	/*if (header->appleformat)
		dst_size = -1;*/
	if (ClassEMemory::UnCompressZLib(decmpmem, dst_size, datamem, src_size, header->appleformat))
	{
		unsigned long *memptr32;
		unsigned char *memptr = NULL;
		unsigned long pswidth[7], psheight[7];
		unsigned long imgmemsize, memindex, scanlinewidth = ELoader_PNG_Linesize(header->width, header->bitdepth, header->usepalette, header->usecolor, header->usealpha) - 1;
		unsigned char bpp, pass;
		if (header->datablocks > 1)
			ClassEMemory::DeAlloc(datamem);
		if (header->usepalette)
			bpp = (header->bitdepth + 7) / 8;
		else if (header->usecolor)
		{
			if (header->usealpha)
				bpp = (header->bitdepth * 4 + 7) / 8;
			else
				bpp = (header->bitdepth * 3 + 7) / 8;
		}
		else
		{
			if (header->usealpha)
				bpp = (header->bitdepth * 2 + 7) / 8;
			else
				bpp = (header->bitdepth + 7) / 8;
		}
		if (header->interlaced)
		{
			pswidth[0]  = (header->width + 7) / 8;
			pswidth[1]  = (header->width + 3) / 8;
			pswidth[2]  = (header->width + 3) / 4;
			pswidth[3]  = (header->width + 1) / 4;
			pswidth[4]  = (header->width + 1) / 2;
			pswidth[5]  = header->width / 2;
			pswidth[6]  = header->width;
			psheight[0] = (header->height + 7) / 8;
			psheight[1] = (header->height + 7) / 8;
			psheight[2] = (header->height + 3) / 8;
			psheight[3] = (header->height + 3) / 4;
			psheight[4] = (header->height + 1) / 4;
			psheight[5] = (header->height + 1) / 2;
			psheight[6] = (header->height) / 2;
			memindex = 0;
			for (pass = 0; pass < 7; pass++)
				if (pswidth[pass] > 0 && psheight[pass] > 0)
				{
					RevertBytes(&decmpmem[memindex], dst_size, ELoader_PNG_Linesize(pswidth[pass], header->bitdepth, header->usepalette, header->usecolor, header->usealpha) - 1, psheight[pass], bpp);
					memindex += ELoader_PNG_Linesize(pswidth[pass], header->bitdepth, header->usepalette, header->usecolor, header->usealpha) * psheight[pass];
				}
		}
		else
			RevertBytes(decmpmem, dst_size, scanlinewidth, header->height, bpp);
		if (header->interlaced)
		{
			unsigned long *lsmem = NULL, *ldmem = NULL;
			unsigned char *tmpmem;
			unsigned long yctr, xctr;
			imgmemsize = header->width * header->height;
			if (!header->usepalette)
				imgmemsize *= 4;
			memptr = (unsigned char *)ClassEMemory::Alloc(imgmemsize);
			if (memptr == NULL)
			{
				ClassEMemory::DeAlloc(decmpmem);
				return(E_NOMEMORY);
			}
			ldmem = (unsigned long *)memptr;
			memindex = 0;
			for (pass = 0; pass < 7; pass++)
				if (pswidth[pass] > 0 && psheight[pass] > 0)
				{
					tmpmem = NULL;
					if (header->usepalette)
						tmpmem = (unsigned char *)ClassEMemory::Alloc(pswidth[pass] * psheight[pass]);
					else
						tmpmem = (unsigned char *)ClassEMemory::Alloc(pswidth[pass] * psheight[pass] * 4);
					if (tmpmem == NULL)
						break;
					lsmem = (unsigned long *)tmpmem;
					Rasterize(tmpmem, &decmpmem[memindex], pswidth[pass], psheight[pass], ELoader_PNG_Linesize(pswidth[pass], header->bitdepth, header->usepalette, header->usecolor, header->usealpha) - 1, header);
					if (pass == 0)
					{
						for (yctr = 0; yctr < psheight[pass]; yctr++)
							for (xctr = 0; xctr < pswidth[pass]; xctr++)
							{
								if (header->usepalette)
									memptr[yctr * 8 * header->width + xctr * 8] = tmpmem[yctr * pswidth[pass] + xctr];
								else
									ldmem[yctr * 8 * header->width + xctr * 8] = lsmem[yctr * pswidth[pass] + xctr];
							}
					}
					if (pass == 1)
					{
						for (yctr = 0; yctr < psheight[pass]; yctr++)
							for (xctr = 0; xctr < pswidth[pass]; xctr++)
							{
								if (header->usepalette)
									memptr[yctr * 8 * header->width + xctr * 8 + 4] = tmpmem[yctr * pswidth[pass] + xctr];
								else
									ldmem[yctr * 8 * header->width + xctr * 8 + 4] = lsmem[yctr * pswidth[pass] + xctr];
							}
					}
					if (pass == 2)
					{
						for (yctr = 0; yctr < psheight[pass]; yctr++)
							for (xctr = 0; xctr < pswidth[pass]; xctr++)
							{
								if (header->usepalette)
									memptr[(yctr * 8 + 4) * header->width + xctr * 4] = tmpmem[yctr * pswidth[pass] + xctr];
								else
									ldmem[(yctr * 8 + 4) * header->width + xctr * 4] = lsmem[yctr * pswidth[pass] + xctr];
							}
					}
					if (pass == 3)
					{
						for (yctr = 0; yctr < psheight[pass]; yctr++)
							for (xctr = 0; xctr < pswidth[pass]; xctr++)
							{
								if (header->usepalette)
									memptr[yctr * 4 * header->width + xctr * 4 + 2] = tmpmem[yctr * pswidth[pass] + xctr];
								else
									ldmem[yctr * 4 * header->width + xctr * 4 + 2] = lsmem[yctr * pswidth[pass] + xctr];
							}
					}
					if (pass == 4)
					{
						for (yctr = 0; yctr < psheight[pass]; yctr++)
							for (xctr = 0; xctr < pswidth[pass]; xctr++)
							{
								if (header->usepalette)
									memptr[(yctr * 4 + 2) * header->width + xctr * 2] = tmpmem[yctr * pswidth[pass] + xctr];
								else
									ldmem[(yctr * 4 + 2) * header->width + xctr * 2] = lsmem[yctr * pswidth[pass] + xctr];
							}
					}
					if (pass == 5)
					{
						for (yctr = 0; yctr < psheight[pass]; yctr++)
							for (xctr = 0; xctr < pswidth[pass]; xctr++)
							{
								if (header->usepalette)
									memptr[yctr * 2 * header->width + xctr * 2 + 1] = tmpmem[yctr * pswidth[pass] + xctr];
								else
									ldmem[yctr * 2 * header->width + xctr * 2 + 1] = lsmem[yctr * pswidth[pass] + xctr];
							}
					}
					if (pass == 6)
					{
						for (yctr = 0; yctr < psheight[pass]; yctr++)
						{
							if (header->usepalette)
								ClassEMemory::Copy(&memptr[(yctr * 2 + 1) * header->width], &tmpmem[yctr * pswidth[pass]], header->width);
							else
								ClassEMemory::Copy(&memptr[(yctr * 2 + 1) * header->width * 4], &tmpmem[yctr * pswidth[pass] * 4], header->width * 4);
						}
					}
					memindex += ELoader_PNG_Linesize(pswidth[pass], header->bitdepth, header->usepalette, header->usecolor, header->usealpha) * psheight[pass];
					ClassEMemory::DeAlloc(tmpmem);
				}
			for (yctr = 0; yctr < header->height; yctr++)
			{
				imgmemsize = header->width;
				if (!header->usepalette)
					imgmemsize *= 4;
				if (!header->usepalette)
				{
					memptr32 = (unsigned long *)&memptr[yctr * imgmemsize];
					for (ctr = 0; ctr < header->width; ctr++)
						if (E_GETRGB(memptr32[ctr]) == header->transparentindex)
							memptr32[ctr] = E_RGBA(E_GETB(memptr32[ctr]), E_GETG(memptr32[ctr]), E_GETR(memptr32[ctr]), 0);
				}
				cbinfo->pixelindex = yctr * cbinfo->width;
				cbinfo->streamdata = &memptr[yctr * imgmemsize];
				cbinfo->streamsize = imgmemsize;
				surface->LdrOnPixelStream(cbinfo);
			}
			ClassEMemory::DeAlloc(memptr);
		}
		else
		{
			imgmemsize = header->width;
			if (!header->usepalette)
				imgmemsize *= 4;
			memptr = (unsigned char *)ClassEMemory::Alloc(imgmemsize);
			if (memptr == NULL)
			{
				ClassEMemory::DeAlloc(decmpmem);
				return(E_NOMEMORY);
			}
			for (mctr = 0; mctr < header->height; mctr++)
			{
				Rasterize(memptr, decmpmem, header->width, 1, scanlinewidth, header, mctr);
				if (!header->usepalette)
				{
					memptr32 = (unsigned long *)memptr;
					for (ctr = 0; ctr < header->width; ctr++)
						if (E_GETRGB(memptr32[ctr]) == header->transparentindex)
							memptr32[ctr] = E_GETRGB(memptr32[ctr]);
				}
				cbinfo->pixelindex = mctr * cbinfo->width;
				cbinfo->streamdata = memptr;
				cbinfo->streamsize = imgmemsize;
				surface->LdrOnPixelStream(cbinfo);
			}
			ClassEMemory::DeAlloc(memptr);
		}
	}
	else
	{
		if (header->datablocks > 1)
			ClassEMemory::DeAlloc(datamem);
		ClassEMemory::DeAlloc(decmpmem);
		return(E_ERROR);
	}
	ClassEMemory::DeAlloc(decmpmem);
	return(E_OK);
}

//Write the PNG header and chunk to a file
void ELoader_PNG::WriteHeader(ClassEFile *ef, unsigned long width, unsigned long height, unsigned char coldepth)
{
	#if (EDITION_SDK < 500)
		return;
	#else
		unsigned long crctable[256];
		unsigned long ctr;
		unsigned long firsthdr[] = { EPNG_SIGNATURE, EPNG_IDENTIFIER, 13, EPNG_IHDR, width, height };
		unsigned char secondhdr[] = { 8, 6, 0, 0, 0 };
		unsigned char hdrmem[sizeof(firsthdr) + sizeof(secondhdr)];
		if (coldepth == 8)
			secondhdr[1] = 3;
		for (ctr = 0; ctr < sizeof(firsthdr) / sizeof(unsigned long); ctr++)
			firsthdr[ctr] = ((firsthdr[ctr] & 0xFF) << 24) | ((firsthdr[ctr] & 0xFF00) << 8) | ((firsthdr[ctr] & 0xFF0000) >> 8) | ((firsthdr[ctr] & 0xFF000000) >> 24);
		ClassEMemory::Copy(hdrmem, firsthdr, sizeof(firsthdr));
		ClassEMemory::Copy(&hdrmem[sizeof(firsthdr)], secondhdr, sizeof(secondhdr));
		ClassEStd_MakeCRCTable(crctable);
		ef->Write(hdrmem, sizeof(hdrmem));
		unsigned long hdrcrc = ELoader_PNG_CalculateCRC(crctable, &hdrmem[12], sizeof(hdrmem) - 12);
		hdrcrc = ((hdrcrc & 0xFF) << 24) | ((hdrcrc & 0xFF00) << 8) | ((hdrcrc & 0xFF0000) >> 8) | ((hdrcrc & 0xFF000000) >> 24);
		ef->Write(&hdrcrc, sizeof(unsigned long));
		/*unsigned char gammachunk[] =
		{
			0x00, 0x00, 0x00, 0x04, 0x67, 0x41, 0x4D, 0x41, 0x00, 0x00, 0xB1, 0x8E, 0x7C, 0xFB, 0x51, 0x93
		};
		ef->Write(gammachunk, sizeof(gammachunk));
		unsigned char chromachunk[] =
		{
			0x00, 0x00, 0x00, 0x20, 0x63, 0x48, 0x52, 0x4d, 0x00, 0x00, 0x7a, 0x25, 0x00, 0x00, 0x80, 0x83,
			0x00, 0x00, 0xf9, 0xff,	0x00, 0x00, 0x80, 0xe8, 0x00, 0x00, 0x75, 0x30, 0x00, 0x00, 0xea, 0x60,
			0x00, 0x00, 0x3a, 0x97, 0x00, 0x00, 0x17, 0x6f, 0x97, 0xa9, 0x99, 0xd4
		};
		ef->Write(chromachunk, sizeof(chromachunk));*/
	#endif
}

//Write the pixel data from an entire surface to the PNG file
void ELoader_PNG::WritePixelData(ClassEFile *ef, E2DSurface *surface, RECT *rc, unsigned char coldepth, unsigned long *paldata)
{
	#if (EDITION_SDK < 500)
		return;
	#else
		unsigned long crctable[256];
		unsigned long lpitch = ELoader_PNG_Linesize(rc->right - rc->left, 8, coldepth == 8, true, true);
		unsigned char *saveptr = (unsigned char *)ClassEMemory::Alloc(lpitch * (rc->bottom - rc->top));
		ClassEStd_MakeCRCTable(crctable);
		if (saveptr != NULL)
		{
			long xc, yc;
			if (coldepth == 8)
			{
				unsigned char *palarray = (unsigned char *)ClassEMemory::Alloc(768);
				if (palarray)
				{
					unsigned long pltehdr[] = { 768, EPNG_PLTE };
					for (xc = 0; (unsigned long)xc < sizeof(pltehdr) / sizeof(unsigned long); xc++)
						pltehdr[xc] = ((pltehdr[xc] & 0xFF) << 24) | ((pltehdr[xc] & 0xFF00) << 8) | ((pltehdr[xc] & 0xFF0000) >> 8) | ((pltehdr[xc] & 0xFF000000) >> 24);
					ef->Write(pltehdr, 8);
					for (xc = 0; xc < 256; xc++)
					{
						palarray[xc * 3] = (unsigned char)(paldata[xc] & 0xFF);
						palarray[xc * 3 + 1] = (unsigned char)((paldata[xc] & 0xFF00) >> 8);
						palarray[xc * 3 + 2] = (unsigned char)((paldata[xc] & 0xFF0000) >> 16);
					}
					ef->Write(palarray, 768);
					unsigned long palcrc = ClassEStd_UpdateCRC(crctable, 0xffffffffL, (unsigned char *)&pltehdr[1], sizeof(pltehdr) - 4);
					palcrc = ClassEStd_UpdateCRC(crctable, palcrc, palarray, 768) ^ 0xffffffffL;
					palcrc = ((palcrc & 0xFF) << 24) | ((palcrc & 0xFF00) << 8) | ((palcrc & 0xFF0000) >> 8) | ((palcrc & 0xFF000000) >> 24);
					ef->Write(&palcrc, sizeof(unsigned long));
					unsigned long trnshdr[] = { 256, EPNG_tRNS };
					for (xc = 0; (unsigned long)xc < sizeof(trnshdr) / sizeof(unsigned long); xc++)
						trnshdr[xc] = ((trnshdr[xc] & 0xFF) << 24) | ((trnshdr[xc] & 0xFF00) << 8) | ((trnshdr[xc] & 0xFF0000) >> 8) | ((trnshdr[xc] & 0xFF000000) >> 24);
					ef->Write(trnshdr, 8);
					for (xc = 0; xc < 256; xc++)
						palarray[xc] = (unsigned char)((paldata[xc] & 0xFF000000) >> 24);
					ef->Write(palarray, 256);
					unsigned long trnscrc = ClassEStd_UpdateCRC(crctable, 0xffffffffL, (unsigned char *)&trnshdr[1], sizeof(trnshdr) - 4);
					trnscrc = ClassEStd_UpdateCRC(crctable, trnscrc, palarray, 256) ^ 0xffffffffL;
					trnscrc = ((trnscrc & 0xFF) << 24) | ((trnscrc & 0xFF00) << 8) | ((trnscrc & 0xFF0000) >> 8) | ((trnscrc & 0xFF000000) >> 24);
					ef->Write(&trnscrc, sizeof(unsigned long));
					ClassEMemory::DeAlloc(palarray);
				}
				for (yc = 0; yc < rc->bottom - rc->top; yc++)
				{
					saveptr[yc * lpitch] = EPNG_FLTRNONE;
					for (xc = 0; xc < rc->right - rc->left; xc++)
						saveptr[yc * lpitch + 1 + xc] = (unsigned char)surface->GetPixel(rc->left + xc, rc->top + yc);
				}
			}
			else
			{
				for (yc = 0; yc < rc->bottom - rc->top; yc++)
				{
					saveptr[yc * lpitch] = EPNG_FLTRNONE;
					for (xc = 0; xc < rc->right - rc->left; xc++)
					{
						COLORREF col = surface->GetPixel(rc->left + xc, rc->top + yc);
						saveptr[yc * lpitch + 1 + xc * 4] = (unsigned char)E_GETR(col);
						saveptr[yc * lpitch + 1 + xc * 4 + 1] = (unsigned char)E_GETG(col);
						saveptr[yc * lpitch + 1 + xc * 4 + 2] = (unsigned char)E_GETB(col);
						saveptr[yc * lpitch + 1 + xc * 4 + 3] = 255;
					}
				}
			}
			unsigned char *cmpmem = (unsigned char *)ClassEMemory::Alloc((lpitch * (rc->bottom - rc->top)) * 110 / 100 + 12);
			if (cmpmem != NULL)
			{
				unsigned long compressedsize;
				ClassEMemory::CompressZLib(cmpmem, compressedsize, saveptr, lpitch * (rc->bottom - rc->top));
				unsigned long datablock[] = { compressedsize, EPNG_IDAT };
				for (xc = 0; xc < (long)(sizeof(datablock) / sizeof(unsigned long)); xc++)
					datablock[xc] = ((datablock[xc] & 0xFF) << 24) | ((datablock[xc] & 0xFF00) << 8) | ((datablock[xc] & 0xFF0000) >> 8) | ((datablock[xc] & 0xFF000000) >> 24);
				ef->Write(datablock, sizeof(datablock));
				ef->Write(cmpmem, compressedsize);
				unsigned long datcrc;
				datcrc = ClassEStd_UpdateCRC(crctable, 0xffffffffL, (unsigned char *)&datablock[1], sizeof(datablock) - 4);
				datcrc = ClassEStd_UpdateCRC(crctable, datcrc, cmpmem, compressedsize) ^ 0xffffffffL;
				datcrc = ((datcrc & 0xFF) << 24) | ((datcrc & 0xFF00) << 8) | ((datcrc & 0xFF0000) >> 8) | ((datcrc & 0xFF000000) >> 24);
				ef->Write(&datcrc, sizeof(unsigned long));
				ClassEMemory::DeAlloc(cmpmem);
			}
			ClassEMemory::DeAlloc(saveptr);
			unsigned long end = 0;
			ef->Write(&end, sizeof(end));
			end = EPNG_IEND;
			end = ((end & 0xFF) << 24) | ((end & 0xFF00) << 8) | ((end & 0xFF0000) >> 8) | ((end & 0xFF000000) >> 24);
			ef->Write(&end, sizeof(end));
			end = 0x826042AE;
			ef->Write(&end, sizeof(end));
		}
	#endif
}

//Write the pixel data from an entire surface to the PNG file
void ELoader_PNG::WritePixelData(ClassEFile *ef, E2DSurfaceRGBA *surface, RECT *rc)
{
	#if (EDITION_SDK < 500)
		return;
	#else
		unsigned long crctable[256];
		unsigned long lpitch = ELoader_PNG_Linesize(rc->right - rc->left, 8, false, true, true);
		unsigned char *saveptr = (unsigned char *)ClassEMemory::Alloc(lpitch * (rc->bottom - rc->top));
		ClassEStd_MakeCRCTable(crctable);
		if (saveptr != NULL)
		{
			long xc, yc;
			for (yc = 0; yc < rc->bottom - rc->top; yc++)
			{
				saveptr[yc * lpitch] = EPNG_FLTRNONE;
				for (xc = 0; xc < rc->right - rc->left; xc++)
				{
					COLORREF col;
					unsigned char opacity;
					surface->GetPixel(rc->left + xc, rc->top + yc, col, opacity);
					saveptr[yc * lpitch + 1 + xc * 4] = (unsigned char)E_GETR(col);
					saveptr[yc * lpitch + 1 + xc * 4 + 1] = (unsigned char)E_GETG(col);
					saveptr[yc * lpitch + 1 + xc * 4 + 2] = (unsigned char)E_GETB(col);
					saveptr[yc * lpitch + 1 + xc * 4 + 3] = opacity;
				}
			}
			unsigned char *cmpmem = (unsigned char *)ClassEMemory::Alloc((lpitch * (rc->bottom - rc->top)) * 110 / 100 + 12);
			if (cmpmem != NULL)
			{
				unsigned long compressedsize;
				ClassEMemory::CompressZLib(cmpmem, compressedsize, saveptr, lpitch * (rc->bottom - rc->top));
				unsigned long datablock[] = { compressedsize, EPNG_IDAT };
				for (xc = 0; xc < (long)(sizeof(datablock) / sizeof(unsigned long)); xc++)
					datablock[xc] = ((datablock[xc] & 0xFF) << 24) | ((datablock[xc] & 0xFF00) << 8) | ((datablock[xc] & 0xFF0000) >> 8) | ((datablock[xc] & 0xFF000000) >> 24);
				ef->Write(datablock, sizeof(datablock));
				ef->Write(cmpmem, compressedsize);
				unsigned long datcrc;
				datcrc = ClassEStd_UpdateCRC(crctable, 0xffffffffL, (unsigned char *)&datablock[1], sizeof(datablock) - 4);
				datcrc = ClassEStd_UpdateCRC(crctable, datcrc, cmpmem, compressedsize) ^ 0xffffffffL;
				datcrc = ((datcrc & 0xFF) << 24) | ((datcrc & 0xFF00) << 8) | ((datcrc & 0xFF0000) >> 8) | ((datcrc & 0xFF000000) >> 24);
				ef->Write(&datcrc, sizeof(unsigned long));
				ClassEMemory::DeAlloc(cmpmem);
			}
			ClassEMemory::DeAlloc(saveptr);
			unsigned long end = 0;
			ef->Write(&end, sizeof(end));
			end = EPNG_IEND;
			end = ((end & 0xFF) << 24) | ((end & 0xFF00) << 8) | ((end & 0xFF0000) >> 8) | ((end & 0xFF000000) >> 24);
			ef->Write(&end, sizeof(end));
			end = 0x826042AE;
			ef->Write(&end, sizeof(end));
		}
	#endif
}
