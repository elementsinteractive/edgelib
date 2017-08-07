// edisplay.h
// The display and graphics module
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED     //Can only be included by the main engine
#ifndef EDISPLAY_INCLUDED //Make sure the module isn't included multiple times
#define EDISPLAY_INCLUDED

///////////////////////////////////////////////////////////////////
// Type definitions                                              //
///////////////////////////////////////////////////////////////////

#define EDSP_NONE              0
#define EDSP_INDEXED8          1
#define EDSP_TRUE12            2
#define EDSP_TRUE15            3
#define EDSP_TRUE16            4
#define EDSP_TRUE24            5
#define EDSP_TRUE24_INV        6
#define EDSP_TRUE32            7
#define EDSP_TRUE32_INV        8
#define EDSP_YUV12             9
#define EDSP_YVU12            10
#define EDSP_YUV420           11
#define EDSP_UYVY             12
#define EDSP_YVYU             13
#define EDSP_YUYV             14
#define EDSP_YUV444           15
#define EDSP_YUV444s          16
#define EDSP_JPEG             17
#define EDSP_EXIF             18
#define EDSP_UNKNOWN          31
#define EDSP_DEFAULT          32 //Video display modes

#define EFX_NONE               0x0
#define EFX_COLORKEY           0x1
#define EFX_COLORFILL          0x2
#define EFX_TINT               0x4
#define EFX_COLORIZE           0x6
#define EFX_OPACITY            0x8
#define EFX_ADD                0x10
#define EFX_SUB                0x20
#define EFX_OPACITY_OVH        0x40
#define EFX_OPACITY_OVL        0x80
#define EFX_COLOREFFECT        0xFE
#define EFX_COLORFILTER        0xFF  //Special effects
#define EFX_DITHER             0x100 //Dither effect with gouraud shading
#define EFX_ALPHA              0x10  //Deprecated

#define E3D_NONE               0x0
#define E3D_WIREFRAME          0x100
#define E3D_FLAT               0x200
#define E3D_GOURAD             0x400
#define E3D_GOURAUD            0x400
#define E3D_TEXTURE            0x800
#define E3D_SHADINGFILTER      0xF00 //3D rendering methods

#define ETEX_AFFINE            0
#define ETEX_PERSPECTIVE       1     //Texture mapping methods

#define EFO_HLEFT              0x0
#define EFO_HRIGHT             0x100
#define EFO_HCENTER            0x200
#define EFO_VTOP               0x0
#define EFO_VBOTTOM            0x400
#define EFO_VCENTER            0x800 //Font outlining flags

#define E2D_ALLOWNATIVE        0x40000000
#define E2D_PREVENTNATIVE      0x80000000 //Extra blit options

#define EFLIP_NONE             0
#define EFLIP_HORIZONTAL       1
#define EFLIP_VERTICAL         2 //Flip blit flags

#define EST_NONE               0
#define EST_DEFAULT            0
#define EST_SYSMEMORY          1
#define EST_SYSTEMAPI          2
#define EST_HARDWARE           3
#define EST_TEXTURE            4
#define EST_COMPRESSEDTEX      5
#define EST_DITHER            16
#define EST_READONLY          32
#define EST_MATCHPALETTE      64
#define EST_ALPHACHANNEL     128
#define EST_DSPINDEXED8       (EDSP_INDEXED8 << 8)
#define EST_DSPTRUE12         (EDSP_TRUE12 << 8)
#define EST_DSPTRUE15         (EDSP_TRUE15 << 8)
#define EST_DSPTRUE16         (EDSP_TRUE16 << 8)
#define EST_DSPTRUE24         (EDSP_TRUE24 << 8)
#define EST_DSPTRUE24_INV     (EDSP_TRUE24_INV << 8)
#define EST_DSPTRUE32         (EDSP_TRUE32 << 8)
#define EST_DSPTRUE32_INV     (EDSP_TRUE32_INV << 8)
#define EST_NONORMALARRAY    256
#define EST_NOCOLORARRAY     512
#define EST_NOTEXCOORDARRAY 1024
#define EST_NOINTERLEAVE    2048
#define EST_FLAGTYPE         255
#define EST_DSPTYPE        65280 //Surface types/flags
#define EST_READONLYPATH   65536 //Extra surface creation parameters

