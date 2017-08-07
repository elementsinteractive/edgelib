// edgemain.h
// Contains functions that can't be linked inside the library.
// Include it once in the .cpp file that contains the
// EdgeMain() function
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Include the Edge headers                                      //
///////////////////////////////////////////////////////////////////

#define E_INCLUDEONCE
#include "edge.h"
#undef  E_INCLUDEONCE

#include "edgerender.h"

#if defined(ESOUND_HEKKUS)
	#include "esound/hekkus.h"
#elif defined(ESOUND_OPENAL)
	#include "esound/openal.h"
#elif defined(ESOUND_ANTIX) || defined(ESOUND_AGP)
	#include "esound/antix.h"
#elif !defined(ESOUND_CUSTOM)
	#include "esound/silent.h"
#endif

#if !defined(EGL_USEGL)
	#include "extapi/gldummy.h"
#endif

///////////////////////////////////////////////////////////////////
// Handle application UID
///////////////////////////////////////////////////////////////////

//Use the Edge application UID when none has been defined
#ifndef APP_UID
	#if (DEVICE_SYMBIAN >= 900)
		#define APP_UID 0xF0205D9D
	#else
		#define APP_UID 0x10205D9D
	#endif
#endif

//Get the current application UID in integer format
unsigned long EdgeGetUIDNr(void){ return(APP_UID); }

///////////////////////////////////////////////////////////////////
// Add default Edge loaders unless defined otherwise             //
///////////////////////////////////////////////////////////////////

#ifndef EDGE_NODEFAULTPLUGINS

bool RegisterDisplayLoaders(ClassEDisplay *display)
{
	ELoader_BMP *bmp = new ELoader_BMP;
	if (bmp == NULL)
		return(false);
	if (!display->RegisterLoader(bmp))
	{
		delete bmp;
		return(false);
	}
	ELoader_GIF *gif = new ELoader_GIF;
	if (gif == NULL)
		return(false);
	if (!display->RegisterLoader(gif))
	{
		delete gif;
		return(false);
	}
	ELoader_PNG *png = new ELoader_PNG;
	if (png == NULL)
		return(false);
	if (!display->RegisterLoader(png))
	{
		delete png;
		return(false);
	}
	ELoader_JPG *jpg = new ELoader_JPG;
	if (jpg == NULL)
		return(false);
	if (!display->RegisterLoader(jpg))
	{
		delete jpg;
		return(false);
	}
	ELoader_TGA *tga = new ELoader_TGA;
	if (tga == NULL)
		return(false);
	if (!display->RegisterLoader(tga))
	{
		delete tga;
		return(false);
	}
	ELoader_3DS *l3ds = new ELoader_3DS;
	if (l3ds == NULL)
		return(false);
	if (!display->RegisterLoader(l3ds))
	{
		delete l3ds;
		return(false);
	}
	ELoader_MS3D *lms3d = new ELoader_MS3D;
	if (lms3d == NULL)
		return(false);
	if (!display->RegisterLoader(lms3d))
	{
		delete lms3d;
		return(false);
	}
	ELoader_E3D *e3d = new ELoader_E3D;
	if (e3d == NULL)
		return(false);
	if (!display->RegisterLoader(e3d))
	{
		delete e3d;
		return(false);
	}
	return(true);
}

#else

bool RegisterDisplayLoaders(ClassEDisplay *display){ return(true); }

#endif

///////////////////////////////////////////////////////////////////
// 4 Default pixel shaders                                       //
///////////////////////////////////////////////////////////////////

//AND the source and destination pixel
unsigned long EPShader_AND(unsigned long dstpixel, unsigned long srcpixel, unsigned long x, unsigned long y, long param)
{
	return(dstpixel & srcpixel);
}

//OR the source and destination pixel
unsigned long EPShader_OR(unsigned long dstpixel, unsigned long srcpixel, unsigned long x, unsigned long y, long param)
{
	return(dstpixel | srcpixel);
}

//XOR the source and destination pixel
unsigned long EPShader_XOR(unsigned long dstpixel, unsigned long srcpixel, unsigned long x, unsigned long y, long param)
{
	return(dstpixel ^ srcpixel);
}

