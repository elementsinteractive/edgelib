// emod_memory.cpp
// Memory class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "ememory.h"

#include "core/edgecore.h"
#include "extra/verinfo.h"

#include "czlib.h"
#if !defined(DEVICE_SYMBIAN)
	#pragma comment(lib, "zlib.lib")
#endif

/////////////////////////////////////////////////////////////////////
// ClassEMemory::Pure functions                                    //
/////////////////////////////////////////////////////////////////////

//Reset a memory buffer
void EMemory_Set(const void *dest, unsigned long value, unsigned long bytecount)
{
	unsigned char *d8 = (unsigned char *)dest;
	if (bytecount)
		if ((unsigned long)(d8) % 2 == 1)
		{
			*d8++ = (unsigned char)(value & 0xFF);
			value = ((value & 0xFFFFFF) << 8) | ((value & 0xFF000000) >> 24);
			bytecount--;
		}
	if (bytecount > 1)
	{
		unsigned short *d16 = (unsigned short *)d8;
		if ((unsigned long)(d16) % 4 == 2)
		{
			*d16++ = (unsigned short)(value & 0xFFFF);
			value = ((value & 0xFFFF) << 16) | ((value & 0xFFFF0000) >> 16);
			bytecount -= 2;
		}
		if (bytecount > 3)
		{
			register unsigned EINT32 *d32 = (unsigned EINT32 *)d16;
			register unsigned EINT32 *d32e = d32 + (bytecount >> 2);
			while(d32 < d32e)
				*d32++ = value;
			d16 = (unsigned short *)d32;
		}
		bytecount &= 3;
		if (bytecount > 1)
		{
			*d16++ = (unsigned short)(value & 0xFFFF);
			value = ((value & 0xFFFF) << 16) | ((value & 0xFFFF0000) >> 16);
			bytecount -= 2;
		}
		d8 = (unsigned char *)d16;
	}
	if (bytecount)
		*d8 = (unsigned char)(value & 0xFF);
}


/////////////////////////////////////////////////////////////////////
// ClassEMemory::Private                                           //
/////////////////////////////////////////////////////////////////////

//Search for a string in the compression/uncompression string table
unsigned long ClassEMemory::FindString(unsigned char *fulldata, unsigned char *str, unsigned long str_size)
{
	if (str_size == 0)
		return(0);
	if (str_size == 1)
		return(str[0] + 1);
	unsigned long ctr;
	for (ctr = 0; ctr < num_str; ctr++)
		if (str_len[ctr] == str_size)
			if (fulldata[str_index[ctr] + str_size - 1] == str[str_size - 1])
				if (Compare(&fulldata[str_index[ctr]], str, str_size) == 0)
					return(ctr + 257);
	return(0);
}

//Add a string in the compression string table
void ClassEMemory::AddCmpString(unsigned char *fulldata, unsigned long srdst_size, unsigned long str_size)
{
	if (num_str + 256 > 4000)
		return;
	str_index[num_str] = srdst_size - str_size + 1;
	str_len[num_str] = (unsigned char)str_size;
	num_str++;
}

//Add a string in the uncompression string table
void ClassEMemory::AddUCmpString(unsigned char *fulldata, unsigned long srdst_size, unsigned long str_size)
{
	if (num_str + 256 > 4000)
		return;
	str_index[num_str] = srdst_size - str_size + 1;
	str_len[num_str] = (unsigned char)str_size;
	num_str++;
}

//Retrieve a string from the compression/uncompression string table
void ClassEMemory::GetString(unsigned char *fulldata, unsigned char *str, unsigned long &str_size, unsigned long code)
{
	if (code >= num_str + 256)
	{
		str_size = 0;
		return;
	}
	if (code < 256)
	{
		str[0] = (unsigned char)code;
		str_size = 1;
	}
	else
	{
		str_size = str_len[code - 256];
		Copy(str, &fulldata[str_index[code - 256]], str_size);
	}
}

#if !defined(DEVICE_LINUX)

//Calculate the Adler32
unsigned long ClassEMemory::Adler32(void *buffer, unsigned long buflen)
{
	#if defined(DEVICE_ANTIX)
		return(0);
	#else
		return(CZLib::Adler32(0, (unsigned char *)buffer, buflen));
	#endif
}

#endif

/////////////////////////////////////////////////////////////////////
// ClassEMemory::Public                                            //
/////////////////////////////////////////////////////////////////////

