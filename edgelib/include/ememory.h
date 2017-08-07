// ememory.h
// The memory allocation, deallocation and management module
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED    //Can only be included by the main engine
#ifndef EMEMORY_INCLUDED //Make sure the module isn't included multiple times
#define EMEMORY_INCLUDED

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class ClassEMemory
{
	friend class EHTTPStack;
	private:
		void **plist;
		unsigned long *psize;
		unsigned long *str_index;
		unsigned char *str_len;
		unsigned long numlist;
		unsigned long num_str;
		unsigned long FindString(unsigned char *fulldata, unsigned char *str, unsigned long str_size);
		void AddCmpString(unsigned char *fulldata, unsigned long srdst_size, unsigned long str_size);
		void AddUCmpString(unsigned char *fulldata, unsigned long srdst_size, unsigned long str_size);
		void GetString(unsigned char *fulldata, unsigned char *str, unsigned long &str_size, unsigned long code);
		unsigned long static Adler32(void *buffer, unsigned long buflen);
	public:
		ClassEMemory(void);
		~ClassEMemory(void);
		void static *Alloc(unsigned long memsize);
		void static *ReAlloc(void *ptr, unsigned long memsize);
		void static DeAlloc(void *ptr);
		void *AllocList(unsigned long memsize);
		void DeAllocList(void *ptr);
		unsigned long InUse(void);
		void static Set(void *dest, unsigned char value, int count);
		void static Set16(void *dest, unsigned long value, unsigned long count);
		void static Set24(void *dest, unsigned long value, unsigned long count);
		void static Set32(void *dest, unsigned long value, unsigned long count);
		void static Copy(void *dest, const void *src, int count, bool checkoverlap = true);
		int static Compare(const void *src1, const void *src2, int memsize);
		bool Compress(void *dst, unsigned long &dst_size, const void *src, unsigned long src_size);
		bool UnCompress(void *dst, unsigned long &dst_size, const void *src, unsigned long src_size);
		bool static CompressZLib(void *dst, unsigned long &dst_size, const void *src, unsigned long src_size);
		bool static UnCompressZLib(void *dst, unsigned long &dst_size, const void *src, unsigned long src_size, bool noheaders = false);
};//The memory class

#endif
#endif
