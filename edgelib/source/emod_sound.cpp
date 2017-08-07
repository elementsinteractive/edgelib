// emod_sound.cpp
// Sound class
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
///////////////////////////////////////////////////////////////////

#include "edgedevice.h"
#include "edgedef.h"

#define EDGE_INCLUDED
#include "estdc.h"
#include "ememory.h"
#include "efile.h"
#include "esound.h"

#include "core/edgecore.h"
#include "extra/verinfo.h"

/////////////////////////////////////////////////////////////////////
// Function prototypes                                             //
/////////////////////////////////////////////////////////////////////

//Open, close, suspend and resume
void *ESound_NewDriver(void);
void ESound_FreeDriver(void *driver);
long ESound_Open(void *driver, unsigned long frequency, unsigned char bitspersample, bool stereo, unsigned char chnlsound, unsigned char chnlmusic);
void ESound_Close(void *driver);
void ESound_Suspend(void *driver);
void ESound_Resume(void *driver);

//Loading of sound effects and music
bool ESound_KeepFilePointers(void);
bool ESound_SoundFromFile(void);
bool ESound_MusicFromFile(void);
void *ESound_NewSound(void *driver);
ERESULT ESound_LoadSound(void *driver, void *object, void *data, unsigned long datasize);
ERESULT ESound_LoadSound(void *driver, void *object, const char *filename);
void ESound_FreeSound(void *driver, void *object);
void *ESound_NewMusic(void *driver);
ERESULT ESound_LoadMusic(void *driver, void *object, void *data, unsigned long datasize);
ERESULT ESound_LoadMusic(void *driver, void *object, const char *filename);
void ESound_FreeMusic(void *driver, void *object);

//Playing sounds and music
void ESound_VolumeSound(void *driver, unsigned char volume);
void ESound_VolumeMusic(void *driver, unsigned char volume);
long ESound_PlaySound(void *driver, void *object, long playspeed, signed char pan, bool loopsound);
long ESound_PlayMusic(void *driver, void *object, bool loopmusic);
void ESound_StopSound(void *driver, void *object, long channel);
void ESound_StopMusic(void *driver, void *object, long channel);
void ESound_Update(void *driver, unsigned long timedelta);

//Channels (advanced)
void ESound_ChannelSoundVolume(void *driver, void *object, long channel, unsigned char volume);
void ESound_ChannelMusicVolume(void *driver, void *object, long channel, unsigned char volume);

/////////////////////////////////////////////////////////////////////
// ClassESound::Private                                            //
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// ClassESound::Public                                             //
/////////////////////////////////////////////////////////////////////

//ClassESound: constructor
ClassESound::ClassESound(void)
{
	unsigned long ctr;
	for (ctr = 0; ctr < MAX_SOUNDEFFECTS; ctr++)
		sound[ctr].object = NULL;
	for (ctr = 0; ctr < MAX_MUSIC; ctr++)
		music[ctr].object = NULL;
	for (ctr = 0; ctr < MAX_CHNLSOUND; ctr++)
		chnlsound[ctr].queuedobject = -1;
	for (ctr = 0; ctr < MAX_CHNLMUSIC; ctr++)
		chnlmusic[ctr].queuedobject = -1;
	opened = false;
	driver = NULL;
}

//ClassESound: destructor
ClassESound::~ClassESound(void)
{
	Close();
}

//Open the sound system using recommended settings per platform
ERESULT ClassESound::Open(void)
{
	#if defined(DEVICE_SYMBIAN)
		ERESULT result = Open(16000, 16, false);
		if (result != E_OK)
			return(Open(16000, 8, false));
	#elif defined(DEVICE_LINUX)
		ERESULT result = Open(44100, 16, true);
		if (result != E_OK)
			return(Open(44100, 8, true));
	#elif defined(DEVICE_POCKETPC)
		ERESULT result = Open(44100, 16, false);
		if (result != E_OK)
			return(Open(44100, 8, false));
	#elif defined(DEVICE_SMARTPHONE)
		ERESULT result = Open(22050, 16, false);
		if (result != E_OK)
			return(Open(22050, 8, false));
	#else
		ERESULT result = Open(44100, 16, true);
		if (result != E_OK)
			return(Open(44100, 8, true));
	#endif
	return(E_OK);
}

