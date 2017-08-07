// cmemory.cpp
// Edgecore: provides memory API
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Device include
#include "edgedevice.h"

//Device SDK includes
#if defined(DEVICE_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#elif defined(DEVICE_SYMBIAN)
	#include <eikenv.h>
#elif defined(DEVICE_LINUX)
	#include <stdlib.h>
	#include <string.h>
#elif defined(DEVICE_MAC)
	#include <stdlib.h>
#endif

//Core include
#include "edgecore.h"

///////////////////////////////////////////////////////////////////
// Internal definitions and structures
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Memory API
///////////////////////////////////////////////////////////////////

//Allocate memory
void *ecMemAlloc(int count)
{
	#if defined(DEVICE_WIN32)
		return(GlobalAlloc(GMEM_FIXED, count));
	#elif defined(DEVICE_SYMBIAN)
		return(User::Alloc(count));
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		return(malloc(count));
	#elif defined(DEVICE_OPENKODE)
		return(kdMalloc(count));
	#else
		return(new unsigned char[count]);
	#endif
}

//Reallocate memory
void *ecMemReAlloc(void *ptr, int newcount)
{
	#if defined(DEVICE_WIN32)
		return(GlobalReAlloc(ptr, newcount, GMEM_MOVEABLE));
	#elif defined(DEVICE_SYMBIAN)
		return(User::ReAlloc(ptr, newcount));
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		return(realloc(ptr, newcount));
	#elif defined(DEVICE_OPENKODE)
		return(kdRealloc(ptr, newcount));
	#else
		unsigned char *newptr = new unsigned char[newcount];
		if (newptr)
		{
			if (newcount < count)
				ecMemCpy(newptr, ptr, newcount);
			else
				ecMemCpy(newptr, ptr, count);
		}
		ecMemFree(ptr);
		return(newptr);
	#endif
}

//Deallocate memory
void ecMemFree(void *src)
{
	#if defined(DEVICE_WIN32)
		GlobalFree(src);
	#elif defined(DEVICE_SYMBIAN)
		User::Free(src);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		free(src);
	#elif defined(DEVICE_OPENKODE)
		return(kdFree(src));
	#else
		delete src;
	#endif
}

//Copy memory buffers
void ecMemCpy(void *dest, const void *src, int count)
{
	#if defined(DEVICE_WIN32)
		CopyMemory(dest, src, count);
	#elif defined(DEVICE_SYMBIAN)
		Mem::Copy(dest, src, count);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		memcpy(dest, src, count);
	#elif defined(DEVICE_OPENKODE)
		kdMemcpy(dest, src, count);
	#else
		for (int i = 0; i < count; i++)
			*((unsigned char *)(dest) + i) = *((unsigned char *)(src) + i);
	#endif
}

//Move memory buffers
void ecMemMove(void *dest, const void *src, int count)
{
	#if defined(DEVICE_WIN32)
		MoveMemory(dest, src, count);
	#elif defined(DEVICE_SYMBIAN)
		Mem::Move(dest, src, count);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		memmove(dest, src, count);
	#elif defined(DEVICE_OPENKODE)
		kdMemmove(dest, src, count);
	#else
		if (src < dest && (char *)(src) + count > dest)
		{
			for (int i = count - 1; i >= 0; i--)
				*((char *)(dest) + i) = *((char *)(src) + i);
		}
		else
		{
			for (int i = 0; i < count; i++)
				*((char *)(dest) + i) = *((char *)(src) + i);
		}
	#endif
}

//Set memory buffer
void ecMemSet(void *dest, unsigned char byte, unsigned long count)
{
	#if defined(DEVICE_WIN32)
		FillMemory(dest, count, byte);
	#elif defined(DEVICE_SYMBIAN)
		Mem::Fill(dest, count, byte);
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		memset(dest, byte, count);
	#elif defined(DEVICE_OPENKODE)
		kdMemset(dest, byte, count);
	#else
		for (int i = 0; i < (int)count; i++)
			*((unsigned char *)(dest) + i) = byte;
	#endif
}
