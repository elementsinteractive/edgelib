/////////////////////////////////////////////////////////////////////
// code/animate3d.cpp
// One of the Edge samples
// Shows how to animate a 3D model and do basic transformations
//
// Displays an animated model on a box
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Use Windows Desktop OpenGL by default
#if !defined(__EDGEIDE__)
	#if !defined(DEVICE_LINUX)
		#define EGL_PC
	#endif
#endif

/////////////////////////////////////////////////////////////////////
// Include and link the library                                    //
/////////////////////////////////////////////////////////////////////

//Use a GL library if an implementation has been defined in the workspace
#if defined(EGL_PC) || defined(EGL_SYMBIAN) || defined(EGL_RASTEROID) || defined(EGL_POWERVR) || defined(EGL_NVIDIA) || defined(EGL_MAEMO)
	#define EGL_USEGL
        #if defined(EGL_MAEMO)
		#define EGL_USEGLES2
	#elif !defined(EGL_PC)
		#define EGL_USEGLES
	#endif
#endif

//Include Edge
#include "edgemain.h"

//Link the Edge static library
#pragma comment(lib, "edge.lib")

//Include headers and link OpenGL libraries
#if defined(EGL_USEGL)
	#if defined(EGL_USEGLES)
		#include "GLES\\gl.h"
		#include "GLES\\egl.h"
	#elif defined(DEVICE_IPHONE)
		#include <OpenGLES/ES1/gl.h>
	#elif defined(EGL_USEGLES2)
		#include "GLES2/gl2.h"
		#include "EGL/egl.h"
	#else
		#include "GL\\gl.h"
	#endif
	#if defined(DEVICE_WIN32)
		#if defined(EGL_PC)
			#pragma comment(lib, "plugingl.lib")
		#else
			#if defined(DEVICE_DESKTOP)
				#if defined(EGL_POWERVR)
					#pragma comment(lib, "pluginpowervr.lib")
				#elif defined(EGL_RASTEROID)
					#pragma comment(lib, "pluginrasteroid.lib")
				#endif
			#else
				#pragma comment(lib, "plugin1-0.lib")
			#endif
		#endif
		#if defined(EGL_PC)
			#pragma comment(lib, "opengl32.lib")
		#elif defined(EGL_POWERVR)
			#pragma comment(lib, "libgles_cl.lib")
		#elif defined(EGL_NVIDIA)
			#pragma comment(lib, "libgles_cm.lib")
		#elif defined(EGL_RASTEROID)
			#pragma comment(lib, "libEGL.lib")
			#pragma comment(lib, "libGLES_CM_NoE.lib")
		#endif
	#endif
#endif

//Contains ClassMain, the application framework
#include "main.h"

/////////////////////////////////////////////////////////////////////
// The program entry point                                         //
/////////////////////////////////////////////////////////////////////

ClassEdge *EdgeMain(EDGESTARTUP *data){ return(new ClassMain); }