//Open the sound system
ERESULT ClassESound::Open(unsigned long frequency, unsigned char bitspersample, bool stereo)
{
	unsigned long ctr;
	Close();
	if (!opened)
	{
		ERESULT openresult;
		if (driver == NULL)
			driver = ESound_NewDriver();
		if (driver == NULL)
			return(E_ERROR);
		opened = true;
		suspended = false;
		muted = false;
		cursoundvolume = 0;
		curmusicvolume = 0;
		openresult = ESound_Open(driver, frequency, bitspersample, stereo, MAX_CHNLSOUND, MAX_CHNLMUSIC);
		if (openresult == E_OK)
		{
			SetSoundVolume(96);
			SetMusicVolume(96);
			for (ctr = 0; ctr < MAX_CHNLSOUND; ctr++)
				chnlsound[ctr].playingobject = -1;
			for (ctr = 0; ctr < MAX_CHNLMUSIC; ctr++)
				chnlmusic[ctr].playingobject = -1;
		}
		else
			opened = false;
		return(openresult);
	}
	return(E_OK);
}

//Suspend sound output
void ClassESound::Suspend(void)
{
	if (opened && !suspended)
		ESound_Suspend(driver);
	suspended = true;
}

//Resume sound output
void ClassESound::Resume(void)
{
	if (!muted)
	{
		if (opened && suspended)
			ESound_Resume(driver);
		suspended = false;
	}
}

//Close the sound system
void ClassESound::Close(void)
{
	if (driver)
	{
		if (opened)
		{
			unsigned long ctr;
			StopSoundAll();
			StopMusicAll();
			/*if (suspended)
			{
				ESound_Resume(driver);
				Sleep(100);
			}*/
			for (ctr = 0; ctr < MAX_SOUNDEFFECTS; ctr++)
				FreeSoundEffect(ctr);
			for (ctr = 0; ctr < MAX_MUSIC; ctr++)
				FreeMusic(ctr);
			ESound_Close(driver);
			opened = false;
		}
		ESound_FreeDriver(driver);
		driver = NULL;
	}
}

//Load a sound effect object
ERESULT ClassESound::LoadSoundEffect(unsigned long index, void *data, unsigned long datasize)
{
	if (!opened)
		return(E_NOACTION);
	if (index < MAX_SOUNDEFFECTS)
	{
		FreeSoundEffect(index);
		sound[index].object = ESound_NewSound(driver);
		if (sound[index].object)
		{
			ERESULT loadresult = ESound_LoadSound(driver, sound[index].object, data, datasize);
			if (loadresult == E_OK)
			{
				if (!ESound_KeepFilePointers())
				{
					ClassEMemory::DeAlloc(data);
					sound[index].data = NULL;
				}
				else
					sound[index].data = data;
				sound[index].lastplayed = false;
			}
			return(loadresult);
		}
		else
			return(E_ERROR);
	}
	return(E_INVALIDPARAM);
}

//Load a sound effect object
ERESULT ClassESound::LoadSoundEffect(unsigned long index, char *sndfile)
{
	ERESULT result;
	USTR_CHECK_BEGIN(sndfile);
	result = LoadSoundEffect(index, ustr_sndfile);
	USTR_CHECK_END(sndfile);
	return(result);
}

//Load a sound effect object
ERESULT ClassESound::LoadSoundEffect(unsigned long index, WCHAR *sndfile)
{
	if (ESound_SoundFromFile())
	{
		if (!opened)
			return(E_NOACTION);
		WCHAR readonlypath[512];
		char utf8file[512];
		ecFGetReadOnlyPath(readonlypath);
		ClassEStd::StrToUtf8(utf8file, readonlypath);
		ClassEStd::StrToUtf8(&utf8file[ClassEStd::StrLen(utf8file)], sndfile);
		if (index < MAX_SOUNDEFFECTS)
		{
			FreeSoundEffect(index);
			sound[index].object = ESound_NewSound(driver);
			if (sound[index].object)
			{
				ERESULT loadresult = ESound_LoadSound(driver, sound[index].object, utf8file);
				sound[index].data = NULL;
				if (loadresult == E_OK)
					sound[index].lastplayed = false;
				return(loadresult);
			}
			else
				return(E_ERROR);
		}
		return(E_INVALIDPARAM);
	}
	ClassEFile sfl;
	ERESULT er;
	void *fdata = NULL;
	unsigned long fsize;
	if (!sfl.Open(sndfile, EFOF_READ))
		return(E_ERROR);
	fsize = sfl.Size();
	if (fsize == 0)
	{
		sfl.Close();
		return(E_ERROR);
	}
	fdata = ClassEMemory::Alloc(fsize);
	if (fdata == NULL)
	{
		sfl.Close();
		return(E_NOMEMORY);
	}
	sfl.Read(fdata, fsize);
	sfl.Close();
	er = LoadSoundEffect(index, fdata, fsize);
	if (er != E_OK)
		ClassEMemory::DeAlloc(fdata);
	return(er);
}

