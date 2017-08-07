// eldr_e3d.cpp
// E3D file loader
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eloader.h"
#include "emath.h"
#include "ememory.h"
#include "edisplay.h"
#include "estdc.h"
#include "econsole.h"

#include "extra/verinfo.h"

///////////////////////////////////////////////////////////////////
// Type definitions                                              //
///////////////////////////////////////////////////////////////////

//Stream data chunk size
#define E3DF_MAXSTREAMDAT  1024

//Macro for 4-character codes
#define E3DF_FOURCC(c1, c2, c3, c4) (c1 | (c2 << 8) | (c3 << 16) | (c4 << 24))

//File identifier
#define E3DF_SIGNATURE E3DF_FOURCC('E', '3', 'D', '_')
#define E3DF_EIBV      E3DF_FOURCC('E', 'I', 'B', 'V')
#define E3DF_VER       E3DF_FOURCC('1', '.', '0', '0')

//Chunks
#define E3DCH_eGRP    E3DF_FOURCC('e', 'G', 'R', 'P')
#define E3DCH_EMSH    E3DF_FOURCC('E', 'M', 'S', 'H')
	#define E3DCH_EVTX    E3DF_FOURCC('E', 'V', 'T', 'X')
	#define E3DCH_ETRI    E3DF_FOURCC('E', 'T', 'R', 'I')
	#define E3DCH_eCOL    E3DF_FOURCC('e', 'C', 'O', 'L')
	#define E3DCH_eNRM    E3DF_FOURCC('e', 'N', 'R', 'M')
	#define E3DCH_eTXC    E3DF_FOURCC('e', 'T', 'X', 'C')
	#define E3DCH_eJNT    E3DF_FOURCC('e', 'J', 'N', 'T')
	#define E3DCH_eKEY    E3DF_FOURCC('e', 'K', 'E', 'Y')
#define E3DCH_END     E3DF_FOURCC('_', 'E', 'N', 'D')

//Content flags
#define E3CFL_VERTICES     0x1
#define E3CFL_TRIANGLES    0x2
#define E3CFL_COLORS       0x4
#define E3CFL_NORMALS      0x8
#define E3CFL_TEXCOORDS   0x10
#define E3CFL_JOINTS      0x20
#define E3CFL_KEYFRAMES   0x40

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

//3D model structure
typedef struct
{
	unsigned long type;
	unsigned long vendor;
	unsigned long ver;
}E3DF_HEADER;

//3D model structure
typedef struct
{
	char name[32];
	unsigned long contentflags;
	unsigned long vertices;
	unsigned long triangles;
}E3DF_MODEL;

///////////////////////////////////////////////////////////////////
// Loader: Public                                                //
///////////////////////////////////////////////////////////////////

