// esurface.h
// The surface classes
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED     //Can only be included by the main engine
#ifndef ESURFACE_INCLUDED //Make sure the module isn't included multiple times
#define ESURFACE_INCLUDED

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class E2DSurfaceBase
{
	friend class ClassEDisplay;
	friend class ClassEdge;
	friend class E2DDisplaySurface;
	friend class E3DSurface;
    protected:
		ECD *ecd;
		COLORREF *palette;
		WCHAR *fontcharset;
		E2DFONTINFO fontinfo;
		RECT cliprc;
		long exthandle;
		short *fontrc;
		short *fontkerning;
		unsigned long *cbitmask;
		char *texformat;
		long vpitch;
		unsigned long swidth;
		unsigned long sheight;
		unsigned long extwidth;
		unsigned long extheight;
		unsigned long virtualwidth;
		unsigned long createtype;
		unsigned char videomode;
		unsigned char bitwidth;
		unsigned char currenttype;
        bool dirty;
        bool autorepair;
		bool ClipRect(RECT *rc);
		bool PrepareBlitRect(long &x, long &y, RECT *rc, long rcwidth, long rcheight, long dwidth, long dheight);
		bool PrepareBlitFlipRect(long &x, long &y, RECT *rc, long rcwidth, long rcheight, long dwidth, long dheight, unsigned short rotation, unsigned char flipflags);
		ERESULT GetBltFastPositions(long &x, long &y, RECT *drct, RECT *srct, E2DSurfaceBase *source);
		ERESULT GetBltFlipPositions(long &x, long &y, RECT *drct, RECT *srct, E2DSurfaceBase *source, unsigned short rotation, unsigned char &flipflags);
        virtual void OnChangeOrientation(long *olddata);
        virtual void Reload(ClassEDisplay *dsp);
	public:
		E2DSurfaceBase(void);
		//#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
		//	~E2DSurfaceBase(void);
		//#else
			virtual ~E2DSurfaceBase(void);
		//#endif
		void virtual Free(void);
		bool IsCreated(void);
		virtual unsigned long GetWidth(void);
		virtual unsigned long GetHeight(void);
		void GetPalette(COLORREF *palettedata, unsigned char index = 0, unsigned long entries = 1);
		unsigned long GetFontWidth(char *text);
		unsigned long GetFontWidth(WCHAR *text);
		unsigned long GetFontHeight(char *text = NULL);
		unsigned long GetFontHeight(WCHAR *text);
		void PrepareFontWrap(char *dst, char *src, unsigned long width);
		void PrepareFontWrap(WCHAR *dst, WCHAR *src, unsigned long width);
		bool Intersect(long x1, long y1, RECT *rc1, long x2, long y2, RECT *rc2, E2DSurfaceBase *cmp, POINT *pnt = NULL);
		void SetFontPadding(long horizontal, long vertical = 0);
		void SetFontCharacters(WCHAR start = 32, WCHAR end = 127);
		void SetFontCharacterSet(char *set);
		void SetFontCharacterSet(WCHAR *set);
		void SetFontSheetLayout(unsigned char numrows, short rowchars = -1);
		void SetFontKerning(WCHAR charleft, WCHAR charright, long kerning);
		void SetFontDefaultChar(long index);
		ERESULT virtual LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata);
};//2D Surface base class

