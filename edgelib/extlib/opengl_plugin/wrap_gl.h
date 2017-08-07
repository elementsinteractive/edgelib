// OpenGL interface
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Include edge
#include "edge.h"

//Include OpenGL API
#if defined(DEVICE_WINDOWS)
    #include "GL\\gl.h"
#elif defined(DEVICE_MAC)
    #include <OpenGL/gl.h>
	void MacCreateOpenGLContext(void *window, void *&context, GLuint &depthformat, GLuint &stencilformat, bool fullscreen, void *format, int api);
	bool MacCreateOpenGLSurface(void *window, void *&windowsurface, void *context, GLuint &renderbuffer, GLuint &framebuffer, GLuint &depthbuffer, GLuint &depthformat, GLuint &stencilbuffer, GLuint &stencilformat, unsigned long &width, unsigned long &height);
	void MacCloseOpenGL(void *context, GLuint &renderbuffer, GLuint &framebuffer, GLuint &depthbuffer, GLuint &depthformat, GLuint &stencilbuffer, GLuint &stencilformat);
	void MacFlipOpenGL(void *windowsurface, void *&contextOpenGL, GLuint &renderbuffer);
#else
    #include <GL/gl.h>
#endif

//Extensions
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_SHORT_5_6_5   0x8363

//Temporary texture list for conversion
#define MAX_TMPTEX   128

/////////////////////////////////////////////////////////////////////
// Interface class                                                 //
/////////////////////////////////////////////////////////////////////

class EdgeGLInterface
{
	private:
		#if defined(DEVICE_WINDOWS)
			DEVMODE screensettings;
			HDC gldc;
			HGLRC context;
			bool FindConfig(unsigned char preferredmode, unsigned long width, unsigned long height, bool fullscreen, unsigned char & stencil);
		#elif defined(DEVICE_MAC)
			//NSOpenGLContext * context;
			CGLPixelFormatObj FindConfig(unsigned char preferredmode, unsigned long width, unsigned long height, bool fullscreen, unsigned char & stencil);
			ECOREOBJ windowsurface;
			ECOREOBJ context;
			ECOREOBJ format;
			GLuint renderbuffer;
			GLuint depthbuffer;
			GLuint depthformat;
			GLuint stencilbuffer;
			GLuint stencilformat;
			GLuint framebuffer;
		#endif
		
		ENATIVETYPE curwnd;
		bool infullscreen;
		bool suspended;
	public:
		unsigned char stencilbits;
		GLfloat *tmp_tex;
		unsigned char videotype;
		unsigned long videowidth;
		unsigned long videoheight;
		unsigned long tmp_tex_size;
		EdgeGLInterface(void);
		~EdgeGLInterface(void);
		ERESULT Open(ENATIVETYPE wnd, unsigned char preferredmode, unsigned long width, unsigned long height, bool fullscreen, bool stencil);
		void Close(void);
		void Suspend(void);
		void Resume(void);
		void Flip(void);
};

/////////////////////////////////////////////////////////////////////
// Interface class implementation                                  //
/////////////////////////////////////////////////////////////////////

//Find an OpenGL configuration
#if defined(DEVICE_WINDOWS)
bool EdgeGLInterface::FindConfig(unsigned char preferredmode, unsigned long width, unsigned long height, bool fullscreen, unsigned char & stencil)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32 - stencil,
		stencil,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	if (fullscreen)
	{
		ClassEMemory::Set(&screensettings, 0, sizeof(screensettings));
		screensettings.dmSize = sizeof(screensettings);
		screensettings.dmPelsWidth  = width;
		screensettings.dmPelsHeight = height;
		if (preferredmode == EDSP_TRUE32)
			screensettings.dmBitsPerPel = 32;
		else if (preferredmode == EDSP_TRUE24)
			screensettings.dmBitsPerPel = 24;
		else if (preferredmode == EDSP_TRUE15 || preferredmode == EDSP_TRUE16)
			screensettings.dmBitsPerPel = 16;
		else if (preferredmode == EDSP_INDEXED8)
			screensettings.dmBitsPerPel = 8;
		screensettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		if (ChangeDisplaySettings(&screensettings, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL)
			infullscreen = true;
		else
			return(false);
	}
	int pixformat;
	if ((pixformat = ChoosePixelFormat(gldc, &pfd)) != 0)
		if (SetPixelFormat(gldc, pixformat, &pfd) != FALSE)
			return(true);

	pfd.cDepthBits = 32;
	pfd.cStencilBits = 0;
	stencil = 0;

	if ((pixformat = ChoosePixelFormat(gldc, &pfd)) == 0)
		return(false);
	if (SetPixelFormat(gldc, pixformat, &pfd) == FALSE)
		return(false);
	return(true);
}
#endif

