// emod_math.cpp
// Math class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "eraw.h"
#include "emath.h"
#include "ememory.h"
#include "evtimer.h"
#include "econsole.h"

#include "extra/verinfo.h"

#if defined(DEVICE_WIN32)
	#define EDGE_TO64(x) ((__int64)x)
#else
	#define EDGE_TO64(x) ((long long)x)
#endif

/////////////////////////////////////////////////////////////////////
// Constant data                                                   //
/////////////////////////////////////////////////////////////////////

const unsigned long edge_rndget_mod[] = {
	0xA7B93291, 0x0389AF83, 0x4CD923AA, 0x823AF340, 0xDF923800, 0xEF7FAB92, 0x923ACDFA, 0xA7239AA3,
	0x93401034, 0xAB49DC34, 0x9106BD8F, 0x0239F1CB, 0xD01750DD, 0x0923D01E, 0x304DC09F, 0x1204FF31
};

const unsigned long edge_rndseed_mod[] = {
	0x0923AB23, 0xDF920D20, 0x03485921, 0xDF230AAD, 0xDAAF239B, 0xEBA13DAF, 0xCE238D01, 0x01592DDE,
	0xCC2309DE, 0xDE395056, 0x205DFAB2, 0x19681549, 0xDBAFF941, 0xAB8DF12C, 0xCD059DD3, 0xFF185FAB
};

/////////////////////////////////////////////////////////////////////
// ClassEMath::Pure functions                                      //
/////////////////////////////////////////////////////////////////////

//Return 1 if value equals to zero
int inline ClassEMath_ShiftNoZero(int value, int shift)
{
	if ((value >> shift) == 0)
	{
		if (value < 0)
			return(-1);
		return(1);
	}
	return(value >> shift);
}

/////////////////////////////////////////////////////////////////////
// ClassEMath::Private                                             //
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// ClassEMath::Public                                              //
/////////////////////////////////////////////////////////////////////

//ClassEMath: constructor
ClassEMath::ClassEMath(void)
{
	etblsin = NULL;
	etblcos = NULL;
	mt_index = 0;
}

//ClassEMath: destructor
ClassEMath::~ClassEMath(void)
{
	if (etblsin != NULL)
	{
		delete etblsin;
		etblsin = NULL;
	}
}

//Prepare the class by creating lookup tables
void ClassEMath::Init(void)
{
	long ctr;
	if (etblsin != NULL)
	{
		delete etblsin;
		etblsin = NULL;
	}
	etblsin = new EINT32[1024 * 5];
	if (etblsin == NULL)
		return;
	for (ctr = 0; ctr < 1024; ctr++)
	{
		etblsin[ctr] = emathtable[ctr];
		etblsin[ctr + 1024] = emathtable[1023 - ctr];
		etblsin[ctr + 4096] = emathtable[ctr];
	}
	for (ctr = 0; ctr < 2048; ctr++)
		etblsin[ctr + 2048] = -etblsin[ctr];
	etblcos = &etblsin[1024];
	SetRandSeed(ClassERTimer::Count());
}

//Return a fixed point sinus value (must have called Init())
long ClassEMath::Sin(unsigned long index)
{
	if (etblsin != NULL)
		return(etblsin[index % 4096]);
	return(0);
}

//Return a fixed point cosinus value (must have called Init())
long ClassEMath::Cos(unsigned long index)
{
	if (etblcos != NULL)
		return(etblcos[index % 4096]);
	return(0);
}

