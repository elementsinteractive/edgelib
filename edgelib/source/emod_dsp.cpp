// emod_dsp.cpp
// Display class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eraw.h"
#include "eloader.h"
#include "estdc.h"
#include "ereg.h"
#include "emath.h"
#include "ememory.h"
#include "efile.h"
#include "eini.h"
#include "evtimer.h"
#include "edisplay.h"
#include "esound.h"
#include "einput.h"
#include "econnect.h"
#include "edevice.h"
#include "econsole.h"
#include "edgeframe.h"

#include "core/edgecore.h"
#include "extra/exfonthdr.h"
#include "extra/verinfo.h"

#define EDSP_SIZEFONTCOMPRESSED 1354
#define EDSP_SIZEFONTRAW        5292
#define EDSP_SIZEFONTWIDTH       126
#define EDSP_SIZEFONTHEIGHT       84
#define EDSP_OBJLISTINCSIZE       32

#define ERDR_NUMFUNCTIONS          2

#define ERDRF_BLTFAST              0
#define ERDRF_BLTFLIP              1

void ERender_Get_BltFast8(void **ptr);
void ERender_Get_BltFast12(void **ptr);
void ERender_Get_BltFast16(void **ptr);
void ERender_Get_BltFast32(void **ptr);

void ERender_Get_BltFlip8(void **ptr);
void ERender_Get_BltFlip12(void **ptr);
void ERender_Get_BltFlip16(void **ptr);
void ERender_Get_BltFlip32(void **ptr);

//Structure containing userdata for font draw callback
typedef struct
{
	ClassEDisplay *dest;
	E2DSurfaceBase *source;
	unsigned long flags;
	unsigned long property;
	bool srcrgba;
}FONTDRAW_USERDATA_DISPLAY;

/////////////////////////////////////////////////////////////////////
// Function prototypes                                             //
/////////////////////////////////////////////////////////////////////

//Open, close
bool EdgeGL_Use(void);
bool EdgeGL_FixedPoint(void);
void *EdgeGL_NewDriver(void);
void EdgeGL_FreeDriver(void *driver);
ERESULT EdgeGL_Open(void *driver, void *dhandle, unsigned char preferredmode, unsigned long width, unsigned long height, bool fullscreen, bool stencil);
void EdgeGL_Close(void *driver);
void EdgeGL_UpdateDisplay(void *driver);
void EdgeGL_Suspend(void *driver);
void EdgeGL_Resume(void *driver);
void EdgeGL_Repair(void * driver);

//Update display
void EdgeGL_Flip(void *driver);

//Status/info
unsigned char EdgeGL_GetVideoType(void *driver);
void EdgeGL_GetVideoSize(void *driver, unsigned long &width, unsigned long &height);
unsigned char EdgeGL_GetStencilBits(void *driver);

//Texture manipulation
unsigned long EdgeGL_GenTexture(void *driver);
void EdgeGL_DeleteTexture(void *driver, unsigned long texid);
void EdgeGL_SelectTexture(void *driver, unsigned long texid);
ERESULT EdgeGL_UpdateTextureData(void *driver, unsigned long texid, unsigned long width, unsigned long height, RECT *rc, unsigned char colformat, unsigned char pixformat, void *ptr, bool updatepixels);
ERESULT EdgeGL_UpdateCompressedTexture(void *driver, unsigned long texid, unsigned long width, unsigned long height, RECT *rc, char *formatstr, void *ptr, bool updatepixels);

//Lighting
void EdgeGL_EnableLight(void *driver, unsigned char nr, bool enable);
void EdgeGL_LightPosition(void *driver, unsigned char nr, long x, long y, long z);
void EdgeGL_LightIntensity(void *driver, unsigned char nr, COLORREF ambient, COLORREF diffuse, COLORREF specular);

//Change state
void EdgeGL_SetColor(void *driver, COLORREF col, unsigned char type);
void EdgeGL_SetShading(void *driver, unsigned char type);
void EdgeGL_SetTextureBlending(void *driver, unsigned char blendtype, unsigned char value);
void EdgeGL_UseTexturing(void *driver, bool enable);
void EdgeGL_Ortho(void *driver, long *coords);
void EdgeGL_Perspective(void *driver, long *coords);
void EdgeGL_PerspectiveMatrix(void *driver, void *matrix);
void EdgeGL_SetOrientation(void * driver, long orientation);
void EdgeGL_Scissor(void *driver, long *rect);

//Matrix mutation
void EdgeGL_IdentityMatrix(void *driver);
void EdgeGL_PushMatrix(void *driver);
void EdgeGL_PopMatrix(void *driver);
void EdgeGL_MultiplyMatrix(void *driver, void *matrix);

//Render functions
void EdgeGL_Clear(void *driver, bool colorbit, bool depthbit);
void EdgeGL_RenderModel(void *driver, void **arrays, long *strides, unsigned long count, bool wireframe, bool strip);

//Other prototypes
void E3DSurface_Perspective_exp(long *matrix, long &dx, long &dy, long vx, long vy, long vz, long dispwidth, long dispheight);
void E3DSurface_Normalize_exp(long &vx, long &vy, long &vz, long prec);
void ExBltColorConvert(unsigned char *dst, unsigned char *src, long dstwidth, long srcwidth, unsigned char dstvideo, unsigned char srcvideo, long copywidth, long copyheight, COLORREF *paldata, unsigned long *realpal);

#if defined(DEVICE_ALP)
	int edgerender_getlibid(void){ return(0); } //temporary solution to develop for ALP (no libraries possible yet...)
#else
	int edgerender_getlibid(void);
#endif

///////////////////////////////////////////////////////////////////
// ClassEDisplay: pure functions                                 //
///////////////////////////////////////////////////////////////////

//Create the internal font
ERESULT EDisplay_CreateInternalFont(ClassEDisplay *dsp, E2DSurface *surf, ClassEMemory *mem, unsigned long usetype, bool palette)
{
	ERESULT er;
	unsigned char *fontrawptr = NULL;
	fontrawptr = (unsigned char *)ClassEMemory::Alloc(EDSP_SIZEFONTRAW);
	if (fontrawptr == NULL)
		return(E_NOMEMORY);
	er = dsp->CreateSurface(surf, EDSP_SIZEFONTWIDTH, EDSP_SIZEFONTHEIGHT, usetype);
	if (er == E_OK)
	{
		unsigned long dst_size = EDSP_SIZEFONTRAW;
		if (mem->UnCompress(fontrawptr, dst_size, (void *)einternalfont, EDSP_SIZEFONTCOMPRESSED))
		{
			if (dst_size == EDSP_SIZEFONTRAW)
			{
				unsigned long xc, yc, pitch = (EDSP_SIZEFONTWIDTH + 1) / 2;
				for (yc = 0; yc < EDSP_SIZEFONTHEIGHT; yc++)
					for (xc = 0; xc < EDSP_SIZEFONTWIDTH; xc++)
					{
						COLORREF putcol;
						unsigned char pixcol = fontrawptr[yc * pitch + xc / 2];
						if (xc % 2 == 0)
							pixcol >>= 4;
						pixcol &= 15;
						if (palette)
						{
							putcol = 0;
							if (pixcol == 1)
								putcol = 255;
							else if (pixcol != 0)
								putcol = 1;
						}
						else
						{
							putcol = ECOLOR_FUCHSIA;
							if (pixcol == 1)
								putcol = ECOLOR_WHITE;
							else if (pixcol == 2)
								putcol = ECOLOR_BLACK;
							else if (pixcol == 3)
								putcol = E_RGBX(128, 0, 128);
						}
						surf->PutPixel(xc, yc, putcol);
					}
			}
			else
				er = E_ERROR;
		}
		else
			er = E_ERROR;
	}
	ClassEMemory::DeAlloc(fontrawptr);
	return(er);
}

//Create a palette lookup table
void EDisplay_CreatePaletteLookup(COLORREF *paldata, unsigned char *lookup, COLORREF *srcpalette = NULL, unsigned short srcpalsize = 256)
{
	COLORREF sortpal[256];
	COLORREF srcpal[256];
	short sortcolstr[256];
	unsigned char sortpalindex[256];
	unsigned char colstrstart[768];
	long ctr, subctr, pos;
	unsigned char numpal = 0;
	bool sorted, foundpal;
	for (ctr = 1; ctr < 256; ctr++)
	{
		foundpal = false;
		for (subctr = 0; subctr < numpal; subctr++)
			if (sortpal[subctr] == paldata[ctr])
			{
				foundpal = true;
				break;
			}
		if (foundpal)
			continue;
		sortpal[numpal] = paldata[ctr];
		sortpalindex[numpal] = (unsigned char)ctr;
		numpal++;
	}
	for (ctr = 0; ctr < numpal; ctr++)
	{
		sortcolstr[ctr] = (short)(((sortpal[ctr] & 0xFF0000) >> 16) + ((sortpal[ctr] & 0xFF00) >> 8) + (sortpal[ctr] & 0xFF)) >> 3;
		sortpal[ctr] = ((sortpal[ctr] & 0xF8) >> 3) + ((sortpal[ctr] & 0xF800) >> 6) + ((sortpal[ctr] & 0xF80000) >> 9);
	}
	if (srcpalette != NULL)
		for (ctr = 0; ctr < srcpalsize; ctr++)
			srcpal[ctr] = ((srcpalette[ctr] & 0xF8) >> 3) + ((srcpalette[ctr] & 0xF800) >> 6) + ((srcpalette[ctr] & 0xF80000) >> 9);
	do
	{
		for (ctr = 0; ctr < numpal - 1; ctr++)
			if (sortcolstr[ctr] > sortcolstr[ctr + 1])
			{
				COLORREF tmpcol = sortpal[ctr];
				unsigned short tmpstr = sortcolstr[ctr];
				unsigned char tmpindex = sortpalindex[ctr];
				sortcolstr[ctr] = sortcolstr[ctr + 1];
				sortpal[ctr] = sortpal[ctr + 1];
				sortpalindex[ctr] = sortpalindex[ctr + 1];
				sortcolstr[ctr + 1] = tmpstr;
				sortpal[ctr + 1] = tmpcol;
				sortpalindex[ctr + 1] = tmpindex;
				sorted = false;
			}
		sorted = true;
	}
	while(!sorted);
	pos = 0;
	for (ctr = 0; ctr < 768; ctr++)
	{
		for (subctr = pos; subctr < numpal; subctr++)
		{
			if (ClassEMath::Abs(ctr - sortcolstr[subctr]) < ClassEMath::Abs(ctr - sortcolstr[pos]))
				pos = subctr;
		}
		colstrstart[ctr] = (unsigned char)pos;
	}
	if (srcpalette == NULL)
	{
		for (ctr = 0; ctr < 32768; ctr++)
		{
			unsigned long lastcoldiff = 0xFFFFFF;
			//long loopstart = (((ctr & 31) + ((ctr & 992) >> 5) + ((ctr & 31744) >> 10))) - 16;
			//if (loopstart < 0)
			//	loopstart = 0;
			//long loopend = loopstart + 32;
			//if (loopend > 767)
			//	loopend = 767;
			//loopstart = 0;
			//loopend = 767;

			//long colstart = colstrstart[loopstart];
			//long colend   = colstrstart[loopend];
			//if (colend == colstart)
			//	colend += 16;
			//if (colend > numpal)
			//	colend = numpal;
			//colstart = 0;
			//colend = numpal;
			//for (subctr = colstrstart[loopstart]; subctr <= colstrstart[loopend]; subctr++)
			//for (subctr = colstart; subctr < colend; subctr++)
			for (subctr = 0; subctr < numpal; subctr++)
			{
				unsigned long newdiff = ClassEMath::Abs((long)(sortpal[subctr] & 31) - (long)(ctr & 31)) + ClassEMath::Abs((long)((sortpal[subctr] & 992) >> 5) - (long)((ctr & 992) >> 5)) + ClassEMath::Abs((long)((sortpal[subctr] & 31744) >> 10) - (long)((ctr & 31744) >> 10));
				if (newdiff < lastcoldiff)
				{
					lookup[ctr] = sortpalindex[subctr];
					lastcoldiff = newdiff;
				}
			}
		}
	}
	else
	{
		lookup[0] = 0;
		for (ctr = 1; ctr < srcpalsize; ctr++)
		{
			unsigned long lastcoldiff = 0xFFFFFF;
			for (subctr = 0; subctr < numpal; subctr++)
			{
				unsigned long newdiff = ClassEMath::Abs((long)(sortpal[subctr] & 31) - (long)(srcpal[ctr] & 31)) + ClassEMath::Abs((long)((sortpal[subctr] & 992) >> 5) - (long)((srcpal[ctr] & 992) >> 5)) + ClassEMath::Abs((long)((sortpal[subctr] & 31744) >> 10) - (long)((srcpal[ctr] & 31744) >> 10));
				if (newdiff < lastcoldiff)
				{
					lookup[ctr] = sortpalindex[subctr];
					lastcoldiff = newdiff;
				}
			}
		}
	}
}

//Check if the size is a power of 2, otherwise padd it
void EDisplay_CheckSizePower2(unsigned long &sv)
{
	if (sv == 0x8 || sv == 0x10 || sv == 0x20 || sv == 0x40 || sv == 0x80 || sv == 0x100 || sv == 0x200 || sv == 0x400 || sv == 0x800 || sv == 0x1000)
		return;
	if (sv < 0x8)
		sv = 0x8;
	else if (sv < 0x10)
		sv = 0x10;
	else if (sv < 0x20)
		sv = 0x20;
	else if (sv < 0x40)
		sv = 0x40;
	else if (sv < 0x80)
		sv = 0x80;
	else if (sv < 0x100)
		sv = 0x100;
	else if (sv < 0x200)
		sv = 0x200;
	else if (sv < 0x400)
		sv = 0x400;
	else if (sv < 0x800)
		sv = 0x800;
	else if (sv < 0x1000)
		sv = 0x1000;
}

//Set prefer method list based on given method
void inline EDisplay_PreferMethodList(unsigned char openmethod, unsigned char *prefermethods)
{
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		if (openmethod == DOM_GDI)
		{
			prefermethods[0] = 2;//GDI
			prefermethods[1] = 1;//DirectDraw
		}
		else
		{
			prefermethods[0] = 1;//DirectDraw
			prefermethods[1] = 2;//GDI
		}
	#elif defined(DEVICE_WINCE)
		if (openmethod == DOM_DIRECTDRAW)
		{
			prefermethods[0] = 1;//DirectDraw
			prefermethods[1] = 0;//Framebuffer
			prefermethods[2] = 3;//GAPI
			prefermethods[3] = 2;//GDI
		}
		else if (openmethod == DOM_GAPI)
		{
			prefermethods[0] = 3;//GAPI
			prefermethods[1] = 0;//Framebuffer
			prefermethods[2] = 1;//DirectDraw
			prefermethods[3] = 2;//GDI
		}
		else if (openmethod == DOM_GDI)
		{
			prefermethods[0] = 2;//GDI
			prefermethods[1] = 0;//Framebuffer
			prefermethods[2] = 1;//DirectDraw
			prefermethods[3] = 3;//GAPI
		}
		else
		{
			prefermethods[0] = 0;//Framebuffer
			prefermethods[1] = 1;//DirectDraw
			prefermethods[2] = 3;//GAPI
			prefermethods[3] = 2;//GDI
		}
		prefermethods[4] = 3;//GAPI
	#elif defined(DEVICE_SYMBIAN)
		if (openmethod == DOM_DEFAULT)
		{
			#if defined(DEVICE_SERIES80) || defined(DEVICE_SERIES90)
				prefermethods[0] = 0;//Framebuffer
				prefermethods[1] = 1;//DirectScreenAccess
				prefermethods[2] = 2;//GDI
			#else
				prefermethods[0] = 1;//DirectScreenAccess
				prefermethods[1] = 2;//GDI
				prefermethods[2] = 0;//Framebuffer
			#endif
		}
		else if (openmethod == DOM_FRAMEBUFFER)
		{
			prefermethods[0] = 0;//Framebuffer
			prefermethods[1] = 1;//DirectScreenAccess
			prefermethods[2] = 2;//GDI
		}
		else if (openmethod == DOM_GDI)
		{
			prefermethods[0] = 2;//GDI
			prefermethods[1] = 1;//DirectScreenAccess
			prefermethods[2] = 0;//Framebuffer
		}
		else
		{
			prefermethods[0] = 1;//DirectScreenAccess
			prefermethods[1] = 2;//GDI
			prefermethods[2] = 0;//Framebuffer
		}
	#elif defined(DEVICE_LINUX)
		if (openmethod == DOM_GDI)
		{
			prefermethods[0] = 2;//XImage
			prefermethods[1] = 0;//Framebuffer
			prefermethods[2] = 3;//X11 with Xv
		}
		else if (openmethod == DOM_FRAMEBUFFER)
		{
			prefermethods[0] = 0;//Framebuffer
			prefermethods[1] = 2;//XImage
			prefermethods[2] = 3;//X11 with Xv
		}
		else if (openmethod == DOM_XV)
		{
			prefermethods[0] = 3;//X11 with Xv
			prefermethods[1] = 2;//XImage
			prefermethods[2] = 0;//Framebuffer			
		}
		else
		{
			#if defined(DEVICE_GP2X)
				prefermethods[0] = 0;//Framebuffer
				prefermethods[1] = 2;//XImage
			#else
				prefermethods[0] = 2;//XImage
				prefermethods[1] = 0;//Framebuffer
				prefermethods[2] = 3;//X11 with Xv
				prefermethods[3] = 4;//Java
			#endif
		}
	#elif defined(DEVICE_MAC)
		prefermethods[0] = 2;//CGImage
	#else
		prefermethods[0] = 0;//Framebuffer
	#endif
}

//Copies a block of pixels
void inline ClassEDisplay_PixelCopyBlock8(unsigned char *dst, unsigned char *src, long dhpitch, long dvpitch, long shpitch, long svpitch, unsigned long cw, unsigned long ch)
{
	unsigned long tw;
	while(ch)
	{
		unsigned char *dstt = dst;
		unsigned char *srct = src;
		ch--;
		tw = cw;
		while(tw)
		{
			tw--;
			*dstt = *srct;
			dstt += dhpitch;
			srct += shpitch;
		}
		dst += dvpitch;
		src += svpitch;
	}
}

