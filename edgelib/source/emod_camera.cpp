// emod_camera.cpp
// Camera class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED

#include "estdc.h"
#include "esound.h"
#include "eloader.h"
#include "edisplay.h"
#include "einput.h"
#include "ememory.h"
#include "evtimer.h"
#include "efile.h"
#include "emath.h"
#include "eini.h"
#include "ereg.h"
#include "econsole.h"
#include "edevice.h"
#include "edgeframe.h"
#include "ecamera.h"
#include "core/edgecore.h"

ClassECamera::ClassECamera(ClassEdge * framework)
    : framework(framework)
{
    this->cameraclass = ecCamCreate(framework, this);
}

ClassECamera::~ClassECamera()
{
    ecCamDelete(cameraclass);
}

int ClassECamera::GetCameraCount()
{
    return ecCamDeviceCount();
}

ERESULT ClassECamera::RequestCamera(int index)
{
    return ecCamRequestCamera(cameraclass, index);
}

ERESULT ClassECamera::RequestStart()
{
    return ecCamStartCapture(cameraclass);
}

ERESULT ClassECamera::RequestStop()
{
    return ecCamStopCapture(cameraclass);
}

int ClassECamera::GetModeCount()
{
    return ecCamGetResolutions(cameraclass);
}

ERESULT ClassECamera::RequestResolution(int index)
{
    return ecCamSetResolution(cameraclass, index);
}

void ClassECamera::GetModeInformation(int index, E_CAMERAMODE & mode)
{
    if (index < 0) return;
    ecCamGetResolution(cameraclass, index, &mode);
}

int ClassECamera::GetCameraIndex()
{
    return ecCamGetIndex(cameraclass);
}

int ClassECamera::GetCameraState()
{
    return ecCamGetState(cameraclass);
}

int ClassECamera::GetCameraOrientation()
{
    return ecCamGetOrientation(cameraclass);
}

void ClassECamera::GetCurrentMode(E_CAMERAMODE & mode)
{
    if (ecCamGetState(cameraclass) == ECS_INVALID) return;

    int index = ecCamGetCurrentResolution(cameraclass);
    GetModeInformation(index, mode);
}

int ClassECamera::GetCurrentModeIndex()
{
    return ecCamGetCurrentResolution(cameraclass);
}


// wanted "fast" conversion functions:
// there are > 15 formats, i.e. over 256 of possible pairs of formats
// while usually, the destination format is known

// YV'12 -> ARGB32 1:1, YU'12 -> ARGB32 1:1, can be modified to include other planar formats
static void ConvYUVpToRGBA32_11(unsigned char * data_out, E2DSURFACEINFO * out_info, const E_CAMERAFRAME * data_in);

// WM
//  - RGB565 -> RGB565 1:2 (~20% of cases on windows mobile)
//  - RGB565 -> RGB565 x:x (~40% of cases on windows mobile)
//  - YV'12  -> RGB565 1:2 (~30% of cases on windows mobile)
//  - YV'12  -> RGB565 x:x (~60% of cases on windows mobile)

// Desktop (from a sample space of 1 item)
//  - ARGB32 -> ARGB32 x:x (for desktop use)

// Symbian (from a sample space of 1 item)
//  - YU'12  -> ARGB32 2:1 (seen on N95)
//  - YU'12  -> ARGB32 1:1 (seen on N95)

// Linux embedded
//  - YUYV   -> RGB565 1:1 (N900)
//  - YUYV   -> RGB565 x:x (N900)


// the "slow" conversion functions
static inline long ConvGetPixel(const E_CAMERAFRAME * data_in, int x, int y);
static inline void ConvSetPixel(unsigned char * data, E2DSURFACEINFO *, int x, int y, int c);
static inline long ConvFromYUV(long Y, long U, long V); // to BGRA32
static inline long ConvFromYUV_Inv(long Y, long U, long V); // to RGBA32

