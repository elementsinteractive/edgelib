// OpenGL ES 2.0 interface
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Include edge
#include "edge.h"

#include <stdio.h>

//Include OpenGL ES API
#if defined(DEVICE_MAEMO)
	#define SUPPORT_X11
#endif
#ifdef DEVICE_IPHONE
	#include <OpenGLES/ES2/gl.h>
	void MacCreateOpenGLContext(void *window, void *&context, GLuint &depthformat, GLuint &stencilformat, bool fullscreen, void *format, int api);
	bool MacCreateOpenGLSurface(void *window, void *&windowsurface, void *context, GLuint &renderbuffer, GLuint &framebuffer, GLuint &depthbuffer, GLuint &depthformat, GLuint &stencilbuffer, GLuint &stencilformat, unsigned long &width, unsigned long &height);
	void MacCloseOpenGL(void *context, GLuint &renderbuffer, GLuint &framebuffer, GLuint &depthbuffer, GLuint &depthformat, GLuint &stencilbuffer, GLuint &stencilformat);
	void MacFlipOpenGL(void *windowsurface, void *&contextOpenGL, GLuint &renderbuffer);
#else
	#include <GLES2/gl2.h>
	#include <EGL/egl.h>
#endif
//#include <stdio.h>

/////////////////////////////////////////////////////////////////////
// Interface class                                                 //
/////////////////////////////////////////////////////////////////////

class EdgeGLInterface
{
	private:
		#if defined(DEVICE_WIN32)
			HDC egldc;
		#endif
		ENATIVETYPE curwnd;
		bool suspended;
		#if defined(DEVICE_IPHONE)
			ECOREOBJ windowsurface;
			ECOREOBJ context;
			NSString format;
			GLuint renderbuffer;
			GLuint depthbuffer;
			GLuint depthformat;
			GLuint stencilbuffer;
			GLuint stencilformat;
			GLuint framebuffer;
		#else
			EGLDisplay display;
			EGLSurface windowsurface;
			EGLContext context;
			EGLConfig config;
			EGLConfig FindConfig(unsigned char & stencil);
		#endif
	public:
		unsigned char videotype;
		unsigned long videowidth;
		unsigned long videoheight;
	unsigned char stencilbits;
		EdgeGLInterface(void);
		~EdgeGLInterface(void);
		ERESULT Open(ENATIVETYPE wnd, unsigned long width, unsigned long height, bool stencil);
		void Close(void);
		void UpdateState(void);
		void Suspend(void);
		void Resume(void);
		void Flip(void);
};

/////////////////////////////////////////////////////////////////////
// Emulation state                                                 //
/////////////////////////////////////////////////////////////////////

static float edge_persp[16];
static float edge_rotate[16];
static float edge_mvmat[256];
static int edge_tos;
static bool edge_matvalid;
GLuint edge_program;
GLuint edge_light_program;
GLuint edge_current_program;

static float edge_basecolor[4];
static bool edge_texenable;

static const char * base_fragment_shader = 
	"varying mediump vec2 uv;\n"
	"uniform sampler2D tex0; \n"
    "uniform mediump vec4 filter; \n"
	"void main()\n" 
	"{\n"
       		"gl_FragColor = texture2D(tex0, uv) * filter;\n"
    	"}\n";
		
void EdgeGL_UpdateMatrix(void);
int EdgeGL_CompileShader(bool, const char *);
void EdgeGL_SetAttributes(void);
	
static const char * base_vertex_shader = 
	"attribute vec4 position; \n"
	"attribute vec2 texcoord; \n"
	"uniform mat4 mvmatrix; \n"
	"uniform mat4 pmatrix; \n"
	"varying mediump vec2 uv; \n"
	"void main()\n"
	"{\n"
		"gl_Position = pmatrix * (mvmatrix * position);\n" // do not (re)move braces - matrix * matrix is broken.
		"uv = texcoord;\n"
	"}\n"
	;
	