//Return the inverted sinus value (must have called Init())
unsigned long ClassEMath::InvSin(long value)
{
	if (etblsin != NULL)
	{
		if (value <= -65536)
			return(3072);
		if (value >= 65536)
			return(1024);
		unsigned long ctr = 512, addctr = 256, foundmatch = 0, checkval = ClassEMath::Abs(value);
		long checkdiff = etblsin[512] - checkval;
		while(1)
		{
			if (addctr == 0)
			{
				if (Abs(etblsin[ctr - 1] - checkval) < Abs(checkdiff))
				{
					ctr--;
					break;
				}
				else if (Abs(etblsin[ctr + 1] - checkval) < Abs(checkdiff))
				{
					ctr++;
					break;
				}
				break;
			}
			if (Abs(etblsin[ctr - 1] - checkval) > Abs(checkdiff) && Abs(etblsin[ctr + 1] - checkval) > Abs(checkdiff) || checkdiff == 0)
				break;
			if (checkdiff > 0)
				ctr -= addctr;
			else
				ctr += addctr;
			addctr /= 2;
			checkdiff = etblsin[ctr] - checkval;
		}
		foundmatch = ctr;
		if (value < 0)
		{
			if (foundmatch == 1024)
				foundmatch--;
			return((4095 - foundmatch) % 4096);
		}
		return(foundmatch);
	}
	return(0);
}

//Return the inverted sinus value (must have called Init())
unsigned long ClassEMath::InvCos(long value)
{
	if (etblcos != NULL)
		return((4096 + 1023 - InvSin(value)) % 4096);
	return(0);
}

//Return a fixed point tangent value (must have called Init())
long ClassEMath::Tan(unsigned long index)
{
	if (etblsin != NULL && etblcos != NULL)
	{
		if (index % 4096 == 1024)
			return(0x7FFFFFFF);
		else if (index % 4096 == 3072)
			return(-0x7FFFFFFF);
		long sinval = Sin(index) << 14, cosval = Cos(index) >> 2;
		if (cosval == 0)
			return(0x7FFFFFFF);
		return(sinval / cosval);
	}
	return(0);
}

//Return a fixed point cotangent value (must have called Init())
long ClassEMath::Cotan(unsigned long index)
{
	if (etblsin != NULL && etblcos != NULL)
	{
		long sinval = Sin(index) << 14, cosval = Cos(index) >> 2;
		if (sinval == 0)
			return(0x7FFFFFFF);
		return(cosval / sinval);
	}
	return(0);
}

//Return the inverted tangent value (must have called Init())
unsigned long ClassEMath::InvTan(long value)
{
	if (etblsin != NULL && etblcos != NULL)
	{
		unsigned long ctr = 512, addctr = 256, foundmatch = 0, checkval = ClassEMath::Abs(value);
		long checkdiff = Tan(512) - checkval;
		while(1)
		{
			if (addctr == 0)
			{
				if (Abs(Tan(ctr - 1) - checkval) < Abs(checkdiff))
				{
					ctr--;
					break;
				}
				else if (Abs(Tan(ctr + 1) - checkval) < Abs(checkdiff))
				{
					ctr++;
					break;
				}
				break;
			}
			if (Abs(Tan(ctr - 1) - checkval) > Abs(checkdiff) && Abs(Tan(ctr + 1) - checkval) > Abs(checkdiff) || checkdiff == 0)
				break;
			if (checkdiff > 0)
				ctr -= addctr;
			else
				ctr += addctr;
			addctr /= 2;
			checkdiff = Tan(ctr) - checkval;
		}
		foundmatch = ctr;
		if (foundmatch == 1024)
			foundmatch--;
		if (value < 0)
			return((4095 - foundmatch) % 4096);
		return(foundmatch);
	}
	return(0);
}

