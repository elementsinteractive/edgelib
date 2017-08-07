// android3dview.h
// Android 3D view
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//#include "nl/elements/edgelib/Edgelib3DView.h"

extern EC_INSTANCEOBJ * android_global_instance;
void debug(const char * msg);

void Call_EdgelibApp_firstframecheck(JNIEnv * env, jobject jthis);
int Call_Edgelib3DView_getOrientation();

extern "C" void Java_nl_elements_edgelib_Edgelib3DView_trackEvent
  (JNIEnv * env, jobject, jint a, jint b, jint c)
{
    Java_nl_elements_edgelib_EdgelibView_trackEvent(env, NULL, a, b, c);
}

/*
 * Class:     nl_elements_edgelib_Edgelib3DView
 * Method:    onGLInit
 * Signature: (Ljavax/microedition/khronos/opengles/GL10;Ljavax/microedition/khronos/egl/EGLConfig;)V
 */
extern "C" void Java_nl_elements_edgelib_Edgelib3DView_onGLInit(JNIEnv *, jobject, jobject, jobject)
{
    
    
}

/*
 * Class:     nl_elements_edgelib_Edgelib3DView
 * Method:    onGLRestore
 * Signature: (Ljavax/microedition/khronos/opengles/GL10;Ljavax/microedition/khronos/egl/EGLConfig;)V
 */
extern "C" void Java_nl_elements_edgelib_Edgelib3DView_onGLRestore
  (JNIEnv * env, jobject jthis, jobject, jobject)
{
    android_global_instance->jviewobject = jthis;
    android_global_instance->jenv = env;

    ecEvent event;
    event.type = ECEV_CONTEXTLOST;
    event.param1 = 0;
    ecOnEvent(android_global_instance, &event);
}

/*
 * Class:     nl_elements_edgelib_Edgelib3DView
 * Method:    onGLDisplaySize
 * Signature: (Ljavax/microedition/khronos/opengles/GL10;II)V
 */
extern "C" void Java_nl_elements_edgelib_Edgelib3DView_onGLDisplaySize
    (JNIEnv * env, jobject jthis, jobject jgl, jint width, jint height, jint orientation, jint first)
{
    android_global_instance->wnd = (ECOREOBJ)jthis;
    android_global_instance->jviewobject = jthis;
    android_global_instance->jenv = env;
    
    defaultbuffer = new AndroidBuffer(NULL);
    defaultbuffer->width = width;
    defaultbuffer->height = height;
    defaultbuffer->orientation = orientation;
    
    ecEvent event;
    if (first)
    {
        Call_EdgelibApp_firstframecheck(env, android_global_instance->jappobject);
        defaultbuffer->orientation = Call_Edgelib3DView_getOrientation();
        
        event.type = ECEV_CREATEWINDOW;
        event.param1 = 0;
        //debug("create event");
        ecOnEvent(android_global_instance, &event);
    }
    
    event.type = ECEV_CHANGEORIENTATION;
    event.param1 = 90 * orientation;
    event.param2 = 0;
    ecOnEvent(android_global_instance, &event);
}

/*
 * Class:     nl_elements_edgelib_Edgelib3DView
 * Method:    onGLFrame
 * Signature: (Ljavax/microedition/khronos/opengles/GL10;)V
 */
extern "C" void Java_nl_elements_edgelib_Edgelib3DView_onGLFrame
  (JNIEnv * env, jobject jthis, jobject jgl)
{
    Java_nl_elements_edgelib_EdgelibApp_idleFrame(env, NULL);
}



bool Call_Edgelib3DView_checkInstallation(void * wnd)
{
	jobject jthis = (jobject) wnd;
    	JNIEnv * env = android_global_instance->jenv;
	if (!jthis) return false;		
    	jclass cls = env->FindClass("nl/elements/edgelib/Edgelib3DView");
	if (!cls) return false;
        jmethodID mid = env->GetMethodID(cls, "checkInstallation", "()Z");
	if (!mid) return false;
        return env->CallBooleanMethod(jthis, mid); 
}

int Call_Edgelib3DView_getWidth()
{
    return defaultbuffer->width;
}

int Call_Edgelib3DView_getHeight()
{
    return defaultbuffer->height;
}

int Call_Edgelib3DView_getOrientation()
{
    JNIEnv * env = android_global_instance->jenv;
    jobject jthis = android_global_instance->jviewobject;
    jclass cls = env->FindClass("nl/elements/edgelib/Edgelib3DView");
    jmethodID mid = env->GetMethodID(cls, "getOrientation", "()I");
    return env->CallIntMethod(jthis, mid);
}
