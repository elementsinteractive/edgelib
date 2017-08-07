// eldr_gif.cpp
// GIF image loader
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eloader.h"
#include "ememory.h"
#include "edisplay.h"

#include "extra/verinfo.h"


///////////////////////////////////////////////////////////////////
// Type definitions                                              //
///////////////////////////////////////////////////////////////////

#define EGIF_DUMMYCHAR     0x00
#define EGIF_EXTSEPARATOR  0x21
#define EGIF_IMGSEPARATOR  0x2C
#define EGIF_EXTENDGFX     0xF9
#define EGIF_PLAINTEXT     0x01
#define EGIF_COMMENT       0xFE
#define EGIF_APPEXTENSION  0xFF

#define MAX_EGIFPALETTE    256

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

typedef struct
{
	char sig[4];
	char version[4];
	unsigned long datastart;
	unsigned short width;
	unsigned short height;
	unsigned short globaltablesize;
	unsigned char bgcolor;
	COLORREF globaltable[MAX_EGIFPALETTE];
}EGIF_HEADER;
//The GIF header

typedef struct
{
	unsigned short sig;
	unsigned short delaytime;
	unsigned char size;
	unsigned char terminator;
	unsigned char disposalmethod;
	unsigned char transparentindex;
	bool userinput;
	bool transparency;
}EGIF_IMGEXTGFX;
//Image extended graphics information

typedef struct
{
	EGIF_IMGEXTGFX gfxext;
	unsigned long datastart;
	unsigned long imgsizecompressed;
	unsigned short xofs;
	unsigned short yofs;
	unsigned short width;
	unsigned short height;
	unsigned short localtablesize;
	unsigned char sig;
	unsigned char compressionsize;
	bool usegfxext;
	bool interlaced;
	COLORREF localtable[MAX_EGIFPALETTE];
}EGIF_IMGINFO;
//Image information

///////////////////////////////////////////////////////////////////
// Loader: Private                                               //
///////////////////////////////////////////////////////////////////

//Add a string in the uncompression string table
void ELoader_GIF::AddUCmpString(unsigned char *fulldata, unsigned long srdst_size, unsigned long str_size, unsigned short initsize)
{
	if (num_str + initsize + 2 >= 4096)
		return;
	str_index[num_str] = srdst_size - str_size + 1;
	str_len[num_str] = str_size;
	num_str++;
}

//Retrieve a string from the compression/uncompression string table
void ELoader_GIF::GetString(unsigned char *fulldata, unsigned char *str, unsigned long &str_size, unsigned long code, unsigned short initsize)
{
	if (code >= num_str + initsize + 2)
	{
		str_size = 0;
		return;
	}
	if (code < (unsigned long)initsize + 2)
	{
		str[0] = (unsigned char)code;
		str_size = 1;
	}
	else
	{
		str_size = str_len[code - initsize - 2];
		ClassEMemory::Copy(str, &fulldata[str_index[code - initsize - 2]], str_size);
	}
}

