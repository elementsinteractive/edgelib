// OpenGL interface
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Include edge
#include "edge.h"

//Include OpenGL API
#include "GL\\gl.h"
#include "..\\include\\fixmath.h"

/////////////////////////////////////////////////////////////////////
// Interface class                                                 //
/////////////////////////////////////////////////////////////////////

class EdgeGLInterface
{
	private:
		bool opened;
	public:
		unsigned char videotype;
		unsigned long videowidth;
		unsigned long videoheight;
		EdgeGLInterface(void);
		~EdgeGLInterface(void);
		ERESULT Open(void);
		void Close(void);
		void Flip(void);
};

/////////////////////////////////////////////////////////////////////
// Interface class implementation                                  //
/////////////////////////////////////////////////////////////////////

//EdgeGLInterface: constructor
EdgeGLInterface::EdgeGLInterface(void)
{
	opened = false;
}

//EdgeGLInterface: destructor
EdgeGLInterface::~EdgeGLInterface(void)
{
	Close();
}

//Open OpenGL
ERESULT EdgeGLInterface::Open(void)
{
	Close();
	videotype = EDSP_TRUE16;
	videowidth = 320;
	videoheight = 240;
	Fix_trig_init();
	if (glOpen(DEPTH_BUFFER) != GL_TRUE)
	{
		char vstr[32];
		int errcode = glGetError();
		ClassEStd::IntToStr(vstr, errcode);
		ClassEdge::Log(vstr);
		return(E_ERROR);
	}
	opened = true;

	/*
	glDisable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glClearColorx(0, 0, 0, 65536);
	glColor4x(65536, 65536, 65536, 65536);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);*/
	return(E_OK);
}

//Close OpenGL
void EdgeGLInterface::Close(void)
{
	if (opened)
	{
		glClose();
		opened = false;
	}
}