static inline long ConvGetPixel(const E_CAMERAFRAME * data_in, int x, int y)
{
    switch (data_in->format)
    {
        case EDSP_TRUE16: /* untested, byte order dependency? */
        {
            long data = *((short *)&(data_in->data[y * data_in->pitch + 2 * x]));
            return ((data & 0x1f) << 3) | ((data & 0x07e0) << 5) | ((data & 0xf800) << 8);
        }

		case EDSP_TRUE24: 
        {
            unsigned char * ptr = (unsigned char *) &(data_in->data[y * data_in->pitch + 3 * x]);
			return ptr[0] | (ptr[1] << 8) | (ptr[2] << 16);
        }

        case EDSP_TRUE32: /* byte order dependency fail! */
        {
            long data = *((EINT32 *)&(data_in->data[y * data_in->pitch + 4 * x]));
            return data & 0x00ffffff;
        }
			
		case EDSP_TRUE32_INV: /* byte order dependency fail! */
        {
            long data = *((EINT32 *)&(data_in->data[y * data_in->pitch + 4 * x]));
            return (data & 0x0000ff00) | ((data >> 16) & 0xff) | ((data & 0xff) << 16);
        }

        case EDSP_YUV420: /* untested */
        {
            long yblock = y >> 1;
            long xblock = x >> 1;
            long offset = (x & 1) + ((y & 1) << 1);
            long Y = data_in->data[6 * yblock * data_in->width + 6 * xblock + offset];
            long U = data_in->data[6 * yblock * data_in->width + 6 * xblock + 4];
            long V = data_in->data[6 * yblock * data_in->width + 6 * xblock + 5];
            return ConvFromYUV(Y,U,V);
        }

        case EDSP_YUV12:
        {
            long yblock = y >> 1;
            long xblock = x >> 1;
            long Y = data_in->data[y * data_in->width + x];
            long U = data_in->data[data_in->height * data_in->width + yblock * (data_in->width >> 1) + xblock];
            long V = data_in->data[data_in->height * data_in->width + ((data_in->height * data_in->width) >> 2) + yblock * (data_in->width >> 1) + xblock];
            return ConvFromYUV(Y,U,V);
        }
        
        case EDSP_YVU12: /* untested */
        {
            long yblock = y >> 1;
            long xblock = x >> 1;
            long Y = data_in->data[y * data_in->width + x];
            long V = data_in->data[data_in->height * data_in->width + yblock * (data_in->width >> 1) + xblock];
            long U = data_in->data[data_in->height * data_in->width + ((data_in->height * data_in->width) >> 2) + yblock * (data_in->width >> 1) + xblock];
            return ConvFromYUV(Y,U,V);
        }


        case EDSP_UYVY:
        {
            long base = 2 * y * data_in->width + 2 * (x & 0xfffffffe);
            long Y = data_in->data[2 * y * data_in->width + 2 * x + 1]; // base+1/base+3
            long U = data_in->data[base];
            long V = data_in->data[base+2];
            return ConvFromYUV(Y,U,V);
        }

	case EDSP_YUYV:
        {
            long base = 2 * y * data_in->width + 2 * (x & 0xfffffffe);
            long Y = data_in->data[2 * y * data_in->width + 2 * x]; // base+1/base+3
            long U = data_in->data[base+1];
            long V = data_in->data[base+3];
            return ConvFromYUV(Y,U,V);
        }

	case EDSP_YVYU:
        {
            long base = 2 * y * data_in->width + 2 * (x & 0xfffffffe);
            long Y = data_in->data[2 * y * data_in->width + 2 * x]; // base+1/base+3
            long U = data_in->data[base+3];
            long V = data_in->data[base+1];
            return ConvFromYUV(Y,U,V);
        }

        default:
            return 0xFF7F00;
    }
}

