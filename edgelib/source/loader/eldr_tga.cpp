// eldr_tga.cpp
// TGA image loader
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eloader.h"
#include "ememory.h"
#include "edisplay.h"
#include "econsole.h"

#include "extra/verinfo.h"

///////////////////////////////////////////////////////////////////
// Loader: Definitions                                           //
///////////////////////////////////////////////////////////////////

#define ETGA_PALETTE             1
#define ETGA_INDEXED             1
#define ETGA_RGB                 2
#define ETGA_GRAYSCALE           3

#define ETGA_DFL_ALPHA          15
#define ETGA_DFL_HFLIP          16
#define ETGA_DFL_VFLIP          32

#define MAX_ETGAPALETTE        256

///////////////////////////////////////////////////////////////////
// Loader: Structures                                            //
///////////////////////////////////////////////////////////////////

typedef struct
{
	unsigned char idlen;
	unsigned char colormaptype;
	unsigned char imgtype;
	unsigned short colmapstart;
	unsigned short colmaplength;
	unsigned char colmapbits;
	unsigned short xstart;
	unsigned short ystart;
	unsigned short width;
	unsigned short height;
	unsigned char cdepth;
	unsigned char descriptor;
	COLORREF palettetable[MAX_ETGAPALETTE];
}ETGA_HEADER;
//The targa header

///////////////////////////////////////////////////////////////////
// Loader: Public                                                //
///////////////////////////////////////////////////////////////////