//Copies a block of pixels
void inline ClassEDisplay_PixelCopyBlock16(unsigned short *dst, unsigned short *src, long dhpitch, long dvpitch, long shpitch, long svpitch, unsigned long cw, unsigned long ch)
{
	unsigned long tw;
	while(ch)
	{
		unsigned short *dstt = dst;
		unsigned short *srct = src;
		ch--;
		tw = cw;
		while(tw)
		{
			tw--;
			*dstt = *srct;
			dstt += dhpitch;
			srct += shpitch;
		}
		dst += dvpitch;
		src += svpitch;
	}
}

//Copies a block of pixels
void inline ClassEDisplay_PixelCopyBlock32(unsigned long *dst, unsigned long *src, long dhpitch, long dvpitch, long shpitch, long svpitch, unsigned long cw, unsigned long ch)
{
	unsigned long tw;
	while(ch)
	{
		unsigned EINT32 *dstt = (unsigned EINT32 *) dst;
		unsigned EINT32 *srct = (unsigned EINT32 *) src;
		ch--;
		tw = cw;
		while(tw)
		{
			tw--;
			*dstt = *srct;
			dstt += dhpitch;
			srct += shpitch;
		}
		dst += dvpitch;
		src += svpitch;
	}
}

//Copies pixel buffer (+ optimizations for odd pitches)
void ClassEDisplay_PixelCopy(void *dst, void *src, long dhpitch, long dvpitch, long shpitch, long svpitch, unsigned long cw, unsigned long ch, unsigned long dw, unsigned long dh, unsigned long sw, unsigned long sh, unsigned char bitdepth)
{
	unsigned char *dst8 = (unsigned char *)dst, *src8 = (unsigned char *)src;
	if (src == NULL)
	{
		if (ClassEMath::Abs(dhpitch) > ClassEMath::Abs(dvpitch))
		{
			long temp = dhpitch;
			dhpitch = dvpitch;
			dvpitch = temp;
		}
		if (dhpitch < 0)
		{
			dst8 += dhpitch * (long)(dw - 1);
			dhpitch = -dhpitch;
		}
		if (dhpitch == bitdepth / 8)
		{
			if ((long)cw != dvpitch / (bitdepth / 8))
			{
				while(ch)
				{
					ch--;
					ClassEMemory::Set(dst8, 0, cw * dhpitch);
					dst8 += dvpitch;
				}
			}
			else
				ClassEMemory::Set(dst8, 0, dvpitch * ch);
		}
		else
		{
			unsigned long register xctr;
			while(ch)
			{
				ch--;
				for (xctr = 0; xctr < cw; xctr++)
				{
					ClassEMemory::Set(dst8, 0, dhpitch);
					dst8 += dhpitch;
				}
				dst8 += dvpitch - (cw * dhpitch);
			}
		}
		return;
	}
	if (ClassEMath::Abs(dhpitch) != bitdepth / 8)
	{
		long temp;
		temp    = dhpitch;
		dhpitch = dvpitch;
		dvpitch = temp;
		temp    = shpitch;
		shpitch = svpitch;
		svpitch = temp;
		temp = ch;
		ch = cw;
		cw = temp;
	}
	if (dhpitch < 0)
	{
		dst8 += dhpitch * (long)(cw - 1);
		src8 += shpitch * (long)(cw - 1);
		dhpitch = -dhpitch;
		shpitch = -shpitch;
	}
	if (dhpitch == shpitch)
	{
		if (dvpitch != svpitch || (long)cw != svpitch / (bitdepth / 8))
		{
			while(ch)
			{
				ch--;
				ClassEMemory::Copy(dst8, src8, cw * shpitch);
				src8 += svpitch;
				dst8 += dvpitch;
			}
		}
		else
			ClassEMemory::Copy(dst8, src8, svpitch * ch);
	}
	else
	{
		if (dvpitch < 0)
		{
			dst8 = dst8 + dvpitch * (ch - 1);
			src8 = src8 + svpitch * (ch - 1);
			dvpitch = -dvpitch;
			svpitch = -svpitch;
		}
		if (bitdepth == 24)
		{
			unsigned long register xctr;
			while(ch)
			{
				ch--;
				for (xctr = 0; xctr < cw; xctr++)
				{
					ClassEMemory::Copy(dst8, src8, 3);
					dst8 += dhpitch;
					src8 += dhpitch;
				}
				dst8 += dvpitch - (cw * dhpitch);
				src8 += svpitch - (cw * shpitch);
			}
			return;
		}
		unsigned long bx, by, subx, suby;
		if (bitdepth == 8)
		{
			suby = 32;
			for (by = 0; by < ch; by += 32)
			{
				subx = 32;
				if (by + 32 > ch)
					suby = ch % 32;
				for (bx = 0; bx < cw; bx += 32)
				{
					if (bx + 32 > cw)
						subx = cw % 32;
					ClassEDisplay_PixelCopyBlock8(&dst8[by * dvpitch + bx * dhpitch], &src8[by * svpitch + bx * shpitch], dhpitch, dvpitch, shpitch, svpitch, subx, suby);
				}
			}
		}
		else if (bitdepth == 16)
		{
			unsigned short *dp16 = (unsigned short *)dst8;
			unsigned short *sp16 = (unsigned short *)src8;
			dhpitch >>= 1; shpitch >>= 1; dvpitch >>= 1; svpitch >>= 1;
			suby = 32;
			for (by = 0; by < ch; by += 32)
			{
				subx = 32;
				if (by + 32 > ch)
					suby = ch % 32;
				for (bx = 0; bx < cw; bx += 32)
				{
					if (bx + 32 > cw)
						subx = cw % 32;
					ClassEDisplay_PixelCopyBlock16(&dp16[by * dvpitch + bx * dhpitch], &sp16[by * svpitch + bx * shpitch], dhpitch, dvpitch, shpitch, svpitch, subx, suby);
				}
			}
		}
		else if (bitdepth == 32)
		{
			unsigned EINT32 *dp32 = (unsigned EINT32 *)dst8;
			unsigned EINT32 *sp32 = (unsigned EINT32 *)src8;
			dhpitch >>= 2; shpitch >>= 2; dvpitch >>= 2; svpitch >>= 2;
			suby = 32;
			for (by = 0; by < ch; by += 32)
			{
				subx = 32;
				if (by + 32 > ch)
					suby = ch % 32;
				for (bx = 0; bx < cw; bx += 32)
				{// Fixme: bit-aware prototype
					if (bx + 32 > cw)
						subx = cw % 32;
					ClassEDisplay_PixelCopyBlock32((unsigned long *) &dp32[by * dvpitch + bx * dhpitch], (unsigned long *) &sp32[by * svpitch + bx * shpitch], dhpitch, dvpitch, shpitch, svpitch, subx, suby);
				}
			}
		}
	}
}

//Bltfast using OpenGL
void ClassEDisplay_GLBltFast(long x, long y, long exthandle, RECT *rc, long swidth, long sheight, bool yreverse, void *gldriver)
{
	EINT32 trivtx[] =
	{
		x, y, 0,
		x + (rc->right - rc->left), y, 0,
		x, y + (rc->bottom - rc->top), 0,
		x + (rc->right - rc->left), y + (rc->bottom - rc->top), 0
	};
	EINT32 texcoordx[] =
	{
		(rc->left << 16) / swidth, (rc->top << 16) / sheight,
		(rc->right << 16) / swidth, (rc->top << 16) / sheight,
		(rc->left << 16) / swidth, (rc->bottom << 16) / sheight,
		(rc->right << 16) / swidth, (rc->bottom << 16) / sheight
	};
	if (yreverse)
	{
		texcoordx[1] = 65536 - texcoordx[1];
		texcoordx[3] = 65536 - texcoordx[3];
		texcoordx[5] = 65536 - texcoordx[5];
		texcoordx[7] = 65536 - texcoordx[7];
	}
	void *rdrarray[] = { trivtx, NULL, NULL, NULL, NULL };
	long strides[] = { sizeof(EINT32) * 3, 0, 0, 0 };
	rdrarray[3] = texcoordx;
	strides[3] = sizeof(EINT32) * 2;
	EdgeGL_RenderModel(gldriver, rdrarray, strides, 4, false, true);
}

//Bltfast using OpenGL
void ClassEDisplay_GLBltFx(long x, long y, long exthandle, RECT *rc, long swidth, long sheight, bool yreverse, void *gldriver, E2DBLTFX *bltfx, ECD *ecd)
{
	EINT32 trivtx[] =
	{
		0, 0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
	};
	unsigned char ctr;
	trivtx[3] = ((rc->right - rc->left) * (long)bltfx->xscale) >> 17;
	trivtx[9] = trivtx[3];
	trivtx[0] = trivtx[3] - (((rc->right - rc->left) * (long)bltfx->xscale) >> 16);
	trivtx[6] = trivtx[0];
	trivtx[7] = ((rc->bottom - rc->top) * (long)bltfx->yscale) >> 17;
	trivtx[10] = trivtx[7];
	trivtx[1] = trivtx[7] - (((rc->bottom - rc->top) * (long)bltfx->yscale) >> 16);
	trivtx[4] = trivtx[1];
	for (ctr = 0; ctr < 4; ctr++)
	{
		long tmpx, tmpy;
		if (bltfx->flipflags & EFLIP_HORIZONTAL)
			trivtx[ctr * 3] = -trivtx[ctr * 3];
		if (bltfx->flipflags & EFLIP_VERTICAL)
			trivtx[ctr * 3 + 1] = -trivtx[ctr * 3 + 1];
		tmpx = trivtx[ctr * 3];
		tmpy = trivtx[ctr * 3 + 1];
		trivtx[ctr * 3] = x + ((tmpx * ecd->math->Cos(bltfx->rotation)) >> 16) - ((tmpy * ecd->math->Sin(bltfx->rotation)) >> 16);
		trivtx[ctr * 3 + 1] = y + ((tmpx * ecd->math->Sin(bltfx->rotation)) >> 16) + ((tmpy * ecd->math->Cos(bltfx->rotation)) >> 16);
	}
	EINT32 texcoordx[] =
	{
		(rc->left << 16) / swidth, (rc->top << 16) / sheight,
		(rc->right << 16) / swidth, (rc->top << 16) / sheight,
		(rc->left << 16) / swidth, (rc->bottom << 16) / sheight,
		(rc->right << 16) / swidth, (rc->bottom << 16) / sheight
	};
	if (yreverse)
	{
		texcoordx[1] = 65536 - texcoordx[1];
		texcoordx[3] = 65536 - texcoordx[3];
		texcoordx[5] = 65536 - texcoordx[5];
		texcoordx[7] = 65536 - texcoordx[7];
	}
	void *rdrarray[] = { trivtx, NULL, NULL, NULL, NULL };
	long strides[] = { sizeof(EINT32) * 3, 0, 0, 0 };
	rdrarray[3] = texcoordx;
	strides[3] = sizeof(EINT32) * 2;
	EdgeGL_RenderModel(gldriver, rdrarray, strides, 4, false, true);
}

//Translates rectangle positions into virtual positions for displays with irregular pitches
void ClassEDisplay_ReVirtualRectFlip(RECT *rc, unsigned long sw, unsigned long sh, long orienth, long orientv, bool switchxy)
{
	long temp;
	if (switchxy)
	{
		temp = orienth;
		orienth = orientv;
		orientv = temp;
	}
	if (orientv < 0)
	{
		temp       = rc->top;
		rc->top    = sh - rc->bottom;
		rc->bottom = sh - temp;
	}
	if (orienth < 0)
	{
		temp       = rc->left;
		rc->left   = sw - rc->right;
		rc->right  = sw - temp;
	}
	if (switchxy)
	{
		temp       = rc->left;
		rc->left   = rc->top;
		rc->top    = temp;
		temp       = rc->right;
		rc->right  = rc->bottom;
		rc->bottom = temp;
	}
}

//Convert a color reference to a native color
void inline ClassEDisplay_ConvertColor(COLORREF &col, unsigned char vmode)
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
void inline ClassEDisplay_RevertColor(COLORREF &col, unsigned char vmode)
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

//Callback function for font character writing
ERESULT ClassEDisplay_DrawFontFunc(long x, long y, const short *srcrc, void *userdata)
{
	FONTDRAW_USERDATA_DISPLAY *info = (FONTDRAW_USERDATA_DISPLAY *)userdata;
	RECT rc;
	SetRect(&rc, srcrc[0], srcrc[1], srcrc[2], srcrc[3]);
	if (info->srcrgba)
	{
		E2DSurfaceRGBA *surfrgba = (E2DSurfaceRGBA *)info->source;
		info->dest->BltFast(x, y, surfrgba, &rc);
	}
	else
	{
		E2DSurface *surf = (E2DSurface *)info->source;
		info->dest->BltFast(x, y, surf, &rc);
	}
	return(E_OK);
}

///////////////////////////////////////////////////////////////////
// ClassEDisplay: private                                        //
///////////////////////////////////////////////////////////////////

//Translates an x,y position into a virtual position for displays with irregular pitches
void ClassEDisplay::VirtualPos(long &x, long &y, unsigned long sw, unsigned long sh)
{
	if (switchxy)
	{
		long t;
		t = x;
		x = y;
		y = t;
	}
	if (orienth < 0)
		x = sw - x - 1;
	if (orientv < 0)
		y = sh - y - 1;
}

//Translates rectangle positions into virtual positions for displays with irregular pitches, reverse backwards
void ClassEDisplay::ReVirtualRect(RECT *rc, unsigned long sw, unsigned long sh)
{
	long temp;
	if (orientv < 0)
	{
		temp       = rc->top;
		rc->top    = sh - rc->bottom;
		rc->bottom = sh - temp;
	}
	if (orienth < 0)
	{
		temp       = rc->left;
		rc->left   = sw - rc->right;
		rc->right  = sw - temp;
	}
	if (switchxy)
	{
		temp       = rc->left;
		rc->left   = rc->top;
		rc->top    = rc->right;
		rc->right  = rc->bottom;
		rc->bottom = rc->top;
		rc->top    = temp;
	}
}

//Translates rectangle positions into virtual positions for displays with irregular pitches
void ClassEDisplay::VirtualRect(RECT *rc, unsigned long sw, unsigned long sh)
{
	long temp;
	if (switchxy)
	{
		temp       = rc->left;
		rc->left   = rc->top;
		rc->top    = rc->right;
		rc->right  = rc->bottom;
		rc->bottom = rc->top;
		rc->top    = temp;
	}
	if (orienth < 0)
	{
		temp       = rc->left;
		rc->left   = sw - rc->right;
		rc->right  = sw - temp;
	}
	if (orientv < 0)
	{
		temp       = rc->top;
		rc->top    = sh - rc->bottom;
		rc->bottom = sh - temp;
	}
}

//Translates both a rectangle and position
void ClassEDisplay::VirtualRectPos(long &x, long &y, RECT *rc, unsigned long sw1, unsigned long sh1, unsigned long sw2, unsigned long sh2)
{
	long temp;
	if (switchxy)
	{
		temp = x;
		x = y;
		y = temp;
	}
	if (orienth < 0)
		x = sw1 - x - 1;
	if (orientv < 0)
		y = sh1 - y - 1;
	if (switchxy)
	{
		temp       = rc->left;
		rc->left   = rc->top;
		rc->top    = rc->right;
		rc->right  = rc->bottom;
		rc->bottom = rc->top;
		rc->top    = temp;
	}
	if (orienth < 0)
	{
		temp       = rc->left;
		rc->left   = sw2 - rc->right;
		rc->right  = sw2 - temp;
	}
	if (orientv < 0)
	{
		temp       = rc->top;
		rc->top    = sh2 - rc->bottom;
		rc->bottom = sh2 - temp;
	}
	if (orienth < 0)
		x -= rc->right - rc->left - 1;
	if (orientv < 0)
		y -= rc->bottom - rc->top - 1;
}

//Translates both a rectangle and position with rotation
void ClassEDisplay::VirtualRectPos(long &x, long &y, RECT *rc, unsigned long sw1, unsigned long sh1, unsigned long sw2, unsigned long sh2, unsigned short rprotation)
{
	long temp;
	if (switchxy)
	{
		temp = x;
		x = y;
		y = temp;
	}
	if (orienth < 0)
		x = sw1 - x - 1;
	if (orientv < 0)
		y = sh1 - y - 1;
	if (switchxy)
	{
		temp       = rc->left;
		rc->left   = rc->top;
		rc->top    = rc->right;
		rc->right  = rc->bottom;
		rc->bottom = rc->top;
		rc->top    = temp;
	}
	if (orienth < 0)
	{
		temp       = rc->left;
		rc->left   = sw2 - rc->right;
		rc->right  = sw2 - temp;
	}
	if (orientv < 0)
	{
		temp       = rc->top;
		rc->top    = sh2 - rc->bottom;
		rc->bottom = sh2 - temp;
	}
	if (orienth < 0)
	{
		if (rprotation % 2048 == 0)
			x -= rc->right - rc->left - 1;
		else
			x -= rc->bottom - rc->top - 1;
	}
	if (orientv < 0)
	{
		if (rprotation % 2048 == 0)
			y -= rc->bottom - rc->top - 1;
		else
			y -= rc->right - rc->left - 1;
	}
}

//Translates an x,y mouse position into a virtual position for rotated display orientations
void ClassEDisplay::VirtualMouse(long &x, long &y)
{
	long rect[4];
	if (ecInstGetWindowRect(curinst, rect, false))
	{
		long t;
		unsigned char checkrotation = (4 + rotation - sysorientation) % 4;
		if (checkrotation == DOR_90DEG)
		{
			t = x;
			x = y;
			y = rect[2] - rect[0] - t - 1;
		}
		else if (checkrotation == DOR_180DEG)
		{
			x = rect[2] - rect[0] - x - 1;
			y = rect[3] - rect[1] - y - 1;
		}
		else if (checkrotation == DOR_270DEG)
		{
			t = x;
			x = rect[3] - rect[1] - y - 1;
			y = t;
		}
	}
}

