// esurf_2drgba.cpp
// 2D Surface RGBA
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eraw.h"
#include "eloader.h"
#include "estdc.h"
#include "ememory.h"
#include "efile.h"
#include "edisplay.h"
#include "econsole.h"

#include "extra/verinfo.h"

///////////////////////////////////////////////////////////////////
// Internal includes                                             //
///////////////////////////////////////////////////////////////////

#include "extra/exmacro.h" //Opacity macro's

#define MAX_PALETTE  256

//Prototype
void ClassEDisplay_PixelCopy(void *dst, void *src, long dhpitch, long dvpitch, long shpitch, long svpitch, unsigned long cw, unsigned long ch, unsigned long dw, unsigned long dh, unsigned long sw, unsigned long sh, unsigned char bitdepth);
bool EdgeGL_Use(void);

///////////////////////////////////////////////////////////////////
// E2DSurfaceRGBA: private                                       //
///////////////////////////////////////////////////////////////////

//Convert a color reference to a native color
void inline E2DSurfaceRGBA::ConvertColor(COLORREF &col, unsigned char vmode)
{
	if (vmode != EDSP_INDEXED8 && vmode != EDSP_TRUE24_INV && vmode != EDSP_TRUE32_INV)
	{
		col = ((col & 0xFF0000) >> 16) | (col & 0xFF00) | ((col & 0xFF) << 16);
		if (vmode == EDSP_TRUE12)
			col = (((col & 0xFF0000) >> 12) & 3840) | (((col & 0xFF00) >> 8) & 240) | ((col & 0xFF) >> 4);
		else if (vmode == EDSP_TRUE15)
			col = (((col & 0xFF0000) >> 9) & 31744) | (((col & 0xFF00) >> 6) & 992) | ((col & 0xFF) >> 3);
		else if (vmode == EDSP_TRUE16)
			col = (((col & 0xFF0000) >> 8) & 63488) | (((col & 0xFF00) >> 5) & 2016) | ((col & 0xFF) >> 3);
	}
}

//Revert a color reference from a native color
void inline E2DSurfaceRGBA::RevertColor(COLORREF &col, unsigned char vmode)
{
	if (vmode != EDSP_INDEXED8 && vmode != EDSP_TRUE24_INV && vmode != EDSP_TRUE32_INV)
	{
		if (vmode == EDSP_TRUE12)
			col = ((col & 3840) << 12) | ((col & 240) << 8) | ((col & 15) << 4);
		else if (vmode == EDSP_TRUE15)
			col = ((col & 31744) << 9) | ((col & 992) << 6) | ((col & 31) << 3);
		else if (vmode == EDSP_TRUE16)
			col = ((col & 63488) << 8) | ((col & 2016) << 5) | ((col & 31) << 3);
		col = ((col & 0xFF0000) >> 16) | (col & 0xFF00) | ((col & 0xFF) << 16);
	}
}

//Dither a high bit color value depending on the index (for example: index = x + y)
void inline E2DSurfaceRGBA::DitherColor(COLORREF &col, unsigned long index)
{
	if (videomode == EDSP_TRUE12)
	{
		if ((col & 0xFF0000) < 0xF00000)
			col = (col & 0x00FFFF) | ((col & 0xF00000) + (edithertable16[((index & 15) << 4) + ((col >> 16) & 15)] << 16));
		if ((col & 0xFF00) < 0xF000)
			col = (col & 0xFF00FF) | ((col & 0xF000) + (edithertable16[((index & 15) << 4) + ((col >> 8) & 15)] << 8));
		if ((col & 0xFF) < 0xF0)
			col = (col & 0xFFFF00) | ((col & 0xF0) + (edithertable16[((index & 15) << 4) + (col & 15)]));
	}
	else if (videomode == EDSP_TRUE15)
	{
		if ((col & 0xFF0000) < 0xF80000)
			col = (col & 0x00FFFF) | ((col & 0xF80000) + (edithertable8[((index & 7) << 3) + ((col >> 16) & 7)] << 16));
		if ((col & 0xFF00) < 0xF800)
			col = (col & 0xFF00FF) | ((col & 0xF800) + (edithertable8[((index & 7) << 3) + ((col >> 8) & 7)] << 8));
		if ((col & 0xFF) < 0xF8)
			col = (col & 0xFFFF00) | ((col & 0xF8) + (edithertable8[((index & 7) << 3) + (col & 7)]));
	}
	else if (videomode == EDSP_TRUE16)
	{
		if ((col & 0xFF0000) < 0xF80000)
			col = (col & 0x00FFFF) | ((col & 0xF80000) + (edithertable8[((index & 7) << 3) + ((col >> 16) & 7)] << 16));
		if ((col & 0xFF00) < 0xFB00)
			col = (col & 0xFF00FF) | ((col & 0xFB00) + (edithertable4[((index & 3) << 2) + ((col >> 8) & 3)] << 8));
		if ((col & 0xFF) < 0xF8)
			col = (col & 0xFFFF00) | ((col & 0xF8) + (edithertable8[((index & 7) << 3) + (col & 7)]));
	}
}

