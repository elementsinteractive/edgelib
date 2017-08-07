// eldr_ms3d.cpp
// Milkshape 3D file loader
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

#include "extra/verinfo.h"


///////////////////////////////////////////////////////////////////
// Type definitions                                              //
///////////////////////////////////////////////////////////////////

#define LMS3D_MAXOBJLIST       32
#define LMS3D_MAXMATLIST       64
#define LMS3D_MAXJOINTLIST    128
#define LMS3D_MAXSTREAMDAT   1024

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

//The MS3D header
typedef struct
{
	char sig[12];
	long version;
	long fps;
	long currenttime;
	long totalframes;
	unsigned long dptr_vertex;
	unsigned long dptr_triangle;
	unsigned long dptr_material;
	unsigned long dptr_joints;
	unsigned short countvertex;
	unsigned short counttriangle;
	unsigned short countmaterial;
	unsigned short countgroup;
	unsigned short countjoints;
}EMS3D_HEADER;

//Milkshape group structure
typedef struct
{
	COLORREF col;
	char name[32];
	unsigned long dptr;
	unsigned short trianglecount;
	unsigned char material;
	unsigned char flags;
}EMS3D_GROUP;

//Milkshape joint structure
typedef struct
{
	char name[32];
	char parentname[32];
	long position[3];
	long rotation[3];
	unsigned short countpos;
	unsigned short countrot;
	unsigned long dptr_animdata;
}EMS3D_JOINT;

///////////////////////////////////////////////////////////////////
// Loader: Public                                                //
///////////////////////////////////////////////////////////////////

//Convert unaligned to short
short ELoader_MS3D::ConvShort(unsigned char *ptr)
{
	short v;
	unsigned char *lptr = (unsigned char *)&v;
	lptr[0] = ptr[0];
	lptr[1] = ptr[1];
	return(v);
}

//Convert unaligned to long
long ELoader_MS3D::ConvLong(unsigned char *ptr)
{
	long v;
	unsigned char *lptr = (unsigned char *)&v;
	lptr[0] = ptr[0];
	lptr[1] = ptr[1];
	lptr[2] = ptr[2];
	lptr[3] = ptr[3];
	return(v);
}

