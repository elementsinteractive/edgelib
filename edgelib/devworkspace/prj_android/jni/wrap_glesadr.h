// OpenGL ES interface
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Include edge
#include "edge.h"

//Include OpenGL ES API
#include "GLES/gl.h"
bool Call_Edgelib3DView_checkInstallation(void * wnd);
int Call_Edgelib3DView_getWidth();
int Call_Edgelib3DView_getHeight();

/////////////////////////////////////////////////////////////////////
// Interface class                                                 //
/////////////////////////////////////////////////////////////////////

class EdgeGLInterface
{
	private:
		ENATIVETYPE curwnd;
		bool suspended;
	public:
		unsigned char videotype;
		unsigned long videowidth;
		unsigned long videoheight;
		EdgeGLInterface(void);
		~EdgeGLInterface(void);
		ERESULT Open(ENATIVETYPE wnd, unsigned long width, unsigned long height);
		void Close(void);
		void UpdateState(void);
		void Suspend(void);
		void Resume(void);
        void Repair(void);
		void Flip(void);
};

/////////////////////////////////////////////////////////////////////
// Interface class implementation                                  //
/////////////////////////////////////////////////////////////////////


//EdgeGLInterface: constructor
EdgeGLInterface::EdgeGLInterface(void)
{
    
}

//EdgeGLInterface: destructor
EdgeGLInterface::~EdgeGLInterface(void)
{
	Close();
}

//Open OpenGL ES
ERESULT EdgeGLInterface::Open(ENATIVETYPE wnd, unsigned long width, unsigned long height)
{
	int bits_r, bits_g, bits_b;
	Close();
	curwnd = wnd;
	
	//if (!Call_Edglib3DView_checkInstallation(wnd)) return E_ERROR;
	
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
	
	videowidth = Call_Edgelib3DView_getWidth();
	videoheight = Call_Edgelib3DView_getHeight();
	
	suspended = false;
    Repair();
	
	return(E_OK);
}

void EdgeGLInterface::Repair(void)
{
    glDisable(GL_NORMALIZE);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glClearColorx(0, 0, 0, 65536);
    glColor4x(65536, 65536, 65536, 65536);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
}

//Close OpenGL ES
void EdgeGLInterface::Close(void)
{

}

//Update state
void EdgeGLInterface::UpdateState(void)
{
	videowidth = Call_Edgelib3DView_getWidth();
	videoheight = Call_Edgelib3DView_getHeight();
	glViewport(0, 0, videowidth, videoheight);
}

//Suspend OpenGL
void EdgeGLInterface::Suspend(void)
{
	if (!suspended)
	{
		//?
		suspended = true;
	}
}

//Resume OpenGL
void EdgeGLInterface::Resume(void)
{
	if (suspended)
	{
		suspended = false;
	}
}

//Update the display
void EdgeGLInterface::Flip(void)
{
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
	return 0; 
}

//Create a interface
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
	return(eglinterface->Open((ENATIVETYPE)dhandle, width, height));
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
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	eglinterface->UpdateState();
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

//Restore context
void EdgeGL_Repair(void *driver)
{
    EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
    eglinterface->Repair();
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

//Update screen
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
		glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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

//Compressed texture definitions
#if !defined(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG)
	#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                   0x8C00
	#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                   0x8C01
	#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                  0x8C02
	#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                  0x8C03
#endif

//Compressed textures for PC OpenGL currently not supported
ERESULT EdgeGL_UpdateCompressedTexture(void *driver, unsigned long texid, unsigned long width, unsigned long height, RECT *rc, char *formatstr, void *ptr, bool updatepixels)
{
	if (ClassEStd::StrFind(formatstr, "pvrtc_") == 0)
	{
		GLenum format;
		unsigned char pvrbpp;
		if (ClassEStd::StrEqual(formatstr, "pvrtc_2bpp"))
		{
			pvrbpp = 2;
			format = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
		}
		else if (ClassEStd::StrEqual(formatstr, "pvrtc_4bpp"))
		{
			pvrbpp = 4;
			format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
		}
		else if (ClassEStd::StrEqual(formatstr, "pvrtc_2bpp_alpha"))
		{
			pvrbpp = 2;
			format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
		}
		else if (ClassEStd::StrEqual(formatstr, "pvrtc_4bpp_alpha"))
		{
			pvrbpp = 4;
			format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
		}
		else
			return(E_UNSUPPORTED);
		GLsizei pvrsize = width * height * pvrbpp / 8;
		if (pvrsize < 32)
			pvrsize = 32;
		glCompressedTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, pvrsize, ptr);
	}
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
	GLfixed lightpos[] = { x * 16, y * 16, z * 16, 65536 };
	glLightxv(GL_LIGHT0 + nr, GL_POSITION, lightpos);
}

