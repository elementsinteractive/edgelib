// antix.h
// Antix implementation for ClassESound
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Include the Antix audio library
#include <audio.h>


/////////////////////////////////////////////////////////////////////
// Sound driver structures
/////////////////////////////////////////////////////////////////////

//Sound driver
typedef struct
{
	ASContext ctx;
	ASLineControl *ctrl;
	ASLine *line;
	unsigned char *linevol;
	unsigned long numsoundchnl;
	unsigned long nummusicchnl;
	unsigned long cursoundchnl;
	unsigned long curmusicchnl;
	unsigned char cursfxvol;
	unsigned char curmusvol;
}ASDRIVER;


/////////////////////////////////////////////////////////////////////
// Open and close functions                                        //
/////////////////////////////////////////////////////////////////////

//Create a new sound driver
void *ESound_NewDriver(void)
{
	ASDRIVER *driver = new ASDRIVER;
	if (driver)
	{
		driver->ctx = NULL;
		driver->ctrl = NULL;
	}
	return(driver);
}

//Free the sound driver
void ESound_FreeDriver(void *driver)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	delete asdriver;
}

//Open the sound driver for sound output
ERESULT ESound_Open(void *driver, unsigned long frequency, unsigned char bitspersample, bool stereo, unsigned char chnlsound, unsigned char chnlmusic)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	asdriver->ctx = asCreateContext();
	if (asdriver->ctx)
	{
		int ctr, maxlines;
		maxlines = asGeti(asdriver->ctx, AS_MAX_LINES);
		if (chnlsound + chnlmusic > maxlines)
		{
			if (chnlmusic > 1)
				chnlmusic = 1;
			if (chnlsound + chnlmusic > maxlines)
				chnlsound = maxlines - chnlmusic;
		}
		asdriver->numsoundchnl = chnlsound;
		asdriver->nummusicchnl = chnlmusic;
		asdriver->cursoundchnl = 0;
		asdriver->curmusicchnl = 0;
		asdriver->cursfxvol = 64;
		asdriver->curmusvol = 64;
		asdriver->line = new ASLine[chnlsound + chnlmusic];
		if (asdriver->line == NULL)
		{
			asDestroyContext(asdriver->ctx);
			return(E_ERROR);
		}
		asdriver->ctrl = new ASLineControl[chnlsound + chnlmusic];
		if (asdriver->ctrl == NULL)
		{
			delete [] asdriver->line;
			asDestroyContext(asdriver->ctx);
			return(E_ERROR);
		}
		asdriver->linevol = new unsigned char[chnlsound + chnlmusic];
		if (asdriver->linevol == NULL)
		{
			delete [] asdriver->line;
			delete [] asdriver->ctrl;
			asDestroyContext(asdriver->ctx);
			return(E_ERROR);
		}
		for (ctr = 0; ctr < chnlsound + chnlmusic; ctr++)
		{
			asdriver->line[ctr] = asCreateLine(asdriver->ctx);
			asdriver->ctrl[ctr] = asCreateLineControl(asdriver->ctx);
			asdriver->linevol[ctr] = 255;
		}
		return(E_OK);
	}
	return(E_ERROR);
}

//Close the sound driver
void ESound_Close(void *driver)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	if (asdriver->ctx)
	{
		unsigned long ctr;
		if (asdriver->line)
		{
			for (ctr = 0; ctr < asdriver->numsoundchnl + asdriver->nummusicchnl; ctr++)
				if (asdriver->line[ctr])
				{
					asLineStop(asdriver->line[ctr]);
					asDestroyLine(asdriver->line[ctr]);
				}
			delete [] asdriver->line;
		}
		if (asdriver->ctrl)
		{
			for (ctr = 0; ctr < asdriver->numsoundchnl + asdriver->nummusicchnl; ctr++)
				if (asdriver->ctrl[ctr])
					asDestroyLineControl(asdriver->ctrl[ctr]);
			delete [] asdriver->ctrl;
		}
		if (asdriver->linevol)
			delete asdriver->linevol;
		asDestroyContext(asdriver->ctx);
	}
}