//Start the loader
ERESULT ELoader_TGA::Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags)
{
	#if (EDITION_SDK < 1000)
		return(E_NOFEATURE);
	#else
		ERESULT result = E_OK;
		ETGA_HEADER header;
		ELDR_2DCALLBACKINFO cbinfo;
		unsigned long datastart = 18;
		LinkData(ldata, lsize);
		cbinfo.nativedisplaymode = nativedisplaymode;
		cbinfo.createflags = createflags;
		cbinfo.paletteindex = 0;
		cbinfo.pixelindex = 0;
		cbinfo.usetransparency = false;
		cbinfo.compressedformat = NULL;
		Read8(&header.idlen);
		Read8(&header.colormaptype);
		Read8(&header.imgtype);
		Read16(&header.colmapstart);
		Read16(&header.colmaplength);
		Read8(&header.colmapbits);
		Read16(&header.xstart);
		Read16(&header.ystart);
		Read16(&header.width);
		Read16(&header.height);
		Read8(&header.cdepth);
		Read8(&header.descriptor);
		if (header.colormaptype > ETGA_PALETTE)
			return(E_UNSUPPORTED);
		if (header.imgtype != ETGA_INDEXED && header.imgtype != ETGA_RGB && header.imgtype != ETGA_GRAYSCALE)
			return(E_UNSUPPORTED);
		if (header.colormaptype == ETGA_PALETTE)
			if (/*header.colmapbits != 15 && header.colmapbits != 16 && */header.colmapbits != 24 && header.colmapbits != 32)
				return(E_UNSUPPORTED);
		if (header.cdepth != 8 && header.cdepth != 16 && header.cdepth != 24 && header.cdepth != 32)
			return(E_UNSUPPORTED);
		if (header.imgtype == ETGA_GRAYSCALE && header.cdepth != 8)
			return(E_UNSUPPORTED);
		if (header.descriptor & ETGA_DFL_HFLIP)
			return(E_UNSUPPORTED);
		cbinfo.width = header.width;
		cbinfo.height = header.height;
		result = surface->LdrOnCreate(&cbinfo);
		if (result != E_OK)
			return(result);
		SeekSet(18 + header.idlen);
		datastart += header.idlen;
		if (header.colormaptype == ETGA_PALETTE)
		{
			unsigned char paldata[1024];
			unsigned long ctr;
			cbinfo.usepalette = true;
			ReadStream(paldata, header.colmaplength * header.colmapbits / 8);
			for (ctr = 0; ctr < header.colmaplength; ctr++)
			{
				if (header.colmapbits == 24)
					header.palettetable[ctr] = E_RGBA(paldata[ctr * 3 + 2], paldata[ctr * 3 + 1], paldata[ctr * 3], 255);
				else if (header.colmapbits == 32)
				{
					cbinfo.usetransparency = true;
					header.palettetable[ctr] = E_RGBA(paldata[ctr * 4 + 2], paldata[ctr * 4 + 1], paldata[ctr * 4], paldata[ctr * 4 + 3]);
				}
			}
			cbinfo.paletteindex = header.colmapstart;
			cbinfo.streamdata = header.palettetable;
			cbinfo.streamsize = header.colmaplength * sizeof(COLORREF);
			surface->LdrOnPaletteStream(&cbinfo);
			datastart += header.colmaplength * header.colmapbits / 8;
		}
		else
			cbinfo.usepalette = false;
		unsigned char *rawlinedata = NULL, *linedata = NULL;
		unsigned long ctr, scanline, rawpitch = header.width * (header.cdepth + 7 / 8 * 8) / 8, pitch = header.width;
		if (!cbinfo.usepalette)
			pitch *= 4;
		if (rawpitch == 0 || pitch == 0)
			return(E_ERROR);
		rawlinedata = (unsigned char *)ClassEMemory::Alloc(rawpitch);
		if (rawlinedata == NULL)
			return(E_NOMEMORY);
		linedata = (unsigned char *)ClassEMemory::Alloc(pitch);
		if (linedata == NULL)
		{
			ClassEMemory::DeAlloc(rawlinedata);
			return(E_NOMEMORY);
		}
		if (cbinfo.usepalette)
			cbinfo.streamdata = rawlinedata;
		else
			cbinfo.streamdata = linedata;
		cbinfo.streamsize = pitch;
		for (scanline = 0; scanline < header.height; scanline++)
		{
			SeekSet(datastart + scanline * rawpitch);
			if (header.descriptor & ETGA_DFL_VFLIP)
				cbinfo.pixelindex = scanline * cbinfo.width;
			else
				cbinfo.pixelindex = (header.height - scanline - 1) * cbinfo.width;
			ReadStream(rawlinedata, rawpitch);
			if (header.cdepth == 16)
			{
				for (ctr = 0; ctr < header.width; ctr++)
				{
					unsigned short b16pix = (rawlinedata[ctr * 2] << 8) | rawlinedata[ctr * 2 + 1];
					*(unsigned long *)(&linedata[ctr * 4]) = E_RGBA((b16pix & 31) << 3, (b16pix & 63488) >> 8, (b16pix & 2016) >> 3, 255);
				}
			}
			else if (header.cdepth == 24)
			{
				for (ctr = 0; ctr < header.width; ctr++)
					*(unsigned long *)(&linedata[ctr * 4]) = E_RGBA(rawlinedata[ctr * 3 + 2], rawlinedata[ctr * 3 + 1], rawlinedata[ctr * 3], 255);
			}
			else if (header.cdepth == 32)
			{
				cbinfo.usetransparency = true;
				for (ctr = 0; ctr < header.width; ctr++)
					*(unsigned long *)(&linedata[ctr * 4]) = E_RGBA(rawlinedata[ctr * 4 + 2], rawlinedata[ctr * 4 + 1], rawlinedata[ctr * 4], rawlinedata[ctr * 4 + 3]);
			}
			else if (header.cdepth == 8 && header.imgtype == ETGA_GRAYSCALE)
			{
				for (ctr = 0; ctr < header.width; ctr++)
					*(unsigned long *)(&linedata[ctr * 4]) = E_RGBA(rawlinedata[ctr], rawlinedata[ctr], rawlinedata[ctr], 255);
			}
			result = surface->LdrOnPixelStream(&cbinfo);
			if (result != E_OK)
			{
				result = E_OK;
				break;
			}
		}
		ClassEMemory::DeAlloc(rawlinedata);
		ClassEMemory::DeAlloc(linedata);
		return(result);
	#endif
}
