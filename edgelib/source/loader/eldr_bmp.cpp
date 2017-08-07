// eldr_bmp.cpp
// BMP image loader
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eloader.h"
#include "ememory.h"
#include "efile.h"
#include "edisplay.h"

#include "extra/verinfo.h"


///////////////////////////////////////////////////////////////////
// Loader: Definitions                                           //
///////////////////////////////////////////////////////////////////

#define EBMP_HEADERSIG      0x4D42
#define EBMP_NOCOMPRESS          0
#define EBMP_OSWINDOWS          40
#define EBMP_WRITESCANLINE    1024

#define MAX_EBMPPALETTE        256

///////////////////////////////////////////////////////////////////
// Loader: Structures                                            //
///////////////////////////////////////////////////////////////////

typedef struct
{
	unsigned short sig;
	unsigned long  filesize;
	unsigned long  reserved;
	unsigned long  headersize;
	unsigned long  os;
	unsigned long  width;
	unsigned long  height;
	unsigned short planes;
	unsigned short cdepth;
	unsigned long  compression;
	unsigned long  datasize;
	unsigned long  hresolution;
	unsigned long  vresolution;
	unsigned long  usecolor;
	unsigned long  priocol;
	COLORREF palettetable[MAX_EBMPPALETTE];
}EBMP_HEADER;
//The bitmap header

///////////////////////////////////////////////////////////////////
// Loader: Public                                                //
///////////////////////////////////////////////////////////////////

//Start the loader
ERESULT ELoader_BMP::Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags)
{
	ERESULT result = E_OK;
	EBMP_HEADER header;
	ELDR_2DCALLBACKINFO cbinfo;
	LinkData(ldata, lsize);
	cbinfo.nativedisplaymode = nativedisplaymode;
	cbinfo.createflags = createflags;
	cbinfo.paletteindex = 0;
	cbinfo.pixelindex = 0;
	cbinfo.usetransparency = false;
	cbinfo.compressedformat = NULL;
	Read16(&header.sig);
	if (header.sig != EBMP_HEADERSIG)
		return(E_UNSUPPORTED);
	Read32(&header.filesize);
	Read32(&header.reserved);
	Read32(&header.headersize);
	Read32(&header.os);
	Read32(&header.width);
	Read32(&header.height);
	Read16(&header.planes);
	Read16(&header.cdepth);
	Read32(&header.compression);
	Read32(&header.datasize);
	Read32(&header.hresolution);
	Read32(&header.vresolution);
	Read32(&header.usecolor);
	Read32(&header.priocol);
	if (header.compression != EBMP_NOCOMPRESS || header.planes != 1)
		return(E_UNSUPPORTED);
	if (header.cdepth == 1 || header.cdepth == 4 || header.cdepth == 8 || header.cdepth == 24)
	{
		unsigned char *rawlinedata = NULL, *linedata = NULL;
		unsigned long ctr, scanline, rawpitch = (header.width * header.cdepth + 31) / 32 * 4, pitch = header.width;
		unsigned char rpixel;
		if (header.cdepth == 24)
		{
			pitch *= 4;
			cbinfo.usepalette = false;
		}
		else
			cbinfo.usepalette = true;
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
		cbinfo.width = header.width;
		cbinfo.height = header.height;
		result = surface->LdrOnCreate(&cbinfo);
		if (result != E_OK)
		{
			ClassEMemory::DeAlloc(rawlinedata);
			ClassEMemory::DeAlloc(linedata);
			return(result);
		}
		if (cbinfo.usepalette)
		{
			unsigned short endpal = 256;
			if (header.cdepth == 1)
				endpal = 2;
			else if (header.cdepth == 4)
				endpal = 16;
			SeekSet(54);
			ReadStream(header.palettetable, endpal * sizeof(COLORREF));
			for (ctr = 0; ctr < endpal; ctr++)
				header.palettetable[ctr] = E_RGBA(((header.palettetable[ctr] >> 16) & 0xFF), ((header.palettetable[ctr] >> 8) & 0xFF), (header.palettetable[ctr] & 0xFF), 255);
			cbinfo.streamdata = header.palettetable;
			cbinfo.streamsize = endpal * sizeof(COLORREF);
			surface->LdrOnPaletteStream(&cbinfo);
		}
		if (header.cdepth == 8)
			cbinfo.streamdata = rawlinedata;
		else
			cbinfo.streamdata = linedata;
		cbinfo.streamsize = pitch;
		for (scanline = 0; scanline < header.height; scanline++)
		{
			SeekSet(header.headersize + scanline * rawpitch);
			cbinfo.pixelindex = (header.height - scanline - 1) * cbinfo.width;
			ReadStream(rawlinedata, rawpitch);
			if (header.cdepth == 1)
			{
				for (ctr = 0; ctr < header.width; ctr++)
				{
					rpixel = rawlinedata[ctr / 8];
					if (7 - ctr % 8)
						rpixel >>= 7 - ctr % 8;
					linedata[ctr] = rpixel & 1;
				}
			}
			else if (header.cdepth == 4)
			{
				for (ctr = 0; ctr < header.width; ctr++)
				{
					rpixel = rawlinedata[ctr / 2];
					if (ctr % 2 == 0)
						rpixel >>= 4;
					linedata[ctr] = rpixel & 15;
				}
			}
			else if (header.cdepth == 24)
				for (ctr = 0; ctr < header.width; ctr++)
					*(unsigned long *)(&linedata[ctr * 4]) = E_RGBA(rawlinedata[ctr * 3 + 2], rawlinedata[ctr * 3 + 1], rawlinedata[ctr * 3], 255);
			result = surface->LdrOnPixelStream(&cbinfo);
			if (result != E_OK)
				break;
		}
		ClassEMemory::DeAlloc(rawlinedata);
		ClassEMemory::DeAlloc(linedata);
	}
	else
		return(E_UNSUPPORTED);
	return(result);
}

