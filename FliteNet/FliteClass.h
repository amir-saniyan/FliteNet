#pragma once

#include <string>

using namespace std;
using namespace System;

namespace FliteNet
{
	public ref class Flite abstract sealed
	{
		static Flite(void)
		{
			flite_init();
			
			awb = register_cmu_us_awb(NULL);
			kal = register_cmu_us_kal(NULL);
			rms = register_cmu_us_rms(NULL);
			slt = register_cmu_us_slt(NULL);

			if
			(
				   (awb == NULL)
				|| (kal == NULL)
				|| (rms == NULL)
				|| (slt == NULL)
			)
			{
				unregister_cmu_us_awb(awb);
				unregister_cmu_us_kal(kal);
				unregister_cmu_us_rms(rms);
				unregister_cmu_us_slt(slt);

				throw gcnew ApplicationException(L"register_cmu_us_xxx() failed.");
			}

			Speaker = FliteSpeaker::Kal;
		}

	public:

		static property FliteSpeaker Speaker
		{
			FliteSpeaker get(void)
			{
				return selectedSpeaker;
			}

			void set(FliteSpeaker value)
			{
				switch (value)
				{
				case FliteSpeaker::Awb:
					selectedVoice = awb;
					break;

				case FliteSpeaker::Kal:
					selectedVoice = kal;
					break;

				case FliteSpeaker::Rms:
					selectedVoice = rms;
					break;

				case FliteSpeaker::Slt:
					selectedVoice = slt;
					break;

				default:
					throw gcnew ArgumentException(L"Invalid FliteSpeaker value.");
				}

				selectedSpeaker = value;

				return;
			}
		}

		static double GetSpeechLength(String^ text)
		{
			if (text == nullptr)
			{
				throw gcnew ArgumentNullException(L"text");
			}

			string stdText = Utility::MarshalString(text);
			float speechLength = flite_text_to_speech(stdText.c_str(), selectedVoice, "none");

			return speechLength;
		}

		static double GetSpeechLength(String^ text, FliteSpeaker speaker)
		{
			Speaker = speaker;
			return GetSpeechLength(text);
		}

		static void Speak(String^ text)
		{
			if (text == nullptr)
			{
				throw gcnew ArgumentNullException(L"text");
			}

			FliteWave^ fliteWave = SpeakToWave(text);

			fliteWave->Play();

			return;
		}

		static void Speak(String^ text, FliteSpeaker speaker)
		{
			Speaker = speaker;
			Speak(text);
			return;
		}

		static FliteWave^ SpeakToWave(String^ text)
		{
			if (text == nullptr)
			{
				throw gcnew ArgumentNullException(L"text");
			}

			string stdText = Utility::MarshalString(text);

			cst_wave* wave = flite_text_to_wave(stdText.c_str(), selectedVoice);
			if (wave == NULL)
			{
				throw gcnew ApplicationException("flite_text_to_wave() failed.");
			}

			FliteWave^ fliteWave = gcnew FliteWave(wave);
			return fliteWave;
		}

		static FliteWave^ SpeakToWave(String^ text, FliteSpeaker speaker)
		{
			Speaker = speaker;
			return SpeakToWave(text);
		}

		static void SpeakToFile(String^ text, String^ fileName)
		{
			if (text == nullptr)
			{
				throw gcnew ArgumentNullException(L"text");
			}
			if (fileName == nullptr)
			{
				throw gcnew ArgumentNullException(L"fileName");
			}

			string stdText = Utility::MarshalString(text);
			string stdFileName = Utility::MarshalString(fileName);

			flite_text_to_speech(stdText.c_str(), selectedVoice, stdFileName.c_str());

			return;
		}

		static void SpeakToFile(String^ text, String^ fileName, FliteSpeaker speaker)
		{
			Speaker = speaker;
			SpeakToFile(text, fileName);
			return;
		}

	private:

		static cst_voice* awb;
		static cst_voice* kal;
		static cst_voice* rms;
		static cst_voice* slt;

		static cst_voice* selectedVoice;
		static FliteSpeaker selectedSpeaker;
	};
}