class E2DSurface : public E2DSurfaceBase
{
	friend class ClassEDisplay;
	private:
		ECOREOBJ nativesurf;
		short *xmin;
		short *xmax;
		EINT32 *ledge;
		EINT32 *redge;
		unsigned char *gdata;
		unsigned char *indexlookup;
		unsigned long colorkey;
		unsigned long locked;
        bool reload_convert;
        bool reload_alpha;
		void MemBlt(EPIXELSHADER, long x, long y, unsigned char *dst, unsigned char *src, RECT *rc, unsigned long dstpitch, unsigned long srcpitch, unsigned char videomode, unsigned long property);
		void MemBltColorkey(EPIXELSHADER, long x, long y, unsigned char *dst, unsigned char *src, RECT *rc, unsigned long dstpitch, unsigned long srcpitch, unsigned char videomode, unsigned long property, COLORREF colorkey);
		void ConvertColor(COLORREF &col, unsigned char vmode);
		void RevertColor(COLORREF &col, unsigned char vmode);
		void DitherColor(COLORREF &col, unsigned long index);
		bool CheckFilterFill(COLORREF &col, long &flags, unsigned long &property);
		bool CheckFilterBlt(long &flags, unsigned long &property);
		bool CheckFilterBlt(long &flags, unsigned long &colorize, unsigned char &opacity, unsigned long property);
		void ScanPolyLine(long x1, long y1, long x2, long y2);
		void ScanPolyLine(long x1, long y1, long x2, long y2, long u1, long v1, long u2, long v2);
		void ScanPolyLinePz(long x1, long y1, long z1, long x2, long y2, long z2, long u1, long v1, long u2, long v2);
		void ScanPolyLine(long x1, long y1, long x2, long y2, long r1, long g1, long b1, long r2, long g2, long b2);
		COLORREF GetPixelRaw(long x, long y, unsigned char *memptr);
		void PutPixelRaw(long x, long y, COLORREF col, long flags, unsigned long property, unsigned char *memptr);
		COLORREF GetPixelV(long x, long y);
		void FillRectV(RECT *rc, COLORREF col, long flags, unsigned long property, bool pconv);
		void PutPixelRowV(long x, long y, unsigned long width, COLORREF col, long flags, unsigned long property, bool pconv);
		void TexturePolyV(long x1, long y1, long u1, long v1, long x2, long y2, long u2, long v2, long x3, long y3, long u3, long v3, E2DSurface *source, long flags = 0, unsigned long property = 0);
		void TexturePolyV(long x1, long y1, long w1, long u1, long v1, long x2, long y2, long w2, long u2, long v2, long x3, long y3, long w3, long u3, long v3, E2DSurface *source, long flags = 0, unsigned long property = 0);
		void FillPolyV(long x1, long y1, long x2, long y2, long x3, long y3, COLORREF col, long flags = 0, unsigned long property = 0);
		void GradientPolyV(long x1, long y1, COLORREF col1, long x2, long y2, COLORREF col2, long x3, long y3, COLORREF col3, long flags = 0);
		ERESULT BltFlip(long x, long y, E2DSurface *source, RECT *rc, unsigned short rotation, unsigned char flipflags, long flags, unsigned long property);
		void LinkECD(ECD *pecd);
		ERESULT LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT Create(unsigned long width, unsigned long height, unsigned char passmode, unsigned long usetype);
		ERESULT LoadImage(void *imgmem, unsigned long memsize, unsigned char passmode, unsigned long usetype);
		ERESULT LoadImage(WCHAR *imgfile, unsigned char passmode, unsigned long usetype);
		ERESULT Init(void);
		void OnChangeOrientation(long *olddata);
        virtual void Reload(ClassEDisplay *dsp);
	public:
		ENATIVETYPE GetSysApiSurface(void);
		E2DSurface(void);
		virtual ~E2DSurface(void);		
		void BuildTableMask(unsigned char brightness = 0);
		void WriteBMP(char *filename, RECT *rc = NULL);
		void WriteBMP(WCHAR *filename, RECT *rc = NULL);
		void WritePNG(char *filename, RECT *rc = NULL);
		void WritePNG(WCHAR *filename, RECT *rc = NULL);
		unsigned long GetColorKey(void);
		bool GetInfo(E2DSURFACEINFO *surfaceinfo);
		bool GetLandscapePoint(RECT *viewport, long pitch, long mapx, long mapy, unsigned short turn, short horizon, unsigned short scale, long objx, long objy, E2DSurface *source, EVECTOR3D *vector);
		void GetClipper(RECT *rc);
		void SetColorKey(COLORREF col = ECOLOR_BLACK);
		void AutoSetColorKey(long x = 0, long y = 0);
		void SetClipper(RECT *rc = NULL);
		void BuildFont(bool variablewidth = false, COLORREF fontcolorkey = ECOLOR_BLACK, long autoreorder = EFR_DEFAULT);
		bool ReplaceColor(COLORREF dst, COLORREF src, bool exclude = false);
		void Resize(unsigned long width, unsigned long height = 0, bool usefilter = true);
		COLORREF GetPixel(long x, long y);
		void PutPixelRow(long x, long y, unsigned long width, COLORREF col, long flags = 0, unsigned long property = 0);
		void TexturePoly(long x1, long y1, long u1, long v1, long x2, long y2, long u2, long v2, long x3, long y3, long u3, long v3, E2DSurface *source, long flags = 0, unsigned long property = 0);
		void TexturePoly(long x1, long y1, long w1, long u1, long v1, long x2, long y2, long w2, long u2, long v2, long x3, long y3, long w3, long u3, long v3, E2DSurface *source, long flags = 0, unsigned long property = 0);
		void FillPoly(long x1, long y1, long x2, long y2, long x3, long y3, COLORREF col, long flags = 0, unsigned long property = 0);
		void GradientPoly(long x1, long y1, COLORREF col1, long x2, long y2, COLORREF col2, long x3, long y3, COLORREF col3, long flags = 0);
		void GradientRect(RECT *rc, COLORREF topleft, COLORREF topright, COLORREF bottomleft, COLORREF bottomright, long flags = 0);
		void GradientRectH(RECT *rc, COLORREF left, COLORREF right, long flags = 0);
		void GradientRectV(RECT *rc, COLORREF top, COLORREF bottom, long flags = 0);
		void Landscape(RECT *viewport, long pitch, long mapx, long mapy, unsigned short turn, short horizon, unsigned short scale, E2DSurface *source);
		bool DirtyFlush(); 

