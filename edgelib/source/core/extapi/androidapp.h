// androidapp.h
// Android app
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include <string.h>
#include <jni.h>
#include <unistd.h>
#include <stdio.h>

//#include "nl/elements/edgelib/EdgelibApp.h"

void debug(const char * msg);
void Call_EdgelibApp_setDebug(JNIEnv * env, jobject jthis, const char * message);
void Call_EdgelibApp_quitloop(JNIEnv * env, jobject jthis);
jobject Call_EdgelibApp_getUtility(JNIEnv * env, jobject jthis);
bool EdgeGL_Use(void);

extern EC_INSTANCEOBJ * android_global_instance;
EC_INSTANCEOBJ * android_global_instance = NULL;

static char * copyjstring(JNIEnv * env, jstring string)
{
    //LOGD("copyjstring (0x%lx, 0x%lx)", (long)env, (long)string);
    const char *nativeString = env->GetStringUTFChars(string, 0);
    char * copy = (char *) malloc(strlen(nativeString) + 1);
    //LOGD("copyjstring 1 (0x%lx, 0x%lx) %i %i", (long)nativeString, (long)copy, (int)strlen(nativeString), (int)strlen(""));
    
    if (copy)
    {
        strcpy(copy, nativeString);
    }
    env->ReleaseStringUTFChars(string, nativeString);
    return copy;
}


//
// Java-to-C calls
//

extern "C" void Java_nl_elements_edgelib_EdgelibApp_edgeStart (JNIEnv * env, jobject jthis, jstring name, jstring model, jint apiversion)
{
    LOGD("edgeStart(0x%lx, 0x%lx, 0x%lx, 0x%lx, %i)", (long)env, (long)jthis, (long)name, (long)model, apiversion);
    ECOREOBJ inst;
    ecErr errcreate;
    inst = ecInstCreate(errcreate, 0);
    LOGD("edgeStart 1");
    if (inst)
    {
        EC_INSTANCEOBJ *io = (EC_INSTANCEOBJ *)inst;
        
        android_global_instance = io;
        io->jenv = env;
        io->jappobject = jthis;
        io->sustainloop = true;
        io->acceptevents = true;
        io->jassetmanager = NULL;
        LOGD("edgeStart 2 (0x%lx, 0x%lx)", (long)env, (long)io->jenv);
        io->jutilobject = Call_EdgelibApp_getUtility(env, jthis);
        LOGD("edgeStart 3 (0x%lx, 0x%lx)", (long)env, (long)io->jenv);
        io->devname = copyjstring(env, name);
        LOGD("edgeStart 4 (0x%lx, 0x%lx)", (long)env, (long)io->jenv);
        io->devmodel = copyjstring(env, model);
        LOGD("edgeStart 5 (0x%lx, 0x%lx)", (long)env, (long)io->jenv);
        io->api = apiversion;
        
        ecEvent event;
        event.type = ECEV_CREATEINSTANCE;
        event.param1 = 0;

        LOGD("edgeStart 6 (0x%lx, 0x%lx)", (long)env, (long)io->jenv);
        ecOnEvent(inst, &event);
        LOGD("edgeStart 7 (0x%lx, 0x%lx)", (long)env, (long)io->jenv);
        
    }
    else
    {
        LOGD("edgeStart 8 (0x%lx)", (long)env);
        Call_EdgelibApp_quitloop(env, jthis);
        LOGD("edgeStart 9 (0x%lx)", (long)env);
    }
    LOGD("edgeStart 10 (0x%lx)", (long)env);
}

