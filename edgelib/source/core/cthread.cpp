// cthread.cpp
// Edgecore: provides threading API
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

//Device include
#include "edgedevice.h"

//Device SDK includes
#if defined(DEVICE_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#elif defined(DEVICE_POSIX)
	#include <pthread.h>
	#include <sys/time.h>
#elif defined(DEVICE_SYMBIAN)
	#include <eikenv.h>	
#endif

//Core include
#include "edgecore.h"

///////////////////////////////////////////////////////////////////
// Internal definitions and structures
///////////////////////////////////////////////////////////////////

//A thread object
typedef struct
{
	#if defined(DEVICE_WIN32)
		HANDLE t;
		DWORD threadid;
	#elif defined(DEVICE_SYMBIAN)
		int t;
	#elif defined(DEVICE_POSIX)
		pthread_t t;
	#elif defined(DEVICE_OPENKODE)
		int t;
	#else
		int t;
	#endif
	void *param;
	void *(*entry)(ECOREOBJ);
	ECOREOBJ exitevent;
}EC_THREADOBJ;

//A mutex object
typedef struct
{
	#if defined(DEVICE_WIN32)
		HANDLE mutex;
		HANDLE event;
	#elif defined(DEVICE_SYMBIAN)
		int mutex;
		int event;
	#elif defined(DEVICE_POSIX)
		pthread_mutex_t mutex;
		pthread_cond_t event;
		pthread_mutex_t eventmutex;
		bool triggered;
	#elif defined(DEVICE_OPENKODE)
		int mutex;
		int event;
	#else
		int mutex;
		int event;
	#endif
}EC_SIGNALOBJ;


///////////////////////////////////////////////////////////////////
// Threading API
///////////////////////////////////////////////////////////////////

#if defined(DEVICE_WIN32)

//Thread entry with parameters matching win32 API
unsigned long __stdcall eciTSubEntry(ECOREOBJ to)
{
	EC_THREADOBJ *tobj = (EC_THREADOBJ *)to;
	tobj->entry(to);
	ecTSetSignal(tobj->exitevent);
	return(0);
}

#elif defined(DEVICE_POSIX)

//Thread entry with parameters matching win32 API
void *eciTSubEntry(ECOREOBJ to)
{
	EC_THREADOBJ *tobj = (EC_THREADOBJ *)to;
	tobj->entry(to);
	ecTSetSignal(tobj->exitevent);
	pthread_exit(NULL);
	return(NULL);
}

#endif

//Create thread
ECOREOBJ ecTCreate(ecErr &errcode, void *(*entry)(ECOREOBJ), void *param)
{
	errcode = 0;
	EC_THREADOBJ *to = new EC_THREADOBJ;
	if (to)
	{
		to->param = param;
		to->entry = entry;
		to->exitevent = ecTCreateSignal(errcode);
		if (errcode)
			ecTDestroySignal(to->exitevent);
		if (errcode == 0)
		{
			#if defined(DEVICE_WIN32)
				to->t = CreateThread(NULL, 0, eciTSubEntry, to, CREATE_SUSPENDED, &to->threadid);
				if (to->t != NULL)
				{
					Sleep(10);
					ResumeThread(to->t);
				}
				else
					errcode = 3; //E_ERROR
			#elif defined(DEVICE_POSIX)
				int result = pthread_create(&to->t, NULL, eciTSubEntry, to);
				if (result != 0)
					errcode = 3;
				else
					pthread_detach(to->t);
			#else
				errcode = 2; //E_UNSUPPORTED
			#endif
		}
		if (errcode)
		{
			delete to;
			to = NULL;
		}
	}
	else
		errcode = 1; //E_NOMEMORY
	return(to);
}

//Destroy thread resources
void ecTDestroy(ECOREOBJ t, bool waitstop)
{
	EC_THREADOBJ *to = (EC_THREADOBJ *)t;
	if (waitstop)
		ecTWaitSignal(to->exitevent, -1);
	ecTDestroySignal(to->exitevent);
	#if defined(DEVICE_WIN32)
		CloseHandle(to->t);
	#endif
}

//Return parameter inside object
void *ecTGetParam(ECOREOBJ t)
{
	EC_THREADOBJ *to = (EC_THREADOBJ *)t;
	return(to->param);
}

