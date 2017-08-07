// OpenGL ES interface
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Include edge
#include "edge.h"

//Include OpenGL ES API
#if defined(DEVICE_OPENKODE)
	#include <GLES/gl.h>
	#include <EGL/egl.h>
	#include <EGL/eglext.h>
#else
	#include "GLES\\egl.h"
	#include "GLES\\gl.h"
#endif

//Load from DLL dynamically
#if defined(DEVICE_WIN32)
	//#define EGL_FROMDLL
	//#define GLES_FROMDLL
#endif


/////////////////////////////////////////////////////////////////////
// DLL Imports                                                     //
/////////////////////////////////////////////////////////////////////

#if defined(EGL_FROMDLL)

//EGL function types
typedef EGLDisplay (*Func_eglGetDisplay)(NativeDisplayType display);
typedef EGLBoolean (*Func_eglInitialize)(EGLDisplay dpy, EGLint *major, EGLint *minor);
typedef EGLBoolean (*Func_eglTerminate)(EGLDisplay dpy);
typedef EGLBoolean (*Func_eglChooseConfig)(EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config);
typedef EGLSurface (*Func_eglCreateWindowSurface)(EGLDisplay dpy, EGLConfig config, NativeWindowType window, const EGLint *attrib_list);
typedef EGLBoolean (*Func_eglDestroySurface)(EGLDisplay dpy, EGLSurface surface);
typedef EGLBoolean (*Func_eglQuerySurface)(EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value);
typedef EGLContext (*Func_eglCreateContext)(EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list);
typedef EGLBoolean (*Func_eglDestroyContext)(EGLDisplay dpy, EGLContext ctx);
typedef EGLBoolean (*Func_eglMakeCurrent)(EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx);
typedef EGLBoolean (*Func_eglSwapBuffers)(EGLDisplay dpy, EGLSurface draw);

//EGL imports
Func_eglGetDisplay imp_eglGetDisplay;
Func_eglInitialize imp_eglInitialize;
Func_eglTerminate imp_eglTerminate;
Func_eglChooseConfig imp_eglChooseConfig;
Func_eglCreateWindowSurface imp_eglCreateWindowSurface;
Func_eglDestroySurface imp_eglDestroySurface;
Func_eglQuerySurface imp_eglQuerySurface;
Func_eglCreateContext imp_eglCreateContext;
Func_eglDestroyContext imp_eglDestroyContext;
Func_eglMakeCurrent imp_eglMakeCurrent;
Func_eglSwapBuffers imp_eglSwapBuffers;

//EGL function reroutes
#define eglGetDisplay imp_eglGetDisplay
#define eglInitialize imp_eglInitialize
#define eglTerminate imp_eglTerminate
#define eglChooseConfig imp_eglChooseConfig
#define eglCreateWindowSurface imp_eglCreateWindowSurface
#define eglDestroySurface imp_eglDestroySurface
#define eglQuerySurface imp_eglQuerySurface
#define eglCreateContext imp_eglCreateContext
#define eglDestroyContext imp_eglDestroyContext
#define eglMakeCurrent imp_eglMakeCurrent
#define eglSwapBuffers imp_eglSwapBuffers

#endif

#if defined(GLES_FROMDLL)

