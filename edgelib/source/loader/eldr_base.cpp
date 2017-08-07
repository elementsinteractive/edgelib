// eldr_base.cpp
// Base loader
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eloader.h"
#include "ememory.h"
#include "edisplay.h"

#include "extra/verinfo.h"


/////////////////////////////////////////////////////////////////////
// ClassELoaderBase::Private                                       //
/////////////////////////////////////////////////////////////////////

//Set byte order for reading integer data
void ClassELoaderBase::SetByteOrder(bool bigendian)
{
	isbigendian = bigendian;
}

//Read the next byte (least significant byte order)
bool ClassELoaderBase::Read8(unsigned char *dest)
{
	if (lptr >= maxsize)
		return(false);
	*dest = lmem[lptr];
	lptr++;
	return(true);
}

//Read the next word (least significant byte order)
bool ClassELoaderBase::Read16(unsigned short *dest)
{
	if (lptr + 1 >= maxsize)
		return(false);
	if (isbigendian)
		*dest = lmem[lptr + 1] + (lmem[lptr] << 8);
	else
		*dest = lmem[lptr] + (lmem[lptr + 1] << 8);
	lptr += 2;
	return(true);
}

//Read the next double word (least significant byte order)
bool ClassELoaderBase::Read32(unsigned long *dest)
{
	if (lptr + 3 >= maxsize)
		return(false);
	if (isbigendian)
		*dest = lmem[lptr + 3] + (lmem[lptr + 2] << 8) + (lmem[lptr + 1] << 16) + (lmem[lptr] << 24);
	else
		*dest = lmem[lptr] + (lmem[lptr + 1] << 8) + (lmem[lptr + 2] << 16) + (lmem[lptr + 3] << 24);
	lptr += 4;
	return(true);
}

//Read a binary stream of data
bool ClassELoaderBase::ReadStream(void *dest, unsigned long rsize)
{
	if (lptr >= maxsize)
		return(false);
	if (lptr + rsize > maxsize)
		rsize = maxsize - lptr;
	ClassEMemory::Copy(dest, &lmem[lptr], rsize);
	lptr += rsize;
	return(true);
}

//Manually set the load pointer
void ClassELoaderBase::SeekSet(unsigned long pos)
{
	lptr = pos;
}

//Manually move the load pointer relative to its current position
void ClassELoaderBase::SeekRelative(long pos)
{
	lptr += pos;
}

//Tell the load pointer
unsigned long ClassELoaderBase::Tell(void)
{
	return(lptr);
}

//Return if there is still data to read
bool ClassELoaderBase::HaveData(void)
{
	return(lptr < maxsize);
}

//Link a pointer to the loader system
void ClassELoaderBase::LinkData(void *ptr, unsigned long psize)
{
	lmem = (unsigned char *)ptr;
	maxsize = psize;
	SeekSet(0);
}

/////////////////////////////////////////////////////////////////////
// ClassELoaderBase::Public                                        //
/////////////////////////////////////////////////////////////////////

//ClassELoaderBase: constructor
ClassELoaderBase::ClassELoaderBase(void)
{
	lptr     = 0;
	lmem     = NULL;
	isbigendian = false;
}

//ClassELoaderBase: destructor
ClassELoaderBase::~ClassELoaderBase(void)
{
}

//Start loading (2D surface)
ERESULT ClassELoaderBase::Start(E2DSurfaceBase *surface, void *ldata, unsigned long lsize, unsigned char nativedisplaymode, unsigned long createflags)
{
	return(E_UNSUPPORTED);
}

//Start loading (3D surface)
ERESULT ClassELoaderBase::Start(E3DLoaderBase *surface, void *ldata, unsigned long lsize, signed char modelsize, unsigned long createflags)
{
	return(E_UNSUPPORTED);
}
