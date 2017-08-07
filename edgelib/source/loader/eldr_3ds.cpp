// eldr_3ds.cpp
// 3DS file loader
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

#define E3DS_EDIT          0x3D3D
#define E3DS_MAINSIG       0x4D4D
#define E3DS_OBJECT        0x4000
#define E3DS_TRIMESH       0x4100
#define E3DS_TRIVTXL       0x4110
#define E3DS_TRIVTXOPTION  0x4111
#define E3DS_TRIFACEL      0x4120
#define E3DS_TRIMATERIAL   0x4130
#define E3DS_TRIMAPCOORDS  0x4140
#define E3DS_TRISMOOTH     0x4150
#define E3DS_TRILOCAL      0x4160
#define E3DS_TRIVISIBLE    0x4165
#define E3DS_TRIMAPSTD     0x4170
#define E3DS_MATERIAL      0xAFFF
#define E3DS_KEYFRAME      0xB000

#define L3DS_MAXSCANMAT        16
#define L3DS_MAXSCANOBJ        32
#define L3DS_MAXMATLIST        16
#define L3DS_MAXSTREAMDAT    1024

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

//The 3DS header
typedef struct
{
	unsigned long totalsize;
	unsigned long info1;
	unsigned long info2;
	unsigned long numobjects;
	unsigned short sig;
	unsigned short info3;
}E3DS_HEADER;

//Object material information
typedef struct
{
	char name[32];
	COLORREF color;
	bool ifound;
}E3DS_MATINFO;

//Object material list
typedef struct
{
	unsigned char refermat;
	unsigned long liststart;
	unsigned short listsize;
}E3DS_MATLIST;

//Object information
typedef struct
{
	E3DS_MATLIST material[L3DS_MAXMATLIST];
	char name[32];
	unsigned short nummatlist;
	unsigned short numpoints;
	unsigned short numpoly;
	unsigned long dptr_vertex;
	unsigned long dptr_poly;
	unsigned long dptr_texel;
	bool ifound;
}E3DS_OBJINFO;

//A vertex point in 3D space
typedef struct
{
	long x;
	long y;
	long z;
	long u;
	long v;
}E3DS_VERTEX;

//A polygon triangle in 3D space
typedef struct
{
	unsigned short v1;
	unsigned short v2;
	unsigned short v3;
	unsigned short ex;
}E3DS_POLYGON;

///////////////////////////////////////////////////////////////////
// Loader: Private                                               //
///////////////////////////////////////////////////////////////////

//Load edit data
void ELoader_3DS::LoadObjectData(void *objarray, void *matarray)
{
	E3DS_OBJINFO *objinfo = (E3DS_OBJINFO *)objarray;
	E3DS_MATINFO *matinfo = (E3DS_MATINFO *)matarray;
	bool segmentreading = true;
	unsigned long segment_size, startpos, dummy32;
	unsigned short segment_id, dummy16;
	unsigned char curobj = 0, curmat = 0, dummy8;
	while(segmentreading && HaveData())
	{
		startpos = Tell();
		Read16(&segment_id);
		Read32(&segment_size);
		switch(segment_id)
		{
			case E3DS_MATERIAL:
				if (curmat < L3DS_MAXSCANMAT)
				{
					unsigned long ctr = 0;
					unsigned char namebyte = 1;
					Read16(&dummy16);
					Read32(&dummy32);
					while (namebyte != 0)
					{
						Read8(&namebyte);
						if (ctr < 30)
						{
							matinfo[curmat].name[ctr] = namebyte;
							matinfo[curmat].name[ctr + 1] = 0;
							ctr++;
						}
					}
					Read32(&dummy32);
					Read32(&dummy32);
					Read32(&dummy32);
					matinfo[curmat].color = E_RGBA(0, 0, 0, 255);
					Read8(&dummy8);
					matinfo[curmat].color |= dummy8;
					Read8(&dummy8);
					matinfo[curmat].color |= dummy8 << 8;
					Read8(&dummy8);
					matinfo[curmat].color |= dummy8 << 16;
					matinfo[curmat].ifound = true;
					curmat++;
				}
				break;
			case E3DS_OBJECT:
				if (curobj < L3DS_MAXSCANOBJ)
				{
					unsigned long ctr = 0;
					unsigned char namebyte = 1;
					while (namebyte != 0)
					{
						Read8(&namebyte);
						if (ctr < 30)
						{
							objinfo[curobj].name[ctr] = namebyte;
							objinfo[curobj].name[ctr + 1] = 0;
							ctr++;
						}
					}
					objinfo[curobj].ifound = true;
					LoadObjectInfo(&objinfo[curobj], matarray);
				}
				curobj++;
				break;
		}
		SeekSet(startpos + segment_size);
	}
}