// U/V lookup tables (256 * 4 * 2 bytes)
const static long TabU[256] = 
{	0x32FEFE, 0x32FF00, 0x31FF02, 0x31FF04,
	0x30FF06, 0x30FF08, 0x30FF0A, 0x2FFF0C,
	0x2FFF0E, 0x2EFF10, 0x2EFF12, 0x2EFF14,
	0x2DFF16, 0x2DFF18, 0x2DFF1A, 0x2CFF1C,
	0x2CFF1E, 0x2BFF20, 0x2BFF22, 0x2BFF24,
	0x2AFF26, 0x2AFF28, 0x29FF2A, 0x29FF2C,
	0x29FF2E, 0x28FF30, 0x28FF32, 0x27FF34,
	0x27FF36, 0x27FF38, 0x26FF3A, 0x26FF3C,
	0x26FF3F, 0x25FF41, 0x25FF43, 0x24FF45,
	0x24FF47, 0x24FF49, 0x23FF4B, 0x23FF4D,
	0x22FF4F, 0x22FF51, 0x22FF53, 0x21FF55,
	0x21FF57, 0x20FF59, 0x20FF5B, 0x20FF5D,
	0x1FFF5F, 0x1FFF61, 0x1EFF63, 0x1EFF65,
	0x1EFF67, 0x1DFF69, 0x1DFF6B, 0x1DFF6D,
	0x1CFF6F, 0x1CFF71, 0x1BFF73, 0x1BFF75,
	0x1BFF77, 0x1AFF79, 0x1AFF7B, 0x19FF7D,
	0x19FF7F, 0x19FF81, 0x18FF83, 0x18FF85,
	0x17FF87, 0x17FF89, 0x17FF8B, 0x16FF8D,
	0x16FF8F, 0x15FF91, 0x15FF93, 0x15FF95,
	0x14FF97, 0x14FF99, 0x14FF9B, 0x13FF9D,
	0x13FF9F, 0x12FFA1, 0x12FFA3, 0x12FFA5,
	0x11FFA7, 0x11FFA9, 0x10FFAB, 0x10FFAD,
	0x10FFAF, 0xFFFB1, 0xFFFB3, 0xEFFB5,
	0xEFFB7, 0xEFFB9, 0xDFFBB, 0xDFFBD,
	0xDFFC0, 0xCFFC2, 0xCFFC4, 0xBFFC6,
	0xBFFC8, 0xBFFCA, 0xAFFCC, 0xAFFCE,
	0x9FFD0, 0x9FFD2, 0x9FFD4, 0x8FFD6,
	0x8FFD8, 0x7FFDA, 0x7FFDC, 0x7FFDE,
	0x6FFE0, 0x6FFE2, 0x5FFE4, 0x5FFE6,
	0x5FFE8, 0x4FFEA, 0x4FFEC, 0x4FFEE,
	0x3FFF0, 0x3FFF2, 0x2FFF4, 0x2FFF6,
	0x2FFF8, 0x1FFFA, 0x1FFFC, 0xFFFE,
	0x0, 0x2, 0xFFFF0004, 0xFFFF0006,
	0xFFFE0008, 0xFFFE000A, 0xFFFE000C, 0xFFFD000E,
	0xFFFD0010, 0xFFFC0012, 0xFFFC0014, 0xFFFC0016,
	0xFFFB0018, 0xFFFB001A, 0xFFFB001C, 0xFFFA001E,
	0xFFFA0020, 0xFFF90022, 0xFFF90024, 0xFFF90026,
	0xFFF80028, 0xFFF8002A, 0xFFF7002C, 0xFFF7002E,
	0xFFF70030, 0xFFF60032, 0xFFF60034, 0xFFF50036,
	0xFFF50038, 0xFFF5003A, 0xFFF4003C, 0xFFF4003E,
	0xFFF40041, 0xFFF30043, 0xFFF30045, 0xFFF20047,
	0xFFF20049, 0xFFF2004B, 0xFFF1004D, 0xFFF1004F,
	0xFFF00051, 0xFFF00053, 0xFFF00055, 0xFFEF0057,
	0xFFEF0059, 0xFFEE005B, 0xFFEE005D, 0xFFEE005F,
	0xFFED0061, 0xFFED0063, 0xFFEC0065, 0xFFEC0067,
	0xFFEC0069, 0xFFEB006B, 0xFFEB006D, 0xFFEB006F,
	0xFFEA0071, 0xFFEA0073, 0xFFE90075, 0xFFE90077,
	0xFFE90079, 0xFFE8007B, 0xFFE8007D, 0xFFE7007F,
	0xFFE70081, 0xFFE70083, 0xFFE60085, 0xFFE60087,
	0xFFE50089, 0xFFE5008B, 0xFFE5008D, 0xFFE4008F,
	0xFFE40091, 0xFFE30093, 0xFFE30095, 0xFFE30097,
	0xFFE20099, 0xFFE2009B, 0xFFE2009D, 0xFFE1009F,
	0xFFE100A1, 0xFFE000A3, 0xFFE000A5, 0xFFE000A7,
	0xFFDF00A9, 0xFFDF00AB, 0xFFDE00AD, 0xFFDE00AF,
	0xFFDE00B1, 0xFFDD00B3, 0xFFDD00B5, 0xFFDC00B7,
	0xFFDC00B9, 0xFFDC00BB, 0xFFDB00BD, 0xFFDB00BF,
	0xFFDB00C2, 0xFFDA00C4, 0xFFDA00C6, 0xFFD900C8,
	0xFFD900CA, 0xFFD900CC, 0xFFD800CE, 0xFFD800D0,
	0xFFD700D2, 0xFFD700D4, 0xFFD700D6, 0xFFD600D8,
	0xFFD600DA, 0xFFD500DC, 0xFFD500DE, 0xFFD500E0,
	0xFFD400E2, 0xFFD400E4, 0xFFD300E6, 0xFFD300E8,
	0xFFD300EA, 0xFFD200EC,	0xFFD200EE, 0xFFD200F0,
	0xFFD100F2, 0xFFD100F4, 0xFFD000F6, 0xFFD000F8,
	0xFFD000FA, 0xFFCF00FC, 0xFFCF00FE, 0xFFCE0100};

