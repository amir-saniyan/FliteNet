#include "Stdafx.h"

#include <stdexcept>
#include <Windows.h>

#include "WavePlayer.h"

using namespace std;

WavePlayer::WavePlayer(int samplingRate, int channelsCount, int bitsPerSample)
{
	if (samplingRate <= 0)
	{
		throw invalid_argument("Invalid argument: samplingRate.");
	}
	if (channelsCount <= 0)
	{
		throw invalid_argument("Invalid argument: channelsCount.");
	}
	if ((bitsPerSample % 8) != 0)
	{
		throw invalid_argument("Invalid argument: bitsPerSample.");
	}

	handle = NULL;
	this->samplingRate = samplingRate;
	this->channelsCount = channelsCount;
	this->bitsPerSample = bitsPerSample;
}

WavePlayer::~WavePlayer(void)
{
	if (IsOpen())
	{
		Close();
	}
}

int WavePlayer::GetSamplingRate(void) const
{
	return samplingRate;
}

int WavePlayer::GetChannelsCount(void) const
{
	return channelsCount;
}

int WavePlayer::GetBitsPerSample(void) const
{
	return bitsPerSample;
}

bool WavePlayer::IsOpen(void) const
{
	if (handle != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void WavePlayer::Open()
{
	if (IsOpen())
	{
		throw logic_error("Object already is open.");
	}

	WAVEFORMATEX wfx;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
	wfx.nSamplesPerSec = samplingRate;
	wfx.wBitsPerSample = bitsPerSample;
	wfx.nChannels = channelsCount;
	wfx.cbSize = 0;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nBlockAlign = (wfx.wBitsPerSample >> 3) * wfx.nChannels;
	wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

	HWAVEOUT hWaveOut;
	if
	(
		waveOutOpen
		(
			&hWaveOut,
			WAVE_MAPPER,
			&wfx,
			0,
			0,
			CALLBACK_NULL
		)
		!= MMSYSERR_NOERROR
	)
	{
		throw runtime_error("waveOutOpen() failed.");
	}

	handle = (void*)hWaveOut;

	return;
}

void WavePlayer::Close()
{
	if (IsOpen())
	{
		HWAVEOUT hWaveOut = (HWAVEOUT)handle;

		waveOutClose(hWaveOut);
		hWaveOut = NULL;
		handle = NULL;
	}

	return;
}

void WavePlayer::PlayBuffer(unsigned char* buffer, int bufferLength)
{
	if (bufferLength < 0)
	{
		throw invalid_argument("Invalid argument: bufferLength.");
	}
	if ((buffer == NULL) && (bufferLength != 0))
	{
		throw invalid_argument("Invalid argument: bufferLength.");
	}

	if (!IsOpen())
	{
		throw logic_error("Object is not open.");
	}

	HWAVEOUT hWaveOut = (HWAVEOUT)handle;

	unsigned char* buff = new unsigned char[bufferLength];
	memcpy(buff, buffer, bufferLength);

	WAVEHDR header;
	ZeroMemory(&header, sizeof(WAVEHDR));
	header.dwBufferLength = bufferLength;
	header.lpData = (LPSTR)buff;
	waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));

	while
	(
		waveOutUnprepareHeader
		(
			hWaveOut,
			&header,
			sizeof(WAVEHDR)
		)
		== WAVERR_STILLPLAYING
	)
	{
		Sleep(1);
	}

	delete[] buff;
	buff = NULL;

	return;
}