//EdgeGLInterface: constructor
EdgeGLInterface::EdgeGLInterface(void)
{
	tmp_tex = NULL;
	tmp_tex_size = 0;
	#if defined(DEVICE_WINDOWS)
		gldc = 0;
		context = 0;
	#elif defined(DEVICE_MAC)
	context = nil;
	#endif
}

//EdgeGLInterface: destructor
EdgeGLInterface::~EdgeGLInterface(void)
{
	Close();
	ClassEMemory::DeAlloc(tmp_tex);
}

//Open OpenGL
ERESULT EdgeGLInterface::Open(ENATIVETYPE wnd, unsigned char preferredmode, unsigned long width, unsigned long height, bool fullscreen, bool stencil)
{
	int bits_r, bits_g, bits_b;
	Close();
	curwnd = wnd;
	#if defined(DEVICE_WINDOWS)
		gldc = GetDC((HWND)wnd);
		stencilbits = (stencil) ? 8 : 0;
		if (!gldc)
			return(E_ERROR);
		if (!FindConfig(preferredmode, width, height, fullscreen, stencilbits))
			return(E_ERROR);
		context = wglCreateContext(gldc);
		if (context == 0)
		{
			Close();
			return(E_ERROR);
		}
		wglMakeCurrent(gldc, context);
	#elif defined(DEVICE_MAC)
	/*
		stencilbits = (stencil) ? 8 : 0;
		CGLPixelFormatObj format = FindConfig(preferredmode, width, height, fullscreen, stencilbits);
		if (!format) return(E_ERROR);
		NSOpenGLPixelFormat * nsformat = [[[NSOpenGLPixelFormat alloc] initWithCGLPixelFormatObj:format] autorelease];
		if (nsformat == nil) return(E_ERROR);
		context = [[NSOpenGLContext alloc] initWithFormat:nsformat shareContext:nil];
		if (context == nil) return(E_ERROR);
		[context makeCurrentContext];
	 */
		GLuint stencilsize = (stencil) ? 8 : 0;
		MacCreateOpenGLContext((void *)wnd, context, depthformat, stencilsize, fullscreen, &format, 0x100);
		if (!context)
		{
			Close();
			return(E_ERROR);
		}
		stencilbits = stencilsize;
		if (!MacCreateOpenGLSurface((void *)wnd, windowsurface, context, renderbuffer, framebuffer, depthbuffer, depthformat, stencilbuffer, stencilsize, width, height))
		{
			Close();
			return(E_ERROR);
		}
		[(NSOpenGLContext *)context makeCurrentContext];

	#else
		return(E_ERROR);
	#endif
	glGetIntegerv(GL_RED_BITS, &bits_r);
	glGetIntegerv(GL_GREEN_BITS, &bits_g);
	glGetIntegerv(GL_BLUE_BITS, &bits_b);
	if (bits_r == 8 && bits_g == 8 && bits_b == 8)
		videotype = EDSP_TRUE32;
	else if (bits_r == 5 && bits_g == 6 && bits_b == 5)
		videotype = EDSP_TRUE16;
	else if (bits_r == 5 && bits_g == 5 && bits_b == 5)
		videotype = EDSP_TRUE15;
	else if (bits_r == 4 && bits_g == 4 && bits_b == 4)
		videotype = EDSP_TRUE12;
	else
		videotype = EDSP_UNKNOWN;
	videowidth = width;
	videoheight = height;
	suspended = false;
	glDisable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	#if defined(DEVICE_WINDOWS)
		if (infullscreen)
		{
			SwapBuffers(gldc);
			MoveWindow((HWND)wnd, 0, 0, width, height, TRUE);
			glViewport(0, 0, width, height);
		}
	#endif
	return(E_OK);
}