//Translates an arrow button number for rotated display orientations
void ClassEDisplay::VirtualDPad(unsigned long &bnr, EBUTTONLIST *blist)
{
	unsigned char ctr, checkrotation = (4 + rotation - sysorientation) % 4;
	if (checkrotation == DOR_NONE)
		return;
	unsigned long switchkey[] = { blist->DPadUp, blist->DPadDown, blist->DPadLeft, blist->DPadRight };
	unsigned long switchorder[] = { 0, 1, 2, 3 };
	#if defined(DEVICE_ANTIX)
		if (bnr >= KD_INPUT_GAMEKEYS_UP && bnr <= KD_INPUT_GAMEKEYS_DOWN)
		{
			switchkey[0] = KD_INPUT_GAMEKEYS_UP;
			switchkey[1] = KD_INPUT_GAMEKEYS_DOWN;
			switchkey[2] = KD_INPUT_GAMEKEYS_LEFT;
			switchkey[3] = KD_INPUT_GAMEKEYS_RIGHT;
		}
		else if (bnr >= KD_INPUT_GAMEKEYSNC_UP && bnr <= KD_INPUT_GAMEKEYSNC_DOWN)
		{
			switchkey[0] = KD_INPUT_GAMEKEYSNC_UP;
			switchkey[1] = KD_INPUT_GAMEKEYSNC_DOWN;
			switchkey[2] = KD_INPUT_GAMEKEYSNC_LEFT;
			switchkey[3] = KD_INPUT_GAMEKEYSNC_RIGHT;
		}
	#endif
	switch(checkrotation)
	{
		case DOR_90DEG: //Up to left, down to right, left to down, right to up
			switchorder[0] = 2; switchorder[1] = 3; switchorder[2] = 1; switchorder[3] = 0;
			break;
		case DOR_180DEG: //Up to down, down to up, left to right, right to left
			switchorder[0] = 1; switchorder[1] = 0; switchorder[2] = 3; switchorder[3] = 2;
			break;
		case DOR_270DEG: //Up to right, down to left, left to up and right to down
			switchorder[0] = 3; switchorder[1] = 2; switchorder[2] = 0; switchorder[3] = 1;
			break;
	}
	for (ctr = 0; ctr < 4; ctr++)
		if (bnr == switchkey[ctr])
		{
			bnr = switchkey[switchorder[ctr]];
			break;
		}
}

//Translates an x,y tilt into a virtual position for rotated display orientations
void ClassEDisplay::VirtualTilt(long &x, long &y)
{
	unsigned char checkrotation = rotation;
	long t;
	#if defined(DEVICE_SYMBIAN)
		checkrotation = (checkrotation + 4 - sysorientation) % 4;
	#endif
	t = x;
	if (checkrotation == DOR_90DEG)
	{
		x = y;
		y = -t;
	}
	else if (checkrotation == DOR_180DEG)
	{
		x = -x;
		y = -y;
	}
	else if (checkrotation == DOR_270DEG)
	{
		x = -y;
		y = t;
	}
}

//Converts a rotation into a virtual rotation for rotated display orientations
void ClassEDisplay::VirtualRotation(unsigned short &r)
{
	if (orienth < 0)
		r = 4096 - r % 4096;
	if (orientv < 0)
		r = 2048 - r % 4096;
	if (switchxy)
		r = 1024 - r % 4096;
	r %= 4096;
}

//Show a message for the evaluation version
bool ClassEDisplay::ShowNagMessage(void)
{
	#if defined(E_EVALUATIONTIMER)
	
	#endif
	return(true);
}

//Create backbuffer surface
ERESULT ClassEDisplay::CreateBuffer(void)
{
	E2DSurface *sysbuf = &buffer;
	ERESULT er = E_OK;
	if (realvideomode != videomode)
		sysbuf = &exbuffer;
	if (gldriver == NULL)
	{
		#if defined(DEVICE_SYMBIAN)
			er = CreateSurface(sysbuf, dispinfo.width, dispinfo.height, EST_SYSTEMAPI | (realvideomode << 8));
			#if defined(DEVICE_SERIES60) && (DEVICE_SYMBIAN <= 800)
				if (dispinfo.realwidth == 352 && dispinfo.realheight == 416 && (openmethod == DOM_GDI || openmethod == DOM_DSA))
				{
					sysbuf->swidth /= 2;
					sysbuf->sheight /= 2;
					sysbuf->virtualwidth /= 2;
					sysbuf->Init();
				}
			#endif
		#else
			unsigned char buffertype = ecd->dispcfg->surfacedefault % 16;
			#if defined(DEVICE_WIN32)
				#if defined(DEVICE_DESKTOP)
					if (openmethod == DOM_DIRECTDRAW && buffertype == EST_SYSMEMORY)
						buffertype = EST_SYSTEMAPI;
				#endif
				if (openmethod == DOM_GDI && buffertype == EST_SYSMEMORY)
					buffertype = EST_SYSTEMAPI;
			#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
				if (openmethod == DOM_GDI)
					buffertype = EST_SYSTEMAPI;
			#elif defined(DEVICE_OPENKODE)
				buffertype = EST_SYSTEMAPI;
			#endif
			er = CreateSurface(sysbuf, dispinfo.width, dispinfo.height, buffertype | (realvideomode << 8));
		#endif
		if (er == E_OK && realvideomode != videomode)
			er = CreateSurface(&buffer, dispinfo.width, dispinfo.height, ecd->dispcfg->surfacedefault);
	}
	return(er);
}

//Initialize after opening the display
ERESULT ClassEDisplay::Initialize(void)
{
	ERESULT er = E_OK;
	unsigned long ctr;
	er = CreateBuffer();
	#if defined(E_EVALUATIONTIMER)
		if (ecd->maincfg->flags & EFL_NOINTERNALFONT)
			ecd->maincfg->flags -= EFL_NOINTERNALFONT;
		demotimer = 0;
	#else
		demotimer = 0xFFFFFF;
	#endif
	if (er == E_OK && !(ecd->maincfg->flags & EFL_NOINTERNALFONT))
	{
		er = EDisplay_CreateInternalFont(this, &fontinternal, ecd->mem, ecd->dispcfg->surfacedefault, videomode == EDSP_INDEXED8);
		if (er == E_OK)
		{
			fontinternal.SetFontCharacters(32, 130);
			fontinternal.SetFontSheetLayout(6);
			if (videomode == EDSP_INDEXED8)
				fontinternal.BuildFont(true, 0);
			else
				fontinternal.BuildFont(true, ECOLOR_FUCHSIA);
			if (gldriver != NULL)
				UploadTexture(&fontinternal, true);
		}
	}
	if (gldriver == NULL)
	{
		#if defined(E_EVALUATIONTIMER)
			if (CreateSurface(&undermsg, buffer.GetWidth(), fontinternal.GetHeight() * 3 + 4) != E_OK)
			{
				Close();
				return(E_ERROR);
			}
		#endif
	}
	if (er != E_OK)
	{
		Close();
		return(E_ERROR);
	}
	if (edgerender == NULL && gldriver == NULL)
	{
		bool allowimport = true;
		#if !defined(E_EVALUATIONTIMER)
			if (edgerender_getlibid() > EDITION_SDK)
				allowimport = false;
		#endif
		edgerender = (void **)ClassEMemory::Alloc(256 * ERDR_NUMFUNCTIONS * sizeof(void *));
		if (edgerender == NULL)
		{
			Close();
			return(E_NOMEMORY);
		}
		ClassEMemory::Set(edgerender, 0, 256 * ERDR_NUMFUNCTIONS * sizeof(void *));
		if (allowimport)
		{
			if (videomode == EDSP_INDEXED8)
			{
				ERender_Get_BltFast8(&edgerender[ERDRF_BLTFAST * 256]);
				ERender_Get_BltFlip8(&edgerender[ERDRF_BLTFLIP * 256]);
			}
			else if (videomode == EDSP_TRUE12)
			{
				ERender_Get_BltFast12(&edgerender[ERDRF_BLTFAST * 256]);
				ERender_Get_BltFlip12(&edgerender[ERDRF_BLTFLIP * 256]);
			}
			else if (videomode == EDSP_TRUE16)
			{
				ERender_Get_BltFast16(&edgerender[ERDRF_BLTFAST * 256]);
				ERender_Get_BltFlip16(&edgerender[ERDRF_BLTFLIP * 256]);
			}
			else if (videomode == EDSP_TRUE32 || videomode == EDSP_TRUE32_INV)
			{
				ERender_Get_BltFast32(&edgerender[ERDRF_BLTFAST * 256]);
				ERender_Get_BltFlip32(&edgerender[ERDRF_BLTFLIP * 256]);
			}
		}
	}
	if (lightdata == NULL && gldriver == NULL)
	{
		lightdata = (E3DLIGHT *)ClassEMemory::Alloc(E3D_MAXLIGHTS * sizeof(E3DLIGHT));
		if (lightdata == NULL)
		{
			Close();
			return(E_NOMEMORY);
		}
		for (ctr = 0; ctr < E3D_MAXLIGHTS; ctr++)
		{
			lightdata[ctr].enabled = false;
			lightdata[ctr].position[0] = 0;
			lightdata[ctr].position[1] = 0;
			lightdata[ctr].position[2] = -65536;
			lightdata[ctr].intensity[0] = ECOLOR_BLACK;
			if (ctr == 0)
				lightdata[ctr].intensity[1] = ECOLOR_WHITE;
			else
				lightdata[ctr].intensity[1] = ECOLOR_BLACK;
		}
	}
	#if defined(DEVICE_WIN32)
		screenwidth  = GetSystemMetrics(SM_CXSCREEN);
		screenheight = GetSystemMetrics(SM_CYSCREEN);
	#else
		screenwidth  = dispinfo.width;
		screenheight = dispinfo.height;
	#endif
	if (gldriver == NULL)
		rendersize = dispinfo.width;
	else
		rendersize = GetWidth();
	curshading = E3D_GOURAUD;
	curshadingprop = 0;
	perspectivemapping = false;
	yreverse = false;
	if (dispinfo.height < rendersize)
		rendersize = dispinfo.height;
	quickpalette[256] = EDSP_NONE;
	ClassEMath::MatrixLoadIdentity(mtxworld);
	ClassEMath::MatrixLoadIdentity(mtxperspective);
	return(E_OK);
}

//Build an inverse palette lookup table for indexed images
bool ClassEDisplay::BuildPaletteLookupIndex(unsigned char *lookup, COLORREF *srcpalette)
{
	EDisplay_CreatePaletteLookup(videopalette, lookup, srcpalette, 256);
	return(true);
}

//Build an inverse palette lookup table
bool ClassEDisplay::BuildPaletteLookup(void)
{
	if (palettelookup != NULL && !refreshpalette)
		return(true);
	if (palettelookup == NULL)
	{
		palettelookup = (unsigned char *)ClassEMemory::Alloc(32768);
		if (palettelookup == NULL)
			return(false);
	}
	EDisplay_CreatePaletteLookup(videopalette, palettelookup);
	refreshpalette = false;
	return(true);
}

//Get a texture handle from OpenGL
long ClassEDisplay::QueryTextureHandle(void)
{
	if (gldriver != NULL)
	{
		unsigned long texid = EdgeGL_GenTexture(gldriver);
		if (texid > 0)
			return(texid);
	}
	return(-1);
}

//Release a texture handle to OpenGL
void ClassEDisplay::ReleaseTextureHandle(long texhandle)
{
	if (gldriver != NULL)
		if ((unsigned long)(texhandle) > 0)
			EdgeGL_DeleteTexture(gldriver, texhandle);
}

//Write pixel data to an OpenGL texture
void ClassEDisplay::WriteTextureData(long texhandle, unsigned long &texwidth, unsigned long &texheight, unsigned long surfwidth, unsigned long surfheight, unsigned long pformat, bool fromrgba, void *ptr, bool update)
{
	if (gldriver != NULL)
		if ((unsigned long)(texhandle) > 0)
		{
			RECT uploadrc;
			unsigned char colformat = 0, pixformat = 0;
			SetRect(&uploadrc, 0, 0, surfwidth, surfheight);
			if (!update)
			{
				texwidth = surfwidth;
				texheight = surfheight;
				EDisplay_CheckSizePower2(texwidth);
				EDisplay_CheckSizePower2(texheight);
			}
			if (pformat == EDSP_TRUE32_INV)
				colformat = 1;
			else if (pformat == EDSP_TRUE12)
				pixformat = 1;
			else if (pformat == EDSP_TRUE15)
				pixformat = 2;
			else if (pformat == EDSP_TRUE16)
				pixformat = 3;
			else if (pformat != EDSP_TRUE24_INV)
				return;
			EdgeGL_UpdateTextureData(gldriver, texhandle, texwidth, texheight, &uploadrc, colformat, pixformat, ptr, update);
		}
}

//Write compressed pixel data to an OpenGL texture
void ClassEDisplay::WriteCompressedTextureData(long texhandle, unsigned long &texwidth, unsigned long &texheight, unsigned long surfwidth, unsigned long surfheight, char *formatstr, void *ptr, bool update)
{
	if (gldriver != NULL)
		if ((unsigned long)(texhandle) > 0)
		{
			RECT uploadrc;
			SetRect(&uploadrc, 0, 0, surfwidth, surfheight);
			if (!update)
			{
				texwidth = surfwidth;
				texheight = surfheight;
				EDisplay_CheckSizePower2(texwidth);
				EDisplay_CheckSizePower2(texheight);
			}
			EdgeGL_UpdateCompressedTexture(gldriver, texhandle, texwidth, texheight, &uploadrc, formatstr, ptr, update);
		}
}

//Handle surface list
bool ClassEDisplay::HandleSurfList(E2DSurfaceBase *surf, bool inc)
{
	unsigned long ctr;
	if (inc)
	{
		unsigned long oldsize = surflistsize;
		for (ctr = 0; ctr < surflistsize; ctr++)
			if (surflist[ctr] == NULL)
			{
				surflist[ctr] = surf;
				return(true);
			}
		if (surflistsize)
		{
			unsigned long newsize = (surflistsize / EDSP_OBJLISTINCSIZE + 1) * EDSP_OBJLISTINCSIZE;
			E2DSurfaceBase **newlist = (E2DSurfaceBase **)ClassEMemory::Alloc(newsize * sizeof(E2DSurfaceBase *));
			if (newlist == NULL)
				return(false);
			ClassEMemory::Copy(newlist, surflist, surflistsize * sizeof(E2DSurfaceBase *));
			ClassEMemory::Set(&newlist[surflistsize], 0, (newsize - surflistsize) * sizeof(E2DSurfaceBase *));
			ClassEMemory::DeAlloc(surflist);
			surflist = newlist;
			surflistsize = newsize;
		}
		else
		{
			surflistsize = EDSP_OBJLISTINCSIZE;
			surflist = (E2DSurfaceBase **)ClassEMemory::Alloc(surflistsize * sizeof(E2DSurfaceBase *));
			if (surflist == NULL)
				return(false);
			ClassEMemory::Set(surflist, 0, surflistsize * sizeof(E2DSurfaceBase *));
		}
		for (ctr = oldsize; ctr < surflistsize; ctr++)
			if (surflist[ctr] == NULL)
			{
				surflist[ctr] = surf;
				return(true);
			}
	}
	else
	{
		for (ctr = 0; ctr < surflistsize; ctr++)
			if (surflist[ctr] == surf)
			{
				surflist[ctr] = NULL;
				return(true);
			}
	}
	return(false);
}

//Handle 3D object list
bool ClassEDisplay::HandleObj3DList(E3DSurface *surf, bool inc)
{
	unsigned long ctr;
	if (inc)
	{
		unsigned long oldsize = obj3dlistsize;
		for (ctr = 0; ctr < obj3dlistsize; ctr++)
			if (obj3dlist[ctr] == NULL)
			{
				obj3dlist[ctr] = surf;
				return(true);
			}
		if (obj3dlistsize)
		{
			unsigned long newsize = (obj3dlistsize / EDSP_OBJLISTINCSIZE + 1) * EDSP_OBJLISTINCSIZE;
			E3DSurface **newlist = (E3DSurface **)ClassEMemory::Alloc(newsize * sizeof(E3DSurface *));
			if (newlist == NULL)
				return(false);
			ClassEMemory::Copy(newlist, obj3dlist, obj3dlistsize * sizeof(E3DSurface *));
			ClassEMemory::Set(&newlist[obj3dlistsize], 0, (newsize - obj3dlistsize) * sizeof(E3DSurface *));
			ClassEMemory::DeAlloc(obj3dlist);
			obj3dlist = newlist;
			obj3dlistsize = newsize;
		}
		else
		{
			obj3dlistsize = EDSP_OBJLISTINCSIZE;
			obj3dlist = (E3DSurface **)ClassEMemory::Alloc(obj3dlistsize * sizeof(E3DSurface *));
			if (obj3dlist == NULL)
				return(false);
			ClassEMemory::Set(obj3dlist, 0, obj3dlistsize * sizeof(E3DSurface *));
		}
		for (ctr = oldsize; ctr < obj3dlistsize; ctr++)
			if (obj3dlist[ctr] == NULL)
			{
				obj3dlist[ctr] = surf;
				return(true);
			}
	}
	else
	{
		for (ctr = 0; ctr < obj3dlistsize; ctr++)
			if (obj3dlist[ctr] == surf)
			{
				obj3dlist[ctr] = NULL;
				return(true);
			}
	}
	return(false);
}