static const char * light_vertex_shader = 
	"attribute vec4 position; \n"
	"attribute vec2 texcoord; \n"
	"attribute vec4 normal; \n"
	"\n"
	"uniform mat4 mvmatrix; \n"
	"uniform mat4 pmatrix; \n"
	"uniform vec4 texlight; \n"
	"uniform vec4 goraudlight; \n"
    "uniform vec4 texambient; \n"
	"uniform vec4 goraudambient; \n"
	"\n"
	"uniform vec4 lightpos[8]; \n"
	"uniform vec4 lightamb[8]; \n"
	"uniform vec4 lightdif[8]; \n"
	"uniform vec4 lightspc[8]; \n"
	"uniform bool lighten[8]; \n"
	"\n"
	"varying mediump vec2 uv; \n"
	"varying mediump vec4 texc; \n"
	"varying mediump vec4 ambc; \n"
	"void main(void) \n"
	"{ \n"
		"vec4 wpos = mvmatrix * position; \n"
		"vec4 nnorm = normal; \n"
	       	"nnorm.w = 0.0; \n"
		"nnorm = normalize(mvmatrix * nnorm); \n"
		"gl_Position = pmatrix * wpos; \n"
		"uv = texcoord; \n"
		"texc = texambient; \n"
		"ambc = goraudambient; \n"
		"vec4 lightc = vec4(0.0, 0.0, 0.0, 0.0); \n"
		"for (int i = 0; i < 8; i++)\n"
		"{\n"
			"if (lighten[i])\n"
			"{"
				"vec4 vl = normalize(lightpos[i] - wpos); \n"
				"float diffuse = dot(vl, nnorm); \n"
				"if (diffuse <= 0.0) diffuse = 0.0; \n"
				"lightc += diffuse * lightdif[i]; \n"
				"lightc += lightamb[i]; \n"
				"\n" //"ambc += lightspc[i]; \n" // correct behaviour of specular lighting? (at least according to default gl 1.0 settings)
			"}"
		"}\n"
		"texc += lightc * texlight; \n"
		"ambc += lightc * goraudlight; \n"
	"} \n"
	;	
	
static const char * light_fragment_shader = 
	"uniform sampler2D tex0; \n"
	"varying mediump vec2 uv; \n"
	"varying mediump vec4 texc; \n"
	"varying mediump vec4 ambc; \n"
	"void main(void) \n"
	"{\n"
		"gl_FragColor = texture2D(tex0, uv) * texc + ambc; \n"
	"}\n"
	;
	
/////////////////////////////////////////////////////////////////////
// Interface class implementation                                  //
/////////////////////////////////////////////////////////////////////
	
//Find an OpenGL ES configuration
#if !defined(DEVICE_IPHONE)
EGLConfig EdgeGLInterface::FindConfig(unsigned char & stencil)
{
	EGLint num_config;
	if (stencil)
	{
	    EGLint stencillist[] = {
		    EGL_LEVEL,
		    0,
		    EGL_SURFACE_TYPE,
		    EGL_WINDOW_BIT,
		    EGL_DEPTH_SIZE,
		    16,
		    EGL_STENCIL_SIZE,
		    8,
		    EGL_NONE
	    };
	    for (stencil = 8; stencil > 0; stencil--)
	    {
		stencillist[7] = stencil;
	    	if (eglChooseConfig(display, stencillist, &config, 1, &num_config) && num_config == 1)
		    return(config);
	    }	    
	}
	
	stencil = 0;
	EGLint list[] = {
		EGL_LEVEL,
		0,
		EGL_SURFACE_TYPE,
		EGL_WINDOW_BIT,
		EGL_DEPTH_SIZE,
		16,
		EGL_NONE
	};
	if (!eglChooseConfig(display, list, &config, 1, &num_config) || num_config != 1)
		return(0);
	return(config);
}
#endif

//EdgeGLInterface: constructor
EdgeGLInterface::EdgeGLInterface(void)
{
	#if defined(DEVICE_WIN32)
		egldc = 0;
	#endif
    #if !defined(DEVICE_IPHONE)
		display = EGL_NO_DISPLAY;
		windowsurface = EGL_NO_SURFACE;
		context = EGL_NO_CONTEXT;
	#endif

	edge_tos = 0;
	edge_matvalid = false;
}

//EdgeGLInterface: destructor
EdgeGLInterface::~EdgeGLInterface(void)
{
	Close();
}

