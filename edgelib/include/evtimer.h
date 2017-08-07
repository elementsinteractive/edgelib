// evtimer.h
// The virtual timer module
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED    //Can only be included by the main engine
#ifndef EVTIMER_INCLUDED //Make sure the module isn't included multiple times
#define EVTIMER_INCLUDED

///////////////////////////////////////////////////////////////////
// Classes                                                       //
///////////////////////////////////////////////////////////////////

class ClassERTimer
{
	private:
		unsigned long lasttime;
		unsigned long prevtime;
	public:
		ClassERTimer(void);
		unsigned long Delta(unsigned long newtime, unsigned long oldtime);
		void Update(void);
		static unsigned long Count(void);
		unsigned long RetrieveDelta(void);
};//The real timer class

class ClassEVTimer
{
	private:
		bool suspended;
		unsigned long starttime;
		unsigned long lasttime;
		unsigned long reserve;
		unsigned long speed;
		unsigned long prevvirtualtime;
		unsigned long virtualtime;
	public:
		ClassERTimer real;
		ClassEVTimer(void);
		void Reset(void);
		void Reset(unsigned long ntime);
		void SetSpeed(unsigned long nspeed);
		void Update(void);
		void Update(unsigned long ntime);
		void Increase(unsigned long ptime);
		unsigned long RetrieveTime(void);
		unsigned long RetrieveDelta(void);
		void Suspend(void);
		void Resume(void);
		void Resume(unsigned long ntime);
};//The virtual timer class

#endif
#endif