//Resize buffer because the window has been resized
void ClassEDisplay::OnChangeSize(RECT *newsize)
{
	if (gldriver != NULL || videomode == EDSP_NONE)
		return;
	long wrc[4];
	bool sendevent = false, dsphandlerotation = ecDspInfHandleOrientation(objdsp);
	unsigned char checkrotation = rotation;
	if (dsphandlerotation)
		checkrotation = (checkrotation + 4 - sysorientation) % 4;
	if (ecInstGetWindowRect(curinst, wrc, !dsphandlerotation))
	{
		dispinfo.realwidth = wrc[2] - wrc[0];
		dispinfo.realheight = wrc[3] - wrc[1];
		if (dsphandlerotation)
			ecInstGetWindowRect(curinst, wrc, true);
		ClassEMemory::Copy(prevwinrc, wrc, sizeof(long) * 4);
	}
	dispinfo.width = dispinfo.realwidth;
	dispinfo.height = dispinfo.realheight;
	if (checkrotation == DOR_90DEG || checkrotation == DOR_270DEG)
	{
		dispinfo.width = dispinfo.realheight;
		dispinfo.height = dispinfo.realwidth;
	}
	dispinfo.xpitch = realxpitch;
	dispinfo.ypitch = realypitch;
	if (checkrotation == DOR_90DEG)
	{
		dispinfo.bufferoffset = dispinfo.xpitch * (long)(dispinfo.realwidth - 1);
		long ptch_temp = -dispinfo.xpitch;
		dispinfo.xpitch = dispinfo.ypitch;
		dispinfo.ypitch = ptch_temp;
	}
	else if (checkrotation == DOR_180DEG)
	{
		dispinfo.bufferoffset = dispinfo.xpitch * (long)(dispinfo.realwidth - 1) + dispinfo.ypitch * (long)(dispinfo.realheight - 1);
		dispinfo.xpitch = -dispinfo.xpitch;
		dispinfo.ypitch = -dispinfo.ypitch;
	}
	else if (checkrotation == DOR_270DEG)
	{
		dispinfo.bufferoffset = dispinfo.ypitch * (long)(dispinfo.realheight - 1);
		long ptch_temp = dispinfo.xpitch;
		dispinfo.xpitch = -dispinfo.ypitch;
		dispinfo.ypitch = ptch_temp;
	}
	else
		dispinfo.bufferoffset = 0;
	if (buffer.IsCreated())
	{
		exbuffer.Free();
		buffer.Free();
		CreateBuffer();
		sendevent = true;
	}
	SetRect(&fliprc, 0, 0, dispinfo.width, dispinfo.height);
	if (sendevent)
		if (edge)
			edge->OnDisplayChange(this);
	tmpsuspend = false;
}

//Re-initialize display because the display has been changed
void ClassEDisplay::OnChangeMode(void)
{
	unsigned long ctr;
	if (suspended)
		Resume();
	for (ctr = 0; ctr < surflistsize; ctr++)
	{
		if (surflist[ctr])
			if (surflist[ctr]->currenttype % 16 != EST_SYSMEMORY)
				surflist[ctr]->Free();
	}
	if (objdsp)
	{
		ENATIVETYPE winhandle = (ENATIVETYPE)ecInstGetWindow(curinst);
		ERESULT er;
		ecDspClose(objdsp);
		objdsp = NULL;
		EC_DISPLAYCONFIG dspstyle;
		unsigned char ctr, preferredmode = ecd->dispcfg->videotype;
		ecErr err = 2;
		ecd->dispcfg->surfacedefault %= 16;
		if (preferredmode == EDSP_DEFAULT)
			preferredmode = EDSP_TRUE16;
		orienth  = 1;
		orientv  = 1;
		switchxy = false;
		realvideomode = preferredmode;
		rotation = ecd->dispcfg->orientation;
		dspstyle.fullscreen = fullscreenmode;
		dspstyle.xres = ecd->dispcfg->width;
		dspstyle.yres = ecd->dispcfg->height;
		dspstyle.hasmenu = (ecd->dispcfg->menu > 0);
		switch(preferredmode)
		{
			case EDSP_INDEXED8:
				dspstyle.bitdepth = 8;
				break;
			case EDSP_TRUE12:
			case EDSP_TRUE15:
			case EDSP_TRUE16:
				dspstyle.bitdepth = 16;
				break;
			case EDSP_TRUE24:
			case EDSP_TRUE24_INV:
				dspstyle.bitdepth = 24;
				break;
			case EDSP_TRUE32:
			case EDSP_TRUE32_INV:
				dspstyle.bitdepth = 32;
				break;
		}
		if (objdsp == NULL)
			objdsp = ecDspCreate();
		if (objdsp == NULL)
		{
			Close();
			return;
		}
		unsigned char prefermethods[] = { 9, 9, 9, 9, 9, 9 };
		unsigned long iorientation = 0;
		EDisplay_PreferMethodList(ecd->dispcfg->openmethod, prefermethods);
		if (sysorientation == DOR_90DEG)
			iorientation = 90;
		else if (sysorientation == DOR_180DEG)
			iorientation = 180;
		else if (sysorientation == DOR_270DEG)
			iorientation = 270;
		for (ctr = 0; ctr < 6; ctr++)
		{
			if (prefermethods[ctr] > 8)
				break;
			dspstyle.prefermethod = prefermethods[ctr];
			err = ecDspOpen(objdsp, winhandle, &dspstyle);
			if (err == 0)
			{
				if (dspstyle.prefermethod == 0)
					openmethod = DOM_FRAMEBUFFER;
				else if (dspstyle.prefermethod == 1)
				{
					#if defined(DEVICE_WIN32)
						openmethod = DOM_DIRECTDRAW;
					#else
						openmethod = DOM_DSA;
					#endif
				}
				else if (dspstyle.prefermethod == 2)
					openmethod = DOM_GDI;
				else if (dspstyle.prefermethod == 3 || dspstyle.prefermethod == 4)
					openmethod = DOM_GAPI;
				break;
			}
			ecDspClose(objdsp);
		}
		if (err)
		{
			Close();
			return;
		}
		if (dspstyle.bitdepth == 8)
		{
			videomode = EDSP_INDEXED8;
			dispinfo.bitwidth = 8;
		}
		else if (dspstyle.bitdepth == 16)
		{
			if (dspstyle.colorchnl == ((4 << 16) | (4 << 8) | 4))
				videomode = EDSP_TRUE12;
			else if (dspstyle.colorchnl == ((5 << 16) | (5 << 8) | 5))
				videomode = EDSP_TRUE15;
			else if (dspstyle.colorchnl == ((5 << 16) | (6 << 8) | 5))
				videomode = EDSP_TRUE16;
			dispinfo.bitwidth = 16;
		}
		else if (dspstyle.bitdepth == 24)
		{
			videomode = EDSP_TRUE24;
			dispinfo.bitwidth = 24;
			#if defined(DEVICE_MAC)
				videomode = EDSP_TRUE24_INV;
			#endif
		}
		else if (dspstyle.bitdepth == 32)
		{
			videomode = EDSP_TRUE32;
			dispinfo.bitwidth = 32;
			#if defined(DEVICE_MAC)
				videomode = EDSP_TRUE32_INV;
			#endif
		}
		else
		{
			Close();
			return;
		}
		dispinfo.realwidth = dspstyle.xres;
		dispinfo.realheight = dspstyle.yres;
		realxpitch = dspstyle.xpitch;
		realypitch = dspstyle.ypitch;
		if (!ChangeOrientation(rotation))
		{
			Close();
			return;
		}
		realvideomode = videomode;
		videomode = ecd->dispcfg->emulvideotype;
		if (videomode == EDSP_DEFAULT)
			videomode = realvideomode;
		if (videomode == EDSP_TRUE15)
			videomode = EDSP_TRUE16;
		else if (videomode == EDSP_TRUE24)
			videomode = EDSP_TRUE32;
		else if (videomode == EDSP_TRUE24_INV)
			videomode = EDSP_TRUE32_INV;
		er = Initialize();
		if (er != E_OK)
		{
			Close();
			return;
		}
		if (edge)
			edge->OnDisplayChange(this);
	}
}

void ClassEDisplay::OnContextLost(void)
{
	if (gldriver)
	{
		EdgeGL_Repair(gldriver);

        for (unsigned long ctr = 0; ctr < surflistsize; ctr++)
            if (surflist[ctr])
                if (surflist[ctr] != &buffer && surflist[ctr] != &exbuffer)
                {
                    surflist[ctr]->exthandle = -1;
                    if (surflist[ctr]->autorepair)
                        surflist[ctr]->Reload(this);
                    else
                        surflist[ctr]->dirty = true;
                }

        return;
	}
}

///////////////////////////////////////////////////////////////////
// ClassEDisplay: public                                         //
///////////////////////////////////////////////////////////////////

//ClassEDisplay: constructor
ClassEDisplay::ClassEDisplay(void) : buffer(this)
{
	objdsp = NULL;
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_GRAPHICSLOADERS; ctr++)
		graphicsloader[ctr] = NULL;
	usetex = NULL;
	edge = NULL;
	ecd = NULL;
	palettelookup = NULL;
	edgerender = NULL;
	lightdata = NULL;
	gldriver = NULL;
	videomode = EDSP_NONE;
	realvideomode = EDSP_NONE;
	sysorientation = DOR_NONE;
	surflistsize = 0;
	obj3dlistsize = 0;
}

//ClassEDisplay: destructor
ClassEDisplay::~ClassEDisplay(void)
{
	unsigned char ctr;
	Close();
	if (obj3dlistsize)
	{
		ClassEMemory::DeAlloc(obj3dlist);
		obj3dlistsize = 0;
	}
	if (surflistsize)
	{
		ClassEMemory::DeAlloc(surflist);
		surflistsize = 0;
	}
	for (ctr = 0; ctr < MAX_GRAPHICSLOADERS; ctr++)
		if (graphicsloader[ctr] != NULL)
			delete graphicsloader[ctr];
	if (objdsp)
	{
		ecDspDestroy(objdsp);
		objdsp = NULL;
	}
	ecd = NULL;
}

//Open the graphics display
ERESULT ClassEDisplay::Open(ECOREOBJ inst, ECOREOBJ winstyle, ECD *pecd)
{
	ecd = pecd;
	if (inst == NULL)
		return(E_INVALIDPARAM);
	ERESULT er;
	ENATIVETYPE winhandle = (ENATIVETYPE)ecInstGetWindow(inst);
	unsigned char ctr, preferredmode = pecd->dispcfg->videotype;
	curinst = inst;
	curwinstyle = winstyle;
	Close();
	if (winhandle == 0)
		return(E_INVALIDPARAM);
	#if defined(DEVICE_DESKTOP) || (defined(DEVICE_WINCE) && !defined(DEVICE_WINMOBILE))
		if (ecd->dispcfg->width < 4 || ecd->dispcfg->height < 4)
			return(E_INVALIDPARAM);
	#endif
	ecd->dispcfg->surfacedefault %= 16;
	if (preferredmode == EDSP_DEFAULT)
		preferredmode = EDSP_TRUE16;
	fullscreenmode = ecd->dispcfg->fullscreen;
	skipflip = 0;
	orienth  = 1;
	orientv  = 1;
	switchxy = false;
	suspended = false;
	tmpsuspend = false;
	resizefromsip = false;
	suspendfrommode = false;
	realvideomode = preferredmode;
	#if (EDITION_SDK >= 1000)
		if (EdgeGL_Use())
		{
			rotation = sysorientation; 
			gldriver = EdgeGL_NewDriver();
			if (gldriver)
			{				
				#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
					if (!fullscreenmode && ecd->dispcfg->dialogbox == 0)
					{
						EC_WINDOWSTYLE *winstyle = (EC_WINDOWSTYLE *)curwinstyle;
						winstyle->width = ecd->dispcfg->width;
						winstyle->height = ecd->dispcfg->height;
						ecInstAdjustWindow(curinst, winstyle);
					}
				#endif
				er = EdgeGL_Open(gldriver, (void *)winhandle, preferredmode, ecd->dispcfg->width, ecd->dispcfg->height, fullscreenmode, ecd->dispcfg->stencil ? true : false);
				if (er != E_OK)
					return(er);
				videomode = EdgeGL_GetVideoType(gldriver);
				if (videomode == EDSP_NONE)
					return(E_ERROR);
				if (videomode == EDSP_TRUE32)
					videomode = EDSP_TRUE32_INV;
				orientv = -1;

				// start the wrapper code in unrotated mode
				rotation = sysorientation;
				buffer.SetGLDriver(gldriver);

				unsigned long x, y;
				EdgeGL_GetVideoSize(gldriver, x, y);
				if (ecd->dispcfg->orientation == DOR_LANDSCAPE)
					rotation = (x > y) ? (sysorientation) : (sysorientation + 1);
				else if (ecd->dispcfg->orientation == DOR_PORTRAIT)
					rotation = (y > x) ? (sysorientation) : (sysorientation + 1);
				else if (ecd->dispcfg->orientation < 4)
					rotation = ecd->dispcfg->orientation;
				else
					rotation = sysorientation;
				rotation = rotation % 4;
				
				EdgeGL_SetOrientation(gldriver, (rotation + 4 - sysorientation) % 4);
			}
			else
				return(E_ERROR);

		}
	#endif
	if (gldriver == NULL)
	{
		EC_DISPLAYCONFIG dspstyle;
		ecErr err = 2;
		rotation = ecd->dispcfg->orientation;
		dspstyle.fullscreen = fullscreenmode;
		dspstyle.xres = ecd->dispcfg->width;
		dspstyle.yres = ecd->dispcfg->height;
		dspstyle.hasmenu = (ecd->dispcfg->menu > 0);
		switch(preferredmode)
		{
			case EDSP_INDEXED8:
				dspstyle.bitdepth = 8;
				break;
			case EDSP_TRUE12:
			case EDSP_TRUE15:
			case EDSP_TRUE16:
				dspstyle.bitdepth = 16;
				break;
			case EDSP_TRUE24:
			case EDSP_TRUE24_INV:
				dspstyle.bitdepth = 24;
				break;
			case EDSP_TRUE32:
			case EDSP_TRUE32_INV:
				dspstyle.bitdepth = 32;
				break;
		}
		if (objdsp == NULL)
			objdsp = ecDspCreate();
		if (objdsp == NULL)
		{
			Close();
			return(E_ERROR);
		}
		unsigned char prefermethods[] = { 9, 9, 9, 9, 9, 9 };
		EDisplay_PreferMethodList(ecd->dispcfg->openmethod, prefermethods);
		unsigned long iorientation = 0;
		if (sysorientation == DOR_90DEG)
			iorientation = 90;
		else if (sysorientation == DOR_180DEG)
			iorientation = 180;
		else if (sysorientation == DOR_270DEG)
			iorientation = 270;
		for (ctr = 0; ctr < 6; ctr++)
		{
			if (prefermethods[ctr] > 8)
				break;
			dspstyle.prefermethod = prefermethods[ctr];
			err = ecDspOpen(objdsp, winhandle, &dspstyle);
			if (err == 0)
			{
				if (dspstyle.prefermethod == 0)
					openmethod = DOM_FRAMEBUFFER;
				else if (dspstyle.prefermethod == 1)
				{
					#if defined(DEVICE_WIN32)
						openmethod = DOM_DIRECTDRAW;
					#else
						openmethod = DOM_DSA;
					#endif
				}
				else if (dspstyle.prefermethod == 2)
					openmethod = DOM_GDI;
				else if (dspstyle.prefermethod == 3 || dspstyle.prefermethod == 4)
					openmethod = DOM_GAPI;
				break;
			}
			ecDspClose(objdsp);
		}
		if (err)
		{
			long errarray[] = { E_OK, E_NOMEMORY, E_UNSUPPORTED, E_ERROR, E_NOENTRY };
			Close();
			return(ecExMapError(err, errarray, 5));
		}
		if (objdsp)
		{
			if (dspstyle.bitdepth == 8)
			{
				videomode = EDSP_INDEXED8;
				dispinfo.bitwidth = 8;
			}
			else if (dspstyle.bitdepth == 16)
			{
				if (dspstyle.colorchnl == ((4 << 16) | (4 << 8) | 4))
					videomode = EDSP_TRUE12;
				else if (dspstyle.colorchnl == ((5 << 16) | (5 << 8) | 5))
					videomode = EDSP_TRUE15;
				else if (dspstyle.colorchnl == ((5 << 16) | (6 << 8) | 5))
					videomode = EDSP_TRUE16;
				dispinfo.bitwidth = 16;
			}
			else if (dspstyle.bitdepth == 24)
			{
				videomode = EDSP_TRUE24;
				dispinfo.bitwidth = 24;
				#if defined(DEVICE_MAC)
					videomode = EDSP_TRUE24_INV;
				#endif
			}
			else if (dspstyle.bitdepth == 32)
			{
				videomode = EDSP_TRUE32;
				dispinfo.bitwidth = 32;
				#if defined(DEVICE_MAC)
					videomode = EDSP_TRUE32_INV;
				#endif
			}
			else
			{
				Close();
				return(E_UNSUPPORTED);
			}
			dispinfo.realwidth = dspstyle.xres;
			dispinfo.realheight = dspstyle.yres;
			realxpitch = dspstyle.xpitch;
			realypitch = dspstyle.ypitch;
			if (!ChangeOrientation(rotation))
			{
				Close();
				return(E_UNSUPPORTED);
			}
		}
	}
	realvideomode = videomode;
	videomode = ecd->dispcfg->emulvideotype;
	if (videomode == EDSP_DEFAULT)
		videomode = realvideomode;
	if (videomode == EDSP_TRUE15)
		videomode = EDSP_TRUE16;
	else if (videomode == EDSP_TRUE24)
		videomode = EDSP_TRUE32;
	else if (videomode == EDSP_TRUE24_INV)
		videomode = EDSP_TRUE32_INV;
	er = Initialize();
	
	#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		if (er == E_OK && gldriver == NULL && !fullscreenmode && ecd->dispcfg->dialogbox == 0)
		{
			EC_WINDOWSTYLE *winstyle = (EC_WINDOWSTYLE *)curwinstyle;
			winstyle->width = dispinfo.realwidth;
			winstyle->height = dispinfo.realheight;
			ecInstAdjustWindow(curinst, winstyle);
		}
	#endif
	if (ecd->maincfg->flags & EFL_SCREENCAPTURE)
		BackFlip();	
	return(er);
}

