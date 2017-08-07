// eldr_pvr.cpp
// PVR loader
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eloader.h"
#include "ememory.h"
#include "estdc.h"
#include "edisplay.h"

#include "extra/verinfo.h"


///////////////////////////////////////////////////////////////////
// Loader: Definitions                                           //
///////////////////////////////////////////////////////////////////

#define EPVR_HEADERSIG       (('P') | ('V' << 8) | ('R' << 16) | ('!' << 24))
#define LPVR_MAXSTREAMDAT    1024


///////////////////////////////////////////////////////////////////
// Loader: Structures                                            //
///////////////////////////////////////////////////////////////////

typedef struct
{
	unsigned long headersize;
	unsigned long width;
	unsigned long height;
	unsigned long mipmaps;
	unsigned long flags;
	unsigned long texsize;
	unsigned long bpp;
	unsigned long redmask;
	unsigned long greenmask;
	unsigned long bluemask;
	unsigned long alphamask;
	unsigned long sig;
	unsigned long imgcount;
}EPVR_HEADER;
//The PVR texture header


///////////////////////////////////////////////////////////////////
// Loader: Public                                                //
///////////////////////////////////////////////////////////////////

//Start the loader
ERESULT ELoader_PVR::Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags)
{
	ERESULT result = E_OK;
	ELDR_2DCALLBACKINFO cbinfo;
	EPVR_HEADER pvrtcheader;
	unsigned char streamdat[LPVR_MAXSTREAMDAT];
	unsigned long headersize;
	LinkData(ldata, lsize);
	cbinfo.nativedisplaymode = nativedisplaymode;
	cbinfo.createflags = createflags;
	cbinfo.paletteindex = 0;
	cbinfo.pixelindex = 0;
	cbinfo.usetransparency = false;
	cbinfo.compressedformat = NULL;
	Read32(&headersize);
	SeekSet(0);
	if (!ReadStream(&pvrtcheader, sizeof(pvrtcheader)))
		return(E_UNSUPPORTED);
	SeekSet(headersize);
	if (pvrtcheader.sig == EPVR_HEADERSIG)
	{
		unsigned long texsize = pvrtcheader.texsize, pvrbpp = pvrtcheader.bpp;
		bool pvralpha = (pvrtcheader.alphamask > 0);
		if (pvrbpp != 2 && pvrbpp != 4)
			return(E_UNSUPPORTED);
		cbinfo.width = pvrtcheader.width;
		cbinfo.height = pvrtcheader.height;
		cbinfo.streamsize = texsize;
		cbinfo.compressedformat = (char *)ClassEMemory::Alloc(64);
		if (pvrbpp == 2)
		{
			if (pvralpha)
				ClassEStd::StrCpy(cbinfo.compressedformat, "pvrtc_2bpp_alpha");
			else
				ClassEStd::StrCpy(cbinfo.compressedformat, "pvrtc_2bpp");
		}
		else
		{
			if (pvralpha)
				ClassEStd::StrCpy(cbinfo.compressedformat, "pvrtc_4bpp_alpha");
			else
				ClassEStd::StrCpy(cbinfo.compressedformat, "pvrtc_4bpp");
		}
		result = surface->LdrOnCreate(&cbinfo);
		ClassEMemory::DeAlloc(cbinfo.compressedformat);
		if (result == E_OK)
		{
			unsigned long ctr, readsize;
			for (ctr = 0; ctr < (texsize + LPVR_MAXSTREAMDAT - 1) / LPVR_MAXSTREAMDAT; ctr++)
			{
				readsize = texsize - ctr * LPVR_MAXSTREAMDAT;
				if (readsize > LPVR_MAXSTREAMDAT)
					readsize = LPVR_MAXSTREAMDAT;
				ReadStream(streamdat, readsize);
				cbinfo.streamdata = streamdat;
				cbinfo.streamsize = readsize;
				result = surface->LdrOnPixelStream(&cbinfo);
				if (result != E_OK)
					break;
			}
		}
	}
	else
		return(E_UNSUPPORTED);
	return(result);
}
