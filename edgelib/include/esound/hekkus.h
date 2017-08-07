// hekkus.h
// Hekkus implementation for ClassESound
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Include and link Hekkus Sound system
#pragma comment(lib, "hss.lib")
#include "HSS.h"

#if defined(DEVICE_DESKTOP) && defined(DEVICE_WIN32)
	#pragma comment(lib, "winmm.lib")
#endif

/////////////////////////////////////////////////////////////////////
// Open and close functions                                        //
/////////////////////////////////////////////////////////////////////

//Create a new sound driver
void *ESound_NewDriver(void)
{
	hssSpeaker *speaker = new hssSpeaker;
	return(speaker);
}

//Free the sound driver
void ESound_FreeDriver(void *driver)
{
	hssSpeaker *speaker = (hssSpeaker *)driver;
	delete speaker;
}

//Open the sound driver for sound output
ERESULT ESound_Open(void *driver, unsigned long frequency, unsigned char bitspersample, bool stereo, unsigned char chnlsound, unsigned char chnlmusic)
{
	hssSpeaker *speaker = (hssSpeaker *)driver;
	if (speaker->open(frequency, bitspersample, stereo, chnlmusic, chnlsound) != HSS_OK)
		return(E_ERROR);
	return(E_OK);
}

//Close the sound driver
void ESound_Close(void *driver)
{
	hssSpeaker *speaker = (hssSpeaker *)driver;
	speaker->close();
}

//Suspend the sound driver
void ESound_Suspend(void *driver)
{
	hssSpeaker *speaker = (hssSpeaker *)driver;
	speaker->suspend();
}

//Resume the sound driver
void ESound_Resume(void *driver)
{
	hssSpeaker *speaker = (hssSpeaker *)driver;
	speaker->resume();
}

/////////////////////////////////////////////////////////////////////
// Sound and music load functions                                  //
/////////////////////////////////////////////////////////////////////

//Return true if ClassESound should preserve sound and music file pointers
bool ESound_KeepFilePointers(void)
{
	return(true);
}

//Return true if sound effects require an actual filename
bool ESound_SoundFromFile(void)
{
	return(false);
}

//Return true if music requires an actual filename
bool ESound_MusicFromFile(void)
{
	return(false);
}

//Create a new sound effect object
void *ESound_NewSound(void *driver)
{
	hssSound *sound = new hssSound;
	return(sound);
}

//Load sound effect data
ERESULT ESound_LoadSound(void *driver, void *object, void *data, unsigned long datasize)
{
	hssSound *sound = (hssSound *)object;
	if (sound->load(data, false) != HSS_OK)
		return(E_ERROR);
	return(E_OK);
}

//Load sound effect file
ERESULT ESound_LoadSound(void *driver, void *object, const char *filename)
{
	return(E_UNSUPPORTED);
}

//Free a sound effect object
void ESound_FreeSound(void *driver, void *object)
{
	hssSound *sound = (hssSound *)object;
	delete sound;
}

//Create a new music object
void *ESound_NewMusic(void *driver)
{
	hssMusic *music = new hssMusic;
	return(music);
}

//Load music data
ERESULT ESound_LoadMusic(void *driver, void *object, void *data, unsigned long datasize)
{
	hssMusic *music = (hssMusic *)object;
	if (music->load(data, false) != HSS_OK)
		return(E_ERROR);
	return(E_OK);
}

//Load music file
ERESULT ESound_LoadMusic(void *driver, void *object, const char *filename)
{
	return(E_UNSUPPORTED);
}

//Free a music object
void ESound_FreeMusic(void *driver, void *object)
{
	hssMusic *music = (hssMusic *)object;
	delete music;
}

/////////////////////////////////////////////////////////////////////
// Playing sounds and music                                        //
/////////////////////////////////////////////////////////////////////

//Set sound effect volume
void ESound_VolumeSound(void *driver, unsigned char volume)
{
	hssSpeaker *speaker = (hssSpeaker *)driver;
	speaker->volumeSounds(volume * 64 / 255);
}

//Set music volume
void ESound_VolumeMusic(void *driver, unsigned char volume)
{
	hssSpeaker *speaker = (hssSpeaker *)driver;
	speaker->volumeMusics(volume * 64 / 255);
}

//Play a sound effect
long ESound_PlaySound(void *driver, void *object, long playspeed, signed char pan, bool loopsound)
{
	long channel;
	hssSound *sound = (hssSound *)object;
	hssSpeaker *speaker = (hssSpeaker *)driver;
	sound->loop(loopsound);
	channel = speaker->playSound(sound);
	if (channel >= 0)
	{
		if (pan != 0)
			speaker->channel(channel)->pan(pan + 128);
		if (playspeed != 0)
			speaker->channel(channel)->speed(playspeed + 32768);
	}
	return(channel);
}

//Play music
long ESound_PlayMusic(void *driver, void *object, bool loopmusic)
{
	long channel;
	hssMusic *music = (hssMusic *)object;
	hssSpeaker *speaker = (hssSpeaker *)driver;
	music->loop(loopmusic);
	channel = speaker->playMusic(music);
	return(channel);
}

//Stop a sound effect
void ESound_StopSound(void *driver, void *object, long channel)
{
	hssSpeaker *speaker = (hssSpeaker *)driver;
	if (channel >= 0)
		speaker->channel(channel)->stop();
}

//Stop music
void ESound_StopMusic(void *driver, void *object, long channel)
{
	hssSpeaker *speaker = (hssSpeaker *)driver;
	if (channel >= 0)
		speaker->channel(channel)->stop();
}

//Update the sound system (unused)
void ESound_Update(void *driver, unsigned long timedelta)
{
}

/////////////////////////////////////////////////////////////////////
// Channels (advanced)                                             //
/////////////////////////////////////////////////////////////////////

//Change the volume of a sound channel
void ESound_ChannelSoundVolume(void *driver, void *object, long channel, unsigned char volume)
{
	hssSpeaker *speaker = (hssSpeaker *)driver;
	if (channel >= 0)
		speaker->channel(channel)->volume(volume * 64 / 255);
}

//Change the volume of a music channel
void ESound_ChannelMusicVolume(void *driver, void *object, long channel, unsigned char volume)
{
	hssSpeaker *speaker = (hssSpeaker *)driver;
	if (channel >= 0)
		speaker->channel(channel)->volume(volume * 64 / 255);
}
