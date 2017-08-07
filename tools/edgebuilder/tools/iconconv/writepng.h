// writepng.h
// The .png file format
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Type definitions                                              //
///////////////////////////////////////////////////////////////////

#define EPNG_SIGNATURE    0x89504E47
#define EPNG_IDENTIFIER   0x0D0A1A0A
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

#define BYTE_REVERSE(r) (((r & 0xFF) << 24) | ((r & 0xFF00) << 8) | ((r & 0xFF0000) >> 8) | ((r & 0xFF000000) >> 24))

///////////////////////////////////////////////////////////////////
// Class definition                                              //
///////////////////////////////////////////////////////////////////

class PngWriter : public E2DSurfaceBase
{
	private:
		ClassEFile *ef;
		long dataoffset;
		unsigned long lpitch;
		unsigned long pngheight;
		unsigned char *saveptr;
		ERESULT LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata);
	public:
		PngWriter(void);
		~PngWriter(void);
		long StartLoading(ClassEFile *passef, unsigned char *fdata, unsigned long fsize);
};

///////////////////////////////////////////////////////////////////
// Functions                                                     //
///////////////////////////////////////////////////////////////////

//Imported functions
void ClassEStd_MakeCRCTable(unsigned long *table);
unsigned long ClassEStd_UpdateCRC(unsigned long *table, unsigned long crc, unsigned char *buf, int len);
unsigned long ELoader_PNG_CalculateCRC(unsigned long *table, unsigned char *buf, int len);
unsigned long ELoader_PNG_Linesize(unsigned long width, unsigned char bitdepth, bool usepalette, bool usecolor, bool usealpha);

//Nieuw bestand
bool pngnewfile(char *fname)
{
	ClassEFile ef;
	char curfolder[MAX_EFILESTR];
	ef.SetCurrentFolder(_getcwd(curfolder, MAX_EFILESTR));
	if (ef.New(fname))
	{
		unsigned long ctr;
		unsigned long identifier[] = { EPNG_SIGNATURE, EPNG_IDENTIFIER };
		for (ctr = 0; ctr < sizeof(identifier) / sizeof(unsigned long); ctr++)
			identifier[ctr] = BYTE_REVERSE(identifier[ctr]);
		if (!ef.Write(&identifier, sizeof(identifier)))
			return(false);
	}
	else
		return(false);
	return(true);
}

//Entry toevoegen
unsigned long pngwriteentry(ClassEFile *ef, unsigned char width, unsigned char height)
{
	unsigned long crctable[256];
	unsigned long firsthdr[] = { 13, EPNG_IHDR, width, height };
	unsigned char secondhdr[] = { 8, 6, 0, 0, 0 };
	unsigned char hdrmem[sizeof(firsthdr) + sizeof(secondhdr)];
	unsigned long ctr;
	for (ctr = 0; ctr < sizeof(firsthdr) / sizeof(unsigned long); ctr++)
		firsthdr[ctr] = BYTE_REVERSE(firsthdr[ctr]);
	ClassEMemory::Copy(hdrmem, firsthdr, sizeof(firsthdr));
	ClassEMemory::Copy(&hdrmem[sizeof(firsthdr)], secondhdr, sizeof(secondhdr));
	ClassEStd_MakeCRCTable(crctable);
	ef->Write(hdrmem, sizeof(hdrmem));
	unsigned long hdrcrc = ELoader_PNG_CalculateCRC(crctable, &hdrmem[4], sizeof(hdrmem) - 4);
	hdrcrc = BYTE_REVERSE(hdrcrc);
	ef->Write(&hdrcrc, sizeof(unsigned long));
	return(ef->Tell());
}

//Data toevoegen
bool pngwritedata(ClassEFile *ef, unsigned char *fdata, unsigned long fsize)
{
	PngWriter writer;
	long result = writer.StartLoading(ef, fdata, fsize);
	if (result >= 0)
		return(true);
	else
		return(false);
	return(true);
}