		virtual unsigned char *Lock(E2DSURFACEINFO *surfaceinfo = NULL);
		virtual void Unlock(void);
		virtual void Free(void);
		virtual void Clear(COLORREF col = ECOLOR_BLACK);
		virtual void DrawFont(long x, long y, E2DSurface *source, const char *text, long flags = EFX_COLORKEY, unsigned long property = 0);
		virtual void DrawFont(long x, long y, E2DSurface *source, const WCHAR *text, long flags = EFX_COLORKEY, unsigned long property = 0);
		virtual void DrawFont(long x, long y, E2DSurface *source, long value, long flags = EFX_COLORKEY, unsigned long property = 0);
		virtual void DrawFont(long x, long y, E2DSurfaceRGBA *source, const char *text, long flags = EFX_NONE, unsigned long property = 0);
		virtual void DrawFont(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, long flags = EFX_NONE, unsigned long property = 0);
		virtual void DrawFont(long x, long y, E2DSurfaceRGBA *source, long value, long flags = EFX_NONE, unsigned long property = 0);
		virtual void DrawFontWrap(long x, long y, E2DSurface *source, const char *text, unsigned long width, long flags = EFX_COLORKEY, unsigned long property = 0);
		virtual void DrawFontWrap(long x, long y, E2DSurface *source, const WCHAR *text, unsigned long width, long flags = EFX_COLORKEY, unsigned long property = 0);
		virtual void DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const char *text, unsigned long width, long flags = EFX_NONE, unsigned long property = 0);
		virtual void DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, unsigned long width, long flags = EFX_NONE, unsigned long property = 0);		
		virtual void PutPixel(long x, long y, COLORREF col, long flags = 0, unsigned long property = 0);		
		
		virtual ERESULT BltFast(long x, long y, E2DSurface *source, RECT *rc, long flags = 0, unsigned long property = 0);
		virtual ERESULT BltFast(long x, long y, E2DSurfaceRGBA *source, RECT *rc, long flags = 0, unsigned long property = 0);
		virtual ERESULT BltPixelShader(long x, long y, E2DSurface *source, RECT *rc, EPIXELSHADER, long flags = 0, unsigned long property = 0);
		virtual ERESULT Blt(long center_x, long center_y, E2DSurface *source, RECT *rc, unsigned short rotation, unsigned long scale, unsigned char flipflags = EFLIP_NONE, long flags = 0, unsigned long property = 0);
		virtual ERESULT BltFx(long center_x, long center_y, E2DSurface *source, RECT *rc, E2DBLTFX *bltfx);
		virtual ERESULT BltStretch(RECT *dstrc, E2DSurface *source, RECT *srcrc, unsigned char flipflags = EFLIP_NONE, long flags = 0, unsigned long property = 0);
		
		virtual void DrawLine(long x1, long y1, long x2, long y2, COLORREF col, long flags = 0, unsigned long property = 0);
		virtual void DrawOval(long x, long y, long xradius, long yradius, COLORREF col, long flags = 0, unsigned long property = 0);
		virtual void DrawRect(RECT *rc, COLORREF col, long flags = 0, unsigned long property = 0);				
		virtual void FillRect(RECT *rc, COLORREF col, long flags = 0, unsigned long property = 0);
		virtual void FillOval(long x, long y, long xradius, long yradius, COLORREF col, long flags = 0, unsigned long property = 0);

		virtual void Render(long x, long y, E3DSurface *source, unsigned long flags = E3D_FLAT, unsigned long property = 0);		
};//The surface containing 2D bitmapped graphics data

