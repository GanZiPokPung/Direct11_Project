#pragma once
class Sound
{
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

public:
	Sound();
	~Sound();

public:
	bool Initialize(HWND hWnd);
	void Shutdown();

private:
	bool InitializeDirectSound(HWND hWnd);
	void ShutdownDirectSound();

	bool LoadWaveFile(char* path, IDirectSoundBuffer8** secondaryBuffer);
	void ShutdownWaveFile(IDirectSoundBuffer8** secondaryBuffer);

	bool PlayWaveFile();

private:
	IDirectSound8* m_DirectSound = nullptr;
	IDirectSoundBuffer* m_primaryBuffer = nullptr;
	IDirectSoundBuffer8* m_secondaryBuffer1 = nullptr;
};

