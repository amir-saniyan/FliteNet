#pragma once

#include <string>

using namespace std;
using namespace System;

namespace FliteNet
{
	public ref class FliteWave
	{
	public:

		property int SamplingRate
		{
			int get(void)
			{
				return cst_wave_sample_rate(wave);
			}
		}

		property int ChannelsCount
		{
			int get(void)
			{
				return cst_wave_num_channels(wave);
			}
		}

		property int BitsPerSample
		{
			int get(void)
			{
				return (sizeof(short) * 8);
			}
		}

		property int SamplesCount
		{
			int get(void)
			{
				return cst_wave_num_samples(wave);
			}
		}

		property short default[int]
		{
			short get(int index)
			{
				if (index < 0 || index >= SamplesCount)
				{
					throw gcnew ArgumentOutOfRangeException(L"index");
				}
				return cst_wave_samples(wave)[index];
			}

			void set(int index, short value)
			{
				if (index < 0 || index >= SamplesCount)
				{
					throw gcnew ArgumentOutOfRangeException(L"index");
				}
				cst_wave_samples(wave)[index] = value;
				return;
			}
		}

		property double Length // In seconds.
		{
			double get(void)
			{
				return ((double)SamplesCount / (double)SamplingRate);
			}
		}

		FliteWave(int samplingRate)
		{
			if (samplingRate <= 0)
			{
				throw gcnew ArgumentOutOfRangeException(L"samplingRate");
			}

			wave = new_wave();
			if (wave == NULL)
			{
				throw gcnew ApplicationException(L"new_wave() failed.");
			}

			cst_wave_set_sample_rate(wave, samplingRate);
			cst_wave_set_num_channels(wave, 1);

			return;
		}

		FliteWave(int samplingRate, int channelsCount)
		{
			if (samplingRate <= 0)
			{
				throw gcnew ArgumentOutOfRangeException(L"samplingRate");
			}
			if (channelsCount < 1)
			{
				throw gcnew ArgumentOutOfRangeException(L"channelsCount");
			}

			if (channelsCount != 1)
			{
				throw gcnew ArgumentException(L"Supported value for channelsCount is 1.", L"channelsCount");
			}

			wave = new_wave();
			if (wave == NULL)
			{
				throw gcnew ApplicationException(L"new_wave() failed.");
			}

			cst_wave_set_sample_rate(wave, samplingRate);
			cst_wave_set_num_channels(wave, channelsCount);

			return;
		}

		FliteWave(int samplingRate, int channelsCount, int bitsPerSample)
		{
			if (samplingRate <= 0)
			{
				throw gcnew ArgumentOutOfRangeException(L"samplingRate");
			}
			if (channelsCount < 1)
			{
				throw gcnew ArgumentOutOfRangeException(L"channelsCount");
			}
			if (bitsPerSample <= 0)
			{
				throw gcnew ArgumentOutOfRangeException(L"bitsPerSample");
			}

			if (channelsCount != 1)
			{
				throw gcnew ArgumentException(L"Supported value for channelsCount is 1.", L"channelsCount");
			}
			if (bitsPerSample != 16)
			{
				throw gcnew ArgumentException(L"Supported value for bitsPerSample is 16.", L"channelsCount");
			}

			wave = new_wave();
			if (wave == NULL)
			{
				throw gcnew ApplicationException(L"new_wave() failed.");
			}

			cst_wave_set_sample_rate(wave, samplingRate);
			cst_wave_set_num_channels(wave, channelsCount);

			return;
		}

		~FliteWave(void)
		{
			if (wave != NULL)
			{
				delete_wave(wave);
				wave = NULL;
			}
		}

		array<short>^ GetSamples(void)
		{
			int samplesCount = SamplesCount;

			array<short>^ samples = gcnew array<short>(samplesCount);
			for (int i = 0; i < samplesCount; i++)
			{
				samples[i] = cst_wave_samples(wave)[i];
			}

			return samples;
		}

		void Resize(int samplesCount)
		{
			if (samplesCount <= 0)
			{
				throw gcnew ArgumentOutOfRangeException(L"samplesCount");
			}

			cst_wave_resize(wave, samplesCount, ChannelsCount);

			return;
		}

		void Resample(int samplingRate)
		{
			if (samplingRate < 1)
			{
				throw gcnew ArgumentOutOfRangeException(L"samplingRate");
			}

			cst_wave_resample(wave, samplingRate);

			return;
		}

		void Amplify(double factor)
		{
			int f = (int)(factor * 65536);

			cst_wave_rescale(wave, f);

			return;
		}

		void Append(FliteWave^ fliteWave)
		{
			if (fliteWave == nullptr)
			{
				throw gcnew ArgumentNullException(L"fliteWave");
			}

			if (fliteWave->SamplingRate != SamplingRate)
			{
				throw gcnew ArgumentException(L"Invalid SamplingRate.", L"fliteWave");
			}

			if (fliteWave->ChannelsCount != ChannelsCount)
			{
				throw gcnew ArgumentException(L"Invalid ChannelsCount.", L"fliteWave");
			}

			if (fliteWave->BitsPerSample != BitsPerSample)
			{
				throw gcnew ArgumentException(L"Invalid BitsPerSample.", L"fliteWave");
			}

			concat_wave(wave, fliteWave->wave);

			return;
		}

		FliteWave^ Clone()
		{
			cst_wave* w = copy_wave(wave);
			FliteWave^ fliteWave = gcnew FliteWave(w);

			return fliteWave;
		}

		void Save(String^ fileName)
		{
			if (fileName == nullptr)
			{
				throw gcnew ArgumentNullException(L"fileName");
			}

			string stdFileName = Utility::MarshalString(fileName);

			cst_wave_save_riff(wave, stdFileName.c_str());

			return;
		}

		void SaveRaw(String^ fileName)
		{
			if (fileName == nullptr)
			{
				throw gcnew ArgumentNullException(L"fileName");
			}

			string stdFileName = Utility::MarshalString(fileName);

			cst_wave_save_raw(wave, stdFileName.c_str());

			return;
		}

		void Play(void)
		{
			// play_wave(wave);

			WavePlayer wavePlayer(SamplingRate, ChannelsCount, BitsPerSample);

			wavePlayer.Open();
			wavePlayer.PlayBuffer
			(
				(unsigned char*)cst_wave_samples(wave),
				ChannelsCount * SamplesCount * (BitsPerSample / 8)
			);
			wavePlayer.Close();

			return;
		}

	internal:

		FliteWave(cst_wave* wave)
		{
			if (wave == NULL)
			{
				throw gcnew ArgumentNullException(L"wave");
			}

			this->wave = wave;
		}

	private:

		cst_wave* wave;
	};
}