//ClassEMemory: constructor
ClassEMemory::ClassEMemory(void)
{
	numlist = 0;
	plist = NULL;
	psize = NULL;
	str_index = NULL;
	str_len   = NULL;
	num_str   = 0;
}

//ClassEMemory: destructor
ClassEMemory::~ClassEMemory(void)
{
	if (plist != NULL)
		delete plist;
	if (psize != NULL)
		delete psize;
	numlist = 0;
}

//Allocate memory
void *ClassEMemory::Alloc(unsigned long memsize)
{
	if (memsize == 0)
		return(NULL);
	return(ecMemAlloc(memsize));
}

//Reallocate memory
void *ClassEMemory::ReAlloc(void *ptr, unsigned long memsize)
{
	if (memsize == 0)
	{
		if (ptr != NULL)
			DeAlloc(ptr);
		return(NULL);
	}
	if (ptr == NULL)
		return(Alloc(memsize));
	return(ecMemReAlloc(ptr, memsize));
}

//Free up previously allocated memory
void ClassEMemory::DeAlloc(void *ptr)
{
	if (ptr == NULL)
		return;
	ecMemFree(ptr);
}

//Allocate memory and put it in the list for memory management
void *ClassEMemory::AllocList(unsigned long memsize)
{
	if (memsize == 0)
		return(NULL);
	char *newptr = NULL;
	unsigned long ctr, found = 0xFFFFFFFF;
	if (numlist == 0)
	{
		if (plist != NULL)
		{
			delete plist;
			plist = NULL;
		}
		if (psize != NULL)
		{
			delete psize;
			psize = NULL;
		}
		numlist = 256;
		plist = new void*[numlist];
		if (plist != NULL)
		{
			psize = new unsigned long[numlist];
			if (psize == NULL)
			{
				if (plist != NULL)
				{
					delete plist;
					plist = NULL;
				}
				numlist = 0;
			}
			else
				Set32(psize, 0, numlist);
		}
		else
			numlist = 0;
	}
	for (ctr = 0; ctr < numlist; ctr++)
		if (psize[ctr] == 0)
		{
			found = ctr;
			break;
		}
	if (found == 0xFFFFFFFF)
	{
		void **newplist = NULL;
		newplist = new void *[numlist + 256];
		if (newplist == NULL)
			return(NULL);
		unsigned long *newpsize = NULL;
		newpsize = new unsigned long[numlist + 256];
		if (newpsize == NULL)
		{
			if (newplist != NULL)
				delete newplist;
			return(NULL);
		}
		Copy(newplist, plist, sizeof(void *) * numlist);
		Copy(newpsize, psize, sizeof(unsigned long) * numlist);
		Set32(&newpsize[numlist], 0, 256);
		if (plist != NULL)
			delete plist;
		if (psize != NULL)
			delete psize;
		plist = newplist;
		psize = newpsize;
		found = numlist;
		numlist += 256;
	}
	newptr = new char[memsize + sizeof(void*)];
	if (newptr == NULL)
		return(NULL);
	plist[found] = newptr;
	psize[found] = memsize + sizeof(void*);
	Copy(newptr, &found, sizeof(void*));
	return((void *)&newptr[sizeof(void*)]);
}

//Free up previously allocated memory (warning: only free up listed pointers by this class!)
void ClassEMemory::DeAllocList(void *ptr)
{
	if (ptr == NULL)
		return;
	char *refptr = (char *)(ptr) - sizeof(void*);
	if (refptr == NULL)
		return;
	if (numlist && plist != NULL && psize != NULL)
	{
		unsigned long index;
		Copy(&index, refptr, sizeof(void*));
		plist[index] = NULL;
		psize[index] = 0;
	}
	delete refptr;
}

//Retrieve the amount of memory that is being used by this class
unsigned long ClassEMemory::InUse(void)
{
	if (numlist == 0 || plist == NULL || psize == NULL)
		return(0);
	unsigned long ctr, totalsize = numlist * (sizeof(void *) + sizeof(unsigned long));
	for (ctr = 0; ctr < numlist; ctr++)
		totalsize += psize[ctr];
	return(totalsize);
}

//Reset a memory buffer
void ClassEMemory::Set(void *dest, unsigned char value, int count)
{
	ecMemSet(dest, value, count);
}