const static long TabV[256] = {
	0x68FF34, 0x67FF35, 0x66FF37, 0x66FF38,
	0x65FF3A, 0x64FF3B, 0x63FF3D, 0x62FF3F,
	0x62FF40, 0x61FF42, 0x60FF43, 0x5FFF45,
	0x5EFF47, 0x5DFF48, 0x5DFF4A, 0x5CFF4B,
	0x5BFF4D, 0x5AFF4F, 0x59FF50, 0x59FF52,
	0x58FF53, 0x57FF55, 0x56FF57, 0x55FF58,
	0x55FF5A, 0x54FF5B, 0x53FF5D, 0x52FF5F,
	0x51FF60, 0x50FF62, 0x50FF63, 0x4FFF65,
	0x4EFF67, 0x4DFF68, 0x4CFF6A, 0x4CFF6B,
	0x4BFF6D, 0x4AFF6F, 0x49FF70, 0x48FF72,
	0x48FF73, 0x47FF75, 0x46FF77, 0x45FF78,
	0x44FF7A, 0x43FF7B, 0x43FF7D, 0x42FF7F,
	0x41FF80, 0x40FF82, 0x3FFF83, 0x3FFF85,
	0x3EFF87, 0x3DFF88, 0x3CFF8A, 0x3BFF8B,
	0x3BFF8D, 0x3AFF8F, 0x39FF90, 0x38FF92,
	0x37FF93, 0x36FF95, 0x36FF97, 0x35FF98,
	0x34FF9A, 0x33FF9B, 0x32FF9D, 0x32FF9F,
	0x31FFA0, 0x30FFA2, 0x2FFFA3, 0x2EFFA5,
	0x2EFFA7, 0x2DFFA8, 0x2CFFAA, 0x2BFFAB,
	0x2AFFAD, 0x29FFAF, 0x29FFB0, 0x28FFB2,
	0x27FFB3, 0x26FFB5, 0x25FFB7, 0x25FFB8,
	0x24FFBA, 0x23FFBB, 0x22FFBD, 0x21FFBE,
	0x21FFC0, 0x20FFC2, 0x1FFFC3, 0x1EFFC5,
	0x1DFFC6, 0x1CFFC8, 0x1CFFCA, 0x1BFFCB,
	0x1AFFCD, 0x19FFCE, 0x18FFD0, 0x18FFD2,
	0x17FFD3, 0x16FFD5, 0x15FFD6, 0x14FFD8,
	0x14FFDA, 0x13FFDB, 0x12FFDD, 0x11FFDE,
	0x10FFE0, 0xFFFE2, 0xFFFE3, 0xEFFE5,
	0xDFFE6, 0xCFFE8, 0xBFFEA, 0xBFFEB,
	0xAFFED, 0x9FFEE, 0x8FFF0, 0x7FFF2,
	0x7FFF3, 0x6FFF5, 0x5FFF6, 0x4FFF8,
	0x3FFFA, 0x2FFFB, 0x2FFFD, 0x1FFFE,
	0x0, 0xFFFF0002, 0xFFFE0003, 0xFFFE0005,
	0xFFFD0006, 0xFFFC0008, 0xFFFB000A, 0xFFFA000B,
	0xFFFA000D, 0xFFF9000E, 0xFFF80010, 0xFFF70012,
	0xFFF60013, 0xFFF50015, 0xFFF50016, 0xFFF40018,
	0xFFF3001A, 0xFFF2001B, 0xFFF1001D, 0xFFF1001E,
	0xFFF00020, 0xFFEF0022, 0xFFEE0023, 0xFFED0025,
	0xFFED0026, 0xFFEC0028, 0xFFEB002A, 0xFFEA002B,
	0xFFE9002D, 0xFFE8002E, 0xFFE80030, 0xFFE70032,
	0xFFE60033, 0xFFE50035, 0xFFE40036, 0xFFE40038,
	0xFFE3003A, 0xFFE2003B, 0xFFE1003D, 0xFFE0003E,
	0xFFE00040, 0xFFDF0042, 0xFFDE0043, 0xFFDD0045,
	0xFFDC0046, 0xFFDB0048, 0xFFDB0049, 0xFFDA004B,
	0xFFD9004D, 0xFFD8004E, 0xFFD70050, 0xFFD70051,
	0xFFD60053, 0xFFD50055, 0xFFD40056, 0xFFD30058,
	0xFFD30059, 0xFFD2005B, 0xFFD1005D, 0xFFD0005E,
	0xFFCF0060, 0xFFCE0061, 0xFFCE0063, 0xFFCD0065,
	0xFFCC0066, 0xFFCB0068, 0xFFCA0069, 0xFFCA006B,
	0xFFC9006D, 0xFFC8006E, 0xFFC70070, 0xFFC60071,
	0xFFC60073, 0xFFC50075, 0xFFC40076, 0xFFC30078,
	0xFFC20079, 0xFFC1007B, 0xFFC1007D, 0xFFC0007E,
	0xFFBF0080, 0xFFBE0081, 0xFFBD0083, 0xFFBD0085,
	0xFFBC0086, 0xFFBB0088, 0xFFBA0089, 0xFFB9008B,
	0xFFB9008D, 0xFFB8008E, 0xFFB70090, 0xFFB60091,
	0xFFB50093, 0xFFB40095, 0xFFB40096, 0xFFB30098,
	0xFFB20099, 0xFFB1009B, 0xFFB0009D, 0xFFB0009E,
	0xFFAF00A0, 0xFFAE00A1, 0xFFAD00A3, 0xFFAC00A5,
	0xFFAC00A6, 0xFFAB00A8, 0xFFAA00A9, 0xFFA900AB,
	0xFFA800AD, 0xFFA700AE, 0xFFA700B0, 0xFFA600B1,
	0xFFA500B3, 0xFFA400B5, 0xFFA300B6, 0xFFA300B8,
	0xFFA200B9, 0xFFA100BB, 0xFFA000BD, 0xFF9F00BE,
	0xFF9F00C0, 0xFF9E00C1, 0xFF9D00C3, 0xFF9C00C5,
	0xFF9B00C6, 0xFF9A00C8, 0xFF9A00C9, 0xFF9900CB};