//Get the pixel color and opacity value
void E2DSurfaceRGBA::GetPixelV(long x, long y, COLORREF &col, unsigned char &opacity)
{
	col = 0;
	opacity = 0;
	if (x < cliprc.left || y < cliprc.top || x >= cliprc.right || y >= cliprc.bottom)
		return;
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return;
	col = *(unsigned EINT32 *)(&memptr[y * swidth * 4 + x * 4]) & 0xFFFFFF;
	opacity = (unsigned char)(*(unsigned EINT32 *)(&memptr[y * swidth * 4 + x * 4]) >> 24);
	RevertColor(col, videomode);
	if (currenttype & EST_READONLY)
	{
		opacity = 255 - opacity;
		if (opacity)
			col = (((((col & 0xFF0000) << 8) / (opacity + 1)) & 0xFF0000) | ((((col & 0xFF00) << 8) / (opacity + 1)) & 0xFF00) | ((((col & 0xFF) << 8) / (opacity + 1)) & 0xFF));
		else
			col = 0;
	}
	Unlock();
}

//Link the display module to the surface
void E2DSurfaceRGBA::LinkECD(ECD *pecd)
{
	ecd = pecd;
}

//Load callback: Create surface
ERESULT E2DSurfaceRGBA::LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata)
{
	if (ldrdata->compressedformat)
	{
		Free();
		texformat = (char *)ClassEMemory::Alloc(ClassEStd::StrLen(ldrdata->compressedformat) + 1);
		if (texformat)
			ClassEStd::StrCpy(texformat, ldrdata->compressedformat);
		if (ldrdata->width > 0 && ldrdata->height > 0)
		{
			swidth = virtualwidth = ldrdata->width;
			sheight = ldrdata->height;
			gdata = (unsigned char *)ecd->mem->AllocList(ldrdata->streamsize);
			if (gdata)
			{
				currenttype = EST_COMPRESSEDTEX;
				if (!ecd->dsp->HandleSurfList(this))
				{
					Free();
					return(E_NOMEMORY);
				}
				return(E_OK);
			}
		}
		return(E_ERROR);
	}
	ERESULT er;
	er = Create(ldrdata->width, ldrdata->height, ldrdata->nativedisplaymode, ldrdata->createflags);
	return(er);
}

//Load callback: Palette stream data
ERESULT E2DSurfaceRGBA::LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata)
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

//Load callback: Pixel stream data
ERESULT E2DSurfaceRGBA::LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata)
{
	if (currenttype == EST_COMPRESSEDTEX)
	{
		ClassEMemory::Copy(&gdata[ldrdata->pixelindex], ldrdata->streamdata, ldrdata->streamsize);
		ldrdata->pixelindex += ldrdata->streamsize;
		return(E_OK);
	}
	E2DSURFACEINFO info;
	unsigned char *memptr = Lock(&info);
	memptr += info.bufferoffset;
	if (memptr != NULL)
	{
		COLORREF col, a;
		unsigned long ctr;
		if (ldrdata->usepalette)
		{
			unsigned char *rptr8 = (unsigned char *)ldrdata->streamdata;
			for (ctr = 0; ctr < ldrdata->streamsize; ctr++)
			{
				long putx = ldrdata->pixelindex % ldrdata->width, puty = ldrdata->pixelindex / ldrdata->width;
				col = palette[(*rptr8++)];
				a = E_GETA(col);
				col = E_GETRGB(col);
				if (ldrdata->createflags & EST_DITHER)
					DitherColor(col, putx + puty);
				ConvertColor(col, videomode);
				if (ldrdata->createflags & EST_READONLY)
				{
					if (videomode == EDSP_TRUE12)
						col = EBCODE_MACRO_HPACITY(col, 0, 3855, 240, 4, (a >> 4));
					else if (videomode == EDSP_TRUE16)
						col = EBCODE_MACRO_HPACITY(col, 0, 63519, 2016, 5, (a >> 3));
					else
						col = EBCODE_MACRO_HPACITY(col, 0, 0xFF00FF, 0xFF00, 8, a);
					*(unsigned EINT32 *)(&memptr[puty * info.ypitch + putx * info.xpitch]) = col | ((255 - a) << 24);
				}
				else
					*(unsigned EINT32 *)(&memptr[puty * info.ypitch + putx * info.xpitch]) = col | (a << 24);
				ldrdata->pixelindex++;
			}
		}
		else
		{
			unsigned EINT32 *rptr32 = (unsigned EINT32 *)ldrdata->streamdata;
			for (ctr = 0; ctr < ldrdata->streamsize / 4; ctr++)
			{
				long putx = ldrdata->pixelindex % ldrdata->width, puty = ldrdata->pixelindex / ldrdata->width;
				col = (*rptr32++);
				a = E_GETA(col);
				col = E_GETRGB(col);
				if (ldrdata->createflags & EST_DITHER)
					DitherColor(col, putx + puty);
				ConvertColor(col, videomode);
				if (ldrdata->createflags & EST_READONLY)
				{
					if (videomode == EDSP_TRUE12)
						col = EBCODE_MACRO_HPACITY(col, 0, 3855, 240, 4, (a >> 4));
					else if (videomode == EDSP_TRUE16)
						col = EBCODE_MACRO_HPACITY(col, 0, 63519, 2016, 5, (a >> 3));
					else
						col = EBCODE_MACRO_HPACITY(col, 0, 0xFF00FF, 0xFF00, 8, a);
					*(unsigned EINT32 *)(&memptr[puty * info.ypitch + putx * info.xpitch]) = col | ((255 - a) << 24);
				}
				else
					*(unsigned EINT32 *)(&memptr[puty * info.ypitch + putx * info.xpitch]) = col | (a << 24);
				ldrdata->pixelindex++;
			}
		}
		Unlock();
		return(E_OK);
	}
	else
		return(E_LOCKFAILED);
	return(E_ERROR);
}

