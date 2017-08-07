// eldr_dae.cpp
// DAE file loader
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
#include "extapi/dataapi.h"
#include "extapi/xmlapi.h"

//Calculate UV from ST:
//U1 = Sx * x1 + Sy * y1 + Sz * z1 + Sw
//V1 = Tx * x1 + Ty * y1 + Tz * z1 + Tw

//ST = Sx, Sy, Sz, Sw, Tx, Ty, Tz, Tw

///////////////////////////////////////////////////////////////////
// Type definitions                                              //
///////////////////////////////////////////////////////////////////

//Stream data chunk size
#define EDAE_MAXSTREAMDAT (1024 * 16)

//Source types
#define DAESRC_NONE          0
#define DAESRC_VERTEX        1
#define DAESRC_NORMAL        2
#define DAESRC_TEXCOORD      3
#define DAESRC_TRIANGLE      4

//List types
#define DAELST_NONE          0
#define DAELST_VERTEX        1
#define DAELST_TRIANGLE      2

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

typedef struct
{
	ELDR_3DCALLBACKINFO cbinfo;
	E3DLoaderBase *surface;
	ERESULT xmlresult;
	long streamarray[EDAE_MAXSTREAMDAT];
	long *valarray[3];
	long valarrcount[3];
	long attr_count;
	unsigned char sourcetype;
	unsigned char listtype;
	signed char modelsize;
	bool readingmesh;
	bool isfirsttag;
	bool yupaxis;
}DAE_WORKDATA;

///////////////////////////////////////////////////////////////////
// Loader: Pure functions
///////////////////////////////////////////////////////////////////

//Init worker data structure
void ELoader_DAE_InitWorkdata(DAE_WORKDATA *wdata)
{
	wdata->valarray[0] = NULL;
	wdata->valarray[1] = NULL;
	wdata->valarray[2] = NULL;
}

//Clean worker data structure and free itself
void ELoader_DAE_DeleteWorkdata(DAE_WORKDATA *wdata)
{
	if (wdata)
	{
		ClassEMemory::DeAlloc(wdata->valarray[0]);
		ClassEMemory::DeAlloc(wdata->valarray[1]);
		ClassEMemory::DeAlloc(wdata->valarray[2]);
		ClassEMemory::DeAlloc(wdata);
	}
}

//Clean worker data structure and free itself
bool ELoader_DAE_FillValues(long *&valarray, WCHAR *str_array, unsigned long count, bool allocmem = true)
{
	if (allocmem)
	{
		if (valarray != NULL)
			ClassEMemory::DeAlloc(valarray);
		valarray = (long *)ClassEMemory::Alloc(count * 4);
		if (valarray == NULL)
			return(false);
	}
	unsigned long ctr, len = ClassEStd::StrLen(str_array), onnr = 0;
	long intvalue = 0, intmantissa = 0, dotpos = -1;
	bool negative = false;
	for (ctr = 0; ctr < len; ctr++)
	{
		if (onnr >= count)
			break;
		if (!ClassEStd::IsWhiteSpace(str_array[ctr]))
		{
			if (str_array[ctr] >= '0' && str_array[ctr] <= '9')
			{
				if (dotpos == -1)
				{
					intvalue *= 10;
					intvalue += str_array[ctr] - '0';
				}
				else
				{
					unsigned long div = 1;
					for (int tmp = 0; tmp < dotpos; tmp++)
						div *= 10;
					intmantissa += ((str_array[ctr] - '0') << 12) / div;
					dotpos++;
				}
			}
			else if (str_array[ctr] == '-')
				negative = true;
			else if (str_array[ctr] == '.')
				dotpos = 0;
		}
		if (ClassEStd::IsWhiteSpace(str_array[ctr]) || ctr == len - 1)
		{
			if (negative)
			{
				if (intvalue != 0)
					intvalue = -intvalue;
				else
					intmantissa = -intmantissa;
			}
			valarray[onnr++] = (intvalue << 20) + ((intmantissa * 65535) >> 12);
			dotpos = -1;
			negative = false;
			intvalue = 0;
			intmantissa = 0;
		}
	}
	return(true);
}

///////////////////////////////////////////////////////////////////
// Loader: Public
///////////////////////////////////////////////////////////////////