//Return the inverted tangent value using 2 parameters (must have called Init())
unsigned long ClassEMath::InvTan2(long xdelta, long ydelta)
{
	if (etblsin != NULL && etblcos != NULL)
	{
		long deltav;
		if (xdelta == 0 || ydelta == 0)
		{
			if (xdelta == 0 && ydelta == 0)
				return(0);
			if (xdelta == 0)
			{
				if (ydelta < 0)
					return(2048);
				else
					return(0);
			}
			if (xdelta < 0)
				return(3072);
			return(1024);
		}
		if (ClassEMath::Abs(xdelta) < 0x8000)
			deltav = (xdelta << 16) / ClassEMath_ShiftNoZero(ydelta, 0);
		else if (ClassEMath::Abs(xdelta) < 0x10000)
			deltav = (xdelta << 15) / ClassEMath_ShiftNoZero(ydelta, 1);
		else if (ClassEMath::Abs(xdelta) < 0x20000)
			deltav = (xdelta << 14) / ClassEMath_ShiftNoZero(ydelta, 2);
		else if (ClassEMath::Abs(xdelta) < 0x40000)
			deltav = (xdelta << 13) / ClassEMath_ShiftNoZero(ydelta, 3);
		else if (ClassEMath::Abs(xdelta) < 0x80000)
			deltav = (xdelta << 12) / ClassEMath_ShiftNoZero(ydelta, 4);
		else if (ClassEMath::Abs(xdelta) < 0x100000)
			deltav = (xdelta << 11) / ClassEMath_ShiftNoZero(ydelta, 5);
		else if (ClassEMath::Abs(xdelta) < 0x200000)
			deltav = (xdelta << 10) / ClassEMath_ShiftNoZero(ydelta, 6);
		else if (ClassEMath::Abs(xdelta) < 0x400000)
			deltav = (xdelta << 9) / ClassEMath_ShiftNoZero(ydelta, 7);
		else if (ClassEMath::Abs(xdelta) < 0x800000)
			deltav = (xdelta << 8) / ClassEMath_ShiftNoZero(ydelta, 8);
		else if (ClassEMath::Abs(xdelta) < 0x1000000)
			deltav = (xdelta << 7) / ClassEMath_ShiftNoZero(ydelta, 9);
		else if (ClassEMath::Abs(xdelta) < 0x2000000)
			deltav = (xdelta << 6) / ClassEMath_ShiftNoZero(ydelta, 10);
		else if (ClassEMath::Abs(xdelta) < 0x4000000)
			deltav = (xdelta << 5) / ClassEMath_ShiftNoZero(ydelta, 11);
		else if (ClassEMath::Abs(xdelta) < 0x8000000)
			deltav = (xdelta << 4) / ClassEMath_ShiftNoZero(ydelta, 12);
		else if (ClassEMath::Abs(xdelta) < 0x10000000)
			deltav = (xdelta << 3) / ClassEMath_ShiftNoZero(ydelta, 13);
		else if (ClassEMath::Abs(xdelta) < 0x20000000)
			deltav = (xdelta << 2) / ClassEMath_ShiftNoZero(ydelta, 14);
		else if (ClassEMath::Abs(xdelta) < 0x40000000)
			deltav = (xdelta << 1) / ClassEMath_ShiftNoZero(ydelta, 15);
		else
			deltav = xdelta / ClassEMath_ShiftNoZero(ydelta, 16);
		xdelta = InvTan(deltav);
		if (ydelta < 0)
		{
			if (xdelta <= 0)
				return(xdelta + 2048);
			return((xdelta + 4096 - 2048) % 4096);
		}
		return(xdelta);
	}
    return(0);
}

//Set random generator seed value
void ClassEMath::SetRandSeed(unsigned long seed)
{
	randseed = seed;
}

//Get random generator seed value
unsigned long ClassEMath::GetRandSeed(void)
{
	return(randseed);
}

//Calculate random value
unsigned long ClassEMath::Rand()
{
	unsigned short seed1 = (unsigned short)(randseed >> 16);
	unsigned short seed2 = (unsigned short)(randseed & 65535);
	seed1 = seed1 * 1103515245 + 12345;
	seed2 = (unsigned short)((seed2 ^ edge_rndseed_mod[seed1 & 15]) * 23743 + 193);
	randseed = (seed1 << 16) | seed2;
	return(randseed ^ edge_rndget_mod[seed1 & 15]);
}

//Calculate random value
unsigned long ClassEMath::Rand(long min, long max)
{
	if (min >= max)
		return(min);
	return(Rand() % (max - min + 1) + min);
}