long inline ConvFromYUV(long Y, long U, long V)
{
    long C = Y - 16;
    long D = U - 128;
    long E = V - 128;

    long R = ( 298 * C           + 409 * E + 128) >> 8;
    long G = ( 298 * C - 100 * D - 208 * E + 128) >> 8;
    long B = ( 298 * C + 516 * D           + 128) >> 8;

    if (R < 0) R = 0;
    if (G < 0) G = 0;
    if (B < 0) B = 0;
    if (R > 255) R = 255;
    if (G > 255) G = 255;
    if (B > 255) B = 255;

    return(B | (G << 8) | (R << 16));
}

long inline ConvFromYUV_Inv(long Y, long U, long V)
{
    long C = Y - 16;
    long D = U - 128;
    long E = V - 128;

    long R = ( 298 * C           + 409 * E + 128) >> 8;
    long G = ( 298 * C - 100 * D - 208 * E + 128) >> 8;
    long B = ( 298 * C + 516 * D           + 128) >> 8;

    if (R < 0) R = 0;
    if (G < 0) G = 0;
    if (B < 0) B = 0;
    if (R > 255) R = 255;
    if (G > 255) G = 255;
    if (B > 255) B = 255;

    return(R | (G << 8) | (B << 16));
}

/*
long inline ConvFromYUV(long Y, long U, long V)
{
	long I = ((Y - 16) * 298 + 128) >> 8;
	long Ux = TabU[U];
	long Vx = TabV[V];
	short Cb = (signed short)(Ux & 0xffff); // sign extension needed
	short Cr = (signed short)(Vx & 0xffff);
	long Cg = (Ux >> 16) + (Vx >> 16);
	long R = I + Cr;
	long G = I + Cg;
	long B = I + Cb;

	if (R < 0) R = 0;
    if (G < 0) G = 0;
    if (B < 0) B = 0;
    if (R > 255) R = 255;
    if (G > 255) G = 255;
    if (B > 255) B = 255;

    return(B | (G << 8) | (R << 16));
}*/

