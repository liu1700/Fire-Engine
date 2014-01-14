
//////////////////////////////////////////////////////////
//�ļ���: soundclass.cpp		����: ������:2014/1/14
//////////////////////////////////////////////////////////
#include "soundclass.h"

SoundClass::SoundClass()
{
	m_DirectSound = NULL;
	m_primaryBuffer = NULL;
	m_secondaryBuffer1 = NULL;
}

SoundClass::SoundClass(const SoundClass& other)
{

}

SoundClass::~SoundClass()
{

}

bool SoundClass::Initialize(HWND hwnd)
{
	// ��ʼ��DSound������������
	if(!InitializeDirectSound(hwnd))
		return false;

	// ���ز����������ڶ�����
	if(!LoadWaveFile("../Fire-Engine/Sounds/songbird1.WAV", &m_secondaryBuffer1))
		return false;

	// ����֮�����̲���
	if(!PlayWaveFile())
		return false;

	return true;
}

void SoundClass::ShutDown()
{
	ShutdownWaveFile(&m_secondaryBuffer1);

	ShutdownDirectSound();

	return;
}

bool SoundClass::InitializeDirectSound(HWND hwnd)
{
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	// ��ʼ��Dsound�ӿ�
	if(FAILED(DirectSoundCreate8(NULL, &m_DirectSound, NULL)))
		return false;

	// �趨��������
	if(FAILED(m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY)))
		return false;

	// ���������������
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// ����������
	if(FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL)))
		return false;

	// ������Ƶ��ʽ�Լ���ϸ��Ϣ
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// ����������ĸ�ʽ
	if(FAILED(m_primaryBuffer->SetFormat(&waveFormat)))
		return false;

	return true;
}

void SoundClass::ShutdownDirectSound()
{
	if(m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	if(m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}

	return;
}

bool SoundClass::LoadWaveFile(char* filename, IDirectSoundBuffer8** secondaryBuffer)
{
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	IDirectSoundBuffer* tempBuffer;
	unsigned char* waveData;
	unsigned char *bufferPtr;
	unsigned long bufferSize;

	// �Զ�������ʽ���ļ�
	if(fopen_s(&filePtr, filename, "rb") != 0)
		return false;

	// ��ʼ��ȡ�ļ�
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if(count != 1)
	{
		return false;
	}

	// ����ʽ
	if((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') || 
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	// ����Ƿ�Ϊ�����ļ�
	if((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	// ����Ƿ�Ϊfmt ��ʽ
	if((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	// �����Ƶ��ʽ�Ƿ�ΪWAVE_FORMAT_PCM.
	if(waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// �����Ƶ�Ƿ���stereo��ʽ��¼
	if(waveFileHeader.numChannels != 1)
	{
		return false;
	}

	// ���Hz
	if(waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	// ȷ��Ϊ16λ
	if(waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	// �������ͷ
	if((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}

	// �趨�ڶ�����������Ƶ��ʽ
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// ���õڶ���������������Ϣ
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// ������ʱ��������
	if(FAILED(m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
		return false;

	// ��黺���ʽ�������ڶ�������
	if(FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&*secondaryBuffer)))
		return false;

	// �ͷ���ʱ����
	tempBuffer->Release();
	tempBuffer = NULL;

	// �ƶ��������ļ�ͷ
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// ������ʱ���汣�������ļ�
	waveData = new unsigned char[waveFileHeader.dataSize];
	if(!waveData)
		return false;

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if(count != waveFileHeader.dataSize)
		return false;

	// �����ر��ļ�
	if(fclose(filePtr) != 0)
		return false;

	// �����ڶ�����ȥ����д��
	if(FAILED((*secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
		return false;

	// �������ļ��������ڴ���
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// �ļ�д������
	if(FAILED((*secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
		return false;

	// �ͷ��ļ�
	delete [] waveData;
	waveData = NULL;

	return true;
}

void SoundClass::ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer)
{
	if(*secondaryBuffer)
	{
		(*secondaryBuffer)->Release();
		*secondaryBuffer = 0;
	}

	return;
}

bool SoundClass::PlayWaveFile()
{

	// ����������ͷ����λ��
	if(FAILED(m_secondaryBuffer1->SetCurrentPosition(0)))
		return false;

	// �趨����
	if(FAILED(m_secondaryBuffer1->SetVolume(DSBVOLUME_MAX)))
		return false;

	// ���ŵڶ����������
	if(FAILED(m_secondaryBuffer1->Play(0, 0, 0)))
		return false;

	return true;
}