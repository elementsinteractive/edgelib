// c_mesh.h
// Mesh container class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include <math.h>

//Container class for 3D data
class MeshContainer : public E3DLoaderBase
{
	public:
		friend class ObjectContainer;
		long *dvtx;
		long *dtex;
		unsigned short *didx;
		signed char *dnorm;
		unsigned long allocvtx;
		unsigned long alloctex;
		unsigned long allocnorm;
		unsigned long allocidx;
		bool normalcalc;
		void Optimize(void);
	public:
		MeshContainer(void);
		~MeshContainer(void);
		ERESULT LdrOnMeshStart(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnMeshEnd(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnVertexStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnNormalStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnTexcoordStream(ELDR_3DCALLBACKINFO *ldrdata);
		ERESULT LdrOnIndexStream(ELDR_3DCALLBACKINFO *ldrdata);
};

///////////////////////////////////////////////////////////////////
// Normal calculator
///////////////////////////////////////////////////////////////////

//Calculate the cross product between 2 vectors
void inline CrossProduct(long &dx, long &dy, long &dz, long v1x, long v1y, long v1z, long v2x, long v2y, long v2z)
{
	dx = v1y * v2z - v2y * v1z;
	dy = v1z * v2x - v2z * v1x;
	dz = v1x * v2y - v2x * v1y;
}

//Normalize a vector into a unit length
void inline Normalize(long &vx, long &vy, long &vz, long prec = 0)
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

void CalculateNormals(long *glvertexarray, signed char *glnormalarray, unsigned short *indexlist, unsigned long numvertex, unsigned long numpoly)
{
	unsigned long cnt, vctr, pctr, vtxstride = 3;
	unsigned long bitarr[] = { 0x40000000, 0x20000000, 0x10000000, 0x8000000, 0x4000000, 0x2000000, 0x1000000, 0x800000, 0x400000, 0x200000, 0x100000, 0x80000, 0x40000, 0x20000, 0x10000, 0x8000, 0x4000 };
	for (vctr = 0; vctr < numvertex; vctr++)
	{
		long normal[3] = { 0, 0, 0 };
		cnt = 0;
		for (pctr = 0; pctr < numpoly * 3; pctr += 3)
			if (indexlist[pctr] == vctr || indexlist[pctr + 1] == vctr || indexlist[pctr + 2] == vctr)
			{
				long tx, ty, tz;
				CrossProduct(tx, ty, tz, glvertexarray[indexlist[pctr] * vtxstride] - glvertexarray[indexlist[pctr + 2] * vtxstride], glvertexarray[indexlist[pctr] * vtxstride + 1] - glvertexarray[indexlist[pctr + 2] * vtxstride + 1], glvertexarray[indexlist[pctr] * vtxstride + 2] - glvertexarray[indexlist[pctr + 2] * vtxstride + 2], glvertexarray[indexlist[pctr + 2] * vtxstride] - glvertexarray[indexlist[pctr + 1] * vtxstride], glvertexarray[indexlist[pctr + 2] * vtxstride + 1] - glvertexarray[indexlist[pctr + 1] * vtxstride + 1], glvertexarray[indexlist[pctr + 2] * vtxstride + 2] - glvertexarray[indexlist[pctr + 1] * vtxstride + 2]);
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
			Normalize(normal[0], normal[1], normal[2], cnt);
			glnormalarray[0] = (signed char)((-normal[0] * 127) >> 14);
			glnormalarray[1] = (signed char)((-normal[1] * 127) >> 14);
			glnormalarray[2] = (signed char)((-normal[2] * 127) >> 14);
		}
		glnormalarray += 3;
	}
}

///////////////////////////////////////////////////////////////////
// Implementation
///////////////////////////////////////////////////////////////////

//Optimize model, remove full duplicates
void MeshContainer::Optimize(void)
{
	unsigned long ctr, ctr2, ctr3;
	for (ctr = 0; ctr < allocvtx; ctr++)
		for (ctr2 = ctr + 1; ctr2 < allocvtx; ctr2++)
		{
			if (dvtx[ctr * 3] != dvtx[ctr2 * 3])
				continue;
			if (dvtx[ctr * 3 + 1] != dvtx[ctr2 * 3 + 1])
				continue;
			if (dvtx[ctr * 3 + 2] != dvtx[ctr2 * 3 + 2])
				continue;
			if (dnorm[ctr * 3] != dnorm[ctr2 * 3])
				continue;
			if (dnorm[ctr * 3 + 1] != dnorm[ctr2 * 3 + 1])
				continue;
			if (dnorm[ctr * 3 + 2] != dnorm[ctr2 * 3 + 2])
				continue;
			if (dtex[ctr * 2] != dtex[ctr2 * 2])
				continue;
			if (dtex[ctr * 2 + 1] != dtex[ctr2 * 2 + 1])
				continue;
			ClassEMemory::Copy(&dvtx[ctr2 * 3], &dvtx[(ctr2 + 1) * 3], (allocvtx - ctr2 - 1) * 12);
			ClassEMemory::Copy(&dnorm[ctr2 * 3], &dnorm[(ctr2 + 1) * 3], (allocvtx - ctr2 - 1) * 3);
			ClassEMemory::Copy(&dtex[ctr2 * 2], &dtex[(ctr2 + 1) * 2], (allocvtx - ctr2 - 1) * 8);
			for (ctr3 = 0; ctr3 < allocidx * 3; ctr3++)
			{
				if (didx[ctr3] == ctr2)
					didx[ctr3] = (unsigned short)ctr;
				else if (didx[ctr3] > ctr2)
					didx[ctr3]--;
			}
			ctr2--;
			allocvtx--;
		}
}

//Constructor
MeshContainer::MeshContainer(void)
{
	dvtx = NULL;
	dtex = NULL;
	dnorm = NULL;
	didx = NULL;
}

//Destructor
MeshContainer::~MeshContainer(void)
{
	FULL_FREE(dvtx);
	FULL_FREE(dtex);
	FULL_FREE(dnorm);
	FULL_FREE(didx);
}

//Load callback: Starting of mesh data
ERESULT MeshContainer::LdrOnMeshStart(ELDR_3DCALLBACKINFO *ldrdata)
{
	normalcalc = true;
	return(E_OK);
}

//Load callback: End of mesh data
ERESULT MeshContainer::LdrOnMeshEnd(ELDR_3DCALLBACKINFO *ldrdata)
{
	unsigned long ctr;
	if (ldrdata->normalonvertex && !normalcalc)
	{
		signed char *newnormdata = (signed char *)ClassEMemory::Alloc(allocidx * 3 * 3);
		if (newnormdata)
		{
			for (ctr = 0; ctr < allocidx * 3; ctr++)
			{
				newnormdata[ctr * 3 + 0] = dnorm[didx[ctr] * 3 + 0];
				newnormdata[ctr * 3 + 1] = dnorm[didx[ctr] * 3 + 1];
				newnormdata[ctr * 3 + 2] = dnorm[didx[ctr] * 3 + 2];
			}
			FULL_FREE(dnorm);
			dnorm = newnormdata;
			allocnorm = allocidx * 3;
		}
		else
			return(E_NOMEMORY);
	}
	if (ldrdata->texelonvertex)
	{
		long *newtexdata = (long *)ClassEMemory::Alloc(allocidx * 3 * 8);
		if (newtexdata)
		{
			for (ctr = 0; ctr < allocidx * 3; ctr++)
			{
				newtexdata[ctr * 2 + 0] = dtex[didx[ctr] * 2 + 0];
				newtexdata[ctr * 2 + 1] = dtex[didx[ctr] * 2 + 1];
			}
			FULL_FREE(dtex);
			dtex = newtexdata;
			alloctex = allocidx * 3;
		}
		else
			return(E_NOMEMORY);
	}
	long *newvtxdata = (long *)ClassEMemory::Alloc(allocidx * 3 * 12);
	if (newvtxdata)
	{
		for (ctr = 0; ctr < allocidx * 3; ctr++)
		{
			newvtxdata[ctr * 3 + 0] = dvtx[didx[ctr] * 3 + 0];
			newvtxdata[ctr * 3 + 1] = dvtx[didx[ctr] * 3 + 1];
			newvtxdata[ctr * 3 + 2] = dvtx[didx[ctr] * 3 + 2];
		}
		FULL_FREE(dvtx);
		dvtx = newvtxdata;
		allocvtx = allocidx * 3;
	}
	else
		return(E_NOMEMORY);
	for (ctr = 0; ctr < allocidx * 3; ctr++)
		didx[ctr] = (unsigned short)ctr;
	if (normalcalc)
	{
		dnorm = (signed char *)ClassEMemory::Alloc(allocidx * 3 * 3);
		if (dnorm == NULL)
			return(E_NOMEMORY);
		allocnorm = allocidx * 3;
		CalculateNormals(dvtx, dnorm, didx, allocvtx, allocidx);
	}
	Optimize();
	return(E_OK);
}

//Load callback: Vertex stream data
ERESULT MeshContainer::LdrOnVertexStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	long *newvtx;
	unsigned long addcount = ldrdata->streamsize / 12, newallocvtx = ldrdata->vertexindex + addcount;
	if (addcount)
	{
		if (dvtx == NULL)
			allocvtx = 0;
		if (newallocvtx > allocvtx)
		{
			if (dvtx)
				newvtx = (long *)ClassEMemory::ReAlloc(dvtx, newallocvtx * 12);
			else
				newvtx = (long *)ClassEMemory::Alloc(newallocvtx * 12);
			if (newvtx == NULL)
				return(E_NOMEMORY);
			allocvtx = newallocvtx;
			dvtx = newvtx;
		}
		ClassEMemory::Copy(&dvtx[ldrdata->vertexindex * 3], ldrdata->streamdata, ldrdata->streamsize);
		ldrdata->vertexindex += addcount;
	}
	return(E_OK);
}

//Load callback: Normal stream data
ERESULT MeshContainer::LdrOnNormalStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	signed char *newnorm;
	unsigned long addcount = ldrdata->streamsize / 12, newallocnorm = ldrdata->normalindex + addcount;
	if (addcount)
	{
		if (dnorm == NULL)
			allocnorm = 0;
		if (newallocnorm > allocnorm)
		{
			if (dnorm)
				newnorm = (signed char *)ClassEMemory::ReAlloc(dnorm, newallocnorm * 3);
			else
				newnorm = (signed char *)ClassEMemory::Alloc(newallocnorm * 3);
			if (newnorm == NULL)
				return(E_NOMEMORY);
			allocnorm = newallocnorm;
			dnorm = newnorm;
		}
		long *streamdat = (long *)ldrdata->streamdata;
		unsigned long ctr;
		for (ctr = 0; ctr < addcount; ctr++)
		{
			dnorm[(ldrdata->normalindex + ctr) * 3 + 0] = (signed char)(streamdat[ctr * 3] * 127 / 65536);
			dnorm[(ldrdata->normalindex + ctr) * 3 + 1] = (signed char)(streamdat[ctr * 3 + 1] * 127 / 65536);
			dnorm[(ldrdata->normalindex + ctr) * 3 + 2] = (signed char)(streamdat[ctr * 3 + 2] * 127 / 65536);
		}
		ldrdata->normalindex += addcount;
	}
	normalcalc = false;
	return(E_OK);
}