//Close OpenGL
void EdgeGLInterface::Close(void)
{
	#if defined(DEVICE_WINDOWS)
		if (context != 0)
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(context);
			context = 0;
		}
		if (infullscreen)
		{
			ChangeDisplaySettings(NULL, 0);
			infullscreen = false;
		}
		if (gldc)
		{
			ReleaseDC((HWND)curwnd, gldc);
			gldc = NULL;
		}
	#endif
}

//Suspend OpenGL
void EdgeGLInterface::Suspend(void)
{
	if (!suspended)
	{
		#if defined(DEVICE_WINDOWS)
			if (infullscreen)
				ChangeDisplaySettings(NULL, 0);
		#endif
		suspended = true;
	}
}

//Resume OpenGL
void EdgeGLInterface::Resume(void)
{
	
	if (suspended)
	{
		#if defined(DEVICE_WINDOWS)
			if (infullscreen)
				ChangeDisplaySettings(&screensettings, CDS_FULLSCREEN);
		#endif
		suspended = false;
	}
}

//Update the display
void EdgeGLInterface::Flip(void)
{
	#if defined(DEVICE_WINDOWS)
		SwapBuffers(gldc);
	#elif defined(DEVICE_MAC)
		MacFlipOpenGL(windowsurface, context, renderbuffer);
	#endif
}

/////////////////////////////////////////////////////////////////////
// Open and close functions                                        //
/////////////////////////////////////////////////////////////////////

//Return true if using an OpenGL API
bool EdgeGL_Use(void)
{
	return(true);
}

//Return true if using fixed point
bool EdgeGL_FixedPoint(void)
{
	return(true);
}

// Return stencil size (no stencil support implemented yet)
unsigned char EdgeGL_GetStencilBits(void *driver)
{ 
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	return eglinterface->stencilbits; 
}

//Create an interface
void *EdgeGL_NewDriver(void)
{
	EdgeGLInterface *eglinterface = new EdgeGLInterface;
	return(eglinterface);
}

//Free the interface
void EdgeGL_FreeDriver(void *driver)
{
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	delete eglinterface;
}

//OpenGL
ERESULT EdgeGL_Open(void *driver, void *dhandle, unsigned char preferredmode, unsigned long width, unsigned long height, bool fullscreen, bool stencil)
{
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	return(eglinterface->Open((ENATIVETYPE)dhandle, preferredmode, width, height, fullscreen, stencil));
}

//CloseGL
void EdgeGL_Close(void *driver)
{
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	eglinterface->Close();
}

//Update display state
void EdgeGL_UpdateDisplay(void *driver)
{
}

//Suspend OpenGL
void EdgeGL_Suspend(void *driver)
{
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	eglinterface->Suspend();
}

//Resume OpenGL
void EdgeGL_Resume(void *driver)
{
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	eglinterface->Resume();
}

//Restart lost contexts
void EdgeGL_Repair(void * driver) 
{ 
	// No volatile context
}

/////////////////////////////////////////////////////////////////////
// Update display function                                         //
/////////////////////////////////////////////////////////////////////

//Update screen
void EdgeGL_Flip(void *driver)
{
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	eglinterface->Flip();
}

/////////////////////////////////////////////////////////////////////
// Get status/information                                          //
/////////////////////////////////////////////////////////////////////

//Get the OpenGL video layout
unsigned char EdgeGL_GetVideoType(void *driver)
{
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	return(eglinterface->videotype);
}

//Get the width and height of the display
void EdgeGL_GetVideoSize(void *driver, unsigned long &width, unsigned long &height)
{
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	width = eglinterface->videowidth;
	height = eglinterface->videoheight;
}

/////////////////////////////////////////////////////////////////////
// Texture manipulation                                            //
/////////////////////////////////////////////////////////////////////

