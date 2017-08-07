// esurf_3d.cpp
// 3D Surface
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eloader.h"
#include "emath.h"
#include "ememory.h"
#include "efile.h"
#include "edisplay.h"
#include "econsole.h"

#include "extra/verinfo.h"

#if defined(DEVICE_DESKTOP)
	#include <math.h>
#endif

///////////////////////////////////////////////////////////////////
// Duplicate list functionality
///////////////////////////////////////////////////////////////////

#define DUPLIST_FRAGMENT 32000

#define DPLF_NORMAL          1
#define DPLF_TEXCOORD        2
#define DPLF_COLOR           4

typedef struct
{
	long forwardlistindex;
	unsigned long triangleid;
	unsigned long vertexid;
	signed char normal[3];
	long texcoord[2];
	unsigned long color;
	unsigned char mask;
}DUPLIST_ITEM;

//Add new duplicate
void E3DDupl_AddNew(void *&list, unsigned long &count, unsigned long triangleid, unsigned long vertexid, unsigned long vertexcount, signed char x, signed char y, signed char z, long u, long v, unsigned long color, unsigned char checkmask)
{
	long toforward = -1;
	unsigned long ctr;
	if (count % DUPLIST_FRAGMENT == DUPLIST_FRAGMENT - 1)
	{
		unsigned long newcount = (count / DUPLIST_FRAGMENT + 2) * DUPLIST_FRAGMENT;
		void *newduplist = ClassEMemory::ReAlloc(list, vertexcount * sizeof(long) + newcount * sizeof(DUPLIST_ITEM));
		if (newduplist == NULL)
			return;
		list = newduplist;
	}
	DUPLIST_ITEM *dupv = (DUPLIST_ITEM *)((unsigned char *)(list) + vertexcount * sizeof(long));

	//Easy check, more complicated with more material (colors and textures)
	if (checkmask & DPLF_NORMAL)
		for (ctr = 0; ctr < count; ctr++)
			if (dupv[ctr].vertexid == vertexid)
				if (dupv[ctr].normal[0] == x && dupv[ctr].normal[1] == y && dupv[ctr].normal[2] == z)
				{
					toforward = ctr;
					break;
				}

	dupv[count].forwardlistindex = toforward;
	dupv[count].triangleid = triangleid;
	if (toforward == -1)
	{
		dupv[count].vertexid = vertexid;
		if (checkmask & DPLF_NORMAL)
		{
			dupv[count].normal[0] = x;
			dupv[count].normal[1] = y;
			dupv[count].normal[2] = z;
		}
		if (checkmask & DPLF_TEXCOORD)
		{
			dupv[count].texcoord[0] = u;
			dupv[count].texcoord[1] = v;
		}
		if (checkmask & DPLF_COLOR)
			dupv[count].color = color;
		dupv[count].mask |= checkmask;
	}
	count++;
}

///////////////////////////////////////////////////////////////////
// E3DLoaderBase: parent class for handling loader callbacks
///////////////////////////////////////////////////////////////////