//Load callback: Texel stream data
ERESULT MeshContainer::LdrOnTexcoordStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	long *newtex;
	unsigned long addcount = ldrdata->streamsize / 8, newalloctex = ldrdata->texelindex + addcount;
	if (addcount)
	{
		if (dtex == NULL)
			alloctex = 0;
		if (newalloctex > alloctex)
		{
			if (dtex)
				newtex = (long *)ClassEMemory::ReAlloc(dtex, newalloctex * 8);
			else
				newtex = (long *)ClassEMemory::Alloc(newalloctex * 8);
			if (newtex == NULL)
				return(E_NOMEMORY);
			alloctex = newalloctex;
			dtex = newtex;
		}
		long *streamdat = (long *)ldrdata->streamdata;
		unsigned long ctr;
		for (ctr = 0; ctr < addcount; ctr++)
		{
			dtex[(ldrdata->texelindex + ctr) * 2 + 0] = streamdat[ctr * 2 + 0];
			dtex[(ldrdata->texelindex + ctr) * 2 + 1] = -streamdat[ctr * 2 + 1];
		}
		ldrdata->texelindex += addcount;
	}
	return(E_OK);
}

//Load callback: Index stream data
ERESULT MeshContainer::LdrOnIndexStream(ELDR_3DCALLBACKINFO *ldrdata)
{
	unsigned short *newidx;
	unsigned long addcount = ldrdata->streamsize / 16, newallocidx = ldrdata->polygonindex + addcount;
	if (addcount)
	{
		if (didx == NULL)
			allocidx = 0;
		if (newallocidx > allocidx)
		{
			if (didx)
				newidx = (unsigned short *)ClassEMemory::ReAlloc(didx, newallocidx * 6);
			else
				newidx = (unsigned short *)ClassEMemory::Alloc(newallocidx * 6);
			if (newidx == NULL)
				return(E_NOMEMORY);
			allocidx = newallocidx;
			didx = newidx;
		}
		unsigned long *streamdat = (unsigned long *)ldrdata->streamdata;
		unsigned long ctr;
		for (ctr = 0; ctr < addcount; ctr++)
		{
			didx[(ldrdata->polygonindex + ctr) * 3 + 0] = (unsigned short)streamdat[ctr * 4];
			didx[(ldrdata->polygonindex + ctr) * 3 + 1] = (unsigned short)streamdat[ctr * 4 + 1];
			didx[(ldrdata->polygonindex + ctr) * 3 + 2] = (unsigned short)streamdat[ctr * 4 + 2];
		}
		ldrdata->polygonindex += addcount;
	}
	return(E_OK);
}