//Create signal object
ECOREOBJ ecTCreateSignal(ecErr &errcode)
{
	errcode = 0;
	EC_SIGNALOBJ *so = new EC_SIGNALOBJ;
	if (so)
	{
		#if defined(DEVICE_WIN32)
			so->mutex = CreateMutex(NULL, FALSE, NULL);
			so->event = CreateEvent(NULL, TRUE, FALSE, NULL);
			if (so->mutex == NULL || so->event == NULL)
			{
				if (so->mutex != NULL)
					CloseHandle(so->mutex);
				errcode = 3; //E_ERROR
			}
		#elif defined(DEVICE_POSIX)
			int result = pthread_mutex_init(&so->mutex, NULL);
			if (result == 0)
			{
				so->triggered = false;
				result = pthread_mutex_init(&so->eventmutex, NULL);
				if (result == 0)
				{
					result = pthread_cond_init(&so->event, NULL);
					if (result != 0)
					{
						pthread_mutex_destroy(&so->mutex);
						pthread_mutex_destroy(&so->eventmutex);
						errcode = 3;
					}
				}
				else
				{
					pthread_mutex_destroy(&so->mutex);
					errcode = 3;
				}
			}
			else
				errcode = 3; //E_ERROR
		#else
			errcode = 2; //E_UNSUPPORTED
		#endif
		if (errcode)
		{
			delete so;
			so = NULL;
		}
	}
	else
		errcode = 1; //E_NOMEMORY
	return(so);
}

//Destroy signal object
void ecTDestroySignal(ECOREOBJ s)
{
	EC_SIGNALOBJ *so = (EC_SIGNALOBJ *)s;
	#if defined(DEVICE_WIN32)
		CloseHandle(so->mutex);
		CloseHandle(so->event);
	#elif defined(DEVICE_POSIX)
		pthread_mutex_destroy(&so->mutex);
		pthread_mutex_destroy(&so->eventmutex);
		pthread_cond_destroy(&so->event);
	#endif
	delete so;
}

//Lock signal object
void ecTLockSignal(ECOREOBJ s)
{
	EC_SIGNALOBJ *so = (EC_SIGNALOBJ *)s;
	#if defined(DEVICE_WIN32)
		WaitForSingleObject(so->mutex, INFINITE);
	#elif defined(DEVICE_POSIX)
		pthread_mutex_lock(&so->mutex);
	#endif
}

//Unlock signal object
void ecTUnlockSignal(ECOREOBJ s)
{
	EC_SIGNALOBJ *so = (EC_SIGNALOBJ *)s;
	#if defined(DEVICE_WIN32)
		ReleaseMutex(so->mutex);
	#elif defined(DEVICE_POSIX)
		pthread_mutex_unlock(&so->mutex);
	#endif
}

//Set signal object
void ecTSetSignal(ECOREOBJ s)
{
	EC_SIGNALOBJ *so = (EC_SIGNALOBJ *)s;
	#if defined(DEVICE_WIN32)
		SetEvent(so->event);
	#elif defined(DEVICE_POSIX)
		pthread_mutex_lock(&so->eventmutex);
		so->triggered = true;
		pthread_cond_signal(&so->event);
		pthread_mutex_unlock(&so->eventmutex);
	#endif
}

//Reset signal object
void ecTResetSignal(ECOREOBJ s)
{
	EC_SIGNALOBJ *so = (EC_SIGNALOBJ *)s;
	#if defined(DEVICE_WIN32)
		ResetEvent(so->event);
	#elif defined(DEVICE_POSIX)
		pthread_mutex_lock(&so->eventmutex);
		so->triggered = true;
		pthread_mutex_unlock(&so->eventmutex);
	#endif
}

//Wait for signal to be set
bool ecTWaitSignal(ECOREOBJ s, long timeout)
{
	EC_SIGNALOBJ *so = (EC_SIGNALOBJ *)s;
	#if defined(DEVICE_WIN32)
		DWORD result, waittype = timeout;
		if (timeout < 0)
			waittype = INFINITE;
		result = WaitForSingleObject(so->event, waittype);
		if (result == WAIT_OBJECT_0)
			return(true);
	#elif defined(DEVICE_POSIX)
		pthread_mutex_lock(&so->eventmutex);
		while (!so->triggered)
		{
			if (timeout >= 0)
			{
				struct timespec ts;
				struct timeval tv;
				gettimeofday(&tv, NULL);
				tv.tv_sec += (timeout + tv.tv_usec / 1000) / 1000;
				tv.tv_usec = (tv.tv_usec + timeout * 1000) % 1000000;
				ts.tv_sec = tv.tv_sec;
				ts.tv_nsec = tv.tv_usec * 1000;
				pthread_cond_timedwait(&so->event, &so->eventmutex, &ts);
				break;
			}
			else
				pthread_cond_wait(&so->event, &so->eventmutex);
		}
		pthread_mutex_unlock(&so->eventmutex);
	#endif
	return(false);
}