//Open OpenGL ES
ERESULT EdgeGLInterface::Open(ENATIVETYPE wnd, unsigned long width, unsigned long height, bool stencil)
{
	int bits_r, bits_g, bits_b;
	Close();
	curwnd = wnd;
	stencilbits = 0;
	#if defined(DEVICE_IPHONE)
		MacCreateOpenGLContext((void *)wnd, context, depthformat, stencilformat, true, &format, 0x200);
		if (!context)
		{
			Close();
			return(E_ERROR);
		}
		if (!stencil) stencilformat = 0;
		MacCreateOpenGLSurface((void *)wnd, windowsurface, context, renderbuffer, framebuffer, depthbuffer, depthformat, stencilbuffer, stencilformat, width, height);
		if (!windowsurface)
		{
			Close();
			return(E_ERROR);
		}
		if (stencilformat) stencilbits = 8;
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
	#else
		#if !defined(DEVICE_OPENKODE)
			#if defined(DEVICE_WIN32)
				egldc = GetDC((HWND)wnd);
				if (!egldc)
					return(E_ERROR);
				display = eglGetDisplay((NativeDisplayType)egldc);
				if (display == EGL_NO_DISPLAY)
				{
					ReleaseDC((HWND)curwnd, egldc);
					egldc = NULL;
					display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
				}
				if (display == EGL_NO_DISPLAY)
				{
					Close();
					return(E_ERROR);
				}
			#elif defined(DEVICE_MAEMO)
				ENATIVETYPE * winh = (ENATIVETYPE *)wnd;
				Display * xdsp = (Display *) winh[0];
				display = eglGetDisplay(xdsp);
			#else
				display = eglGetDisplay((EGLNativeDisplayType) EGL_DEFAULT_DISPLAY);
				if (display == EGL_NO_DISPLAY)
				{
					Close();
					return(E_ERROR);
				}
			#endif
			if (!eglInitialize(display, NULL, NULL))
			{
				//printf("bail: %i (%i)", eglGetError(), (int)display);			
				Close();
				return(E_ERROR);
			}

			EGLint eglattr[5] = { EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT, EGL_DEPTH_SIZE, 16, EGL_NONE };		
			
			stencilbits = 8;
			if (!stencil) stencilbits = 0;
			config = FindConfig(stencilbits);
			EGLint request_count;
			if (!eglChooseConfig(display, eglattr, &config, 1, &request_count))
			{
				//printf("bail: %i (%i)", eglGetError(), (int)display);
				Close();
				return(E_ERROR);
			}
				
		#else	
			ENATIVETYPE *winhandlearr = (ENATIVETYPE *)wnd;
			config = (EGLConfig)winhandlearr[2];
			display = (EGLDisplay)winhandlearr[0];
		#endif
		EGLint ctxattr[3] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
		context = eglCreateContext(display, config, EGL_NO_CONTEXT, ctxattr);
		if (context == EGL_NO_CONTEXT)
		{
			//printf("bail: %i (%i, %i)", eglGetError(), (int)display, (int)config);
			Close();
			return(E_ERROR);
		}
		#if defined(DEVICE_WIN32)
			windowsurface = eglCreateWindowSurface(display, config, (HWND)wnd, /*s_surfaceAttribs*/NULL);
		#elif defined(DEVICE_OPENKODE)
			windowsurface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)winhandlearr[1], /*s_surfaceAttribs*/NULL);
		#elif defined(DEVICE_MAEMO)
			windowsurface = eglCreateWindowSurface(display, config, *(Window *) winh[1], NULL);
		#else
			windowsurface = eglCreateWindowSurface(display, config, (void *)wnd, /*s_surfaceAttribs*/NULL);
		#endif
		if (windowsurface == EGL_NO_SURFACE)
		{
			Close();
			return(E_ERROR);
		}
		eglMakeCurrent(display, windowsurface, windowsurface, context);
		#if defined(DEVICE_WINMOBILE)
			SetForegroundWindow((HWND)wnd);
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
		#if defined(DEVICE_DESKTOP)
			videowidth = width;
			videoheight = height;
		#else
			int sizearray[2];
			eglQuerySurface(display, windowsurface, EGL_WIDTH, &sizearray[0]);
			eglQuerySurface(display, windowsurface, EGL_HEIGHT, &sizearray[1]);
			videowidth = sizearray[0];
			videoheight = sizearray[1];
		#endif
	#endif
	suspended = false;

	int vshader = EdgeGL_CompileShader(false, base_vertex_shader);
	int pshader = EdgeGL_CompileShader(true, base_fragment_shader);	
	edge_program = glCreateProgram();

	if (!vshader || !pshader || !edge_program) return(E_ERROR);

	glAttachShader(edge_program, vshader);
	glAttachShader(edge_program, pshader);
	glBindAttribLocation(edge_program, 0, "position");
	glBindAttribLocation(edge_program, 1, "texcoord");
	glLinkProgram(edge_program);
	glUseProgram(edge_program);
	edge_current_program = edge_program;

	vshader = EdgeGL_CompileShader(false, light_vertex_shader);
	pshader = EdgeGL_CompileShader(true, light_fragment_shader);
	edge_light_program = glCreateProgram();
	
	if (!vshader || !pshader || !edge_light_program) return(E_ERROR);
	glAttachShader(edge_light_program, vshader);
	glAttachShader(edge_light_program, pshader);
	glBindAttribLocation(edge_light_program, 0, "position");
	glBindAttribLocation(edge_light_program, 1, "texcoord");
	glBindAttribLocation(edge_light_program, 2, "normal");
	glLinkProgram(edge_light_program);	

	glViewport(0, 0, videowidth, videoheight);
	edge_tos = 0;
        for (int i = 0; i < 16; i++)
	{
	    float val = ((i >> 2) == (i & 3)) ? 1.0f : 0;
	    edge_persp[i] = val;
		edge_rotate[i] = val;
	    edge_mvmat[i] = val;
	}	
	edge_matvalid = false;
	EdgeGL_UpdateMatrix;

	for (int i = 0; i < 4; i++) edge_basecolor[i] = 1.0f;
	edge_texenable = true;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	
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
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	*/
	return(E_OK);
}