//Free a sound effect
void ClassESound::FreeSoundEffect(unsigned long index)
{
	if (!opened)
		return;
	if (index < MAX_SOUNDEFFECTS)
	{
		if (sound[index].object)
		{
			StopSound(index);
			ESound_FreeSound(driver, sound[index].object);
			sound[index].object = NULL;
			if (sound[index].data)
			{
				ClassEMemory::DeAlloc(sound[index].data);
				sound[index].data = NULL;
			}
		}
	}
}

//Load a music object
ERESULT ClassESound::LoadMusic(unsigned long index, void *data, unsigned long datasize)
{
	if (!opened)
		return(E_NOACTION);
	if (index < MAX_MUSIC)
	{
		FreeMusic(index);
		music[index].object = ESound_NewMusic(driver);
		if (music[index].object)
		{
			ERESULT loadresult = ESound_LoadMusic(driver, music[index].object, data, datasize);
			if (loadresult == E_OK)
			{
				if (!ESound_KeepFilePointers())
				{
					ClassEMemory::DeAlloc(data);
					music[index].data = NULL;
				}
				else
					music[index].data = data;
			}
			return(loadresult);
		}
		else
			return(E_ERROR);
	}
	return(E_INVALIDPARAM);
}

//Load a music object
ERESULT ClassESound::LoadMusic(unsigned long index, char *sndfile)
{
	ERESULT result;
	USTR_CHECK_BEGIN(sndfile);
	result = LoadMusic(index, ustr_sndfile);
	USTR_CHECK_END(sndfile);
	return(result);
}

//Load a music object
ERESULT ClassESound::LoadMusic(unsigned long index, WCHAR *sndfile)
{
	if (ESound_MusicFromFile())
	{
		if (!opened)
			return(E_NOACTION);
		WCHAR readonlypath[512];
		char utf8file[512];
		ecFGetReadOnlyPath(readonlypath);
		ClassEStd::StrToUtf8(utf8file, readonlypath);
		ClassEStd::StrToUtf8(&utf8file[ClassEStd::StrLen(utf8file)], sndfile);
		if (index < MAX_MUSIC)
		{
			FreeMusic(index);
			music[index].object = ESound_NewMusic(driver);
			if (music[index].object)
			{
				ERESULT loadresult = ESound_LoadMusic(driver, music[index].object, utf8file);
				music[index].data = NULL;
				return(loadresult);
			}
			else
				return(E_ERROR);
		}
		return(E_INVALIDPARAM);
	}
	ClassEFile sfl;
	ERESULT er;
	void *fdata = NULL;
	unsigned long fsize;
	if (!sfl.Open(sndfile, EFOF_READ))
		return(E_ERROR);
	fsize = sfl.Size();
	if (fsize == 0)
	{
		sfl.Close();
		return(E_ERROR);
	}
	fdata = ClassEMemory::Alloc(fsize);
	if (fdata == NULL)
	{
		sfl.Close();
		return(E_NOMEMORY);
	}
	sfl.Read(fdata, fsize);
	sfl.Close();
	er = LoadMusic(index, fdata, fsize);
	if (er != E_OK)
		ClassEMemory::DeAlloc(fdata);
	return(er);
}