//Generate a texture, return the handle
unsigned long EdgeGL_GenTexture(void *driver)
{
	unsigned int texid;
	glGenTextures(1, &texid);
	if (texid > 0)
	{
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	return(texid);
}

//Delete a texture
void EdgeGL_DeleteTexture(void *driver, unsigned long texid)
{
	unsigned int delid = texid;
	glDeleteTextures(1, &delid);
}

//Select an active texture
void EdgeGL_SelectTexture(void *driver, unsigned long texid)
{
	glBindTexture(GL_TEXTURE_2D, texid);
}

//Copy pixel data to texture
ERESULT EdgeGL_UpdateTextureData(void *driver, unsigned long texid, unsigned long width, unsigned long height, RECT *rc, unsigned char colformat, unsigned char pixformat, void *ptr, bool updatepixels)
{
	GLenum glformat = GL_RGB, gltype = GL_UNSIGNED_BYTE;
	if (colformat == 1 || pixformat == 1 || pixformat == 2)
		glformat = GL_RGBA;
	if (pixformat == 1)
		gltype = GL_UNSIGNED_SHORT_4_4_4_4;
	else if (pixformat == 2)
		gltype = GL_UNSIGNED_SHORT_5_5_5_1;
	else if (pixformat == 3)
		gltype = GL_UNSIGNED_SHORT_5_6_5;
	if (pixformat > 3)
		return(E_INVALIDPARAM);
	glBindTexture(GL_TEXTURE_2D, texid);
	if (!updatepixels)
		glTexImage2D(GL_TEXTURE_2D, 0, glformat, width, height, 0, glformat, gltype, NULL);
	glTexSubImage2D(GL_TEXTURE_2D, 0, rc->left, height - (rc->bottom - rc->top), (rc->right - rc->left), (rc->bottom - rc->top), glformat, gltype, ptr);
	return(E_OK);
}

//Compressed textures for PC OpenGL currently not supported
ERESULT EdgeGL_UpdateCompressedTexture(void *driver, unsigned long texid, unsigned long width, unsigned long height, RECT *rc, char *formatstr, void *ptr, bool updatepixels)
{
	return(E_UNSUPPORTED);
}


/////////////////////////////////////////////////////////////////////
// Lighting                                                        //
/////////////////////////////////////////////////////////////////////

//Change light on/off state
void EdgeGL_EnableLight(void *driver, unsigned char nr, bool enable)
{
	if (enable)
		glEnable(GL_LIGHT0 + nr);
	else
		glDisable(GL_LIGHT0 + nr);
}

//Change light position
void EdgeGL_LightPosition(void *driver, unsigned char nr, long x, long y, long z)
{
	GLfloat lightpos[] = { (GLfloat)(x) * 16, (GLfloat)(y) * 16, (GLfloat)(z) * 16, 1.0f };
	glLightfv(GL_LIGHT0 + nr, GL_POSITION, lightpos);
}

//Change light intensity
void EdgeGL_LightIntensity(void *driver, unsigned char nr, COLORREF ambient, COLORREF diffuse, COLORREF specular)
{
	GLfloat lightamb[] = { (GLfloat)(E_GETR(ambient)) / 256, (GLfloat)(E_GETG(ambient)) / 256, (GLfloat)(E_GETB(ambient)) / 256, (GLfloat)(E_GETA(ambient)) / 256 };
	GLfloat lightdiff[] = { (GLfloat)(E_GETR(diffuse)) / 256, (GLfloat)(E_GETG(diffuse)) / 256, (GLfloat)(E_GETB(diffuse)) / 256, (GLfloat)(E_GETA(diffuse)) / 256 };
	GLfloat lightspec[] = { (GLfloat)(E_GETR(specular)) / 256, (GLfloat)(E_GETG(specular)) / 256, (GLfloat)(E_GETB(specular)) / 256, (GLfloat)(E_GETA(specular)) / 256 };
	glLightfv(GL_LIGHT0 + nr, GL_AMBIENT, lightamb);
	glLightfv(GL_LIGHT0 + nr, GL_DIFFUSE, lightdiff);
	glLightfv(GL_LIGHT0 + nr, GL_SPECULAR, lightspec);
}

/////////////////////////////////////////////////////////////////////
// Change state                                                    //
/////////////////////////////////////////////////////////////////////

//Change drawing color
void EdgeGL_SetColor(void *driver, COLORREF col, unsigned char type)
{
	if (type == 0)
		glClearColor((GLfloat)(E_GETR(col)) / 256, (GLfloat)(E_GETG(col)) / 256, (GLfloat)(E_GETB(col)) / 256, (GLfloat)(E_GETA(col)) / 256);
	else if (type == 1)
		glColor4f((GLfloat)(E_GETR(col)) / 256, (GLfloat)(E_GETG(col)) / 256, (GLfloat)(E_GETB(col)) / 256, (GLfloat)(E_GETA(col)) / 256);
}

//Change shading method (flat or smooth)
void EdgeGL_SetShading(void *driver, unsigned char type)
{
	if (type == 0)
		glShadeModel(GL_FLAT);
	else
		glShadeModel(GL_SMOOTH);
}

//Change texture blending mode (with background or color depending on type)
void EdgeGL_SetTextureBlending(void *driver, unsigned char blendtype, unsigned char value)
{
	if (blendtype == 0)
	{
		if (value == 0)
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		else if (value == 1)
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		else if (value == 2)
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		else if (value == 3)
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	}
	else if (blendtype == 1)
	{
		if (value == 0)
			glDisable(GL_BLEND);
		else
			glEnable(GL_BLEND);
		if (value == 1)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		else if (value == 2)
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
}

//Enable or disable texture mapping
void EdgeGL_UseTexturing(void *driver, bool enable)
{
	if (enable)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

//Change to orthographic (2D) view
void EdgeGL_Ortho(void *driver, long *coords)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPushMatrix();
	glOrtho(coords[0], coords[2], coords[1], coords[3], 0.0f, 0.1f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

//Change to perspective (3D) view
void EdgeGL_Perspective(void *driver, long *coords)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPushMatrix();
	glFrustum(coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//Change the perspective (3D) view with matrix
void EdgeGL_PerspectiveMatrix(void *driver, void *matrix)
{
	long *orgmtx = (long *)matrix;
	float tmtx[16];
	for (unsigned char mc = 0; mc < 16; mc++)
		tmtx[mc] = (GLfloat)(orgmtx[mc]) / 4096;
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPushMatrix();
	glMultMatrixf((const GLfloat *)tmtx);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//Enable/disable scissor testing and set scissor rectangle
void EdgeGL_Scissor(void *driver, long *rect)
{
	if (rect == NULL)
	{
		glDisable(GL_SCISSOR_TEST);
		return;
	}
	if (rect[0] >= 0)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(rect[0], rect[1], rect[2] - rect[0], rect[3] - rect[1]);
	}
	else
		glDisable(GL_SCISSOR_TEST);
}

// Change OpenGL orientation
void EdgeGL_SetOrientation(void * driver, long orientation)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glLoadIdentity();
	switch(orientation)
	{
		case DOR_NONE:
			break;
		case DOR_90DEG:
			glRotatef(-90, 0, 0, 1);
			break;
		case DOR_180DEG:
			glRotatef(180, 0, 0, 1);
			break;
		case DOR_270DEG:
			glRotatef(90, 0, 0, 1);
			break;

	}
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);    
}


/////////////////////////////////////////////////////////////////////
// Matrix manipulation                                             //
/////////////////////////////////////////////////////////////////////

//Load the identity matrix
void EdgeGL_IdentityMatrix(void *driver)
{
	glLoadIdentity();
}

//Push the current matrix on the stack
void EdgeGL_PushMatrix(void *driver)
{
	glPushMatrix();
}

//Pop the matrix from the stack
void EdgeGL_PopMatrix(void *driver)
{
	glPopMatrix();
}

//Multiply the OpenGL matrix with the new matrix
void EdgeGL_MultiplyMatrix(void *driver, void *matrix)
{
	long *orgmtx = (long *)matrix;
	float tmtx[16];
	for (unsigned char mc = 0; mc < 16; mc++)
	{
		if (mc < 12)
			tmtx[mc] = (GLfloat)(orgmtx[mc]) / 4096;
		else if (mc < 15)
			tmtx[mc] = (GLfloat)orgmtx[mc];
		else
			tmtx[15] = 1;
	}
	glMultMatrixf((const GLfloat *)tmtx);
}

/////////////////////////////////////////////////////////////////////
// Rendering functions                                             //
/////////////////////////////////////////////////////////////////////

//Clear screen
void EdgeGL_Clear(void *driver, bool colorbit, bool depthbit)
{
	if (colorbit && depthbit)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else if (colorbit)
		glClear(GL_COLOR_BUFFER_BIT);
	else if (depthbit)
		glClear(GL_DEPTH_BUFFER_BIT);
}

//Render a model
void EdgeGL_RenderModel(void *driver, void **arrays, long *strides, unsigned long count, bool wireframe, bool strip)
{
	if (count == 0)
		return;
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	if (arrays[3]) //emulate texcoordx
	{
		unsigned long ctr, vtxcount;
		if (arrays[4])
		{
			unsigned short *idxarr = (unsigned short *)arrays[4];
			vtxcount = 0;
			for (ctr = 0; ctr < count * 3; ctr++)
				if (idxarr[ctr] >= vtxcount)
					vtxcount = idxarr[ctr] + 1;
		}
		else
			vtxcount = count;
		if (vtxcount > eglinterface->tmp_tex_size)
		{
			eglinterface->tmp_tex_size = vtxcount;
			ClassEMemory::DeAlloc(eglinterface->tmp_tex);
			eglinterface->tmp_tex = (GLfloat *)ClassEMemory::Alloc(eglinterface->tmp_tex_size * 2 * sizeof(GLfloat));
			if (eglinterface->tmp_tex == NULL)
			{
				eglinterface->tmp_tex_size = 0;
				return;
			}
		}
		for (ctr = 0; ctr < vtxcount * 2; ctr++)
		{
			EINT32 *texarr = (EINT32 *)&((unsigned char *)(arrays[3]))[ctr / 2 * strides[3]];
			eglinterface->tmp_tex[ctr] = (GLfloat)(texarr[ctr % 2]) / 65536;
		}
	}
	if (arrays[0])
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_INT, strides[0], arrays[0]);
	}
	else
		glDisableClientState(GL_VERTEX_ARRAY);
	if (arrays[1])
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_BYTE, strides[1], arrays[1]);
	}
	else
		glDisableClientState(GL_NORMAL_ARRAY);
	if (arrays[2])
	{
		glEnableClientState(GL_COLOR_ARRAY);
		glColorPointer(4, GL_UNSIGNED_BYTE, strides[2], arrays[2]);
	}
	else
		glDisableClientState(GL_COLOR_ARRAY);
	if (arrays[3])
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, sizeof(GLfloat) * 2, eglinterface->tmp_tex);
	}
	else
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (arrays[4])
	{
		if (!strip)
		{
			if (wireframe)
				glDrawElements(GL_LINES, count * 3, GL_UNSIGNED_SHORT, arrays[4]);
			else
				glDrawElements(GL_TRIANGLES, count * 3, GL_UNSIGNED_SHORT, arrays[4]);
		}
	}
	else
	{
		if (strip)
		{
			if (wireframe)
				glDrawArrays(GL_LINE_STRIP, 0, count);
			else
				glDrawArrays(GL_TRIANGLE_STRIP, 0, count);
		}
	}

}

/////////////////////////////////////////////////////////////////////
// Utility functions                                               //
/////////////////////////////////////////////////////////////////////
void EdgeGL_Screenshot(void * driver, void * buffer, int x1, int y1, int x2, int y2)
{
	unsigned long width, height;
	EdgeGL_GetVideoSize(driver, width, height);
	glReadPixels(x1, height - y2, x2-x1, y2-y1, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
}