//Clear memory: 16 bits at once (warning: memory addresses must be divisible by 2)
void ClassEMemory::Set16(void *dest, unsigned long value, unsigned long count)
{
	if ((value & 255) == ((value >> 8) & 255))
		Set(dest, (unsigned char)(value & 255), count * 2);
	else
		EMemory_Set(dest, (value << 16) | value, count * 2);
}

//Clear memory: 24 bits at once
void ClassEMemory::Set24(void *dest, unsigned long value, unsigned long count)
{
	if ((value & 255) == ((value >> 8) & 255) && (value & 255) == ((value >> 16) & 255))
	{
		Set(dest, (unsigned char)(value & 255), count * 3);
		return;
	}
	unsigned char value1 = (unsigned char)((value & 255));
	unsigned char value2 = (unsigned char)((value >> 8) & 255);
	unsigned char value3 = (unsigned char)((value >> 16) & 255);
	unsigned char *pTar  = (unsigned char *)dest;
	unsigned char *pStop = (unsigned char *)pTar + (count * 3);
	while(pTar < pStop)
	{
		*pTar ++= value1;
		*pTar ++= value2;
		*pTar ++= value3;
	}
}

//Clear memory: 32 bits at once (warning: memory addresses must be divisible by 4)
void ClassEMemory::Set32(void *dest, unsigned long value, unsigned long count)
{
	if ((value & 255) == ((value >> 8) & 255) && (value & 255) == ((value >> 16) & 255) && (value & 255) == ((value >> 24) & 255))
		Set(dest, (unsigned char)(value & 255), count * 4);
	else
		EMemory_Set(dest, value, count * 4);
}

//Copy memory buffers
void ClassEMemory::Copy(void *dest, const void *src, int count, bool checkoverlap)
{
	if (dest == src || count == 0)
		return;
	if (src < dest && (char *)(src) + count > dest && checkoverlap)
		ecMemMove(dest, src, count);
	else
		ecMemCpy(dest, src, count);
}

//Compare 2 memory character buffers
int ClassEMemory::Compare(const void *src1, const void *src2, int memsize)
{
	register char *se, *s1 = (char *)src1, *s2 = (char *)src2;
	for (se = s1 + memsize; s1 < se; s1++, s2++)
		if (*s1 != *s2)
			return(*s1 - *s2);
	return(0);
}

//Compress data (also returns false when the compressed data is bigger than the original)
bool ClassEMemory::Compress(void *dst, unsigned long &dst_size, const void *src, unsigned long src_size)
{
	if (src == NULL || src_size == 0)
		return(false);
	unsigned char cur_str[256];
	bool founderror = false;
	unsigned long prevfound = 0;
	unsigned long code, ctr, found, cur_str_size = 0;
	unsigned char *cdata = (unsigned char *)dst, *sdata = (unsigned char *)src;
	unsigned long codesize = 512, fcode = 0;
	unsigned char bituse = 0, bitsize = 9;
	if (str_index != NULL)
	{
		delete str_index;
		str_index = NULL;
	}
	if (str_len != NULL)
	{
		delete str_len;
		str_len = NULL;
	}
	str_index = new unsigned long[4096];
	if (str_index == NULL)
		return(false);
	str_len   = new unsigned char[4096];
	if (str_len == NULL)
	{
		delete str_index;
		str_index = NULL;
		return(false);
	}
	num_str  = 0;
	dst_size = 16;
	for (ctr = 0; ctr < src_size; ctr++)
	{
		cur_str[cur_str_size] = sdata[ctr];
		cur_str_size++;
		if (cur_str_size < 192)
			found = FindString(sdata, cur_str, cur_str_size);
		else
			found = 0;
		if (found == 0)
		{
			AddCmpString(cdata, ctr, cur_str_size);
			code = prevfound - 1;
			fcode = (fcode << bitsize) | code;
			bituse += bitsize;
			while (bituse >= 8)
			{
				cdata[dst_size] = (unsigned char)(fcode >> (bituse - 8));
				dst_size++;
				bituse -= 8;
			}
			if (num_str + 255 >= 4000)
			{
				codesize = 512;
				bitsize = 9;
				num_str = 0;
			}
			if (num_str + 255 >= codesize)
			{
				codesize *= 2;
				bitsize++;
			}
			cur_str[0] = cur_str[cur_str_size - 1];
			cur_str_size = 1;
			prevfound = cur_str[0] + 1;
			if (dst_size + 6 >= src_size)
			{
				founderror = true;
				break;
			}
		}
		else
			prevfound = found;
	}
	if (founderror)
	{
		delete str_index;
		delete str_len;
		str_index = NULL;
		str_len = NULL;
		return(false);
	}
	found = FindString(sdata, cur_str, cur_str_size);
	if (found != 0)
	{
		code = found - 1;
		fcode = (fcode << bitsize) | code;
		bituse += bitsize;
		while (bituse >= 8)
		{
			cdata[dst_size] = (unsigned char)(fcode >> (bituse - 8));
			dst_size++;
			bituse -= 8;
		}
		if (bituse != 0)
		{
			cdata[dst_size] = (unsigned char)(fcode << (8 - bituse));
			dst_size++;
		}
	}
	delete str_index;
	delete str_len;
	str_index = NULL;
	str_len = NULL;
	Copy(cdata + 8, &dst_size, 4);
	Copy(cdata + 12, &src_size, 4);
	return(true);
}

