// silent.h
// Silent implementation for ClassESound
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Open and close functions                                        //
/////////////////////////////////////////////////////////////////////

//Create a new sound driver
void *ESound_NewDriver(void)
{
	return((void *)1);
}

//Free the sound driver
void ESound_FreeDriver(void *driver)
{
}

//Open the sound driver for sound output
long ESound_Open(void *driver, unsigned long frequency, unsigned char bitspersample, bool stereo, unsigned char chnlsound, unsigned char chnlmusic)
{
	return(E_OK);
}

//Close the sound driver
void ESound_Close(void *driver)
{
}

//Suspend the sound driver
void ESound_Suspend(void *driver)
{
}

//Resume the sound driver
void ESound_Resume(void *driver)
{
}

/////////////////////////////////////////////////////////////////////
// Sound and music load functions                                  //
/////////////////////////////////////////////////////////////////////

//Return true if ClassESound should preserve sound and music file pointers
bool ESound_KeepFilePointers(void)
{
	return(false);
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
	return((void *)1);
}

//Load sound effect data
ERESULT ESound_LoadSound(void *driver, void *object, void *data, unsigned long datasize)
{
	return(E_OK);
}

//Load sound effect file
ERESULT ESound_LoadSound(void *driver, void *object, const char *filename)
{
	return(E_OK);
}

//Free a sound effect object
void ESound_FreeSound(void *driver, void *object)
{
}

//Create a new music object
void *ESound_NewMusic(void *driver)
{
	return((void *)1);
}

//Load music data
ERESULT ESound_LoadMusic(void *driver, void *object, void *data, unsigned long datasize)
{
	return(E_OK);
}

//Load music file
ERESULT ESound_LoadMusic(void *driver, void *object, const char *filename)
{
	return(E_OK);
}

//Free a music object
void ESound_FreeMusic(void *driver, void *object)
{
}

/////////////////////////////////////////////////////////////////////
// Playing sounds and music                                        //
/////////////////////////////////////////////////////////////////////

//Set sound effect volume
void ESound_VolumeSound(void *driver, unsigned char volume)
{
}

//Set music volume
void ESound_VolumeMusic(void *driver, unsigned char volume)
{
}

//Play a sound effect
long ESound_PlaySound(void *driver, void *object, long playspeed, signed char pan, bool loopsound)
{
	return(0);
}

//Play music
long ESound_PlayMusic(void *driver, void *object, bool loopmusic)
{
	return(0);
}

//Stop a sound effect
void ESound_StopSound(void *driver, void *object, long channel)
{
}

//Stop music
void ESound_StopMusic(void *driver, void *object, long channel)
{
}

//Update the sound system
void ESound_Update(void *driver, unsigned long timedelta)
{
}

/////////////////////////////////////////////////////////////////////
// Channels (advanced)                                             //
/////////////////////////////////////////////////////////////////////

//Change the volume of a sound channel
void ESound_ChannelSoundVolume(void *driver, void *object, long channel, unsigned char volume)
{
}

//Change the volume of a music channel
void ESound_ChannelMusicVolume(void *driver, void *object, long channel, unsigned char volume)
{
}
