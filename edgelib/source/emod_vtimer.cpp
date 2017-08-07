// emod_vtimer.cpp
// Virtual timer class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#if defined(DEVICE_SYMBIAN)
	#include <hal.h>
#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
	#include <sys/time.h>
#endif

#define EDGE_INCLUDED
#include "evtimer.h"

#include "extra/verinfo.h"

/////////////////////////////////////////////////////////////////////
// ClassERTimer::Public                                            //
/////////////////////////////////////////////////////////////////////

//The ClassERTimer class constructor
ClassERTimer::ClassERTimer(void)
{
	lasttime    = 0;
	prevtime    = 0;
}

//Calculate the difference in time (prevent data overflow errors)
unsigned long ClassERTimer::Delta(unsigned long newtime, unsigned long oldtime)
{
	unsigned long temp = 0;
	if (oldtime > newtime)
		temp = ~((unsigned long)0) - oldtime + newtime + 1;
	else
		temp = newtime - oldtime;
	
	return temp;
}

//Update the timer
void ClassERTimer::Update(void)
{
	prevtime = lasttime;
	lasttime = Count();
}

//Retrieve the time tick count
unsigned long ClassERTimer::Count(void)
{
	#if defined(DEVICE_SYMBIAN)
		TInt precision;
		HAL::Get(HALData::ESystemTickPeriod, precision);
		return(User::TickCount() * precision / 1000);
	#elif defined(DEVICE_WIN32)
		#if defined(DEVICE_DESKTOP)
			LARGE_INTEGER time_freq, time_ctr;
			if (QueryPerformanceFrequency(&time_freq) == TRUE)
				if (QueryPerformanceCounter(&time_ctr) == TRUE)
					if (time_freq.QuadPart > 0)
						return((unsigned long)(time_ctr.QuadPart * 1000 / time_freq.QuadPart));
		#endif
		return(GetTickCount());
	#elif defined(DEVICE_LINUX) || defined(DEVICE_MAC)
		struct timeval tv;
		struct timezone tz;
		gettimeofday(&tv, &tz);
		return(tv.tv_usec / 1000 + tv.tv_sec * 1000);
	#elif defined(DEVICE_OPENKODE)
		return((unsigned long)(kdGetTimeUST() / 1000000));
	#endif
	return(0);
}

//Retrieve the difference in time before a call to update()
unsigned long ClassERTimer::RetrieveDelta(void)
{
	return(Delta(lasttime, prevtime));
}

/////////////////////////////////////////////////////////////////////
// ClassEVTimer::Public                                            //
/////////////////////////////////////////////////////////////////////

//The ClassEVTimer class constructor
ClassEVTimer::ClassEVTimer(void)
{
	starttime       = 0;
	lasttime        = 0;
	reserve         = 0;
	prevvirtualtime = 0;
	virtualtime     = 0;
	speed           = 1000;
	suspended       = false;
}

//Reset the virtual timer automatically
void ClassEVTimer::Reset(void)
{
	Reset(real.Count());
}

//Reset the virtual timer manually
void ClassEVTimer::Reset(unsigned long ntime)
{
	starttime = ntime;
	lasttime  = starttime;
	virtualtime = 0;
	reserve     = 0;
	suspended = false;
}

//Set the speed of the virtual timer
void ClassEVTimer::SetSpeed(unsigned long nspeed)
{
	speed = nspeed;
}

//Update the virtual timer automatically
void ClassEVTimer::Update(void)
{
	Update(real.Count());
}

//Update the virtual timer manually
void ClassEVTimer::Update(unsigned long ntime)
{
	unsigned long exreserve = reserve;
	real.Update();
	if (suspended)
	{
		prevvirtualtime = virtualtime;
		return;
	}
	prevvirtualtime = virtualtime;
	reserve = (reserve + real.Delta(ntime, lasttime) * speed) % 1000;
	virtualtime += (real.Delta(ntime, lasttime) * speed + exreserve) / 1000;
	lasttime = ntime;
}

//Increase the timer counter
void ClassEVTimer::Increase(unsigned long ptime)
{
	unsigned long exreserve = reserve;
	if (suspended)
	{
		prevvirtualtime = virtualtime;
		return;
	}
	prevvirtualtime = virtualtime;
	reserve = (reserve + ptime * speed) % 1000;
	virtualtime += (ptime * speed + exreserve) / 1000;
}

//Retrieve the time count of the virtual timer
unsigned long ClassEVTimer::RetrieveTime(void)
{
	return(virtualtime);
}

//Retrieve the difference in time before a call to update()
unsigned long ClassEVTimer::RetrieveDelta(void)
{
	return(real.Delta(virtualtime, prevvirtualtime));
}

//Suspend the virtual timer
void ClassEVTimer::Suspend(void)
{
	if (suspended)
		return;
	suspended = true;
}

//Resume the virtual timer automatically
void ClassEVTimer::Resume(void)
{
	Resume(real.Count());
}

//Resume the virtual timer, give a manual time update
void ClassEVTimer::Resume(unsigned long ntime)
{
	if (!suspended)
		return;
	lasttime = ntime;
	suspended = false;
}
