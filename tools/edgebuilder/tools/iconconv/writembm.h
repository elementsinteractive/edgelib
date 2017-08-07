// writembm.h
// The Symbian .mbm file format
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Limieten
#define MAX_MBMFILES 32

//Data structures
typedef struct
{
	unsigned long uid1;
	unsigned long uid2;
	unsigned long uid3;
	unsigned long checksum;
	unsigned long traileroffset;
}MBMHDR;

typedef struct
{
	unsigned long count;
	unsigned long fileindex[MAX_MBMFILES];
}MBMTRAILER;

typedef struct
{
	unsigned long entrysize;
	unsigned long hdrsize;
	unsigned long width;
	unsigned long height;
	unsigned long twipsx;
	unsigned long twipsy;
	unsigned long bitcount;
	unsigned long colored;
	unsigned long palettesize;
	unsigned long compression;
}MBMENTRY;

///////////////////////////////////////////////////////////////////
// Class definition                                              //
///////////////////////////////////////////////////////////////////

class MbmWriter : public E2DSurfaceBase
{
	private:
		ClassEFile *ef;
		unsigned long fileoffset;
		long byteswritten;
		long dataoffset;
		unsigned char bmpscanline[1024];
		bool alphalayer;
		ERESULT LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata);
	public:
		MbmWriter(void);
		~MbmWriter(void);
		long StartLoading(unsigned long passfileoffset, ClassEFile *passef, unsigned char *fdata, unsigned long fsize, bool passalphalayer);
};

///////////////////////////////////////////////////////////////////
// Functions                                                     //
///////////////////////////////////////////////////////////////////

//Nieuw bestand
bool mbmnewfile(char *fname)
{
	ClassEFile ef;
	char curfolder[MAX_EFILESTR];
	ef.SetCurrentFolder(_getcwd(curfolder, MAX_EFILESTR));
	if (ef.New(fname))
	{
		MBMHDR hdr;
		MBMTRAILER trl;
		unsigned char ctr;
		hdr.uid1 = 0x10000037;
		hdr.uid2 = 0x10000042;
		hdr.uid3 = 0;
		hdr.checksum = 0x47396439;
		hdr.traileroffset = sizeof(hdr);
		trl.count = 0;
		for (ctr = 0; ctr < MAX_MBMFILES; ctr++)
			trl.fileindex[ctr] = sizeof(hdr);
		if (!ef.Write(&hdr, sizeof(hdr)))
			return(false);
		if (!ef.Write(&trl, sizeof(4)))
			return(false);
		ef.Close();
	}
	else
		return(false);
	return(true);
}

//Entry toevoegen
unsigned long mbmwriteentry(unsigned long fileoffset, ClassEFile *ef, unsigned char width, unsigned char height, unsigned char colordepth)
{
	MBMENTRY entry;
	entry.bitcount = colordepth;
	entry.colored = 1;
	entry.compression = 0;
	entry.hdrsize = sizeof(entry);
	entry.width = width;
	entry.height = height;
	entry.twipsx = 0;//width * 3 / 2;
	entry.twipsy = 0;//height * 3 / 2;
	if (colordepth == 8)
		entry.palettesize = 256;
	else
		entry.palettesize = 0;
	entry.entrysize = entry.hdrsize + entry.palettesize * 4 + (width * ((colordepth + 7) / 8) + 3) / 4 * 4 * height;
	ef->Seek(fileoffset);
	if (!ef->Write(&entry, sizeof(entry)))
		return(0);
	return(entry.entrysize);
}

//Data toevoegen
long mbmwritedata(unsigned long fileoffset, ClassEFile *ef, unsigned char *fdata, unsigned long fsize, bool alphalayer)
{
	MbmWriter writer;
	return(writer.StartLoading(fileoffset, ef, fdata, fsize, alphalayer));
}