// Generate an array of 624 untempered numbers
void ClassEMath::mt_generate() 
{
	for (int i = 0; i < 623; i++) 
	{
		//int y = 32nd bit of(MT[i]) + last 31 bits of(MT[(i+1) mod 624]) 
		//int y = (mt_seeds[i] >> 31) + (mt_seeds[(i+1) % 624] & 0x7fffffff);
		int y = (mt_seeds[i] & 0x80000000) + (mt_seeds[(i+1) % 624] & 0x7fffffff);
	    
		mt_seeds[i] = mt_seeds[(i + 397) % 624] ^ (y >> 1);
		if (y & 1) 
		{ // y is odd
			mt_seeds[i] ^= 0x9908b0df; // 0x9908b0df
		}
	}
} 
 
// Initialize the generator from a seed
void ClassEMath::MT_Seed(unsigned long seed) 
{
	mt_seeds[0] = seed;
	for(int i = 1; i < 624; i++) 
	{ 
		mt_seeds[i] = (0x6c078965 * (mt_seeds[i-1] ^ ((mt_seeds[i-1] >> 30))) + i); // 0x6c078965
	}
	mt_index = 0;
	mt_generate(); // discard the first outputs
}
 
// Extract a tempered pseudorandom number based on the index-th value,
// calling generateNumbers() every 624 numbers
unsigned long ClassEMath::MT_Rand() 
{
	if (mt_index == 0) {
		mt_generate();
	}
     
	int y = mt_seeds[mt_index];
     
	y ^= (y >> 11);
	y ^= ((y << 7) & (0x9d2c5680)); 
	y ^= ((y << 15) & (0xefc60000)); 
	y ^= (y >> 18);
     
	mt_index++;
	if (mt_index == 624) mt_index = 0;
	return y;
}


//Returns the absolute value
long ClassEMath::Abs(long value)
{
	return(value < 0 ? -value : value);
}

//Returns a square root value
unsigned long ClassEMath::IntSqrt(long value)
{
	if (value <= 0)
		return(0);
	int result = 0x8000, rpos = 0x8000;
	while(rpos)
	{
		if (result * result > value)
		{
			result -= rpos;
			result += (rpos >> 1);
		}
		else if (result * result < value)
			result += (rpos >> 1);
		rpos >>= 1;
	}
	return(result);
}

//Convert a float to fixed point (emulated)
long ClassEMath::FloatToFixed(unsigned long value, int fixedprecision)
{
	long f_sign, f_value, f_fragment;
	unsigned long ctr, mask, i_value, intf = value;
	f_sign = (long)((intf & 0x80000000) >> 31);
	f_fragment = (long)((intf & 0x7F800000) >> 23);
	f_value = (long)(intf & 0x7FFFFF);
	if (f_fragment == 255)
	{
		if (f_value == 0) //Infinite
		{
			if (f_sign)
				return(-0x7FFFFFFF);
			else
				return(0x7FFFFFFF);
		}
		else
			return(0); //Not a number
	}
	if (f_sign == 0)
		f_sign = 1;
	else
		f_sign = -1;
	f_fragment -= 127;
	f_value += 0x800000;
	i_value = 0;
	mask = 0x800000;
	if ((-23 + fixedprecision + f_fragment) < -31)
		return(0);
	if ((-23 + fixedprecision + f_fragment) > 31)
		return(0x7FFFFFFF * f_sign);
	for (ctr = 0; ctr < 24; ctr++)
	{
		long shift = -23 + fixedprecision + f_fragment;
		if (shift < 0)
			i_value += (f_value & mask) >> ClassEMath::Abs(shift);
		else if (shift > 0)
			i_value += (f_value & mask) << shift;
		else
			i_value += (f_value & mask);
		mask >>= 1;
	}
	return(i_value * f_sign);
}