class ClassEDisplay;

class E2DDisplaySurface : public E2DSurface
{
	private:
		void * gldriver;
		ClassEDisplay * display;
		long lockcount;
		unsigned long syswidth, sysheight;

		void DoUpload(E2DSurface * surface);
		void DoUpload(E2DSurfaceRGBA * surface, bool font);
		void PrepareBlit(unsigned long flags, unsigned long property);
        virtual void Reload(ClassEDisplay *dsp);

	protected:
		bool ClipTextureRect(long &x, long &y, RECT * rc, long texw, long texh); 
		void Lockrotate(E2DSURFACEINFO *surfaceinfo);

	public:
		E2DDisplaySurface(ClassEDisplay *);
		virtual ~E2DDisplaySurface();
		void SetGLDriver(void * driver);

		virtual unsigned char *Lock(E2DSURFACEINFO *surfaceinfo = NULL);
		virtual void Unlock(void);
		virtual void Free(void);
		virtual void Clear(COLORREF col = ECOLOR_BLACK);
		virtual void DrawFont(long x, long y, E2DSurface *source, const char *text, long flags = EFX_COLORKEY, unsigned long property = 0);
		virtual void DrawFont(long x, long y, E2DSurface *source, const WCHAR *text, long flags = EFX_COLORKEY, unsigned long property = 0);
		virtual void DrawFont(long x, long y, E2DSurface *source, long value, long flags = EFX_COLORKEY, unsigned long property = 0);
		virtual void DrawFont(long x, long y, E2DSurfaceRGBA *source, const char *text, long flags = EFX_NONE, unsigned long property = 0);
		virtual void DrawFont(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, long flags = EFX_NONE, unsigned long property = 0);
		virtual void DrawFont(long x, long y, E2DSurfaceRGBA *source, long value, long flags = EFX_NONE, unsigned long property = 0);
		virtual void DrawFontWrap(long x, long y, E2DSurface *source, const char *text, unsigned long width, long flags = EFX_COLORKEY, unsigned long property = 0);
		virtual void DrawFontWrap(long x, long y, E2DSurface *source, const WCHAR *text, unsigned long width, long flags = EFX_COLORKEY, unsigned long property = 0);
		virtual void DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const char *text, unsigned long width, long flags = EFX_NONE, unsigned long property = 0);
		virtual void DrawFontWrap(long x, long y, E2DSurfaceRGBA *source, const WCHAR *text, unsigned long width, long flags = EFX_NONE, unsigned long property = 0);		
		virtual void PutPixel(long x, long y, COLORREF col, long flags = 0, unsigned long property = 0);		
		virtual void DrawLine(long x1, long y1, long x2, long y2, COLORREF col, long flags = 0, unsigned long property = 0);
		virtual void DrawOval(long x, long y, long xradius, long yradius, COLORREF col, long flags = 0, unsigned long property = 0);
		virtual void DrawRect(RECT *rc, COLORREF col, long flags = 0, unsigned long property = 0);		
		virtual ERESULT BltFast(long x, long y, E2DSurface *source, RECT *rc, long flags = 0, unsigned long property = 0);
		virtual ERESULT BltFast(long x, long y, E2DSurfaceRGBA *source, RECT *rc, long flags = 0, unsigned long property = 0);
		virtual ERESULT BltPixelShader(long x, long y, E2DSurface *source, RECT *rc, EPIXELSHADER, long flags = 0, unsigned long property = 0);
		virtual ERESULT Blt(long center_x, long center_y, E2DSurface *source, RECT *rc, unsigned short rotation, unsigned long scale, unsigned char flipflags = EFLIP_NONE, long flags = 0, unsigned long property = 0);
		virtual ERESULT BltFx(long center_x, long center_y, E2DSurface *source, RECT *rc, E2DBLTFX *bltfx);
		virtual ERESULT BltStretch(RECT *dstrc, E2DSurface *source, RECT *srcrc, unsigned char flipflags = EFLIP_NONE, long flags = 0, unsigned long property = 0);
		virtual void Render(long x, long y, E3DSurface *source, unsigned long flags = E3D_FLAT, unsigned long property = 0);	
		virtual unsigned long GetWidth(void);
		virtual unsigned long GetHeight(void);
		virtual void FillRect(RECT *rc, COLORREF col, long flags = 0, unsigned long property = 0);
		virtual void FillOval(long x, long y, long xradius, long yradius, COLORREF col, long flags = 0, unsigned long property = 0);
		void SetDirty();
};

