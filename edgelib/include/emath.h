// emath.h
// A math module for fixed point math
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED   //Can only be included by the main engine
#ifndef EMATH_INCLUDED  //Make sure the module isn't included multiple times
#define EMATH_INCLUDED

#define EMATH_FIXEDPI   205887

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

#ifdef FixedToFloat
#undef FixedToFloat
#undef FloatToFixed
#endif

class ClassEMath
{
	private:
		EINT32 *etblsin;
		EINT32 *etblcos;
		unsigned long randseed;
		
		unsigned long mt_seeds[624];
		int mt_index;
		void mt_generate();
		
	public:
		ClassEMath(void);
		~ClassEMath(void);
		void Init(void);
		long Sin(unsigned long index);
		long Cos(unsigned long index);
		unsigned long InvSin(long value);
		unsigned long InvCos(long value);
		long Tan(unsigned long index);
		long Cotan(unsigned long index);
		unsigned long InvTan(long value);
		unsigned long InvTan2(long xdelta, long ydelta);
		void SetRandSeed(unsigned long seed);
		unsigned long GetRandSeed(void);
		unsigned long Rand();
		unsigned long Rand(long min, long max);
		void MT_Seed(unsigned long seed);
		unsigned long MT_Rand(void);
		long static Abs(long value);
		unsigned long static IntSqrt(long value);
		long static FloatToFixed(unsigned long value, int fixedprecision = 16);
		unsigned long static FixedToFloat(long value, int fixedprecision = 16);
		void static MatrixLoadIdentity(long *matrix);
		void MatrixSetRotate(unsigned short x, unsigned short y, unsigned short z, long *matrix);
		void static MatrixSetTranslate(long x, long y, long z, long *matrix);
		void static MatrixMultiply(long *dst, const long *src);
		void static MatrixReverse(long *matrix);
};

#endif
#endif