//OpenGLES function types
typedef void (*Func_glBindTexture)(GLenum target, GLuint texture);
typedef void (*Func_glBlendFunc)(GLenum sfactor, GLenum dfactor);
typedef void (*Func_glClear)(GLbitfield mask);
typedef void (*Func_glClearColorx)(GLclampx red, GLclampx green, GLclampx blue, GLclampx alpha);
typedef void (*Func_glColor4x)(GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha);
typedef void (*Func_glColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (*Func_glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void (*Func_glCompressedTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void (*Func_glDeleteTextures)(GLsizei n, const GLuint *textures);
typedef void (*Func_glDepthFunc)(GLenum func);
typedef void (*Func_glDisable)(GLenum cap);
typedef void (*Func_glDisableClientState)(GLenum array);
typedef void (*Func_glDrawArrays)(GLenum mode, GLint first, GLsizei count);
typedef void (*Func_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
typedef void (*Func_glEnable)(GLenum cap);
typedef void (*Func_glEnableClientState)(GLenum array);
typedef void (*Func_glFrustumx)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
typedef void (*Func_glGenTextures)(GLsizei n, GLuint *textures);
typedef void (*Func_glGetIntegerv)(GLenum pname, GLint *params);
typedef void (*Func_glHint)(GLenum target, GLenum mode);
typedef void (*Func_glLightxv)(GLenum light, GLenum pname, const GLfixed *params);
typedef void (*Func_glLoadIdentity)(void);
typedef void (*Func_glMatrixMode)(GLenum mode);
typedef void (*Func_glMultMatrixx)(const GLfixed *m);
typedef void (*Func_glNormalPointer)(GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (*Func_glOrthox)(GLfixed left, GLfixed right, GLfixed bottom, GLfixed top, GLfixed zNear, GLfixed zFar);
typedef void (*Func_glPopMatrix)(void);
typedef void (*Func_glPushMatrix)(void);
typedef void (*Func_glRotatex)(GLfixed angle, GLfixed x, GLfixed y, GLfixed z);
typedef void (*Func_glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (*Func_glShadeModel)(GLenum mode);
typedef void (*Func_glTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (*Func_glTexEnvx)(GLenum target, GLenum pname, GLfixed param);
typedef void (*Func_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (*Func_glTexParameterx)(GLenum target, GLenum pname, GLfixed param);
typedef void (*Func_glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (*Func_glVertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (*Func_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);

//OpenGLES imports
Func_glBindTexture imp_glBindTexture;
Func_glBlendFunc imp_glBlendFunc;
Func_glClear imp_glClear;
Func_glClearColorx imp_glClearColorx;
Func_glColor4x imp_glColor4x;
Func_glColorPointer imp_glColorPointer;
Func_glCompressedTexImage2D imp_glCompressedTexImage2D;
Func_glCompressedTexSubImage2D imp_glCompressedTexSubImage2D;
Func_glDeleteTextures imp_glDeleteTextures;
Func_glDepthFunc imp_glDepthFunc;
Func_glDisable imp_glDisable;
Func_glDisableClientState imp_glDisableClientState;
Func_glDrawArrays imp_glDrawArrays;
Func_glDrawElements imp_glDrawElements;
Func_glEnable imp_glEnable;
Func_glEnableClientState imp_glEnableClientState;
Func_glFrustumx imp_glFrustumx;
Func_glGenTextures imp_glGenTextures;
Func_glGetIntegerv imp_glGetIntegerv;
Func_glHint imp_glHint;
Func_glLightxv imp_glLightxv;
Func_glLoadIdentity imp_glLoadIdentity;
Func_glMatrixMode imp_glMatrixMode;
Func_glMultMatrixx imp_glMultMatrixx;
Func_glNormalPointer imp_glNormalPointer;
Func_glOrthox imp_glOrthox;
Func_glPopMatrix imp_glPopMatrix;
Func_glPushMatrix imp_glPushMatrix;
Func_glRotatex imp_glRotatex;
Func_glScissor imp_glScissor;
Func_glShadeModel imp_glShadeModel;
Func_glTexCoordPointer imp_glTexCoordPointer;
Func_glTexEnvx imp_glTexEnvx;
Func_glTexImage2D imp_glTexImage2D;
Func_glTexParameterx imp_glTexParameterx;
Func_glTexSubImage2D imp_glTexSubImage2D;
Func_glVertexPointer imp_glVertexPointer;
Func_glViewport imp_glViewport;

//OpenGLES function reroutes
#define glBindTexture imp_glBindTexture
#define glBlendFunc imp_glBlendFunc
#define glClear imp_glClear
#define glClearColorx imp_glClearColorx
#define glColor4x imp_glColor4x
#define glColorPointer imp_glColorPointer
#define glCompressedTexImage2D imp_glCompressedTexImage2D
#define glCompressedTexSubImage2D imp_glCompressedTexSubImage2D
#define glDeleteTextures imp_glDeleteTextures
#define glDepthFunc imp_glDepthFunc
#define glDisable imp_glDisable
#define glDisableClientState imp_glDisableClientState
#define glDrawArrays imp_glDrawArrays
#define glDrawElements imp_glDrawElements
#define glEnable imp_glEnable
#define glEnableClientState imp_glEnableClientState
#define glFrustumx imp_glFrustumx
#define glGenTextures imp_glGenTextures
#define glGetIntegerv imp_glGetIntegerv
#define glHint imp_glHint
#define glLightxv imp_glLightxv
#define glLoadIdentity imp_glLoadIdentity
#define glMatrixMode imp_glMatrixMode
#define glMultMatrixx imp_glMultMatrixx
#define glNormalPointer imp_glNormalPointer
#define glOrthox imp_glOrthox
#define glPopMatrix imp_glPopMatrix
#define glPushMatrix imp_glPushMatrix
#define glRotatex imp_glRotatex
#define glScissor imp_glScissor
#define glShadeModel imp_glShadeModel
#define glTexCoordPointer imp_glTexCoordPointer
#define glTexEnvx imp_glTexEnvx
#define glTexImage2D imp_glTexImage2D
#define glTexParameterx imp_glTexParameterx
#define glTexSubImage2D imp_glTexSubImage2D
#define glVertexPointer imp_glVertexPointer
#define glViewport imp_glViewport

#endif


/////////////////////////////////////////////////////////////////////
// Interface class                                                 //
/////////////////////////////////////////////////////////////////////

class EdgeGLInterface
{
	private:
		#if defined(DEVICE_WIN32)
			HDC egldc;
			HMODULE egldll;
			HMODULE glesdll;
		#endif
		ENATIVETYPE curwnd;
		EGLDisplay display;
		EGLSurface windowsurface;
		EGLContext context;
		EGLConfig config;
		bool suspended;
		bool ImportDLL(void);
		EGLConfig FindConfig();
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
		void Flip(void);
};

/////////////////////////////////////////////////////////////////////
// Interface class implementation                                  //
/////////////////////////////////////////////////////////////////////

//Import functions from DLL (no static linkage, Windows-only)
bool EdgeGLInterface::ImportDLL(void)
{
	#if defined(EGL_FROMDLL)
		if (egldll == 0)
		{
			egldll = LoadLibrary(TEXT("libEGL.dll"));
			if (egldll)
			{
				imp_eglGetDisplay = (Func_eglGetDisplay)GetProcAddress(egldll, TEXT("eglGetDisplay"));
				if (imp_eglGetDisplay == NULL)
					return(false);
				imp_eglInitialize = (Func_eglInitialize)GetProcAddress(egldll, TEXT("eglInitialize"));
				if (imp_eglInitialize == NULL)
					return(false);
				imp_eglTerminate = (Func_eglTerminate)GetProcAddress(egldll, TEXT("eglTerminate"));
				if (imp_eglTerminate == NULL)
					return(false);
				imp_eglChooseConfig = (Func_eglChooseConfig)GetProcAddress(egldll, TEXT("eglChooseConfig"));
				if (imp_eglChooseConfig == NULL)
					return(false);
				imp_eglCreateWindowSurface = (Func_eglCreateWindowSurface)GetProcAddress(egldll, TEXT("eglCreateWindowSurface"));
				if (imp_eglCreateWindowSurface == NULL)
					return(false);
				imp_eglDestroySurface = (Func_eglDestroySurface)GetProcAddress(egldll, TEXT("eglDestroySurface"));
				if (imp_eglDestroySurface == NULL)
					return(false);
				imp_eglQuerySurface = (Func_eglQuerySurface)GetProcAddress(egldll, TEXT("eglQuerySurface"));
				if (imp_eglQuerySurface == NULL)
					return(false);
				imp_eglCreateContext = (Func_eglCreateContext)GetProcAddress(egldll, TEXT("eglCreateContext"));
				if (imp_eglCreateContext == NULL)
					return(false);
				imp_eglDestroyContext = (Func_eglDestroyContext)GetProcAddress(egldll, TEXT("eglDestroyContext"));
				if (imp_eglDestroyContext == NULL)
					return(false);
				imp_eglMakeCurrent = (Func_eglMakeCurrent)GetProcAddress(egldll, TEXT("eglMakeCurrent"));
				if (imp_eglMakeCurrent == NULL)
					return(false);
				imp_eglSwapBuffers = (Func_eglSwapBuffers)GetProcAddress(egldll, TEXT("eglSwapBuffers"));
				if (imp_eglSwapBuffers == NULL)
					return(false);
			}
			else
				return(false);
		}
	#endif
	#if defined(GLES_FROMDLL)
		if (glesdll == 0)
		{
			glesdll = LoadLibrary(TEXT("libGLESv1_CM.dll"));
			if (glesdll == 0)
				glesdll = LoadLibrary(TEXT("libGLES_CM.dll"));
			if (glesdll == 0)
				glesdll = LoadLibrary(TEXT("libGLESv1_CL.dll"));
			if (glesdll == 0)
				glesdll = LoadLibrary(TEXT("libGLES_CL.dll"));
			if (glesdll == 0)
				glesdll = LoadLibrary(TEXT("libGLES_CM_NoE.dll"));
			if (glesdll)
			{
				imp_glBindTexture = (Func_glBindTexture)GetProcAddress(glesdll, TEXT("glBindTexture"));
				if (imp_glBindTexture == NULL)
					return(false);
				imp_glBlendFunc = (Func_glBlendFunc)GetProcAddress(glesdll, TEXT("glBlendFunc"));
				if (imp_glBlendFunc == NULL)
					return(false);
				imp_glClear = (Func_glClear)GetProcAddress(glesdll, TEXT("glClear"));
				if (imp_glClear == NULL)
					return(false);
				imp_glClearColorx = (Func_glClearColorx)GetProcAddress(glesdll, TEXT("glClearColorx"));
				if (imp_glClearColorx == NULL)
					return(false);
				imp_glColor4x = (Func_glColor4x)GetProcAddress(glesdll, TEXT("glColor4x"));
				if (imp_glColor4x == NULL)
					return(false);
				imp_glColorPointer = (Func_glColorPointer)GetProcAddress(glesdll, TEXT("glColorPointer"));
				if (imp_glColorPointer == NULL)
					return(false);
				imp_glDeleteTextures = (Func_glDeleteTextures)GetProcAddress(glesdll, TEXT("glDeleteTextures"));
				if (imp_glDeleteTextures == NULL)
					return(false);
				imp_glDepthFunc = (Func_glDepthFunc)GetProcAddress(glesdll, TEXT("glDepthFunc"));
				if (imp_glDepthFunc == NULL)
					return(false);
				imp_glDisable = (Func_glDisable)GetProcAddress(glesdll, TEXT("glDisable"));
				if (imp_glDisable == NULL)
					return(false);
				imp_glDisableClientState = (Func_glDisableClientState)GetProcAddress(glesdll, TEXT("glDisableClientState"));
				if (imp_glDisableClientState == NULL)
					return(false);
				imp_glDrawArrays = (Func_glDrawArrays)GetProcAddress(glesdll, TEXT("glDrawArrays"));
				if (imp_glDrawArrays == NULL)
					return(false);
				imp_glDrawElements = (Func_glDrawElements)GetProcAddress(glesdll, TEXT("glDrawElements"));
				if (imp_glDrawElements == NULL)
					return(false);
				imp_glEnable = (Func_glEnable)GetProcAddress(glesdll, TEXT("glEnable"));
				if (imp_glEnable == NULL)
					return(false);
				imp_glEnableClientState = (Func_glEnableClientState)GetProcAddress(glesdll, TEXT("glEnableClientState"));
				if (imp_glEnableClientState == NULL)
					return(false);
				imp_glFrustumx = (Func_glFrustumx)GetProcAddress(glesdll, TEXT("glFrustumx"));
				if (imp_glFrustumx == NULL)
					return(false);
				imp_glGenTextures = (Func_glGenTextures)GetProcAddress(glesdll, TEXT("glGenTextures"));
				if (imp_glGenTextures == NULL)
					return(false);
				imp_glGetIntegerv = (Func_glGetIntegerv)GetProcAddress(glesdll, TEXT("glGetIntegerv"));
				if (imp_glGetIntegerv == NULL)
					return(false);
				imp_glHint = (Func_glHint)GetProcAddress(glesdll, TEXT("glHint"));
				if (imp_glHint == NULL)
					return(false);
				imp_glLightxv = (Func_glLightxv)GetProcAddress(glesdll, TEXT("glLightxv"));
				if (imp_glLightxv == NULL)
					return(false);
				imp_glLoadIdentity = (Func_glLoadIdentity)GetProcAddress(glesdll, TEXT("glLoadIdentity"));
				if (imp_glLoadIdentity == NULL)
					return(false);
				imp_glMatrixMode = (Func_glMatrixMode)GetProcAddress(glesdll, TEXT("glMatrixMode"));
				if (imp_glMatrixMode == NULL)
					return(false);
				imp_glMultMatrixx = (Func_glMultMatrixx)GetProcAddress(glesdll, TEXT("glMultMatrixx"));
				if (imp_glMultMatrixx == NULL)
					return(false);
				imp_glNormalPointer = (Func_glNormalPointer)GetProcAddress(glesdll, TEXT("glNormalPointer"));
				if (imp_glNormalPointer == NULL)
					return(false);
				imp_glOrthox = (Func_glOrthox)GetProcAddress(glesdll, TEXT("glOrthox"));
				if (imp_glOrthox == NULL)
					return(false);
				imp_glPopMatrix = (Func_glPopMatrix)GetProcAddress(glesdll, TEXT("glPopMatrix"));
				if (imp_glPopMatrix == NULL)
					return(false);
				imp_glPushMatrix = (Func_glPushMatrix)GetProcAddress(glesdll, TEXT("glPushMatrix"));
				if (imp_glPushMatrix == NULL)
					return(false);
				imp_glRotatex = (Func_glRotatex)GetProcAddress(glesdll, TEXT("glRotatex"));
				if (imp_glRotatex == NULL)
					return(false);
				imp_glScissor = (Func_glScissor)GetProcAddress(glesdll, TEXT("glScissor"));
				if (imp_glScissor == NULL)
					return(false);
				imp_glShadeModel = (Func_glShadeModel)GetProcAddress(glesdll, TEXT("glShadeModel"));
				if (imp_glShadeModel == NULL)
					return(false);
				imp_glTexCoordPointer = (Func_glTexCoordPointer)GetProcAddress(glesdll, TEXT("glTexCoordPointer"));
				if (imp_glTexCoordPointer == NULL)
					return(false);
				imp_glTexEnvx = (Func_glTexEnvx)GetProcAddress(glesdll, TEXT("glTexEnvx"));
				if (imp_glTexEnvx == NULL)
					return(false);
				imp_glTexImage2D = (Func_glTexImage2D)GetProcAddress(glesdll, TEXT("glTexImage2D"));
				if (imp_glTexImage2D == NULL)
					return(false);
				imp_glTexParameterx = (Func_glTexParameterx)GetProcAddress(glesdll, TEXT("glTexParameterx"));
				if (imp_glTexParameterx == NULL)
					return(false);
				imp_glTexSubImage2D = (Func_glTexSubImage2D)GetProcAddress(glesdll, TEXT("glTexSubImage2D"));
				if (imp_glTexSubImage2D == NULL)
					return(false);
				imp_glVertexPointer = (Func_glVertexPointer)GetProcAddress(glesdll, TEXT("glVertexPointer"));
				if (imp_glVertexPointer == NULL)
					return(false);
				imp_glViewport = (Func_glViewport)GetProcAddress(glesdll, TEXT("glViewport"));
				if (imp_glViewport == NULL)
					return(false);
				imp_glCompressedTexImage2D = (Func_glCompressedTexImage2D)GetProcAddress(glesdll, TEXT("glCompressedTexImage2D"));
				imp_glCompressedTexSubImage2D = (Func_glCompressedTexSubImage2D)GetProcAddress(glesdll, TEXT("glCompressedTexSubImage2D"));
			}
			else
				return(false);
		}
	#endif
	return(true);
}

//Find an OpenGL ES configuration
EGLConfig EdgeGLInterface::FindConfig()
{
	EGLint num_config;
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

//EdgeGLInterface: constructor
EdgeGLInterface::EdgeGLInterface(void)
{
	#if defined(DEVICE_WIN32)
		egldc = 0;
		egldll = 0;
		glesdll = 0;
	#endif
	display = EGL_NO_DISPLAY;
	windowsurface = EGL_NO_SURFACE;
	context = EGL_NO_CONTEXT;
}

//EdgeGLInterface: destructor
EdgeGLInterface::~EdgeGLInterface(void)
{
	Close();
	#if defined(DEVICE_WIN32)
		if (egldll)
			FreeLibrary(egldll);
		if (glesdll)
			FreeLibrary(glesdll);
	#endif
}

//Open OpenGL ES
ERESULT EdgeGLInterface::Open(ENATIVETYPE wnd, unsigned long width, unsigned long height)
{
	int bits_r, bits_g, bits_b;
	Close();
	if (!ImportDLL())
		return(E_NOENTRY);
	curwnd = wnd;
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
		#else
			display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			if (display == EGL_NO_DISPLAY)
			{
				Close();
				return(E_ERROR);
			}
		#endif
		if (!eglInitialize(display, NULL, NULL))
		{
			Close();
			return(E_ERROR);
		}
		config = FindConfig();
	#else
		ENATIVETYPE *winhandlearr = (ENATIVETYPE *)wnd;
		config = (EGLConfig)winhandlearr[2];
		display = (EGLDisplay)winhandlearr[0];
	#endif
	context = eglCreateContext(display, config, NULL, NULL);
	if (context == EGL_NO_CONTEXT)
	{
		Close();
		return(E_ERROR);
	}
	#if defined(DEVICE_WIN32)
		windowsurface = eglCreateWindowSurface(display, config, (HWND)wnd, /*s_surfaceAttribs*/NULL);
	#elif defined(DEVICE_OPENKODE)
		windowsurface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)winhandlearr[1], /*s_surfaceAttribs*/NULL);
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
	suspended = false;
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
	
	return(E_OK);
}

//Close OpenGL ES
void EdgeGLInterface::Close(void)
{
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
}

//Update state
void EdgeGLInterface::UpdateState(void)
{
	int sizearray[2];
	eglQuerySurface(display, windowsurface, EGL_WIDTH, &sizearray[0]);
	eglQuerySurface(display, windowsurface, EGL_HEIGHT, &sizearray[1]);
	videowidth = sizearray[0];
	videoheight = sizearray[1];
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
	if (display != EGL_NO_DISPLAY && windowsurface != EGL_NO_SURFACE)
		eglSwapBuffers(display, windowsurface);
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