extern "C" void Java_nl_elements_edgelib_EdgelibApp_idleFrame(JNIEnv * env, jobject jthis)
{
    //LOGD("idleFrame(0x%lx, 0x%lx)", (long)env, (long)jthis);
    android_global_instance->jenv = env;
    ecEvent event;
    event.type = ECEV_IDLE;

    //LOGD("idleFrame 1 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    if (android_global_instance->acceptevents)
    {
        //debug("idle event");
        //LOGD("idleFrame 2 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
        ecOnEvent(android_global_instance, &event);
        //LOGD("idleFrame 3 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
        if (android_global_instance->loopsleep)
        {
            //LOGD("idleFrame 4 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
            usleep(android_global_instance->loopsleep * 1000);
            android_global_instance->loopsleep = 0;
        }
        //LOGD("idleFrame 5 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    }
}

extern "C" jboolean Java_nl_elements_edgelib_EdgelibApp_getSustainLoop(JNIEnv * env, jobject jthis)
{
    //LOGD("getSustainLoop(0x%lx, 0x%lx)", (long)env, (long)jthis);
    return android_global_instance->sustainloop;
}

extern "C"  void Java_nl_elements_edgelib_EdgelibApp_keyEvent (JNIEnv * env, jobject jthis, jint keyscan, jint keysym, jint down)
{
    LOGD("keyEvent(0x%lx, 0x%lx, %i/%i, %i)", (long)env, (long)jthis, keyscan, keysym, down);
    if (android_global_instance->acceptevents)
    {
        LOGD("keyevent 1 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
        android_global_instance->jenv = env;
        ecEvent event;
        event.param1 = keysym;
        event.param2 = keyscan;
        if (down == 1)
            event.type = ECEV_KEYDOWN;
        else if (down == 0)
            return; // autorepeat
        else if (down == -1)
            event.type = ECEV_KEYUP;
        else
            return; // error
                
        //debug("key event");
        LOGD("keyevent 2 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
        ecOnEvent(android_global_instance, &event);
        LOGD("keyevent 3 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    }
}

extern "C"  void Java_nl_elements_edgelib_EdgelibApp_touchEvent (JNIEnv * env, jobject jthis, jint pointer, jint x, jint y, jint down)
{
    LOGD("touchEvent(0x%lx, 0x%lx, %i, [%i, %i], %i)", (long)env, (long)jthis, pointer, x, y, down);
    if (android_global_instance->acceptevents)
    {
        LOGD("touchEvent 2 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
        android_global_instance->jenv = env;
        ecEvent event;
        event.param1 = pointer;
        event.param2 = x;
        event.param3 = y;
        if (down == 0)
            event.type = ECEV_CLICKMOVE;
        else if (down == 1)
            event.type = ECEV_CLICKUP;
        else
            event.type = ECEV_CLICKDOWN;
        
        //debug("mouse event");
        LOGD("touchEvent 3 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
        ecOnEvent(android_global_instance, &event);
        LOGD("touchEvent 4 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    }
}

extern "C" jboolean Java_nl_elements_edgelib_EdgelibApp_getOpenGL (JNIEnv * env, jobject jthis)
{
    //LOGD("getOpenGL(0x%lx, 0x%lx)", (long)env, (long)jthis);
    return EdgeGL_Use();
}

/*onDrawEvent
* Class:     nl_elements_edgelib_EdgelibApp
* Method:    focusChanged
* Signature: (I)V
*/
extern "C" void Java_nl_elements_edgelib_EdgelibApp_focusChanged(JNIEnv * env, jobject jthis, jint focus)
{
    LOGD("focusChanged(0x%lx, 0x%lx, %i)", (long)env, (long)jthis, focus);
    if (android_global_instance->acceptevents)
    {
        LOGD("focusChanged 1 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
        android_global_instance->jenv = env;
        ecEvent event;
        event.type = ECEV_CHANGEFOCUS;
        event.param1 = focus;

        LOGD("focusChanged 2 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
        ecOnEvent(android_global_instance, &event);
        LOGD("focusChanged 3 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    }
}

//
// C-to-java calls
//
int Call_EdgelibApp_edgeWindowCreate(JNIEnv * env, jobject jthis, int options)
{
    LOGD("edgeWindowCreate(0x%lx, 0x%lx, %i)", (long)env, (long)jthis, options);    
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibApp");
    //if (cls == 0) return;
    jmethodID mid = env->GetMethodID(cls, "edgeWindowCreate", "(I)I");
    //if (mid == 0) return;
    return env->CallIntMethod(jthis, mid, options);
    
}

void Call_EdgelibApp_setDebug(JNIEnv * env, jobject jthis, const char * message)
{
    LOGD("setDebug(0x%lx, 0x%lx, %s)", (long)env, (long)jthis, message);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibApp");
    jstring js = env->NewStringUTF(message);
    jmethodID mid = env->GetMethodID(cls, "edgeDebug", "(Ljava/lang/String;)V");
    env->CallVoidMethod(jthis, mid, js);
}

void Call_EdgelibApp_setBacklight(JNIEnv * env, jobject jthis, int status)
{
    LOGD("setBacklight(0x%lx, 0x%lx, %i)", (long)env, (long)jthis, status);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibApp");
    jmethodID mid = env->GetMethodID(cls, "setBacklight", "(I)V");
    env->CallVoidMethod(jthis, mid, status);
}

void Call_EdgelibApp_suspendloop(JNIEnv * env, jobject jthis, bool suspend)
{
    LOGD("suspendloop(0x%lx, 0x%lx, %s)", (long)env, (long)jthis, suspend ? "true" : "false");
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibApp");
    jmethodID mid = env->GetMethodID(cls, "suspendloop", "(Z)V");
    if (mid) env->CallVoidMethod(jthis, mid, (jboolean)suspend);
}

void Call_EdgelibApp_firstframecheck(JNIEnv * env, jobject jthis)
{
    LOGD("firstframecheck(0x%lx, 0x%lx)", (long)env, (long)jthis);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibApp");
    jmethodID mid = env->GetMethodID(cls, "firstframecheck", "()V");
    env->CallVoidMethod(jthis, mid);
}

void Call_EdgelibApp_quitloop(JNIEnv * env, jobject jthis)
{
    LOGD("quitloop(0x%lx, 0x%lx)", (long)env, (long)jthis);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibApp");
    jmethodID mid = env->GetMethodID(cls, "quitloop", "()V");
    env->CallVoidMethod(jthis, mid);
}

jobject Call_EdgelibApp_getAssetManager(JNIEnv * env, jobject jthis)
{
    LOGD("getAssetManager(0x%lx, 0x%lx)", (long)env, (long)jthis);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibApp");
    jmethodID mid = env->GetMethodID(cls, "getAssetManager", "()Landroid/content/res/AssetManager;");
    return env->CallObjectMethod(jthis, mid);
}

jobject Call_EdgelibApp_getUtility(JNIEnv * env, jobject jthis)
{
    LOGD("getUtility(0x%lx, 0x%lx)", (long)env, (long)jthis);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibApp");
    jmethodID mid = env->GetMethodID(cls, "getUtility", "()Lnl/elements/edgelib/EdgelibUtil;");
    return env->CallObjectMethod(jthis, mid);
}

char * Call_EdgelibApp_getStoragePath()
{
    //LOGD("getStoragePath()");
    JNIEnv * env = android_global_instance->jenv;
    jobject jthis = android_global_instance->jappobject;
    //LOGD("getStoragePath 1 (0x%lx, 0x%lx)", (long)env, (long)jthis);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibApp");
    jmethodID mid = env->GetMethodID(cls, "getStoragePath", "()Ljava/lang/String;");
    jobject s = env->CallObjectMethod(jthis, mid);
    char * str = copyjstring(env, (jstring) s);
    //LOGD("getStoragePath 2 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    env->DeleteLocalRef(cls);
    env->DeleteLocalRef(s);
    //LOGD("getStoragePath 3 (0x%lx, 0x%lx, %s, 0x%lx)", (long)env, (long)jthis, str, (long)s);
    return str;
}

//
// Helper functions
//

char * AndroidGetDeviceModel()
{
    return android_global_instance->devmodel;
}

char * AndroidGetDeviceName()
{
    return android_global_instance->devname;
}

int AndroidGetDeviceOS()
{
    return android_global_instance->api;
}
