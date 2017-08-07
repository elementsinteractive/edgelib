// esurf_2ddisplay.cpp
// 2D Display
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eraw.h"
#include "eloader.h"
#include "estdc.h"
#include "emath.h"
#include "ememory.h"
#include "efile.h"
#include "edisplay.h"
#include "econsole.h"

#include "core/edgecore.h"
#include "extra/verinfo.h"

// slow(1) or fast(0) way of compatibility
//#define EMUL_PIXMAP 1
#ifndef EMUL_PIXMAP
#define EMUL_PIXMAP 0
#endif

// dirty bit
#if EMUL_PIXMAP == 1
#define UPDATE_DIRTY() dirty = true
#define GLFORWARD 0
#else
#define UPDATE_DIRTY() {if (!dirty) display->Perspective2D(GetWidth(), GetHeight(), false); display->SetShading(E3D_TEXTURE); dirty = true;  }
#define GLFORWARD gldriver
#endif

void EdgeGL_SetOrientation(void * driver, long orientation);
void EdgeGL_Screenshot(void * driver, void * buffer, int x1, int y1, int x2, int y2);
void EdgeGL_GetVideoSize(void *driver, unsigned long &width, unsigned long &height);

E2DDisplaySurface::E2DDisplaySurface(ClassEDisplay * bundled)
{
	display = bundled;
	gldriver = NULL;
	dirty = false;
	lockcount = 0;
}

E2DDisplaySurface::~E2DDisplaySurface()
{

}

void E2DDisplaySurface::SetGLDriver(void * driver)
{
	EdgeGL_GetVideoSize(driver, syswidth, sysheight);

	gldriver = driver;
	if (driver) display->CreateSurface(this, syswidth, sysheight);

	RECT rc;
	SetRect(&rc, 0, 0, syswidth, sysheight);
	this->SetClipper(&rc);
}

unsigned char * E2DDisplaySurface::Lock(E2DSURFACEINFO *surfaceinfo)
{
	if (!GLFORWARD) return E2DSurface::Lock(surfaceinfo);

	if (lockcount > 0)
	{
		lockcount++;
		unsigned char * buffer = E2DSurface::Lock(surfaceinfo);
		//todo: deal with rotation if necessary
		Lockrotate(surfaceinfo);
		return buffer;
	}

	lockcount++;

	unsigned char * buffer = E2DSurface::Lock(surfaceinfo);
	Lockrotate(surfaceinfo);	
	EdgeGL_Screenshot(gldriver, buffer, 0, 0, syswidth, sysheight);
	return buffer;
}

void E2DDisplaySurface::Unlock(void)
{
	if (!GLFORWARD)
	{	
		E2DSurface::Unlock();
		return;
	}

	UPDATE_DIRTY();
	if (lockcount == 1)
	{
		// fix rotation, upload, blit
		int temporient = display->rotation;
		display->rotation = display->sysorientation;
		DoUpload(this);		
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = GetWidth();
		rc.bottom = GetHeight();
		EdgeGL_SetOrientation(gldriver, 0);
		display->rotation = temporient;
		display->Perspective2D(syswidth, sysheight, false);
		display->BltFast(0, 0, this, &rc);
		EdgeGL_SetOrientation(gldriver, (display->rotation + 4 - display->sysorientation) % 4);		
	}
	E2DSurface::Unlock();
	lockcount--;
}

void E2DDisplaySurface::Free(void)
{
	E2DSurface::Free();
}
void E2DDisplaySurface::Clear(COLORREF col)
{
	UPDATE_DIRTY();
	if (GLFORWARD)
	{
		RECT rc;
		rc.top = 0;
		rc.bottom = GetHeight();
		rc.left = 0;
		rc.right = GetWidth();
		display->FillRect(&rc, col);
	}
	else
		E2DSurface::Clear(col);
}

void E2DDisplaySurface::Reload(ClassEDisplay *dsp)
{
}

void E2DDisplaySurface::DrawFont(long x, long y, E2DSurface *source, const char *text, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)	
	{
		E2DSurface::DrawFont(x, y, source, text, flags, property);
		return;
	}

	if (source->DirtyFlush()) DoUpload(source);
	this->PrepareBlit(flags, property);	
	display->DrawFont(x, y, source, text, flags);
}

