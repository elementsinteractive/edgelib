// writeico.h
// The Windows .ico file format
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Data structures
typedef struct
{
	unsigned short sig;
	unsigned short type;
	unsigned short count;
}ICOMAINHDR;

typedef struct
{
	unsigned char width;
	unsigned char height;
	unsigned char numcolors;
	unsigned char reserved;
	unsigned short planes;
	unsigned short bitcount;
	unsigned long datasize;
	unsigned long offset;
}ICOENTRY;

typedef struct
{
	unsigned long size;
	unsigned long width;
	unsigned long height;
	unsigned short planes;
	unsigned short bitcount;
	unsigned long compression;
	unsigned long imagesize;
	unsigned long unused[4];
}ICOHDR;

///////////////////////////////////////////////////////////////////
// Class definition                                              //
///////////////////////////////////////////////////////////////////

class IcoWriter : public E2DSurfaceBase
{
	private:
		ClassEFile *ef;
		unsigned short fileentry;
		long byteswritten;
		long dataoffset;
		bool paletteonce;
		unsigned char bmpscanline[1024];
		unsigned char bmpandline[256];
		ERESULT LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata);
	public:
		IcoWriter(void);
		~IcoWriter(void);
		long StartLoading(unsigned short passfileentry, ClassEFile *passef, unsigned char *fdata, unsigned long fsize);
};

///////////////////////////////////////////////////////////////////
// Functions                                                     //
///////////////////////////////////////////////////////////////////

//Nieuw bestand
bool iconewfile(char *fname)
{
	ClassEFile ef;
	char curfolder[MAX_EFILESTR];
	ef.SetCurrentFolder(_getcwd(curfolder, MAX_EFILESTR));
	if (ef.New(fname))
	{
		ICOMAINHDR hdr;
		hdr.sig = 0;
		hdr.type = 1;
		hdr.count = 0;
		if (!ef.Write(&hdr, sizeof(hdr)))
			return(false);
		ef.Close();
	}
	else
		return(false);
	return(true);
}

//Entry toevoegen
unsigned long icowriteentry(unsigned short fileentry, ClassEFile *ef, unsigned char width, unsigned char height, unsigned char colordepth)
{
	ICOHDR hdr;
	ICOENTRY entry;
	entry.bitcount = colordepth;
	entry.datasize = (width * colordepth / 8 + 3) / 4 * 4 * height + ((width + 7) / 8 + 3) / 4 * 4 * height + sizeof(hdr);
	if (colordepth == 8)
		entry.datasize += 1024;
	entry.width = width;
	entry.height = height;
	entry.numcolors = 0;
	entry.planes = 1;
	entry.reserved = 0;
	if (fileentry == 0)
		entry.offset = sizeof(ICOMAINHDR) + sizeof(ICOENTRY);
	else
		entry.offset = ef->Size();
	ef->Seek(sizeof(ICOMAINHDR) + fileentry * sizeof(ICOENTRY));
	if (!ef->Write(&entry, sizeof(ICOENTRY)))
		return(0);
	hdr.bitcount = colordepth;
	hdr.compression = 0;
	hdr.width = width;
	hdr.height = height * 2;
	hdr.imagesize = (width * colordepth / 8 + 3) / 4 * 4 * height + (width / 8 + 3) / 4 * 4 * height;
	hdr.planes = 1;
	hdr.size = sizeof(hdr);
	hdr.unused[0] = 0;
	hdr.unused[1] = 0;
	hdr.unused[2] = 0;
	hdr.unused[3] = 0;
	ef->Seek(entry.offset);
	if (!ef->Write(&hdr, sizeof(ICOHDR)))
		return(0);
	return(entry.offset + hdr.size);
}

//Data toevoegen
bool icowritedata(unsigned short fileentry, ClassEFile *ef, unsigned char *fdata, unsigned long fsize)
{
	IcoWriter writer;
	long result = writer.StartLoading(fileentry, ef, fdata, fsize);
	if (result >= 0)
		return(true);
	else
		return(false);
	return(true);
}

