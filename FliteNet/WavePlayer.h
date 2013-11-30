#pragma once

class WavePlayer
{
public:

	WavePlayer(int samplingRate, int channelsCount, int bitsPerSample);

	~WavePlayer(void);

	int GetSamplingRate(void) const;

	int GetChannelsCount(void) const;

	int GetBitsPerSample(void) const;

	bool IsOpen(void) const;

	void Open(void);

	void Close(void);

	void PlayBuffer(unsigned char* buffer, int bufferLength);

private:

	void* handle;
	int samplingRate;
	int channelsCount;
	int bitsPerSample;
};