void E2DDisplaySurface::DrawFont(long x, long y, E2DSurface *source, const WCHAR *text, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
	{
		E2DSurface::DrawFont(x, y, source, text, flags, property);
		return;
	}

	if (source->DirtyFlush()) DoUpload(source);
	this->PrepareBlit(flags, property);	
	display->DrawFont(x, y, source, text, flags);
}

void E2DDisplaySurface::DrawFont(long x, long y, E2DSurface *source, long value, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
	{
		E2DSurface::DrawFont(x, y, source, value, flags, property);
		return;		
	}

	if (source->DirtyFlush()) DoUpload(source);
	this->PrepareBlit(flags, property);	
	display->DrawFont(x, y, source, value, flags);		
		
}

void E2DDisplaySurface::DrawFont(long x, long y, E2DSurfaceRGBA *source, const char *text, long flags, unsigned long property)
{	
	UPDATE_DIRTY();
	if (!GLFORWARD)		
	{
		E2DSurface::DrawFont(x, y, source, text, flags, property);
		return;
	}
	
	if (source->DirtyFlush()) DoUpload(source, true);
	this->PrepareBlit(flags, property);	
	display->DrawFont(x, y, source, text, flags);
}

void E2DDisplaySurface::DrawFont(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)		
	{
		E2DSurface::DrawFont(x, y, source, text, flags, property);
		return;
	}

	if (source->DirtyFlush()) DoUpload(source, true);
	this->PrepareBlit(flags, property);	
	display->DrawFont(x, y, source, text, flags);
}

void E2DDisplaySurface::DrawFont(long x, long y, E2DSurfaceRGBA *source, long value, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)		
	{
		E2DSurface::DrawFont(x, y, source, value, flags, property);
		return;
	}

	if (source->DirtyFlush()) DoUpload(source, true);
	this->PrepareBlit(flags, property);	
	display->DrawFont(x, y, source, value, flags);
}

void E2DDisplaySurface::DrawFontWrap(long x, long y, E2DSurface *source, const char *text, unsigned long width, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
	{
		E2DSurface::DrawFontWrap(x, y, source, text, width, flags, property);
		return;
	}

	if (source->DirtyFlush()) DoUpload(source);
	this->PrepareBlit(flags, property);	
	display->DrawFontWrap(x, y, source, text, width, flags);
}

void E2DDisplaySurface::DrawFontWrap(long x, long y, E2DSurface *source, const WCHAR *text, unsigned long width, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
	{
		E2DSurface::DrawFontWrap(x, y, source, text, width, flags, property);
		return;
	}
	if (source->DirtyFlush()) DoUpload(source);
	this->PrepareBlit(flags, property);	
	display->DrawFontWrap(x, y, source, text, width, flags);
}

void E2DDisplaySurface::DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const char *text, unsigned long width, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
	{
		E2DSurface::DrawFontWrap(x, y, source, text, width, flags, property);
		return;
	}
	if (source->DirtyFlush()) DoUpload(source, true);
	this->PrepareBlit(flags, property);	
	display->DrawFontWrap(x, y, source, text, width, flags);
}

void E2DDisplaySurface::DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, unsigned long width, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
	{
		E2DSurface::DrawFontWrap(x, y, source, text, width, flags, property);
		return;
	}
	if (source->DirtyFlush()) DoUpload(source, true);
	this->PrepareBlit(flags, property);	
	display->DrawFontWrap(x, y, source, text, width, flags);
}

void E2DDisplaySurface::PutPixel(long x, long y, COLORREF col, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	E2DSurface::PutPixel(x, y, col, flags, property);
}

void E2DDisplaySurface::DrawLine(long x1, long y1, long x2, long y2, COLORREF col, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
	{
		E2DSurface::DrawLine(x1, y1, x2, y2, col, flags, property);
		return;
	}
	else
	{
		if (flags == 0)
			display->DrawLine(x1, y1, x2, y2, col | 0xFF000000);
		else if (flags & EFX_OPACITY)
			display->DrawLine(x1, y1, x2, y2, col | (property << 24));
		else
			display->DrawLine(x1, y1, x2, y2, col);
	}
}