//Load all object information
void ELoader_3DS::LoadObjectInfo(void *objinfo, void *matarray)
{
	E3DS_OBJINFO *info = (E3DS_OBJINFO *)objinfo;
	bool segmentreading = true;
	unsigned long segment_size, startpos;
	unsigned short segment_id;
	while(segmentreading && HaveData())
	{
		startpos = Tell();
		Read16(&segment_id);
		Read32(&segment_size);
		switch(segment_id)
		{
			case E3DS_TRIMESH:
				LoadObjectMeshInfo(info, matarray);
				break;
		}
		SeekSet(startpos + segment_size);
	}
}

//Load object mesh info and data pointers
void ELoader_3DS::LoadObjectMeshInfo(void *objinfo, void *matarray)
{
	E3DS_OBJINFO *info = (E3DS_OBJINFO *)objinfo;
	E3DS_MATINFO *matinfo = (E3DS_MATINFO *)matarray;
	char matname[32];
	bool segmentreading = true;
	unsigned long ctr, segment_size, startpos;
	unsigned short segment_id, dummy;
	while(segmentreading && HaveData())
	{
		startpos = Tell();
		Read16(&segment_id);
		Read32(&segment_size);
		switch(segment_id)
		{
			case E3DS_TRIVTXL:
				Read16(&info->numpoints);
				info->dptr_vertex = Tell();
				break;
			case E3DS_TRIMAPCOORDS:
				Read16(&dummy);
				info->dptr_texel = Tell();
				break;
			case E3DS_TRILOCAL:
				//load translation info
				break;
			case E3DS_TRIFACEL:
				Read16(&info->numpoly);
				info->dptr_poly = Tell();
				segment_size = info->numpoly * 2 * 4 + 2 + 6;
				break;
			case E3DS_TRIMATERIAL:
				{
					unsigned char namebyte = 1;
					ctr = 0;
					while (namebyte != 0)
					{
						Read8(&namebyte);
						if (ctr < 30)
						{
							matname[ctr] = namebyte;
							matname[ctr + 1] = 0;
							ctr++;
						}
					}
				}
				if (info->nummatlist < L3DS_MAXMATLIST)
				{
					for (ctr = 0; ctr < L3DS_MAXSCANMAT; ctr++)
						if (matinfo[ctr].ifound)
							if (ClassEStd::StrEqual(matinfo[ctr].name, matname))
							{
								Read16(&info->material[info->nummatlist].listsize);
								info->material[info->nummatlist].liststart = Tell();
								info->material[info->nummatlist].refermat = (unsigned char)ctr;
								info->nummatlist++;
								break;
							}
				}
				break;
		}
		SeekSet(startpos + segment_size);
	}
}

///////////////////////////////////////////////////////////////////
// Loader: Public                                                //
///////////////////////////////////////////////////////////////////

