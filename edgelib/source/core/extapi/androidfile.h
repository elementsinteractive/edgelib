// androidfile.h
// Android file
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//#include "nl/elements/edgelib/EdgelibFile.h"
#include <cstring>
extern EC_INSTANCEOBJ * android_global_instance;

jobject Call_EdgelibApp_getAssetManager(JNIEnv * env, jobject jthis);

// Java-to-c
extern "C" void Java_nl_elements_edgelib_EdgelibFile_sinkdata (JNIEnv * env, jobject jthis, jbyteArray source, jint dest, jint bytes)
{
    LOGD("sinkdata (0x%lx, 0x%lx, 0x%lx, %i@0x%x)", (long)env, (long)jthis, (long)source, bytes, dest);
    jbyte *body = env->GetByteArrayElements(source, 0);
    
    char * dest_ptr = (char*) dest;
    char * src_ptr = (char*) body;
    memcpy(dest_ptr, src_ptr, bytes);
    
    env->ReleaseByteArrayElements(source, body, 0);
    LOGD("sinkdata 1");
}

extern "C" void Java_nl_elements_edgelib_EdgelibFile_debug
  (JNIEnv * env, jobject jthis, jstring string)
{
    LOGD("sinkdata (0x%lx, 0x%lx, 0x%lx)", (long)env, (long)jthis, (long)string);
    const char *nativeString = env->GetStringUTFChars(string, 0);
    //debug(nativeString);
    env->ReleaseStringUTFChars(string, nativeString);
}

// C-to-java