//Close OpenGL ES
void EdgeGLInterface::Close(void)
{
	#if defined(DEVICE_IPHONE)
		
	#else
		if (display != EGL_NO_DISPLAY)
		{
			#if defined(DEVICE_SYMBIAN)
				Flip();
				eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
			#endif
			if (context != EGL_NO_CONTEXT)
			{
				eglDestroyContext(display, context);
				context = EGL_NO_CONTEXT;
			}
			if (windowsurface != EGL_NO_SURFACE)
			{
				eglDestroySurface(display, windowsurface);
				windowsurface = EGL_NO_SURFACE;
			}
			#if !defined(DEVICE_OPENKODE)
				eglTerminate(display);
			#endif
			display = EGL_NO_DISPLAY;
		}
		#if defined(DEVICE_WIN32)
			if (egldc)
			{
				ReleaseDC((HWND)curwnd, egldc);
				egldc = NULL;
			}
		#endif
	#endif
}

//Update state
void EdgeGLInterface::UpdateState(void)
{
	#if !defined(DEVICE_IPHONE)
		int sizearray[2];
		eglQuerySurface(display, windowsurface, EGL_WIDTH, &sizearray[0]);
		eglQuerySurface(display, windowsurface, EGL_HEIGHT, &sizearray[1]);
		videowidth = sizearray[0];
		videoheight = sizearray[1];
	#endif
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
		//eglMakeCurrent(display, windowsurface, windowsurface, context);
		suspended = false;
	}
}

//Update the display
void EdgeGLInterface::Flip(void)
{
	#if defined(DEVICE_IPHONE)
		MacFlipOpenGL(windowsurface, context, renderbuffer);
	#else
		if (display != EGL_NO_DISPLAY && windowsurface != EGL_NO_SURFACE)
			eglSwapBuffers(display, windowsurface);
	#endif
}

//Restart lost contexts
void EdgeGL_Repair(void * driver) 
{ 
	// No volatile context
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
	return(false);
}