//Change light intensity
void EdgeGL_LightIntensity(void *driver, unsigned char nr, COLORREF ambient, COLORREF diffuse, COLORREF specular)
{
	GLfixed lightamb[] = { E_GETR(ambient) * 256, E_GETG(ambient) * 256, E_GETB(ambient) * 256, E_GETA(ambient) * 256 };
	GLfixed lightdiff[] = { E_GETR(diffuse) * 256, E_GETG(diffuse) * 256, E_GETB(diffuse) * 256, E_GETA(diffuse) * 256 };
	GLfixed lightspec[] = { E_GETR(specular) * 256, E_GETG(specular) * 256, E_GETB(specular) * 256, E_GETA(specular) * 256 };
	glLightxv(GL_LIGHT0 + nr, GL_AMBIENT, lightamb);
	glLightxv(GL_LIGHT0 + nr, GL_DIFFUSE, lightdiff);
	glLightxv(GL_LIGHT0 + nr, GL_SPECULAR, lightspec);
}

/////////////////////////////////////////////////////////////////////
// Change state                                                    //
/////////////////////////////////////////////////////////////////////

//Change drawing color
void EdgeGL_SetColor(void *driver, COLORREF col, unsigned char type)
{
	if (type == 0)
		glClearColorx(E_GETR(col) << 8, E_GETG(col) << 8, E_GETB(col) << 8, E_GETA(col) << 8);
	else if (type == 1)
		glColor4x(E_GETR(col) << 8, E_GETG(col) << 8, E_GETB(col) << 8, E_GETA(col) << 8);
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
			glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		else if (value == 1)
			glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		else if (value == 2)
			glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		else if (value == 3)
			glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
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
	glOrthox(coords[0], coords[2], coords[1], coords[3], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

//Change to perspective (3D) view
void EdgeGL_Perspective(void *driver, long *coords)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPushMatrix();
	glFrustumx(coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

//Change the perspective (3D) view with matrix
void EdgeGL_PerspectiveMatrix(void *driver, void *matrix)
{
	long *orgmtx = (long *)matrix;
	int tmtx[16];
	for (unsigned char mc = 0; mc < 16; mc++)
		tmtx[mc] = orgmtx[mc] << 4;
	glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	glPopMatrix();
	glPushMatrix();
	glMultMatrixx((const int *)tmtx);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
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
			glRotatex(5898240, 0, 0, 65536);
			break;
		case DOR_180DEG:
			glRotatex(180*65536, 0, 0, 65536);
			break;
		case DOR_270DEG:
			glRotatex(-5898240, 0, 0, 65536);
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
	int tmtx[16];
	for (unsigned char mc = 0; mc < 16; mc++)
	{
		if (mc < 12)
			tmtx[mc] = orgmtx[mc] << 4;
		else if (mc < 15)
			tmtx[mc] = orgmtx[mc];
		else
			tmtx[15] = 65536;
	}
	glMultMatrixx((const int *)tmtx);
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
	if (arrays[0])
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FIXED, strides[0], arrays[0]);
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
		glTexCoordPointer(2, GL_FIXED, strides[3], arrays[3]);
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