//Toevoegen bestand
bool icoaddfile(char *dstf, char *srcf)
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
		ICOMAINHDR hdr;
		ICOENTRY entry;
		if (!ef.Read(&hdr, sizeof(hdr)))
		{
			free(fdata);
			return(false);
		}
		if (hdr.count)
		{
			unsigned long ectr, datadiff = sizeof(entry);
			if (!ef.Read(&entry, sizeof(entry)))
			{
				free(fdata);
				return(false);
			}
			if (!ef.MoveData(entry.offset + datadiff, entry.offset, ef.Size() - entry.offset))
			{
				free(fdata);
				return(false);
			}
			for (ectr = 0; ectr < hdr.count; ectr++)
			{
				ef.Seek(sizeof(hdr) + ectr * sizeof(entry));
				if (!ef.Read(&entry, sizeof(entry)))
				{
					free(fdata);
					return(false);
				}
				entry.offset += datadiff;
				ef.Seek(sizeof(hdr) + ectr * sizeof(entry));
				if (!ef.Write(&entry, sizeof(entry)))
				{
					free(fdata);
					return(false);
				}
			}
		}
		hdr.count++;
		ef.Seek(0);
		if (!ef.Write(&hdr, sizeof(hdr)))
		{
			free(fdata);
			return(false);
		}
		if (!icowritedata(hdr.count - 1, &ef, fdata, fsize))
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
ERESULT IcoWriter::LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata)
{
	unsigned char sizeresult = sizeissupported(ldrdata->width, ldrdata->height);
	byteswritten = 0;
	if (sizeresult == 0)
		return(E_ERROR);
	else if (sizeresult == 2)
		return(E_OK);
	unsigned char cdepth = 32;
	if (ldrdata->usepalette)
		cdepth = 8;
	dataoffset = icowriteentry(fileentry, ef, (unsigned char)ldrdata->width, (unsigned char)ldrdata->height, cdepth);
	byteswritten = dataoffset;
	paletteonce = true;
	if (dataoffset == 0)
		return(E_ERROR);
	return(E_OK);
}