//Start the loader
ERESULT ELoader_E3D::Start(E3DLoaderBase *surface, void *ldata, unsigned long lsize, signed char modelsize, unsigned long createflags)
{
	#if (EDITION_SDK < 1000)
		return(E_NOFEATURE);
	#else
		ERESULT result = E_OK;
		ELDR_3DCALLBACKINFO cbinfo;
		E3DF_HEADER header;
		E3DF_MODEL model;
		bool firstmdl = true, forcebreak = false;
		unsigned long ctr, segment_id, segment_size, segment_size_bak;
		unsigned char streamdat[E3DF_MAXSTREAMDAT];
		unsigned char tmpstream[E3DF_MAXSTREAMDAT];
		long *streamdat32 = (long *)streamdat;
		unsigned long *tmpstream32 = (unsigned long *)tmpstream;
		//unsigned short *streamdat16 = (unsigned short *)streamdat;
		unsigned short *tmpstream16 = (unsigned short *)tmpstream;
		signed char *tmpstream8 = (signed char *)tmpstream;
		LinkData(ldata, lsize);
		if (!ReadStream(&header, sizeof(header)))
			return(E_ERROR);
		if (header.type != E3DF_SIGNATURE)
			return(E_UNSUPPORTED);
		if (header.vendor != E3DF_EIBV)
			return(E_UNSUPPORTED);
		cbinfo.createflags = createflags;
		cbinfo.vertexindex = 0;
		cbinfo.polygonindex = 0;
		cbinfo.texelindex = 0;
		cbinfo.colorindex = 0;
		cbinfo.normalindex = 0;
		cbinfo.texelonvertex = true;
		cbinfo.coloronvertex = true;
		cbinfo.normalonvertex = true;
		do
		{
			unsigned long prevpos;
			Read32(&segment_id);
			Read32(&segment_size);
			segment_size_bak = segment_size;
			prevpos = Tell();
			if (segment_size == 0)
				break;
			switch(segment_id)
			{
				case E3DCH_EMSH:
					if (!firstmdl)
					{
						forcebreak = true;
						break;
					}
					firstmdl = false;
					ReadStream(&model, sizeof(model));
					cbinfo.reservevertices = model.vertices;
					cbinfo.reserveindices = model.triangles;
					result = surface->LdrOnMeshStart(&cbinfo);
					if (result != E_OK)
						return(result);
					break;
				case E3DCH_EVTX:
					cbinfo.streamdata = streamdat;
					while(segment_size)
					{
						cbinfo.streamsize = segment_size;
						if (cbinfo.streamsize > E3DF_MAXSTREAMDAT)
							cbinfo.streamsize = E3DF_MAXSTREAMDAT / 12 * 12;
						ReadStream(tmpstream, cbinfo.streamsize);
						for (ctr = 0; ctr < cbinfo.streamsize / 12; ctr++)
						{
							streamdat32[ctr * 3] = tmpstream32[ctr * 3];
							streamdat32[ctr * 3 + 1] = tmpstream32[ctr * 3 + 1];
							streamdat32[ctr * 3 + 2] = tmpstream32[ctr * 3 + 2];
							if (modelsize < 0)
							{
								streamdat32[ctr * 3] >>= -modelsize;
								streamdat32[ctr * 3 + 1] >>= -modelsize;
								streamdat32[ctr * 3 + 2] >>= -modelsize;
							}
							else if (modelsize > 0)
							{
								streamdat32[ctr * 3] <<= modelsize;
								streamdat32[ctr * 3 + 1] <<= modelsize;
								streamdat32[ctr * 3 + 2] <<= modelsize;
							}
						}
						result = surface->LdrOnVertexStream(&cbinfo);
						if (result != E_OK)
							return(result);
						if (cbinfo.streamsize > segment_size)
							break;
						segment_size -= cbinfo.streamsize;
					}
					break;
				case E3DCH_ETRI:
					cbinfo.streamdata = streamdat;
					while(segment_size)
					{
						cbinfo.streamsize = segment_size * 16 / 6;
						if (cbinfo.streamsize > E3DF_MAXSTREAMDAT)
							cbinfo.streamsize = E3DF_MAXSTREAMDAT / 16 * 16;
						ReadStream(tmpstream, cbinfo.streamsize / 16 * 6);
						for (ctr = 0; ctr < cbinfo.streamsize / 16; ctr++)
						{
							streamdat32[ctr * 4] = tmpstream16[ctr * 3];
							streamdat32[ctr * 4 + 1] = tmpstream16[ctr * 3 + 1];
							streamdat32[ctr * 4 + 2] = tmpstream16[ctr * 3 + 2];
							streamdat32[ctr * 4 + 3] = E_RGBA(255, 255, 255, 255);
						}
						result = surface->LdrOnIndexStream(&cbinfo);
						if (result != E_OK)
							return(result);
						if (cbinfo.streamsize / 16 * 6 > segment_size)
							break;
						segment_size -= cbinfo.streamsize / 16 * 6;
					}
					break;
				case E3DCH_eCOL:
					cbinfo.streamdata = streamdat;
					while(segment_size)
					{
						cbinfo.streamsize = segment_size;
						if (cbinfo.streamsize > E3DF_MAXSTREAMDAT)
							cbinfo.streamsize = E3DF_MAXSTREAMDAT / 4 * 4;
						ReadStream(streamdat32, cbinfo.streamsize);
						result = surface->LdrOnColorStream(&cbinfo);
						if (result != E_OK)
							return(result);
						if (cbinfo.streamsize > segment_size)
							break;
						segment_size -= cbinfo.streamsize;
					}
					break;
				case E3DCH_eNRM:
					cbinfo.streamdata = streamdat;
					while(segment_size)
					{
						cbinfo.streamsize = segment_size * 12 / 3;
						if (cbinfo.streamsize > E3DF_MAXSTREAMDAT)
							cbinfo.streamsize = E3DF_MAXSTREAMDAT / 12 * 12;
						ReadStream(tmpstream8, cbinfo.streamsize * 3 / 12);
						for (ctr = 0; ctr < cbinfo.streamsize / 12; ctr++)
						{
							streamdat32[ctr * 3] = tmpstream8[ctr * 3] * 256;
							streamdat32[ctr * 3 + 1] = tmpstream8[ctr * 3 + 1] * 256;
							streamdat32[ctr * 3 + 2] = tmpstream8[ctr * 3 + 2] * 256;
						}
						result = surface->LdrOnNormalStream(&cbinfo);
						if (result != E_OK)
							return(result);
						if (cbinfo.streamsize * 3 / 12 > segment_size)
							break;
						segment_size -= cbinfo.streamsize * 3 / 12;
					}
					break;
				case E3DCH_eTXC:
					cbinfo.streamdata = streamdat;
					while(segment_size)
					{
						cbinfo.streamsize = segment_size;
						if (cbinfo.streamsize > E3DF_MAXSTREAMDAT)
							cbinfo.streamsize = E3DF_MAXSTREAMDAT / 8 * 8;
						ReadStream(tmpstream32, cbinfo.streamsize);
						for (ctr = 0; ctr < cbinfo.streamsize / 8; ctr++)
						{
							streamdat32[ctr * 2] = tmpstream32[ctr * 2];
							streamdat32[ctr * 2 + 1] = tmpstream32[ctr * 2 + 1];
						}
						result = surface->LdrOnTexcoordStream(&cbinfo);
						if (result != E_OK)
							return(result);
						if (cbinfo.streamsize > segment_size)
							break;
						segment_size -= cbinfo.streamsize;
					}
					break;
				case E3DCH_END:
					forcebreak = true;
					break;
			}
			SeekRelative(segment_size_bak - (Tell() - prevpos));
		}
		while(HaveData() && !forcebreak);
		if (result == E_OK)
			surface->LdrOnMeshEnd(&cbinfo);
		return(result);
	#endif
}