//Close the display mode
void ClassEDisplay::Close(void)
{
	unsigned long ctr;
	if (suspended)
		Resume();
	for (ctr = 0; ctr < surflistsize; ctr++)
	{
		if (surflist[ctr])
			surflist[ctr]->Free();
	}
	for (ctr = 0; ctr < obj3dlistsize; ctr++)
	{
		if (obj3dlist[ctr])
			obj3dlist[ctr]->Free();
	}
	if (gldriver)
	{
		EdgeGL_Close(gldriver);
		EdgeGL_FreeDriver(gldriver);
		gldriver = NULL;
	}
	FULL_FREE(lightdata);
	FULL_FREE(edgerender);
	FULL_FREE(palettelookup);
	if (objdsp)
		ecDspClose(objdsp);
	videomode = EDSP_NONE;
}

//Suspend display operations
void ClassEDisplay::Suspend(void)
{
	if (suspended || videomode == EDSP_NONE)
		return;
	if (gldriver)
		EdgeGL_Suspend(gldriver);
	else
		ecDspSuspend(objdsp);
	suspended = true;
}

//Resume display operations
void ClassEDisplay::Resume(void)
{
	tmpsuspend = false;
	suspendfrommode = false;
	if (!suspended || videomode == EDSP_NONE)
		return;
	if (gldriver)
		EdgeGL_Resume(gldriver);
	else
		ecDspResume(objdsp);
	suspended = false;
}

//Change display orientation
bool ClassEDisplay::ChangeOrientation(unsigned char orientation)
{
	if (gldriver != NULL)
	{
		rotation = orientation;
		EdgeGL_UpdateDisplay(gldriver);

		unsigned long x, y;
		EdgeGL_GetVideoSize(gldriver, x, y);
		if (orientation == DOR_LANDSCAPE)
			rotation = (x > y) ? (sysorientation) : (sysorientation + 1);
		else if (orientation == DOR_PORTRAIT)
			rotation = (y > x) ? (sysorientation) : (sysorientation + 1);
		else if (orientation < 4)
			rotation = orientation;
		else
			rotation = sysorientation;
		rotation = rotation % 4;

		EdgeGL_SetOrientation(gldriver, (rotation + 4 - sysorientation) % 4);
		if (edge)
			edge->OnDisplayChange(this);

		return(true);
	}

	if (videomode == EDSP_NONE)
		return(false);

	long wrc[4], nativeres[2];
	unsigned char newrotation = orientation;
	bool sendevent = false, dsphandlerotation = ecDspInfHandleOrientation(objdsp);
	if (dsphandlerotation)
	{
		#if !defined(DEVICE_DESKTOP)
			long wpitch[2];
			if (ecDspLock(objdsp, wpitch, true))
			{
				realxpitch = wpitch[0];
				realypitch = wpitch[1];
				ecDspUnlock(objdsp);
			}
		#endif
	}
	if (ecInstGetWindowRect(curinst, wrc, !dsphandlerotation))
	{
		#if !defined(DEVICE_DESKTOP)
			dispinfo.realwidth = wrc[2] - wrc[0];
			dispinfo.realheight = wrc[3] - wrc[1];
		#endif
		if (dsphandlerotation)
			ecInstGetWindowRect(curinst, wrc, true);
		ClassEMemory::Copy(prevwinrc, wrc, sizeof(long) * 4);
	}
	ecDspInfNativeResolution(objdsp, nativeres);
	if (newrotation == DOR_PORTRAIT)
	{
		if (nativeres[0] > nativeres[1] && sysorientation != DOR_90DEG && sysorientation != DOR_270DEG)
			newrotation = DOR_270DEG;
		else if (nativeres[0] < nativeres[1])
			newrotation = DOR_NONE;
		else
			newrotation = sysorientation;
	}
	else if (newrotation == DOR_LANDSCAPE)
	{
		if (nativeres[0] < nativeres[1] && sysorientation != DOR_90DEG && sysorientation != DOR_270DEG)
			newrotation = DOR_90DEG;
		else if (nativeres[0] > nativeres[1])
			newrotation = DOR_NONE;
		else
			newrotation = sysorientation;
	}
	else if (newrotation == DOR_AUTO || newrotation == DOR_STARTUP)
		newrotation = sysorientation;
	if (newrotation != DOR_NONE && newrotation != DOR_90DEG && newrotation != DOR_180DEG && newrotation != DOR_270DEG)
		return(false);
	long olddspdata[3];
	unsigned char checkrotation = newrotation;
	if (dsphandlerotation)
		checkrotation = (checkrotation + 4 - sysorientation) % 4;
	olddspdata[0] = orienth;
	olddspdata[1] = orientv;
	if (switchxy)
		olddspdata[2] = 1;
	else
		olddspdata[2] = 0;
	orienth = 1;
	orientv = 1;
	switchxy = false;
	dispinfo.xpitch = realxpitch;
	dispinfo.ypitch = realypitch;
	dispinfo.width = dispinfo.realwidth;
	dispinfo.height = dispinfo.realheight;
	dispinfo.bufferoffset = 0;
	
	#if defined(DEVICE_IPHONE)
		// Y-points-up systems
		dispinfo.bufferoffset += dispinfo.ypitch * (long)(dispinfo.realheight - 1);
		dispinfo.ypitch = -dispinfo.ypitch;
	#endif
	
	if (checkrotation == DOR_90DEG || checkrotation == DOR_270DEG)
	{
		dispinfo.width = dispinfo.realheight;
		dispinfo.height = dispinfo.realwidth;
	}
	if (checkrotation == DOR_90DEG)
	{
		dispinfo.bufferoffset += dispinfo.xpitch * (long)(dispinfo.realwidth - 1);
		long ptch_temp = -dispinfo.xpitch;
		dispinfo.xpitch = dispinfo.ypitch;
		dispinfo.ypitch = ptch_temp;
	}
	else if (checkrotation == DOR_180DEG)
	{
		dispinfo.bufferoffset += dispinfo.xpitch * (long)(dispinfo.realwidth - 1) + dispinfo.ypitch * (long)(dispinfo.realheight - 1);
		dispinfo.xpitch = -dispinfo.xpitch;
		dispinfo.ypitch = -dispinfo.ypitch;
	}
	else if (checkrotation == DOR_270DEG)
	{
		dispinfo.bufferoffset += dispinfo.ypitch * (long)(dispinfo.realheight - 1);
		long ptch_temp = dispinfo.xpitch;
		dispinfo.xpitch = -dispinfo.ypitch;
		dispinfo.ypitch = ptch_temp;
	}
	else
		dispinfo.bufferoffset += 0;
	if (ecd->dispcfg->surfprerotate)
	{
		if (ClassEMath::Abs(dispinfo.xpitch) > ClassEMath::Abs(dispinfo.ypitch))
			switchxy = true;
		else
			switchxy = false;
		if (dispinfo.xpitch < 0)
		{
			if (switchxy)
				orientv = -1;
			else
				orienth = -1;
		}
		if (dispinfo.ypitch < 0)
		{
			if (switchxy)
				orienth = -1;
			else
				orientv = -1;
		}
		prerotation = true;
	}
	else
		prerotation = false;
	
	rotation = newrotation;
	if (buffer.IsCreated())
	{
		unsigned long ctr;
		exbuffer.Free();
		buffer.Free();
		CreateBuffer();
		if (prerotation != ecd->dispcfg->surfprerotate || prerotation == true)
			for (ctr = 0; ctr < surflistsize; ctr++)
				if (surflist[ctr])
					if (surflist[ctr] != &buffer && surflist[ctr] != &exbuffer)
						surflist[ctr]->OnChangeOrientation(olddspdata);
		sendevent = true;
	}
	SetRect(&fliprc, 0, 0, dispinfo.width, dispinfo.height);
	if (sendevent)
		if (edge)
			edge->OnDisplayChange(this);
	tmpsuspend = false;
	return(true);
}

//Flip the backbuffer onto the primary surface
void ClassEDisplay::Flip(bool forceblack)
{
	#if defined(DEVICE_WINMOBILE)
		if (GetForegroundWindow() != (HWND)ecInstGetWindow(curinst))
			return;
		if (suspendfrommode)
			return;
	#endif
	if (skipflip)
	{
		skipflip--;
		return;
	}
	if (tmpsuspend)
		return;
	if (gldriver != NULL && !suspended)
	{
		#if defined(E_EVALUATIONTIMER)
			ShowNagMessage();
		#endif
		EdgeGL_Flip(gldriver);
		return;
	}
	if (suspended || videomode == EDSP_NONE)
		return;
	if ((realvideomode == videomode) && (realvideomode != buffer.videomode))
		return;
	if ((realvideomode != videomode) && (realvideomode != exbuffer.videomode))
		return;
	if (!ShowNagMessage())
		return;
	E2DSurface *flipbuf = &buffer;
	if (realvideomode != videomode)
	{
		flipbuf = &exbuffer;
		flipbuf->BltFast(0, 0, &buffer, NULL);
	}
	#if defined(E_EVALUATIONTIMER)
		RECT prevfliprc;
		if (demotimer < E_EVALUATIONTIMER * 1000)
		{
			ClassEMemory::Copy(&prevfliprc, &fliprc, sizeof(RECT));
			SetFlipRect(NULL);
		}
	#endif
	if (objdsp)
	{
		bool dsphandlerotation = ecDspInfHandleOrientation(objdsp);
		long pos[] = { fliprc.left, fliprc.top };
		long rect[] = { fliprc.left, fliprc.top, fliprc.right, fliprc.bottom };
		#if defined(DEVICE_WIN32)
			if (!fullscreenmode)
			{
				long winrc[4];
				ecInstGetWindowRect(curinst, winrc, true);
				#if defined(DEVICE_WINMOBILE)
					if (winrc[0] != prevwinrc[0] || winrc[1] != prevwinrc[1])
						return;
				#endif
				if (dsphandlerotation)
					ecInstGetWindowRect(curinst, winrc, false);
				if (openmethod != DOM_GDI)
				{
					pos[0] += winrc[0];
					pos[1] += winrc[1];
					#if defined(DEVICE_DESKTOP)
						if (ecd->dispcfg->dialogbox)
						{
							pos[0] += ecd->dispcfg->dlgleft;
							pos[1] += ecd->dispcfg->dlgtop;
						}
					#endif
				}
			}
		#endif
		if (ecd->maincfg->flags & EFL_NOTEARING)
			ecDspWaitSync(objdsp);
		if (!flipbuf->nativesurf || forceblack || (sysorientation != rotation && !prerotation))
		{
			long wpitch[2];
			unsigned char *displock = (unsigned char *)ecDspLock(objdsp, wpitch, false);
			if (dsphandlerotation && (wpitch[0] != realxpitch || wpitch[1] != realypitch))
			{
				ecDspUnlock(objdsp);
				displock = NULL;
			}
			if (displock)
			{
				E2DSURFACEINFO bufinfo;
				unsigned char *buflock = flipbuf->Lock(&bufinfo);
				//displock += pos[0] * realxpitch + pos[1] * realypitch;
				displock += (pos[0] - fliprc.left) * realxpitch + (pos[1] - fliprc.top) * realypitch + fliprc.left * dispinfo.xpitch + fliprc.top * dispinfo.ypitch;
				displock += dispinfo.bufferoffset;
				if (buflock)
				{
					buflock += bufinfo.bufferoffset;
					if (forceblack)
						ClassEDisplay_PixelCopy(&displock[/*rect[0] * dispinfo.xpitch + rect[1] * dispinfo.ypitch*/0], NULL, dispinfo.xpitch, dispinfo.ypitch, bufinfo.xpitch, bufinfo.ypitch, rect[2] - rect[0], rect[3] - rect[1], dispinfo.realwidth, dispinfo.realheight, bufinfo.realwidth, bufinfo.realheight, dispinfo.bitwidth);
					else
						ClassEDisplay_PixelCopy(&displock[/*rect[0] * dispinfo.xpitch + rect[1] * dispinfo.ypitch*/0], &buflock[rect[0] * bufinfo.xpitch + rect[1] * bufinfo.ypitch], dispinfo.xpitch, dispinfo.ypitch, bufinfo.xpitch, bufinfo.ypitch, rect[2] - rect[0], rect[3] - rect[1], dispinfo.realwidth, dispinfo.realheight, bufinfo.realwidth, bufinfo.realheight, dispinfo.bitwidth);
						//ClassEDisplay_PixelCopy(&displock[/*rect[0] * dispinfo.xpitch + rect[1] * dispinfo.ypitch*/0], &buflock[rect[0] * bufinfo.bitwidth / 8 + rect[1] * bufinfo.realpitch], dispinfo.xpitch, dispinfo.ypitch, bufinfo.xpitch, bufinfo.ypitch, rect[2] - rect[0], rect[3] - rect[1], dispinfo.realwidth, dispinfo.realheight, bufinfo.realwidth, bufinfo.realheight, dispinfo.bitwidth);
					flipbuf->Unlock();
				}
				ecDspUnlock(objdsp);
			}
		}
		else
		{
			if ((sysorientation != rotation && prerotation))
			{
				RECT vfliprc;
				ClassEMemory::Copy(&vfliprc, &fliprc, sizeof(RECT));
				ClassEDisplay_ReVirtualRectFlip(&vfliprc, dispinfo.width, dispinfo.height, orienth, orientv, switchxy);
				pos[0] = vfliprc.left + (pos[0] - fliprc.left);
				pos[1] = vfliprc.top + (pos[1] - fliprc.top);
				rect[0] = vfliprc.left;
				rect[1] = vfliprc.top;
				rect[2] = vfliprc.right;
				rect[3] = vfliprc.bottom;
			}
			if (ecDspFlipNativeSurface(objdsp, flipbuf->nativesurf, pos, rect) == 2)
				OnChangeMode();
		}
	}
	#if defined(E_EVALUATIONTIMER)
		if (demotimer < E_EVALUATIONTIMER * 1000)
			ClassEMemory::Copy(&fliprc, &prevfliprc, sizeof(RECT));
		#if !defined(DEVICE_IPHONEEMU)
			if (demotimer < E_EVALUATIONTIMER * 1000)
			{
				RECT cliprc;
				buffer.GetClipper(&cliprc);
				buffer.SetClipper(NULL);
				buffer.BltFast(0, buffer.GetHeight() - undermsg.GetHeight(), &undermsg, NULL);
				buffer.SetClipper(&cliprc);
			}
		#endif
	#endif
}

//Back flip: capture the screen to the backbuffer
void ClassEDisplay::BackFlip(void)
{
	if (suspended || videomode == EDSP_NONE)
		return;
	if (videomode != buffer.videomode)
		return;
	if (gldriver)
		return;
	E2DSurface *flipbuf = &buffer;
	if (realvideomode != videomode)
		flipbuf = &exbuffer;
	unsigned char *displock = (unsigned char *)ecDspLock(objdsp, NULL, false);
	if (displock)
	{
		E2DSURFACEINFO bufinfo;
		unsigned char *buflock = buffer.Lock(&bufinfo);
		if (buflock)
		{
			RECT rflip;
			ClassEMemory::Copy(&rflip, &fliprc, sizeof(RECT));
			ReVirtualRect(&rflip, dispinfo.width, dispinfo.height);
			long pos[] = { rflip.left, rflip.top };
			long rect[] = { rflip.left, rflip.top, rflip.right, rflip.bottom };
			#if defined(DEVICE_WIN32)
				if (!fullscreenmode)
				{
					long winrc[4];
					ecInstGetWindowRect(curinst, winrc, true);
					pos[0] += winrc[0];
					pos[1] += winrc[1];
					#if defined(DEVICE_DESKTOP)
						if (ecd->dispcfg->dialogbox > 0)
						{
							pos[0] += ecd->dispcfg->dlgleft;
							pos[1] += ecd->dispcfg->dlgtop;
						}
					#endif
				}
			#endif
			//displock += pos[0] * realxpitch + pos[1] * realypitch;
			displock += (pos[0] - fliprc.left) * realxpitch + (pos[1] - fliprc.top) * realypitch + fliprc.left * dispinfo.xpitch + fliprc.top * dispinfo.ypitch;
			displock += dispinfo.bufferoffset;
			if (prerotation)
			{
				bufinfo.xpitch = bufinfo.bitwidth / 8;
				bufinfo.ypitch = bufinfo.realpitch;
			}
			//ClassEDisplay_PixelCopy(&buflock[rect[0] * bufinfo.bitwidth / 8 + rect[1] * bufinfo.realpitch], &displock[/*rect[0] * dispinfo.xpitch + rect[1] * dispinfo.ypitch*/0], bufinfo.xpitch, bufinfo.ypitch, dispinfo.xpitch, dispinfo.ypitch, rect[2] - rect[0], rect[3] - rect[1], bufinfo.realwidth, bufinfo.realheight, dispinfo.realwidth, dispinfo.realheight, dispinfo.bitwidth);
			ClassEDisplay_PixelCopy(&buflock[rect[0] * bufinfo.xpitch + rect[1] * bufinfo.ypitch], &displock[/*rect[0] * dispinfo.xpitch + rect[1] * dispinfo.ypitch*/0], bufinfo.xpitch, bufinfo.ypitch, dispinfo.xpitch, dispinfo.ypitch, rect[2] - rect[0], rect[3] - rect[1], bufinfo.realwidth, bufinfo.realheight, dispinfo.realwidth, dispinfo.realheight, dispinfo.bitwidth);
			buffer.Unlock();
		}
		ecDspUnlock(objdsp);
	}
	if (realvideomode != videomode)
		buffer.BltFast(0, 0, flipbuf, NULL);
}

//Clear screen
void ClassEDisplay::Clear(bool colorbuffer, bool depthbuffer)
{
	if (gldriver == NULL)
	{
		if (colorbuffer)
			buffer.Clear();
	}
	else
		EdgeGL_Clear(gldriver, colorbuffer, depthbuffer);
}

//Set a rectangle to flip
void ClassEDisplay::SetFlipRect(RECT *rc)
{
	if (rc != NULL)
	{
		ClassEMemory::Copy(&fliprc, rc, sizeof(RECT));
		if (fliprc.left < 0)
			fliprc.left = 0;
		if (fliprc.right > (long)dispinfo.width)
			fliprc.right = dispinfo.width;
		if (fliprc.top < 0)
			fliprc.top = 0;
		if (fliprc.bottom > (long)dispinfo.height)
			fliprc.bottom = dispinfo.height;
	}
	else
		SetRect(&fliprc, 0, 0, dispinfo.width, dispinfo.height);
}

