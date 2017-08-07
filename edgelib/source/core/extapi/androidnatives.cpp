// androidnatives.cpp
// Android natives
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#if defined(ANDROID)
#include <jni.h>
#if 0
    #include <android/log.h>
    #define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,   "EAN", __VA_ARGS__)
#else
    #define LOGD(...) 
#endif
#include "../edgecore.h"
#include "../instance.h"

#include "androidapp.h"
#include "androidview.h"
#include "android3dview.h"
#include "androidfile.h"
#include "androidutil.h"
#endif