// Return stencil size
unsigned char EdgeGL_GetStencilBits(void *driver)
{ 
	EdgeGLInterface *eglinterface = (EdgeGLInterface *) driver;
	return eglinterface->stencilbits; 
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
	return(eglinterface->Open((ENATIVETYPE)dhandle, width, height, stencil));
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

//Compressed texture definitions
#if !defined(GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG)
	#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG                   0x8C00
	#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG                   0x8C01
	#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG                  0x8C02
	#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG                  0x8C03
#endif

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

	// other compressed formats for ES 2.0
	// format: OES_compressed_ETC1_RGB8_texture
	// format: OES_compressed_paletted_texture
	// format: EXT_texture_compression_dxt1
	
	return(E_UNSUPPORTED);
}


/////////////////////////////////////////////////////////////////////
// Lighting                                                        //
/////////////////////////////////////////////////////////////////////

//Change light on/off state
void EdgeGL_EnableLight(void *driver, unsigned char nr, bool enable)
{
	GLint loc = glGetUniformLocation(edge_light_program, "lighten");
	if (edge_current_program != edge_light_program)
		glUseProgram(edge_light_program);
	
	if (enable)
		glUniform1i(loc + nr, 1);
	else
		glUniform1i(loc + nr, 0);

	if (edge_current_program != edge_light_program)
		glUseProgram(edge_current_program);
}

//Change light position
void EdgeGL_LightPosition(void *driver, unsigned char nr, long x, long y, long z)
{
	GLfloat lightpos[4] = {(float)x / 4096.0f, (float)y / 4096.0f, (float)z / 4096.0f, 1.0f};
	GLint loc = glGetUniformLocation(edge_light_program, "lightpos");
	if (edge_current_program != edge_light_program)
		glUseProgram(edge_light_program);

	glUniform4fv(loc + nr, 4, lightpos);

	if (edge_current_program != edge_light_program)
		glUseProgram(edge_light_program);

	
/*	GLfixed lightpos[] = { x * 16, y * 16, z * 16, 65536 };
	glLightxv(GL_LIGHT0 + nr, GL_POSITION, lightpos);
*/}

//Change light intensity
void EdgeGL_LightIntensity(void *driver, unsigned char nr, COLORREF ambient, COLORREF diffuse, COLORREF specular)
{
	GLint loca = glGetUniformLocation(edge_light_program, "lightamb");
	GLint locd = glGetUniformLocation(edge_light_program, "lightdif");
	GLint locs = glGetUniformLocation(edge_light_program, "lightspc");

	if (edge_current_program != edge_light_program)
		glUseProgram(edge_light_program);

	glUniform4f(loca + nr, 1.0f * (float)E_GETR(ambient) / 255.0f, 1.0f * (float)E_GETG(ambient) / 255.0f, 1.0f * (float)E_GETB(ambient) / 255.0f, (float)E_GETA(ambient) / 255.0f);
	glUniform4f(locd + nr, 0.8f * (float)E_GETR(diffuse) / 255.0f, 0.8f * (float)E_GETG(diffuse) / 255.0f, 0.8f * (float)E_GETB(diffuse) / 255.0f, (float)E_GETA(diffuse) / 255.0f);
	glUniform4f(locs + nr, (float)E_GETR(specular)/ 255.0f, (float)E_GETG(specular)/ 255.0f, (float)E_GETB(specular)/ 255.0f, (float)E_GETA(specular)/ 255.0f);	

	if (edge_current_program != edge_light_program)
		glUseProgram(edge_current_program);
	
/*	GLfixed lightamb[] = { E_GETR(ambient) * 256, E_GETG(ambient) * 256, E_GETB(ambient) * 256, E_GETA(ambient) * 256 };
	GLfixed lightdiff[] = { E_GETR(diffuse) * 256, E_GETG(diffuse) * 256, E_GETB(diffuse) * 256, E_GETA(diffuse) * 256 };
	GLfixed lightspec[] = { E_GETR(specular) * 256, E_GETG(specular) * 256, E_GETB(specular) * 256, E_GETA(specular) * 256 };
	glLightxv(GL_LIGHT0 + nr, GL_AMBIENT, lightamb);
	glLightxv(GL_LIGHT0 + nr, GL_DIFFUSE, lightdiff);
	glLightxv(GL_LIGHT0 + nr, GL_SPECULAR, lightspec);
*/}

/////////////////////////////////////////////////////////////////////
// Change state                                                    //
/////////////////////////////////////////////////////////////////////