class E2DSurfaceRGBA : public E2DSurfaceBase
{
	friend class ClassEDisplay;
	friend class E2DSurface;
	private:
		short *xmin;
		short *xmax;
		EINT32 *ledge;
		EINT32 *redge;
		unsigned char *gdata;
		unsigned char *xdraw;
		unsigned long locked;
		unsigned char videomode;
		bool dirty;

		void ConvertColor(COLORREF &col, unsigned char vmode);
		void RevertColor(COLORREF &col, unsigned char vmode);
		void DitherColor(COLORREF &col, unsigned long index);
		void GetPixelV(long x, long y, COLORREF &col, unsigned char &opacity);
		void LinkECD(ECD *pecd);
		ERESULT LdrOnCreate(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT LdrOnPaletteStream(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT LdrOnPixelStream(ELDR_2DCALLBACKINFO *ldrdata);
		ERESULT Create(unsigned long width, unsigned long height, unsigned char passmode, unsigned long usetype);
		ERESULT LoadImage(void *imgmem, unsigned long memsize, unsigned char passmode, unsigned long usetype);
		ERESULT LoadImage(WCHAR *imgfile, unsigned char passmode, unsigned long usetype);
		ERESULT Init(void);
		void OnChangeOrientation(long *olddata);
        virtual void Reload(ClassEDisplay *dsp);
	public:
		E2DSurfaceRGBA(void);
		~E2DSurfaceRGBA(void);
		unsigned char *Lock(E2DSURFACEINFO *surfaceinfo = NULL);
		void Unlock(void);
		void Free(void);
		void Clear(COLORREF col = ECOLOR_BLACK);
		void BuildTableMask(unsigned char opacity = 255);
		void WritePNG(char *filename, RECT *rc = NULL);
		void WritePNG(WCHAR *filename, RECT *rc = NULL);
		bool GetInfo(E2DSURFACEINFO *surfaceinfo);
		void GetClipper(RECT *rc);
		void SetClipper(RECT *rc = NULL);
		bool ReplaceColor(COLORREF dst, COLORREF src, bool exclude = false);
		void BuildFont(bool variablewidth = false, long autoreorder = EFR_DEFAULT);
		void Resize(unsigned long width, unsigned long height = 0, bool usefilter = true);
		void GetPixel(long x, long y, COLORREF &col, unsigned char &opacity);
		void PutPixel(long x, long y, COLORREF col, unsigned char opacity = 255);
		bool DirtyFlush(); 
};//The surface containing 2D bitmapped graphics data and an alpha channel

class E3DLoaderBase
{
	public:
		E3DLoaderBase(void);
		#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
			~E3DLoaderBase(void);
		#else
			virtual ~E3DLoaderBase(void);
		#endif
		ERESULT virtual LdrOnMeshStart(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnMeshEnd(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnObjectStart(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnObjectEnd(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnVertexStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnNormalStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnTexcoordStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnColorStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnIndexStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnLightStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnCameraStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnJointStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT virtual LdrOnKeyFrameStream(ELDR_3DCALLBACKINFO *ldrdata);
		void virtual CreateJoints(unsigned long count);
		void virtual SetJointProperties(unsigned long index, EVECTOR3D *location, EVECTOR3D *rotation, long parent = -1);
		void virtual CreateJointKeyFrames(unsigned long index, unsigned long numtranslate, unsigned long numrotate);
};//The 3D object base class

class E3DSurface : public E3DLoaderBase
{
	friend class ClassEDisplay;
	friend class E2DSurface;
	private:
		ECD *ecd;
		E3DJOINT *joint;
		long *wmatrix;
		long matrix[16];
		long matrix_tmp[16];
		short *sortindexlist;
		void *duplist;
		unsigned short *indexlist;
		unsigned char *coordlist;
		unsigned char *coordlist_transformed;
		unsigned char *drawlist;
		unsigned long lofs_vertex;
		unsigned long lofs_texel;
		unsigned long lofs_normal;
		unsigned long lofs_color;
		unsigned long lstr_vertex;
		unsigned long lstr_texel;
		unsigned long lstr_normal;
		unsigned long lstr_color;
		unsigned long allocpoly;
		unsigned long allocvertex;
		unsigned long numpoly;
		unsigned long numvertex;
		unsigned long numjoint;
		unsigned long coordlistsize;
		unsigned long locked;
		unsigned long dupcount;
		bool modelchanged;
		bool useanimation;
		bool calcloadnormal;
		bool jointnew;
		void LinkECD(ECD *pecd);
		ERESULT Create(unsigned long vertices, unsigned long polygons, unsigned long usetype);
		ERESULT LoadModel(void *mdlmem, unsigned long memsize, unsigned char passmode, signed char modelsize, unsigned long usetype);
		ERESULT LoadModel(WCHAR *mdlfile, unsigned char passmode, signed char modelsize, unsigned long usetype);
		unsigned long PrepareRender(bool lighting, bool coloring, bool smoothcolors, bool texels, bool cull);
	public:
		E3DSurface(void);
		~E3DSurface(void);
		bool Lock(E3DSURFACEINFO *surfaceinfo, bool render = false);
		void Unlock(void);
		void Free(void);
		bool IsCreated(void);
		void GetVertex(unsigned long index, EVERTEX3D *vertex);
		void SetVertex(unsigned long index, long x, long y, long z);
		void SetVertex(unsigned long index, EVERTEX3D *vertex);
		void SetNormal(unsigned long index, signed char x, signed char y, signed char z);
		void SetColor(unsigned long index, COLORREF color);
		void SetTexCoord(unsigned long index, long u, long v);
		void SetPolygon(unsigned long index, unsigned long v1, unsigned long v2, unsigned long v3);
		void SetPolygon(unsigned long index, EVERTEX3D *v1, EVERTEX3D *v2, EVERTEX3D *v3, bool checkduplicates = false);
		void SetModelColor(COLORREF color);
		void SetMatrix(long *newmatrix);
		void CalculateNormals(void);
		void FixTexCoords(E2DSurfaceBase *paddedtexture);
		void Translate(long x, long y, long z);
		void Rotate(long xr, long yr, long zr);
		void Scale(long xz, long yz, long zz);
		void Animate(unsigned long time);
		void Reset(void);
		void ResetAnimation(void);
		void CreateJoints(unsigned long count);
		void AttachJoint(unsigned long index, unsigned short *vertexlist, unsigned short listsize);
		void SetJointProperties(unsigned long index, EVECTOR3D *location, EVECTOR3D *rotation, long parent = -1);
		void CreateJointKeyFrames(unsigned long index, unsigned long numtranslate, unsigned long numrotate);
		void SetJointKeyFrames(unsigned long index, long *keyframelist, unsigned long listsize, bool isrotation = false);
		unsigned long VerticesInUse(void);
		unsigned long PolygonsInUse(void);
		ERESULT LdrOnMeshStart(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnMeshEnd(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnVertexStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnNormalStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnTexcoordStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnColorStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnIndexStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnJointStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnKeyFrameStream(ELDR_3DCALLBACKINFO *ldrdata);
};//The surface containing 3D polygons

#endif
#endif
