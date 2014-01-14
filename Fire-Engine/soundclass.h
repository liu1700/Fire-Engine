
//////////////////////////////////////////////////////////
//�ļ���: soundclass.h		����: ������:2014/1/14
//////////////////////////////////////////////////////////
#ifndef _SOUNDCLASS_H_
#define _SOUNDCLASS_H_

/////////
//LINKING //
/////////
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")

/////////
//INCLUDES //
/////////
#include <Windows.h>
#include <MMSystem.h>
#include <dsound.h>
#include <stdio.h>

////////////////////////////////////
//����: SoundClass
////////////////////////////////////
class SoundClass
{
public:
	SoundClass();
	SoundClass(const SoundClass&);
	~SoundClass();

	bool Initialize(HWND);
	void ShutDown();

private:
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

private:
	bool InitializeDirectSound(HWND);
	void ShutdownDirectSound();

	bool LoadWaveFile(char*, IDirectSoundBuffer8**);
	void ShutdownWaveFile(IDirectSoundBuffer8**);

	bool PlayWaveFile();

private:
	IDirectSound8* m_DirectSound;
	IDirectSoundBuffer* m_primaryBuffer;

	IDirectSoundBuffer8* m_secondaryBuffer1;
	IDirectSoundBuffer8* m_secondaryBuffer2;
};

#endif