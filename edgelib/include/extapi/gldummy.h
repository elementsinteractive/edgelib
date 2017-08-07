// gldummy.h
// Dummy interface for OpenGL
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

bool EdgeGL_Use(void){ return(false); }
bool EdgeGL_FixedPoint(void){ return(true); }
void *EdgeGL_NewDriver(void){ return(NULL); }
void EdgeGL_FreeDriver(void *driver){ }
ERESULT EdgeGL_Open(void *driver, void *dhandle, unsigned char preferredmode, unsigned long width, unsigned long height, bool fullscreen, bool stencil){ return(E_NOACTION); }
void EdgeGL_Close(void *driver){ }
void EdgeGL_UpdateDisplay(void *driver){ }
void EdgeGL_Suspend(void *driver){ }
void EdgeGL_Resume(void *driver){ }

void EdgeGL_Flip(void *driver){ }

unsigned char EdgeGL_GetVideoType(void *driver){ return(EDSP_NONE); }
unsigned char EdgeGL_GetStencilBits(void *driver){ return 0; }

void EdgeGL_GetVideoSize(void *driver, unsigned long &width, unsigned long &height){ }

unsigned long EdgeGL_GenTexture(void *driver){ return(0); }
void EdgeGL_DeleteTexture(void *driver, unsigned long texid){ }
void EdgeGL_SelectTexture(void *driver, unsigned long texid){ }
ERESULT EdgeGL_UpdateTextureData(void *driver, unsigned long texid, unsigned long width, unsigned long height, RECT *rc, unsigned char colformat, unsigned char pixformat, void *ptr, bool updatepixels){ return(E_UNSUPPORTED); }
ERESULT EdgeGL_UpdateCompressedTexture(void *driver, unsigned long texid, unsigned long width, unsigned long height, RECT *rc, char *formatstr, void *ptr, bool updatepixels){ return(E_UNSUPPORTED); }

void EdgeGL_EnableLight(void *driver, unsigned char nr, bool enable){ }
void EdgeGL_LightPosition(void *driver, unsigned char nr, long x, long y, long z){ }
void EdgeGL_LightIntensity(void *driver, unsigned char nr, COLORREF ambient, COLORREF diffuse, COLORREF specular){ }

void EdgeGL_SetColor(void *driver, COLORREF col, unsigned char type){ }
void EdgeGL_SetShading(void *driver, unsigned char type){ }
void EdgeGL_SetTextureBlending(void *driver, unsigned char blendtype, unsigned char value){ }
void EdgeGL_UseTexturing(void *driver, bool enable){ }
void EdgeGL_Ortho(void *driver, long *coords){ }
void EdgeGL_Perspective(void *driver, long *coords){ }
void EdgeGL_PerspectiveMatrix(void *driver, void *matrix){ }
void EdgeGL_Scissor(void *driver, long *rect){ }

void EdgeGL_IdentityMatrix(void *driver){ }
void EdgeGL_PushMatrix(void *driver){ }
void EdgeGL_PopMatrix(void *driver){ }
void EdgeGL_MultiplyMatrix(void *driver, void *matrix){ }

void EdgeGL_Clear(void *driver, bool colorbit, bool depthbit){ }
void EdgeGL_RenderModel(void *driver, void **arrays, long *strides, unsigned long count, bool wireframe, bool strip){ }
void EdgeGL_SetOrientation(void * driver, long orientation) { }
void EdgeGL_Screenshot(void * driver, void * buffer, int x1, int y1, int x2, int y2) { }
void EdgeGL_Repair(void * driver) { }