//Xml Callback function wrapper
bool ELoader_DAE_XmlCallback(ClassEXml *parser, unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value, void *userdata)
{
	ELoader_DAE *dae = (ELoader_DAE *)userdata;
	return(dae->XmlCallback(parser, event, name, attribute, value));
}

//Start the loader
ERESULT ELoader_DAE::Start(E3DLoaderBase *surface, void *ldata, unsigned long lsize, signed char modelsize, unsigned long createflags)
{
	return(E_NOFEATURE);//tmp, don't add to current edgelib release
	#if (EDITION_SDK < 1000)
		return(E_NOFEATURE);
	#else
		if (lsize == 0)
			return(E_NODATA);
		workerdata = ClassEMemory::Alloc(sizeof(DAE_WORKDATA));
		if (workerdata == NULL)
			return(E_NOMEMORY);
		ELoader_DAE_InitWorkdata((DAE_WORKDATA *)workerdata);
		ClassEXml parser;
		DAE_WORKDATA *workdata = (DAE_WORKDATA *)workerdata;
		char *xmldata = (char *)ldata;
		if (((char *)(ldata))[lsize - 1] != 0)
		{
			xmldata = (char *)ClassEMemory::Alloc(lsize + 1);
			if (xmldata == NULL)
			{
				ELoader_DAE_DeleteWorkdata(workdata);
				return(E_NOMEMORY);
			}
			ClassEMemory::Copy(xmldata, ldata, lsize);
			xmldata[lsize] = 0;
		}
		if (parser.Open((char *)xmldata))
		{
			workdata->cbinfo.coloronvertex = false;
			workdata->cbinfo.normalonvertex = false;
			workdata->cbinfo.texelonvertex = false;
			workdata->cbinfo.createflags = createflags;
			workdata->cbinfo.vertexindex = 0;
			workdata->cbinfo.normalindex = 0;
			workdata->cbinfo.colorindex = 0;
			workdata->cbinfo.texelindex = 0;
			workdata->cbinfo.polygonindex = 0;
			workdata->surface = surface;
			workdata->modelsize = modelsize;
			workdata->isfirsttag = true;
			workdata->readingmesh = true;
			workdata->xmlresult = E_UNSUPPORTED;
			workdata->sourcetype = DAESRC_NONE;
			workdata->listtype = DAELST_NONE;
			if (!parser.Parse(false, ELoader_DAE_XmlCallback, this))
			{
				if (xmldata != ldata)
					ClassEMemory::DeAlloc(xmldata);
				ERESULT parseresult = workdata->xmlresult;
				ELoader_DAE_DeleteWorkdata(workdata);
				return(parseresult);
			}
		}
		else
		{
			if (xmldata != ldata)
				ClassEMemory::DeAlloc(xmldata);
			ELoader_DAE_DeleteWorkdata(workdata);
			return(E_UNSUPPORTED);
		}
		parser.Close();
		if (xmldata != ldata)
			ClassEMemory::DeAlloc(xmldata);
		ERESULT finalresult = workdata->xmlresult;
		ELoader_DAE_DeleteWorkdata(workdata);
		return(finalresult);
	#endif
}

