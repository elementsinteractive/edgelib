// androidview.h
// Android view
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include <string.h>
#include <jni.h>

#include "androidbuffer.h"
//#include "nl/elements/edgelib/EdgelibView.h"

extern EC_INSTANCEOBJ * android_global_instance;
void debug(const char * msg);

AndroidBuffer * defaultbuffer = NULL;
void Call_EdgelibApp_firstframecheck(JNIEnv * env, jobject jthis);
int Call_EdgelibView_getOrientation();

extern "C" void Java_nl_elements_edgelib_EdgelibView_onNewBuffer(JNIEnv * env, jobject jthis, jintArray array, jint x, jint y, jint orientation)
{
    LOGD("onNewBuffer (0x%lx, 0x%lx, 0x%lx, [%ix%i], %i)", (long)env, (long)jthis, (long)array, x, y, orientation);
    android_global_instance->wnd = (ECOREOBJ)jthis;
    android_global_instance->jviewobject = jthis;
    android_global_instance->jenv = env;

    LOGD("onNewBuffer 2 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    defaultbuffer = new AndroidBuffer(array);
    defaultbuffer->width = x;
    defaultbuffer->height = y;

    LOGD("onNewBuffer 3 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    Call_EdgelibApp_firstframecheck(env, android_global_instance->jappobject);
    defaultbuffer->orientation = Call_EdgelibView_getOrientation();
    LOGD("onNewBuffer 4 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    
    ecEvent event;
    event.type = ECEV_CREATEWINDOW;
    event.param1 = 0;
    //debug("create event");
    LOGD("onNewBuffer 5 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    ecOnEvent(android_global_instance, &event);
    LOGD("onNewBuffer 6 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    
    event.type = ECEV_CHANGEORIENTATION;
    event.param1 = 90 * orientation;
    event.param2 = 0;
    LOGD("onNewBuffer 7 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    ecOnEvent(android_global_instance, &event);
    LOGD("onNewBuffer 8 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
}

extern "C" void Java_nl_elements_edgelib_EdgelibView_onChangeBuffer(JNIEnv * env, jobject jthis, jintArray array, jint x, jint y, jint orientation)
{
    //debug("change event");
    LOGD("onChangeBuffer (0x%lx, 0x%lx, 0x%lx, [%ix%i], %i)", (long)env, (long)jthis, (long)array, x, y, orientation);
    
    if (defaultbuffer) delete defaultbuffer;

    LOGD("onChangeBuffer 1 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);

    android_global_instance->jviewobject = jthis;
    android_global_instance->jenv = env;
    defaultbuffer = new AndroidBuffer(array);
    defaultbuffer->width = x;
    defaultbuffer->height = y;
    defaultbuffer->orientation = orientation;

    LOGD("onChangeBuffer 2 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    
    ecEvent event;
    event.type = ECEV_CHANGEORIENTATION;
    event.param1 = 90 * orientation;
    event.param2 = 0;
    LOGD("onChangeBuffer 3 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    ecOnEvent(android_global_instance, &event);
    LOGD("onChangeBuffer 4 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
}

extern "C" void Java_nl_elements_edgelib_EdgelibView_onDrawEvent(JNIEnv * env, jobject jthis, jintArray array, jint width, jint height)
{
    LOGD("onDrawEvent 1 (0x%lx, 0x%lx, 0x%lx, %i, %i)", (long)env, (long)jthis, (long)array, width, height);
    jint *body = env->GetIntArrayElements(array, 0);
    
    jint * ptr = body;
    /*for (jint y = 0; y < height; y++)
    {
	for (jint x = 0; x < width; x++)
	{
	    *ptr++ = (2 * x / 3) + (y / 2) * 0x10100;
	}
    }*/
    env->ReleaseIntArrayElements(array, body, 0);
    LOGD("onDrawEvent 1");

}

extern "C" void Java_nl_elements_edgelib_EdgelibView_trackEvent(JNIEnv * env, jobject jthis, jint dx, jint dy, jint direction)
{
    LOGD("trackEvent (0x%lx, 0x%lx, (%i, %i), %i)", (long)env, (long)jthis, dx, dy, direction);
    if (android_global_instance->acceptevents)
    {
        LOGD("trackEvent 1");
		// translate to button presses
		android_global_instance->jenv = env;
		ecEvent event;	
		if (dy > 0)
		{
			event.param1 = 20;
			event.type = ECEV_KEYDOWN;
			ecOnEvent(android_global_instance, &event);
			event.type = ECEV_KEYUP;
			ecOnEvent(android_global_instance, &event);
		}
		if (dy < 0)
		{
			event.param1 = 19;
			event.type = ECEV_KEYDOWN;
			ecOnEvent(android_global_instance, &event);
			event.type = ECEV_KEYUP;
			ecOnEvent(android_global_instance, &event);
		}
		if (dx > 0)
		{
			event.param1 = 22;
			event.type = ECEV_KEYDOWN;
			ecOnEvent(android_global_instance, &event);
			event.type = ECEV_KEYUP;
			ecOnEvent(android_global_instance, &event);
		}
		if (dx < 0)
		{
			event.param1 = 21;
			event.type = ECEV_KEYDOWN;
			ecOnEvent(android_global_instance, &event);
			event.type = ECEV_KEYUP;
			ecOnEvent(android_global_instance, &event);
		}
		
		if (direction == 1)
		{
			event.param1 = 23;
			event.type = ECEV_KEYDOWN;
			ecOnEvent(android_global_instance, &event);	    
		}
		if (direction == -1)
		{
			event.param1 = 23;
			event.type = ECEV_KEYUP;
			ecOnEvent(android_global_instance, &event);	    
		}
		LOGD("trackEvent 2");
    }
    LOGD("trackEvent 3");
}

int Call_EdgelibView_getHeight()
{
    LOGD("getHeight");
	JNIEnv * env = android_global_instance->jenv;
	jobject jthis = android_global_instance->jviewobject;
    LOGD("getHeight (0x%lx, 0x%lx)", (long)env, (long)jthis);
	jclass cls = env->FindClass("nl/elements/edgelib/EdgelibView");
    jmethodID mid = env->GetMethodID(cls, "getHeight", "()I");
    return env->CallIntMethod(jthis, mid); 
}

int Call_EdgelibView_getWidth()
{
    LOGD("getWidth");
	JNIEnv * env = android_global_instance->jenv;
	jobject jthis = android_global_instance->jviewobject;
    LOGD("getWidth (0x%lx, 0x%lx)", (long)env, (long)jthis);
	jclass cls = env->FindClass("nl/elements/edgelib/EdgelibView");
    jmethodID mid = env->GetMethodID(cls, "getWidth", "()I");
    return env->CallIntMethod(jthis, mid); 
}

int Call_EdgelibView_getOrientation()
{
    LOGD("getOrientation");
    JNIEnv * env = android_global_instance->jenv;
    jobject jthis = android_global_instance->jviewobject;
    LOGD("getOrientation (0x%lx, 0x%lx)", (long)env, (long)jthis);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibView");
    jmethodID mid = env->GetMethodID(cls, "getOrientation", "()I");
    return env->CallIntMethod(jthis, mid);
}