//Create a new surface from the display class
ERESULT E2DSurfaceRGBA::Create(unsigned long width, unsigned long height, unsigned char passmode, unsigned long usetype)
{
	#if (EDITION_SDK < 1000)
		return(E_NOFEATURE);
	#endif
	if (palette == NULL)
	{
		palette = (COLORREF *)ClassEMemory::Alloc(MAX_PALETTE * sizeof(COLORREF));
		if (palette == NULL)
			return(E_NOMEMORY);
	}
	usetype = usetype / 16 * 16 + EST_SYSMEMORY;
	if (usetype % 16 != EST_SYSMEMORY && usetype % 16 != EST_NONE)
		return(E_INVALIDPARAM);
	ERESULT er;
	if (currenttype % 16 != EST_NONE && currenttype % 16 != EST_TEXTURE)
	{
		if (usetype == createtype && width == GetWidth() && height == GetHeight())
		{
			Clear();
			return(E_OK);
		}
		else
			Free();
	}
	createtype = usetype;
	if (currenttype % 16 != EST_NONE)
		Free();
	if (usetype % 16 == EST_NONE)
		return(E_OK);
	if (palette == NULL)
	{
		palette = (COLORREF *)ClassEMemory::Alloc(MAX_PALETTE * sizeof(COLORREF));
		if (palette == NULL)
			return(E_NOMEMORY);
	}
	if (((usetype & EST_DSPTYPE) >> 8) != EST_DEFAULT)
	{
		videomode = (unsigned char)((usetype & EST_DSPTYPE) >> 8);
		usetype = usetype / 16 * 16 + EST_SYSMEMORY;
	}
	else
		videomode = passmode;
	bitwidth = 32;
	if (ecd->dsp->switchxy)
	{
		swidth  = height;
		sheight = width;
	}
	else
	{
		swidth  = width;
		sheight = height;
	}
	virtualwidth = swidth;
	if (swidth % 4 != 0)
		swidth = swidth / 4 * 4 + 4;
	gdata = (unsigned char *)ecd->mem->AllocList(swidth * sheight * 4);
	if (gdata == NULL)
		return(E_NOMEMORY);
	Clear();
	vpitch = swidth * 4;
	er = Init();
	if (!ecd->dsp->HandleSurfList(this))
		er = E_NOMEMORY;
	if (er != E_OK)
		Free();
	currenttype = (unsigned char)(usetype & EST_FLAGTYPE);
	return(er);
}

//Create a new surface by loading an image from memory
ERESULT E2DSurfaceRGBA::LoadImage(void *imgmem, unsigned long memsize, unsigned char passmode, unsigned long usetype)
{
	ERESULT er = E_UNSUPPORTED;
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_GRAPHICSLOADERS; ctr++)
		if (ecd->dsp->graphicsloader[ctr] != NULL)
		{
			er = ecd->dsp->graphicsloader[ctr]->Start(this, (unsigned char *)imgmem, memsize, passmode, usetype);
			if (er != E_UNSUPPORTED && er != E_NOFEATURE)
				break;
			if (er == E_OK)
				break;
		}
	if (locked)
	{
		locked = 1;
		Unlock();
	}
	return(er);
}

//Create a new surface by loading an image
ERESULT E2DSurfaceRGBA::LoadImage(WCHAR *imgfile, unsigned char passmode, unsigned long usetype)
{
	ClassEFile iml;
	ERESULT er;
	unsigned char *fdata = NULL;
	unsigned long fsize;
	unsigned char opentype = EFOF_READ | EFOF_NOEPKCHECK;
	if (usetype & EST_READONLYPATH)
		opentype |= EFOF_READONLYPATH;
	if (!iml.Open(imgfile, opentype))
		return(E_NOTEXISTS);
	fsize = iml.Size();
	if (fsize == 0)
	{
		iml.Close();
		return(E_NODATA);
	}
	fdata = (unsigned char *)ecd->mem->AllocList(fsize);
	if (fdata == NULL)
	{
		iml.Close();
		return(E_NOMEMORY);
	}
	iml.Read(fdata, fsize);
	iml.Close();
	er = LoadImage(fdata, fsize, passmode, usetype);
	FULL_FREE_L(fdata);
	return(er);
}