//Suspend the sound driver
void ESound_Suspend(void *driver)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	KDboolean pausesetting = KD_TRUE;
	asSeti(asdriver->ctx, AS_OUTPUT_PAUSE, pausesetting);	
}

//Resume the sound driver
void ESound_Resume(void *driver)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	KDboolean pausesetting = KD_FALSE;
	asSeti(asdriver->ctx, AS_OUTPUT_PAUSE, pausesetting);
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
	return(true);
}

//Return true if music requires an actual filename
bool ESound_MusicFromFile(void)
{
	return(true);
}

//Create a new sound effect object
void *ESound_NewSound(void *driver)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	if (asdriver->numsoundchnl == 0)
		return(NULL);
	return((void *)asCreateStream(asdriver->ctx));
}

//Load sound effect data
ERESULT ESound_LoadSound(void *driver, void *object, void *data, unsigned long datasize)
{
	return(E_UNSUPPORTED);
}

//Load sound effect file
ERESULT ESound_LoadSound(void *driver, void *object, const char *filename)
{
	ASError result;
	ASStream stream = (ASStream)object;
	asSeti(stream, AS_STREAM_LOW_LATENCY, KD_TRUE);
	result = asSetp(stream, AS_STREAM_FILENAME, filename);
	if (result != AS_OK)
		return(E_ERROR);
	return(E_OK);
}

//Free a sound effect object
void ESound_FreeSound(void *driver, void *object)
{
	ASStream stream = (ASStream)object;
	asDestroyStream(stream);
}

//Create a new music object
void *ESound_NewMusic(void *driver)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	if (asdriver->nummusicchnl == 0)
		return(NULL);
	return((void *)asCreateStream(asdriver->ctx));
}

//Load music data
ERESULT ESound_LoadMusic(void *driver, void *object, void *data, unsigned long datasize)
{
	return(E_UNSUPPORTED);
}

//Load music file
ERESULT ESound_LoadMusic(void *driver, void *object, const char *filename)
{
	ASError result;
	ASStream stream = (ASStream)object;
	asSeti(stream, AS_STREAM_LOW_LATENCY, KD_FALSE);
	result = asSetp(stream, AS_STREAM_FILENAME, filename);
	if (result != AS_OK)
		return(E_ERROR);
	return(E_OK);
}

//Free a music object
void ESound_FreeMusic(void *driver, void *object)
{
	ASStream stream = (ASStream)object;
	asDestroyStream(stream);
}

/////////////////////////////////////////////////////////////////////
// Playing sounds and music                                        //
/////////////////////////////////////////////////////////////////////

//Set sound effect volume
void ESound_VolumeSound(void *driver, unsigned char volume)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	unsigned long ctr;
	asdriver->cursfxvol = volume;
	for (ctr = 0; ctr < asdriver->numsoundchnl; ctr++)
	{
		asSeti(asdriver->ctrl[ctr], AS_CONTROL_VOLUME, volume * asdriver->linevol[ctr]);
		asLineSetControls(asdriver->line[ctr], asdriver->ctrl[ctr]);
		asSeti(asdriver->line[ctr], AS_CONTROL_VOLUME, volume * asdriver->linevol[ctr]);
	}
}

//Set music volume
void ESound_VolumeMusic(void *driver, unsigned char volume)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	unsigned long ctr;
	asdriver->curmusvol = volume;
	for (ctr = 0; ctr < asdriver->nummusicchnl; ctr++)
	{
		asSeti(asdriver->ctrl[asdriver->numsoundchnl + ctr], AS_CONTROL_VOLUME, volume * asdriver->linevol[asdriver->numsoundchnl + ctr]);
		asLineSetControls(asdriver->line[asdriver->numsoundchnl + ctr], asdriver->ctrl[asdriver->numsoundchnl + ctr]);
		asSeti(asdriver->line[asdriver->numsoundchnl + ctr], AS_CONTROL_VOLUME, volume * asdriver->linevol[asdriver->numsoundchnl + ctr]);
	}
}

