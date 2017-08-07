// exmacro.h
// Macro's used for optimized drawing routines
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#define EBCODE_MACRO_HPACITY_O(d, s, o1, o2)                (d - ((d >> o1) & o2) + s)
#define EBCODE_MACRO_HPACITY_O2(d, s, o1, o2)               (s + ((d >> o1) & o2))
#define EBCODE_MACRO_HPACITY(d, s, m1, m2, o, dv)           (s + (((((d & m1) * dv) >> o) & m1) | ((((d & m2) * dv) >> o) & m2)))
#define EBCODE_MACRO_HPACITY_DBL(d, s, m1, m2, o, dv)       (s + (((((d & m1) >> o) * dv) & m1) | ((((d & m2) * dv) >> o) & m2)))
#define EBCODE_MACRO_OPACITY_O(d, s, o1, o2)                ((s - ((s >> o1) & o2)) + ((d >> o1) & o2))
#define EBCODE_MACRO_OPACITY_O2(d, s, o1, o2)               ((d - ((d >> o1) & o2)) + ((s >> o1) & o2))
#define EBCODE_MACRO_OPACITY(d, s, m1, m2, o, dv, sv)       (((((s & m1) * sv + (d & m1) * dv) >> o) & m1) | ((((s & m2) * sv + (d & m2) * dv) >> o) & m2))
#define EBCODE_MACRO_OPACITY_DBL(d, s, m1, m2, o, dv, sv)   (((((s & m1) >> o) * sv + ((d & m1) >> o) * dv) & m1) | ((((s & m2) * sv + (d & m2) * dv) >> o) & m2))
#define EBCODE_MACRO_ADDALPHA(d, s, m1, m2, m3, o1, o2, o3) ((((d & m1) + (s & m1)) & o1 ? m1 : ((d & m1) + (s & m1))) | (((d & m2) + (s & m2)) & o2 ? m2 : ((d & m2) + (s & m2))) | (((d & m3) + (s & m3)) & o3 ? m3 : ((d & m3) + (s & m3))))
#define EBCODE_MACRO_SUBALPHA(d, s, m1, m2, m3)             (((unsigned)(s & m1) > (unsigned)(d & m1) ? 0 : (d & m1) - (s & m1)) | ((unsigned)(s & m2) > (unsigned)(d & m2) ? 0 : (d & m2) - (s & m2)) | ((unsigned)(s & m3) > (unsigned)(d & m3) ? 0 : (d & m3) - (s & m3)))
#define EBCODE_MACRO_TINT(s, m, c)                          (((s >> 1) & m) + c)
#define EBCODE_MACRO_COLORIZE2(c, m1, m2, m3, v, o)         (((((c & m1) * v) >> o) & m1) | ((((c & m2) * v) >> o) & m2) | ((((c & m3) * v) >> o) & m3))
#define EBCODE_MACRO_COLORIZE(d, s, m1, m2, m3, o1, o2, o3, o4, c, typec) { unsigned long tmpb = 0; if ((unsigned)((s & m1) >> o1) > tmpb) tmpb = (s & m1) >> o1; if ((unsigned)((s & m2) >> o2) > tmpb) tmpb = (s & m2) >> o2; if ((unsigned)((s & m3) >> o3) > tmpb) tmpb = (s & m3) >> o3; d = (typec)EBCODE_MACRO_COLORIZE2(c, m1, m2, m3, tmpb, o4); }

//#define EBCODE_MACRO_BILINEAR(m, c1, c2, c3, c4, u, v)  ((((c1) & m) * (256 - u) + ((c2) & m) * u) * (256 - v) + (((c3) & m) * (256 - u) + ((c4) & m) * u) * v)
//Reserved

//#define EBCODE_MACRO_OPACITY(d, s, m1, m2, o, dv, sv)       (((((((s & m1) - (d & m1)) * sv) >> o) + (d & m1)) & m1) | ((((((s & m2) - (d & m2)) * sv) >> o) + (d & m2)) & m2))
//#define EBCODE_MACRO_OPACITY_DBL(d, s, m1, m2, o, dv, sv)   (((((((s & m1) >> o) - ((d & m1) >> o)) * sv) + (d & m1)) & m1) | ((((((s & m2) - (d & m2)) * sv) >> o) + (d & m2)) & m2))
//Opacity macro's (v3) using 1 multiply, but slower?

//#define EBCODE_MACRO_OPACITY(d, s, m1, m2, o, dv, sv)       (((((s & m1) * sv + (d & m1) * dv) >> o) & m1) | ((((s & m2) * sv + (d & m2) * dv) >> o) & m2))
//#define EBCODE_MACRO_OPACITY_DBL(d, s, m1, m2, o, dv, sv)   (((((s & m1) >> o) * sv + ((d & m1) >> o) * dv) & m1) | ((((s & m2) * sv + (d & m2) * dv) >> o) & m2))
//Opacity macro's (v2)

//#define EBCODE_MACRO_OPACITY(d, s, m1, m2, o, dv, sv)       ((((((s & m1) * sv) >> o) & m1) | ((((s & m2) * sv) >> o) & m2)) + (((((d & m1) * dv) >> o) & m1) | ((((d & m2) * dv) >> o) & m2)))
//#define EBCODE_MACRO_OPACITY_DBL(d, s, m1, m2, o, dv, sv)   ((((((s & m1) >> o) * sv) & m1) | ((((s & m2) * sv) >> o) & m2)) + (((((d & m1) >> o) * dv) & m1) | ((((d & m2) * dv) >> o) & m2)))
//Old opacity macro's (v1)