//Initializer after creating a surface in any way
ERESULT E2DSurfaceRGBA::Init(void)
{
	SetRect(&cliprc, 0, 0, virtualwidth, sheight);
	fontinfo.charstart = 32;
	fontinfo.charend = 127;
	fontinfo.xspacing = 0;
	fontinfo.yspacing = 0;
	fontinfo.numrows = 1;
	fontinfo.rowchars = 128;
	fontinfo.defaultchar = -1;
	FULL_FREE_L(fontrc);
	FULL_FREE(fontkerning);
	xmin = (short *)ecd->mem->AllocList(sheight * 2);
	if (xmin == NULL)
		return(E_NOMEMORY);
	xmax = (short *)ecd->mem->AllocList(sheight * 2);
	if (xmax == NULL)
	{
		FULL_FREE_L(xmin);
		return(E_NOMEMORY);
	}
	ledge = (EINT32 *)ecd->mem->AllocList(sheight * 16);
	if (ledge == NULL)
	{
		FULL_FREE_L(xmax);
		FULL_FREE_L(xmin);
		return(E_NOMEMORY);
	}
	redge = (EINT32 *)ecd->mem->AllocList(sheight * 16);
	if (redge == NULL)
	{
		FULL_FREE_L(ledge);
		FULL_FREE_L(xmax);
		FULL_FREE_L(xmin);
		return(E_NOMEMORY);
	}
	xdraw = (unsigned char *)ecd->mem->AllocList(sheight);
	if (xdraw == NULL)
	{
		FULL_FREE_L(redge);
		FULL_FREE_L(ledge);
		FULL_FREE_L(xmax);
		FULL_FREE_L(xmin);
		return(E_NOMEMORY);
	}
	return(E_OK);
}

//Handle orientation changes
void E2DSurfaceRGBA::OnChangeOrientation(long *olddata)
{
	bool newxy = ecd->dsp->switchxy;
	long newoh = ecd->dsp->orienth, newov = ecd->dsp->orientv;
	ecd->dsp->orienth = olddata[0];
	ecd->dsp->orientv = olddata[1];
	if (olddata[2])
		ecd->dsp->switchxy = true;
	else
		ecd->dsp->switchxy = false;
	if (ecd->dsp->orienth == newoh && ecd->dsp->orientv == newov && ecd->dsp->switchxy == newxy)
		return;
	E2DSURFACEINFO curinfo;
	unsigned char *curptr = Lock(&curinfo);
	ecd->dsp->orienth = newoh;
	ecd->dsp->orientv = newov;
	ecd->dsp->switchxy = newxy;
	if (curptr)
	{
		E2DSurfaceRGBA *tmpnew;
		bool tocopy = false;
		tmpnew = new E2DSurfaceRGBA;
		if (tmpnew == NULL)
		{
			Free();
			return;
		}
		if (ecd->dsp->CreateSurface(tmpnew, curinfo.width, curinfo.height, currenttype % 16 | (videomode << 8) | (currenttype & EST_READONLY)) == E_OK)
		{
			E2DSURFACEINFO newinfo;
			unsigned char *newptr = tmpnew->Lock(&newinfo);
			if (newptr)
			{
				ClassEDisplay_PixelCopy(newptr + newinfo.bufferoffset, curptr + curinfo.bufferoffset, newinfo.xpitch, newinfo.ypitch, curinfo.xpitch, curinfo.ypitch, curinfo.width, curinfo.height, newinfo.realwidth, newinfo.realheight, curinfo.realwidth, curinfo.realheight, curinfo.bitwidth);
				tmpnew->Unlock();
			}
			ClassEMemory::Copy(&tmpnew->fontinfo, &fontinfo, sizeof(fontinfo));
			if (fontcharset)
				tmpnew->SetFontCharacterSet(fontcharset);
			if (fontkerning)
			{
				tmpnew->fontkerning = (short *)ClassEMemory::Alloc(fontkerning[0] * sizeof(short));
				if (tmpnew->fontkerning)
					ClassEMemory::Copy(tmpnew->fontkerning, fontkerning, fontkerning[0] * sizeof(short));
			}
			if (fontrc)
			{
				unsigned long fontrc_size = (fontrc[3] * 4 + 4) * sizeof(short);
				tmpnew->fontrc = (short *)ecd->mem->AllocList(fontrc_size);
				ClassEMemory::Copy(tmpnew->fontrc, fontrc, fontrc_size);
			}
			tocopy = true;
		}
		unsigned long ctr, thisnr = 0xFFFFFFFF;
		if (tocopy)
			for (ctr = 0; ctr < ecd->dsp->surflistsize; ctr++)
				if (ecd->dsp->surflist[ctr] == this)
				{
					thisnr = ctr;
					break;
				}
		Unlock();
		Free();
		if (thisnr == 0xFFFFFFFF)
		{
			tmpnew->Free();
			return;
		}
		if (tocopy)
		{
			unsigned long ctr;
			ClassEMemory::Copy(this, tmpnew, sizeof(E2DSurfaceRGBA));
			for (ctr = 0; ctr < ecd->dsp->surflistsize; ctr++)
				if (ecd->dsp->surflist[ctr] == tmpnew)
				{
					ecd->dsp->surflist[ctr] = NULL;
					break;
				}
			ecd->dsp->surflist[thisnr] = this;
		}
	}
	else
		Free();
}

///////////////////////////////////////////////////////////////////
// E2DSurfaceRGBA: public                                        //
///////////////////////////////////////////////////////////////////