//Create a displacement map for a screen wipe
void ClassEDisplay::MapScreenWipe(unsigned char *dst, unsigned long width, unsigned long height, unsigned char type)
{
	#if (EDITION_SDK >= 1000)
		if (dst == NULL)
			return;
		if (type == EWIPE_SCREENDISSOLVE)
		{
			for (unsigned long yctr = 0; yctr < height; yctr++)
				for (unsigned long xctr = 0; xctr < width; xctr++)
					dst[yctr * width + xctr] = (unsigned char)ecd->math->Rand();
		}
	#endif
}

//Initialize a wipe
void ClassEDisplay::StartWipe(unsigned char type, E2DSurface *dst)
{
	prevwipetimer = 0;
}

//Different wipes between screens
void ClassEDisplay::Wipe(E2DSurface *dst, E2DSurface *src1, E2DSurface *src2, unsigned long timer, unsigned char type, void *customdata, long property)
{
	#if (EDITION_SDK >= 1000)
		if (timer > 65536)
			timer = 65536;
		if (type == EWIPE_SLIDELEFT || type == EWIPE_SLIDERIGHT || type == EWIPE_SLIDEUP || type == EWIPE_SLIDEDOWN)
		{
			if (dst != src1)
				dst->BltFast(0, 0, src1, NULL);
			if (type == EWIPE_SLIDELEFT)
				dst->BltFast(dst->GetWidth() - timer * dst->GetWidth() / 65536, 0, src2, NULL);
			else if (type == EWIPE_SLIDERIGHT)
				dst->BltFast(timer * dst->GetWidth() / 65536 - dst->GetWidth(), 0, src2, NULL);
			else if (type == EWIPE_SLIDEUP)
				dst->BltFast(0, dst->GetHeight() - timer * dst->GetHeight() / 65536, src2, NULL);
			else if (type == EWIPE_SLIDEDOWN)
				dst->BltFast(0, timer * dst->GetHeight() / 65536 - dst->GetHeight(), src2, NULL);
		}
		else if (type == EWIPE_BLOCKS)
		{
			long startx, starty, xctr, yctr, bx, by, maxbx, maxby;
			unsigned short bsize;
			unsigned char fademap[256] =
			{
				  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,
				 59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  16,
				 58, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,  74,  17,
				 57, 110, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 124,  75,  18,
				 56, 109, 154, 191, 192, 193, 194, 195, 196, 197, 198, 199, 166, 125,  76,  19,
				 55, 108, 153, 190, 219, 220, 221, 222, 223, 224, 225, 200, 167, 126,  77,  20,
				 54, 107, 152, 189, 218, 239, 240, 241, 242, 243, 226, 201, 168, 127,  78,  21,
				 53, 106, 151, 188, 217, 238, 251, 252, 253, 244, 227, 202, 169, 128,  79,  22,
				 52, 105, 150, 187, 216, 237, 250, 255, 254, 245, 228, 203, 170, 129,  80,  23,
				 51, 104, 149, 186, 215, 236, 249, 248, 247, 246, 229, 204, 171, 130,  81,  24,
				 50, 103, 148, 185, 214, 235, 234, 233, 232, 231, 230, 205, 172, 131,  82,  25,
				 49, 102, 147, 184, 213, 212, 211, 210, 209, 208, 207, 206, 173, 132,  83,  26,
				 48, 101, 146, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 133,  84,  27,
				 47, 100, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134,  85,  28,
				 46,  99,  98,  97,  96,  95,  94,  93,  92,  91,  90,  89,  88,  87,  86,  29,
				 45,  44,  43,  42,  41,  40,  39,  38,  37,  36,  35,  34,  33,  32,  31,  30
			};
			if (customdata != NULL)
				ClassEMemory::Copy(fademap, customdata, 256);
			if (dst != src1)
			{
				dst->BltFast(0, 0, src1, NULL);
				prevwipetimer = 0;
			}
			timer /= 256;
			if (dst->GetWidth() < dst->GetHeight())
				bsize = (unsigned short)(dst->GetHeight() / 15);
			else
				bsize = (unsigned short)(dst->GetWidth() / 15);
			if (property > 0)
				bsize = (unsigned short)property;
			startx = ((long)dst->GetWidth()  - (long)bsize * 16) / 2;
			starty = ((long)dst->GetHeight() - (long)bsize * 16) / 2;
			if (startx > 0)
				startx = 0;
			if (starty > 0)
				starty = 0;
			if (bsize == 1)
			{
				maxbx = dst->GetWidth()  / 16 + 1;
				maxby = dst->GetHeight() / 16 + 1;
				for (yctr = 0; yctr < 16; yctr++)
					for (xctr = 0; xctr < 16; xctr++)
						if (timer >= fademap[yctr * 16 + xctr] && fademap[yctr * 16 + xctr] >= prevwipetimer)
							for (by = 0; by < maxby; by++)
								for (bx = 0; bx < maxbx; bx++)
								{
									if (src2 != NULL)
										dst->PutPixel(startx + bx * 16 + xctr, starty + by * 16 + yctr, src2->GetPixel(startx + bx * 16 + xctr, starty + by * 16 + yctr));
									else
										dst->PutPixel(startx + bx * 16 + xctr, starty + by * 16 + yctr, RGB(0, 0, 0));
								}
			}
			else if (bsize > 1)
			{
				RECT rc;
				maxbx = dst->GetWidth()  / (bsize * 16) + 1;
				maxby = dst->GetHeight() / (bsize * 16) + 1;
				for (yctr = 0; yctr < 16; yctr++)
					for (xctr = 0; xctr < 16; xctr++)
						if (timer >= fademap[yctr * 16 + xctr] && fademap[yctr * 16 + xctr] >= prevwipetimer)
							for (by = 0; by < maxby; by++)
								for (bx = 0; bx < maxbx; bx++)
								{
									SetRect(&rc, startx + bx * bsize * 16 + xctr * bsize, starty + by * bsize * 16 + yctr * bsize, 0, 0);
									rc.right = rc.left + bsize;
									rc.bottom = rc.top + bsize;
									if (src2 != NULL)
										dst->BltFast(rc.left, rc.top, src2, &rc);
									else
										dst->FillRect(&rc, RGB(0, 0, 0));
								}
			}
			prevwipetimer = timer + 1;
		}
		else if (type == EWIPE_PIXELATE)
		{
			unsigned short bsize;
			long xctr, yctr, px, py;
			RECT rc;
			if (dst->GetWidth() < dst->GetHeight())
				bsize = (unsigned short)(dst->GetHeight() * timer / 65536);
			else
				bsize = (unsigned short)(dst->GetWidth()  * timer / 65536);
			if (bsize >= 2)
				for (yctr = 0; yctr < (signed)dst->GetHeight() / bsize + 1; yctr++)
					for (xctr = 0; xctr < (signed)dst->GetWidth() / bsize + 1; xctr++)
					{
						SetRect(&rc, xctr * bsize, yctr * bsize, (xctr + 1) * bsize, (yctr + 1) * bsize);
						px = rc.left + bsize / 2;
						py = rc.top + bsize / 2;
						if (px >= (long)dst->GetWidth())
							px = dst->GetWidth() - 1;
						if (py >= (long)dst->GetHeight())
							py = dst->GetHeight() - 1;
						dst->FillRect(&rc, src1->GetPixel(px, py));
					}
		}
		else if (type == EWIPE_BLEND)
		{
			dst->BltFast(0, 0, src1, NULL);
			dst->BltFast(0, 0, src2, NULL, EFX_OPACITY, timer * 255 / 65536);
		}
		else if (type == EWIPE_SCREEN)
		{
			unsigned char *d_map = NULL;
			long xctr, yctr, maxbx, maxby;
			if (customdata != NULL)
				d_map = (unsigned char *)customdata;
			if (d_map == NULL)
				return;
			if (dst != src1)
			{
				dst->BltFast(0, 0, src1, NULL);
				prevwipetimer = 0;
			}
			timer /= 256;
			maxbx = dst->GetWidth();
			maxby = dst->GetHeight();
			for (yctr = 0; yctr < maxby; yctr++)
				for (xctr = 0; xctr < maxbx; xctr++)
					if (timer >= d_map[yctr * maxbx + xctr] && d_map[yctr * maxbx + xctr] >= prevwipetimer)
					{
						if (src2 != NULL)
							dst->PutPixel(xctr, yctr, src2->GetPixel(xctr, yctr));
						else
							dst->PutPixel(xctr, yctr, RGB(0, 0, 0));
					}
			prevwipetimer = timer + 1;
		}
	#endif
}

//Set the size of the 3D render engine
void ClassEDisplay::SetRenderSize(unsigned long newsize)
{
	rendersize = newsize;
}

//Get the current amount of video memory
ERESULT ClassEDisplay::GetVideoMemory(unsigned long &memfree, unsigned long &memtotal)
{
	memfree = 0;
	memtotal = 0;
	ecErr err;
	unsigned long vmem[2];
	err = ecDspGetVideoMemory(objdsp, vmem);
	if (err == 0)
	{
		memtotal = vmem[0];
		memfree = vmem[1];
		return(E_OK);
	}
	long errarray[] = { E_OK, E_UNSUPPORTED, E_ERROR };
	return(ecExMapError(err, errarray, 3));
}

//Get the system startup rotation
unsigned char ClassEDisplay::GetSystemRotation(void)
{
	return(sysorientation);
}

//Get the width of the active display
unsigned long ClassEDisplay::GetWidth(void)
{
	if (gldriver != NULL)
	{
		unsigned long x, y;
		EdgeGL_GetVideoSize(gldriver, x, y);
		if ((rotation + 4 - sysorientation ) % 2 == 0)
			return(x);
		else
			return(y);
	}
	return(buffer.GetWidth());
}

//Get the height of the active display
unsigned long ClassEDisplay::GetHeight(void)
{
	if (gldriver != NULL)
	{
		unsigned long x, y;
		EdgeGL_GetVideoSize(gldriver, x, y);
		if ((rotation + 4 - sysorientation ) % 2 == 0)
			return(y);
		else
			return(x);
	}
	return(buffer.GetHeight());
}

//Return world matrix
void ClassEDisplay::GetWorldMatrix(long *matrix)
{
	ClassEMemory::Copy(matrix, mtxworld, sizeof(long) * 16);
}

//Return perspective matrix
void ClassEDisplay::GetPerspectiveMatrix(long *matrix)
{
	ClassEMemory::Copy(matrix, mtxperspective, sizeof(long) * 16);
}

//Get the perspective
void ClassEDisplay::CalculatePerspective(EVECTOR3D *vector, POINT *point)
{
	E3DSurface_Perspective_exp(mtxperspective, point->x, point->y, vector->x, vector->y, vector->z, GetWidth(), GetHeight());
	point->x >>= 20;
	point->y >>= 20;
}

//Change the palette for indexed color modes
void ClassEDisplay::SetPalette(COLORREF *palette, unsigned char index, unsigned long entries)
{
	unsigned short palctr;
	if (index + entries > 256)
		entries = 256 - index;
	for (palctr = index; palctr < index + entries; palctr++)
		videopalette[palctr] = palette[palctr - index];
	quickpalette[256] = EDSP_NONE;
	if (realvideomode == EDSP_INDEXED8)
		ecDspSetPalette(objdsp, videopalette);
	refreshpalette = true;
}

//Get palette data for indexed color modes
void ClassEDisplay::GetPalette(COLORREF *palette, unsigned char index, unsigned long entries)
{
	unsigned short palctr;
	if (index + entries > 256)
		entries = 256 - index;
	for (palctr = index; palctr < index + entries; palctr++)
		palette[palctr - index] = E_RGBX(E_GETR(videopalette[palctr]), E_GETG(videopalette[palctr]), E_GETB(videopalette[palctr]));
}

//Set the light status to on or off
void ClassEDisplay::UseLight(unsigned char nr, bool enable)
{
	if (gldriver == NULL)
	{
		if (lightdata != NULL && nr < E3D_MAXLIGHTS)
			lightdata[nr].enabled = enable;
	}
	else
		EdgeGL_EnableLight(gldriver, nr, enable);
}

//Change light properties
void ClassEDisplay::SetLightSource(unsigned char nr, long x, long y, long z, COLORREF diffuse, COLORREF ambient, COLORREF specular)
{
	if (gldriver == NULL)
	{
		E3DSurface_Normalize_exp(x, y, z, 0);
		x <<= 3;
		y <<= 3;
		z <<= 3;
		if (lightdata != NULL && nr < E3D_MAXLIGHTS)
		{
			lightdata[nr].position[0] = x;
			lightdata[nr].position[1] = y;
			lightdata[nr].position[2] = z;
			lightdata[nr].intensity[0] = E_GETRGB(ambient);
			lightdata[nr].intensity[1] = E_GETRGB(diffuse);
		}
	}
	else
	{
		EdgeGL_LightPosition(gldriver, nr, x, y, -z);
		EdgeGL_LightIntensity(gldriver, nr, ambient, diffuse, specular);
	}
}

//Upload texture to Open GL
ERESULT ClassEDisplay::UploadTexture(E2DSurface *texture, bool uploadrgba, bool cleardata, bool conversion, bool autorepair)
{
	if (texture->currenttype % 16 == EST_NONE)
		return(E_ERROR);
	if (texture->currenttype % 16 == EST_TEXTURE)
		return(E_NODATA);
	if (gldriver == NULL)
		return(E_NOACTION);
	bool texupdate = true;
	if (texture->exthandle == -1)
	{
		texture->exthandle = QueryTextureHandle();
		if (texture->exthandle == -1)
			return(E_UNSUPPORTED);
		texupdate = false;
	}
	texture->autorepair = autorepair;
    texture->reload_alpha = uploadrgba;
    texture->reload_convert = conversion;

    ERESULT result = E_OK;
	unsigned char *udataptr, *lockptr, *udata = NULL;
	unsigned long x, y;
	if (texture->currenttype % 16 == EST_COMPRESSEDTEX)
	{
		WriteCompressedTextureData(texture->exthandle, texture->extwidth, texture->extheight, texture->swidth, texture->sheight, texture->texformat, texture->gdata, texupdate);
	}
	else
	{
		if (uploadrgba)
		{
			udata = (unsigned char *)ClassEMemory::Alloc(texture->swidth * texture->sheight * 4);
			if (udata == NULL)
				return(E_NOMEMORY);
			lockptr = texture->Lock();
			if (lockptr)
			{
				unsigned long ck = texture->GetColorKey();
				unsigned EINT32 *udataptr32;
				udataptr = udata;
				ExBltColorConvert(udataptr, lockptr, texture->swidth, texture->swidth, EDSP_TRUE32_INV, texture->videomode, texture->virtualwidth, texture->sheight, videopalette, quickpalette);
				udataptr32 = (unsigned EINT32 *)udataptr;
				for (y = 0; y < texture->sheight; y++)
				{
					for (x = 0; x < texture->swidth; x++)
					{
						if ((*udataptr32 & 0xFFFFFF) != ck)
							*udataptr32 |= 0xFF000000;
						udataptr32++;
					}
				}
				texture->Unlock();
				WriteTextureData(texture->exthandle, texture->extwidth, texture->extheight, texture->swidth, texture->sheight, EDSP_TRUE32_INV, false, udata, texupdate);
			}
			else
				result = E_ERROR;
		}
		else
		{
			if (texture->videomode == EDSP_INDEXED8)
				conversion = true;
			if (conversion)
			{
				udata = (unsigned char *)ClassEMemory::Alloc(texture->swidth * texture->sheight * 3);
				if (udata == NULL)
					return(E_NOMEMORY);
			}
			lockptr = texture->Lock();
			if (lockptr)
			{
				if (texture->videomode == EDSP_TRUE24_INV)
					conversion = false;
				if (conversion)
				{
					udataptr = udata;
					ExBltColorConvert(udataptr, lockptr, texture->swidth, texture->swidth, EDSP_TRUE24_INV, texture->videomode, texture->virtualwidth, texture->sheight, videopalette, quickpalette);
				}
				if (conversion)
					WriteTextureData(texture->exthandle, texture->extwidth, texture->extheight, texture->swidth, texture->sheight, EDSP_TRUE24_INV, false, udata, texupdate);
				else
					WriteTextureData(texture->exthandle, texture->extwidth, texture->extheight, texture->swidth, texture->sheight, texture->videomode, false, lockptr, texupdate);
				texture->Unlock();
			}
			else
				result = E_ERROR;
		}
	}
	ClassEMemory::DeAlloc(udata);
	if (cleardata)
	{
		if (texture->locked > 0)
		{
			texture->locked = 1;
			texture->Unlock();
		}
		if (ecd != NULL)
		{
			if (texture->currenttype % 16 == EST_SYSMEMORY || texture->currenttype % 16 == EST_COMPRESSEDTEX)
				FULL_FREE_L(texture->gdata);
			FULL_FREE(texture->indexlookup);
		}
		texture->currenttype = EST_TEXTURE;
		texture->locked = 0;
		if (texture->nativesurf)
		{
			ecDspDestroyNativeSurface(objdsp, texture->nativesurf);
			texture->nativesurf = NULL;
		}
	}
	usetex = NULL;
	return(result);
}