//Change drawing color
void EdgeGL_SetColor(void *driver, COLORREF col, unsigned char type)
{
	if (type == 0)
		glClearColor(E_GETR(col) / 255.0f, E_GETG(col) / 255.0f, E_GETB(col) / 255.0f, E_GETA(col) / 255.0f);
	else if (type == 1)
	{
		edge_basecolor[0] = (float)E_GETR(col) / 255.0f;
		edge_basecolor[1] = (float)E_GETG(col) / 255.0f;
		edge_basecolor[2] = (float)E_GETB(col) / 255.0f;
		edge_basecolor[3] = (float)E_GETA(col) / 255.0f;
		EdgeGL_SetAttributes();
	}
	//	glColor4x(E_GETR(col) << 8, E_GETG(col) << 8, E_GETB(col) << 8, E_GETA(col) << 8);
}

//Change shading method (flat or smooth)
void EdgeGL_SetShading(void *driver, unsigned char type)
{
	// needs separate program to function
	
/*	if (type == 0)
		glShadeModel(GL_FLAT);
	else
		glShadeModel(GL_SMOOTH);
*/}

//Change texture blending mode (with background or color depending on type)
void EdgeGL_SetTextureBlending(void *driver, unsigned char blendtype, unsigned char value)
{
	if (blendtype == 0)
	{
		blendtype++;
		return;
		/*
		if (value == 0)
			glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		else if (value == 1)
			glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		else if (value == 2)
			glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
		else if (value == 3)
			glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		*/
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
	edge_texenable = enable;
	EdgeGL_SetAttributes();
}

//Change to orthographic (2D) view
void EdgeGL_Ortho(void *driver, long *coords)
{
	glUseProgram(edge_program);
	edge_current_program = edge_program;
	
	//printf("[ortho: %x %x %x %x]", coords[0], coords[1], coords[2], coords[3]);
	float mat[16];
	for (int i = 0; i < 16; i++) mat[i] = 0;

	mat[0] = 2 / ((float)(coords[2] - coords[0])) * 4096;
	mat[5] = 2 / ((float)(coords[3] - coords[1])) * 4096;
	mat[10] = 1;
	mat[12] = -1 * (float)(coords[2] + coords[0]) / (float)(coords[2] - coords[0]);
	mat[13] = -1 * (float)(coords[3] + coords[1]) / (float)(coords[3] - coords[1]);
	mat[15] = 1;

	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			float sum = 0;
			for (int n = 0; n < 4; n++)
				sum += edge_rotate[4 * n + x] * mat[4 * y + n];
			
			edge_persp[x + 4 * y] = sum;
		}
	}
	
	for (int i = 0; i < 16; i++)
	{
		edge_mvmat[i + edge_tos] = ((i >> 2) == (i & 3)) ? 1 : 0;
	}

	edge_matvalid = false;

	GLint loc = glGetUniformLocation(edge_current_program, "pmatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, edge_persp);
	
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
}

//Change to perspective (3D) view
void EdgeGL_Perspective(void *driver, long *coords)
{
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glUseProgram(edge_light_program);
	edge_current_program = edge_light_program;
	EdgeGL_SetAttributes();

	float persp[16];
	for (int i = 0; i < 16; i++)
		persp[i] = 0;

	float fl = (float)(coords[0]) / 4096;
	float fr = (float)(coords[1]) / 4096;
	float fb = (float)(coords[2]) / 4096;
	float ft = (float)(coords[3]) / 4096;
	float zn = (float)(coords[4]) / 4096;
	float zf = (float)(coords[5]) / 4096;
	

	persp[0] = 2 * zn / (fr - fl);
	persp[5] = 2 * zn / (ft - fb);
	persp[10] = -(zf+zn)/(zf-zn);
	persp[11] = -1;
	persp[14] = -2 * zf * zn / (zf-zn);	
	persp[8] = (fr + fl) / (fr - fl);
	persp[9] = (ft + fb) / (ft - fb);
	
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			float sum = 0;
			for (int n = 0; n < 4; n++)
				sum += edge_rotate[4 * n + x] * persp[4 * y + n];
			
			edge_persp[x + 4 * y] = sum;
		}
	}

	GLint loc = glGetUniformLocation(edge_current_program, "pmatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, edge_persp);
	
	edge_matvalid = false;

	//for (int i = 0; i < 16; i++)
	//	printf("[%i: %f]", i, edge_persp[i]);
	
}