//E2DSurfaceRGBA: constructor
E2DSurfaceRGBA::E2DSurfaceRGBA(void)
{
	currenttype = EST_NONE;
	palette  = NULL;
	ecd      = NULL;
	xmin     = NULL;
	xmax     = NULL;
	ledge    = NULL;
	redge    = NULL;
	xdraw    = NULL;
	cbitmask = NULL;
	fontcharset = NULL;
	fontrc      = NULL;
	fontkerning = NULL;
	gdata       = NULL;
	texformat   = NULL;
	locked    = 0;
	exthandle = -1;
	dirty = true;
    autorepair = false;
}

//E2DSurface: destructor
E2DSurfaceRGBA::~E2DSurfaceRGBA(void)
{
	Free();
}

//Lock this surface & return its pointer of the topleft corner pixel
unsigned char *E2DSurfaceRGBA::Lock(E2DSURFACEINFO *surfaceinfo)
{
	if (currenttype % 16 == EST_NONE || currenttype % 16 == EST_TEXTURE)
		return(NULL);
	dirty = true;
	if (gdata != NULL)
	{
		if (surfaceinfo != NULL)
			GetInfo(surfaceinfo);
		locked++;
	}
	return(gdata);
}

//Unlock this surface
void E2DSurfaceRGBA::Unlock(void)
{
	if (locked > 0)
		locked--;
}

//Free resourced used by the surface
void E2DSurfaceRGBA::Free(void)
{
	if (currenttype == EST_NONE)
		return;
	if (locked > 0)
	{
		locked = 1;
		Unlock();
	}
	if (ecd != NULL)
	{
		if (exthandle != -1)
		{
			ecd->dsp->ReleaseTextureHandle(exthandle);
			exthandle = -1;
		}
		if (currenttype % 16 == EST_SYSMEMORY || currenttype % 16 == EST_COMPRESSEDTEX)
		{
			FULL_FREE_L(gdata);
		}
		else
			gdata = NULL;
		FULL_FREE_L(fontcharset);
		FULL_FREE_L(cbitmask);
		FULL_FREE_L(fontrc);
		FULL_FREE(fontkerning);
		FULL_FREE_L(xmax);
		FULL_FREE_L(xmin);
		FULL_FREE_L(ledge);
		FULL_FREE_L(redge);
		FULL_FREE_L(xdraw);
		FULL_FREE(palette);
		FULL_FREE(texformat);
		ecd->dsp->HandleSurfList(this, false);
	}
	currenttype = EST_NONE;
	locked = 0;
    autorepair = false;
}

//Clear surface information
void E2DSurfaceRGBA::Clear(COLORREF col)
{
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return;
	if (currenttype & EST_READONLY)
		ClassEMemory::Set(memptr, 255, swidth * sheight * 4);
	else
		ClassEMemory::Set(memptr, 0, swidth * sheight * 4);
	Unlock();
	FULL_FREE_L(fontrc);
	FULL_FREE(fontkerning);
	FULL_FREE_L(cbitmask);
    autorepair = false;
}

void E2DSurfaceRGBA::Reload(ClassEDisplay *dsp)
{
    dsp->UploadTexture(this, false);
}

//Build a bit mask for pixel collision testing
void E2DSurfaceRGBA::BuildTableMask(unsigned char opacity)
{
	#if (EDITION_SDK < 500)
		return;
	#endif
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return;
	unsigned long col, xctr, yctr, bitm = 0, mwidth = (virtualwidth + 31) / 32 * 32;
	unsigned char opac;
	FULL_FREE_L(cbitmask);
	cbitmask = (unsigned long *)ecd->mem->AllocList(mwidth / 32 * 4 * sheight);
	if (cbitmask == NULL)
	{
		Unlock();
		return;
	}
	ClassEMemory::Set(cbitmask, 0, mwidth / 32 * 4 * sheight);
	for (yctr = 0; yctr < sheight; yctr++)
		for (xctr = 0; xctr < mwidth; xctr++)
		{
			GetPixelV(xctr, yctr, col, opac);
			if (opac >= opacity)
				bitm |= 1 << (31 - (xctr & 31));
			if (xctr % 32 == 31)
			{
				cbitmask[yctr * mwidth / 32 + xctr / 32] = bitm;
				bitm = 0;
			}
		}
	Unlock();
}

//Write the content of the surface to a PNG file
void E2DSurfaceRGBA::WritePNG(char *filename, RECT *rc)
{
	USTR_CHECK_BEGIN(filename);
	WritePNG(ustr_filename, rc);
	USTR_CHECK_END(filename);
}

//Write the content of the surface to a PNG file
void E2DSurfaceRGBA::WritePNG(WCHAR *filename, RECT *rc)
{
	RECT copyrc;
	ClassEFile ef;
	if (rc != NULL)
		ClassEMemory::Copy(&copyrc, rc, sizeof(RECT));
	else
		SetRect(&copyrc, 0, 0, GetWidth(), GetHeight());
	if (Lock() == NULL)
		return;
	if (!ef.New(filename))
	{
		Unlock();
		return;
	}
	ELoader_PNG ldr_png;
	unsigned long width = copyrc.right - copyrc.left, height = copyrc.bottom - copyrc.top;
	ldr_png.WriteHeader(&ef, width, height, bitwidth);
	ldr_png.WritePixelData(&ef, this, &copyrc);
	ef.Close();
	Unlock();
}