//Update the display
void EdgeGLInterface::Flip(void)
{
	//glSwapBuffers();
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
ERESULT EdgeGL_Open(void *driver, void *dhandle, unsigned char preferredmode, unsigned long width, unsigned long height, bool fullscreen)
{
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	return(eglinterface->Open());
}

//CloseGL
void EdgeGL_Close(void *driver)
{
	EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	eglinterface->Close();
}

//Suspend OpenGL
void EdgeGL_Suspend(void *driver)
{
}

//Resume OpenGL
void EdgeGL_Resume(void *driver)
{
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
	//EdgeGLInterface *eglinterface = (EdgeGLInterface *)driver;
	//eglinterface->Flip();
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
	unsigned int texid = 0;
	//glGenTextures(1, &texid);
	//if (texid > 0)
	//{
	//	glBindTexture(GL_TEXTURE_2D, texid);
	//	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//}
	return(texid);
}

//Delete a texture
void EdgeGL_DeleteTexture(void *driver, unsigned long texid)
{
	//unsigned int delid = texid;
	//glDeleteTextures(1, &delid);
}

//Select an active texture
void EdgeGL_SelectTexture(void *driver, unsigned long texid)
{
	//glBindTexture(GL_TEXTURE_2D, texid);
}

//Copy pixel data to texture
ERESULT EdgeGL_UpdateTextureData(void *driver, unsigned long texid, unsigned long width, unsigned long height, RECT *rc, unsigned char colformat, unsigned char pixformat, void *ptr, bool updatepixels)
{
	//GLenum glformat = GL_RGB, gltype = GL_UNSIGNED_BYTE;
	//if (colformat == 1 || pixformat == 1 || pixformat == 2)
	//	glformat = GL_RGBA;

	//if (pixformat == 1)
	//	gltype = GL_UNSIGNED_SHORT_4_4_4_4;
	//else if (pixformat == 2)
	//	gltype = GL_UNSIGNED_SHORT_5_5_5_1;
	//else if (pixformat == 3)
	//	gltype = GL_UNSIGNED_SHORT_5_6_5;
	
	//if (pixformat > 0)
	//	return(E_INVALIDPARAM);
	//glBindTexture(GL_TEXTURE_2D, texid);
	//if (!updatepixels)
	//	glTexImage2D(GL_TEXTURE_2D, 0, glformat, width, height, 0, glformat, gltype, NULL);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, rc->left, height - (rc->bottom - rc->top), (rc->right - rc->left), (rc->bottom - rc->top), glformat, gltype, ptr);
	return(E_OK);
}

/////////////////////////////////////////////////////////////////////
// Lighting                                                        //
/////////////////////////////////////////////////////////////////////

//Change light on/off state
void EdgeGL_EnableLight(void *driver, unsigned char nr, bool enable)
{
	//if (enable)
	//	glEnable(GL_LIGHT0 + nr);
	//else
	//	glDisable(GL_LIGHT0 + nr);
}

//Change light position
void EdgeGL_LightPosition(void *driver, unsigned char nr, long x, long y, long z)
{
	//GLfixed lightpos[] = { x * 16, y * 16, z * 16, 65536 };
	//glLightxv(GL_LIGHT0 + nr, GL_POSITION, lightpos);
}

//Change light intensity
void EdgeGL_LightIntensity(void *driver, unsigned char nr, COLORREF ambient, COLORREF diffuse, COLORREF specular)
{
	//GLfixed lightamb[] = { E_GETR(ambient) * 256, E_GETG(ambient) * 256, E_GETB(ambient) * 256, E_GETA(ambient) * 256 };
	//GLfixed lightdiff[] = { E_GETR(diffuse) * 256, E_GETG(diffuse) * 256, E_GETB(diffuse) * 256, E_GETA(diffuse) * 256 };
	//GLfixed lightspec[] = { E_GETR(specular) * 256, E_GETG(specular) * 256, E_GETB(specular) * 256, E_GETA(specular) * 256 };
	//glLightxv(GL_LIGHT0 + nr, GL_AMBIENT, lightamb);
	//glLightxv(GL_LIGHT0 + nr, GL_DIFFUSE, lightdiff);
	//glLightxv(GL_LIGHT0 + nr, GL_SPECULAR, lightspec);
}

/////////////////////////////////////////////////////////////////////
// Change state                                                    //
/////////////////////////////////////////////////////////////////////

//Change drawing color
void EdgeGL_SetColor(void *driver, COLORREF col, unsigned char type)
{
	//if (type == 0)
	//	glClearColorx(E_GETR(col) << 8, E_GETG(col) << 8, E_GETB(col) << 8, E_GETA(col) << 8);
	//else if (type == 1)
	//	glColor4x(E_GETR(col) << 8, E_GETG(col) << 8, E_GETB(col) << 8, E_GETA(col) << 8);
}

//Change shading method (flat or smooth)
void EdgeGL_SetShading(void *driver, unsigned char type)
{
	//if (type == 0)
	//	glShadeModel(GL_FLAT);
	//else
	//	glShadeModel(GL_SMOOTH);
}

//Enable or disable texture mapping
void EdgeGL_UseTexturing(void *driver, bool enable)
{
	//if (enable)
	//	glEnable(GL_TEXTURE_2D);
	//else
	//	glDisable(GL_TEXTURE_2D);
}

//Change to orthographic (2D) view
void EdgeGL_Ortho(void *driver, long *coords)
{
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrthox(coords[0], coords[2], coords[1], coords[3], -1, 1);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glDisable(GL_LIGHTING);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);
	//glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

//Change to perspective (3D) view
void EdgeGL_Perspective(void *driver, long *coords)
{
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glFrustumx(coords[0], coords[1], coords[2], coords[3], coords[4], coords[5]);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

/////////////////////////////////////////////////////////////////////
// Matrix manipulation                                             //
/////////////////////////////////////////////////////////////////////

//Push the current matrix on the stack
void EdgeGL_PushMatrix(void *driver)
{
	//glPushMatrix();
}

//Pop the matrix from the stack
void EdgeGL_PopMatrix(void *driver)
{
	//glPopMatrix();
}

//Multiply the OpenGL matrix with the new matrix
void EdgeGL_MultiplyMatrix(void *driver, void *matrix)
{
	//glMultMatrixx((const int *)matrix);
}

/////////////////////////////////////////////////////////////////////
// Rendering functions                                             //
/////////////////////////////////////////////////////////////////////

//Render a model
void EdgeGL_RenderModel(void *driver, void **arrays, long *strides, unsigned long count, bool wireframe, bool strip)
{
	/*
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
	*/
}
