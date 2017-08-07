// androidbuffer.h
// Android buffer
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#ifndef ANDROIDBUFFER_H
#define ANDROIDBUFFER_H

#include <jni.h>

class AndroidBuffer
{
    public:
	AndroidBuffer()
	{
	}
	
	AndroidBuffer(jintArray a)
	{
	    array = a;
	}
	
	jintArray array;
	jint * body;
	int width;
	int height;
	int orientation;
	
	void * lock(JNIEnv * env)
	{
	    body = env->GetIntArrayElements(array, 0);   
	    return (void*) body;
	}
	
	void unlock(JNIEnv * env)
	{
	    env->ReleaseIntArrayElements(array, body, 0);   
	}
};

extern AndroidBuffer * defaultbuffer;

#endif