//Change the perspective (3D) view with matrix
void EdgeGL_PerspectiveMatrix(void *driver, void *matrix)
{
	glUseProgram(edge_light_program);
	edge_current_program = edge_light_program;
	EdgeGL_SetAttributes();
	//glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	
	float mat[16];
	
	for (int i = 0; i < 16; i++)
	{
		mat[i] = (float)(((long*)matrix)[i]) / 4096.0f;
		//printf("[p%i: %f]", i, edge_persp[i]);
	}
	
	for (int y = 0; y < 4; y++)
	{
		for (int x = 0; x < 4; x++)
		{
			float sum = 0;
			for (int n = 0; n < 4; n++)
				sum += edge_rotate[4 * n + x] * mat[4 * y + n];
			
			edge_persp[x + 4 * y] = sum;
		}
	}

	edge_matvalid = false;
	
	GLint loc = glGetUniformLocation(edge_current_program, "pmatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, edge_persp);
	
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
		//glEnable(GL_SCISSOR_TEST);
		glScissor(rect[0], rect[1], rect[2] - rect[0], rect[3] - rect[1]);
	}
	else
		glDisable(GL_SCISSOR_TEST);
}

void EdgeGL_SetOrientation(void * driver, long orientation)
{
	for (int i = 0; i < 8; i++)
		edge_rotate[i] = 0.0f;
	switch (orientation % 4) {
		case DOR_NONE:
			edge_rotate[0] = 1.0f;
			edge_rotate[5] = 1.0f;
			break;
		case DOR_90DEG:
			edge_rotate[1] = -1.0f;
			edge_rotate[4] = 1.0f;
			break;			
		case DOR_180DEG:
			edge_rotate[0] = -1.0f;
			edge_rotate[5] = -1.0f;
			break;			
		case DOR_270DEG:
			edge_rotate[1] = 1.0f;
			edge_rotate[4] = -1.0f;
			break;
	}	
}

/////////////////////////////////////////////////////////////////////
// Matrix manipulation                                             //
/////////////////////////////////////////////////////////////////////

//Load the identity matrix
void EdgeGL_IdentityMatrix(void *driver)
{
	for (int i = 0; i < 16; i++)
		edge_mvmat[i + edge_tos] = ((i & 3) == (i >> 2)) ? 1 : 0;

	edge_matvalid = false;
}

//Push the current matrix on the stack
void EdgeGL_PushMatrix(void *driver)
{
	int old_tos = edge_tos;
	edge_tos += 16;
	if (edge_tos == 256) edge_tos = 0;

	for (int i = 0; i < 16; i++) // copy matrix
		edge_mvmat[i + edge_tos] = edge_mvmat[i + old_tos];

	//printf("[^ tos: %i]", edge_tos);	
}

//Pop the matrix from the stack
void EdgeGL_PopMatrix(void *driver)
{
	edge_tos -= 16;
	if (edge_tos < 0) edge_tos += 256;
	
	edge_matvalid = false;
	//printf("[v tos: %i]", edge_tos);
}

