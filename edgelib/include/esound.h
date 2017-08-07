// esound.h
// Sound wrapper class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Library definition                                            //
///////////////////////////////////////////////////////////////////

#ifdef  EDGE_INCLUDED   //Can only be included by the main engine
#ifndef ESOUND_INCLUDED //Make sure the module isn't included multiple times
#define ESOUND_INCLUDED

///////////////////////////////////////////////////////////////////
// Definitions                                                   //
///////////////////////////////////////////////////////////////////

//Limits
#define MAX_SOUNDEFFECTS   256
#define MAX_MUSIC           32
#define MAX_CHNLSOUND        8
#define MAX_CHNLMUSIC        2

//Effect flags
#define SFX_NONE             0
#define SFX_FADEIN           1
#define SFX_FADEOUT          2
#define SFX_CROSSFADE        4
#define SFX_MULTIMUSIC       8

///////////////////////////////////////////////////////////////////
// Structures                                                    //
///////////////////////////////////////////////////////////////////

//Sound effect or music object
typedef struct
{
	void *object;
	void *data;
	bool lastplayed;
}ESNDOBJECT;

//A sound or music channel
typedef struct
{
	long playingobject;
	long queuedobject;
	unsigned long fxprop;
	long fxtimer;
	unsigned long usefx;
	bool loop;
}ESOUNDCHANNEL;

///////////////////////////////////////////////////////////////////
// Class definition                                              //
///////////////////////////////////////////////////////////////////

class ClassESound
{
	private:
		ESNDOBJECT sound[MAX_SOUNDEFFECTS];
		ESNDOBJECT music[MAX_MUSIC];
		ESOUNDCHANNEL chnlsound[MAX_CHNLSOUND];
		ESOUNDCHANNEL chnlmusic[MAX_CHNLMUSIC];
		void *driver;
		unsigned char cursoundvolume;
		unsigned char curmusicvolume;
		bool opened;
		bool suspended;
		bool muted;
	public:
		ClassESound(void);
		~ClassESound(void);
		ERESULT Open(void);
		ERESULT Open(unsigned long frequency, unsigned char bitspersample, bool stereo);
		void Suspend(void);
		void Resume(void);
		void Close(void);
		ERESULT LoadSoundEffect(unsigned long index, void *data, unsigned long datasize);
		ERESULT LoadSoundEffect(unsigned long index, char *sndfile);
		ERESULT LoadSoundEffect(unsigned long index, WCHAR *sndfile);
		void FreeSoundEffect(unsigned long index);
		ERESULT LoadMusic(unsigned long index, void *data, unsigned long datasize);
		ERESULT LoadMusic(unsigned long index, char *sndfile);
		ERESULT LoadMusic(unsigned long index, WCHAR *sndfile);
		void FreeMusic(unsigned long index);
		void SetSoundVolume(unsigned char newvolume);
		void SetMusicVolume(unsigned char newvolume);
		long PlaySound(unsigned long index, long playspeed = 0, signed char pan = 0, bool loopsound = false, unsigned long flags = SFX_NONE, unsigned long property = 0);
		long PlayMusic(unsigned long index, bool loopmusic = true, unsigned long flags = SFX_NONE, unsigned long property = 0);
		void StopSound(unsigned long index, unsigned long flags = SFX_NONE, unsigned long property = 0);
		void StopMusic(unsigned long index, unsigned long flags = SFX_NONE, unsigned long property = 0);
		void StopSoundAll(unsigned long flags = SFX_NONE, unsigned long property = 0);
		void StopMusicAll(unsigned long flags = SFX_NONE, unsigned long property = 0);
		void StopSoundChannel(long channel, unsigned long flags = SFX_NONE, unsigned long property = 0);
		void StopMusicChannel(long channel, unsigned long flags = SFX_NONE, unsigned long property = 0);
		long MusicPlaying(void);
		void SetMute(bool state);
		void Update(unsigned long timedelta);
};//The sound class

#endif
#endif