void inline ConvSetPixel(unsigned char * data, E2DSURFACEINFO * info, int x, int y, int c)
{
    switch (info->pixelformat)
    {
        case EDSP_TRUE16:
        {
            short * ptr = (short *) &(data[info->bufferoffset + x * info->xpitch + y * info->ypitch]);
            *ptr = ((c & 0xf8) >> 3) | ((c & 0xfc00) >> 5) | ((c & 0xf80000) >> 8);
            break;
        }

        case EDSP_TRUE32:
        {
            EINT32 * ptr = (EINT32 *) &(data[info->bufferoffset + x * info->xpitch + y * info->ypitch]);
            *ptr = c | 0xff000000;
            break;
        }
			
		case EDSP_TRUE32_INV:
		{
			unsigned char * ptr = (unsigned char *) &(data[info->bufferoffset + x * info->xpitch + y * info->ypitch]);
			ptr[0] = c >> 16;
			ptr[1] = c >> 8;
			ptr[2] = c;
			ptr[3] = 0xff;
		}
    }
}

void ConvYUVpToRGBA32_11(unsigned char * data_out, E2DSURFACEINFO * out_info, const E_CAMERAFRAME * data_in)
{
	long dx = out_info->xpitch;
	long dy = out_info->ypitch;
	long sx = data_in->width;
	long sy = data_in->height;
	long dro = 2 * dy - sx * dx;
	long dry = sx; // skip a row for Y
	//long drc = 0; // UV are, for now, consecutive, might be useful for interleaved planar formats

	const unsigned char * Y1 = data_in->data;
	const unsigned char * U;
	const unsigned char * V;
	if (data_in->format == EDSP_YUV12)
	{
		U = Y1 + sx * sy;
		V = U + (sx >> 1) * (sy >> 1);
	} else if (data_in->format == EDSP_YVU12)
	{
		V = Y1 + sx * sy;
		U = V + (sx >> 1) * (sy >> 1);
	} 
	// other planar 4:2:0 YUV formats can be added here
	else return;	
	
	unsigned char * out1 = data_out + out_info->bufferoffset;
	unsigned char * out2 = out1 + dy;
	const unsigned char * Y2 = Y1 + sx;

	// paired loop, to match the 1:4 chrominance:luminance storage in 4:2:0 planar formats
	for (int y = 0; y < sy; y += 2)
	{
		// maybe unroll this x4 to get rid of unneeded memory accesses
		for (int x = 0; x < sx; x += 2)
		{
			
			unsigned char vu = *U; U++;
			unsigned char vv = *V; V++;
			
			
			unsigned char vy1 = *Y1; Y1++;
			unsigned char vy2 = *Y1; Y1++;
			unsigned char vy3 = *Y2; Y2++;
			unsigned char vy4 = *Y2; Y2++;
			

			*((EINT32 *)out1) = ConvFromYUV_Inv(vy1, vu, vv) | 0xff000000; out1 += dx;
			*((EINT32 *)out1) = ConvFromYUV_Inv(vy2, vu, vv) | 0xff000000; out1 += dx;
			*((EINT32 *)out2) = ConvFromYUV_Inv(vy3, vu, vv) | 0xff000000; out2 += dx;
			*((EINT32 *)out2) = ConvFromYUV_Inv(vy4, vu, vv) | 0xff000000; out2 += dx;
		}
		out1 += dro;
		out2 += dro;
		Y1 += dry;
		Y2 += dry;
		//U += drc;
		//V += drc;
	}
}