//Start the loader
ERESULT ELoader_MS3D::Start(E3DLoaderBase *surface, void *ldata, unsigned long lsize, signed char modelsize, unsigned long createflags)
{
	#if (EDITION_SDK < 1000)
		return(E_NOFEATURE);
	#else
		ERESULT result = E_OK;
		ELDR_3DCALLBACKINFO cbinfo;
		EMS3D_HEADER header;
		unsigned char streamdat[LMS3D_MAXSTREAMDAT];
		unsigned char tmpstream[LMS3D_MAXSTREAMDAT];
		unsigned short *streamdat16 = (unsigned short *)streamdat;
		unsigned short *tmpstream16 = (unsigned short *)tmpstream;
		EINT32 *streamdat32 = (EINT32 *)streamdat;
		EINT32 *tmpstream32 = (EINT32 *)tmpstream;
		unsigned long orgstreamsize, groupctr, ctr;
		LinkData(ldata, lsize);
		cbinfo.createflags = createflags;
		cbinfo.vertexindex = 0;
		cbinfo.polygonindex = 0;
		cbinfo.texelindex = 0;
		cbinfo.normalindex = 0;
		cbinfo.texelonvertex = false;
		cbinfo.coloronvertex = false;
		cbinfo.normalonvertex = false;
		ReadStream(header.sig, 10);
		header.sig[10] = 0;
		Read32((unsigned long *)&header.version);
		if (!ClassEStd::StrEqual(header.sig, "MS3D000000") || header.version < 3 || header.version > 4)
			return(E_UNSUPPORTED);
		Read16(&header.countvertex);
		header.dptr_vertex = Tell();
		SeekSet(Tell() + header.countvertex * 15);
		Read16(&header.counttriangle);
		header.dptr_triangle = Tell();
		SeekSet(Tell() + header.counttriangle * 70);
		Read16(&header.countgroup);
		EMS3D_GROUP *group = new EMS3D_GROUP[LMS3D_MAXOBJLIST];
		if (group == NULL)
			return(E_NOMEMORY);
		EMS3D_JOINT *joint = new EMS3D_JOINT[LMS3D_MAXJOINTLIST];
		if (joint == NULL)
		{
			delete group;
			return(E_NOMEMORY);
		}
		for (ctr = 0; ctr < header.countgroup; ctr++)
		{
			if (ctr < LMS3D_MAXOBJLIST)
			{
				group[ctr].dptr = Tell();
				group[ctr].col = E_RGBA(255, 255, 255, 255);
				Read8(&group[ctr].flags);
				ReadStream(group[ctr].name, 32);
				Read16(&group[ctr].trianglecount);
				SeekSet(Tell() + group[ctr].trianglecount * 2);
				Read8(&group[ctr].material);
			}
			else
			{
				unsigned short cnttrg_dummy;
				SeekSet(Tell() + 33);
				Read16(&cnttrg_dummy);
				SeekSet(Tell() + cnttrg_dummy * 2 + 1);
			}
		}
		Read16(&header.countmaterial);
		header.dptr_material = Tell();
		SeekSet(Tell() + header.countmaterial * 361);
		Read32((unsigned long *)&header.fps);
		header.fps = ClassEMath::FloatToFixed(header.fps, 1);
		Read32((unsigned long *)&header.currenttime);
		header.currenttime = ClassEMath::FloatToFixed(header.currenttime, 0);
		Read32((unsigned long *)&header.totalframes);
		Read16(&header.countjoints);
		header.dptr_joints = Tell();
		cbinfo.reservevertices = header.countvertex;
		cbinfo.reserveindices = header.counttriangle;
		result = surface->LdrOnMeshStart(&cbinfo);
		if (result != E_OK)
		{
			delete group;
			delete joint;
			return(result);
		}
		surface->CreateJoints(header.countjoints);
		SeekSet(header.dptr_vertex);
		cbinfo.streamdata = streamdat;
		unsigned long jointidctr = 0;
		while(HaveData() && header.countvertex)
		{
			cbinfo.streamsize = header.countvertex * 15;
			if (cbinfo.streamsize > LMS3D_MAXSTREAMDAT)
				cbinfo.streamsize = LMS3D_MAXSTREAMDAT / 15 * 15;
			ReadStream(tmpstream, cbinfo.streamsize);
			orgstreamsize = cbinfo.streamsize;
			cbinfo.streamsize = orgstreamsize / 15 * 4;
			for (ctr = 0; ctr < cbinfo.streamsize / 4; ctr++)
			{
				unsigned char *tjntstream = (unsigned char *)&tmpstream[ctr * 15 + 13];
				streamdat16[ctr * 2] = (unsigned short)(jointidctr + ctr);
				streamdat16[ctr * 2 + 1] = *tjntstream;
			}
			if (header.countjoints > 0)
			{
				result = surface->LdrOnJointStream(&cbinfo);
				jointidctr += cbinfo.streamsize / 4;
				if (result != E_OK)
				{
					delete group;
					delete joint;
					return(result);
				}
			}
			cbinfo.streamsize = orgstreamsize / 15 * 12;
			for (ctr = 0; ctr < cbinfo.streamsize / 12; ctr++)
			{
				unsigned char *tvtxstream = (unsigned char *)&tmpstream[ctr * 15 + 1];
				streamdat32[ctr * 3] = ClassEMath::FloatToFixed(ConvLong(&tvtxstream[0]), 12);
				streamdat32[ctr * 3 + 1] = ClassEMath::FloatToFixed(ConvLong(&tvtxstream[4]), 12);
				streamdat32[ctr * 3 + 2] = ClassEMath::FloatToFixed(ConvLong(&tvtxstream[8]), 12);
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
			{
				delete group;
				delete joint;
				return(result);
			}
			header.countvertex -= (unsigned short)(orgstreamsize / 15);
		}
		SeekSet(header.dptr_triangle);
		while(HaveData() && header.counttriangle)
		{
			cbinfo.streamsize = header.counttriangle * 70;
			if (cbinfo.streamsize > LMS3D_MAXSTREAMDAT)
				cbinfo.streamsize = LMS3D_MAXSTREAMDAT / 70 * 70;
			ReadStream(tmpstream, cbinfo.streamsize);
			orgstreamsize = cbinfo.streamsize;
			for (ctr = 0; ctr < orgstreamsize / 70; ctr++)
			{
				unsigned short *ttrgstream = (unsigned short *)&tmpstream[ctr * 70 + 2];
				streamdat32[ctr * 4] = ttrgstream[0];
				streamdat32[ctr * 4 + 1] = ttrgstream[1];
				streamdat32[ctr * 4 + 2] = ttrgstream[2];
				streamdat32[ctr * 4 + 3] = E_RGBX(255, 255, 255);
			}
			cbinfo.streamsize = orgstreamsize / 70 * 16;
			result = surface->LdrOnIndexStream(&cbinfo);
			if (result != E_OK)
			{
				delete group;
				delete joint;
				return(result);
			}
			for (ctr = 0; ctr < orgstreamsize / 70; ctr++)
			{
				unsigned char *tnmstream = (unsigned char *)&tmpstream[ctr * 70 + 8];
				streamdat32[ctr * 9] = ClassEMath::FloatToFixed(ConvLong(&tnmstream[0]), 16);
				streamdat32[ctr * 9 + 1] = ClassEMath::FloatToFixed(ConvLong(&tnmstream[4]), 16);
				streamdat32[ctr * 9 + 2] = ClassEMath::FloatToFixed(ConvLong(&tnmstream[8]), 16);
				streamdat32[ctr * 9 + 3] = ClassEMath::FloatToFixed(ConvLong(&tnmstream[12]), 16);
				streamdat32[ctr * 9 + 4] = ClassEMath::FloatToFixed(ConvLong(&tnmstream[16]), 16);
				streamdat32[ctr * 9 + 5] = ClassEMath::FloatToFixed(ConvLong(&tnmstream[20]), 16);
				streamdat32[ctr * 9 + 6] = ClassEMath::FloatToFixed(ConvLong(&tnmstream[24]), 16);
				streamdat32[ctr * 9 + 7] = ClassEMath::FloatToFixed(ConvLong(&tnmstream[28]), 16);
				streamdat32[ctr * 9 + 8] = ClassEMath::FloatToFixed(ConvLong(&tnmstream[32]), 16);
			}
			cbinfo.streamsize = orgstreamsize / 70 * 36;
			result = surface->LdrOnNormalStream(&cbinfo);
			if (result != E_OK)
			{
				delete group;
				delete joint;
				return(result);
			}
			for (ctr = 0; ctr < orgstreamsize / 70; ctr++)
			{
				unsigned char *tuvstream = (unsigned char *)&tmpstream[ctr * 70 + 44];
				streamdat32[ctr * 6] = ClassEMath::FloatToFixed(ConvLong(&tuvstream[0]), 16) + 65536;
				streamdat32[ctr * 6 + 1] = -ClassEMath::FloatToFixed(ConvLong(&tuvstream[12]), 16) + 65536;
				streamdat32[ctr * 6 + 2] = ClassEMath::FloatToFixed(ConvLong(&tuvstream[4]), 16) + 65536;
				streamdat32[ctr * 6 + 3] = -ClassEMath::FloatToFixed(ConvLong(&tuvstream[16]), 16) + 65536;
				streamdat32[ctr * 6 + 4] = ClassEMath::FloatToFixed(ConvLong(&tuvstream[8]), 16) + 65536;
				streamdat32[ctr * 6 + 5] = -ClassEMath::FloatToFixed(ConvLong(&tuvstream[20]), 16) + 65536;
				if ((streamdat32[ctr * 6] & 65535) == 65535)
					streamdat32[ctr * 6]++;
				if ((streamdat32[ctr * 6 + 1] & 65535) == 65535)
					streamdat32[ctr * 6 + 1]++;
				if ((streamdat32[ctr * 6 + 2] & 65535) == 65535)
					streamdat32[ctr * 6 + 2]++;
				if ((streamdat32[ctr * 6 + 3] & 65535) == 65535)
					streamdat32[ctr * 6 + 3]++;
				if ((streamdat32[ctr * 6 + 4] & 65535) == 65535)
					streamdat32[ctr * 6 + 4]++;
				if ((streamdat32[ctr * 6 + 5] & 65535) == 65535)
					streamdat32[ctr * 6 + 5]++;
				if ((streamdat32[ctr * 6] & 65535) == 1)
					streamdat32[ctr * 6]--;
				if ((streamdat32[ctr * 6 + 1] & 65535) == 1)
					streamdat32[ctr * 6 + 1]--;
				if ((streamdat32[ctr * 6 + 2] & 65535) == 1)
					streamdat32[ctr * 6 + 2]--;
				if ((streamdat32[ctr * 6 + 3] & 65535) == 1)
					streamdat32[ctr * 6 + 3]--;
				if ((streamdat32[ctr * 6 + 4] & 65535) == 1)
					streamdat32[ctr * 6 + 4]--;
				if ((streamdat32[ctr * 6 + 5] & 65535) == 1)
					streamdat32[ctr * 6 + 5]--;
			}
			cbinfo.streamsize = orgstreamsize / 70 * 24;
			result = surface->LdrOnTexcoordStream(&cbinfo);
			if (result != E_OK)
			{
				delete group;
				delete joint;
				return(result);
			}
			header.counttriangle -= (unsigned short)(orgstreamsize / 70);
		}
		for (ctr = 0; ctr < header.countmaterial; ctr++)
		{
			if (ctr < LMS3D_MAXMATLIST)
			{
				COLORREF matcol;
				long ambientc[4];
				unsigned char chctr;
				SeekSet(header.dptr_material + ctr * 361);
				SeekSet(Tell() + 32);
				ReadStream(ambientc, 16);
				for (chctr = 0; chctr < 4; chctr++)
				{
					ambientc[chctr] = ClassEMath::FloatToFixed(ambientc[chctr], 8);
					if (ambientc[chctr] > 255)
						ambientc[chctr] = 255;
				}
				matcol = E_RGBA(ambientc[0], ambientc[1], ambientc[2], ambientc[3]);
				for (groupctr = 0; groupctr < header.countgroup && groupctr < LMS3D_MAXOBJLIST; groupctr++)
					if (group[groupctr].material == ctr)
						group[groupctr].col = matcol;
			}
		}
		for (groupctr = 0; groupctr < header.countgroup && groupctr < LMS3D_MAXOBJLIST; groupctr++)
		{
			unsigned long tmpstreamsize, tlistsize, tlastpos;
			SeekSet(group[groupctr].dptr + 35);
			while(group[groupctr].trianglecount)
			{
				cbinfo.streamsize = group[groupctr].trianglecount * 4;
				if (cbinfo.streamsize > LMS3D_MAXSTREAMDAT)
					cbinfo.streamsize = LMS3D_MAXSTREAMDAT / 4 * 4;
				ReadStream(tmpstream, cbinfo.streamsize / 4 * 2);
				tmpstreamsize = cbinfo.streamsize;
				cbinfo.colorindex = tmpstream16[0];
				tlistsize = 0;
				tlastpos = 0;
				for (ctr = 0; ctr < tmpstreamsize / 4; ctr++)
				{
					bool listbreak = false;
					streamdat32[ctr] = group[groupctr].col;
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
						{
							delete group;
							delete joint;
							return(result);
						}
						if (ctr < tmpstreamsize + 4 - 1)
							cbinfo.colorindex = tmpstream16[ctr + 1];
						tlistsize = 0;
						tlastpos = ctr + 1;
					}
				}
				group[groupctr].trianglecount -= (unsigned short)(tmpstreamsize / 4);
			}
		}
		if (header.countjoints > LMS3D_MAXJOINTLIST)
			header.countjoints = LMS3D_MAXJOINTLIST;
		SeekSet(header.dptr_joints);
		for (groupctr = 0; groupctr < header.countjoints; groupctr++)
		{
			SeekSet(Tell() + 1);
			ReadStream(joint[groupctr].name, 32);
			ReadStream(joint[groupctr].parentname, 32);
			for (ctr = 0; ctr < 3; ctr++)
			{
				Read32((unsigned long *)&joint[groupctr].rotation[ctr]);
				joint[groupctr].rotation[ctr] = ClassEMath::FloatToFixed(joint[groupctr].rotation[ctr], 12) * 100 / 628;
			}
			ReadStream(streamdat32, 12);
			joint[groupctr].position[0] = ClassEMath::FloatToFixed(streamdat32[0], 6 + modelsize);
			joint[groupctr].position[1] = ClassEMath::FloatToFixed(streamdat32[1], 6 + modelsize);
			joint[groupctr].position[2] = ClassEMath::FloatToFixed(streamdat32[2], 6 + modelsize);
			Read16(&joint[groupctr].countrot);
			Read16(&joint[groupctr].countpos);
			joint[groupctr].dptr_animdata = Tell();
			SeekSet(Tell() + joint[groupctr].countrot * 16 + joint[groupctr].countpos * 16);
		}
		for (groupctr = 0; groupctr < header.countjoints; groupctr++)
		{
			long parent = -1;
			for (ctr = 0; ctr < header.countjoints; ctr++)
				if (ctr != groupctr)
					if (ClassEStd::StrEqual(joint[groupctr].parentname, joint[ctr].name))
					{
						parent = ctr;
						break;
					}
			surface->SetJointProperties(groupctr, (EVECTOR3D *)joint[groupctr].position, (EVECTOR3D *)joint[groupctr].rotation, parent);
			surface->CreateJointKeyFrames(groupctr, joint[groupctr].countpos, joint[groupctr].countrot);
			SeekSet(joint[groupctr].dptr_animdata);
			cbinfo.jointindex = groupctr;
			cbinfo.keyframeindex = 0;
			cbinfo.keyframetype = 1;
			while (joint[groupctr].countrot)
			{
				cbinfo.streamsize = joint[groupctr].countrot * 16;
				if (cbinfo.streamsize > LMS3D_MAXSTREAMDAT)
					cbinfo.streamsize = LMS3D_MAXSTREAMDAT / 16 * 16;
				ReadStream(tmpstream, cbinfo.streamsize);
				for (ctr = 0; ctr < cbinfo.streamsize / 16; ctr++)
				{
					streamdat32[ctr * 4] = ClassEMath::FloatToFixed(tmpstream32[ctr * 4], 10) * 1000 / 1024;
					streamdat32[ctr * 4 + 1] = ClassEMath::FloatToFixed(tmpstream32[ctr * 4 + 1], 12) * 100 / 628;
					streamdat32[ctr * 4 + 2] = ClassEMath::FloatToFixed(tmpstream32[ctr * 4 + 2], 12) * 100 / 628;
					streamdat32[ctr * 4 + 3] = ClassEMath::FloatToFixed(tmpstream32[ctr * 4 + 3], 12) * 100 / 628;
				}
				result = surface->LdrOnKeyFrameStream(&cbinfo);
				if (result != E_OK)
				{
					delete group;
					delete joint;
					return(result);
				}
				joint[groupctr].countrot -= (unsigned short)(cbinfo.streamsize / 16);
			}
			cbinfo.keyframeindex = 0;
			cbinfo.keyframetype = 0;
			while (joint[groupctr].countpos)
			{
				cbinfo.streamsize = joint[groupctr].countpos * 16;
				if (cbinfo.streamsize > LMS3D_MAXSTREAMDAT)
					cbinfo.streamsize = LMS3D_MAXSTREAMDAT / 16 * 16;
				ReadStream(tmpstream, cbinfo.streamsize);
				for (ctr = 0; ctr < cbinfo.streamsize / 16; ctr++)
				{
					streamdat32[ctr * 4] = ClassEMath::FloatToFixed(tmpstream32[ctr * 4], 10) * 1000 / 1024;
					streamdat32[ctr * 4 + 1] = ClassEMath::FloatToFixed(tmpstream32[ctr * 4 + 1], 6 + modelsize);
					streamdat32[ctr * 4 + 2] = ClassEMath::FloatToFixed(tmpstream32[ctr * 4 + 2], 6 + modelsize);
					streamdat32[ctr * 4 + 3] = ClassEMath::FloatToFixed(tmpstream32[ctr * 4 + 3], 6 + modelsize);
				}
				result = surface->LdrOnKeyFrameStream(&cbinfo);
				if (result != E_OK)
				{
					delete group;
					delete joint;
					return(result);
				}
				joint[groupctr].countpos -= (unsigned short)(cbinfo.streamsize / 16);
			}
		}
		delete group;
		delete joint;
		if (result == E_OK)
			surface->LdrOnMeshEnd(&cbinfo);
		return(result);
	#endif
}