#define EWIPE_SLIDELEFT        0
#define EWIPE_SLIDERIGHT       1
#define EWIPE_SLIDEUP          2
#define EWIPE_SLIDEDOWN        3
#define EWIPE_BLOCKS           4
#define EWIPE_PIXELATE         5
#define EWIPE_BLEND            6
#define EWIPE_SCREEN           7 //Wipe types

#define EWIPE_SCREENDISSOLVE   0 //Screen wipe displacement map types

#define ECOLOR_BLACK           RGB(  0,   0,   0)
#define ECOLOR_GREEN           RGB(  0, 128,   0)
#define ECOLOR_SILVER          RGB(192, 192, 192)
#define ECOLOR_LIME            RGB(  0, 255,   0)
#define ECOLOR_GRAY            RGB(128, 128, 128)
#define ECOLOR_OLIVE           RGB(128, 128,   0)
#define ECOLOR_WHITE           RGB(255, 255, 255)
#define ECOLOR_YELLOW          RGB(255, 255,   0)
#define ECOLOR_MAROON          RGB(128,   0,   0)
#define ECOLOR_NAVY            RGB(  0,   0, 128)
#define ECOLOR_RED             RGB(255,   0,   0)
#define ECOLOR_BLUE            RGB(  0,   0, 255)
#define ECOLOR_PURPLE          RGB(128,   0, 128)
#define ECOLOR_TEAL            RGB(  0, 128, 128)
#define ECOLOR_FUCHSIA         RGB(255,   0, 255)
#define ECOLOR_AQUA            RGB(  0, 255, 255) // HTML 4.01 color definitions
#define ECOLOR_ORANGE          RGB(255, 165,   0) // Additional CSS 2.1 color definition

#define EFR_DEFAULT			   0
#define EFR_ALWAYS			   1
#define EFR_NEVER              2 // Font reordering

#define E3DFOCAL            2048 //Perspective focal point
#define E3D_MAXLIGHTS          8 //Maximum amount of lights for the Edge internal renderer
#define EKERNING_WILDCARD   0x7F //Wildcard character for font kerning

#define DOM_NONE               0
#define DOM_DEFAULT            0
#define DOM_FRAMEBUFFER        1
#define DOM_DIRECTDRAW         2
#define DOM_DSA                2
#define DOM_XV                 2
#define DOM_GDI                3
#define DOM_GAPI               4 //Display open methods

#define MAX_GRAPHICSLOADERS   16 //Maximum amount of registered loaders

class E2DSurface;
class E2DSurfaceRGBA;
class E3DSurface;
class ClassEDisplay;
class ClassEGL;
//Class prototypes

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

//A point in 3D space
typedef struct
{
	long x;
	long y;
	long z;
}EVECTOR3D;

//A 3D point with color, normal and texture coordinates
typedef struct
{
	EVECTOR3D position;
	EVECTOR3D normal;
	COLORREF color;
	long u;
	long v;
}EVERTEX3D;

//Information about a 2D surface
typedef struct
{
	long xpitch;
	long ypitch;
	long realpitch;
	long bufferoffset;
	unsigned long width;
	unsigned long height;
	unsigned long realwidth;
	unsigned long realheight;
	unsigned char bitwidth;
	unsigned char pixelformat;
}E2DSURFACEINFO;

//Information about a 3D surface
typedef struct
{
	long *vertexarray;
	signed char *normalarray;
	COLORREF *colorarray;
	long *texcoordarray;
	unsigned short *indexarray;
	long *matrix;
	long vertexstride;
	long normalstride;
	long colorstride;
	long texcoordstride;
	bool useindices;
}E3DSURFACEINFO;

//A blit effect structure for use in the function E2DSurface::BltFx()
typedef struct
{
	long flags;
	unsigned long property;
	unsigned long xscale;
	unsigned long yscale;
	unsigned long rotation;
	unsigned char flipflags;
}E2DBLTFX;