//Multiply the OpenGL matrix with the new matrix
void EdgeGL_MultiplyMatrix(void *driver, void *matrix)
{
	float mat_1[16];
	float mat_2[16];

	for (int i = 0; i < 16; i++)
	{
		mat_1[i] = edge_mvmat[i+edge_tos];
		mat_2[i] = ((float)((long *)matrix)[i]) / 4096;
		//printf("[in %i: %f]", i, mat_2[i]);
	}
	mat_2[15] = 1.0f; // is passed 1/4096th in fixed point ?!

	for (int x = 0; x < 4; x++)
	for (int y = 0; y < 4; y++)
	{
	    float sum = 0;
	    for (int u = 0; u < 4; u++)
		    sum += mat_1[x + 4 * u] * mat_2[u + 4 * y];

	    edge_mvmat[edge_tos+x+4*y] = sum;
	    //printf("[m %i: %f]", x + 4*y, edge_mvmat[edge_tos+x+y*4]);
	}

	edge_matvalid = false;
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

	EdgeGL_PushMatrix(driver);
	for (int i = 0; i < 12; i++)
	{
		edge_mvmat[i + edge_tos] *= 16;
		//printf("[m %i: %f]", i, edge_mvmat[i+edge_tos]);
	}
	for (int i = 12; i < 16; i++)
	{
		//printf("[m %i: %f]", i, edge_mvmat[i+edge_tos]);
	}
	edge_matvalid = false;
	EdgeGL_UpdateMatrix();
	
	if (arrays[0])
	{
		//glEnableClientState(GL_VERTEX_ARRAY);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FIXED, false, strides[0], arrays[0]);
	}
	else
		glDisableVertexAttribArray(0);
		//glDisableClientState(GL_VERTEX_ARRAY);
	if (arrays[1])
	{
		//glEnableClientState(GL_NORMAL_ARRAY);
		//glNormalPointer(GL_BYTE, strides[1], arrays[1]);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_BYTE, false, strides[1], arrays[1]);
	}
	else
		glDisableVertexAttribArray(2);
		//glDisableClientState(GL_NORMAL_ARRAY);
	if (arrays[2])
	{
		//glEnableClientState(GL_COLOR_ARRAY);
		//glColorPointer(4, GL_UNSIGNED_BYTE, strides[2], arrays[2]);
	}
	else
		;//glDisableClientState(GL_COLOR_ARRAY);
	if (arrays[3])
	{
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FIXED, false, strides[3], arrays[3]);
	}
	else
		glDisableVertexAttribArray(1);
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

	EdgeGL_PopMatrix(driver);
}

int EdgeGL_CompileShader(bool pixelshader, const char * shadertext)
{
	GLuint shader = glCreateShader(pixelshader ? GL_FRAGMENT_SHADER : GL_VERTEX_SHADER);
	if (!shader) return 0;

	glShaderSource(shader, 1, &shadertext, NULL);
	glCompileShader(shader);
	GLint rv;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &rv);
      	if (rv == GL_FALSE)
	{
		char log[8193];
		int reallength;
		glGetShaderInfoLog(shader, 8192, &reallength, log);
		log[reallength] = 0;
		puts(log);
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

void EdgeGL_UpdateMatrix(void)
{
	if (edge_matvalid) return;
	GLint loc = glGetUniformLocation(edge_current_program, "mvmatrix");

	
	float outmat[16];
	for (int y = 0; y < 4; y++)
	for (int x = 0; x < 4; x++)
	{
		float sum = 0;
		for (int n = 0; n < 4; n++)
			sum += edge_persp[4 * n + x] * edge_mvmat[4 * y + n + edge_tos];
		
		outmat[x + 4 * y] = sum;
		//printf("[u %i: %f]", (x+4*y), outmat[x+4*y]);
	}
	
	//glUniformMatrix4fv(loc, 1, GL_FALSE, outmat); 
	glUniformMatrix4fv(loc, 1, GL_FALSE, &(edge_mvmat[edge_tos]));

	edge_matvalid = true;
}

void EdgeGL_SetAttributes(void)
{
	GLint c_ta = glGetUniformLocation(edge_current_program, "texambient");
	GLint c_ga = glGetUniformLocation(edge_current_program, "goraudambient");
	GLint c_tl = glGetUniformLocation(edge_current_program, "texlight");
	GLint c_gl = glGetUniformLocation(edge_current_program, "goraudlight");
	GLint c_xx = glGetUniformLocation(edge_current_program, "filter");
	
	if (edge_texenable)
	{
		glUniform4f(c_ta, 0, 0, 0, 1);
		glUniform4f(c_ga, 0, 0, 0, 0);
		glUniform4f(c_tl, edge_basecolor[0], edge_basecolor[1], edge_basecolor[2], 0);
		glUniform4f(c_gl, 0, 0, 0, 0);
	}
	else
	{
		glUniform4f(c_ta, 0, 0, 0, 0);
		glUniform4f(c_ga, 0, 0, 0, 1);
		glUniform4f(c_tl, 0, 0, 0, 0);
		glUniform4f(c_gl, edge_basecolor[0], edge_basecolor[1], edge_basecolor[2], 0);
	}

	glUniform4f(c_xx, edge_basecolor[0], edge_basecolor[1], edge_basecolor[2], 1.0f);
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