jobject Call_EdgelibFile_open(const char * fn)
{
    JNIEnv * env = android_global_instance->jenv;
    LOGD("File::open (%s, 0x%lx)", fn, (long)env);
    
    //fn = "http://www.google.com/";
    
    if (!android_global_instance->jassetmanager)
    	android_global_instance->jassetmanager = env->NewGlobalRef(Call_EdgelibApp_getAssetManager(env, android_global_instance->jappobject));    

    LOGD("File::open 1 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    
    if (!android_global_instance->jassetmanager) return NULL;    

    LOGD("File::open 2 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibFile");
    if (!cls) return NULL;
    jstring js = env->NewStringUTF(fn);
    jmethodID mid = env->GetStaticMethodID(cls, "open", "(Landroid/content/res/AssetManager;Ljava/lang/String;)Lnl/elements/edgelib/EdgelibFile;");
    if (!mid) return NULL;
    LOGD("File::open 3 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    jobject localfile = env->CallStaticObjectMethod(cls, mid, android_global_instance->jassetmanager, js);
    LOGD("File::open 4 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    //debug("[EdgelibFile::Open]\n");
    env->DeleteLocalRef(js);
    env->DeleteLocalRef(cls);
    jobject globalfile = env->NewGlobalRef(localfile);
    env->DeleteLocalRef(localfile);
    LOGD("File::open 5 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    return globalfile;
}

jobject Call_EdgelibFile_opendir(const char * fn)
{
    JNIEnv * env = android_global_instance->jenv;
    LOGD("File::opendir (%s, 0x%lx)", fn, (long)env);
    
    if (!android_global_instance->jassetmanager)
        android_global_instance->jassetmanager = env->NewGlobalRef(Call_EdgelibApp_getAssetManager(env, android_global_instance->jappobject));
    
    LOGD("File::opendir 1 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    
    if (!android_global_instance->jassetmanager) return NULL;
    
    LOGD("File::opendir 2 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibFile");
    if (!cls) return NULL;
    jstring js = env->NewStringUTF(fn);
    jmethodID mid = env->GetStaticMethodID(cls, "opendir", "(Landroid/content/res/AssetManager;Ljava/lang/String;)Lnl/elements/edgelib/EdgelibFile;");
    if (!mid) return NULL;
    LOGD("File::opendir 3 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    jobject localfile = env->CallStaticObjectMethod(cls, mid, android_global_instance->jassetmanager, js);
    LOGD("File::opendir 4 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    //debug("[EdgelibFile::Open]\n");
    env->DeleteLocalRef(js);
    env->DeleteLocalRef(cls);
    jobject globalfile = env->NewGlobalRef(localfile);
    env->DeleteLocalRef(localfile);
    LOGD("File::opendir 5 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
    return globalfile;
}

void Call_EdgelibFile_close(jobject file)
{
    LOGD("File::close 1");
    if (!file) return;
    //debug("[EdgelibFile::Close]\n");
    JNIEnv * env = android_global_instance->jenv;
    LOGD("File::close (0x%lx, 0x%lx)", (long)file, (long)env);
    env->DeleteGlobalRef(file);
    LOGD("File::close 2 (0x%lx, 0x%lx)", (long)env, (long)android_global_instance->jenv);
}

// public boolean isValid()
bool Call_EdgelibFile_isValid(jobject file)
{
    JNIEnv * env = android_global_instance->jenv;
    LOGD("File::isValid (0x%lx, 0x%lx)", (long)env, (long)file);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibFile");
    jmethodID mid = env->GetMethodID(cls, "isValid", "()I");
    int result = env->CallIntMethod(file, mid);    
    env->DeleteLocalRef(cls);
    if (result == 0) return true;
    return false;
}

// public int read(int pointer, int bytes);
int Call_EdgelibFile_read(jobject file, char * data, int len)
{   
    JNIEnv * env = android_global_instance->jenv;
    LOGD("File::read (0x%lx, 0x%lx, %i@0x%lx)", (long)env, (long)file, len, (long)data);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibFile");
    jmethodID mid = env->GetMethodID(cls, "read", "(II)I");
    //if (len <= 0) debug("[read: negative length]");
    int retval = env->CallIntMethod(file, mid, (int)data, len);
    //if (retval < 0) *(char*)(2-retval) = 0;
    env->DeleteLocalRef(cls);
    return retval;
}

// public int seekstart(int pos);
int Call_EdgelibFile_seekstart(jobject file, int pos)
{
    JNIEnv * env = android_global_instance->jenv;
    LOGD("File::seekstart (0x%lx, 0x%lx, %i)", (long)env, (long)file, pos);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibFile");
    jmethodID mid = env->GetMethodID(cls, "seekstart", "(I)I");
    env->DeleteLocalRef(cls);
    return env->CallIntMethod(file, mid, pos);
}

// public int seekskip(int pos);
int Call_EdgelibFile_seekskip(jobject file, int pos)
{
    JNIEnv * env = android_global_instance->jenv;
    LOGD("File::seekskip (0x%lx, 0x%lx, %i)", (long)env, (long)file, pos);    
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibFile");
    jmethodID mid = env->GetMethodID(cls, "seekskip", "(I)I");
    env->DeleteLocalRef(cls);
    return env->CallIntMethod(file, mid, pos);
}

int Call_EdgelibFile_seekend(jobject file, int pos)
{
    JNIEnv * env = android_global_instance->jenv;
    LOGD("File::seekend (0x%lx, 0x%lx, %i)", (long)env, (long)file, pos);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibFile");
    jmethodID mid = env->GetMethodID(cls, "seekend", "(I)I");
    env->DeleteLocalRef(cls);
    return env->CallIntMethod(file, mid, pos);
}

int Call_EdgelibFile_getLength(jobject file)
{
    JNIEnv * env = android_global_instance->jenv;
    LOGD("File::getLength (0x%lx, 0x%lx)", (long)env, (long)file);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibFile");
    jmethodID mid = env->GetMethodID(cls, "getLength", "()I");
    env->DeleteLocalRef(cls);
    return env->CallIntMethod(file, mid);
}

int Call_EdgelibFile_statlength(const char * fn)
{
    static char lastfile[128] = "";
    static int lastsize = 0;

    LOGD("statlength(%s)", fn);
    
    if (strcmp(&(lastfile[0]), fn) == 0)
        return lastsize;

    LOGD("statlength 1");
    jobject testfile = Call_EdgelibFile_open(fn);
    if (!Call_EdgelibFile_isValid(testfile)) 
    {
        LOGD("statlength 2");
        Call_EdgelibFile_close(testfile);
        LOGD("statlength 3");
        return -1;
    }
    LOGD("statlength 4");
    int len = Call_EdgelibFile_getLength(testfile);
    LOGD("statlength 5");
    if (len > 0)
    {
        // cache result for speed
        lastsize = len;
        LOGD("statlength 6");
        strcpy(lastfile, fn);
    }
    LOGD("statlength 7");
    Call_EdgelibFile_close(testfile);
    LOGD("statlength 8");
    return len;
}

int Call_EdgelibFile_getDirectoryCount(jobject file)
{
    JNIEnv * env = android_global_instance->jenv;
    LOGD("File::getDirectoryCount (0x%lx, 0x%lx)", (long)env, (long)file);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibFile");
    jmethodID mid = env->GetMethodID(cls, "getDirectoryCount", "()I");
    env->DeleteLocalRef(cls);
    return env->CallIntMethod(file, mid);
}

char * Call_EdgelibFile_getDirectoryEntry(jobject file, int index)
{
    JNIEnv * env = android_global_instance->jenv;
    LOGD("File::getDirectoryCount (0x%lx, 0x%lx, %i)", (long)env, (long)file, index);
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibFile");
    jmethodID mid = env->GetMethodID(cls, "getDirectoryEntry", "(I)Ljava/lang/String;");
    env->DeleteLocalRef(cls);
    jobject text = env->CallObjectMethod(file, mid, index);
    if (!text) return NULL;

    char * output = copyjstring(env, (jstring)text);
    env->DeleteLocalRef(text);
    return output;
}