//Convert fixed point to a float (emulated)
unsigned long ClassEMath::FixedToFloat(long value, int fixedprecision)
{
	long f_sign, f_value, f_fragment;
	unsigned long ctr, mask, i_value;
	f_fragment = 31 - fixedprecision;
	i_value = ClassEMath::Abs(value);
	while(!(i_value & 0x80000000))
	{
		if (i_value == 0)
		{
			//return 0?
			break;
		}
		i_value <<= 1;
		f_fragment--;
	}
	i_value = ClassEMath::Abs(value);
	f_value = 0;
	mask = 0x800000;
	for (ctr = 0; ctr < 24; ctr++)
	{
		long shift = -23 + fixedprecision + f_fragment;
		if (shift < 0)
			f_value += (i_value << ClassEMath::Abs(shift)) & mask;
		else if (shift > 0)
			f_value += (i_value >> shift) & mask;
		else
			f_value += i_value & mask;
		mask >>= 1;
	}
	f_value -= 0x800000;
	f_fragment += 127;
	if (value < 0)
		f_sign = 1;
	else
		f_sign = 0;
	return((f_sign << 31) | (f_fragment << 23) | f_value);
}

//Load the identity matrix
void ClassEMath::MatrixLoadIdentity(long *matrix)
{
	for (unsigned char ctr = 0; ctr < 16; ctr++)
	{
		if ((ctr >> 2) == (ctr & 3))
			matrix[ctr] = 4096;
		else
			matrix[ctr] = 0;
	}
}

//Set matrix rotation
void ClassEMath::MatrixSetRotate(unsigned short x, unsigned short y, unsigned short z, long *matrix)
{
	long cr = Cos(x) >> 2;
	long sr = Sin(x) >> 2;
	long cp = Cos(y) >> 2;
	long sp = Sin(y) >> 2;
	long cy = Cos(z) >> 2;
	long sy = Sin(z) >> 2;
	matrix[0] = (cp * cy) >> 16;
	matrix[1] = (cp * sy) >> 16;
	matrix[2] = -sp >> 2;
	long srsp = (sr * sp) >> 14;
	long crsp = (cr * sp) >> 14;
	matrix[4] = ((srsp * cy) >> 16) - ((cr * sy) >> 16);
	matrix[5] = ((srsp * sy) >> 16) + ((cr * cy) >> 16);
	matrix[6] = (sr * cp) >> 16;
	matrix[8] = ((crsp * cy) >> 16) + ((sr * sy) >> 16);
	matrix[9] = ((crsp * sy) >> 16) - ((sr * cy) >> 16);
	matrix[10] = (cr * cp) >> 16;
}

//Set matrix translation
void ClassEMath::MatrixSetTranslate(long x, long y, long z, long *matrix)
{
	matrix[12] = x;
	matrix[13] = y;
	matrix[14] = z;
}