//Uncompress data
bool ELoader_GIF::UnCompress(void *dst, unsigned long dst_size, void *src, unsigned long src_size, unsigned char gifcodesize)
{
	if (src == NULL || src_size == 0)
		return(false);
	unsigned char cur_str[4096];
	unsigned char *cdata = (unsigned char *)src, *udata = (unsigned char *)dst;
	unsigned long codesize, fcode = 0, d_pos = 0, s_pos = 0, cur_str_size = 0;
	unsigned short code, oldcode, initsize, gif_clear, gif_eof;
	unsigned char ctr, gifstreamsize, bituse = 0, bitsize = gifcodesize + 1;
	codesize = 2;
	for (ctr = 0; ctr < bitsize - 1; ctr++)
		codesize *= 2;
	initsize = (unsigned short)(codesize / 2);
	if (str_index != NULL)
	{
		delete str_index;
		str_index = NULL;
	}
	if (str_len != NULL)
	{
		delete str_len;
		str_len = NULL;
	}
	str_index = new unsigned long[4096];
	if (str_index == NULL)
		return(false);
	str_len   = new unsigned long[4096];
	if (str_len == NULL)
	{
		delete str_index;
		str_index = NULL;
		return(false);
	}
	num_str = 0;
	gif_clear = (unsigned short)(codesize / 2);
	gif_eof   = (unsigned short)(codesize / 2 + 1);
	gifstreamsize = cdata[s_pos];
	s_pos++;
	while ((s_pos < src_size && gifstreamsize > 0) || bituse < bitsize)
	{
		while (bituse < bitsize && s_pos < src_size)
		{
			if (bituse == 0)
				fcode = cdata[s_pos];
			else
				fcode = fcode | (cdata[s_pos] << bituse);
			bituse += 8;
			s_pos++;
			gifstreamsize--;
			if (gifstreamsize == 0 && s_pos < src_size)
			{
				gifstreamsize = cdata[s_pos];
				s_pos++;
			}
		}
		code = (unsigned short)(fcode & (codesize - 1));
		fcode >>= bitsize;
		bituse -= bitsize;
		if (code == gif_clear)
		{
			cur_str_size = 0;
			bitsize = gifcodesize + 1;
			codesize = 2;
			for (ctr = 0; ctr < bitsize - 1; ctr++)
				codesize *= 2;
			num_str = 0;
		}
		else if (code == gif_eof)
			break;
		else if (cur_str_size == 0)
		{
			if (d_pos < dst_size)
				udata[d_pos] = (unsigned char)code;
			cur_str[0]   = (unsigned char)code;
			cur_str_size = 1;
			d_pos++;
			oldcode = code;
		}
		else
		{
			if (code < num_str + initsize + 2)
			{
				if (d_pos + cur_str_size + 1 < dst_size)
				{
					AddUCmpString(udata, d_pos, cur_str_size + 1, initsize);
					GetString(udata, cur_str, cur_str_size, code, initsize);
					ClassEMemory::Copy(&udata[d_pos], cur_str, cur_str_size);
				}
				d_pos += cur_str_size;
				oldcode = code;
			}
			else
			{
				cur_str[cur_str_size] = cur_str[0];
				cur_str_size++;
				if (d_pos + cur_str_size < dst_size)
				{
					ClassEMemory::Copy(&udata[d_pos], cur_str, cur_str_size);
					AddUCmpString(udata, d_pos + cur_str_size - 1, cur_str_size, initsize);
				}
				d_pos += cur_str_size;
				oldcode = code;
			}
			if (num_str + initsize + 2 >= codesize && bitsize < 12)
			{
				codesize *= 2;
				bitsize++;
			}
		}
	}
	delete str_index;
	delete str_len;
	str_index = NULL;
	str_len = NULL;
	return(true);
}