//Retrieve more information about the surface
bool E2DSurfaceRGBA::GetInfo(E2DSURFACEINFO *surfaceinfo)
{
	if (surfaceinfo != NULL)
	{
		long ofsx = 0, ofsy = 0;
		surfaceinfo->width       = GetWidth();
		surfaceinfo->height      = GetHeight();
		surfaceinfo->realwidth   = virtualwidth;
		surfaceinfo->realheight  = sheight;
		surfaceinfo->pixelformat = videomode;
		surfaceinfo->realpitch   = swidth * 4;
		surfaceinfo->bitwidth    = bitwidth;
		if (ecd->dsp->switchxy)
		{
			surfaceinfo->xpitch = surfaceinfo->realpitch * ecd->dsp->orientv;
			surfaceinfo->ypitch = surfaceinfo->bitwidth / 8 * ecd->dsp->orienth;
		}
		else
		{
			surfaceinfo->xpitch = surfaceinfo->bitwidth / 8 * ecd->dsp->orienth;
			surfaceinfo->ypitch = surfaceinfo->realpitch * ecd->dsp->orientv;
		}
		ecd->dsp->VirtualPos(ofsx, ofsy, virtualwidth, sheight);
		surfaceinfo->bufferoffset = ofsy * surfaceinfo->realpitch + ofsx * bitwidth / 8;
	}
	else
		return(false);
	return(true);
}

//Retrieve the clip rectangle
void E2DSurfaceRGBA::GetClipper(RECT *rc)
{
	if (rc != NULL)
	{
		ClassEMemory::Copy(rc, &cliprc, sizeof(RECT));
		ecd->dsp->ReVirtualRect(rc, virtualwidth, sheight);
	}
}

//Set a smaller clipper rectangle
void E2DSurfaceRGBA::SetClipper(RECT *rc)
{
	if (rc != NULL)
	{
		ecd->dsp->VirtualRect(rc, virtualwidth, sheight);
		if (rc->left < 0)
			rc->left = 0;
		if (rc->right > (long)virtualwidth)
			rc->right = virtualwidth;
		if (rc->top < 0)
			rc->top = 0;
		if (rc->bottom > (long)sheight)
			rc->bottom = sheight;
		SetRect(&cliprc, rc->left, rc->top, rc->right, rc->bottom);
	}
	else
		SetRect(&cliprc, 0, 0, virtualwidth, sheight);
}

//Replace one color with another
bool E2DSurfaceRGBA::ReplaceColor(COLORREF dst, COLORREF src, bool exclude)
{
	if (currenttype & EST_READONLY)
		return(false);
	ConvertColor(dst, videomode);
	ConvertColor(src, videomode);
	long xc, yc, w = cliprc.right - cliprc.left;
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return(false);
	unsigned EINT32 *psd = (unsigned EINT32 *)&memptr[cliprc.top * swidth * 4 + cliprc.left * 4];
	if (exclude)
	{
		for (yc = cliprc.top; yc < cliprc.bottom; yc++)
		{
			for (xc = 0; xc < w; xc++)
				if ((psd[xc] & 0xFFFFFF) != src)
					psd[xc] = (psd[xc] & 0xFF000000) | dst;
			psd += swidth;
		}
	}
	else
	{
		for (yc = cliprc.top; yc < cliprc.bottom; yc++)
		{
			for (xc = 0; xc < w; xc++)
				if ((psd[xc] & 0xFFFFFF) == src)
					psd[xc] = (psd[xc] & 0xFF000000) | dst;
			psd += swidth;
		}
	}
	Unlock();
	return(true);
}