E3DLoaderBase::E3DLoaderBase(void){ }
E3DLoaderBase::~E3DLoaderBase(void){ }
ERESULT E3DLoaderBase::LdrOnMeshStart(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnMeshEnd(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnObjectStart(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnObjectEnd(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnVertexStream(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnNormalStream(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnTexcoordStream(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnColorStream(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnIndexStream(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnLightStream(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnCameraStream(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnJointStream(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
ERESULT E3DLoaderBase::LdrOnKeyFrameStream(ELDR_3DCALLBACKINFO *ldrdata){ return(E_OK); }
void E3DLoaderBase::CreateJoints(unsigned long count){ }
void E3DLoaderBase::SetJointProperties(unsigned long index, EVECTOR3D *location, EVECTOR3D *rotation, long parent){ }
void E3DLoaderBase::CreateJointKeyFrames(unsigned long index, unsigned long numtranslate, unsigned long numrotate){ }

///////////////////////////////////////////////////////////////////
// E3DSurface: pure (inline) functions                           //
///////////////////////////////////////////////////////////////////

//Calculate the dot product between 2 vectors
long inline E3DSurface_DotProduct(long v1x, long v1y, long v1z, long v2x, long v2y, long v2z)
{
	return(v1x * v2x + v1y * v2y + v1z * v2z);
}

//Calculate the cross product between 2 vectors
void inline E3DSurface_CrossProduct(long &dx, long &dy, long &dz, long v1x, long v1y, long v1z, long v2x, long v2y, long v2z)
{
	dx = v1y * v2z - v2y * v1z;
	dy = v1z * v2x - v2z * v1x;
	dz = v1x * v2y - v2x * v1y;
}

//Normalize a vector into a unit length
void inline E3DSurface_Normalize(long &vx, long &vy, long &vz, long prec = 0)
{
	long magnitude;
	if (prec)
	{
		#if defined(DEVICE_DESKTOP)
			magnitude = (long)sqrt((double)((vx >> prec) * (vx >> prec) + (vy >> prec) * (vy >> prec) + (vz >> prec) * (vz >> prec)));
		#else
			magnitude = ClassEMath::IntSqrt((vx >> prec) * (vx >> prec) + (vy >> prec) * (vy >> prec) + (vz >> prec) * (vz >> prec));
		#endif
	}
	else
	{
		#if defined(DEVICE_DESKTOP)
			magnitude = (long)sqrt((double)(vx * vx + vy * vy + vz * vz));
		#else
			magnitude = ClassEMath::IntSqrt(vx * vx + vy * vy + vz * vz);
		#endif
	}
	if (magnitude > 0)
	{
		vx = (vx << (14 - prec)) / magnitude;
		vy = (vy << (14 - prec)) / magnitude;
		vz = (vz << (14 - prec)) / magnitude;
		return;
	}
	vx = 0;
	vy = 0;
	vz = 0;
}

//2D perspective correction (old function, flat z)
void inline E3DSurface_PerspectiveOld(long &dx, long &dy, long vx, long vy, long vz, long rdrsize)
{
	long scale, vpoint1 = (E3DFOCAL << 12), vpoint2 = (E3DFOCAL << 4);
	scale = -vz;
	if (scale == -vpoint2)
		scale = -vpoint2 + 1;
	scale = (vpoint1 / ((vpoint2) + scale)) * rdrsize;
	dx = ((vx * scale) >> 1);
	dy = ((vy * scale) >> 1);
}

//2D perspective correction using matrix (fast optimized version)
void E3DSurface_PerspectiveMtxFast(long *matrix, long &dx, long &dy, long &dz, long vx, long vy, long vz, long dispw, long disph)
{
	long w = ((vx * matrix[3] + vy * matrix[7] + vz * matrix[11]) >> 12) + matrix[15];
	long x = vx * matrix[0];
	long ox = x / w;
	long y  = vy * matrix[5];
	long oy = y / w;
	//long z = vx * matrix[2] + vy * matrix[6] + vz * matrix[10] + matrix[14];
	//long oz = z / w;
	//long ow = w;
	dx = ox * dispw * 128;
	dy = oy * disph * 128;
	dz = w;
}

//2D perspective correction using matrix
void E3DSurface_PerspectiveMtxFull(long *matrix, long &dx, long &dy, long &dz, long vx, long vy, long vz, long dispw, long disph)
{
	long x = ((vx * matrix[0] + vy * matrix[4] + vz * matrix[ 8]) >> 12) + matrix[12];
	long y = ((vx * matrix[1] + vy * matrix[5] + vz * matrix[ 9]) >> 12) + matrix[13];
	//long z = ((vx * matrix[2] + vy * matrix[6] + vz * matrix[10]) >> 12) + matrix[14];
	long w = ((vx * matrix[3] + vy * matrix[7] + vz * matrix[11]) >> 12) + matrix[15];
	long ox = (x << 12) / w;
	long oy = (y << 12) / w;
	//long oz = (z << 12) / w;
	//long ow = w;
	dx = ox * dispw * 128;
	dy = oy * disph * 128;
	dz = w;
}

//Return whether the polygon faces backwards (this will not be drawn)
bool inline E3DSurface_CullFace(long s1x, long s1y, long s2x, long s2y, long s3x, long s3y)
{
	if (((s1x - s2x) >> 16) * ((s3y - s2y) >> 16) - ((s1y - s2y) >> 16) * ((s3x - s2x) >> 16) < 0)
		return(false);
	return(true);
}

//Compare 2 polygons
int inline E3DSurface_ComparePolygon(int z1, int z2)
{
	return(z1 - z2);
}

//Polygon sort helper function
void inline E3DSurface_SiftPolygon(int start, int count, short *sortarray)
{
	short tmp;
	int root = start, child;
	while ((root << 1) < count)
	{
		child = (root << 1);
		if (child < count - 1 && E3DSurface_ComparePolygon(sortarray[child * 2 + 1], sortarray[(child + 1) * 2 + 1]) < 0)
			child++;
		if (E3DSurface_ComparePolygon(sortarray[root * 2 + 1], sortarray[child * 2 + 1]) < 0)
		{
			tmp = sortarray[root * 2];
			sortarray[root * 2] = sortarray[child * 2];
			sortarray[child * 2] = tmp;
			tmp = sortarray[root * 2 + 1];
			sortarray[root * 2 + 1] = sortarray[child * 2 + 1];
			sortarray[child * 2 + 1] = tmp;
			root = child;
		}
		else
			return;
	}
}

//Sort the polygon list by z-order
void inline E3DSurface_SortPolygons(unsigned long count, short *sortarray)
{
	short tmp;
	int start = count / 2 - 1, end = count - 1;
	while (start >= 0)
	{
		E3DSurface_SiftPolygon(start, count, sortarray);
		start--;
	}
	while (end >= 0)
	{
		tmp = sortarray[end * 2];
		sortarray[end * 2] = sortarray[0 * 2];
		sortarray[0 * 2] = tmp;
		tmp = sortarray[end * 2 + 1];
		sortarray[end * 2 + 1] = sortarray[0 * 2 + 1];
		sortarray[0 * 2 + 1] = tmp;
		E3DSurface_SiftPolygon(0, end, sortarray);
		end--;
	}
}

//Calculate lighting
COLORREF inline E3DSurface_Lighting(long nx, long ny, long nz, COLORREF vcol, long *lightdata, unsigned char numlights)
{
	if (numlights == 0)
		return(vcol);
	unsigned char matr = (unsigned char)E_GETR(vcol), matg = (unsigned char)E_GETG(vcol), matb = (unsigned char)E_GETB(vcol), ctr;
	unsigned long r = matr * 13107, g = matg * 13107, b = matb * 13107;
	long angle;
	for (ctr = 0; ctr < numlights; ctr++)
	{
		r += E_GETR(lightdata[3]) * matr * 256;
		g += E_GETG(lightdata[3]) * matg * 256;
		b += E_GETB(lightdata[3]) * matb * 256;
		angle = E3DSurface_DotProduct(nx, ny, nz, lightdata[0], lightdata[1], -lightdata[2]) >> 7;
		if (angle < 0)
		{
			lightdata += 5;
			continue;
		}
		else if (angle > 65535)
			angle = 65535;
		r += E_GETR(lightdata[4]) * angle / 256 * matr;
		g += E_GETG(lightdata[4]) * angle / 256 * matg;
		b += E_GETB(lightdata[4]) * angle / 256 * matb;
		lightdata += 5;
	}
	if (r > 0xFFFFFF)
		r = 0xFFFFFF;
	if (g > 0xFFFFFF)
		g = 0xFFFFFF;
	if (b > 0xFFFFFF)
		b = 0xFFFFFF;
	return(E_RGBX((r >> 16), (g >> 16), (b >> 16)));
}

//Calculate polygon and the surrounding vertex normals
void inline E3DSurface_PolygonNormals(unsigned char *coordlist, unsigned short *indexlist, unsigned long lofs_vertex, unsigned long lofs_normal, unsigned long lstr_vertex, unsigned long lstr_normal, unsigned long numvertex, unsigned long numpoly, bool usegl)
{
	if (lofs_normal == 0xFFFFFFFF)
		return;
	long *glvertexarray = (long *)&coordlist[lofs_vertex];
	signed char *glnormalarray = (signed char *)&coordlist[lofs_normal];
	unsigned long cnt, vctr, pctr, vtxstride = lstr_vertex / sizeof(long);
	unsigned long bitarr[] = { 0x40000000, 0x20000000, 0x10000000, 0x8000000, 0x4000000, 0x2000000, 0x1000000, 0x800000, 0x400000, 0x200000, 0x100000, 0x80000, 0x40000, 0x20000, 0x10000, 0x8000, 0x4000 };
	for (vctr = 0; vctr < numvertex; vctr++)
	{
		long normal[3] = { 0, 0, 0 };
		cnt = 0;
		for (pctr = 0; pctr < numpoly * 3; pctr += 3)
			if (indexlist[pctr] == vctr || indexlist[pctr + 1] == vctr || indexlist[pctr + 2] == vctr)
			{
				long tx, ty, tz;
				E3DSurface_CrossProduct(tx, ty, tz, glvertexarray[indexlist[pctr] * vtxstride] - glvertexarray[indexlist[pctr + 2] * vtxstride], glvertexarray[indexlist[pctr] * vtxstride + 1] - glvertexarray[indexlist[pctr + 2] * vtxstride + 1], glvertexarray[indexlist[pctr] * vtxstride + 2] - glvertexarray[indexlist[pctr + 2] * vtxstride + 2], glvertexarray[indexlist[pctr + 2] * vtxstride] - glvertexarray[indexlist[pctr + 1] * vtxstride], glvertexarray[indexlist[pctr + 2] * vtxstride + 1] - glvertexarray[indexlist[pctr + 1] * vtxstride + 1], glvertexarray[indexlist[pctr + 2] * vtxstride + 2] - glvertexarray[indexlist[pctr + 1] * vtxstride + 2]);
				normal[0] += tx;
				normal[1] += ty;
				normal[2] += tz;
				cnt++;
			}
		if (cnt)
		{
			normal[0] /= (long)cnt;
			normal[1] /= (long)cnt;
			normal[2] /= (long)cnt;
			cnt = 0;
			for (pctr = 0; pctr < sizeof(bitarr) / sizeof(unsigned long); pctr++)
			{
				if (ClassEMath::Abs(normal[0]) & bitarr[pctr])
				{
					cnt = sizeof(bitarr) / sizeof(unsigned long) - pctr;
					break;
				}
				else if (ClassEMath::Abs(normal[1]) & bitarr[pctr])
				{
					cnt = sizeof(bitarr) / sizeof(unsigned long) - pctr;
					break;
				}
				else if (ClassEMath::Abs(normal[2]) & bitarr[pctr])
				{
					cnt = sizeof(bitarr) / sizeof(unsigned long) - pctr;
					break;
				}
			}
			E3DSurface_Normalize(normal[0], normal[1], normal[2], cnt);
			if (usegl)
			{
				glnormalarray[0] = (signed char)((-normal[0] * 127) >> 14);
				glnormalarray[1] = (signed char)((-normal[1] * 127) >> 14);
				glnormalarray[2] = (signed char)((-normal[2] * 127) >> 14);
			}
			else
			{
				glnormalarray[0] = (signed char)((-normal[0] * 88) >> 14);
				glnormalarray[1] = (signed char)((-normal[1] * 88) >> 14);
				glnormalarray[2] = (signed char)((-normal[2] * 88) >> 14);
			}
		}
		glnormalarray += lstr_normal;
	}
}

//Transform vertex
void inline E3DSurface_MatrixTransform(long &dstx, long &dsty, long &dstz, long srcx, long srcy, long srcz, long *matrix)
{
	dstx = ((srcx * matrix[0]) >> 12) + ((srcy * matrix[4]) >> 12) + ((srcz * matrix[8]) >> 12) + matrix[12];
	dsty = ((srcx * matrix[1]) >> 12) + ((srcy * matrix[5]) >> 12) + ((srcz * matrix[9]) >> 12) + matrix[13];
	dstz = ((srcx * matrix[2]) >> 12) + ((srcy * matrix[6]) >> 12) + ((srcz * matrix[10]) >> 12) + matrix[14];
}

//Transform vertex
void inline E3DSurface_MatrixRotate(long &dstx, long &dsty, long &dstz, long srcx, long srcy, long srcz, long *matrix)
{
	dstx = ((srcx * matrix[0]) >> 12) + ((srcy * matrix[4]) >> 12) + ((srcz * matrix[8]) >> 12);
	dsty = ((srcx * matrix[1]) >> 12) + ((srcy * matrix[5]) >> 12) + ((srcz * matrix[9]) >> 12);
	dstz = ((srcx * matrix[2]) >> 12) + ((srcy * matrix[6]) >> 12) + ((srcz * matrix[10]) >> 12);
}

//Apply translation to the matrix
void E3DSurface_UpdateMatrixTranslate(long x, long y, long z, long *matrix)
{
	long translatematrix[] = {
		4096, 0, 0, 0,
		0, 4096, 0, 0,
		0, 0, 4096, 0,
		x, y, z, 4096
	};
	ClassEMath::MatrixMultiply(matrix, translatematrix);
}

//Exported functions
void E3DSurface_Normalize_exp(long &vx, long &vy, long &vz, long prec){ E3DSurface_Normalize(vx, vy, vz, prec); }
void E3DSurface_Perspective_exp(long *matrix, long &dx, long &dy, long vx, long vy, long vz, long dispwidth, long dispheight)
{
	long dummy;
	E3DSurface_PerspectiveMtxFull(matrix, dx, dy, dummy, vx, vy, vz, dispwidth, dispheight);
}

///////////////////////////////////////////////////////////////////
// E3DSurface: private                                           //
///////////////////////////////////////////////////////////////////

//Link the display module to the surface
void E3DSurface::LinkECD(ECD *pecd){ ecd = pecd; }

//Create a new 3D surface containing a number of points and polygons
ERESULT E3DSurface::Create(unsigned long vertices, unsigned long polygons, unsigned long usetype)
{
	#if (EDITION_SDK < 1000)
		return(E_NOFEATURE);
	#endif
	if (allocpoly)
		Free();
	if (vertices == 0 || polygons == 0)
		return(E_INVALIDPARAM);
	unsigned long curidx = 0;
	unsigned char unitsize = sizeof(long);
	bool interleaved = !(usetype & EST_NOINTERLEAVE);
	coordlistsize = vertices * 3 * sizeof(long);
	if (!(usetype & EST_NONORMALARRAY))
		coordlistsize += vertices * 4 * sizeof(signed char);
	if (!(usetype & EST_NOCOLORARRAY))
		coordlistsize += vertices * 4 * sizeof(unsigned char);
	if (!(usetype & EST_NOTEXCOORDARRAY))
		coordlistsize += vertices * 2 * unitsize;
	coordlist = (unsigned char *)ecd->mem->AllocList(coordlistsize);
	if (coordlist == NULL)
	{
		Free();
		return(E_ERROR);
	}
	indexlist = (unsigned short *)ecd->mem->AllocList(polygons * 3 * sizeof(unsigned short));
	if (ecd->dsp->gldriver == NULL)
	{
		sortindexlist = (short *)ecd->mem->AllocList(polygons * 2 * sizeof(unsigned short));
		drawlist = (unsigned char *)ecd->mem->AllocList(polygons * 3 * 5 * sizeof(long));
	}
	if (indexlist == NULL || (ecd->dsp->gldriver == NULL && (sortindexlist == NULL || drawlist == NULL)))
	{
		Free();
		return(E_ERROR);
	}
	ClassEMemory::Set(coordlist, 0, coordlistsize);
	lofs_vertex = 0xFFFFFFFF;
	lofs_normal = 0xFFFFFFFF;
	lofs_color = 0xFFFFFFFF;
	lofs_texel = 0xFFFFFFFF;
	if (interleaved)
	{
		lofs_vertex = 0;
		lstr_vertex = coordlistsize / vertices;
		curidx += 3 * sizeof(long);
		if (!(usetype & EST_NONORMALARRAY))
		{
			lofs_normal = curidx;
			lstr_normal = coordlistsize / vertices;
			curidx += 4 * sizeof(signed char);
		}
		if (!(usetype & EST_NOCOLORARRAY))
		{
			lofs_color = curidx;
			lstr_color = coordlistsize / vertices;
			curidx += 4 * sizeof(unsigned char);
		}
		if (!(usetype & EST_NOTEXCOORDARRAY))
		{
			lofs_texel = curidx;
			lstr_texel = coordlistsize / vertices;
			curidx += 2 * unitsize;
		}
	}
	else
	{
		lofs_vertex = curidx;
		lstr_vertex = sizeof(long) * 3;
		curidx += vertices * lstr_vertex;
		if (!(usetype & EST_NONORMALARRAY))
		{
			lofs_normal = curidx;
			lstr_normal = sizeof(signed char) * 4;
			curidx += vertices * lstr_normal;
		}
		if (!(usetype & EST_NOCOLORARRAY))
		{
			lofs_color = curidx;
			lstr_color = sizeof(unsigned char) * 4;
			curidx += vertices * lstr_color;
		}
		if (!(usetype & EST_NOTEXCOORDARRAY))
		{
			lofs_texel = curidx;
			lstr_texel = unitsize * 2;
			curidx += vertices * lstr_texel;
		}
	}
	if (lofs_color != 0xFFFFFFFF)
	{
		unsigned char *colarr = (unsigned char *)&coordlist[lofs_color];
		for (curidx = 0; curidx < vertices; curidx++)
		{
			colarr[0] = 255;
			colarr[1] = 255;
			colarr[2] = 255;
			colarr[3] = 255;
			colarr += lstr_color;
		}
	}
	for (curidx = 0; curidx < 16; curidx++)
	{
		matrix[curidx] = 0;
		if ((curidx >> 2) == (curidx & 3))
			matrix[curidx] = 4096;
	}
	ClassEMemory::Set(indexlist, 0, polygons * 3 * sizeof(unsigned short));
	allocvertex = vertices;
	allocpoly = polygons;
	numvertex = 0;
	numpoly = 0;
	modelchanged = false;
	wmatrix = NULL;
	if (!ecd->dsp->HandleObj3DList(this))
	{
		Free();
		return(E_NOMEMORY);
	}
	return(E_OK);
}

//Create a new surface by loading a model from memory
ERESULT E3DSurface::LoadModel(void *mdlmem, unsigned long memsize, unsigned char passmode, signed char modelsize, unsigned long usetype)
{
	ERESULT er = E_UNSUPPORTED;
	unsigned char ctr;
	for (ctr = 0; ctr < MAX_GRAPHICSLOADERS; ctr++)
		if (ecd->dsp->graphicsloader[ctr] != NULL)
		{
			er = ecd->dsp->graphicsloader[ctr]->Start(this, (unsigned char *)mdlmem, memsize, modelsize, usetype);
			if (er != E_UNSUPPORTED && er != E_NOFEATURE)
				break;
			if (er == E_OK)
				break;
		}
	return(er);
}

//Create a new surface by loading a model
ERESULT E3DSurface::LoadModel(WCHAR *mdlfile, unsigned char passmode, signed char modelsize, unsigned long usetype)
{
	ClassEFile iml;
	ERESULT er;
	unsigned char *fdata = NULL;
	unsigned long fsize;
	unsigned char opentype = EFOF_READ | EFOF_NOEPKCHECK;
	if (usetype & EST_READONLYPATH)
		opentype |= EFOF_READONLYPATH;
	if (!iml.Open(mdlfile, opentype))
		return(E_NOTEXISTS);
	fsize = iml.Size();
	if (fsize == 0)
	{
		iml.Close();
		return(E_NODATA);
	}
	fdata = (unsigned char *)ecd->mem->AllocList(fsize);
	if (fdata == NULL)
	{
		iml.Close();
		return(E_NOMEMORY);
	}
	iml.Read(fdata, fsize);
	iml.Close();
	er = LoadModel(fdata, fsize, passmode, modelsize, usetype);
	FULL_FREE_L(fdata);
	return(er);
}

#if defined(DEVICE_WINMOBILE) && (DEVICE_WINMOBILE <= 200) //Embedded Visual tools 3 (Windows Mobile 2002) can't handle below function with optimization
	#pragma optimize("", off)
#endif

//Prepare the software 3D rasterizer
unsigned long E3DSurface::PrepareRender(bool lighting, bool coloring, bool smoothcolors, bool texels, bool cull)
{
	void (*perspective3d_func)(long *matrix, long &dx, long &dy, long &dz, long vx, long vy, long vz, long dispw, long disph);
	perspective3d_func = E3DSurface_PerspectiveMtxFull;
	if (ecd->dsp->mtxperspective[1] == 0 && ecd->dsp->mtxperspective[2] == 0 && ecd->dsp->mtxperspective[3] == 0)
		if (ecd->dsp->mtxperspective[4] == 0 && ecd->dsp->mtxperspective[6] == 0 && ecd->dsp->mtxperspective[7] == 0)
			if (ecd->dsp->mtxperspective[8] == 0 && ecd->dsp->mtxperspective[9] == 0 && ClassEMath::Abs(ecd->dsp->mtxperspective[10]) == 4096 && ClassEMath::Abs(ecd->dsp->mtxperspective[11]) == 4096)
				if (ecd->dsp->mtxperspective[12] == 0 && ecd->dsp->mtxperspective[13] == 0 && ecd->dsp->mtxperspective[15] == 0)
					perspective3d_func = E3DSurface_PerspectiveMtxFast;
	unsigned char *checklist = coordlist;
	if (coordlist_transformed && useanimation)
		checklist = coordlist_transformed;
	long *gldrawarray = (long *)drawlist;
	long *glvertexarray = (long *)&checklist[lofs_vertex];
	long *gltexelarray = (long *)&checklist[lofs_texel];
	long *cmatrix = matrix, *pmatrix = ecd->dsp->mtxperspective;
	signed char *glnormalarray = (signed char *)&checklist[lofs_normal];
	unsigned char *glcolorarray = (unsigned char *)&checklist[lofs_color];
	unsigned short *glindexarray = &indexlist[-3];
	unsigned long ctr, texwidth = 0, texheight = 0, drawstride = 9, useidx = 0;
	unsigned long dispwidth = ecd->dsp->GetWidth(), dispheight = ecd->dsp->GetHeight();
	unsigned long vtxstride = lstr_vertex / sizeof(long);
	unsigned long nrmstride = lstr_normal / sizeof(signed char);
	unsigned long colstride = lstr_color / sizeof(unsigned char);
	unsigned long texstride = lstr_texel / sizeof(long);
	unsigned char lightcount = 0, pflags = 0;
	long tmpx, tmpy, tmpz, sortz;
	long lightdata[E3D_MAXLIGHTS * 5];
	long nmatrix[16];
	if (wmatrix)
	{
		ClassEMemory::Copy(nmatrix, wmatrix, 64);
		ClassEMath::MatrixMultiply(nmatrix, matrix);
		cmatrix = nmatrix;
	}
	if (lighting)
	{
		if (ecd->dsp->lightdata != NULL)
		{
			for (ctr = 0; ctr < E3D_MAXLIGHTS; ctr++)
				if (ecd->dsp->lightdata[ctr].enabled)
				{
					lightdata[lightcount * 5] = ecd->dsp->lightdata[ctr].position[0];
					lightdata[lightcount * 5 + 1] = ecd->dsp->lightdata[ctr].position[1];
					lightdata[lightcount * 5 + 2] = ecd->dsp->lightdata[ctr].position[2];
					lightdata[lightcount * 5 + 3] = ecd->dsp->lightdata[ctr].intensity[0];
					lightdata[lightcount * 5 + 4] = ecd->dsp->lightdata[ctr].intensity[1];
					lightcount++;
				}
		}
		if (lightcount == 0)
			lighting = false;
	}
	if (cull)
		pflags |= 1;
	if (coloring)
	{
		pflags |= 2;
		drawstride += 1;
		if (smoothcolors)
		{
			drawstride += 2;
			pflags |= 4;
		}
		if (lighting)
			pflags |= 8;
		if (lofs_color != 0xFFFFFFFF)
			pflags |= 16;
	}
	if (texels)
	{
		pflags |= 32;
		drawstride += 6;
		if (ecd->dsp->usetex != NULL)
		{
			texwidth = ecd->dsp->usetex->GetWidth();
			texheight = ecd->dsp->usetex->GetHeight();
			#if !defined(DEVICE_DESKTOP)
				if ((texwidth == 0x40 || texwidth == 0x80 || texwidth == 0x100 || texwidth == 0x200 || texwidth == 0x400) && (texheight == 0x40 || texheight == 0x80 || texheight == 0x100 || texheight == 0x200 || texheight == 0x400))
				{
					if (texwidth == 0x40)
						texwidth = 6;
					else if (texwidth == 0x80)
						texwidth = 7;
					else if (texwidth == 0x100)
						texwidth = 8;
					else if (texwidth == 0x200)
						texwidth = 9;
					else if (texwidth == 0x400)
						texwidth = 10;
					if (texheight == 0x40)
						texheight = 6;
					else if (texheight == 0x80)
						texheight = 7;
					else if (texheight == 0x100)
						texheight = 8;
					else if (texheight == 0x200)
						texheight = 9;
					else if (texheight == 0x400)
						texheight = 10;
					pflags |= 64;
				}
			#endif
			if (texwidth == 0 || texheight == 0)
				return(0);
		}
		else
			return(0);
	}
	for (ctr = 0; ctr < numpoly; ctr++)
	{
		//bool zpositive = false;
		glindexarray += 3;
		sortz = 0;
		E3DSurface_MatrixTransform(tmpx, tmpy, tmpz, glvertexarray[glindexarray[0] * vtxstride], glvertexarray[glindexarray[0] * vtxstride + 1], glvertexarray[glindexarray[0] * vtxstride + 2], cmatrix);
		//if (tmpz <= 0)
		//	zpositive = true;
		perspective3d_func(pmatrix, gldrawarray[0], gldrawarray[1], gldrawarray[2], tmpx, tmpy, tmpz, dispwidth, dispheight);
		sortz += tmpz;
		E3DSurface_MatrixTransform(tmpx, tmpy, tmpz, glvertexarray[glindexarray[1] * vtxstride], glvertexarray[glindexarray[1] * vtxstride + 1], glvertexarray[glindexarray[1] * vtxstride + 2], cmatrix);
		//if (tmpz <= 0)
		//	zpositive = true;
		perspective3d_func(pmatrix, gldrawarray[3], gldrawarray[4], gldrawarray[5], tmpx, tmpy, tmpz, dispwidth, dispheight);
		sortz += tmpz;
		E3DSurface_MatrixTransform(tmpx, tmpy, tmpz, glvertexarray[glindexarray[2] * vtxstride], glvertexarray[glindexarray[2] * vtxstride + 1], glvertexarray[glindexarray[2] * vtxstride + 2], cmatrix);
		//if (tmpz <= 0)
		//	zpositive = true;
		perspective3d_func(pmatrix, gldrawarray[6], gldrawarray[7], gldrawarray[8], tmpx, tmpy, tmpz, dispwidth, dispheight);
		sortz += (sortz + tmpz) / 4;
		//if (!zpositive)
		//	continue;
		if (pflags & 1)
			if (E3DSurface_CullFace(gldrawarray[0], gldrawarray[1], gldrawarray[3], gldrawarray[4], gldrawarray[6], gldrawarray[7]))
				continue;
		if (pflags & 2)
		{
			if (pflags & 8)
			{
				if (pflags & 16)
				{
					if (pflags & 4)
					{
						E3DSurface_MatrixRotate(tmpx, tmpy, tmpz, glnormalarray[glindexarray[0] * nrmstride], glnormalarray[glindexarray[0] * nrmstride + 1], glnormalarray[glindexarray[0] * nrmstride + 2], cmatrix);
						gldrawarray[9] = E3DSurface_Lighting(tmpx, tmpy, tmpz, E_RGBX(glcolorarray[glindexarray[0] * colstride], glcolorarray[glindexarray[0] * colstride + 1], glcolorarray[glindexarray[0] * colstride + 2]), lightdata, lightcount);
						E3DSurface_MatrixRotate(tmpx, tmpy, tmpz, glnormalarray[glindexarray[1] * nrmstride], glnormalarray[glindexarray[1] * nrmstride + 1], glnormalarray[glindexarray[1] * nrmstride + 2], cmatrix);
						gldrawarray[10] = E3DSurface_Lighting(tmpx, tmpy, tmpz, E_RGBX(glcolorarray[glindexarray[1] * colstride], glcolorarray[glindexarray[1] * colstride + 1], glcolorarray[glindexarray[1] * colstride + 2]), lightdata, lightcount);
						E3DSurface_MatrixRotate(tmpx, tmpy, tmpz, glnormalarray[glindexarray[2] * nrmstride], glnormalarray[glindexarray[2] * nrmstride + 1], glnormalarray[glindexarray[2] * nrmstride + 2], cmatrix);
						gldrawarray[11] = E3DSurface_Lighting(tmpx, tmpy, tmpz, E_RGBX(glcolorarray[glindexarray[2] * colstride], glcolorarray[glindexarray[2] * colstride + 1], glcolorarray[glindexarray[2] * colstride + 2]), lightdata, lightcount);
					}
					else
					{
						E3DSurface_MatrixRotate(tmpx, tmpy, tmpz, glnormalarray[glindexarray[2] * nrmstride], glnormalarray[glindexarray[2] * nrmstride + 1], glnormalarray[glindexarray[2] * nrmstride + 2], cmatrix);
						gldrawarray[9] = E3DSurface_Lighting(tmpx, tmpy, tmpz, E_RGBX(glcolorarray[glindexarray[2] * colstride], glcolorarray[glindexarray[2] * colstride + 1], glcolorarray[glindexarray[2] * colstride + 2]), lightdata, lightcount);
					}
				}
				else
				{
					E3DSurface_MatrixRotate(tmpx, tmpy, tmpz, glnormalarray[glindexarray[0] * nrmstride], glnormalarray[glindexarray[0] * nrmstride + 1], glnormalarray[glindexarray[0] * nrmstride + 2], cmatrix);
					gldrawarray[9] = E3DSurface_Lighting(tmpx, tmpy, tmpz, ECOLOR_WHITE, lightdata, lightcount);
					if (pflags & 4)
					{
						E3DSurface_MatrixRotate(tmpx, tmpy, tmpz, glnormalarray[glindexarray[1] * nrmstride], glnormalarray[glindexarray[1] * nrmstride + 1], glnormalarray[glindexarray[1] * nrmstride + 2], cmatrix);
						gldrawarray[10] = E3DSurface_Lighting(tmpx, tmpy, tmpz, ECOLOR_WHITE, lightdata, lightcount);
						E3DSurface_MatrixRotate(tmpx, tmpy, tmpz, glnormalarray[glindexarray[2] * nrmstride], glnormalarray[glindexarray[2] * nrmstride + 1], glnormalarray[glindexarray[2] * nrmstride + 2], cmatrix);
						gldrawarray[11] = E3DSurface_Lighting(tmpx, tmpy, tmpz, ECOLOR_WHITE, lightdata, lightcount);
					}
				}
			}
			else
			{
				if (pflags & 16)
				{
					if (pflags & 4)
					{
						gldrawarray[9] = E_RGBX(glcolorarray[glindexarray[0] * colstride], glcolorarray[glindexarray[0] * colstride + 1], glcolorarray[glindexarray[0] * colstride + 2]);
						gldrawarray[10] = E_RGBX(glcolorarray[glindexarray[1] * colstride], glcolorarray[glindexarray[1] * colstride + 1], glcolorarray[glindexarray[1] * colstride + 2]);
						gldrawarray[11] = E_RGBX(glcolorarray[glindexarray[2] * colstride], glcolorarray[glindexarray[2] * colstride + 1], glcolorarray[glindexarray[2] * colstride + 2]);
					}
					else
						gldrawarray[9] = E_RGBX(glcolorarray[glindexarray[2] * colstride], glcolorarray[glindexarray[2] * colstride + 1], glcolorarray[glindexarray[2] * colstride + 2]);
				}
				else
				{
					gldrawarray[9] = ECOLOR_WHITE;
					if (pflags & 4)
					{
						gldrawarray[10] = ECOLOR_WHITE;
						gldrawarray[11] = ECOLOR_WHITE;
					}
				}
			}
		}
		if (pflags & 32)
		{
			if (pflags & 64)
			{
				gldrawarray[9]  = (gltexelarray[glindexarray[0] * texstride] << texwidth) >> 8;
				gldrawarray[10] = ((65536 - gltexelarray[glindexarray[0] * texstride + 1]) << texheight) >> 8;
				gldrawarray[11] = (gltexelarray[glindexarray[1] * texstride] << texwidth) >> 8;
				gldrawarray[12] = ((65536 - gltexelarray[glindexarray[1] * texstride + 1]) << texheight) >> 8;
				gldrawarray[13] = (gltexelarray[glindexarray[2] * texstride] << texwidth) >> 8;
				gldrawarray[14] = ((65536 - gltexelarray[glindexarray[2] * texstride + 1]) << texheight) >> 8;
			}
			else
			{
				gldrawarray[9]  = (gltexelarray[glindexarray[0] * texstride] * texwidth) >> 8;
				gldrawarray[10] = ((65536 - gltexelarray[glindexarray[0] * texstride + 1]) * texheight) >> 8;
				gldrawarray[11] = (gltexelarray[glindexarray[1] * texstride] * texwidth) >> 8;
				gldrawarray[12] = ((65536 - gltexelarray[glindexarray[1] * texstride + 1]) * texheight) >> 8;
				gldrawarray[13] = (gltexelarray[glindexarray[2] * texstride] * texwidth) >> 8;
				gldrawarray[14] = ((65536 - gltexelarray[glindexarray[2] * texstride + 1]) * texheight) >> 8;
			}
		}
		sortindexlist[useidx * 2] = (short)useidx;
		sortindexlist[useidx * 2 + 1] = (short)sortz;
		gldrawarray += drawstride;
		useidx++;
	}
	E3DSurface_SortPolygons(useidx, sortindexlist);
	return(useidx);
}

#if defined(DEVICE_WINMOBILE) && (DEVICE_WINMOBILE <= 200) //Fix compiler bug
	#pragma optimize("", on)
#endif

///////////////////////////////////////////////////////////////////
// E3DSurface: public                                            //
///////////////////////////////////////////////////////////////////

//E3DSurface: constructor
E3DSurface::E3DSurface(void)
{
	ecd = NULL;
	coordlist     = NULL;
	coordlist_transformed = NULL;
	indexlist     = NULL;
	sortindexlist = NULL;
	drawlist      = NULL;
	joint         = NULL;
	duplist       = NULL;
	allocvertex = 0;
	allocpoly = 0;
	locked = 0;
}

//E2DSurface: destructor
E3DSurface::~E3DSurface(void)
{
	Free();
}

//Lock this surface & return arrays
bool E3DSurface::Lock(E3DSURFACEINFO *surfaceinfo, bool render)
{
	if (surfaceinfo == NULL || allocpoly == 0)
		return(false);
	unsigned char *check_list = coordlist;
	if (render && useanimation && coordlist_transformed != NULL)
		check_list = coordlist_transformed;
	surfaceinfo->vertexarray   = NULL;
	surfaceinfo->normalarray   = NULL;
	surfaceinfo->colorarray    = NULL;
	surfaceinfo->texcoordarray = NULL;
	surfaceinfo->indexarray    = NULL;
	surfaceinfo->vertexstride   = 0;
	surfaceinfo->normalstride   = 0;
	surfaceinfo->colorstride    = 0;
	surfaceinfo->texcoordstride = 0;
	if (allocpoly)
	{
		if (lofs_vertex != 0xFFFFFFFF)
		{
			surfaceinfo->vertexarray = (long *)&check_list[lofs_vertex];
			surfaceinfo->vertexstride = lstr_vertex;
		}
		if (lofs_normal != 0xFFFFFFFF)
		{
			surfaceinfo->normalarray = (signed char *)&check_list[lofs_normal];
			surfaceinfo->normalstride = lstr_normal;
		}
		if (lofs_color != 0xFFFFFFFF)
		{
			surfaceinfo->colorarray = (COLORREF *)&check_list[lofs_color];
			surfaceinfo->colorstride = lstr_color;
		}
		if (lofs_texel != 0xFFFFFFFF)
		{
			surfaceinfo->texcoordarray = (long *)&check_list[lofs_texel];
			surfaceinfo->texcoordstride = lstr_texel;
		}
		surfaceinfo->indexarray = indexlist;
		surfaceinfo->useindices = true;
		surfaceinfo->matrix = matrix;
		locked++;
		return(true);
	}
	return(false);
}

//Unlock this surface
void E3DSurface::Unlock(void)
{
	if (locked > 0)
	{
		locked--;
		if (locked == 0)
			modelchanged = true;
	}
}

//Free resourced used by the surface
void E3DSurface::Free(void)
{
	if (allocvertex == 0 || allocpoly == 0)
		return;
	if (locked > 0)
	{
		locked = 1;
		Unlock();
	}
	if (ecd != NULL)
	{
		if (joint)
		{
			unsigned long ctr;
			for (ctr = 0; ctr < numjoint; ctr++)
			{
				FULL_FREE_L(joint[ctr].animdata);
				FULL_FREE_L(joint[ctr].ref);
			}
			FULL_FREE_L(joint);
		}
		FULL_FREE_L(drawlist);
		FULL_FREE_L(sortindexlist);
		FULL_FREE_L(indexlist);
		FULL_FREE_L(coordlist);
		FULL_FREE_L(coordlist_transformed);
		ecd->dsp->HandleObj3DList(this, false);
	}
	allocvertex = 0;
	allocpoly = 0;
	locked = 0;
}

//Return if the surface is created
bool E3DSurface::IsCreated(void)
{
	if (allocpoly)
		return(true);
	return(false);
}

//Get a vertex with additional info
void E3DSurface::GetVertex(unsigned long index, EVERTEX3D *vertex)
{
	vertex->position.x = 0;
	vertex->position.y = 0;
	vertex->position.z = 0;
	vertex->normal.x = 0;
	vertex->normal.y = 0;
	vertex->normal.z = 0;
	vertex->color = E_RGBA(255, 255, 255, 255);
	vertex->u = 0;
	vertex->v = 0;
	if (index < numvertex)
	{
		E3DSURFACEINFO info;
		if (Lock(&info))
		{
			if (info.vertexarray)
			{
				vertex->position.x = ((long *)((unsigned char *)(info.vertexarray) + index * info.vertexstride))[0];
				vertex->position.y = ((long *)((unsigned char *)(info.vertexarray) + index * info.vertexstride))[1];
				vertex->position.z = ((long *)((unsigned char *)(info.vertexarray) + index * info.vertexstride))[2];
			}
			if (info.normalarray)
			{
				vertex->normal.x = ((signed char *)((unsigned char *)(info.normalarray) + index * info.normalstride))[0];
				vertex->normal.y = ((signed char *)((unsigned char *)(info.normalarray) + index * info.normalstride))[1];
				vertex->normal.z = ((signed char *)((unsigned char *)(info.normalarray) + index * info.normalstride))[2];
			}
			if (info.colorarray)
				vertex->color = E_RGBA(((unsigned char *)(info.colorarray) + index * info.colorstride)[0], ((unsigned char *)(info.colorarray) + index * info.colorstride)[1], ((unsigned char *)(info.colorarray) + index * info.colorstride)[2], ((unsigned char *)(info.colorarray) + index * info.colorstride)[3]);
			vertex->u = ((long *)((unsigned char *)(info.texcoordarray) + index * info.texcoordstride))[0];
			vertex->v = ((long *)((unsigned char *)(info.texcoordarray) + index * info.texcoordstride))[1];
			Unlock();
		}
	}
}

//Define a vertex in 3D space
void E3DSurface::SetVertex(unsigned long index, long x, long y, long z)
{
	if (index >= allocvertex)
		return;
	E3DSURFACEINFO info;
	if (Lock(&info))
	{
		if (info.vertexarray)
		{
			long *glvertexarray = (long *)((unsigned char *)(info.vertexarray) + index * info.vertexstride);
			if (index + 1 > numvertex)
				numvertex = index + 1;
			*glvertexarray++ = x;
			*glvertexarray++ = y;
			*glvertexarray++ = z;
		}
		Unlock();
	}
}

//Define a vertex in 3D space with coordinates, normals, colors and texture coordinates
void E3DSurface::SetVertex(unsigned long index, EVERTEX3D *vertex)
{
	SetVertex(index, vertex->position.x, vertex->position.y, vertex->position.z);
	SetNormal(index, (signed char)vertex->normal.x, (signed char)vertex->normal.y, (signed char)vertex->normal.z);
	SetColor(index, vertex->color);
	SetTexCoord(index, vertex->u, vertex->v);
}

//Define a vertex normal
void E3DSurface::SetNormal(unsigned long index, signed char x, signed char y, signed char z)
{
	if (index >= allocvertex)
		return;
	E3DSURFACEINFO info;
	if (Lock(&info))
	{
		if (info.normalarray)
		{
			signed char *glnormalarray = (signed char *)((unsigned char *)(info.normalarray) + index * info.normalstride);
			if (index + 1 > numvertex)
				numvertex = index + 1;
			if (ecd->dsp->gldriver == NULL)
			{
				*glnormalarray++ = x * 88 / 128;
				*glnormalarray++ = y * 88 / 128;
				*glnormalarray++ = z * 88 / 128;
			}
			else
			{
				*glnormalarray++ = x;
				*glnormalarray++ = y;
				*glnormalarray++ = z;
			}
		}
		Unlock();
	}
}

//Define a vertex color
void E3DSurface::SetColor(unsigned long index, COLORREF color)
{
	if (index >= allocvertex)
		return;
	E3DSURFACEINFO info;
	if (Lock(&info))
	{
		if (info.colorarray)
		{
			unsigned char *glcolorarray = (unsigned char *)((unsigned char *)(info.colorarray) + index * info.colorstride);
			if (index + 1 > numvertex)
				numvertex = index + 1;
			*glcolorarray++ = (unsigned char)E_GETR(color);
			*glcolorarray++ = (unsigned char)E_GETG(color);
			*glcolorarray++ = (unsigned char)E_GETB(color);
			*glcolorarray++ = (unsigned char)E_GETA(color);
		}
		Unlock();
	}
}

//Define a vertex texture coordinate
void E3DSurface::SetTexCoord(unsigned long index, long u, long v)
{
	if (index >= allocvertex)
		return;
	E3DSURFACEINFO info;
	if (Lock(&info))
	{
		if (info.texcoordarray)
		{
			long *gltexelarray = (long *)((unsigned char *)(info.texcoordarray) + index * info.texcoordstride);
			if (index + 1 > numvertex)
				numvertex = index + 1;
			*gltexelarray++ = u;
			*gltexelarray++ = v;
		}
		Unlock();
	}
}

//Link 3 points into a polygon
void E3DSurface::SetPolygon(unsigned long index, unsigned long v1, unsigned long v2, unsigned long v3)
{
	if (index >= allocpoly)
		return;
	if (v1 >= allocvertex || v2 >= allocvertex || v3 >= allocvertex)
		return;
	if (index + 1 > numpoly)
		numpoly = index + 1;
	E3DSURFACEINFO info;
	if (Lock(&info))
	{
		if (info.indexarray)
		{
			unsigned short *glindexarray = (unsigned short *)((unsigned char *)(info.indexarray) + index * 3 * sizeof(unsigned short));
			*glindexarray++ = (unsigned short)v1;
			*glindexarray++ = (unsigned short)v2;
			*glindexarray++ = (unsigned short)v3;
		}
		Unlock();
	}
}

//Link 3 points into a polygon and set vertices simultaniously
void E3DSurface::SetPolygon(unsigned long index, EVERTEX3D *v1, EVERTEX3D *v2, EVERTEX3D *v3, bool checkduplicates)
{
	if (index >= allocpoly)
		return;
	unsigned long v1i = numvertex, v2i = numvertex, v3i = numvertex;
	if (checkduplicates)
	{
		E3DSURFACEINFO info;
		if (Lock(&info))
		{
			unsigned long ctr;
			long *glvertexarray = (long *)info.vertexarray;
			signed char *glnormalarray = (signed char *)info.normalarray;
			COLORREF *glcolorarray = (COLORREF *)info.colorarray;
			long *gltexelarray = (long *)info.texcoordarray;
			for (ctr = 0; ctr < numvertex; ctr++)
			{
				if (glvertexarray)
					if (glvertexarray[ctr * info.vertexstride / sizeof(long)] != v1->position.x || glvertexarray[ctr * info.vertexstride / sizeof(long) + 1] != v1->position.y || glvertexarray[ctr * info.vertexstride / sizeof(long) + 2] != v1->position.z)
						continue;
				if (glnormalarray)
					if (glnormalarray[ctr * info.normalstride] != v1->normal.x || glnormalarray[ctr * info.normalstride + 1] != v1->normal.y || glnormalarray[ctr * info.normalstride + 2] != v1->normal.z)
						continue;
				if (glcolorarray)
					if (glcolorarray[ctr * info.colorstride] != v1->color)
						continue;
				if (gltexelarray)
					if (gltexelarray[ctr * info.texcoordstride] != v1->u || gltexelarray[ctr * info.texcoordstride + 1] != v1->v)
						continue;
				v1i = ctr;
				break;
			}
			for (ctr = 0; ctr < numvertex; ctr++)
			{
				if (glvertexarray)
					if (glvertexarray[ctr * info.vertexstride / sizeof(long)] != v2->position.x || glvertexarray[ctr * info.vertexstride / sizeof(long) + 1] != v2->position.y || glvertexarray[ctr * info.vertexstride / sizeof(long) + 2] != v2->position.z)
						continue;
				if (glnormalarray)
					if (glnormalarray[ctr * info.normalstride] != v2->normal.x || glnormalarray[ctr * info.normalstride + 1] != v2->normal.y || glnormalarray[ctr * info.normalstride + 2] != v2->normal.z)
						continue;
				if (glcolorarray)
					if (glcolorarray[ctr * info.colorstride] != v2->color)
						continue;
				if (gltexelarray)
					if (gltexelarray[ctr * info.texcoordstride] != v2->u || gltexelarray[ctr * info.texcoordstride + 1] != v2->v)
						continue;
				v2i = ctr;
				break;
			}
			for (ctr = 0; ctr < numvertex; ctr++)
			{
				if (glvertexarray)
					if (glvertexarray[ctr * info.vertexstride / sizeof(long)] != v3->position.x || glvertexarray[ctr * info.vertexstride / sizeof(long) + 1] != v3->position.y || glvertexarray[ctr * info.vertexstride / sizeof(long) + 2] != v3->position.z)
						continue;
				if (glnormalarray)
					if (glnormalarray[ctr * info.normalstride] != v3->normal.x || glnormalarray[ctr * info.normalstride + 1] != v3->normal.y || glnormalarray[ctr * info.normalstride + 2] != v3->normal.z)
						continue;
				if (glcolorarray)
					if (glcolorarray[ctr * info.colorstride] != v3->color)
						continue;
				if (gltexelarray)
					if (gltexelarray[ctr * info.texcoordstride] != v3->u || gltexelarray[ctr * info.texcoordstride + 1] != v3->v)
						continue;
				v3i = ctr;
				break;
			}
			Unlock();
		}
	}
	if (v2i >= numvertex && v1i >= numvertex)
		v2i = v1i + 1;
	if (v3i >= numvertex && v2i >= numvertex)
		v3i = v2i + 1;
	else if (v3i >= numvertex && v1i >= numvertex)
		v3i = v1i + 1;
	if (v1i >= allocvertex || v2i >= allocvertex || v3i >= allocvertex)
		return;
	SetVertex(v1i, v1);
	SetVertex(v2i, v2);
	SetVertex(v3i, v3);
	SetPolygon(index, v1i, v2i, v3i);
}

//Change the color of the entire model
void E3DSurface::SetModelColor(COLORREF color)
{
	unsigned long ctr;
	for (ctr = 0; ctr < numvertex; ctr++)
		SetColor(ctr, color);
}

//Manually set the transformation matrix
void E3DSurface::SetMatrix(long *newmatrix)
{
	ClassEMemory::Copy(matrix, newmatrix, 64);
}

//Calculate normals for the polygons
void E3DSurface::CalculateNormals(void)
{
	E3DSURFACEINFO info;
	if (Lock(&info))
	{
		if (info.vertexarray && info.normalarray)
			E3DSurface_PolygonNormals(coordlist, indexlist, lofs_vertex, lofs_normal, lstr_vertex, lstr_normal, numvertex, numpoly, (ecd->dsp->gldriver != NULL));
		Unlock();
	}
}

//Fix texture coordinates for padded textures
void EDisplay_CheckSizePower2(unsigned long &sv);
void E3DSurface::FixTexCoords(E2DSurfaceBase *paddedtexture)
{
	long realwidth = paddedtexture->virtualwidth, realheight = paddedtexture->sheight;
	unsigned long paddedwidth = realwidth, paddedheight = realheight;
	EDisplay_CheckSizePower2(paddedwidth);
	EDisplay_CheckSizePower2(paddedheight);
	if ((long)paddedwidth == realwidth && (long)paddedheight == realheight)
		return;
	unsigned long ctr, vtxcount = numvertex;
	for (ctr = 0; ctr < vtxcount; ctr++)
	{
		unsigned long *txc = (unsigned long *)&coordlist[lofs_texel + ctr * lstr_texel];
		txc[0] = txc[0] * realwidth / paddedwidth;
		txc[1] = txc[1] * realheight / paddedheight + 65536 * (paddedheight - realheight) / paddedheight;
	}
}

//Move all vertices
void E3DSurface::Translate(long x, long y, long z)
{
	if (allocpoly == 0)
		return;
	if (x != 0 || y != 0 || z != 0)
	{
		E3DSurface_UpdateMatrixTranslate(x, y, z, matrix);
		modelchanged = true;
	}
}

//Rotate all vertices around the center point
void E3DSurface::Rotate(long xr, long yr, long zr)
{
	if (allocpoly == 0)
		return;
	if (xr != 0 || yr != 0 || zr != 0)
	{
		long imatrix[] = {
			4096, 0, 0, 0,
			0, 4096, 0, 0,
			0, 0, 4096, 0,
			0, 0, 0, 4096
		};
		ecd->math->MatrixSetRotate((unsigned short)xr, (unsigned short)yr, (unsigned short)zr, imatrix);
		ecd->math->MatrixMultiply(matrix, imatrix);
		modelchanged = true;
	}
}

//Scale all vertices to the desired sizes
void E3DSurface::Scale(long xz, long yz, long zz)
{
	if (allocpoly == 0)
		return;
	if (xz != 65536 || yz != 65536 || zz != 65536)
	{
		long scalematrix[] = {
			xz >> 4, 0, 0, 0,
			0, yz >> 4, 0, 0,
			0, 0, zz >> 4, 0,
			0, 0, 0, 4096
		};
		ClassEMath::MatrixMultiply(matrix, scalematrix);
		modelchanged = true;
	}
}

//Set the animation time
void E3DSurface::Animate(unsigned long time)
{
	if (joint == NULL)
		return;
	unsigned long ctr;
	if (jointnew)
	{
		ClassEMemory::Copy(coordlist_transformed, coordlist, coordlistsize);
		jointnew = false;
	}
	for (ctr = 0; ctr < numjoint; ctr++)
	{
		joint[ctr].position[0] = 0;
		joint[ctr].position[1] = 0;
		joint[ctr].position[2] = 0;
		joint[ctr].rotation[0] = 0;
		joint[ctr].rotation[1] = 0;
		joint[ctr].rotation[2] = 0;
		if (joint[ctr].animdata)
		{
			long *adata = joint[ctr].animdata;
			unsigned long frame;
			frame = 0;
			while (frame < joint[ctr].countanimpos && adata[frame * 4] < (long)time)
				frame++;
			if (frame > 0 && frame < joint[ctr].countanimpos)
			{
				long interpol = adata[frame * 4] - adata[frame * 4 - 4];
				if (interpol)
					interpol = (time - adata[frame * 4 - 4]) * 256 / interpol;
				joint[ctr].position[0] = adata[frame * 4 - 4 + 1] + (adata[frame * 4 + 1] - adata[frame * 4 - 4 + 1]) * interpol / 256;
				joint[ctr].position[1] = adata[frame * 4 - 4 + 2] + (adata[frame * 4 + 2] - adata[frame * 4 - 4 + 2]) * interpol / 256;
				joint[ctr].position[2] = adata[frame * 4 - 4 + 3] + (adata[frame * 4 + 3] - adata[frame * 4 - 4 + 3]) * interpol / 256;
			}
			else if (frame == 0)
			{
				joint[ctr].position[0] = adata[1];
				joint[ctr].position[1] = adata[2];
				joint[ctr].position[2] = adata[3];
			}
			else
			{
				joint[ctr].position[0] = adata[(joint[ctr].countanimpos - 1) * 4 + 1];
				joint[ctr].position[1] = adata[(joint[ctr].countanimpos - 1) * 4 + 2];
				joint[ctr].position[2] = adata[(joint[ctr].countanimpos - 1) * 4 + 3];
			}
			adata += joint[ctr].countanimpos * 4;
			frame = 0;
			while (frame < joint[ctr].countanimrot && adata[frame * 4] < (long)time)
				frame++;
			if (frame > 0 && frame < joint[ctr].countanimrot)
			{
				long interpol = adata[frame * 4] - adata[frame * 4 - 4];
				if (interpol)
					interpol = (time - adata[frame * 4 - 4]) * 256 / interpol;
				joint[ctr].rotation[0] = adata[frame * 4 - 4 + 1] + (adata[frame * 4 + 1] - adata[frame * 4 - 4 + 1]) * interpol / 256;
				joint[ctr].rotation[1] = adata[frame * 4 - 4 + 2] + (adata[frame * 4 + 2] - adata[frame * 4 - 4 + 2]) * interpol / 256;
				joint[ctr].rotation[2] = adata[frame * 4 - 4 + 3] + (adata[frame * 4 + 3] - adata[frame * 4 - 4 + 3]) * interpol / 256;
			}
			else if (frame == 0)
			{
				joint[ctr].rotation[0] = adata[1];
				joint[ctr].rotation[1] = adata[2];
				joint[ctr].rotation[2] = adata[3];
			}
			else
			{
				joint[ctr].rotation[0] = adata[(joint[ctr].countanimrot - 1) * 4 + 1];
				joint[ctr].rotation[1] = adata[(joint[ctr].countanimrot - 1) * 4 + 2];
				joint[ctr].rotation[2] = adata[(joint[ctr].countanimrot - 1) * 4 + 3];
			}
		}
	}
	for (ctr = 0; ctr < numjoint; ctr++)
	{
		long transform[16], tmpmatrix[16];
		ClassEMemory::Copy(transform, &joint[ctr].matrix[16], 64);
		ecd->math->MatrixLoadIdentity(tmpmatrix);
		ecd->math->MatrixSetRotate((unsigned short)joint[ctr].rotation[0], (unsigned short)joint[ctr].rotation[1], (unsigned short)joint[ctr].rotation[2], tmpmatrix);
		ecd->math->MatrixSetTranslate(joint[ctr].position[0], joint[ctr].position[1], joint[ctr].position[2], tmpmatrix);
		ecd->math->MatrixMultiply(transform, tmpmatrix);
		if (joint[ctr].parent >= 0)
		{
			ClassEMemory::Copy(&joint[ctr].matrix[48], &joint[joint[ctr].parent].matrix[48], 64);
			ClassEMath::MatrixMultiply(&joint[ctr].matrix[48], transform);
		}
		else
			ClassEMemory::Copy(&joint[ctr].matrix[48], transform, 64);
		ClassEMemory::Copy(&joint[ctr].matrix[32], &joint[ctr].matrix[48], 64);
		ClassEMath::MatrixMultiply(&joint[ctr].matrix[32], &joint[ctr].matrix[64]);
	}
	for (ctr = 0; ctr < numjoint; ctr++)
	{
		if (joint[ctr].ref)
		{
			unsigned long vtxstride = lstr_vertex / sizeof(long);
			long *glvertexarray_src = (long *)&coordlist[lofs_vertex];
			long *glvertexarray_dst = (long *)&coordlist_transformed[lofs_vertex];
			unsigned long tctr;
			for (tctr = 0; tctr < joint[ctr].vertexcount; tctr++)
				E3DSurface_MatrixTransform(glvertexarray_dst[joint[ctr].ref[tctr] * vtxstride], glvertexarray_dst[joint[ctr].ref[tctr] * vtxstride + 1], glvertexarray_dst[joint[ctr].ref[tctr] * vtxstride + 2], glvertexarray_src[joint[ctr].ref[tctr] * vtxstride], glvertexarray_src[joint[ctr].ref[tctr] * vtxstride + 1], glvertexarray_src[joint[ctr].ref[tctr] * vtxstride + 2], &joint[ctr].matrix[32]);
			signed char *glnormalarray_src = (signed char *)&coordlist[lofs_normal];
			signed char *glnormalarray_dst = (signed char *)&coordlist_transformed[lofs_normal];
			for (tctr = 0; tctr < joint[ctr].vertexcount; tctr++)
			{
				long tmpx, tmpy, tmpz;
				E3DSurface_MatrixRotate(tmpx, tmpy, tmpz, glnormalarray_src[joint[ctr].ref[tctr] * lstr_normal], glnormalarray_src[joint[ctr].ref[tctr] * lstr_normal + 1], glnormalarray_src[joint[ctr].ref[tctr] * lstr_normal + 2], &joint[ctr].matrix[32]);
				glnormalarray_dst[joint[ctr].ref[tctr] * lstr_normal] = (signed char)tmpx;
				glnormalarray_dst[joint[ctr].ref[tctr] * lstr_normal + 1] = (signed char)tmpy;
				glnormalarray_dst[joint[ctr].ref[tctr] * lstr_normal + 2] = (signed char)tmpz;
			}
		}
	}
	useanimation = true;
}

//Reset the surface to its initial state (for absolute translating, rotating, etc.)
void E3DSurface::Reset(void)
{
	if (allocpoly == 0 || !modelchanged)
		return;
	unsigned long ctr;
	for (ctr = 0; ctr < 16; ctr++)
	{
		matrix[ctr] = 0;
		if ((ctr >> 2) == (ctr & 3))
			matrix[ctr] = 4096;
	}
	modelchanged = false;
}

//Reset model animation
void E3DSurface::ResetAnimation(void)
{
	useanimation = false;
}

//Create joints for animation
void E3DSurface::CreateJoints(unsigned long count)
{
	if (allocpoly == 0)
		return;
	if (joint)
	{
		unsigned long ctr;
		for (ctr = 0; ctr < numjoint; ctr++)
			FULL_FREE_L(joint[ctr].ref);
		FULL_FREE_L(joint);
	}
	if (count == 0)
		return;
	joint = (E3DJOINT *)ecd->mem->AllocList(count * sizeof(E3DJOINT));
	if (joint != NULL)
	{
		long imatrix[] = {
			4096, 0, 0, 0,
			0, 4096, 0, 0,
			0, 0, 4096, 0,
			0, 0, 0, 4096
		};
		unsigned long ctr;
		numjoint = count;
		for (ctr = 0; ctr < numjoint; ctr++)
		{
			ClassEMemory::Copy(&joint[ctr].matrix[0], imatrix, 64);
			ClassEMemory::Copy(&joint[ctr].matrix[16], imatrix, 64);
			ClassEMemory::Copy(&joint[ctr].matrix[32], imatrix, 64);
			joint[ctr].vertexcount = 0;
			joint[ctr].ref = NULL;
			joint[ctr].animdata = NULL;
			joint[ctr].parent = -1;
			joint[ctr].countanimpos = 0;
			joint[ctr].countanimrot = 0;
			joint[ctr].position[0] = 0;
			joint[ctr].position[1] = 0;
			joint[ctr].position[2] = 0;
			joint[ctr].rotation[0] = 0;
			joint[ctr].rotation[1] = 0;
			joint[ctr].rotation[2] = 0;
		}
		coordlist_transformed = (unsigned char *)ecd->mem->AllocList(coordlistsize);
		if (coordlist_transformed == NULL)
			FULL_FREE_L(joint);
		useanimation = false;
		jointnew = true;
	}
}

//Attach vertices to the joint
void E3DSurface::AttachJoint(unsigned long index, unsigned short *vertexlist, unsigned short listsize)
{
	if (joint && index < numjoint)
	{
		unsigned long totallist = joint[index].vertexcount + listsize;
		unsigned short *allocptr = NULL;
		if (totallist)
		{
			allocptr = (unsigned short *)ecd->mem->AllocList(totallist * 2);
			if (allocptr)
			{
				if (joint[index].vertexcount)
					ClassEMemory::Copy(allocptr, joint[index].ref, joint[index].vertexcount * 2);
				if (joint[index].ref)
					FULL_FREE_L(joint[index].ref);
				joint[index].ref = allocptr;
				ClassEMemory::Copy(&joint[index].ref[joint[index].vertexcount], vertexlist, listsize * 2);
				joint[index].vertexcount += listsize;
			}
		}
		jointnew = true;
	}
}

//Set joint parent, location and rotation
void E3DSurface::SetJointProperties(unsigned long index, EVECTOR3D *location, EVECTOR3D *rotation, long parent)
{
	if (joint && index < numjoint)
	{
		joint[index].parent = (short)parent;
		ecd->math->MatrixLoadIdentity(&joint[index].matrix[16]);
		ecd->math->MatrixSetRotate((unsigned short)rotation->x, (unsigned short)rotation->y, (unsigned short)rotation->z, &joint[index].matrix[16]);
		ecd->math->MatrixSetTranslate(location->x, location->y, location->z, &joint[index].matrix[16]);
		if (parent >= 0)
		{
			ClassEMemory::Copy(&joint[index].matrix[0], &joint[joint[index].parent].matrix[0], 64);
			ClassEMath::MatrixMultiply(&joint[index].matrix[0], &joint[index].matrix[16]);
		}
		else
			ClassEMemory::Copy(&joint[index].matrix[0], &joint[index].matrix[16], 64);
		ClassEMemory::Copy(&joint[index].matrix[64], &joint[index].matrix[0], 64);
		ClassEMath::MatrixReverse(&joint[index].matrix[64]);
		jointnew = true;
	}
}

//Create animation key frames for joint
void E3DSurface::CreateJointKeyFrames(unsigned long index, unsigned long numtranslate, unsigned long numrotate)
{
	if (joint && index < numjoint)
	{
		FULL_FREE_L(joint[index].animdata);
		if ((numtranslate + numrotate) > 0)
		{
			joint[index].countanimpos = numtranslate;
			joint[index].countanimrot = numrotate;
			joint[index].animdata = (long *)ecd->mem->AllocList((numtranslate * 4 + numrotate * 4) * sizeof(long));
		}
	}
}

//Set joint keyframe data
void E3DSurface::SetJointKeyFrames(unsigned long index, long *keyframelist, unsigned long listsize, bool isrotation)
{
	if (joint && index < numjoint && listsize > 0)
		if (joint[index].animdata)
		{
			long *animptr = joint[index].animdata;
			if (isrotation)
				animptr += joint[index].countanimpos * 4;
			ClassEMemory::Copy(animptr, keyframelist, listsize * 16);
		}
}

//Return the amount of vertices in use
unsigned long E3DSurface::VerticesInUse(void)
{
	return(numvertex);
}

//Return the amount of polygons in use
unsigned long E3DSurface::PolygonsInUse(void)
{
	return(numpoly);
}

//Start loading a mesh
ERESULT E3DSurface::LdrOnMeshStart(ELDR_3DCALLBACKINFO *ldrdata)
{
	ERESULT er;
	//bool calcdups = !ldrdata->normalonvertex;//(!ldrdata->coloronvertex || !ldrdata->normalonvertex || !ldrdata->texelonvertex);
	//if (calcdups)
	//	ldrdata->reservevertices += ldrdata->reservevertices * 16;// / 2;
	calcloadnormal = true;
	er = Create(ldrdata->reservevertices, ldrdata->reserveindices, ldrdata->createflags);
	if (er != E_OK)
		return(er);
	/*if (calcdups)
	{
		duplist = ClassEMemory::Alloc(ldrdata->reservevertices * sizeof(long) + DUPLIST_FRAGMENT * sizeof(DUPLIST_ITEM));
		ClassEMemory::Set(duplist, 0, ldrdata->reservevertices * sizeof(long));
		if (duplist == NULL)
			return(E_NOMEMORY);
		dupcount = 0;
	}*/
	return(E_OK);
}

//Mesh has been loaded
ERESULT E3DSurface::LdrOnMeshEnd(ELDR_3DCALLBACKINFO *ldrdata)
{
	/*
	if (duplist)
	{
		if (dupcount)
		{
			DUPLIST_ITEM *dupv = (DUPLIST_ITEM *)((unsigned char *)(duplist) + allocvertex * sizeof(long));
			unsigned long ctr, addvertex = 0;
			for (ctr = 0; ctr < dupcount; ctr++)
				if (dupv[ctr].forwardlistindex == -1)
					addvertex++;
			if (numvertex + addvertex <= allocvertex)
			{
				EVERTEX3D v3d;
				for (ctr = 0; ctr < dupcount; ctr++)
				{
					if (dupv[ctr].forwardlistindex == -1)
					{
						GetVertex(dupv[ctr].vertexid, &v3d);
						if (dupv[ctr].mask & DPLF_NORMAL)
						{
							v3d.normal.x = dupv[ctr].normal[0];
							v3d.normal.y = dupv[ctr].normal[1];
							v3d.normal.z = dupv[ctr].normal[2];
						}
						dupv[ctr].vertexid = numvertex;
						indexlist[dupv[ctr].triangleid] = (unsigned short)dupv[ctr].vertexid;
						SetVertex(dupv[ctr].vertexid, &v3d);
					}
					else
						indexlist[dupv[ctr].triangleid] = (unsigned short)dupv[dupv[ctr].forwardlistindex].vertexid;
				}
				//ClassEConsole::MsgWindow(numvertex);
				//ClassEConsole::MsgWindow(allocvertex);
			}//add to list (lbl)
		}
		FULL_FREE(duplist);
	}
	*/
	if (calcloadnormal)
		CalculateNormals();
	if (locked)
	{
		locked = 1;
		Unlock();
	}
	return(E_ABORTED);
}

//Load callback: Vertex stream data
ERESULT E3DSurface::LdrOnVertexStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	long *streamptr = (long *)ldrdata->streamdata;
	unsigned long ctr;
	for (ctr = 0; ctr < ldrdata->streamsize / 12; ctr++)
		SetVertex(ctr + ldrdata->vertexindex, streamptr[ctr * 3], streamptr[ctr * 3 + 1], streamptr[ctr * 3 + 2]);
	ldrdata->vertexindex += ldrdata->streamsize / 12;
	return(E_OK);
}

//Load callback: Normal stream data
ERESULT E3DSurface::LdrOnNormalStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	if (lofs_normal == 0xFFFFFFFF)
		return(E_OK);
	signed char *glnormalarray = (signed char *)&coordlist[lofs_normal];
	long *streamptr = (long *)ldrdata->streamdata;
	unsigned long ctr;
	signed char multip = 127;
	calcloadnormal = false;
	if (ecd->dsp->gldriver == NULL)
		multip = 88;
	if (ldrdata->normalonvertex)
	{
		glnormalarray += ldrdata->normalindex * lstr_normal;
		for (ctr = 0; ctr < ldrdata->streamsize / 12; ctr++)
		{
			glnormalarray[0] = (signed char)(streamptr[ctr * 3] * multip / 65536);
			glnormalarray[1] = (signed char)(streamptr[ctr * 3 + 1] * multip / 65536);
			glnormalarray[2] = (signed char)(streamptr[ctr * 3 + 2] * multip / 65536);
			glnormalarray += lstr_normal;
		}
		ldrdata->normalindex += ldrdata->streamsize / 12;
	}
	else
	{
		//unsigned long *dupidx = (unsigned long *)duplist;
		for (ctr = 0; ctr < ldrdata->streamsize / 36; ctr++)
		{
			//bool createdup;
			unsigned long idx = (ldrdata->normalindex + ctr) * 3;
			/*createdup = true;
			if (dupidx[indexlist[idx]] & DPLF_NORMAL)
			{
				if (glnormalarray[indexlist[idx] * lstr_normal] != (signed char)(streamptr[ctr * 9] * multip / 65536))
					createdup = true;
				else if (glnormalarray[indexlist[idx] * lstr_normal + 1] != (signed char)(streamptr[ctr * 9 + 1] * multip / 65536))
					createdup = true;
				else if (glnormalarray[indexlist[idx] * lstr_normal + 2] = (signed char)(streamptr[ctr * 9 + 2] * multip / 65536))
					createdup = true;
			}
			if (createdup)
			{
				E3DDupl_AddNew(duplist, dupcount, idx, indexlist[idx], allocvertex, (signed char)(streamptr[ctr * 9] * multip / 65536), (signed char)(streamptr[ctr * 9 + 1] * multip / 65536), (signed char)(streamptr[ctr * 9 + 2] * multip / 65536), 0, 0, 0, DPLF_NORMAL);
			}
			else
			{*/
				glnormalarray[indexlist[idx] * lstr_normal] = (signed char)(streamptr[ctr * 9] * multip / 65536);
				glnormalarray[indexlist[idx] * lstr_normal + 1] = (signed char)(streamptr[ctr * 9 + 1] * multip / 65536);
				glnormalarray[indexlist[idx] * lstr_normal + 2] = (signed char)(streamptr[ctr * 9 + 2] * multip / 65536);
			//	dupidx[indexlist[idx]] |= DPLF_NORMAL;
			//}
			/*createdup = true;
			if (dupidx[indexlist[idx + 1]] & DPLF_NORMAL)
			{
				if (glnormalarray[indexlist[idx + 1] * lstr_normal] != (signed char)(streamptr[ctr * 9 + 3] * multip / 65536))
					createdup = true;
				else if (glnormalarray[indexlist[idx + 1] * lstr_normal + 1] != (signed char)(streamptr[ctr * 9 + 4] * multip / 65536))
					createdup = true;
				else if (glnormalarray[indexlist[idx + 1] * lstr_normal + 2] = (signed char)(streamptr[ctr * 9 + 5] * multip / 65536))
					createdup = true;
			}
			if (createdup)
			{
				E3DDupl_AddNew(duplist, dupcount, idx + 1, indexlist[idx + 1], allocvertex, (signed char)(streamptr[ctr * 9 + 3] * multip / 65536), (signed char)(streamptr[ctr * 9 + 4] * multip / 65536), (signed char)(streamptr[ctr * 9 + 5] * multip / 65536), 0, 0, 0, DPLF_NORMAL);
			}
			else
			{*/
				glnormalarray[indexlist[idx + 1] * lstr_normal] = (signed char)(streamptr[ctr * 9 + 3] * multip / 65536);
				glnormalarray[indexlist[idx + 1] * lstr_normal + 1] = (signed char)(streamptr[ctr * 9 + 4] * multip / 65536);
				glnormalarray[indexlist[idx + 1] * lstr_normal + 2] = (signed char)(streamptr[ctr * 9 + 5] * multip / 65536);
			//	dupidx[indexlist[idx + 1]] |= DPLF_NORMAL;
			//}
			/*createdup = true;
			int tidx = idx + 2;
			int tidx2 = indexlist[tidx];
			int tidx3 = dupidx[tidx2];
			if (dupidx[indexlist[idx + 2]] & DPLF_NORMAL)
			{
				if (glnormalarray[indexlist[idx + 2] * lstr_normal] != (signed char)(streamptr[ctr * 9 + 6] * multip / 65536))
					createdup = true;
				else if (glnormalarray[indexlist[idx + 2] * lstr_normal + 1] != (signed char)(streamptr[ctr * 9 + 7] * multip / 65536))
					createdup = true;
				else if (glnormalarray[indexlist[idx + 2] * lstr_normal + 2] = (signed char)(streamptr[ctr * 9 + 8] * multip / 65536))
					createdup = true;
			}
			if (createdup)
			{
				E3DDupl_AddNew(duplist, dupcount, idx + 2, indexlist[idx + 2], allocvertex, (signed char)(streamptr[ctr * 9 + 6] * multip / 65536), (signed char)(streamptr[ctr * 9 + 7] * multip / 65536), (signed char)(streamptr[ctr * 9 + 8] * multip / 65536), 0, 0, 0, DPLF_NORMAL);
			}
			else
			{*/
				glnormalarray[indexlist[idx + 2] * lstr_normal] = (signed char)(streamptr[ctr * 9 + 6] * multip / 65536);
				glnormalarray[indexlist[idx + 2] * lstr_normal + 1] = (signed char)(streamptr[ctr * 9 + 7] * multip / 65536);
				glnormalarray[indexlist[idx + 2] * lstr_normal + 2] = (signed char)(streamptr[ctr * 9 + 8] * multip / 65536);
			//	dupidx[indexlist[idx + 2]] |= DPLF_NORMAL;
			//}
		}
		ldrdata->normalindex += ldrdata->streamsize / 36;
	}
	return(E_OK);
}

//Load callback: Texel stream data
ERESULT E3DSurface::LdrOnTexcoordStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	if (lofs_texel == 0xFFFFFFFF)
		return(E_OK);
	long *gltexelarray;
	gltexelarray = (long *)&coordlist[lofs_texel];
	unsigned long *streamptr = (unsigned long *)ldrdata->streamdata;
	unsigned long ctr;
	if (ldrdata->texelonvertex)
	{
		gltexelarray += ldrdata->texelindex * lstr_texel / sizeof(long);
		for (ctr = 0; ctr < ldrdata->streamsize / 8; ctr++)
		{
			gltexelarray[0] = streamptr[ctr * 2];
			gltexelarray[1] = streamptr[ctr * 2 + 1];
			gltexelarray += lstr_texel / sizeof(long);
		}
		ldrdata->texelindex += ldrdata->streamsize / 8;
	}
	else
	{
		for (ctr = 0; ctr < ldrdata->streamsize / 24; ctr++)
		{
			gltexelarray[indexlist[(ldrdata->texelindex + ctr) * 3] * (lstr_texel / sizeof(long))] = streamptr[ctr * 6];
			gltexelarray[indexlist[(ldrdata->texelindex + ctr) * 3] * (lstr_texel / sizeof(long)) + 1] = streamptr[ctr * 6 + 1];
			gltexelarray[indexlist[(ldrdata->texelindex + ctr) * 3 + 1] * (lstr_texel / sizeof(long))] = streamptr[ctr * 6 + 2];
			gltexelarray[indexlist[(ldrdata->texelindex + ctr) * 3 + 1] * (lstr_texel / sizeof(long)) + 1] = streamptr[ctr * 6 + 3];
			gltexelarray[indexlist[(ldrdata->texelindex + ctr) * 3 + 2] * (lstr_texel / sizeof(long))] = streamptr[ctr * 6 + 4];
			gltexelarray[indexlist[(ldrdata->texelindex + ctr) * 3 + 2] * (lstr_texel / sizeof(long)) + 1] = streamptr[ctr * 6 + 5];
		}
		ldrdata->texelindex += ldrdata->streamsize / 24;
	}
	return(E_OK);
}

//Load callback: Color stream data
ERESULT E3DSurface::LdrOnColorStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	if (lofs_color == 0xFFFFFFFF)
		return(E_OK);
	unsigned char *glcolorarray = (unsigned char *)&coordlist[lofs_color];
	unsigned long *streamptr = (unsigned long *)ldrdata->streamdata;
	unsigned long ctr;
	if (ldrdata->coloronvertex)
	{
		glcolorarray += ldrdata->colorindex * lstr_color;
		for (ctr = 0; ctr < ldrdata->streamsize / 4; ctr++)
		{
			glcolorarray[0] = (unsigned char)E_GETR(streamptr[ctr]);
			glcolorarray[1] = (unsigned char)E_GETG(streamptr[ctr]);
			glcolorarray[2] = (unsigned char)E_GETB(streamptr[ctr]);
			glcolorarray[3] = (unsigned char)E_GETA(streamptr[ctr]);
			glcolorarray += lstr_color;
		}
		ldrdata->colorindex += ldrdata->streamsize / 4;
	}
	else
	{
		for (ctr = 0; ctr < ldrdata->streamsize / 4; ctr++)
		{
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3] * lstr_color] = (unsigned char)E_GETR(streamptr[ctr]);
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3] * lstr_color + 1] = (unsigned char)E_GETG(streamptr[ctr]);
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3] * lstr_color + 2] = (unsigned char)E_GETB(streamptr[ctr]);
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3] * lstr_color + 3] = (unsigned char)E_GETA(streamptr[ctr]);
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3 + 1] * lstr_color] = (unsigned char)E_GETR(streamptr[ctr]);
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3 + 1] * lstr_color + 1] = (unsigned char)E_GETG(streamptr[ctr]);
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3 + 1] * lstr_color + 2] = (unsigned char)E_GETB(streamptr[ctr]);
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3 + 1] * lstr_color + 3] = (unsigned char)E_GETA(streamptr[ctr]);
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3 + 2] * lstr_color] = (unsigned char)E_GETR(streamptr[ctr]);
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3 + 2] * lstr_color + 1] = (unsigned char)E_GETG(streamptr[ctr]);
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3 + 2] * lstr_color + 2] = (unsigned char)E_GETB(streamptr[ctr]);
			glcolorarray[indexlist[(ldrdata->colorindex + ctr) * 3 + 2] * lstr_color + 3] = (unsigned char)E_GETA(streamptr[ctr]);
		}
		ldrdata->colorindex += ldrdata->streamsize / 4;
	}
	return(E_OK);
}