//Search for image data
bool ELoader_GIF::SearchImageData(ELDR_2DCALLBACKINFO *cbinfo, void *phdrinfo, void *pimginfo)
{
	EGIF_HEADER *header = (EGIF_HEADER *)phdrinfo;
	EGIF_IMGINFO *imginfo = (EGIF_IMGINFO *)pimginfo;
	unsigned char dummy, segmentid, segmentsize, packedfields;
	bool segmentreading = true;
	imginfo->usegfxext = false;
	SeekSet(header->datastart);
	while (segmentreading)
	{
		Read8(&segmentid);
		if (segmentid == EGIF_EXTSEPARATOR)
		{
			unsigned long old_lptr;
			Read8(&segmentid);
			Read8(&segmentsize);
			old_lptr = Tell();
			if (segmentid == EGIF_EXTENDGFX)
			{
				unsigned char terminator;
				imginfo->usegfxext = true;
				Read8(&packedfields);
				Read16(&imginfo->gfxext.delaytime);
				Read8(&imginfo->gfxext.transparentindex);
				Read8(&terminator);
				if (terminator == 0)
				{
					imginfo->gfxext.disposalmethod = (packedfields >> 2) & 7;
					if (packedfields & 2)
						imginfo->gfxext.userinput = true;
					else
						imginfo->gfxext.userinput = false;
					if (packedfields & 1)
						imginfo->gfxext.transparency = true;
					else
						imginfo->gfxext.transparency = false;
				}
				else
					return(false);
			}
			else if (segmentid == EGIF_APPEXTENSION)
			{
				SeekSet(old_lptr + segmentsize);
				Read8(&dummy);
				segmentsize += dummy + 1;
			}
			SeekSet(old_lptr + segmentsize + 1);
		}
		else if (segmentid == EGIF_IMGSEPARATOR)
		{
			Read16(&imginfo->xofs);
			Read16(&imginfo->yofs);
			Read16(&imginfo->width);
			Read16(&imginfo->height);
			Read8(&packedfields);
			if (packedfields & 64)
				imginfo->interlaced = true;
			else
				imginfo->interlaced = false;
			if (packedfields & 128)
			{
				unsigned char paltable[MAX_EGIFPALETTE * 3];
				unsigned short ctr, bitsize = packedfields & 7;
				imginfo->localtablesize = 2;
				for (ctr = 0; ctr < bitsize; ctr++)
					imginfo->localtablesize *= 2;
				ReadStream(paltable, imginfo->localtablesize * 3);
				for (ctr = 0; ctr < imginfo->localtablesize; ctr++)
					imginfo->localtable[ctr] = E_RGBA(paltable[ctr * 3], paltable[ctr * 3 + 1], paltable[ctr * 3 + 2], 255);
			}
			else
				imginfo->localtablesize = 0;
			Read8(&imginfo->compressionsize);
			imginfo->datastart = Tell();
			Read8(&segmentsize);
			while (segmentsize > 0)
			{
				SeekRelative(segmentsize);
				segmentsize = 0;
				Read8(&segmentsize);
			}
			imginfo->imgsizecompressed = maxsize - imginfo->datastart;
			header->datastart = Tell();
			return(true);
		}
		else if (segmentid == EGIF_DUMMYCHAR)
		{
			unsigned char dummy;
			Read8(&dummy);
		}
		else
			segmentreading = false;
	}
	return(false);
}

//Decode pixel data within a GIF image
unsigned char *ELoader_GIF::DecodeImage(void *pimginfo)
{
	EGIF_IMGINFO *imginfo = (EGIF_IMGINFO *)pimginfo;
	unsigned char *memptr = NULL;
	unsigned long dst_size;
	bool result;
	str_index = NULL;
	str_len   = NULL;
	dst_size = imginfo->width * imginfo->height * 10 / 9;
	memptr = (unsigned char *)ClassEMemory::Alloc(dst_size);
	if (memptr == NULL)
		return(NULL);
	SeekSet(imginfo->datastart);
	result = UnCompress(memptr, dst_size, &lmem[lptr], imginfo->imgsizecompressed, imginfo->compressionsize);
	if (result)
		return(memptr);
	ClassEMemory::DeAlloc(memptr);
	return(NULL);
}

///////////////////////////////////////////////////////////////////
// Loader: Public                                                //
///////////////////////////////////////////////////////////////////