void ClassECamera::FrameToSurface(const E_CAMERAFRAME * data_in, E2DSurface * surface)
{
    if (data_in == NULL) return;
    if (surface == NULL) return;

    //int format_in = data_in->format;
    E2DSURFACEINFO data_out;
    unsigned char * out_ptr = surface->Lock(&data_out);
    
	ClassEdge::Log(data_out.pixelformat);
	
    //ClassEdge::Log("convert");

    long ystep = data_in->height / data_out.height;
    long yadd = data_in->height % data_out.height;
    long ysub = data_out.height;

    //ClassEdge::Log(ystep); ClassEdge::Log(ysub); ClassEdge::Log(yadd);

    long xstep = data_in->width / data_out.width;
    long xadd = data_in->width % data_out.width;
    long xsub = data_out.width;
    //ClassEdge::Log(xstep); ClassEdge::Log(xsub); ClassEdge::Log(xadd);

    long ypos = 0;
    long yerr = 0;

    // bresenham-based scaler (fast, but gives pixellated results)
    for (unsigned int y = 0; y < data_out.height; y++)
    {
        long xpos = 0;
        long xerr = 0;

        for (unsigned int x = 0; x < data_out.width; x++)
        {
            long pixel = ConvGetPixel(data_in, xpos, ypos);
            ConvSetPixel(out_ptr, &data_out, x, y, pixel);

            xpos += xstep;
            xerr += xadd;
            if (xerr > xsub)
            {
                xerr -= xsub;
                xpos++;
            }

        }

        ypos += ystep;
        yerr += yadd;
        if (yerr > ysub)
        {
            yerr -= ysub;
            ypos++;
        }

    }


    surface->Unlock();

}