//Free music
void ClassESound::FreeMusic(unsigned long index)
{
	if (!opened)
		return;
	if (index < MAX_MUSIC)
	{
		if (music[index].object)
		{
			StopMusic(index);
			ESound_FreeMusic(driver, music[index].object);
			music[index].object = NULL;
			if (music[index].data)
			{
				ClassEMemory::DeAlloc(music[index].data);
				music[index].data = NULL;
			}
		}
	}
}

//Change sound volume
void ClassESound::SetSoundVolume(unsigned char newvolume)
{
	if (opened && cursoundvolume != newvolume)
	{
		ESound_VolumeSound(driver, newvolume);
		cursoundvolume = newvolume;
	}
}

//Change music volume
void ClassESound::SetMusicVolume(unsigned char newvolume)
{
	if (opened && curmusicvolume != newvolume)
	{
		ESound_VolumeMusic(driver, newvolume);
		curmusicvolume = newvolume;
	}
}

//Play a sound effect
long ClassESound::PlaySound(unsigned long index, long playspeed, signed char pan, bool loopsound, unsigned long flags, unsigned long property)
{
	if (!opened)
		return(-1);
	if (suspended && !loopsound)
		return(-1);
	if (index < MAX_SOUNDEFFECTS)
		if (sound[index].object)
			if (!sound[index].lastplayed)
			{
				long playchannel = ESound_PlaySound(driver, sound[index].object, playspeed, pan, loopsound);
				if (playchannel >= 0)
				{
					sound[index].lastplayed = true;
					chnlsound[playchannel].playingobject = index;
					chnlsound[playchannel].loop = loopsound;
					chnlsound[playchannel].usefx = flags;
					chnlsound[playchannel].fxprop = property;
					if (flags & SFX_FADEIN)
					{
						chnlsound[playchannel].fxtimer = 0;
						ESound_ChannelSoundVolume(driver, sound[chnlsound[playchannel].playingobject].object, playchannel, 0);
					}
					else
						chnlsound[playchannel].fxtimer = property;
					return(playchannel);
				}
			}
	return(-1);
}

//Play music
long ClassESound::PlayMusic(unsigned long index, bool loopmusic, unsigned long flags, unsigned long property)
{
	if (!opened)
		return(-1);
	if (suspended && !loopmusic)
		return(-1);
	if (flags & SFX_CROSSFADE)
	{
		long chnlfound, ctr;
		if (MusicPlaying() >= 0)
		{
			for (ctr = 0; ctr < MAX_CHNLMUSIC; ctr++)
				if (chnlmusic[ctr].usefx & SFX_FADEOUT)
					StopMusicChannel(ctr);
			StopMusicAll(SFX_FADEOUT, property);
		}
		chnlfound = -1;
		for (ctr = 0; ctr < MAX_CHNLMUSIC; ctr++)
		{
			if (chnlmusic[ctr].playingobject < 0)
				chnlfound = ctr;
			else if (!chnlmusic[ctr].loop && chnlfound < 0)
				chnlfound = ctr;
		}
		if (chnlfound == -1)
			chnlfound = MAX_CHNLMUSIC - 1;
		StopMusicChannel(chnlfound);
		flags |= SFX_FADEIN;
	}
	else if (!(flags & SFX_MULTIMUSIC))
		StopMusicAll();
	if (index < MAX_MUSIC)
		if (music[index].object)
		{
			long playchannel = ESound_PlayMusic(driver, music[index].object, loopmusic);
			if (playchannel >= 0)
			{
				chnlmusic[playchannel].playingobject = index;
				chnlmusic[playchannel].loop = loopmusic;
				chnlmusic[playchannel].usefx = flags;
				chnlmusic[playchannel].fxprop = property;
				if (flags & SFX_FADEIN)
				{
					chnlmusic[playchannel].fxtimer = 0;
					ESound_ChannelMusicVolume(driver, music[chnlmusic[playchannel].playingobject].object, playchannel, 0);
				}
				else
					chnlmusic[playchannel].fxtimer = property;
				if (flags & SFX_CROSSFADE)
					chnlmusic[playchannel].fxtimer -= (long)(property / 8);
				return(playchannel);
			}
		}
	return(-1);
}