//Toevoegen bestand
bool mbmaddfile(char *dstf, char *srcf)
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
		MBMHDR hdr;
		MBMTRAILER trl;
		unsigned long datawritten, alphctr;
		for (alphctr = 0; alphctr < 2; alphctr++)
		{
			ef.Seek(0);
			if (!ef.Read(&hdr, sizeof(hdr)))
			{
				free(fdata);
				return(false);
			}
			ef.Seek(hdr.traileroffset);
			if (!ef.Read(&trl.count, 4))
			{
				free(fdata);
				return(false);
			}
			if (trl.count)
				if (!ef.Read(trl.fileindex, trl.count * 4))
				{
					free(fdata);
					return(false);
				}
			if (trl.count >= MAX_MBMFILES)
			{
				free(fdata);
				return(false);
			}
			trl.fileindex[trl.count] = hdr.traileroffset;
			datawritten = mbmwritedata(trl.fileindex[trl.count], &ef, fdata, fsize, alphctr == 1);
			if (datawritten == 0 || datawritten == -1)
			{
				free(fdata);
				if (datawritten == 0)
					return(false);
				else
					return(true);
			}
			hdr.traileroffset += datawritten;
			trl.count++;
			ef.Seek(0);
			if (!ef.Write(&hdr, sizeof(hdr)))
			{
				free(fdata);
				return(false);
			}
			ef.Seek(hdr.traileroffset);
			if (!ef.Write(&trl, 4 + trl.count * 4))
			{
				free(fdata);
				return(false);
			}
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
ERESULT MbmWriter::LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata)
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
	dataoffset = mbmwriteentry(fileoffset, ef, (unsigned char)ldrdata->width, (unsigned char)ldrdata->width, 12);
	if (dataoffset == 0)
		return(E_ERROR);
	byteswritten = dataoffset;
	fileoffset += sizeof(MBMENTRY);
	if (dataoffset == 0)
		return(E_ERROR);
	return(E_OK);
}

//Callback: palette stream
ERESULT MbmWriter::LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata)
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
ERESULT MbmWriter::LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata)
{
	if (byteswritten == 0)
		return(E_OK);
	unsigned long linepitch = (ldrdata->width * 2 + 3) / 4 * 4;
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
			memset(bmpscanline, 0, linepitch);
		if (ldrdata->usepalette)
			col = palette[rptr8[ctr]];
		else
			col = rptr32[ctr];
		if (alphalayer)
		{
			if (E_GETRGB(col) == (long)E_GETRGB(icocolorkey))
				col = 0;
			else
				col = (E_GETA(col) >> 4) | ((E_GETA(col) >> 4) << 4) | ((E_GETA(col) >> 4) << 8);
			if (!icoinvertalpha)
				col ^= 0xFFF;
		}
		else
		{
			if (E_GETRGB(col) == (long)E_GETRGB(icocolorkey))
				col = 0;
			col = ((col & 0xF00000) >> 20) | ((col & 0xF000) >> 8) | ((col & 0xF0) << 4);
		}
		if (ctr % 2 == 0)
		{
			bmpscanline[ctr * 2] = (unsigned char)(col & 0xFF);
			bmpscanline[ctr * 2 + 1] = (unsigned char)(col >> 8);
		}
		else
		{
			bmpscanline[ctr * 2] = (unsigned char)(col & 0xFF);
			bmpscanline[ctr * 2 + 1] = (unsigned char)(col >> 8);
		}
		if (lineidx == ldrdata->width - 1)
		{
			ef->Seek(fileoffset + scanline * linepitch);
			if (!ef->Write(bmpscanline, linepitch))
				return(E_ERROR);
		}
	}
	return(E_OK);
}

//Start loading of image
long MbmWriter::StartLoading(unsigned long passfileoffset, ClassEFile *passef, unsigned char *fdata, unsigned long fsize, bool passalphalayer)
{
	ClassELoaderBase *graphicsloader[MAX_GFXLOADER];
	ERESULT er = E_UNSUPPORTED;
	unsigned char ctr, ldridx = 0;
	byteswritten = 0;
	for (ctr = 0; ctr < MAX_GFXLOADER; ctr++)
		graphicsloader[ctr] = NULL;
	ef = passef;
	fileoffset = passfileoffset;
	alphalayer = passalphalayer;
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
MbmWriter::MbmWriter(void)
{
	palette = (COLORREF *)ClassEMemory::Alloc(MAX_PALETTE * sizeof(COLORREF));
}

//Destructor
MbmWriter::~MbmWriter(void)
{
	ClassEMemory::DeAlloc(palette);
}