//Build the font sheet
void E2DSurfaceRGBA::BuildFont(bool variablewidth, long autoreorder)
{
	FULL_FREE_L(fontrc);
	FULL_FREE(fontkerning);
	unsigned long sheetwidth = GetWidth(), sheetheight = GetHeight();
	unsigned long numchars = fontinfo.charend - fontinfo.charstart + 1;
	if (fontcharset)
		numchars = ClassEStd::StrLen(fontcharset);
	if (numchars == 0)
		return;
	if (fontinfo.numrows == 0)
		fontinfo.numrows = 1;
	short *fontrcptr;
	long ctr, fixedwidth = 0, fixedheight = 0;
	if (fontinfo.numrows > 0)
		fixedheight = sheetheight / fontinfo.numrows;
	if (fontinfo.numrows == 1)
		fontinfo.rowchars = numchars;
	fontrc = (short *)ecd->mem->AllocList((numchars * 4 + 4) * sizeof(short));
	if (fontrc == NULL)
		return;
	ClassEMemory::Set(fontrc, 0, (numchars * 4 + 4) * sizeof(short));
	fontrc[0] = (short)fontinfo.xspacing;
	fontrc[1] = (short)fontinfo.yspacing;
	fontrc[2] = fontinfo.charstart;
	fontrc[3] = (short)numchars;
	fontrcptr = &fontrc[4];
	if (variablewidth)
	{
		bool onnewline = true;
		unsigned long currow = 0;
		for (ctr = 0; ctr < (long)numchars; ctr++)
		{
			COLORREF dummy;
			unsigned char opac;
			if (onnewline)
				fontrcptr[0] = 0;
			else
				fontrcptr[0] = fontrcptr[-2];
			do
			{
				GetPixel(fontrcptr[0], currow * fixedheight, dummy, opac);
				if (opac < 128)
					fontrcptr[0]++;
			}
			while(opac < 128 && fontrcptr[0] < (short)sheetwidth);
			fontrcptr[2] = fontrcptr[0];
			do
			{
				GetPixel(fontrcptr[2], currow * fixedheight, dummy, opac);
				if (opac >= 128)
					fontrcptr[2]++;
			}
			while(opac >= 128 && fontrcptr[2] < (short)sheetwidth);
			if (fontrcptr[0] == fontrcptr[2])
			{
				onnewline = true;
				currow++;
				if (currow * fixedheight >= sheetheight)
					break;
				ctr--;
				continue;
			}
			else
			{
				onnewline = false;
				fontrcptr[1] = (short)(currow * fixedheight + 2);
				fontrcptr[3] = (short)(fontrcptr[1] + fixedheight - 2);
				fontrcptr += 4;
			}
		}
	}
	else
	{
		if (fontinfo.rowchars <= 0)
			fontinfo.rowchars = numchars / fontinfo.numrows;
		if (fontinfo.rowchars == 0)
			return;
		fixedwidth = sheetwidth / fontinfo.rowchars;
		for (ctr = 0; ctr < (long)numchars; ctr++)
		{
			if (ctr / fontinfo.rowchars * fixedheight >= (long)sheetheight)
				break;
			fontrcptr[0] = (short)(ctr % fontinfo.rowchars * fixedwidth);
			fontrcptr[1] = (short)(ctr / fontinfo.rowchars * fixedheight);
			fontrcptr[2] = (short)(fontrcptr[0] + fixedwidth);
			fontrcptr[3] = (short)(fontrcptr[1] + fixedheight);
			fontrcptr += 4;
		}
	}

	if (autoreorder == EFR_ALWAYS || (autoreorder == EFR_DEFAULT && EdgeGL_Use()))
	{
		long fitwidth = 1024;
		if ((unsigned)(fitwidth * fitwidth * 3 / 16) > GetWidth() * GetHeight()) fitwidth >>= 1; // if smaller square can contain font + 33%, shrink
		if ((unsigned)(fitwidth * fitwidth * 3 / 16) > GetWidth() * GetHeight()) fitwidth >>= 1;

		unsigned EINT32 * newmap = (unsigned EINT32 *) ecd->mem->Alloc(fitwidth*fitwidth*sizeof(EINT32));
		short * newfontrc = (short *)ecd->mem->AllocList((numchars * 4 + 4) * sizeof(short));
		if (newmap && newfontrc)
		{
			newfontrc[0] = fontrc[0];
			newfontrc[1] = fontrc[1];
			newfontrc[2] = fontrc[2];
			newfontrc[3] = fontrc[3];

			short * oldrcptr = &(fontrc[4]);
			short * newrcptr = &(newfontrc[4]);

			int rowstart = 0;
			int rowh = 0;
			int rowx = 0;

			unsigned int i;
			for (i = 0; i < numchars; i++)
			{
				int charw = oldrcptr[2] - oldrcptr[0];
				int charh = oldrcptr[3] - oldrcptr[1];
				if (charh > rowh)
				{
					rowh = charh;
					if (rowstart + rowh >= fitwidth) break;
				}
				if (rowx + charw >= fitwidth)
				{
					rowstart += rowh + 1;
					rowh = charh;
					rowx = 0;
					if (rowstart + rowh >= fitwidth) break;
				}
				newrcptr[0] = rowx;
				newrcptr[1] = rowstart;
				newrcptr[2] = rowx + charw;
				newrcptr[3] = rowstart + charh;

				for (int y = 0; y <= charh; y++)
				{	
					for (int x = 0; x <= charw; x++)
					{	
						unsigned long col; unsigned char opac;
						GetPixel(oldrcptr[0] + x, oldrcptr[1] + y, col, opac);
						newmap[(x + rowx) + fitwidth*(y+ rowstart)] = col | ((unsigned long) opac << 24);
					}
				}
				newrcptr = &(newrcptr[4]);
				oldrcptr = &(oldrcptr[4]);

				rowx += charw + 1;
			}
			if (i == numchars)
			{
				// update original texture
				rowstart += rowh + 1;
				Create(1024, rowstart, videomode, EST_DEFAULT);
				for (int y = 0; y < rowstart; y++)
				{
					for (int x = 0; x < fitwidth; x++)
					{
						this->PutPixel(x, y, newmap[x + fitwidth*y] & 0xffffff, (unsigned char)(newmap[x + fitwidth*y] >> 24));
					}
				}
				FULL_FREE_L(fontrc);				
				fontrc = newfontrc;
			} 
			else 
			{
				FULL_FREE_L(newfontrc);
			}
			ecd->mem->DeAlloc(newmap);
		}
		else
		{
			if (newfontrc) FULL_FREE_L(newfontrc);
			if (newmap) ecd->mem->DeAlloc(newmap);
		}

	}
}