//Load callback: Polygon stream data
ERESULT E3DSurface::LdrOnIndexStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	unsigned long *streamptr = (unsigned long *)ldrdata->streamdata;
	unsigned long ctr;
	for (ctr = 0; ctr < ldrdata->streamsize / 16; ctr++)
		SetPolygon(ctr + ldrdata->polygonindex, streamptr[ctr * 4], streamptr[ctr * 4 + 1], streamptr[ctr * 4 + 2]);
	ldrdata->polygonindex += ldrdata->streamsize / 16;
	return(E_OK);
}

//Load callback: Joint stream data
ERESULT E3DSurface::LdrOnJointStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	if (joint == NULL || ldrdata->streamsize == 0)
		return(E_NODATA);
	bool sorted = true;
	unsigned short *streamptr16 = (unsigned short *)ldrdata->streamdata;
	unsigned long *streamptr32 = (unsigned long *)ldrdata->streamdata;
	unsigned long ctr, startlist, listsize;
	unsigned short curjoint;
	while(sorted)
	{
		sorted = false;
		for (ctr = 0; ctr < ldrdata->streamsize / 4 - 1; ctr++)
			if ((streamptr32[ctr] >> 16) > (streamptr32[ctr + 1] >> 16))
			{
				unsigned long tmp = streamptr32[ctr];
				streamptr32[ctr] = streamptr32[ctr + 1];
				streamptr32[ctr + 1] = tmp;
				sorted = true;
			}
	}
	curjoint = streamptr16[1];
	startlist = 0;
	listsize = 0;
	for (ctr = 0; ctr < ldrdata->streamsize / 4 + 1; ctr++)
	{
		if (streamptr16[(ctr << 1) + 1] != curjoint || ctr == ldrdata->streamsize / 4)
		{
			AttachJoint(curjoint, &streamptr16[startlist], (unsigned short)listsize);
			curjoint = streamptr16[(ctr << 1) + 1];
			startlist = ctr;
			listsize = 0;
		}
		if (ctr < ldrdata->streamsize / 4)
		{
			streamptr16[ctr] = streamptr16[ctr << 1];
			listsize++;
		}
	}
	return(E_OK);
}