void ClassECamera::FrameToSurfaceAspect(const E_CAMERAFRAME * data_in, E2DSurface * surface)
{
    if (data_in == NULL) return;
    if (surface == NULL) return;

    //int format_in = data_in->format;
    E2DSURFACEINFO data_out;
    unsigned char * out_ptr = surface->Lock(&data_out);

    if (data_in->orientation & ECO_YFLIP)
    {
        data_out.bufferoffset += (data_out.height-1) * data_out.ypitch;
        data_out.ypitch = 0 - data_out.ypitch;
    }
    if (data_in->orientation & ECO_XFLIP)
    {
        data_out.bufferoffset += (data_out.width-1) * data_out.xpitch;
        data_out.xpitch = 0 - data_out.xpitch;
    }
    if (data_in->orientation & ECO_SWAPAXIS)
    {
        long temp = data_out.height;
        data_out.height = data_out.width;
        data_out.width = temp;
        
        temp = data_out.xpitch;
        data_out.xpitch = data_out.ypitch;
        data_out.ypitch = temp;
    }

	if ((long)data_out.width == data_in->width && (long)data_out.height == data_in->height)
	{
		// might want to special case this
		if (data_out.pixelformat == EDSP_TRUE32_INV)
		{
			if (data_in->format == EDSP_YVU12 || data_in->format == EDSP_YUV12)
			{
				ConvYUVpToRGBA32_11(out_ptr, &data_out, data_in);
				surface->Unlock();
				return;
			}
			//else ClassEdge::Log("coderfail 1");
		}		
		//else ClassEdge::Log("coderfail 2");
	}

    //ClassEdge::Log("convert");

    long ystep = data_in->height / data_out.height;
    long yadd = data_in->height % data_out.height;
    long ysub = data_out.height;

    //ClassEdge::Log(ystep); ClassEdge::Log(ysub); ClassEdge::Log(yadd);

    long xstep = data_in->width / data_out.width;
    long xadd = data_in->width % data_out.width;
    long xsub = data_out.width;
    //ClassEdge::Log(xstep); ClassEdge::Log(xsub); ClassEdge::Log(xadd);

    unsigned int xmin = 0, xmax = xsub;
    unsigned int ymin = 0, ymax = ysub;

    if (xstep != ystep || (xadd * ysub != yadd * xsub))
    {
       // correct aspect ratio
       
       if ((xstep > ystep) || ((xstep == ystep) && (xadd * ysub > yadd * xsub)))
       {
          unsigned int newsize = data_in->height * data_out.width / data_in->width;
          ystep = data_in->height / newsize;
          yadd = data_in->height % newsize;
          ysub = newsize;
          ymin = (data_out.height - newsize) / 2;
          ymax = ymin + newsize;
       }
       else
       {
          unsigned int newsize = data_in->width * data_out.height / data_in->height;
          xstep = data_in->width / newsize;
          xadd = data_in->width % newsize;
          xsub = newsize;
          xmin = (data_out.width - newsize) / 2;
          xmax = xmin + newsize;
       }

    }

    long ypos = 0;
    long yerr = 0;

    // bresenham-based scaler (fast, but gives pixellated results)
    for (unsigned int y = ymin; y < ymax; y++)
    {
        long xpos = 0;
        long xerr = 0;

        for (unsigned int x = xmin; x < xmax; x++)
        {
            long pixel = ConvGetPixel(data_in, xpos, ypos);
            ConvSetPixel(out_ptr, &data_out, x, y, pixel);

            xpos += xstep;
            xerr += xadd;
            if (xerr > xsub)
            {
                xerr -= xsub;
                xpos++;
            }

        }

        ypos += ystep;
        yerr += yadd;
        if (yerr > ysub)
        {
            yerr -= ysub;
            ypos++;
        }

    }


    surface->Unlock();

}