//Uncompress data
bool ClassEMemory::UnCompress(void *dst, unsigned long &dst_size, const void *src, unsigned long src_size)
{
	if (src == NULL || src_size == 0)
		return(false);
	unsigned char cur_str[256];
	unsigned char *cdata = (unsigned char *)src, *udata = (unsigned char *)dst;
	unsigned long fcode = 0, d_pos = 0, s_pos = 16, cur_str_size = 0, codesize = 512;
	unsigned short code, oldcode;
	unsigned char bituse = 0, bitsize = 9;
	Copy(&dst_size, cdata + 12, 4);
	if (str_index != NULL)
	{
		delete str_index;
		str_index = NULL;
	}
	if (str_len != NULL)
	{
		delete str_len;
		str_len = NULL;
	}
	str_index = new unsigned long[4096];
	if (str_index == NULL)
		return(false);
	str_len   = new unsigned char[4096];
	if (str_len == NULL)
	{
		delete str_index;
		str_index = NULL;
		return(false);
	}
	num_str = 0;
	while (s_pos < src_size)
	{
		while (bituse < bitsize)
		{
			if (s_pos >= src_size)
				break;
			fcode = (fcode << 8) | cdata[s_pos];
			bituse += 8;
			s_pos++;
		}
		if (bituse < bitsize)
			break;
		code = (unsigned short)((fcode >> (bituse - bitsize)) & (codesize - 1));
		bituse -= bitsize;
		if (cur_str_size == 0)
		{
			udata[d_pos] = (unsigned char)code;
			cur_str[0]   = (unsigned char)code;
			cur_str_size = 1;
			d_pos++;
			oldcode = code;
		}
		else
		{
			if (code < num_str + 256)
			{
				AddUCmpString(udata, d_pos, cur_str_size + 1);
				GetString(udata, cur_str, cur_str_size, code);
				Copy(&udata[d_pos], cur_str, cur_str_size);
				d_pos += cur_str_size;
				oldcode = code;
			}
			else
			{
				cur_str[cur_str_size] = cur_str[0];
				cur_str_size++;
				Copy(&udata[d_pos], cur_str, cur_str_size);
				d_pos += cur_str_size;
				AddUCmpString(udata, d_pos - 1, cur_str_size);
				oldcode = code;
			}
			if (num_str + 256 >= 4000)
			{
				cur_str_size = 0;
				codesize = 512;
				bitsize = 9;
				num_str = 0;
			}
			if (num_str + 256 >= codesize)
			{
				codesize *= 2;
				bitsize++;
			}
		}
	}
	delete str_index;
	delete str_len;
	str_index = NULL;
	str_len = NULL;
	return(true);
}

//Compress data using ZLib (also returns false when the compressed data is bigger than the original)
bool ClassEMemory::CompressZLib(void *dst, unsigned long &dst_size, const void *src, unsigned long src_size)
{
	if (CZLib::Compress((unsigned char *)dst, &dst_size, (const unsigned char *)src, src_size) == Z_OK)
		return(true);
	return(false);
}

//Uncompress data using ZLib
bool ClassEMemory::UnCompressZLib(void *dst, unsigned long &dst_size, const void *src, unsigned long src_size, bool noheaders)
{
	if (CZLib::Uncompress((unsigned char *)dst, &dst_size, (const unsigned char *)src, src_size, noheaders) == Z_OK)
		return(true);
	return(false);
}