//Start the loader
ERESULT ELoader_GIF::Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags)
{
	#if (EDITION_SDK < 500)
		return(E_NOFEATURE);
	#else
		ERESULT result = E_OK;
		EGIF_HEADER header;
		ELDR_2DCALLBACKINFO cbinfo;
		unsigned short ctr;
		unsigned char packedfields, pixelratio;
		LinkData(ldata, lsize);
		cbinfo.nativedisplaymode = nativedisplaymode;
		cbinfo.createflags = createflags;
		cbinfo.paletteindex = 0;
		cbinfo.pixelindex = 0;
		cbinfo.customparam = 0;
		cbinfo.usetransparency = false;
		cbinfo.compressedformat = NULL;
		Read8((unsigned char *)&header.sig[0]);
		Read8((unsigned char *)&header.sig[1]);
		Read8((unsigned char *)&header.sig[2]);
		header.sig[3] = 0;
		if (header.sig[0] != 'G' || header.sig[1] != 'I' || header.sig[2] != 'F')
			return(E_UNSUPPORTED);
		Read8((unsigned char *)&header.version[0]);
		Read8((unsigned char *)&header.version[1]);
		Read8((unsigned char *)&header.version[2]);
		header.version[3] = 0;
		if (header.version[0] != '8' || header.version[1] < '7')
			return(E_UNSUPPORTED);
		Read16(&header.width);
		Read16(&header.height);
		Read8(&packedfields);
		Read8(&header.bgcolor);
		Read8(&pixelratio);
		header.globaltablesize = 0;
		cbinfo.usepalette = true;
		cbinfo.width = header.width;
		cbinfo.height = header.height;
		result = surface->LdrOnCreate(&cbinfo);
		if (result != E_OK)
			return(result);
		if (packedfields & 128)
		{
			unsigned char paltable[MAX_EGIFPALETTE * 3];
			unsigned short bitsize = packedfields & 7;
			header.globaltablesize = 2;
			for (ctr = 0; ctr < bitsize; ctr++)
				header.globaltablesize *= 2;
			ReadStream(paltable, header.globaltablesize * 3);
			for (ctr = 0; ctr < header.globaltablesize; ctr++)
				header.globaltable[ctr] = E_RGBA(paltable[ctr * 3], paltable[ctr * 3 + 1], paltable[ctr * 3 + 2], 255);
			cbinfo.streamdata = header.globaltable;
			cbinfo.streamsize = header.globaltablesize * sizeof(COLORREF);
			surface->LdrOnPaletteStream(&cbinfo);
		}
		header.datastart = Tell();
		for (ctr = 0; ctr <= cbinfo.customparam; ctr++)
		{
			EGIF_IMGINFO imginfo;
			cbinfo.paletteindex = 0;
			cbinfo.pixelindex = 0;
			if (SearchImageData(&cbinfo, &header, &imginfo))
			{
				unsigned char *imgptr;
				if (imginfo.localtablesize)
				{
					cbinfo.streamdata = imginfo.localtable;
					cbinfo.streamsize = imginfo.localtablesize * sizeof(COLORREF);
					surface->LdrOnPaletteStream(&cbinfo);
				}
				imgptr = DecodeImage(&imginfo);
				if (imgptr)
				{
					unsigned long realy, scanline;
					if (imginfo.usegfxext)
						if (imginfo.gfxext.transparency)
						{
							cbinfo.usetransparency = true;
							cbinfo.paletteindex = imginfo.gfxext.transparentindex;
							if (imginfo.localtablesize)
							{
								imginfo.localtable[imginfo.gfxext.transparentindex] &= 0xFFFFFF;
								cbinfo.streamdata = &imginfo.localtable[imginfo.gfxext.transparentindex];
							}
							else
							{
								header.globaltable[imginfo.gfxext.transparentindex] &= 0xFFFFFF;
								cbinfo.streamdata = &header.globaltable[imginfo.gfxext.transparentindex];
							}
							cbinfo.streamsize = sizeof(COLORREF);
							surface->LdrOnPaletteStream(&cbinfo);
						}
					realy = 0;
					for (scanline = 0; scanline < imginfo.height; scanline++)
					{
						if (!imginfo.interlaced)
							realy = scanline;
						cbinfo.pixelindex = (realy + imginfo.yofs) * cbinfo.width + imginfo.xofs;
						cbinfo.streamdata = &imgptr[scanline * imginfo.width];
						cbinfo.streamsize = cbinfo.width;
						result = surface->LdrOnPixelStream(&cbinfo);
						if (result != E_OK)
						{
							ClassEMemory::DeAlloc(imgptr);
							break;
						}
						if (imginfo.interlaced)
						{
							if (realy % 4 == 0)
								realy += 8;
							else if (realy % 4 == 2)
								realy += 4;
							else if (realy % 2 == 1)
								realy += 2;
							if (realy >= imginfo.height)
							{
								if (realy % 8 == 0)
									realy = 4;
								else if (realy % 8 == 4)
									realy = 2;
								else
									realy = 1;
							}
						}
					}
					ClassEMemory::DeAlloc(imgptr);
				}
				else
					return(E_ERROR);
			}
			else
				return(E_NOTEXISTS);
		}
		return(result);
	#endif
}