//Toevoegen bestand
bool pngaddfile(char *dstf, char *srcf)
{
	ClassEFile ef;
	char curfolder[MAX_EFILESTR];
	unsigned char *fdata = NULL;
	unsigned long fsize;
	ef.SetCurrentFolder(_getcwd(curfolder, MAX_EFILESTR));
	if (ef.Open(srcf))
	{
		fsize = ef.Size();
		if (fsize == 0)
			return(false);
		fdata = (unsigned char *)malloc(fsize);
		if (fdata == NULL)
			return(false);
		if (!ef.Read(fdata, fsize))
		{
			free(fdata);
			return(false);
		}
		ef.Close();
	}
	else
		return(false);
	if (ef.Open(dstf))
	{
		unsigned long hdrmem[2];
		if (ef.Size() != 8)
			return(false);
		ef.Read(hdrmem, 8);
		if (BYTE_REVERSE(hdrmem[0]) != EPNG_SIGNATURE)
			return(false);
		if (BYTE_REVERSE(hdrmem[1]) != EPNG_IDENTIFIER)
			return(false);
		ef.Seek(8);
		if (!pngwritedata(&ef, fdata, fsize))
		{
			free(fdata);
			return(false);
		}
		ef.Close();
	}
	else
	{
		free(fdata);
		return(false);
	}
	free(fdata);
	return(true);
}

///////////////////////////////////////////////////////////////////
// Class implementation                                          //
///////////////////////////////////////////////////////////////////

//Callback: create
ERESULT PngWriter::LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata)
{
	unsigned char sizeresult = sizeissupported(ldrdata->width, ldrdata->height);
	if (sizeresult == 0)
		return(E_ERROR);
	else if (sizeresult == 2)
		return(E_OK);
	dataoffset = pngwriteentry(ef, (unsigned char)ldrdata->width, (unsigned char)ldrdata->height);
	if (dataoffset == 0)
		return(E_ERROR);
	lpitch = ELoader_PNG_Linesize(ldrdata->width, 8, false, true, true);
	saveptr = (unsigned char *)ClassEMemory::Alloc(lpitch * ldrdata->height);
	if (saveptr == NULL)
		return(E_NOMEMORY);
	pngheight = ldrdata->height;
	return(E_OK);
}

//Callback: palette stream
ERESULT PngWriter::LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata)
{
	if (ldrdata->paletteindex >= MAX_PALETTE)
		return(E_NOACTION);
	unsigned long tocopy = ldrdata->streamsize / sizeof(COLORREF);
	if (ldrdata->paletteindex + tocopy > MAX_PALETTE)
		tocopy = MAX_PALETTE - ldrdata->paletteindex;
	ClassEMemory::Copy(&palette[ldrdata->paletteindex], ldrdata->streamdata, tocopy * sizeof(COLORREF));
	ldrdata->paletteindex += tocopy;
	return(E_OK);
}

//Callback: pixel stream
ERESULT PngWriter::LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata)
{
	if (saveptr == NULL)
		return(E_OK);
	unsigned long ctr, maxctr;
	unsigned char *rptr8 = (unsigned char *)ldrdata->streamdata;
	unsigned long *rptr32 = (unsigned long *)ldrdata->streamdata;
	if (ldrdata->usepalette)
		maxctr = ldrdata->streamsize;
	else
		maxctr = ldrdata->streamsize / 4;
	for (ctr = 0; ctr < maxctr; ctr++)
	{
		unsigned long lineidx = (ldrdata->pixelindex + ctr) % ldrdata->width;
		unsigned long scanline = (ldrdata->pixelindex + ctr) / ldrdata->width;
		long col;
		if (lineidx == 0)
			saveptr[scanline * lpitch] = EPNG_FLTRNONE;
		if (ldrdata->usepalette)
			col = palette[rptr8[ctr]];
		else
			col = rptr32[ctr];
		if (E_GETRGB(col) == (long)icocolorkey)
			col = E_RGBA(0, 0, 0, 0);
		saveptr[scanline * lpitch + 1 + lineidx * 4] = (unsigned char)E_GETR(col);
		saveptr[scanline * lpitch + 1 + lineidx * 4 + 1] = (unsigned char)E_GETG(col);
		saveptr[scanline * lpitch + 1 + lineidx * 4 + 2] = (unsigned char)E_GETB(col);
		saveptr[scanline * lpitch + 1 + lineidx * 4 + 3] = (unsigned char)E_GETA(col);
	}
	return(E_OK);
}

