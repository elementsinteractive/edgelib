// exfonthdr.h
// Core functionality for font rendering, prototypes
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Font draw callback function
typedef ERESULT (*FUNC_DRAWFONTCHARACTER)(long x, long y, const short *srcrc, void *userdata);

//Function prototypes
unsigned long FontDraw_GetFontWidth(const unsigned char *text, unsigned long wrapwidth, const WCHAR *fcset, const short *fontrc, const short *fontkerning, long defaultchar, bool useunicode, bool stopnewline = false, long maxcount = -1);
unsigned long FontDraw_GetFontHeight(const unsigned char *text, unsigned long wrapwidth, const WCHAR *fcset, const short *fontrc, const short *fontkerning, long defaultchar, bool useunicode);
void FontDraw_PrepareFontWrap(unsigned char *dst, const unsigned char *src, unsigned long wrapwidth, const WCHAR *fcset, const short *fontrc, const short *fontkerning, long defaultchar, bool useunicode);
void FontDraw_Draw(FUNC_DRAWFONTCHARACTER drawfunc, long x, long y, const unsigned char *text, unsigned long wrapwidth, unsigned long alignment, const WCHAR *fcset, const short *fontrc, const short *fontkerning, long defaultchar, bool useunicode, void *userdata);
