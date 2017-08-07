// imp_gx.h
// Import header: provides backwards compatibility for GAPI display routines (Windows Mobile 6+)
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_WINCE) && !defined(DEVICE_WINMOBILE)

#ifdef GXDLL_EXPORTS
#define GXDLL_API __declspec(dllexport)
#else
#define GXDLL_API __declspec(dllimport)
#endif

struct GXKeyList {
    short vkUp;             // key for up
    POINT ptUp;             // x,y position of key/button.  Not on screen but in screen coordinates.
    short vkDown;
    POINT ptDown;
    short vkLeft;
    POINT ptLeft;
    short vkRight;
    POINT ptRight;
    short vkA;
    POINT ptA;
    short vkB;
    POINT ptB;
    short vkC;
    POINT ptC;
    short vkStart;
    POINT ptStart;
};

GXDLL_API int GXOpenInput();
GXDLL_API int GXCloseInput();
GXDLL_API GXKeyList GXGetDefaultKeys(int iOptions);
GXDLL_API int GXSuspend();
GXDLL_API int GXResume();

// Orientation key layout constants
#define GX_NORMALKEYS   0x02
#define GX_LANDSCAPEKEYS    0x03

#endif

struct GXDisplayProperties {
    DWORD cxWidth;
    DWORD cyHeight;         // notice lack of 'th' in the word height.
    long cbxPitch;          // number of bytes to move right one x pixel - can be negative.
    long cbyPitch;          // number of bytes to move down one y pixel - can be negative.
    long cBPP;              // # of bits in each pixel
    DWORD ffFormat;         // format flags.
};

GXDLL_API int GXOpenDisplay(HWND hWnd, DWORD dwFlags);
GXDLL_API int GXCloseDisplay();
GXDLL_API void * GXBeginDraw();
GXDLL_API int GXEndDraw();
GXDLL_API GXDisplayProperties GXGetDisplayProperties();
GXDLL_API int GXSetViewport( DWORD dwTop, DWORD dwHeight, DWORD dwReserved1, DWORD dwReserved2 );
GXDLL_API BOOL GXIsDisplayDRAMBuffer();

#define GX_FULLSCREEN   0x01        // for OpenDisplay()

#ifndef kfLandscape
    #define kfLandscape 0x8         // Screen is rotated 270 degrees
    #define kfPalette   0x10        // Pixel values are indexes into a palette
    #define kfDirect    0x20        // Pixel values contain actual level information
    #define kfDirect555 0x40        // 5 bits each for red, green and blue values in a pixel.
    #define kfDirect565 0x80        // 5 red bits, 6 green bits and 5 blue bits per pixel
    #define kfDirect888 0x100       // 8 bits each for red, green and blue values in a pixel.
    #define kfDirect444 0x200       // 4 red, 4 green, 4 blue
    #define kfDirectInverted 0x400
#endif

#ifndef GETRAWFRAMEBUFFER
    #define GETRAWFRAMEBUFFER   0x00020001
    typedef struct _RawFrameBufferInfo
    {
	    WORD wFormat;
	    WORD wBPP;
	    VOID *pFramePointer;
	    int	cxStride;
	    int	cyStride;
        int cxPixels;
        int cyPixels;
    } RawFrameBufferInfo;

    #define FORMAT_565 1
    #define FORMAT_555 2
    #define FORMAT_OTHER 3
#endif