//Upload texture to Open GL
ERESULT ClassEDisplay::UploadTexture(E2DSurfaceRGBA *texture, bool cleardata, bool autorepair)
{
	if (texture->currenttype % 16 == EST_NONE)
		return(E_ERROR);
	if (texture->currenttype % 16 == EST_TEXTURE)
		return(E_NODATA);
	if (gldriver == NULL)
		return(E_NOACTION);
	bool texexists = true;
	if (texture->exthandle == -1)
	{
		texture->exthandle = QueryTextureHandle();
		if (texture->exthandle == -1)
			return(E_UNSUPPORTED);
		texexists = false;
	}
	texture->autorepair = autorepair;
	ERESULT result = E_OK;
	unsigned char *udataptr, *lockptr, *udata = NULL;
	unsigned long x, y;
	if (texture->currenttype % 16 == EST_COMPRESSEDTEX)
	{
		WriteCompressedTextureData(texture->exthandle, texture->extwidth, texture->extheight, texture->swidth, texture->sheight, texture->texformat, texture->gdata, texexists);
	}
	else
	{
		lockptr = texture->Lock();
		if (lockptr)
		{
			if (texture->videomode != EDSP_TRUE32_INV || (texture->currenttype & EST_READONLY))
			{
				udata = (unsigned char *)ClassEMemory::Alloc(texture->virtualwidth * texture->sheight * 4);
				if (udata != NULL)
				{
					unsigned EINT32 *udataptr32;
					udataptr = udata;
					udataptr32 = (unsigned EINT32 *)udataptr;
					
					for (y = 0; y < texture->sheight; y++)
					{
						for (x = 0; x < texture->virtualwidth; x++)
						{
							COLORREF col = 0;
							unsigned char opac;
							col = *(unsigned EINT32 *)(&lockptr[y * texture->swidth * 4 + x * 4]) & 0xFFFFFF;
							opac = (unsigned char)(*(unsigned EINT32 *)(&lockptr[y * texture->swidth * 4 + x * 4]) >> 24);
							ClassEDisplay_RevertColor(col, texture->videomode);

							// if readonly, alpha has been premultiplied.
							if (texture->currenttype & EST_READONLY)
							{
								opac = 255 - opac;
								if (opac)
									col = (((((col & 0xFF0000) << 8) / (opac + 1)) & 0xFF0000) | ((((col & 0xFF00) << 8) / (opac + 1)) & 0xFF00) | ((((col & 0xFF) << 8) / (opac + 1)) & 0xFF));
								else
									col = 0;
							}
							*udataptr32 = col | (opac << 24);
							udataptr32++;
						}
					}
				
					WriteTextureData(texture->exthandle, texture->extwidth, texture->extheight, texture->virtualwidth, texture->sheight, EDSP_TRUE32_INV, false, udata, texexists);
				}
				else
					result = E_NOMEMORY;
				ClassEMemory::DeAlloc(udata);
			}
			else
			{
				WriteTextureData(texture->exthandle, texture->extwidth, texture->extheight, texture->swidth, texture->sheight, EDSP_TRUE32_INV, false, lockptr, texexists);
			}
			texture->Unlock();
		}
		else
			result = E_ERROR;
	}
	if (cleardata)
	{
		if (texture->locked > 0)
		{
			texture->locked = 1;
			texture->Unlock();
		}
		if (ecd != NULL)
		{
			if (texture->currenttype % 16 == EST_SYSMEMORY || texture->currenttype % 16 == EST_COMPRESSEDTEX)
				FULL_FREE_L(texture->gdata);
			FULL_FREE_L(texture->cbitmask);
			FULL_FREE_L(texture->xmax);
			FULL_FREE_L(texture->xmin);
			FULL_FREE_L(texture->ledge);
			FULL_FREE_L(texture->redge);
			FULL_FREE_L(texture->xdraw);
		}
		texture->currenttype = EST_TEXTURE;
		texture->locked = 0;
	}
	usetex = NULL;
	return(result);
}

//Bind a 2D surface as texture
void ClassEDisplay::BindTexture(E2DSurface *texture)
{
	usetex = texture;
	if (gldriver != NULL)
		EdgeGL_SelectTexture(gldriver, texture->exthandle);
}

//Bind a 2D surface as texture
void ClassEDisplay::BindTexture(E2DSurfaceRGBA *texture)
{
	usetex = texture;
	if (gldriver != NULL)
		EdgeGL_SelectTexture(gldriver, texture->exthandle);
}

//Change the shading of the 3D renderer
void ClassEDisplay::SetShading(unsigned long type, unsigned long property)
{
	if (gldriver != NULL)
	{
		if (type & E3D_TEXTURE)
			EdgeGL_UseTexturing(gldriver, true);
		else
			EdgeGL_UseTexturing(gldriver, false);
		if (type & E3D_GOURAUD)
			EdgeGL_SetShading(gldriver, 1);
		else if (type & E3D_FLAT)
			EdgeGL_SetShading(gldriver, 0);
	}
	curshading = type;
	curshadingprop = property;
}

//Register a loader
bool ClassEDisplay::RegisterLoader(ClassELoaderBase *loader)
{
	if (loader == NULL)
		return(false);
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_GRAPHICSLOADERS; ctr++)
		if (graphicsloader[ctr] == NULL)
		{
			graphicsloader[ctr] = loader;
			return(true);
		}
	return(false);
}

//Register a loader
bool ClassEDisplay::UnRegisterLoader(ClassELoaderBase *loader)
{
	if (loader == NULL)
		return(false);
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_GRAPHICSLOADERS; ctr++)
		if (graphicsloader[ctr] == loader)
		{
			graphicsloader[ctr] = NULL;
			return(true);
		}
	return(false);
}

//Create a new 2D surface using the specified width/height, use the specified type of surface
ERESULT ClassEDisplay::CreateSurface(E2DSurface *surface, unsigned long width, unsigned long height, unsigned long usetype)
{
	surface->LinkECD(ecd);
	return(surface->Create(width, height, videomode, usetype));
}

//Create a new 2D surface by loading an image from memory data, use the specified type of surface
ERESULT ClassEDisplay::CreateSurface(E2DSurface *surface, void *imgmem, unsigned long memsize, unsigned long usetype)
{
	surface->LinkECD(ecd);
	return(surface->LoadImage(imgmem, memsize, videomode, usetype));
}

//Create a new 2D surface by loading an image file, use the specified type of surface
ERESULT ClassEDisplay::CreateSurface(E2DSurface *surface, char *imgfile, unsigned long usetype)
{
	WCHAR wfile[MAX_EFILESTR];
	ClassEStd::StrCpy(wfile, imgfile);
	surface->LinkECD(ecd);
	return(surface->LoadImage(wfile, videomode, usetype));
}

//Create a new 2D surface by loading an image file, use the specified type of surface
ERESULT ClassEDisplay::CreateSurface(E2DSurface *surface, WCHAR *imgfile, unsigned long usetype)
{
	surface->LinkECD(ecd);
	return(surface->LoadImage(imgfile, videomode, usetype));
}

//Create a new 3D surface using the specified number of polygons
ERESULT ClassEDisplay::CreateSurface(E3DSurface *surface, unsigned long vertices, unsigned long polygons, unsigned long usetype)
{
	surface->LinkECD(ecd);
	return(surface->Create(vertices, polygons, usetype));
}

//Create a new 3D surface by loading a model from memory data
ERESULT ClassEDisplay::CreateSurface(E3DSurface *surface, void *mdlmem, unsigned long memsize, signed char modelsize, unsigned long usetype)
{
	surface->LinkECD(ecd);
	return(surface->LoadModel(mdlmem, memsize, videomode, modelsize, usetype));
}

//Create a new 3D surface by loading a model file
ERESULT ClassEDisplay::CreateSurface(E3DSurface *surface, char *mdlfile, signed char modelsize, unsigned long usetype)
{
	WCHAR wfile[MAX_EFILESTR];
	ClassEStd::StrCpy(wfile, mdlfile);
	surface->LinkECD(ecd);
	return(surface->LoadModel(wfile, videomode, modelsize, usetype));
}

//Create a new 3D surface by loading a model file
ERESULT ClassEDisplay::CreateSurface(E3DSurface *surface, WCHAR *mdlfile, signed char modelsize, unsigned long usetype)
{
	surface->LinkECD(ecd);
	return(surface->LoadModel(mdlfile, videomode, modelsize, usetype));
}

//Create a new 2D surface using the specified width/height, use the specified type of surface
ERESULT ClassEDisplay::CreateSurface(E2DSurfaceRGBA *surface, unsigned long width, unsigned long height, unsigned long usetype)
{
	surface->LinkECD(ecd);
	return(surface->Create(width, height, videomode, usetype));
}

//Create a new 2D surface by loading an image from memory data, use the specified type of surface
ERESULT ClassEDisplay::CreateSurface(E2DSurfaceRGBA *surface, void *imgmem, unsigned long memsize, unsigned long usetype)
{
	surface->LinkECD(ecd);
	return(surface->LoadImage(imgmem, memsize, videomode, usetype));
}

//Create a new 2D surface by loading an image file, use the specified type of surface
ERESULT ClassEDisplay::CreateSurface(E2DSurfaceRGBA *surface, char *imgfile, unsigned long usetype)
{
	WCHAR wfile[MAX_EFILESTR];
	ClassEStd::StrCpy(wfile, imgfile);
	surface->LinkECD(ecd);
	return(surface->LoadImage(wfile, videomode, usetype));
}

//Create a new 2D surface by loading an image file, use the specified type of surface
ERESULT ClassEDisplay::CreateSurface(E2DSurfaceRGBA *surface, WCHAR *imgfile, unsigned long usetype)
{
	surface->LinkECD(ecd);
	return(surface->LoadImage(imgfile, videomode, usetype));
}

//Set perspective transformation matrix manually
void ClassEDisplay::SetPerspectiveMatrix(long *matrix)
{
	ClassEMemory::Copy(mtxperspective, matrix, 64);
	if (gldriver)
		EdgeGL_PerspectiveMatrix(gldriver, mtxperspective);
}

//Enable or disable perspective correct texture mapping
void ClassEDisplay::SetTextureMapMethod(unsigned char method)
{
	perspectivemapping = (method == ETEX_PERSPECTIVE);
}

//Set the display perspective based on a frustum
void ClassEDisplay::Frustum(long xmin, long xmax, long ymin, long ymax, long znear, long zfar)
{
	if (gldriver != NULL)
	{
		long coords[] = { xmin, xmax, ymin, ymax, znear, zfar };
		EdgeGL_Perspective(gldriver, coords);
	}
	mtxperspective[ 0] = ((2 * znear) << 12) / (xmax - xmin);
	mtxperspective[ 4] = 0;
	mtxperspective[ 8] = 0;
	mtxperspective[12] = 0;
	mtxperspective[ 1] = 0;
	mtxperspective[ 5] = ((2 * znear) << 12) / (ymax - ymin);
	mtxperspective[ 9] = 0;
	mtxperspective[13] = 0;
	mtxperspective[ 2] = ((xmax + xmin) << 12) / (xmax - xmin);
	mtxperspective[ 6] = ((ymax + ymin) << 12) / (ymax - ymin);
	mtxperspective[10] = -((zfar + znear) << 12) / (zfar - znear);
	mtxperspective[14] = -4096;
	mtxperspective[ 3] = 0;
	mtxperspective[ 7] = 0;
	mtxperspective[11] = -(2 * zfar * znear) / (zfar - znear);
	mtxperspective[15] = 0;
	if (mtxperspective[11] > -4096)
		mtxperspective[11] = -4096;
}

//Set clipper to buffer or OpenGL
void ClassEDisplay::SetClipper(RECT *rc)
{
	if (gldriver == NULL)
	{
		buffer.SetClipper(rc);
		return;
	}
	long screct[4];
	if (rc)
	{
		if (yreverse)
		{
			screct[1] = GetHeight() - rc->bottom;
			screct[3] = GetHeight() - rc->top;
		}
		else
		{
			screct[1] = rc->top;
			screct[3] = rc->bottom;
		}
		screct[0] = rc->left;
		screct[2] = rc->right;
	}
	else
		screct[0] = -1;
	EdgeGL_Scissor(gldriver, screct);
}

//Change OpenGL perspective to 2D
void ClassEDisplay::Perspective2D(long width, long height, bool ypointsup)
{
	if (gldriver == NULL)
		return;
	long coords[] = { 0, 0, width, height };
	if (!ypointsup)
	{
		coords[1] = height;
		coords[3] = 0;
	}
	EdgeGL_Ortho(gldriver, coords);
	yreverse = !ypointsup;
}

//Change OpenGL perspective to 3D
void ClassEDisplay::Perspective3D(long width, long height, long fov, long xoffs, long yoffs)
{
	long znear = 4096, zfar = 4096 * 100;
	long xmin, xmax, ymin, ymax;
	ymax = znear * ecd->math->Tan(fov / 2) / 65536;
	ymin = -ymax;
	xmin = ymin * width / height;
	xmax = ymax * width / height;
	Frustum(xmin + xoffs, xmax + xoffs, ymin + yoffs, ymax + yoffs, znear, zfar);
}

//Reset the world orientation
void ClassEDisplay::WorldReset(void)
{
	ClassEMath::MatrixLoadIdentity(mtxworld);
}

//Translate world orientation
void ClassEDisplay::WorldTranslate(long x, long y, long z)
{
	if (x != 0 || y != 0 || z != 0)
	{
		long imatrix[] = {
			4096, 0, 0, 0,
			0, 4096, 0, 0,
			0, 0, 4096, 0,
			x, y, z, 4096
		};
		ecd->math->MatrixMultiply(mtxworld, imatrix);
	}
}

//Rotate world orientation
void ClassEDisplay::WorldRotate(long xr, long yr, long zr)
{
	if (xr != 0 || yr != 0 || zr != 0)
	{
		long imatrix[] = {
			4096, 0, 0, 0,
			0, 4096, 0, 0,
			0, 0, 4096, 0,
			0, 0, 0, 4096
		};
		ecd->math->MatrixSetRotate((unsigned short)xr, (unsigned short)yr, (unsigned short)zr, imatrix);
		ecd->math->MatrixMultiply(mtxworld, imatrix);
	}
}

//Scale world orientation
void ClassEDisplay::WorldScale(long xz, long yz, long zz)
{
	if (xz != 65536 || yz != 65536 || zz != 65536)
	{
		long scalematrix[] = {
			xz >> 4, 0, 0, 0,
			0, yz >> 4, 0, 0,
			0, 0, zz >> 4, 0,
			0, 0, 0, 4096
		};
		ClassEMath::MatrixMultiply(mtxworld, scalematrix);
	}
}