void E2DDisplaySurface::DrawOval(long x, long y, long xradius, long yradius, COLORREF col, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
		E2DSurface::DrawOval(x, y, xradius, yradius, col, flags, property);
	else
	{
		if (flags == 0)
			display->DrawOval(x, y, xradius, yradius, col | 0xFF000000);
		else if (flags & EFX_OPACITY)
			display->DrawOval(x, y, xradius, yradius, col | (property << 24));
		else
			display->DrawOval(x, y, xradius, yradius, col);
	}
}

void E2DDisplaySurface::DrawRect(RECT *rc, COLORREF col, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
	{	
		E2DSurface::DrawRect(rc, col, flags, property);
		return;
	}
	else
	{
		if (flags == 0)
			display->DrawRect(rc, col | 0xFF000000);
		else if (flags & EFX_OPACITY)
			display->DrawRect(rc, col | (property << 24));
		else
			display->DrawRect(rc, col);
	}
}

ERESULT E2DDisplaySurface::BltFast(long x, long y, E2DSurface *source, RECT *rc, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
		return E2DSurface::BltFast(x, y, source, rc, flags, property);

	if (source->DirtyFlush()) DoUpload(source);

	if (!ClipTextureRect(x, y, rc, source->virtualwidth, source->sheight)) return E_INVALIDPARAM;

	//display->Perspective2D(GetWidth(), GetHeight(), false);
	
	this->PrepareBlit(flags, property);
	display->BltFast(x, y, source, rc);	
	return E_OK;
}

ERESULT E2DDisplaySurface::BltFast(long x, long y, E2DSurfaceRGBA *source, RECT *rc, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
		return E2DSurface::BltFast(x, y, source, rc, flags, property);

	if (source->DirtyFlush()) 
	{
		DoUpload(source, false);		
	}

	if (!ClipTextureRect(x, y, rc, source->virtualwidth, source->sheight)) return E_INVALIDPARAM;

	this->PrepareBlit(flags, property);	
	display->BltFast(x, y, source, rc);

	return E_OK;
}

ERESULT E2DDisplaySurface::BltPixelShader(long x, long y, E2DSurface *source, RECT *rc, EPIXELSHADER, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	return E2DSurface::BltPixelShader(x, y, source, rc, shader, flags, property);
}

ERESULT E2DDisplaySurface::Blt(long center_x, long center_y, E2DSurface *source, RECT *rc, unsigned short rotation, unsigned long scale, unsigned char flipflags, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
		return E2DSurface::Blt(center_x, center_y, source, rc, rotation, scale, flipflags, flags, property);

	if (source->DirtyFlush()) 
	{
		DoUpload(source);		
	}

	this->PrepareBlit(flags, property);	
	display->Blt(center_x, center_y, source, rc, rotation, scale, flipflags);

	return E_OK;
}

ERESULT E2DDisplaySurface::BltFx(long center_x, long center_y, E2DSurface *source, RECT *rc, E2DBLTFX *bltfx)
{
	UPDATE_DIRTY();
	if (source->DirtyFlush()) DoUpload(source);
	return E2DSurface::BltFx(center_x, center_y, source, rc, bltfx);
}

ERESULT E2DDisplaySurface::BltStretch(RECT *dstrc, E2DSurface *source, RECT *srcrc, unsigned char flipflags, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (source->DirtyFlush()) DoUpload(source);
	return E2DSurface::BltStretch(dstrc, source, srcrc, flipflags, flags, property);
}

void E2DDisplaySurface::Render(long x, long y, E3DSurface *source, unsigned long flags, unsigned long property)
{	
	UPDATE_DIRTY();
	if (!GLFORWARD)
	{
		E2DSurface::Render(x, y, source, flags, property);
		return;
	}
	display->Render(source);
}

unsigned long E2DDisplaySurface::GetWidth(void)
{
	if (GLFORWARD)
		return display->GetWidth();
	else
		return E2DSurface::GetWidth();
}

unsigned long E2DDisplaySurface::GetHeight(void)
{
	if (GLFORWARD)
		return display->GetHeight(); 
	else
		return E2DSurface::GetHeight();
}

void E2DDisplaySurface::DoUpload(E2DSurface * surface)
{
	display->UploadTexture(surface, true, false, true);
	surface->DirtyFlush();
}