//Stop all sounds from index
void ClassESound::StopSound(unsigned long index, unsigned long flags, unsigned long property)
{
	if (!opened)
		return;
	unsigned long ctr;
	for (ctr = 0; ctr < MAX_CHNLSOUND; ctr++)
		if (chnlsound[ctr].playingobject == (long)index)
			StopSoundChannel(ctr, flags, property);
}

//Stop all music from index
void ClassESound::StopMusic(unsigned long index, unsigned long flags, unsigned long property)
{
	if (!opened)
		return;
	unsigned long ctr;
	for (ctr = 0; ctr < MAX_CHNLMUSIC; ctr++)
		if (chnlmusic[ctr].playingobject == (long)index)
			StopMusicChannel(ctr, flags, property);
}

//Stop all sound effects
void ClassESound::StopSoundAll(unsigned long flags, unsigned long property)
{
	unsigned long ctr;
	for (ctr = 0; ctr < MAX_CHNLSOUND; ctr++)
		StopSoundChannel(ctr, flags, property);
}

//Stop all music
void ClassESound::StopMusicAll(unsigned long flags, unsigned long property)
{
	unsigned long ctr;
	for (ctr = 0; ctr < MAX_CHNLMUSIC; ctr++)
		StopMusicChannel(ctr, flags, property);
}

//Stop a sound channel
void ClassESound::StopSoundChannel(long channel, unsigned long flags, unsigned long property)
{
	if (!opened)
		return;
	if (channel < 0 || channel >= MAX_CHNLSOUND)
		return;
	if (chnlsound[channel].playingobject < 0 || chnlsound[channel].playingobject >= MAX_SOUNDEFFECTS)
		return;
	if (sound[chnlsound[channel].playingobject].object == NULL)
		return;
	if ((flags & (SFX_FADEIN | SFX_FADEOUT)) == (SFX_FADEIN | SFX_FADEOUT))
		flags -= SFX_FADEIN | SFX_FADEOUT;
	if ((flags & SFX_FADEIN) || (flags & SFX_FADEOUT))
	{
		chnlsound[channel].usefx = flags & (SFX_FADEIN | SFX_FADEOUT);
		if (flags & SFX_FADEIN)
			chnlsound[channel].fxtimer = 0;
		else
			chnlsound[channel].fxtimer = property;
		chnlsound[channel].fxprop = property;
	}
	else
	{
		ESound_StopSound(driver, sound[chnlsound[channel].playingobject].object, channel);
		chnlsound[channel].playingobject = -1;
	}
}

//Stop a music channel
void ClassESound::StopMusicChannel(long channel, unsigned long flags, unsigned long property)
{
	if (!opened)
		return;
	if (channel < 0 || channel >= MAX_CHNLMUSIC)
		return;
	if (chnlmusic[channel].playingobject < 0 || chnlmusic[channel].playingobject >= MAX_MUSIC)
		return;
	if (music[chnlmusic[channel].playingobject].object == NULL)
		return;
	if ((flags & (SFX_FADEIN | SFX_FADEOUT)) == (SFX_FADEIN | SFX_FADEOUT))
		flags -= SFX_FADEIN | SFX_FADEOUT;
	if ((flags & SFX_FADEIN) || (flags & SFX_FADEOUT))
	{
		chnlmusic[channel].usefx = flags & (SFX_FADEIN | SFX_FADEOUT);
		if (flags & SFX_FADEIN)
			chnlmusic[channel].fxtimer = 0;
		else
			chnlmusic[channel].fxtimer = property;
		chnlmusic[channel].fxprop = property;
	}
	else
	{
		ESound_StopMusic(driver, music[chnlmusic[channel].playingobject].object, channel);
		chnlmusic[channel].playingobject = -1;
	}
}

//Return the current playing music
long ClassESound::MusicPlaying(void)
{
	if (!opened)
		return(-1);
	long foundindex = -1;
	unsigned long ctr, diff = 0xFFFFFFF;
	for (ctr = 0; ctr < MAX_CHNLMUSIC; ctr++)
		if (chnlmusic[ctr].playingobject >= 0 && chnlmusic[ctr].playingobject < MAX_MUSIC)
			if (!(chnlmusic[ctr].usefx & SFX_FADEOUT))
			{
				unsigned long value = 1024;
				if (chnlmusic[ctr].usefx & SFX_FADEIN)
					if (chnlmusic[ctr].fxprop >= 1)
						value = chnlmusic[ctr].fxtimer * 1024 / chnlmusic[ctr].fxprop;
				if (chnlmusic[ctr].loop)
					value += 16384;
				if (value < diff)
				{
					foundindex = ctr;
					diff = value;
				}
			}
	if (foundindex >= 0)
		return(chnlmusic[foundindex].playingobject);
	return(foundindex);
}