//Change size of the surface
void E2DSurfaceRGBA::Resize(unsigned long width, unsigned long height, bool usefilter)
{
	if (height == 0 && width > 0)
		height = GetHeight() * width / GetWidth();
	else if (width == 0 && height > 0)
		width = GetWidth() * height / GetHeight();
	if (width == GetWidth() && height == GetHeight())
		return;
	if (width > GetWidth() || height > GetHeight())
		usefilter = false;
	E2DSurfaceRGBA old;
	if (ecd->dsp->CreateSurface(&old, GetWidth(), GetHeight(), EST_SYSMEMORY | (videomode << 8) | (currenttype & EST_READONLY)) == E_OK)
	{
		E2DSURFACEINFO info;
		unsigned char *memptr1, *memptr2;
		memptr1 = old.Lock(&info);
		memptr2 = Lock();
		if (memptr1 != NULL && memptr2 != NULL)
			ClassEMemory::Copy(memptr1, memptr2, info.realheight * info.realpitch);
		if (memptr1 != NULL)
			old.Unlock();
		if (memptr2 != NULL)
			Unlock();
		if (Create(width, height, videomode, currenttype % 16 | (currenttype & EST_READONLY)) == E_OK)
		{
			unsigned long x, y;
			if (usefilter)
			{
				unsigned long *utable = NULL, *vtable = NULL;
				utable = (unsigned long *)ClassEMemory::Alloc((width + 1) * sizeof(unsigned long));
				vtable = (unsigned long *)ClassEMemory::Alloc((height + 1) * sizeof(unsigned long));
				if (utable != NULL && vtable != NULL)
				{
					for (x = 0; x < width; x++)
						utable[x] = x * old.GetWidth() * 256 / width;
					utable[width] = old.GetWidth() * 256;
					for (y = 0; y < height; y++)
						vtable[y] = y * old.GetHeight() * 256 / height;
					vtable[height] = old.GetHeight() * 256;
					for (y = 0; y < height; y++)
					{
						for (x = 0; x < width; x++)
						{
							unsigned long vr = 0, vg = 0, vb = 0, va = 0;
							unsigned long v, nextv;
							v = vtable[y];
							while(v < vtable[y + 1])
							{
								unsigned long ur = 0, ug = 0, ub = 0, ua = 0;
								unsigned long u, nextu, lineworth = 256;
								if (v + 255 >= vtable[y + 1])
								{
									lineworth = vtable[y + 1] & 255;
									nextv = vtable[y + 1];
								}
								else if (v & 255)
								{
									lineworth = 256 - (v & 255);
									nextv = (v & 0xFFFFFF00) + 256;
								}
								else
									nextv = v + 256;
								u = utable[x];
								while (u < utable[x + 1])
								{
									COLORREF col;
									unsigned char alpha;
									unsigned long pixworth = 256;
									if (u + 255 >= utable[x + 1])
									{
										pixworth = utable[x + 1] & 255;
										nextu = utable[x + 1];
									}
									else if (u & 255)
									{
										pixworth = 256 - (u & 255);
										nextu = (u & 0xFFFFFF00) + 256;
									}
									else
										nextu = u + 256;
									old.GetPixel(u >> 8, v >> 8, col, alpha);
									ur += E_GETR(col) * pixworth;
									ug += E_GETG(col) * pixworth;
									ub += E_GETB(col) * pixworth;
									ua += alpha * pixworth;
									u = nextu;
								}
								ur /= utable[x + 1] - utable[x];
								ug /= utable[x + 1] - utable[x];
								ub /= utable[x + 1] - utable[x];
								ua /= utable[x + 1] - utable[x];
								vr += ur * lineworth;
								vg += ug * lineworth;
								vb += ub * lineworth;
								va += ua * lineworth;
								v = nextv;
							}
							vr /= vtable[y + 1] - vtable[y];
							vg /= vtable[y + 1] - vtable[y];
							vb /= vtable[y + 1] - vtable[y];
							va /= vtable[y + 1] - vtable[y];
							PutPixel(x, y, E_RGBX(vr, vg, vb), (unsigned char)va);
						}
					}
				}
				ClassEMemory::DeAlloc(utable);
				ClassEMemory::DeAlloc(vtable);
			}
			else
			{
				for (y = 0; y < height; y++)
					for (x = 0; x < width; x++)
					{
						COLORREF col;
						unsigned char alpha;
						old.GetPixel(x * old.GetWidth() / width, y * old.GetHeight() / height, col, alpha);
						PutPixel(x, y, col, alpha);
					}
			}
		}
		old.Free();
	}
}

//Get the pixel color and opacity value
void E2DSurfaceRGBA::GetPixel(long x, long y, COLORREF &col, unsigned char &opacity)
{
	ecd->dsp->VirtualPos(x, y, virtualwidth, sheight);
	GetPixelV(x, y, col, opacity);
}

//Put a pixel on the specified position
void E2DSurfaceRGBA::PutPixel(long x, long y, COLORREF col, unsigned char opacity)
{
	ecd->dsp->VirtualPos(x, y, virtualwidth, sheight);
	if (x < cliprc.left || y < cliprc.top || x >= cliprc.right || y >= cliprc.bottom)
		return;
	unsigned char *memptr = Lock();
	if (memptr == NULL)
		return;
	ConvertColor(col, videomode);
	*(unsigned EINT32 *)(&memptr[y * swidth * 4 + x * 4]) = col | (opacity << 24);
	Unlock();
}

// check for modifications
bool E2DSurfaceRGBA::DirtyFlush()
{
	bool olddirty = dirty;
	dirty = false;
	return olddirty;
}