void E2DDisplaySurface::DoUpload(E2DSurfaceRGBA * surface, bool font)
{
	display->UploadTexture(surface, false);
	surface->DirtyFlush();
}
	
void E2DDisplaySurface::FillRect(RECT * rc, COLORREF col, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (GLFORWARD) 
	{
		if (flags == 0)
			display->FillRect(rc, col | 0xFF000000);
		else if (flags & EFX_OPACITY)
			display->FillRect(rc, col | (property << 24));
		else
			display->FillRect(rc, col);
	}
	else
		E2DSurface::FillRect(rc, col, flags, property);
	
}

void E2DDisplaySurface::FillOval(long x, long y, long xradius, long yradius, COLORREF col, long flags, unsigned long property)
{
	UPDATE_DIRTY();
	if (!GLFORWARD)
		E2DSurface::FillOval(x, y, xradius, yradius, col, flags, property);
	else
	{
		if (flags == 0)
			display->FillOval(x, y, xradius, yradius, col | 0xFF000000);
		else if (flags & EFX_OPACITY)
			display->FillOval(x, y, xradius, yradius, col | (property << 24));
		else
			display->FillOval(x, y, xradius, yradius, col);
	}
}

bool E2DDisplaySurface::ClipTextureRect(long &x, long &y, RECT * rc, long w, long h)
{
	if (rc == NULL)
		return(true);
	if (rc->left >= w)
		return(false);
	if (rc->top >= h)
		return(false);
	if (rc->bottom < 0)
		return(false);
	if (rc->right < 0)
		return(false);
	if (rc->left < 0)
	{
		x -= rc->left;
		rc->left = 0;
	}
	if (rc->top < 0)
	{
		y -= rc->top;
		rc->top = 0;
	}
	if (rc->right > w)
		rc->right = w;
	if (rc->bottom > h)
		rc->bottom = h;
	if (rc->left >= rc->right)
		return(false);
	if (rc->top >= rc->bottom)
		return(false);
	return(true);
}

void E2DDisplaySurface::PrepareBlit(unsigned long flags, unsigned long property)
{
	if (flags & EFX_OPACITY)
	{
		if (property < 0) property = 0;			// saturate to avoid artefacts
		if (property > 255) property = 255;		
	}
	else if (flags & EFX_COLORIZE)
	{
		property = property | 0xff000000;	    // don't do extra alphablending
	}
	flags |= E3D_TEXTURE;

	if (display->curshading == flags && display->curshadingprop == property) return;

	display->SetShading(flags, property);
}

void E2DDisplaySurface::Lockrotate(E2DSURFACEINFO *surfaceinfo)
{
	if (!surfaceinfo) return;
	int temp;

	switch( (display->rotation + 4 - display->sysorientation) % 4)
	{
		case 0:
			// up - no change
			break;
		case 1:
			temp = surfaceinfo->xpitch;
			surfaceinfo->xpitch = surfaceinfo->ypitch;
			surfaceinfo->ypitch = temp;
			// done via GetWidth/GetHeight
			//temp = surfaceinfo->width;
			//surfaceinfo->width = surfaceinfo->height;
			//surfaceinfo->height = temp;

			surfaceinfo->bufferoffset += (surfaceinfo->height - 1) * surfaceinfo->ypitch;			
			surfaceinfo->ypitch *= -1;			
			break;

		case 2:
			// rotate 180' (= flip x and y)
			surfaceinfo->bufferoffset += (surfaceinfo->width - 1) * surfaceinfo->xpitch;
			surfaceinfo->bufferoffset += (surfaceinfo->height - 1) * surfaceinfo->ypitch;
			surfaceinfo->xpitch *= -1;
			surfaceinfo->ypitch *= -1;
			break;

		case 3:
			temp = surfaceinfo->xpitch;
			surfaceinfo->xpitch = surfaceinfo->ypitch;
			surfaceinfo->ypitch = temp;
			//temp = surfaceinfo->width;
			//surfaceinfo->width = surfaceinfo->height;
			//surfaceinfo->height = temp;

			surfaceinfo->bufferoffset += (surfaceinfo->width - 1) * surfaceinfo->xpitch;
			surfaceinfo->xpitch *= -1;
			break;
	}
}

void E2DDisplaySurface::SetDirty()
{
	this->dirty = false;
}