//Draw text using a font to the active display interface
void ClassEDisplay::DrawFont(long x, long y, E2DSurface *source, const char *text, long flags)
{
	if (gldriver == NULL)
	{
		buffer.DrawFont(x, y, source, text, EFX_COLORKEY | flags | (curshading & EFX_COLORFILTER), curshadingprop);
		return;
	}
	if (source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	FONTDRAW_USERDATA_DISPLAY info;
	info.dest = this;
	info.source = source;
	info.srcrgba = false;
	FontDraw_Draw(ClassEDisplay_DrawFontFunc, x, y, (const unsigned char *)text, 0, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, false, &info);
}

//Draw text using a font to the active display interface
void ClassEDisplay::DrawFont(long x, long y, E2DSurface *source, const WCHAR *text, long flags)
{
	if (gldriver == NULL)
	{
		buffer.DrawFont(x, y, source, text, EFX_COLORKEY | flags | (curshading & EFX_COLORFILTER), curshadingprop);
		return;
	}
	if (source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	FONTDRAW_USERDATA_DISPLAY info;
	info.dest = this;
	info.source = source;
	info.srcrgba = false;
	FontDraw_Draw(ClassEDisplay_DrawFontFunc, x, y, (const unsigned char *)text, 0, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, true, &info);
}

//Draw text using a font to the active display interface
void ClassEDisplay::DrawFont(long x, long y, E2DSurface *source, long value, long flags)
{
	char valuestr[32];
	if (value < 0)
	{
		valuestr[0] = '-';
		value = ClassEMath::Abs(value);
		ClassEStd::IntToStr(&valuestr[1], value, 10);
	}
	else
		ClassEStd::IntToStr(valuestr, value, 10);
	DrawFont(x, y, source, valuestr, flags);
}

//Draw text using a font to the active display interface
void ClassEDisplay::DrawFont(long x, long y, E2DSurfaceRGBA *source, const char *text, long flags)
{
	if (gldriver == NULL)
	{
		buffer.DrawFont(x, y, source, text, EFX_COLORKEY | flags | (curshading & EFX_COLORFILTER), curshadingprop);
		return;
	}
	if (source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	FONTDRAW_USERDATA_DISPLAY info;
	info.dest = this;
	info.source = source;
	info.srcrgba = true;
	FontDraw_Draw(ClassEDisplay_DrawFontFunc, x, y, (const unsigned char *)text, 0, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, false, &info);
}

//Draw text using a font to the active display interface
void ClassEDisplay::DrawFont(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, long flags)
{
	if (gldriver == NULL)
	{
		buffer.DrawFont(x, y, source, text, EFX_COLORKEY | flags | (curshading & EFX_COLORFILTER), curshadingprop);
		return;
	}
	if (source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	FONTDRAW_USERDATA_DISPLAY info;
	info.dest = this;
	info.source = source;
	info.srcrgba = true;
	FontDraw_Draw(ClassEDisplay_DrawFontFunc, x, y, (const unsigned char *)text, 0, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, true, &info);
}

//Draw text using a font to the active display interface
void ClassEDisplay::DrawFont(long x, long y, E2DSurfaceRGBA *source, long value, long flags)
{
	char valuestr[32];
	if (value < 0)
	{
		valuestr[0] = '-';
		value = ClassEMath::Abs(value);
		ClassEStd::IntToStr(&valuestr[1], value, 10);
	}
	else
		ClassEStd::IntToStr(valuestr, value, 10);
	DrawFont(x, y, source, valuestr, flags);
}

void ClassEDisplay::DrawFontWrap(long x, long y, E2DSurface *source, const char *text, unsigned long width, long flags)
{
	if (gldriver == NULL)
	{
		buffer.DrawFontWrap(x, y, source, text, width, EFX_COLORKEY | flags | (curshading & EFX_COLORFILTER), curshadingprop);
		return;
	}
	if (source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	FONTDRAW_USERDATA_DISPLAY info;
	info.dest = this;
	info.source = source;
	info.srcrgba = false;
	info.flags = flags;
	FontDraw_Draw(ClassEDisplay_DrawFontFunc, x, y, (const unsigned char *)text, width, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, false, &info);
}

void ClassEDisplay::DrawFontWrap(long x, long y, E2DSurface *source, const WCHAR *text, unsigned long width, long flags)
{
	if (gldriver == NULL)
	{
		buffer.DrawFontWrap(x, y, source, text, width, EFX_COLORKEY | flags | (curshading & EFX_COLORFILTER), curshadingprop);
		return;
	}
	if (source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	FONTDRAW_USERDATA_DISPLAY info;
	info.dest = this;
	info.source = source;
	info.srcrgba = false;
	info.flags = flags;
	FontDraw_Draw(ClassEDisplay_DrawFontFunc, x, y, (const unsigned char *)text, width, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, true, &info);
}


void ClassEDisplay::DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const char *text, unsigned long width, long flags)
{
	if (gldriver == NULL)
	{
		buffer.DrawFontWrap(x, y, source, text, width, EFX_COLORKEY | flags | (curshading & EFX_COLORFILTER), curshadingprop);
		return;
	}
	if (source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	FONTDRAW_USERDATA_DISPLAY info;
	info.dest = this;
	info.source = source;
	info.srcrgba = true;
	info.flags = flags;
	FontDraw_Draw(ClassEDisplay_DrawFontFunc, x, y, (const unsigned char *)text, width, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, false, &info);
}

void ClassEDisplay::DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, unsigned long width, long flags)
{
	if (gldriver == NULL)
	{
		buffer.DrawFontWrap(x, y, source, text, width, EFX_COLORKEY | flags | (curshading & EFX_COLORFILTER), curshadingprop);
		return;
	}
	if (source->currenttype % 16 == EST_NONE || source->fontrc == NULL)
		return;
	FONTDRAW_USERDATA_DISPLAY info;
	info.dest = this;
	info.source = source;
	info.srcrgba = true;
	info.flags = flags;
	FontDraw_Draw(ClassEDisplay_DrawFontFunc, x, y, (const unsigned char *)text, width, flags, source->fontcharset, source->fontrc, source->fontkerning, source->fontinfo.defaultchar, true, &info);
}

//Fill a rectangle
void ClassEDisplay::FillRect(RECT *rc, COLORREF col)
{
	if (gldriver == NULL)
	{
		buffer.FillRect(rc, E_GETRGB(col), EFX_OPACITY, E_GETA(col));
		return;
	}
	RECT drawrc;
	if (rc == NULL)
		SetRect(&drawrc, 0, 0, GetWidth(), GetHeight());
	else
		ClassEMemory::Copy(&drawrc, rc, sizeof(RECT));
	EINT32 trivtx[] =
	{
		drawrc.left, drawrc.top, 0,
		drawrc.right, drawrc.top, 0,
		drawrc.left, drawrc.bottom, 0,
		drawrc.right, drawrc.bottom, 0
	};
	void *rdrarray[] = { trivtx, NULL, NULL, NULL, NULL };
	long strides[] = { sizeof(EINT32) * 3, 0, 0, 0 };
	unsigned long prevshading = curshading;
	EdgeGL_SetColor(gldriver, col, 1);
	SetShading(E3D_FLAT);
	EdgeGL_RenderModel(gldriver, rdrarray, strides, 4, false, true);
	EdgeGL_SetColor(gldriver, E_RGBA(255, 255, 255, 255), 1);
	SetShading(prevshading);
}

void ClassEDisplay::DrawRect(RECT *rc, COLORREF col)
{
	if (gldriver == NULL)
	{
		buffer.DrawRect(rc, E_GETRGB(col), EFX_OPACITY, E_GETA(col));
		return;
	}
	RECT drawrc;
	if (rc == NULL)
		SetRect(&drawrc, 0, 0, GetWidth(), GetHeight());
	else
		ClassEMemory::Copy(&drawrc, rc, sizeof(RECT));
	EINT32 trivtx[] =
	{
		drawrc.left, drawrc.top, 0,
		drawrc.left, drawrc.bottom, 0,
		drawrc.right, drawrc.bottom, 0,
		drawrc.right, drawrc.top, 0,
		drawrc.left, drawrc.top, 0
	};
	void *rdrarray[] = { trivtx, NULL, NULL, NULL, NULL };
	long strides[] = { sizeof(EINT32) * 3, 0, 0, 0 };
	unsigned long prevshading = curshading;
	EdgeGL_SetColor(gldriver, col, 1);
	SetShading(E3D_FLAT);
	EdgeGL_RenderModel(gldriver, rdrarray, strides, 5, true, true);
	EdgeGL_SetColor(gldriver, E_RGBA(255, 255, 255, 255), 1);
	SetShading(prevshading);
}

void ClassEDisplay::DrawLine(long x1, long y1, long x2, long y2, COLORREF col)
{
	if (gldriver == NULL)
	{
		buffer.DrawLine(x1, y1, x2, y2, E_GETRGB(col), EFX_OPACITY, E_GETA(col));
		return;
	}
	EINT32 trivtx[] =
	{
		x1, y1, 0,
		x2, y2, 0
	};
	void *rdrarray[] = { trivtx, NULL, NULL, NULL, NULL };
	long strides[] = { sizeof(EINT32) * 3, 0, 0, 0 };
	unsigned long prevshading = curshading;
	EdgeGL_SetColor(gldriver, col, 1);
	SetShading(E3D_FLAT);
	EdgeGL_RenderModel(gldriver, rdrarray, strides, 2, true, true);
	EdgeGL_SetColor(gldriver, E_RGBA(255, 255, 255, 255), 1);
	SetShading(prevshading);
}

//Bltfast to the active display interface
void ClassEDisplay::BltFast(long x, long y, E2DSurface *source, RECT *rc)
{
	if (gldriver == NULL)
	{
		buffer.BltFast(x, y, source, rc, curshading & EFX_COLORFILTER, curshadingprop);
		return;
	}
	RECT drawrc;
	long swidth = source->GetWidth(), sheight = source->GetHeight();
	if (swidth == 0 || sheight == 0)
		return;
	if (source->exthandle == -1)
		return;
	if (usetex != source)
		BindTexture(source);
	if (rc == NULL)
		SetRect(&drawrc, 0, 0, swidth, sheight);
	else
		ClassEMemory::Copy(&drawrc, rc, sizeof(RECT));
	if (curshading & (EFX_OPACITY | EFX_COLORIZE))
	{
		COLORREF modifcol;
		if (curshading & EFX_COLORIZE)
			modifcol = curshadingprop;
		else
			modifcol = E_RGBA(255, 255, 255, curshadingprop);
		EdgeGL_SetTextureBlending(gldriver, 0, 1); //Texture blend: modulate
		EdgeGL_SetColor(gldriver, modifcol, 1);
	}
	if (curshading & EFX_ADD)
		EdgeGL_SetTextureBlending(gldriver, 1, 2); //Enable blending to background (additive)
	else
		EdgeGL_SetTextureBlending(gldriver, 1, 1); //Enable blending to background (opacity)
	ClassEDisplay_GLBltFast(x, y, source->exthandle, &drawrc, source->extwidth, source->extheight, yreverse, gldriver);
	if (curshading & (EFX_OPACITY | EFX_COLORIZE))
		EdgeGL_SetColor(gldriver, E_RGBA(255, 255, 255, 255), 1);
	if (curshading & EFX_ADD)
		EdgeGL_SetTextureBlending(gldriver, 1, 1); //Enable blending to background (opacity)
}

//Bltfast to the active display interface
void ClassEDisplay::BltFast(long x, long y, E2DSurfaceRGBA *source, RECT *rc)
{
	if (gldriver == NULL)
	{
		buffer.BltFast(x, y, source, rc, curshading & EFX_COLORFILTER, curshadingprop);
		return;
	}
	RECT drawrc;
	long swidth = source->GetWidth(), sheight = source->GetHeight();
	if (swidth == 0 || sheight == 0)
		return;
	if (source->exthandle == -1)
		return;
	if (usetex != source)
		BindTexture(source);
	if (rc == NULL)
		SetRect(&drawrc, 0, 0, swidth, sheight);
	else
		ClassEMemory::Copy(&drawrc, rc, sizeof(RECT));
	if (curshading & (EFX_OPACITY | EFX_COLORIZE))
	{
		COLORREF modifcol;
		if (curshading & EFX_COLORIZE)
			modifcol = curshadingprop;
		else
			modifcol = E_RGBA(255, 255, 255, curshadingprop);
		EdgeGL_SetTextureBlending(gldriver, 0, 1); //Texture blend: modulate
		EdgeGL_SetColor(gldriver, modifcol, 1);
	}
	if (curshading & EFX_ADD)
		EdgeGL_SetTextureBlending(gldriver, 1, 2); //Enable blending to background (additive)
	else
		EdgeGL_SetTextureBlending(gldriver, 1, 1); //Enable blending to background (opacity)
	ClassEDisplay_GLBltFast(x, y, source->exthandle, &drawrc, source->extwidth, source->extheight, yreverse, gldriver);
	if (curshading & (EFX_OPACITY | EFX_COLORIZE))
		EdgeGL_SetColor(gldriver, E_RGBA(255, 255, 255, 255), 1);
	if (curshading & EFX_ADD)
		EdgeGL_SetTextureBlending(gldriver, 1, 1); //Enable blending to background (opacity)
}

//Blt to the active display interface
void ClassEDisplay::Blt(long center_x, long center_y, E2DSurface *source, RECT *rc, unsigned short rotation, unsigned long scale, unsigned char flipflags)
{
	E2DBLTFX tmpfx;
	tmpfx.flags = 0;
	tmpfx.flipflags = flipflags;
	tmpfx.property = 0;
	tmpfx.rotation = rotation;
	tmpfx.xscale = scale;
	tmpfx.yscale = scale;
	BltFx(center_x, center_y, source, rc, &tmpfx);
}

//Blt to the active display interface
void ClassEDisplay::Blt(long center_x, long center_y, E2DSurfaceRGBA *source, RECT *rc, unsigned short rotation, unsigned long scale, unsigned char flipflags)
{
	E2DBLTFX tmpfx;
	tmpfx.flags = curshading & EFX_COLORFILTER;
	tmpfx.flipflags = flipflags;
	tmpfx.property = curshadingprop;
	tmpfx.rotation = rotation;
	tmpfx.xscale = scale;
	tmpfx.yscale = scale;
	BltFx(center_x, center_y, source, rc, &tmpfx);
}

//Blt to the active display interface with effect
void ClassEDisplay::BltFx(long center_x, long center_y, E2DSurface *source, RECT *rc, E2DBLTFX *bltfx)
{
	if (gldriver == NULL)
	{
		buffer.BltFx(center_x, center_y, source, rc, bltfx);
		return;
	}
	RECT drawrc;
	long swidth = source->GetWidth(), sheight = source->GetHeight();
	if (swidth == 0 || sheight == 0)
		return;
	if (source->exthandle == -1)
		return;
	if (usetex != source)
		BindTexture(source);
	if (rc == NULL)
		SetRect(&drawrc, 0, 0, swidth, sheight);
	else
		ClassEMemory::Copy(&drawrc, rc, sizeof(RECT));
	if (curshading & (EFX_OPACITY | EFX_COLORIZE))
	{
		COLORREF modifcol;
		if (curshading & EFX_COLORIZE)
			modifcol = curshadingprop;
		else
			modifcol = E_RGBA(255, 255, 255, curshadingprop);
		EdgeGL_SetTextureBlending(gldriver, 0, 1); //Texture blend: modulate
		EdgeGL_SetColor(gldriver, modifcol, 1);
	}
	if (curshading & EFX_ADD)
		EdgeGL_SetTextureBlending(gldriver, 1, 2); //Enable blending to background (additive)
	else
		EdgeGL_SetTextureBlending(gldriver, 1, 1); //Enable blending to background (opacity)
	ClassEDisplay_GLBltFx(center_x, center_y, source->exthandle, &drawrc, source->extwidth, source->extheight, yreverse, gldriver, bltfx, ecd);
	if (curshading & (EFX_OPACITY | EFX_COLORIZE))
		EdgeGL_SetColor(gldriver, E_RGBA(255, 255, 255, 255), 1);
	if (curshading & EFX_ADD)
		EdgeGL_SetTextureBlending(gldriver, 1, 1); //Enable blending to background (opacity)
}

//Blt to the active display interface with effect
void ClassEDisplay::BltFx(long center_x, long center_y, E2DSurfaceRGBA *source, RECT *rc, E2DBLTFX *bltfx)
{
	if (gldriver == NULL)
		return;
	RECT drawrc;
	long swidth = source->GetWidth(), sheight = source->GetHeight();
	if (swidth == 0 || sheight == 0)
		return;
	if (source->exthandle == -1)
		return;
	if (usetex != source)
		BindTexture(source);
	if (rc == NULL)
		SetRect(&drawrc, 0, 0, swidth, sheight);
	else
		ClassEMemory::Copy(&drawrc, rc, sizeof(RECT));
	if (curshading & (EFX_OPACITY | EFX_COLORIZE))
	{
		COLORREF modifcol;
		if (curshading & EFX_COLORIZE)
			modifcol = curshadingprop;
		else
			modifcol = E_RGBA(255, 255, 255, curshadingprop);
		EdgeGL_SetTextureBlending(gldriver, 0, 1); //Texture blend: modulate
		EdgeGL_SetColor(gldriver, modifcol, 1);
	}
	if (curshading & EFX_ADD)
		EdgeGL_SetTextureBlending(gldriver, 1, 2); //Enable blending to background (additive)
	else
		EdgeGL_SetTextureBlending(gldriver, 1, 1); //Enable blending to background (opacity)
	ClassEDisplay_GLBltFx(center_x, center_y, source->exthandle, &drawrc, source->extwidth, source->extheight, yreverse, gldriver, bltfx, ecd);
	if (curshading & (EFX_OPACITY | EFX_COLORIZE))
		EdgeGL_SetColor(gldriver, E_RGBA(255, 255, 255, 255), 1);
	if (curshading & EFX_ADD)
		EdgeGL_SetTextureBlending(gldriver, 1, 1); //Enable blending to background (opacity)
}

void ClassEDisplay::DrawOval(long x, long y, long xradius, long yradius, COLORREF col)
{
	if (gldriver == NULL)
	{
		buffer.DrawOval(x, y, xradius, yradius, E_GETRGB(col), EFX_OPACITY, E_GETA(col));
		return;
	}

	long yctr, sinv, cosv;	
	yradius = ClassEMath::Abs(yradius);
	xradius = ClassEMath::Abs(xradius);

	EINT32 arcvtx[771];
	
	for (yctr = 0; yctr <= 256; yctr++)
	{
		cosv = (ecd->math->Cos(yctr << 4) * yradius) >> 16;
		sinv = (ecd->math->Sin(yctr << 4) * xradius) >> 16;
		arcvtx[3*yctr] = x + sinv;
		arcvtx[3*yctr+1] = y + cosv;
		arcvtx[3*yctr+2] = 0;
	}
	void *rdrarray[] = { arcvtx, NULL, NULL, NULL, NULL };
	long strides[] = { sizeof(EINT32) * 3, 0, 0, 0 };
	unsigned long prevshading = curshading;
	EdgeGL_SetColor(gldriver, col, 1);
	SetShading(E3D_FLAT);
	EdgeGL_RenderModel(gldriver, rdrarray, strides, 257, true, true);
	EdgeGL_SetColor(gldriver, E_RGBA(255, 255, 255, 255), 1);
	SetShading(prevshading);

}

void ClassEDisplay::FillOval(long x, long y, long xradius, long yradius, COLORREF col)
{
	if (gldriver == NULL)
	{
		buffer.FillOval(x, y, xradius, yradius, E_GETRGB(col), EFX_OPACITY, E_GETA(col));
		return;
	}

	long yctr, sinv, cosv;	
	yradius = ClassEMath::Abs(yradius);
	xradius = ClassEMath::Abs(xradius);

	EINT32 arcvtx[1542];

	for (yctr = 0; yctr <= 256; yctr++)
	{
		cosv = (ecd->math->Cos(yctr << 4) * yradius) >> 16;
		sinv = (ecd->math->Sin(yctr << 4) * xradius) >> 16;
		arcvtx[6*yctr+0] = x;
		arcvtx[6*yctr+1] = y;
		arcvtx[6*yctr+2] = 0;
		arcvtx[6*yctr+3] = x + sinv;
		arcvtx[6*yctr+4] = y + cosv;
		arcvtx[6*yctr+5] = 0;
	}
	void *rdrarray[] = { arcvtx, NULL, NULL, NULL, NULL };
	long strides[] = { sizeof(EINT32) * 3, 0, 0, 0 };
	unsigned long prevshading = curshading;
	EdgeGL_SetColor(gldriver, col, 1);
	SetShading(E3D_FLAT);
	EdgeGL_RenderModel(gldriver, rdrarray, strides, 514, false, true);
	EdgeGL_SetColor(gldriver, E_RGBA(255, 255, 255, 255), 1);
	SetShading(prevshading);

}

//Render a 3D model to the active display interface
void ClassEDisplay::Render(E3DSurface *surface)
{
	if (gldriver == NULL)
	{
		surface->wmatrix = mtxworld;
		buffer.Render(0, 0, surface, curshading, curshadingprop);
		surface->wmatrix = NULL;
		return;
	}
	E3DSURFACEINFO info;
	if (surface->Lock(&info, true))
	{
		void *rdrarray[] = { NULL, NULL, NULL, NULL, NULL };
		long strides[] = { info.vertexstride, info.normalstride, info.colorstride, info.texcoordstride };
		rdrarray[0] = info.vertexarray;
		rdrarray[1] = info.normalarray;
		rdrarray[2] = info.colorarray;
		rdrarray[3] = info.texcoordarray;
		if (info.useindices)
			rdrarray[4] = info.indexarray;
		EdgeGL_PushMatrix(gldriver);
		EdgeGL_IdentityMatrix(gldriver);
		EdgeGL_MultiplyMatrix(gldriver, mtxworld);
		EdgeGL_MultiplyMatrix(gldriver, info.matrix);
		EdgeGL_RenderModel(gldriver, rdrarray, strides, surface->PolygonsInUse(), (curshading & E3D_WIREFRAME) == E3D_WIREFRAME, false);
		EdgeGL_PopMatrix(gldriver);
		surface->Unlock();
	}
}


unsigned char ClassEDisplay::GetStencilBits(void)
{
	if (gldriver == NULL) return 0;
	return EdgeGL_GetStencilBits(gldriver);
}