//XML callback function
bool ELoader_DAE::XmlCallback(void *parser, unsigned char event, const WCHAR *name, const WCHAR *attribute, const WCHAR *value)
{
	DAE_WORKDATA *workdata = (DAE_WORKDATA *)workerdata;
	switch(event)
	{
		case EXML_OPENTAG:
			if (workdata->isfirsttag && !ClassEStd::StrEqual(name, "COLLADA", false))
			{
				workdata->xmlresult = E_UNSUPPORTED;
				return(false);
			}
			else if (workdata->isfirsttag)
				workdata->xmlresult = E_OK;
			workdata->isfirsttag = false;
			if (ClassEStd::StrEqual(name, "mesh", false))
			{
				workdata->readingmesh = true;
				workdata->cbinfo.reservevertices = 1000;//tmp!
				workdata->cbinfo.reserveindices = 2000;//tmp!
				workdata->xmlresult = workdata->surface->LdrOnMeshStart(&workdata->cbinfo);
				if (workdata->xmlresult != E_OK)
					return(false);
			}
			else if (ClassEStd::StrEqual(name, "vertices", false))
				workdata->listtype = DAELST_VERTEX;
			else if (ClassEStd::StrEqual(name, "triangles", false))
				workdata->listtype = DAELST_TRIANGLE;
			break;
		case EXML_CLOSETAG:
			if (ClassEStd::StrEqual(name, "mesh", false))
			{
				workdata->readingmesh = false;
				workdata->xmlresult = E_OK;
				workdata->surface->LdrOnMeshEnd(&workdata->cbinfo);
				return(false);//Read only one mesh for now...
			}
			else if (ClassEStd::StrEqual(name, "source", false))
				workdata->sourcetype = DAESRC_NONE;
			else if (ClassEStd::StrEqual(name, "vertices", false) || ClassEStd::StrEqual(name, "triangles", false))
				workdata->listtype = DAELST_NONE;
			break;
		case EXML_ADDATTRIBUTE:
			if (ClassEStd::StrEqual(name, "source", false))
			{
				if (ClassEStd::StrEqual(attribute, "name", false))
				{
					if (ClassEStd::StrEqual(value, "position", false))
						workdata->sourcetype = DAESRC_VERTEX;
					else if (ClassEStd::StrEqual(value, "normal", false))
						workdata->sourcetype = DAESRC_NORMAL;
					else if (ClassEStd::StrEqual(value, "texcoord", false))
						workdata->sourcetype = DAESRC_TEXCOORD;
				}
			}
			else if (ClassEStd::StrEqual(name, "input", false))
			{
				if (ClassEStd::StrEqual(attribute, "semantic", false) && workdata->listtype == DAELST_VERTEX)
				{
					if (ClassEStd::StrEqual(value, "position", false))
					{
						unsigned long ctr = 0, startidx = 0;
						while (workdata->valarrcount[0])
						{
							workdata->streamarray[ctr] = workdata->valarray[0][startidx + ctr] >> (14 - workdata->modelsize);
							workdata->valarrcount[0]--;
							ctr++;
							if (((ctr + 3) >= EDAE_MAXSTREAMDAT / 12 && ctr % 3 == 0) || workdata->valarrcount[0] == 0)
							{
								startidx += ctr;
								workdata->cbinfo.streamdata = workdata->streamarray;
								workdata->cbinfo.streamsize = ctr * 4;
								workdata->xmlresult = workdata->surface->LdrOnVertexStream(&workdata->cbinfo);
								if (workdata->xmlresult != E_OK)
									return(false);
								ctr = 0;
							}
						}
					}
				}
			}
			if (ClassEStd::StrEqual(attribute, "count", false))
				workdata->attr_count = ClassEStd::StrToInt((WCHAR *)value);
			break;
		case EXML_CLOSEATTRIBUTES:
			break;
		case EXML_ADDCONTENT:
			if (ClassEStd::StrEqual(name, "up_axis", false))
			{
				if (ClassEStd::StrEqual(value, "Y_UP", false))
					workdata->yupaxis = true;
				else
					workdata->yupaxis = true;
			}
			else if (ClassEStd::StrEqual(name, "float_array", false) && workdata->sourcetype != DAESRC_NONE)
			{
				bool fillresult = false;
				if (workdata->sourcetype == DAESRC_VERTEX)
				{
					workdata->valarrcount[0] = workdata->attr_count;
					fillresult = ELoader_DAE_FillValues(workdata->valarray[0], (WCHAR *)value, workdata->valarrcount[0]);
				}
				else if (workdata->sourcetype == DAESRC_NORMAL)
				{
					workdata->valarrcount[1] = workdata->attr_count;
					fillresult = ELoader_DAE_FillValues(workdata->valarray[1], (WCHAR *)value, workdata->valarrcount[1]);
				}
				else if (workdata->sourcetype == DAESRC_TEXCOORD)
				{
					workdata->valarrcount[2] = workdata->attr_count;
					fillresult = ELoader_DAE_FillValues(workdata->valarray[2], (WCHAR *)value, workdata->valarrcount[2]);
				}
				if (!fillresult)
				{
					workdata->xmlresult = E_NOMEMORY;
					return(false);
				}
			}
			if (ClassEStd::StrEqual(name, "p", false))
			{
				unsigned long ctr;
				bool fillresult = ELoader_DAE_FillValues(workdata->valarray[0], (WCHAR *)value, workdata->attr_count * 9);
				if (!fillresult)
				{
					workdata->xmlresult = E_NOMEMORY;
					return(false);
				}
				for (ctr = 0; ctr < (unsigned long)workdata->attr_count; ctr++)//tmp!
				{
					workdata->streamarray[ctr * 4] = workdata->valarray[0][ctr * 9] >> 20;
					workdata->streamarray[ctr * 4 + 1] = workdata->valarray[0][ctr * 9 + 3] >> 20;
					workdata->streamarray[ctr * 4 + 2] = workdata->valarray[0][ctr * 9 + 6] >> 20;
					workdata->streamarray[ctr * 4 + 3] = E_RGBX(255, 255, 255);
				}
				workdata->cbinfo.streamdata = workdata->streamarray;
				workdata->cbinfo.streamsize = workdata->attr_count * 4 * 4;
				workdata->xmlresult = workdata->surface->LdrOnIndexStream(&workdata->cbinfo);
				if (workdata->xmlresult != E_OK)
					return(false);
				for (ctr = 0; ctr < (unsigned long)workdata->attr_count; ctr++)
				{
					long idx[] = { workdata->valarray[0][ctr * 9 + 1] >> 20, workdata->valarray[0][ctr * 9 + 4] >> 20, workdata->valarray[0][ctr * 9 + 7] >> 20 };
					workdata->streamarray[ctr * 9] = workdata->valarray[1][idx[0] * 3] >> 4;
					workdata->streamarray[ctr * 9 + 1] = workdata->valarray[1][idx[0] * 3 + 1] >> 4;
					workdata->streamarray[ctr * 9 + 2] = workdata->valarray[1][idx[0] * 3 + 2] >> 4;
					workdata->streamarray[ctr * 9 + 3] = workdata->valarray[1][idx[1] * 3] >> 4;
					workdata->streamarray[ctr * 9 + 4] = workdata->valarray[1][idx[1] * 3 + 1] >> 4;
					workdata->streamarray[ctr * 9 + 5] = workdata->valarray[1][idx[1] * 3 + 2] >> 4;
					workdata->streamarray[ctr * 9 + 6] = workdata->valarray[1][idx[2] * 3] >> 4;
					workdata->streamarray[ctr * 9 + 7] = workdata->valarray[1][idx[2] * 3 + 1] >> 4;
					workdata->streamarray[ctr * 9 + 8] = workdata->valarray[1][idx[2] * 3 + 2] >> 4;
				}
				workdata->cbinfo.streamdata = workdata->streamarray;
				workdata->cbinfo.streamsize = workdata->attr_count * 9 * 4;
				workdata->xmlresult = workdata->surface->LdrOnNormalStream(&workdata->cbinfo);
				if (workdata->xmlresult != E_OK)
					return(false);
				for (ctr = 0; ctr < (unsigned long)workdata->attr_count; ctr++)
				{
					long idx[] = { workdata->valarray[0][ctr * 9 + 2] >> 20, workdata->valarray[0][ctr * 9 + 5] >> 20, workdata->valarray[0][ctr * 9 + 8] >> 20 };
					workdata->streamarray[ctr * 6] = workdata->valarray[2][idx[0] * 2] >> 4;
					workdata->streamarray[ctr * 6 + 1] = workdata->valarray[2][idx[0] * 2 + 1] >> 4;
					workdata->streamarray[ctr * 6 + 2] = workdata->valarray[2][idx[1] * 2] >> 4;
					workdata->streamarray[ctr * 6 + 3] = workdata->valarray[2][idx[1] * 2 + 1] >> 4;
					workdata->streamarray[ctr * 6 + 4] = workdata->valarray[2][idx[2] * 2] >> 4;
					workdata->streamarray[ctr * 6 + 5] = workdata->valarray[2][idx[2] * 2 + 1] >> 4;
				}
				workdata->cbinfo.streamdata = workdata->streamarray;
				workdata->cbinfo.streamsize = workdata->attr_count * 6 * 4;
				workdata->xmlresult = workdata->surface->LdrOnTexcoordStream(&workdata->cbinfo);
				if (workdata->xmlresult != E_OK)
					return(false);
			}
			break;
	}
	return(true);
}