//Play a sound effect
long ESound_PlaySound(void *driver, void *object, long playspeed, signed char pan, bool loopsound)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	ASStream stream = (ASStream)object;
	if (asdriver->numsoundchnl == 0)
		return(-1);
	long playchannel = asdriver->cursoundchnl;
	asdriver->cursoundchnl = (asdriver->cursoundchnl + 1) % asdriver->numsoundchnl;
	asLineStop(asdriver->line[playchannel]);
	asdriver->linevol[playchannel] = 255;
	asSeti(asdriver->line[playchannel], AS_CONTROL_VOLUME, asdriver->cursfxvol);
	if (loopsound)
		asSeti(asdriver->ctrl[playchannel], AS_CONTROL_LOOPS, 0);
	else
		asSeti(asdriver->ctrl[playchannel], AS_CONTROL_LOOPS, 1);
	asLinePlay(asdriver->line[playchannel], stream, asdriver->ctrl[playchannel]);
	return(playchannel);
}

//Play music
long ESound_PlayMusic(void *driver, void *object, bool loopmusic)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	ASStream stream = (ASStream)object;
	if (asdriver->nummusicchnl == 0)
		return(-1);
	long playchannel = asdriver->curmusicchnl;
	asdriver->curmusicchnl = (asdriver->curmusicchnl + 1) % asdriver->nummusicchnl;
	asLineStop(asdriver->line[asdriver->numsoundchnl + playchannel]);
	asdriver->linevol[asdriver->numsoundchnl + playchannel] = 255;
	asSeti(asdriver->line[asdriver->numsoundchnl + playchannel], AS_CONTROL_VOLUME, asdriver->curmusvol);
	if (loopmusic)
		asSeti(asdriver->ctrl[asdriver->numsoundchnl + playchannel], AS_CONTROL_LOOPS, 0);
	else
		asSeti(asdriver->ctrl[asdriver->numsoundchnl + playchannel], AS_CONTROL_LOOPS, 1);
	asLinePlay(asdriver->line[asdriver->numsoundchnl + playchannel], stream, asdriver->ctrl[asdriver->numsoundchnl + playchannel]);
	return(playchannel);
}

//Stop a sound effect
void ESound_StopSound(void *driver, void *object, long channel)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	if (channel >= 0)
		asLineStop(asdriver->line[channel]);
}

//Stop music
void ESound_StopMusic(void *driver, void *object, long channel)
{
	ASDRIVER *asdriver = (ASDRIVER *)driver;
	if (channel >= 0)
		asLineStop(asdriver->line[asdriver->numsoundchnl + channel]);
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
	if (channel >= 0)
	{
		ASDRIVER *asdriver = (ASDRIVER *)driver;
		asdriver->linevol[channel] = volume;
		asSeti(asdriver->ctrl[channel], AS_CONTROL_VOLUME, asdriver->cursfxvol * asdriver->linevol[channel]);
		asLineSetControls(asdriver->line[channel], asdriver->ctrl[channel]);
		asSeti(asdriver->line[channel], AS_CONTROL_VOLUME, asdriver->cursfxvol * asdriver->linevol[channel]);
	}
}

//Change the volume of a music channel
void ESound_ChannelMusicVolume(void *driver, void *object, long channel, unsigned char volume)
{
	if (channel >= 0)
	{
		ASDRIVER *asdriver = (ASDRIVER *)driver;
		asdriver->linevol[asdriver->numsoundchnl + channel] = volume;
		asSeti(asdriver->ctrl[asdriver->numsoundchnl + channel], AS_CONTROL_VOLUME, asdriver->curmusvol * asdriver->linevol[asdriver->numsoundchnl + channel]);
		asLineSetControls(asdriver->line[asdriver->numsoundchnl + channel], asdriver->ctrl[asdriver->numsoundchnl + channel]);
		asSeti(asdriver->line[asdriver->numsoundchnl + channel], AS_CONTROL_VOLUME, asdriver->curmusvol * asdriver->linevol[asdriver->numsoundchnl + channel]);
	}
}