//Change mute state
void ClassESound::SetMute(bool state)
{
	if (!opened)
		return;
	if (state)
	{
		if (!muted)
		{
			Suspend();
			muted = true;
		}
	}
	else
	{
		if (muted)
		{
			muted = false;
			Resume();
		}
	}
}

//Update the sound system
void ClassESound::Update(unsigned long timedelta)
{
	if (opened)
	{
		unsigned long ctr;
		for (ctr = 0; ctr < MAX_SOUNDEFFECTS; ctr++)
			sound[ctr].lastplayed = false;
		for (ctr = 0; ctr < MAX_CHNLSOUND; ctr++)
			if (chnlsound[ctr].playingobject >= 0 && chnlsound[ctr].playingobject < MAX_SOUNDEFFECTS)
				if (sound[chnlsound[ctr].playingobject].object != NULL)
					if (chnlsound[ctr].usefx != SFX_NONE)
					{
						if (chnlsound[ctr].usefx & SFX_FADEIN)
						{
							chnlsound[ctr].fxtimer += timedelta;
							if (chnlsound[ctr].fxtimer >= (long)chnlsound[ctr].fxprop)
							{
								chnlsound[ctr].fxtimer = chnlsound[ctr].fxprop;
								chnlsound[ctr].usefx -= SFX_FADEIN;
							}
						}
						else if (chnlsound[ctr].usefx & SFX_FADEOUT)
						{
							if (chnlsound[ctr].fxtimer <= (long)timedelta)
							{
								chnlsound[ctr].fxtimer = 0;
								chnlsound[ctr].usefx -= SFX_FADEOUT;
								StopSoundChannel(ctr);
							}
							else
								chnlsound[ctr].fxtimer -= timedelta;
						}
						if (chnlsound[ctr].fxtimer > 0 && chnlsound[ctr].fxtimer < (long)chnlsound[ctr].fxprop)
							ESound_ChannelSoundVolume(driver, sound[chnlsound[ctr].playingobject].object, ctr, (unsigned char)(chnlsound[ctr].fxtimer * 255 / chnlsound[ctr].fxprop));
					}
		for (ctr = 0; ctr < MAX_CHNLMUSIC; ctr++)
			if (chnlmusic[ctr].playingobject >= 0 && chnlmusic[ctr].playingobject < MAX_MUSIC)
				if (music[chnlmusic[ctr].playingobject].object != NULL)
					if (chnlmusic[ctr].usefx != SFX_NONE)
					{
						if (chnlmusic[ctr].usefx & SFX_FADEIN)
						{
							chnlmusic[ctr].fxtimer += timedelta;
							if (chnlmusic[ctr].fxtimer >= (long)chnlmusic[ctr].fxprop)
							{
								chnlmusic[ctr].fxtimer = chnlmusic[ctr].fxprop;
								chnlmusic[ctr].usefx -= SFX_FADEIN;
							}
						}
						else if (chnlmusic[ctr].usefx & SFX_FADEOUT)
						{
							if (chnlmusic[ctr].fxtimer <= (long)timedelta)
							{
								chnlmusic[ctr].fxtimer = 0;
								chnlmusic[ctr].usefx -= SFX_FADEOUT;
								StopMusicChannel(ctr);
							}
							else
								chnlmusic[ctr].fxtimer -= timedelta;
						}
						if (chnlmusic[ctr].fxtimer > 0 && chnlmusic[ctr].fxtimer < (long)chnlmusic[ctr].fxprop)
							ESound_ChannelMusicVolume(driver, music[chnlmusic[ctr].playingobject].object, ctr, (unsigned char)(chnlmusic[ctr].fxtimer * 255 / chnlmusic[ctr].fxprop));
					}
		ESound_Update(driver, timedelta);
	}
}