//Load callback: Keyframe stream data
ERESULT E3DSurface::LdrOnKeyFrameStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	if (joint == NULL || ldrdata->jointindex >= numjoint)
		return(E_ERROR);
	if (joint[ldrdata->jointindex].animdata == NULL)
		return(E_ERROR);
	long *animptr = joint[ldrdata->jointindex].animdata;
	if (ldrdata->keyframetype == 1)
		animptr += joint[ldrdata->jointindex].countanimpos * 4;
	ClassEMemory::Copy(&animptr[ldrdata->keyframeindex * 4], ldrdata->streamdata, ldrdata->streamsize);
	ldrdata->keyframeindex += ldrdata->streamsize / 16;
	return(E_OK);
}

/*
//Transform vertices through the matrix
void E3DSurface::Transform(void)
{
	if (allocpoly == 0 || coordlist_org == NULL)
		return;
	long *glvertexarray = (long *)&coordlist[lofs_vertex];
	unsigned long ctr;
	long tmpx, tmpy, tmpz;
	for (ctr = 0; ctr < numvertex; ctr++)
	{
		tmpx = glvertexarray[0];
		tmpy = glvertexarray[1];
		tmpz = glvertexarray[2];
		glvertexarray[0] = ((tmpx * matrix[0]) >> 12) + ((tmpy * matrix[1]) >> 12) + ((tmpz * matrix[2]) >> 12) + matrix[3];
		glvertexarray[1] = ((tmpx * matrix[4]) >> 12) + ((tmpy * matrix[5]) >> 12) + ((tmpz * matrix[6]) >> 12) + matrix[7];
		glvertexarray[2] = ((tmpx * matrix[8]) >> 12) + ((tmpy * matrix[9]) >> 12) + ((tmpz * matrix[10]) >> 12) + matrix[11];
		glvertexarray += lstr_vertex / sizeof(long);
	}
	if (lofs_normal != 0xFFFFFFFF)
	{
		signed char *glnormalarray = (signed char *)&coordlist[lofs_normal];
		for (ctr = 0; ctr < numvertex; ctr++)
		{
			tmpx = glnormalarray[0];
			tmpy = glnormalarray[1];
			tmpz = glnormalarray[2];
			glnormalarray[0] = (signed char)(((tmpx * matrix[0]) >> 12) + ((tmpy * matrix[1]) >> 12) + ((tmpz * matrix[2]) >> 12));
			glnormalarray[1] = (signed char)(((tmpx * matrix[4]) >> 12) + ((tmpy * matrix[5]) >> 12) + ((tmpz * matrix[6]) >> 12));
			glnormalarray[2] = (signed char)(((tmpx * matrix[8]) >> 12) + ((tmpy * matrix[9]) >> 12) + ((tmpz * matrix[10]) >> 12));
			glnormalarray += lstr_normal / sizeof(signed char);
		}
	}
	modelchanged = true;
}
*/