//Callback: palette stream
ERESULT IcoWriter::LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata)
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
ERESULT IcoWriter::LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata)
{
	if (byteswritten == 0)
		return(E_OK);
	unsigned long ctr, palctr, linepitch, bitpitch;
	unsigned char cdepth = 32;
	if (ldrdata->usepalette)
		cdepth = 8;
	if (ldrdata->usepalette && paletteonce)
	{
		for (palctr = 0; palctr < 256; palctr++)
		{
			if (E_GETRGB(palette[palctr]) == icocolorkey)
				palette[palctr] = E_RGBA(0, 0, 0, 0);
			if (E_GETA(palette[palctr]) < 128)
				palette[palctr] = E_RGBA(0, 0, 0, 0);
			palette[palctr] = E_RGBA(E_GETB(palette[palctr]), E_GETG(palette[palctr]), E_GETR(palette[palctr]), E_GETA(palette[palctr]));
		}
		ef->Seek(dataoffset);
		if (!ef->Write(palette, 1024))
			return(false);
		dataoffset += 1024;
		paletteonce = false;
	}
	linepitch = (ldrdata->width * cdepth / 8 + 3) / 4 * 4;
	bitpitch = ((ldrdata->width + 7) / 8 + 3) / 4 * 4;
	if (ldrdata->usepalette)
	{
		unsigned char *rptr8 = (unsigned char *)ldrdata->streamdata;
		for (ctr = 0; ctr < ldrdata->streamsize; ctr++)
		{
			unsigned long lineidx = (ldrdata->pixelindex + ctr) % ldrdata->width;
			unsigned long scanline = (ldrdata->pixelindex + ctr) / ldrdata->width;
			if (lineidx == 0)
			{
				memset(bmpscanline, 0, ldrdata->width);
				memset(bmpandline,  0, ldrdata->width / 8);
			}
			if (E_GETA(palette[rptr8[ctr]]) < 128)
				bmpandline[lineidx / 8] |= 1 << (7 - (lineidx % 8));
			bmpscanline[lineidx] = rptr8[ctr];
			if (lineidx == ldrdata->width - 1)
			{
				unsigned long ictr;
				if (icoinvertalpha)
					for (ictr = 0; ictr < bitpitch; ictr++)
						bmpandline[ictr] ^= 255;
				ef->Seek(dataoffset + (ldrdata->height - scanline - 1) * linepitch);
				if (!ef->Write(bmpscanline, linepitch))
					return(E_ERROR);
				ef->Seek(dataoffset + ldrdata->height * linepitch + (ldrdata->height - scanline - 1) * bitpitch);
				if (!ef->Write(bmpandline, bitpitch))
					return(E_ERROR);
			}
		}
	}
	else
	{
		unsigned long *rptr32 = (unsigned long *)ldrdata->streamdata;
		for (ctr = 0; ctr < ldrdata->streamsize / 4; ctr++)
		{
			unsigned long lineidx = (ldrdata->pixelindex + ctr) % ldrdata->width;
			unsigned long scanline = (ldrdata->pixelindex + ctr) / ldrdata->width;
			long col;
			if (lineidx == 0)
			{
				memset(bmpscanline, 0, ldrdata->width * 4);
				memset(bmpandline,  0, ldrdata->width / 8);
			}
			col = rptr32[ctr];
			if (E_GETRGB(col) == (long)icocolorkey)
				col = E_RGBA(E_GETR(col), E_GETG(col), E_GETB(col), 0);
			if (icoinvertalpha)
				col = E_RGBA(E_GETR(col), E_GETG(col), E_GETB(col), 255 - E_GETA(col));
			if (E_GETA(rptr32[ctr]) < 128)
				bmpandline[lineidx / 8] |= 1 << (7 - (ctr % 8));
			else
				bmpscanline[ctr * 4 + 3] = 0xFF;
			bmpscanline[ctr * 4] = E_GETB(col);
			bmpscanline[ctr * 4 + 1] = E_GETG(col);
			bmpscanline[ctr * 4 + 2] = E_GETR(col);
			bmpscanline[ctr * 4 + 3] = E_GETA(col);
			if (lineidx == ldrdata->width - 1)
			{
				ef->Seek(dataoffset + (ldrdata->height - scanline - 1) * linepitch);
				if (!ef->Write(bmpscanline, linepitch))
					return(E_ERROR);
				ef->Seek(dataoffset + ldrdata->height * linepitch + (ldrdata->height - scanline - 1) * bitpitch);
				if (!ef->Write(bmpandline, bitpitch))
					return(false);
			}
		}
	}
	return(E_OK);
}

//Start loading of image
long IcoWriter::StartLoading(unsigned short passfileentry, ClassEFile *passef, unsigned char *fdata, unsigned long fsize)
{
	ClassELoaderBase *graphicsloader[MAX_GFXLOADER];
	ERESULT er = E_UNSUPPORTED;
	unsigned char ctr, ldridx = 0;
	byteswritten = 0;
	for (ctr = 0; ctr < MAX_GFXLOADER; ctr++)
		graphicsloader[ctr] = NULL;
	ef = passef;
	fileentry = passfileentry;
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
		return(byteswritten);
	return(-1);
}

//Constructor
IcoWriter::IcoWriter(void)
{
	palette = (COLORREF *)ClassEMemory::Alloc(MAX_PALETTE * sizeof(COLORREF));
}

//Destructor
IcoWriter::~IcoWriter(void)
{
	ClassEMemory::DeAlloc(palette);
}