//Start the loader
ERESULT ELoader_3DS::Start(E3DLoaderBase *surface, void *ldata, unsigned long lsize, signed char modelsize, unsigned long createflags)
{
	#if (EDITION_SDK < 1000)
		return(E_NOFEATURE);
	#else
		ERESULT result = E_OK;
		ELDR_3DCALLBACKINFO cbinfo;
		E3DS_HEADER header;
		E3DS_OBJINFO objinfo[L3DS_MAXSCANOBJ];
		E3DS_MATINFO matinfo[L3DS_MAXSCANMAT];
		unsigned char streamdat[L3DS_MAXSTREAMDAT];
		unsigned char tmpstream[L3DS_MAXSTREAMDAT];
		bool segmentreading = true;
		unsigned long segment_size, startpos, indexobjinfo;
		unsigned short segment_id;
		EINT32 *streamdat32 = (EINT32 *)streamdat;
		unsigned EINT32 *tmpstream32 = (unsigned EINT32 *)tmpstream;
		//unsigned short *streamdat16 = (unsigned short *)streamdat;
		unsigned short *tmpstream16 = (unsigned short *)tmpstream;
		LinkData(ldata, lsize);
		cbinfo.createflags = createflags;
		cbinfo.vertexindex = 0;
		cbinfo.polygonindex = 0;
		cbinfo.texelindex = 0;
		cbinfo.texelonvertex = true;
		cbinfo.coloronvertex = false;
		Read16(&header.sig);
		if (header.sig != E3DS_MAINSIG)
			return(E_UNSUPPORTED);
		Read32(&header.totalsize);
		Read32(&header.info1);
		Read32(&header.info2);
		Read16(&header.info3);
		for (indexobjinfo = 0; indexobjinfo < L3DS_MAXSCANOBJ; indexobjinfo++)
		{
			objinfo[indexobjinfo].dptr_poly = 0;
			objinfo[indexobjinfo].dptr_texel = 0;
			objinfo[indexobjinfo].dptr_vertex = 0;
			objinfo[indexobjinfo].numpoints = 0;
			objinfo[indexobjinfo].numpoly = 0;
			objinfo[indexobjinfo].nummatlist = 0;
			objinfo[indexobjinfo].ifound = false;
		}
		for (indexobjinfo = 0; indexobjinfo < L3DS_MAXSCANMAT; indexobjinfo++)
			matinfo[indexobjinfo].ifound = false;
		while(segmentreading && HaveData())
		{
			startpos = Tell();
			Read16(&segment_id);
			Read32(&segment_size);
			switch(segment_id)
			{
				case E3DS_EDIT:
					LoadObjectData(objinfo, matinfo);
					break;
				case E3DS_KEYFRAME:
					//start loading keyframes
					//ClassEConsole::MsgWindow(segment_size, "Keyframes found");
					break;
			}
			SeekSet(startpos + segment_size);
		}
		unsigned long ctr, idxctr, totalvertex = 0, totalpoly = 0;
		for (ctr = 0; ctr < L3DS_MAXSCANOBJ; ctr++)
		{
			totalvertex += objinfo[ctr].numpoints;
			totalpoly += objinfo[ctr].numpoly;
		}
		cbinfo.reservevertices = totalvertex;
		cbinfo.reserveindices = totalpoly;
		result = surface->LdrOnMeshStart(&cbinfo);
		if (result != E_OK)
			return(result);
		for (indexobjinfo = 0; indexobjinfo < L3DS_MAXSCANOBJ; indexobjinfo++)
		{
			if (objinfo[indexobjinfo].dptr_vertex > 0)
			{
				unsigned short prevpoints = objinfo[indexobjinfo].numpoints;
				SeekSet(objinfo[indexobjinfo].dptr_vertex);
				cbinfo.streamdata = streamdat;
				while(objinfo[indexobjinfo].numpoints)
				{
					cbinfo.streamsize = objinfo[indexobjinfo].numpoints * 12;
					if (cbinfo.streamsize > L3DS_MAXSTREAMDAT)
						cbinfo.streamsize = L3DS_MAXSTREAMDAT / 12 * 12;
					ReadStream(tmpstream, cbinfo.streamsize);
					for (ctr = 0; ctr < cbinfo.streamsize / 12; ctr++)
					{
						streamdat32[ctr * 3] = ClassEMath::FloatToFixed(tmpstream32[ctr * 3], 12);
						streamdat32[ctr * 3 + 1] = ClassEMath::FloatToFixed(tmpstream32[ctr * 3 + 1], 12);
						streamdat32[ctr * 3 + 2] = ClassEMath::FloatToFixed(tmpstream32[ctr * 3 + 2], 12);
						if ((streamdat32[ctr * 3] & 4095) == 4095)
							streamdat32[ctr * 3]++;
						if ((streamdat32[ctr * 3 + 1] & 4095) == 4095)
							streamdat32[ctr * 3 + 1]++;
						if ((streamdat32[ctr * 3 + 2] & 4095) == 4095)
							streamdat32[ctr * 3 + 2]++;
						if ((streamdat32[ctr * 3] & 4095) == 1)
							streamdat32[ctr * 3]--;
						if ((streamdat32[ctr * 3 + 1] & 4095) == 1)
							streamdat32[ctr * 3 + 1]--;
						if ((streamdat32[ctr * 3 + 2] & 4095) == 1)
							streamdat32[ctr * 3 + 2]--;
						streamdat32[ctr * 3] >>= 6 - modelsize;
						streamdat32[ctr * 3 + 1] >>= 6 - modelsize;
						streamdat32[ctr * 3 + 2] >>= 6 - modelsize;
					}
					result = surface->LdrOnVertexStream(&cbinfo);
					if (result != E_OK)
						return(result);
					objinfo[indexobjinfo].numpoints -= (unsigned short)(cbinfo.streamsize / 12);
				}
				objinfo[indexobjinfo].numpoints = prevpoints;
			}
			if (objinfo[indexobjinfo].dptr_poly > 0)
			{
				SeekSet(objinfo[indexobjinfo].dptr_poly);
				cbinfo.streamdata = streamdat;
				while(objinfo[indexobjinfo].numpoly)
				{
					cbinfo.streamsize = objinfo[indexobjinfo].numpoly * 16;
					if (cbinfo.streamsize > L3DS_MAXSTREAMDAT)
						cbinfo.streamsize = L3DS_MAXSTREAMDAT / 16 * 16;
					ReadStream(tmpstream, cbinfo.streamsize / 4 * 2);
					for (ctr = 0; ctr < cbinfo.streamsize / 16; ctr++)
					{
						streamdat32[ctr * 4] = tmpstream16[ctr * 4 + 0];
						streamdat32[ctr * 4 + 1] = tmpstream16[ctr * 4 + 1];
						streamdat32[ctr * 4 + 2] = tmpstream16[ctr * 4 + 2];
						streamdat32[ctr * 4 + 3] = E_RGBX(255, 255, 255);
					}
					result = surface->LdrOnIndexStream(&cbinfo);
					if (result != E_OK)
						return(result);
					objinfo[indexobjinfo].numpoly -= (unsigned short)(cbinfo.streamsize / 16);
				}
			}
			if (objinfo[indexobjinfo].dptr_texel > 0)
			{
				SeekSet(objinfo[indexobjinfo].dptr_texel);
				cbinfo.streamdata = streamdat;
				while(objinfo[indexobjinfo].numpoints)
				{
					cbinfo.streamsize = objinfo[indexobjinfo].numpoints * 8;
					if (cbinfo.streamsize > L3DS_MAXSTREAMDAT)
						cbinfo.streamsize = L3DS_MAXSTREAMDAT / 8 * 8;
					ReadStream(tmpstream, cbinfo.streamsize);
					for (ctr = 0; ctr < cbinfo.streamsize / 8; ctr++)
					{
						streamdat32[ctr * 2] = ClassEMath::FloatToFixed(tmpstream32[ctr * 2], 16) + 65536;
						streamdat32[ctr * 2 + 1] = ClassEMath::FloatToFixed(tmpstream32[ctr * 2 + 1], 16) + 65536;
						if ((streamdat32[ctr * 2] & 65535) == 65535)
							streamdat32[ctr * 2]++;
						if ((streamdat32[ctr * 2 + 1] & 65535) == 65535)
							streamdat32[ctr * 2 + 1]++;
						if ((streamdat32[ctr * 2] & 65535) == 1)
							streamdat32[ctr * 2]--;
						if ((streamdat32[ctr * 2 + 1] & 65535) == 1)
							streamdat32[ctr * 2 + 1]--;
					}
					result = surface->LdrOnTexcoordStream(&cbinfo);
					if (result != E_OK)
						return(result);
					objinfo[indexobjinfo].numpoints -= (unsigned short)(cbinfo.streamsize / 8);
				}
			}
			for (idxctr = 0; idxctr < objinfo[indexobjinfo].nummatlist; idxctr++)
			{
				E3DS_MATINFO *curmatinfo = &matinfo[objinfo[indexobjinfo].material[idxctr].refermat];
				unsigned long tmpstreamsize, tlistsize, tlastpos;
				SeekSet(objinfo[indexobjinfo].material[idxctr].liststart);
				cbinfo.streamdata = streamdat;
				while(objinfo[indexobjinfo].material[idxctr].listsize)
				{
					cbinfo.streamsize = objinfo[indexobjinfo].material[idxctr].listsize * 4;
					if (cbinfo.streamsize > L3DS_MAXSTREAMDAT)
						cbinfo.streamsize = L3DS_MAXSTREAMDAT / 4 * 4;
					ReadStream(tmpstream, cbinfo.streamsize / 4 * 2);
					tmpstreamsize = cbinfo.streamsize;
					cbinfo.colorindex = tmpstream16[0];
					tlistsize = 0;
					tlastpos = 0;
					for (ctr = 0; ctr < tmpstreamsize / 4; ctr++)
					{
						bool listbreak = false;
						streamdat32[ctr] = curmatinfo->color;
						if (ctr == tmpstreamsize / 4 - 1)
							listbreak = true;
						else if (tmpstream16[ctr] != tmpstream16[ctr + 1] - 1)
							listbreak = true;
						tlistsize++;
						if (listbreak)
						{
							cbinfo.streamsize = tlistsize * 4;
							cbinfo.streamdata = &streamdat32[tlastpos];
							result = surface->LdrOnColorStream(&cbinfo);
							if (result != E_OK)
								return(result);
							if (ctr < tmpstreamsize + 4 - 1)
								cbinfo.colorindex = tmpstream16[ctr + 1];
							tlistsize = 0;
							tlastpos = ctr + 1;
						}
					}
					objinfo[indexobjinfo].material[idxctr].listsize -= (unsigned short)(tmpstreamsize / 4);
				}
			}
		}
		if (result == E_OK)
			surface->LdrOnMeshEnd(&cbinfo);
		return(result);
	#endif
}