//Convert the source pixel to grayscale
unsigned long EPShader_Grayscale(unsigned long dstpixel, unsigned long srcpixel, unsigned long x, unsigned long y, long param)
{
	unsigned long col = (((srcpixel & 0xFF0000) >> 16) + ((srcpixel & 0xFF00) >> 8) + (srcpixel & 0xFF));
	col = col / 3;
	return(col | (col << 8) | (col << 16));
}

///////////////////////////////////////////////////////////////////
// Open-source blitters for use with a pixelshader               //
///////////////////////////////////////////////////////////////////

//A generic blitter to use with a pixel shader
void E2DSurface::MemBlt(EPIXELSHADER, long x, long y, unsigned char *dst, unsigned char *src, RECT *rc, unsigned long dstpitch, unsigned long srcpitch, unsigned char videomode, unsigned long property)
{
	unsigned long xctr, yctr = rc->top, lw = rc->right - rc->left;
	if (videomode == EDSP_INDEXED8)
	{
		unsigned char *pdd8 = &dst[y * dstpitch + x];
		unsigned char *psd8 = &src[yctr * srcpitch + rc->left];
		while(yctr < (unsigned long)rc->bottom)
		{
			for (xctr = 0; xctr < lw; xctr++)
				pdd8[xctr] = psd8[xctr];
			pdd8 += dstpitch; psd8 += srcpitch; yctr++;
		}
		//no pixel shader plugin support for 8-bit indexed mode
	}
	else if (videomode == EDSP_TRUE12 || videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
	{
		unsigned long tsc, tdc;
		unsigned short *pdd16 = (unsigned short *)&dst[(y * dstpitch + x) * 2];
		unsigned short *psd16 = (unsigned short *)&src[(yctr * srcpitch + rc->left) * 2];
		if (videomode == EDSP_TRUE12)
		{
			while(yctr < (unsigned long)rc->bottom)
			{
				for (xctr = 0; xctr < lw; xctr++)
				{
					tsc = ((psd16[xctr] & 0xF00) << 12) | ((psd16[xctr] & 0xF0) << 8) | ((psd16[xctr] & 0xF) << 4);
					tdc = ((pdd16[xctr] & 0xF00) << 12) | ((pdd16[xctr] & 0xF0) << 8) | ((pdd16[xctr] & 0xF) << 4);
					tdc = shader(tdc, tsc, xctr, (yctr - rc->top), property);
					pdd16[xctr] = (unsigned short)(((tdc >> 12) & 0xF00) | ((tdc >> 8) & 0xF0) | ((tdc >> 4) & 0xF));
				}
				pdd16 += dstpitch; psd16 += srcpitch; yctr++;
			}
		}
		else if (videomode == EDSP_TRUE15)
		{
			while(yctr < (unsigned long)rc->bottom)
			{
				for (xctr = 0; xctr < lw; xctr++)
				{
					tsc = ((psd16[xctr] & 31744) << 9) | ((psd16[xctr] & 992) << 6) | ((psd16[xctr] & 31) << 3);
					tdc = ((pdd16[xctr] & 31744) << 9) | ((pdd16[xctr] & 992) << 6) | ((pdd16[xctr] & 31) << 3);
					tdc = shader(tdc, tsc, xctr, (yctr - rc->top), property);
					pdd16[xctr] = (unsigned short)(((tdc >> 9) & 31744) | ((tdc >> 6) & 992) | ((tdc >> 3) & 31));
				}
				pdd16 += dstpitch; psd16 += srcpitch; yctr++;
			}
		}
		else
		{
			while(yctr < (unsigned long)rc->bottom)
			{
				for (xctr = 0; xctr < lw; xctr++)
				{
					tsc = ((psd16[xctr] & 63488) << 8) | ((psd16[xctr] & 2016) << 5) | ((psd16[xctr] & 31) << 3);
					tdc = ((pdd16[xctr] & 63488) << 8) | ((pdd16[xctr] & 2016) << 5) | ((pdd16[xctr] & 31) << 3);
					tdc = shader(tdc, tsc, xctr, (yctr - rc->top), property);
					pdd16[xctr] = (unsigned short)(((tdc >> 8) & 63488) | ((tdc >> 5) & 2016) | ((tdc >> 3) & 31));
				}
				pdd16 += dstpitch; psd16 += srcpitch; yctr++;
			}
		}
	}
	else if (videomode == EDSP_TRUE24)
	{
		unsigned long tsc, tdc;
		unsigned char *pdd8 = &dst[(y * dstpitch + x) * 3];
		unsigned char *psd8 = &src[(yctr * srcpitch + rc->left) * 3];
		while(yctr < (unsigned long)rc->bottom)
		{
			for (xctr = 0; xctr < lw; xctr++)
			{
				tsc = (psd8[xctr * 3] | (psd8[xctr * 3 + 1] << 8) | (psd8[xctr * 3 + 2] << 16));
				tdc = (pdd8[xctr * 3] | (pdd8[xctr * 3 + 1] << 8) | (pdd8[xctr * 3 + 2] << 16));
				tdc = shader(tdc, tsc, xctr, (yctr - rc->top), property);
				pdd8[xctr * 3] = (unsigned char)(tdc & 0xFF);
				pdd8[xctr * 3 + 1] = (unsigned char)((tdc >> 8) & 0xFF);
				pdd8[xctr * 3 + 2] = (unsigned char)(tdc >> 16);
			}
			pdd8 += dstpitch * 3; psd8 += srcpitch * 3; yctr++;
		}
	}
	else if (videomode == EDSP_TRUE32)
	{
		unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[(y * dstpitch + x) * 4];
		unsigned EINT32 *psd32 = (unsigned EINT32 *)&src[(yctr * srcpitch + rc->left) * 4];
		while(yctr < (unsigned long)rc->bottom)
		{
			for (xctr = 0; xctr < lw; xctr++)
				pdd32[xctr] = shader(pdd32[xctr], psd32[xctr], xctr, (yctr - rc->top), property);
			pdd32 += dstpitch; psd32 += srcpitch; yctr++;
		}
	}
}

//A generic colorkey blitter to use with a pixel shader
void E2DSurface::MemBltColorkey(EPIXELSHADER, long x, long y, unsigned char *dst, unsigned char *src, RECT *rc, unsigned long dstpitch, unsigned long srcpitch, unsigned char videomode, unsigned long property, COLORREF colorkey)
{
	unsigned long xctr, yctr = rc->top, lw = rc->right - rc->left;
	if (videomode == EDSP_INDEXED8)
	{
		unsigned char *pdd8 = &dst[y * dstpitch + x];
		unsigned char *psd8 = &src[yctr * srcpitch + rc->left];
		while(yctr < (unsigned long)rc->bottom)
		{
			for (xctr = 0; xctr < lw; xctr++)
				if (psd8[xctr] != colorkey)
					pdd8[xctr] = psd8[xctr];
			pdd8 += dstpitch; psd8 += srcpitch; yctr++;
		}
		//no pixel shader plugin support for 8-bit indexed mode
	}
	else if (videomode == EDSP_TRUE12 || videomode == EDSP_TRUE15 || videomode == EDSP_TRUE16)
	{
		unsigned long tsc, tdc;
		unsigned short *pdd16 = (unsigned short *)&dst[(y * dstpitch + x) * 2];
		unsigned short *psd16 = (unsigned short *)&src[(yctr * srcpitch + rc->left) * 2];
		if (videomode == EDSP_TRUE12)
		{
			while(yctr < (unsigned long)rc->bottom)
			{
				for (xctr = 0; xctr < lw; xctr++)
					if (psd16[xctr] != colorkey)
					{
						tsc = ((psd16[xctr] & 0xF00) << 12) | ((psd16[xctr] & 0xF0) << 8) | ((psd16[xctr] & 0xF) << 4);
						tdc = ((pdd16[xctr] & 0xF00) << 12) | ((pdd16[xctr] & 0xF0) << 8) | ((pdd16[xctr] & 0xF) << 4);
						tdc = shader(tdc, tsc, xctr, (yctr - rc->top), property);
						pdd16[xctr] = (unsigned short)(((tdc >> 12) & 0xF00) | ((tdc >> 8) & 0xF0) | ((tdc >> 4) & 0xF));
					}
				pdd16 += dstpitch; psd16 += srcpitch; yctr++;
			}
		}
		else if (videomode == EDSP_TRUE15)
		{
			while(yctr < (unsigned long)rc->bottom)
			{
				for (xctr = 0; xctr < lw; xctr++)
					if (psd16[xctr] != colorkey)
					{
						tsc = ((psd16[xctr] & 31744) << 9) | ((psd16[xctr] & 992) << 6) | ((psd16[xctr] & 31) << 3);
						tdc = ((pdd16[xctr] & 31744) << 9) | ((pdd16[xctr] & 992) << 6) | ((pdd16[xctr] & 31) << 3);
						tdc = shader(tdc, tsc, xctr, (yctr - rc->top), property);
						pdd16[xctr] = (unsigned short)(((tdc >> 9) & 31744) | ((tdc >> 6) & 992) | ((tdc >> 3) & 31));
					}
				pdd16 += dstpitch; psd16 += srcpitch; yctr++;
			}
		}
		else
		{
			while(yctr < (unsigned long)rc->bottom)
			{
				for (xctr = 0; xctr < lw; xctr++)
					if (psd16[xctr] != colorkey)
					{
						tsc = ((psd16[xctr] & 63488) << 8) | ((psd16[xctr] & 2016) << 5) | ((psd16[xctr] & 31) << 3);
						tdc = ((pdd16[xctr] & 63488) << 8) | ((pdd16[xctr] & 2016) << 5) | ((pdd16[xctr] & 31) << 3);
						tdc = shader(tdc, tsc, xctr, (yctr - rc->top), property);
						pdd16[xctr] = (unsigned short)(((tdc >> 8) & 63488) | ((tdc >> 5) & 2016) | ((tdc >> 3) & 31));
					}
				pdd16 += dstpitch; psd16 += srcpitch; yctr++;
			}
		}
	}
	else if (videomode == EDSP_TRUE24)
	{
		unsigned long tsc, tdc;
		unsigned char *pdd8 = &dst[(y * dstpitch + x) * 3];
		unsigned char *psd8 = &src[(yctr * srcpitch + rc->left) * 3];
		while(yctr < (unsigned long)rc->bottom)
		{
			for (xctr = 0; xctr < lw; xctr++)
			{
				tsc = (psd8[xctr * 3] | (psd8[xctr * 3 + 1] << 8) | (psd8[xctr * 3 + 2] << 16));
				if (tsc != colorkey)
				{
					tdc = (pdd8[xctr * 3] | (pdd8[xctr * 3 + 1] << 8) | (pdd8[xctr * 3 + 2] << 16));
					tdc = shader(tdc, tsc, xctr, (yctr - rc->top), property);
					pdd8[xctr * 3] = (unsigned char)(tdc & 0xFF);
					pdd8[xctr * 3 + 1] = (unsigned char)((tdc >> 8) & 0xFF);
					pdd8[xctr * 3 + 2] = (unsigned char)(tdc >> 16);
				}
			}
			pdd8 += dstpitch * 3; psd8 += srcpitch * 3; yctr++;
		}
	}
	else if (videomode == EDSP_TRUE32)
	{
		unsigned EINT32 *pdd32 = (unsigned EINT32 *)&dst[(y * dstpitch + x) * 4];
		unsigned EINT32 *psd32 = (unsigned EINT32 *)&src[(yctr * srcpitch + rc->left) * 4];
		while(yctr < (unsigned long)rc->bottom)
		{
			for (xctr = 0; xctr < lw; xctr++)
				if (psd32[xctr] != colorkey)
					pdd32[xctr] = shader(pdd32[xctr], psd32[xctr], xctr, (yctr - rc->top), property);
			pdd32 += dstpitch; psd32 += srcpitch; yctr++;
		}
	}
}
