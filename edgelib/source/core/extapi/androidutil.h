// androidutil.h
// Android util
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

void Call_EdgelibUtil_vibrate()
{
    JNIEnv * env = android_global_instance->jenv;
    
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibUtil");
    jmethodID mid = env->GetMethodID(cls, "vibrate", "()V");
    env->CallVoidMethod(android_global_instance->jutilobject, mid);	
    env->DeleteLocalRef(cls);
}

// public int getBatteryLevel()
int Call_EdgelibUtil_getBatteryLevel()
{
	JNIEnv * env = android_global_instance->jenv;
	jobject jthis = android_global_instance->jutilobject;
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibUtil");
    jmethodID mid = env->GetMethodID(cls, "getBatteryLevel", "()I");
    int retval = env->CallIntMethod(jthis, mid);
    env->DeleteLocalRef(cls);
    return retval;
}

// public int getBatteryState()
int Call_EdgelibUtil_getBatteryState()
{
	JNIEnv * env = android_global_instance->jenv;
	jobject jthis = android_global_instance->jutilobject;
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibUtil");
    jmethodID mid = env->GetMethodID(cls, "getBatteryState", "()I");
    int retval = env->CallIntMethod(jthis, mid);
    env->DeleteLocalRef(cls);
    return retval;
}

static char * copyjstringto(JNIEnv * env, jstring string, char * output)
{
	const char *nativeString = env->GetStringUTFChars(string, 0);
	strcpy(output, nativeString);
	env->ReleaseStringUTFChars(string, nativeString);
	return output;
}

void Call_EdgelibUtil_getIMEI(char * output)
{
	JNIEnv * env = android_global_instance->jenv;
	jobject jthis = android_global_instance->jutilobject;
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibUtil");
    jmethodID mid = env->GetMethodID(cls, "getIMEI", "()Ljava/lang/String;");
    jobject retval = env->CallObjectMethod(jthis, mid);
	copyjstringto(env, (jstring) retval, output);
    env->DeleteLocalRef(cls);
}

void Call_EdgelibUtil_getSerial(char * output)
{
	JNIEnv * env = android_global_instance->jenv;
	jobject jthis = android_global_instance->jutilobject;
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibUtil");
    jmethodID mid = env->GetMethodID(cls, "getSerial", "()Ljava/lang/String;");
    jobject retval = env->CallObjectMethod(jthis, mid);
	copyjstringto(env, (jstring) retval, output);
    env->DeleteLocalRef(cls);
}

void Call_EdgelibUtil_getLocale(char * output)
{
	JNIEnv * env = android_global_instance->jenv;
	jobject jthis = android_global_instance->jutilobject;
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibUtil");
    jmethodID mid = env->GetMethodID(cls, "getLocale", "()Ljava/lang/String;");
    jobject retval = env->CallObjectMethod(jthis, mid);
	copyjstringto(env, (jstring) retval, output);
    env->DeleteLocalRef(cls);
}

int Call_EdgelibUtil_getTimezoneOffset()
{
	JNIEnv * env = android_global_instance->jenv;
	jobject jthis = android_global_instance->jutilobject;
    jclass cls = env->FindClass("nl/elements/edgelib/EdgelibUtil");
    jmethodID mid = env->GetMethodID(cls, "getTimezoneOffset", "()I");
    int retval = env->CallIntMethod(jthis, mid);
    env->DeleteLocalRef(cls);
    return retval;
}