//Multiply 2 matrices
void ClassEMath::MatrixMultiply(long *dst, const long *src)
{
	long matrixtmp[16];
	unsigned long mc;
	for (mc = 0; mc < 16; mc++)
		matrixtmp[mc] = dst[mc];
	dst[0]  = (long)(((EDGE_TO64(matrixtmp[0])*EDGE_TO64(src[0])) >> 12) + ((EDGE_TO64(matrixtmp[4])*EDGE_TO64(src[1])) >> 12) + ((EDGE_TO64(matrixtmp[8])*EDGE_TO64(src[2])) >> 12));
	dst[1]  = (long)(((EDGE_TO64(matrixtmp[1])*EDGE_TO64(src[0])) >> 12) + ((EDGE_TO64(matrixtmp[5])*EDGE_TO64(src[1])) >> 12) + ((EDGE_TO64(matrixtmp[9])*EDGE_TO64(src[2])) >> 12));
	dst[2]  = (long)(((EDGE_TO64(matrixtmp[2])*EDGE_TO64(src[0])) >> 12) + ((EDGE_TO64(matrixtmp[6])*EDGE_TO64(src[1])) >> 12) + ((EDGE_TO64(matrixtmp[10])*EDGE_TO64(src[2])) >> 12));
	dst[3]  = 0;
	dst[4]  = (long)(((EDGE_TO64(matrixtmp[0])*EDGE_TO64(src[4])) >> 12) + ((EDGE_TO64(matrixtmp[4])*EDGE_TO64(src[5])) >> 12) + ((EDGE_TO64(matrixtmp[8])*EDGE_TO64(src[6])) >> 12));
	dst[5]  = (long)(((EDGE_TO64(matrixtmp[1])*EDGE_TO64(src[4])) >> 12) + ((EDGE_TO64(matrixtmp[5])*EDGE_TO64(src[5])) >> 12) + ((EDGE_TO64(matrixtmp[9])*EDGE_TO64(src[6])) >> 12));
	dst[6]  = (long)(((EDGE_TO64(matrixtmp[2])*EDGE_TO64(src[4])) >> 12) + ((EDGE_TO64(matrixtmp[6])*EDGE_TO64(src[5])) >> 12) + ((EDGE_TO64(matrixtmp[10])*EDGE_TO64(src[6])) >> 12));
	dst[7]  = 0;
	dst[8]  = (long)(((EDGE_TO64(matrixtmp[0])*EDGE_TO64(src[8])) >> 12) + ((EDGE_TO64(matrixtmp[4])*EDGE_TO64(src[9])) >> 12) + ((EDGE_TO64(matrixtmp[8])*EDGE_TO64(src[10])) >> 12));
	dst[9]  = (long)(((EDGE_TO64(matrixtmp[1])*EDGE_TO64(src[8])) >> 12) + ((EDGE_TO64(matrixtmp[5])*EDGE_TO64(src[9])) >> 12) + ((EDGE_TO64(matrixtmp[9])*EDGE_TO64(src[10])) >> 12));
	dst[10] = (long)(((EDGE_TO64(matrixtmp[2])*EDGE_TO64(src[8])) >> 12) + ((EDGE_TO64(matrixtmp[6])*EDGE_TO64(src[9])) >> 12) + ((EDGE_TO64(matrixtmp[10])*EDGE_TO64(src[10])) >> 12));
	dst[11] = 0;
	dst[12] = (long)(((EDGE_TO64(matrixtmp[0])*EDGE_TO64(src[12])) >> 12) + ((EDGE_TO64(matrixtmp[4])*EDGE_TO64(src[13])) >> 12) + ((EDGE_TO64(matrixtmp[8])*(src[14])) >> 12) + matrixtmp[12]);
	dst[13] = (long)(((EDGE_TO64(matrixtmp[1])*EDGE_TO64(src[12])) >> 12) + ((EDGE_TO64(matrixtmp[5])*EDGE_TO64(src[13])) >> 12) + ((EDGE_TO64(matrixtmp[9])*(src[14])) >> 12) + matrixtmp[13]);
	dst[14] = (long)(((EDGE_TO64(matrixtmp[2])*EDGE_TO64(src[12])) >> 12) + ((EDGE_TO64(matrixtmp[6])*EDGE_TO64(src[13])) >> 12) + ((EDGE_TO64(matrixtmp[10])*(src[14])) >> 12) + matrixtmp[14]);
	dst[15] = 1;
}

//Reverse the matrix operation
void ClassEMath::MatrixReverse(long *matrix)
{
	long trans[] = {
		4096, 0, 0, 0,
		0, 4096, 0, 0,
		0, 0, 4096, 0,
		-matrix[12], -matrix[13], -matrix[14], 4096
	};
	long tmp;
	tmp = matrix[1];
	matrix[1] = matrix[4];
	matrix[4] = tmp;
	tmp = matrix[2];
	matrix[2] = matrix[8];
	matrix[8] = tmp;
	tmp = matrix[6];
	matrix[6] = matrix[9];
	matrix[9] = tmp;
	matrix[12] = 0;
	matrix[13] = 0;
	matrix[14] = 0;
	MatrixMultiply(matrix, trans);
}
