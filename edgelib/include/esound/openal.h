// openal.h
// OpenAL implementation for ClassESound
//
// Copyright (c) 2004-2017 Elements Interactive B.V.
/////////////////////////////////////////////////////////////////////

//Include OpenAL
#if defined(DEVICE_MAC)
	#include <OpenAL/al.h>
	#include <OpenAL/alc.h>
#else
	#include "al.h"
	#include "alc.h"
#endif

#define OPENAL_MUSIC_BUFFERS			3
#define OPENAL_MUSIC_BUFFER_SIZE	 4096

//Sound information (header) structure for wav
typedef struct
{
	unsigned long fileSize;
	unsigned long subChuck1Size;
	unsigned long remainingSize;
	unsigned long mFreq;
	unsigned short audioFormat;
	unsigned short channels;
	unsigned short bps;
	long format;
	long bufferSize;
	long startDataPos;
}SOUND_INFO;

//Sound effect object
typedef struct
{
	ALuint buffer;
	ALuint source;
}SOUND_EFFECT;

//Music object
//typedef struct
//{
//	ALuint buffers[OPENAL_MUSIC_BUFFERS];
//	ALuint source;
//	SOUND_INFO info;
//	unsigned long startPos;
//	unsigned long currentPos;
//	unsigned long dataSize;
//	void *data;
//}SOUND_MUSIC;


/////////////////////////////////////////////////////////////////////
// Load helper functions                                           //
/////////////////////////////////////////////////////////////////////

//Load raw sound buffer
void LoadBufferMemory(SOUND_EFFECT *effect, SOUND_INFO *info, void *memory, unsigned long memorysize)
{
	char *dataBuf = (char *)ClassEMemory::Alloc(info->remainingSize);
	ClassEMemory::Set(dataBuf, 0, info->remainingSize);
	ClassEMemory::Copy(dataBuf, (unsigned char *)memory + info->startDataPos, info->remainingSize);
	alBufferData(effect->buffer, info->format, dataBuf, info->remainingSize, info->mFreq);
	ClassEMemory::DeAlloc(dataBuf);
}

//Load sound info for wave files (header)
void LoadSoundInfo(SOUND_INFO *info, void *memory, unsigned long memorysize)
{
	unsigned char *data = (unsigned char *)memory;
	char buf[256];
	long start = 0;
	size_t r;

	// check magic
	r = 1 * 4;
	ClassEMemory::Copy(buf, data + start, r);
	start += r;
	buf[4] = 0;
	if (!ClassEStd::StrEqual("RIFF", buf))
		return;

	// The next 4 bytes are the file size, we can skip this since we get the size from the DataStream
	r = 1 * 4;
	ClassEMemory::Copy(buf, data + start, r);
	start += r;
	info->fileSize = *(int *)buf;
	
	// check file format
	r = 1 * 4;
	ClassEMemory::Copy(buf, data + start, r);
	start += r;
	buf[4] = 0;
	if (!ClassEStd::StrEqual("WAVE", buf))
		return;

	// check 'fmt ' sub chunk (1)
	r = 1 * 4;
	ClassEMemory::Copy(buf, data + start, r);
	start += r;
	buf[4] = 0;
	if (!ClassEStd::StrEqual("fmt ", buf))
		return;

	// read (1)'s size
	r = 1 * 4;
	ClassEMemory::Copy(buf, data + start, r);
	start += r;
	info->subChuck1Size = *(unsigned long *)buf;

	// check PCM audio format
	r = 1 * 2;
	ClassEMemory::Copy(buf, data + start, r);
	start += r;
	info->audioFormat = *(unsigned short *)buf;

	// read number of channels
	r = 1 * 2;
	ClassEMemory::Copy(buf, data + start, r);
	start += r;
	info->channels = *(unsigned short *)buf;

	// read frequency (sample rate)
	r = 1 * 4;
	ClassEMemory::Copy(buf, data + start, r);
	start += r;
	info->mFreq = *(unsigned long *)buf;

	// skip 6 bytes (Byte rate (4), Block align (2))
	r = 1 * 6;
	start += r;
	
	// read bits per sample
	r = 1 * 2;
	ClassEMemory::Copy(buf, data + start, r);
	start += r;
	info->bps = *(unsigned short *)buf;
	
	if (info->channels == 1)
	{
		if (info->bps == 8)
		{
			info->format = AL_FORMAT_MONO8;
			info->bufferSize = info->mFreq / 4; //Set BufferSize to 250ms (Frequency divided by 4 (quarter of a second))
		}
		else
		{
			info->format = AL_FORMAT_MONO16;
			info->bufferSize = info->mFreq >> 1; //Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
			info->bufferSize -= (info->bufferSize % 2); // IMPORTANT: The Buffer Size must be an exact multiple of the BlockAlignment ...
		}
	}
	else
	{
		if (info->bps == 8)
		{
			info->format = AL_FORMAT_STEREO16;
			info->bufferSize = info->mFreq >> 1; //Set BufferSize to 250ms (Frequency * 2 (8bit stereo) divided by 4 (quarter of a second))
			info->bufferSize -= (info->bufferSize % 2); //IMPORTANT: The Buffer Size must be an exact multiple of the BlockAlignment ...
		}
		else
		{
			info->format = AL_FORMAT_STEREO16;
			info->bufferSize = info->mFreq; //Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
			info->bufferSize -= (info->bufferSize % 4); //IMPORTANT: The Buffer Size must be an exact multiple of the BlockAlignment ...
		}
	}
	
	// check 'data' sub chunk (2)
	r = 1 * 4;
	start += 2;
	ClassEMemory::Copy(buf, data + start, r);
	start += r;
	buf[4] = 0;
	if (!ClassEStd::StrEqual("data", buf) && !ClassEStd::StrEqual(buf, "fact"))
	{
		start -= r;
		r = 1 * 4;
		start -= 2;
		ClassEMemory::Copy(buf, data + start, r);
		start += r;
		buf[4] = 0;
		if ((!ClassEStd::StrEqual("data", buf)) && (!ClassEStd::StrEqual("fact", buf)))
			return;
	}
	
	// fact is an option section we don't need to worry about
	if (ClassEStd::StrEqual(buf, "fact"))
	{
		// skip
		r = 1 * 8;
		start += r;

		// Now we shoudl hit the data chunk
		r = 1 * 4;
		ClassEMemory::Copy(buf, data + start, r);
		start += r;
		buf[4] = 0;
		if (!ClassEStd::StrEqual("data", buf))
			return;
	}
	
	// The next four bytes are the size remaing of the file
	r = 1 * 4;
	ClassEMemory::Copy(buf, data + start, r);
	info->startDataPos = start + r;
	info->remainingSize = *(unsigned long *)buf;
}