//A font information structure for the font builder
typedef struct
{
	WCHAR charstart;
	WCHAR charend;
	long xspacing;
	long yspacing;
	long rowchars;
	long defaultchar;
	unsigned long numrows;
}E2DFONTINFO;

//3D joint structure
typedef struct
{
	long matrix[80];
	long position[3];
	long rotation[3];
	short parent;
	unsigned long countanimpos;
	unsigned long countanimrot;
	unsigned short vertexcount;
	unsigned short *ref;
	long *animdata;
}E3DJOINT;

typedef struct
{
	bool enabled;
	long position[3];
	COLORREF intensity[2];
}E3DLIGHT;
//An internal 3D light structure

///////////////////////////////////////////////////////////////////
// Pixel shader plugin support                                   //
///////////////////////////////////////////////////////////////////

#define EPIXELSHADER unsigned long (*shader)(unsigned long dstpixel, unsigned long srcpixel, unsigned long x, unsigned long y, long param)

#if defined(DEVICE_ANDROID)
    #define EDGE_SAFE_CONTEXT false
#else
    #define EDGE_SAFE_CONTEXT true
#endif

#include "esurface.h"


///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class ClassEDisplay
{
	friend class ClassEdge;
	friend class ClassEInput;
	friend class DirectShowCamera; // better create a GetCurrentOrientation or similar...
	friend class SymbianCameraInterface;
	friend class E2DSurfaceBase;
	friend class E2DSurface;
	friend class E2DDisplaySurface;
	friend class E2DSurfaceRGBA;
	friend class E3DSurface;
	private:
		E2DSURFACEINFO dispinfo;
		ECOREOBJ objdsp;
		ECOREOBJ curinst;
		ECOREOBJ curwinstyle;
		ClassELoaderBase *graphicsloader[MAX_GRAPHICSLOADERS];
		E2DSurfaceBase **surflist;
		E3DSurface **obj3dlist;
		E2DSurface undermsg;
		E2DSurface exbuffer;
		E2DSurfaceBase *usetex;
		RECT fliprc;
		ClassEdge *edge;
		ECD *ecd;
		E3DLIGHT *lightdata;
		void *gldriver;
		void **edgerender;
		COLORREF videopalette[256];
		long mtxworld[16];
		long mtxperspective[16];
		long prevwinrc[4];
		unsigned long quickpalette[257];
		unsigned char *palettelookup;
		long orienth;
		long orientv;
		long realxpitch;
		long realypitch;
		unsigned long screenwidth;
		unsigned long screenheight;
		unsigned long prevwipetimer;
		unsigned long rendersize;
		unsigned long demotimer;
		unsigned long curshading;
		unsigned long curshadingprop;
		unsigned long surflistsize;
		unsigned long obj3dlistsize;
		unsigned char realvideomode;
		unsigned char videomode;
		unsigned char rotation;
		unsigned char sysorientation;
		unsigned char openmethod;
		unsigned char skipflip;
		bool refreshpalette;
		bool switchxy;
		bool fullscreenmode;
		bool suspended;
		bool yreverse;
		bool prerotation;
		bool tmpsuspend;
		bool resizefromsip;
		bool suspendfrommode;
		bool perspectivemapping;
		void VirtualPos(long &x, long &y, unsigned long sw, unsigned long sh);
		void ReVirtualRect(RECT *rc, unsigned long sw, unsigned long sh);
		void VirtualRect(RECT *rc, unsigned long sw, unsigned long sh);
		void VirtualRectPos(long &x, long &y, RECT *rc, unsigned long sw1, unsigned long sh1, unsigned long sw2, unsigned long sh2);
		void VirtualRectPos(long &x, long &y, RECT *rc, unsigned long sw1, unsigned long sh1, unsigned long sw2, unsigned long sh2, unsigned short rprotation);
		void VirtualMouse(long &x, long &y);
		void VirtualDPad(unsigned long &bnr, EBUTTONLIST *blist);
		void VirtualTilt(long &x, long &y);
		void VirtualRotation(unsigned short &r);
		bool ShowNagMessage(void);
		ERESULT CreateBuffer(void);
		ERESULT Initialize(void);
		bool BuildPaletteLookupIndex(unsigned char *lookup, COLORREF *srcpalette);
		bool BuildPaletteLookup(void);
		long QueryTextureHandle(void);
		void ReleaseTextureHandle(long texhandle);
		void WriteTextureData(long texhandle, unsigned long &texwidth, unsigned long &texheight, unsigned long surfwidth, unsigned long surfheight, unsigned long pformat, bool fromrgba, void *ptr, bool update);
		void WriteCompressedTextureData(long texhandle, unsigned long &texwidth, unsigned long &texheight, unsigned long surfwidth, unsigned long surfheight, char *formatstr, void *ptr, bool update);
		bool HandleSurfList(E2DSurfaceBase *surf, bool inc = true);
		bool HandleObj3DList(E3DSurface *surf, bool inc = true);
		void OnContextLost(void);
		void OnChangeSize(RECT *newsize = NULL);
		void OnChangeMode(void);
	public:
		E2DDisplaySurface buffer;
		E2DSurface fontinternal;
		ClassEDisplay(void);
		~ClassEDisplay(void);
		ERESULT Open(ECOREOBJ inst, ECOREOBJ winstyle, ECD *pecd);
		void Close(void);
		void Suspend(void);
		void Resume(void);
		bool ChangeOrientation(unsigned char orientation);
		void Flip(bool forceblack = false);
		void BackFlip(void);
		void Clear(bool colorbuffer = true, bool depthbuffer = true);
		void SetFlipRect(RECT *rc = NULL);
		void MapScreenWipe(unsigned char *dst, unsigned long width, unsigned long height, unsigned char type);
		void StartWipe(unsigned char type = 0, E2DSurface *dst = NULL);
		void Wipe(E2DSurface *dst, E2DSurface *src1, E2DSurface *src2, unsigned long timer, unsigned char type, void *customdata = NULL, long property = 0);
		void SetRenderSize(unsigned long newsize);
		ERESULT GetVideoMemory(unsigned long &memfree, unsigned long &memtotal);
		unsigned char GetSystemRotation(void);
		unsigned long GetWidth(void);
		unsigned long GetHeight(void);
		unsigned char GetStencilBits(void);
		void GetWorldMatrix(long *matrix);
		void GetPerspectiveMatrix(long *matrix);
		void CalculatePerspective(EVECTOR3D *vector, POINT *point);
		void SetPalette(COLORREF *palette, unsigned char index = 0, unsigned long entries = 1);
		void GetPalette(COLORREF *palette, unsigned char index = 0, unsigned long entries = 1);
		void UseLight(unsigned char nr, bool enable = true);
		void SetLightSource(unsigned char nr, long x, long y, long z, COLORREF diffuse = ECOLOR_BLACK, COLORREF ambient = ECOLOR_BLACK, COLORREF specular = ECOLOR_BLACK);
        ERESULT UploadTexture(E2DSurface *texture, bool uploadrgba = false, bool cleardata = EDGE_SAFE_CONTEXT, bool conversion = true, bool autorepair = true);
        ERESULT UploadTexture(E2DSurfaceRGBA *texture, bool cleardata = EDGE_SAFE_CONTEXT, bool autorepair = true);
		void BindTexture(E2DSurface *texture);
		void BindTexture(E2DSurfaceRGBA *texture);
		void SetShading(unsigned long type, unsigned long property = 0);
		bool RegisterLoader(ClassELoaderBase *loader);
		bool UnRegisterLoader(ClassELoaderBase *loader);
		ERESULT CreateSurface(E2DSurface *surface, unsigned long width, unsigned long height, unsigned long usetype = EST_DEFAULT);
		ERESULT CreateSurface(E2DSurface *surface, void *imgmem, unsigned long memsize, unsigned long usetype = EST_DEFAULT);
		ERESULT CreateSurface(E2DSurface *surface, char *imgfile, unsigned long usetype = EST_DEFAULT);
		ERESULT CreateSurface(E2DSurface *surface, WCHAR *imgfile, unsigned long usetype = EST_DEFAULT);
		ERESULT CreateSurface(E3DSurface *surface, unsigned long vertices, unsigned long polygons, unsigned long usetype = EST_DEFAULT);
		ERESULT CreateSurface(E3DSurface *surface, void *mdlmem, unsigned long memsize, signed char modelsize = 0, unsigned long usetype = EST_DEFAULT);
		ERESULT CreateSurface(E3DSurface *surface, char *mdlfile, signed char modelsize = 0, unsigned long usetype = EST_DEFAULT);
		ERESULT CreateSurface(E3DSurface *surface, WCHAR *mdlfile, signed char modelsize = 0, unsigned long usetype = EST_DEFAULT);
		ERESULT CreateSurface(E2DSurfaceRGBA *surface, unsigned long width, unsigned long height, unsigned long usetype = EST_DEFAULT);
		ERESULT CreateSurface(E2DSurfaceRGBA *surface, void *imgmem, unsigned long memsize, unsigned long usetype = EST_DEFAULT);
		ERESULT CreateSurface(E2DSurfaceRGBA *surface, char *imgfile, unsigned long usetype = EST_DEFAULT);
		ERESULT CreateSurface(E2DSurfaceRGBA *surface, WCHAR *imgfile, unsigned long usetype = EST_DEFAULT);
		void SetTextureMapMethod(unsigned char method);
		void SetPerspectiveMatrix(long *matrix);
		void Frustum(long xmin, long xmax, long ymin, long ymax, long znear, long zfar);
		void SetClipper(RECT *rc);
		void Perspective2D(long width, long height, bool ypointsup = true);
		void Perspective3D(long width, long height, long fov = 512, long xoffs = 0, long yoffs = 0);
		void WorldReset(void);
		void WorldTranslate(long x, long y, long z);
		void WorldRotate(long xr, long yr, long zr);
		void WorldScale(long xz, long yz, long zz);
		void DrawFont(long x, long y, E2DSurface *source, const char *text, long flags = 0);
		void DrawFont(long x, long y, E2DSurface *source, const WCHAR *text, long flags = 0);
		void DrawFont(long x, long y, E2DSurface *source, long value, long flags = 0);
		void DrawFont(long x, long y, E2DSurfaceRGBA *source, const char *text, long flags = 0);
		void DrawFont(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, long flags = 0);
		void DrawFont(long x, long y, E2DSurfaceRGBA *source, long value, long flags = 0);
		void DrawFontWrap(long x, long y, E2DSurface *source, const char *text, unsigned long width, long flags = 0);
		void DrawFontWrap(long x, long y, E2DSurface *source, const WCHAR *text, unsigned long width, long flags = 0);
		void DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const char *text, unsigned long width, long flags = 0);
		void DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, unsigned long width, long flags = 0);
		void DrawRect(RECT *rc, COLORREF col);
		void DrawLine(long x1, long y1, long x2, long y2, COLORREF col);
		void DrawOval(long x, long y, long xradius, long yradius, COLORREF col);
		void FillRect(RECT *rc, COLORREF col);
		void FillOval(long x, long y, long xradius, long yradius, COLORREF col);
		void BltFast(long x, long y, E2DSurface *source, RECT *rc);
		void BltFast(long x, long y, E2DSurfaceRGBA *source, RECT *rc);
		void Blt(long center_x, long center_y, E2DSurface *source, RECT *rc, unsigned short rotation, unsigned long scale, unsigned char flipflags = EFLIP_NONE);
		void Blt(long center_x, long center_y, E2DSurfaceRGBA *source, RECT *rc, unsigned short rotation, unsigned long scale, unsigned char flipflags = EFLIP_NONE);
		void BltFx(long center_x, long center_y, E2DSurface *source, RECT *rc, E2DBLTFX *bltfx);
		void BltFx(long center_x, long center_y, E2DSurfaceRGBA *source, RECT *rc, E2DBLTFX *bltfx);
		void Render(E3DSurface *surface);
};//The display engine

#endif
#endif