//Write the bitmap header to a file
void ELoader_BMP::WriteHeader(ClassEFile *ef, unsigned long width, unsigned long height, unsigned char coldepth)
{
	EBMP_HEADER header;
	unsigned char hdrmem[54];
	header.sig         = EBMP_HEADERSIG;
	header.compression = EBMP_NOCOMPRESS;
	header.planes      = 1;
	header.width       = width;
	header.height      = height;
	if (coldepth == 8)
		header.cdepth  = 8;
	else
		header.cdepth  = 24;
	header.headersize  = 54;
	header.datasize    = (header.width * header.cdepth / 8 + 3) / 4 * 4 * header.height;
	if (coldepth == 8)
		header.headersize += 1024;
	header.filesize    = header.datasize + header.headersize;
	header.hresolution = 2834;
	header.vresolution = 2834;
	header.os          = EBMP_OSWINDOWS;
	header.priocol     = 0;
	header.reserved    = 0;
	header.usecolor    = 0;
	ClassEMemory::Copy(&hdrmem[ 0], &header.sig,         2);
	ClassEMemory::Copy(&hdrmem[ 2], &header.filesize,    4);
	ClassEMemory::Copy(&hdrmem[ 6], &header.reserved,    4);
	ClassEMemory::Copy(&hdrmem[10], &header.headersize,  4);
	ClassEMemory::Copy(&hdrmem[14], &header.os,          4);
	ClassEMemory::Copy(&hdrmem[18], &header.width,       4);
	ClassEMemory::Copy(&hdrmem[22], &header.height,      4);
	ClassEMemory::Copy(&hdrmem[26], &header.planes,      2);
	ClassEMemory::Copy(&hdrmem[28], &header.cdepth,      2);
	ClassEMemory::Copy(&hdrmem[30], &header.compression, 4);
	ClassEMemory::Copy(&hdrmem[34], &header.datasize,    4);
	ClassEMemory::Copy(&hdrmem[38], &header.hresolution, 4);
	ClassEMemory::Copy(&hdrmem[42], &header.vresolution, 4);
	ClassEMemory::Copy(&hdrmem[46], &header.usecolor,    4);
	ClassEMemory::Copy(&hdrmem[50], &header.priocol,     4);
	ef->Write(hdrmem, 54);
	ClassEMemory::Copy(hdrdata, &header, 54);
}

//Write the pixel data from an entire surface to the bitmap
void ELoader_BMP::WritePixelData(ClassEFile *ef, E2DSurface *surface, RECT *rc, unsigned long *paldata)
{
	EBMP_HEADER header;
	ClassEMemory::Copy(&header, hdrdata, 54);
	unsigned char *saveptr = NULL;
	unsigned char bytewidth = 3;
	if (header.cdepth == 8)
		bytewidth = 1;
	unsigned long xc, yc, pitch = (rc->right - rc->left) * bytewidth / 4 * 4;
	if ((rc->right - rc->left) * bytewidth % 4 != 0)
		pitch += 4;
	header.headersize = 54;
	if (header.height * pitch < 1024)
		saveptr = (unsigned char *)ClassEMemory::Alloc(1024);
	else
		saveptr = (unsigned char *)ClassEMemory::Alloc(header.height * pitch);
	if (saveptr != NULL)
	{
		if (header.cdepth == 8)
		{
			unsigned long *savepal = (unsigned long *)saveptr;
			for (xc = 0; xc < 256; xc++)
				savepal[xc] = ((paldata[xc] & 0xFF0000) >> 16) | ((paldata[xc] & 0xFF00)) | ((paldata[xc] & 0xFF) << 16);
			ef->Write(saveptr, 1024);
			for (yc = 0; yc < header.height; yc++)
				for (xc = 0; xc < header.width; xc++)
					saveptr[yc * pitch + xc] = (unsigned char)surface->GetPixel(rc->left + xc, rc->bottom - yc - 1);
		}
		else
		{
			for (yc = 0; yc < header.height; yc++)
				for (xc = 0; xc < header.width; xc++)
				{
					COLORREF col = surface->GetPixel(rc->left + xc, rc->bottom - yc - 1);
					saveptr[yc * pitch + xc * 3]     = (unsigned char)((col >> 16) & 0xFF);
					saveptr[yc * pitch + xc * 3 + 1] = (unsigned char)((col >> 8) & 0xFF);
					saveptr[yc * pitch + xc * 3 + 2] = (unsigned char)(col & 0xFF);
				}
		}
		ef->Write(saveptr, header.height * pitch);
		ClassEMemory::DeAlloc(saveptr);
	}
}
