// eloader.h
// Edge Loader base class + included loaders
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Loader base definition                                        //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED    //Can only be included by the main engine
#ifndef ELOADER_INCLUDED //Make sure the loaders aren't included multiple times
#define ELOADER_INCLUDED

class E2DSurfaceBase;
class E3DLoaderBase;

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

//2D Loader callback structure
typedef struct
{
	unsigned long width;
	unsigned long height;
	bool usepalette;
	bool usetransparency;
	void *streamdata;
	unsigned long streamsize;
	unsigned long customparam;
	unsigned long userparam;
	unsigned char nativedisplaymode;
	unsigned long createflags;
	unsigned long paletteindex;
	unsigned long pixelindex;
	char *compressedformat;
}ELDR_2DCALLBACKINFO;

//3D Loader callback structure
typedef struct
{
	unsigned long reservevertices;
	unsigned long reserveindices;
	unsigned long reservejoints;
	unsigned long reservekeyframes;
	bool texelonvertex;
	bool coloronvertex;
	bool normalonvertex;
	void *streamdata;
	unsigned long streamsize;
	unsigned long customparam;
	unsigned long userparam;
	unsigned long createflags;
	unsigned long vertexindex;
	unsigned long polygonindex;
	unsigned long texelindex;
	unsigned long colorindex;
	unsigned long normalindex;
	unsigned long jointindex;
	unsigned long keyframeindex;
	unsigned char keyframetype;
}ELDR_3DCALLBACKINFO;

///////////////////////////////////////////////////////////////////
// Loader base class                                             //
///////////////////////////////////////////////////////////////////

class ClassELoaderBase
{
	protected:
		bool isbigendian;
		unsigned char *lmem;
		unsigned long lptr, maxsize;
		void SetByteOrder(bool bigendian);
		bool Read8(unsigned char *dest);
		bool Read16(unsigned short *dest);
		bool Read32(unsigned long *dest);
		bool ReadStream(void *dest, unsigned long rsize);
		void SeekSet(unsigned long pos);
		void SeekRelative(long pos);
		unsigned long Tell(void);
		bool HaveData(void);
		void LinkData(void *ptr, unsigned long psize);
	public:
		ClassELoaderBase(void);
		virtual ~ClassELoaderBase(void);
		ERESULT virtual Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags);
		ERESULT virtual Start(E3DLoaderBase *surface, void *ldata, unsigned long lsize, signed char modelsize, unsigned long createflags);
};//The loader base class

///////////////////////////////////////////////////////////////////
// Included 2D and 3D loaders                                           //
///////////////////////////////////////////////////////////////////

class ELoader_BMP : public ClassELoaderBase
{
	private:
		unsigned char hdrdata[64];
	public:
		ERESULT Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags);
		void WriteHeader(ClassEFile *ef, unsigned long width, unsigned long height, unsigned char coldepth);
		void WritePixelData(ClassEFile *ef, class E2DSurface *surface, RECT *rc, unsigned long *paldata);
};//Loading of uncompressed BMP (Bitmap) files

class ELoader_GIF : public ClassELoaderBase
{
	private:
		unsigned long *str_index;
		unsigned long *str_len;
		unsigned long num_str;
		void AddUCmpString(unsigned char *fulldata, unsigned long srdst_size, unsigned long str_size, unsigned short initsize);
		void GetString(unsigned char *fulldata, unsigned char *str, unsigned long &str_size, unsigned long code, unsigned short initsize);
		bool UnCompress(void *dst, unsigned long dst_size, void *src, unsigned long src_size, unsigned char gifcodesize);
		bool SearchImageData(ELDR_2DCALLBACKINFO *cbinfo, void *phdrinfo, void *pimginfo);
		unsigned char *DecodeImage(void *pimginfo);
	public:
		ERESULT Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags);
};//Loading of Compuserve GIF files

class ELoader_PNG : public ClassELoaderBase
{
	private:
		void RevertBytes(unsigned char *cmem, unsigned long csize, unsigned long cwidth, unsigned long cheight, unsigned char bpp);
		void Rasterize(unsigned char *dst, unsigned char *src, unsigned long cwidth, unsigned long cheight, unsigned long scanlinewidth, void *hdrdata, unsigned long yofs = 0);
	public:
		ERESULT Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags);
		ERESULT DecodeImage(E2DSurfaceBase *surface, ELDR_2DCALLBACKINFO *cbinfo, void *hdrdata);
		void WriteHeader(ClassEFile *ef, unsigned long width, unsigned long height, unsigned char coldepth);
		void WritePixelData(ClassEFile *ef, class E2DSurface *surface, RECT *rc, unsigned char coldepth, unsigned long *paldata);
		void WritePixelData(ClassEFile *ef, class E2DSurfaceRGBA *surface, RECT *rc);
};//Loading of PNG (Portable Network Graphics) files

class ELoader_JPG : public ClassELoaderBase
{
	public:
		ERESULT Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags);
};//Loading of JPEG (Joint Photographic Experts Group) files

class ELoader_TGA : public ClassELoaderBase
{
	public:
		ERESULT Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags);
};//Loading of uncompressed TGA (Targa) files

class ELoader_3DS : public ClassELoaderBase
{
	private:
		void LoadObjectData(void *objarray, void *matarray);
		void LoadObjectInfo(void *objinfo, void *matarray);
		void LoadObjectMeshInfo(void *objinfo, void *matarray);
	public:
		ERESULT Start(E3DLoaderBase *surface, void *ldata, unsigned long lsize, signed char modelsize, unsigned long createflags);
};//Loading of 3DS (3D studio MAX) files

class ELoader_MS3D : public ClassELoaderBase
{
	public:
		short ConvShort(unsigned char *ptr);
		long ConvLong(unsigned char *ptr);
		ERESULT Start(E3DLoaderBase *surface, void *ldata, unsigned long lsize, signed char modelsize, unsigned long createflags);
};//Loading of MS3D (MilkShape 3D) files

class ELoader_E3D : public ClassELoaderBase
{
	public:
		ERESULT Start(E3DLoaderBase *surface, void *ldata, unsigned long lsize, signed char modelsize, unsigned long createflags);
};//Loading of E3D (EDGELIB 3D) files

class ELoader_DAE : public ClassELoaderBase
{
	private:
		void *workerdata;
	public:
		ERESULT Start(E3DLoaderBase *surface, void *ldata, unsigned long lsize, signed char modelsize, unsigned long createflags);
		bool XmlCallback(void *parser, unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value);
};//Loading of DAE (COLLADA) files

class ELoader_PVR : public ClassELoaderBase
{
	public:
		ERESULT Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags);
};//Loading of compressed PVR (PVRTC) textures

#endif
#endif