//To be implemented
//void ESound_LoadMusicBuffers(SOUND_MUSIC *music)
//{
//}


/////////////////////////////////////////////////////////////////////
// Open and close functions                                        //
/////////////////////////////////////////////////////////////////////

//Create a new sound driver
void *ESound_NewDriver(void)
{
	return(alcOpenDevice(NULL));
}

//Free the sound driver
void ESound_FreeDriver(void *driver)
{
	alcCloseDevice((ALCdevice *)driver);
}

//Open the sound driver for sound output
long ESound_Open(void *driver, unsigned long frequency, unsigned char bitspersample, bool stereo, unsigned char chnlsound, unsigned char chnlmusic)
{
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
	ALCcontext *ctx = alcCreateContext((ALCdevice *)driver, NULL);
	if (ctx)
	{
		alcMakeContextCurrent(ctx);
		alListenerfv(AL_POSITION,    ListenerPos);
		alListenerfv(AL_VELOCITY,    ListenerVel);
		alListenerfv(AL_ORIENTATION, ListenerOri);
		return(E_OK);
	}
	return(E_ERROR);
}

//Close the sound driver
void ESound_Close(void *driver)
{
	ALCcontext *ctx = alcGetCurrentContext();
	alcMakeContextCurrent(NULL);
	alcDestroyContext(ctx);
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
	SOUND_EFFECT *effect = new SOUND_EFFECT;
	alGenBuffers(1, &effect->buffer);
	alGenSources(1, &effect->source);
	return(effect);
}

//Load sound effect data
ERESULT ESound_LoadSound(void *driver, void *object, void *data, unsigned long datasize)
{
	SOUND_INFO info;
	SOUND_EFFECT *effect = (SOUND_EFFECT *)object;
	LoadSoundInfo(&info, data, datasize);
	LoadBufferMemory(effect, &info, data, datasize);
	
	// initialize the source
	static ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
	static ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
	static ALboolean loop = false;
	alSourcei (effect->source, AL_BUFFER, effect->buffer);
	alSourcef (effect->source, AL_PITCH, 1.0);
	alSourcef (effect->source, AL_GAIN, 1.0);
	alSourcefv(effect->source, AL_POSITION, SourcePos);
	alSourcefv(effect->source, AL_VELOCITY, SourceVel);
	alSourcei (effect->source, AL_LOOPING,  loop     );
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
	SOUND_EFFECT *effect = (SOUND_EFFECT *)object;
	alDeleteBuffers(1, &effect->buffer);
	alDeleteSources(1, &effect->source);
	delete effect;
}

//Create a new music object
void *ESound_NewMusic(void *driver)
{
	//SOUND_MUSIC *music = new SOUND_MUSIC;
	//alGenBuffers(OPENAL_MUSIC_BUFFERS, music->buffers);
	//alGenSources(1, &music->source);
	//return(music);
	return(NULL);
}

//Load music data
ERESULT ESound_LoadMusic(void *driver, void *object, void *data, unsigned long datasize)
{
	//SOUND_MUSIC *music = (SOUND_MUSIC *)object;
	//LoadSoundInfo(&music->info, data, datasize);
	//return(E_OK);
	return(E_UNSUPPORTED);
}

//Load music file
ERESULT ESound_LoadMusic(void *driver, void *object, const char *filename)
{
	return(E_UNSUPPORTED);
}

//Free a music object
void ESound_FreeMusic(void *driver, void *object)
{
	//SOUND_MUSIC *music = (SOUND_MUSIC *)object;
	//alDeleteBuffers(OPENAL_MUSIC_BUFFERS, music->buffers);
	//alDeleteSources(1, &music->source);
}


/////////////////////////////////////////////////////////////////////
// Playing sounds and music                                        //
/////////////////////////////////////////////////////////////////////

//Set sound effect volume
void ESound_VolumeSound(void *driver, unsigned char volume)
{
	alListenerf(AL_GAIN, (float)volume / 255);
}

//Set music volume
void ESound_VolumeMusic(void *driver, unsigned char volume)
{
	alListenerf(AL_GAIN, (float)volume / 255);
}

//Play a sound effect
long ESound_PlaySound(void *driver, void *object, long playspeed, signed char pan, bool loopsound)
{
	SOUND_EFFECT *effect = (SOUND_EFFECT *)object;
	if (loopsound)
		alSourcei(effect->source, AL_LOOPING, AL_TRUE);
	alSourcePlay(effect->source);
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
	SOUND_EFFECT *effect = (SOUND_EFFECT *)object;
	alSourceStop(effect->source);
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