//Start loading of image
long PngWriter::StartLoading(ClassEFile *passef, unsigned char *fdata, unsigned long fsize)
{
	ClassELoaderBase *graphicsloader[MAX_GFXLOADER];
	ERESULT er = E_UNSUPPORTED;
	unsigned char ctr, ldridx = 0;
	for (ctr = 0; ctr < MAX_GFXLOADER; ctr++)
		graphicsloader[ctr] = NULL;
	ef = passef;
	ELoader_BMP *bmp = new ELoader_BMP;
	if (bmp != NULL)
		graphicsloader[ldridx++] = bmp;
	ELoader_GIF *gif = new ELoader_GIF;
	if (gif != NULL)
		graphicsloader[ldridx++] = gif;
	ELoader_PNG *png = new ELoader_PNG;
	if (png != NULL)
		graphicsloader[ldridx++] = png;
	ELoader_JPG *jpg = new ELoader_JPG;
	if (jpg != NULL)
		graphicsloader[ldridx++] = jpg;
	ELoader_TGA *tga = new ELoader_TGA;
	if (tga != NULL)
		graphicsloader[ldridx++] = tga;
	saveptr = NULL;
	for (ctr = 0; ctr < ldridx; ctr++)
	{
		er = graphicsloader[ctr]->Start(this, (unsigned char *)fdata, fsize, EDSP_TRUE32, 0);
		if (er != E_UNSUPPORTED && er != E_NOFEATURE)
			break;
	}
	for (ctr = 0; ctr < ldridx; ctr++)
		if (graphicsloader[ctr] != NULL)
			delete graphicsloader[ctr];
	if (er == E_OK)
	{
		if (saveptr != NULL)
		{
			unsigned long crctable[256];
			unsigned char *cmpmem = (unsigned char *)ClassEMemory::Alloc((lpitch * pngheight) * 110 / 100 + 12);
			ClassEStd_MakeCRCTable(crctable);
			if (cmpmem != NULL)
			{
				unsigned long compressedsize = (lpitch * pngheight) * 110 / 100 + 12;
				ClassEMemory::CompressZLib(cmpmem, compressedsize, saveptr, lpitch * pngheight);
				unsigned long datablock[] = { compressedsize, EPNG_IDAT };
				for (ctr = 0; ctr < sizeof(datablock) / sizeof(unsigned long); ctr++)
					datablock[ctr] = BYTE_REVERSE(datablock[ctr]);
				ef->Write(datablock, sizeof(datablock));
				ef->Write(cmpmem, compressedsize);
				unsigned long datcrc;
				datcrc = ClassEStd_UpdateCRC(crctable, 0xffffffffL, (unsigned char *)&datablock[1], sizeof(datablock) - 4);
				datcrc = ClassEStd_UpdateCRC(crctable, datcrc, cmpmem, compressedsize) ^ 0xffffffffL;
				datcrc = ((datcrc & 0xFF) << 24) | ((datcrc & 0xFF00) << 8) | ((datcrc & 0xFF0000) >> 8) | ((datcrc & 0xFF000000) >> 24);
				ef->Write(&datcrc, sizeof(unsigned long));
				unsigned long endblock[] = { 0, BYTE_REVERSE(0x49454E44), BYTE_REVERSE(0xAE426082) };
				ef->Write(endblock, sizeof(endblock));
				ClassEMemory::DeAlloc(cmpmem);
			}
			ClassEMemory::DeAlloc(saveptr);
		}
		return(1);
	}
	if (saveptr != NULL)
		ClassEMemory::DeAlloc(saveptr);
	return(-1);
}

//Constructor
PngWriter::PngWriter(void)
{
	palette = (COLORREF *)ClassEMemory::Alloc(MAX_PALETTE * sizeof(COLORREF));
}

//Destructor
PngWriter::~PngWriter(void)
{
	ClassEMemory::DeAlloc(palette);
}
