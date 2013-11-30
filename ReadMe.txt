In the name of God

FliteNet 1.0.0, a .NET wrapper for Flite.
Amir Saniyan <amir_saniyan@yahoo.com>
Saturday - 30 November 2013

About FliteNet
-----------------
FliteNet is a .NET wrapper for Flite.
Flite (festival-lite) is a small, fast run-time synthesis (Text To Speech) engine developed at CMU and primarily designed for small embedded machines and/or large servers. Flite is designed as an alternative synthesis engine to Festival for voices built using the FestVox suite of voice building tools.

Using FliteNet
-----------------
Using FliteNet is simple. Add reference of FliteNet.dll to your project. Use Flite class to convert text to speech.

Simple text to speech
---------------------------
* Use Flite.Speak(string text) to convert text to speech and play it from speakers.
* Use Flite.Speaker for get or set current speaker.
* Use Flite.Speak(string text, FliteSpeaker speaker) to synthesis with specified speaker.

Example:

using FliteNet;
using System;

namespace Demo
{
    class Program
    {
        private static void Main(string[] args)
        {
            // Play Hello world!
            Console.WriteLine("Hello world!");
            Flite.Speak("Hello world!");

            // Show current speaker.
            Console.WriteLine("Current speaker: {0}", Flite.Speaker);

            // Change speaker and play Hello world!

            Console.WriteLine("Change speaker to Slt");
            Flite.Speaker = FliteSpeaker.Slt;

            Console.WriteLine("Hello world!");
            Flite.Speak("Hello world!");

            // Play with differnet speakers.

            Console.WriteLine("Awb says hello world!");
            Flite.Speak("Awb says hello world!", FliteSpeaker.Awb);

            Console.WriteLine("Kal says hello world!");
            Flite.Speak("Kal says hello world!", FliteSpeaker.Kal);

            Console.WriteLine("Rms says hello world!");
            Flite.Speak("Rms says hello world!", FliteSpeaker.Rms);

            Console.WriteLine("Slt says hello world!");
            Flite.Speak("Slt says hello world!", FliteSpeaker.Slt);

            return;
        }
    }
}

Generating waveform files
--------------------------------
Use one of the below methods:
* Flite.SpeakToFile(string text, string fileName)
* Flite.SpeakToFile(string text, string fileName, FliteSpeaker speaker)

Example:

using FliteNet;
using System;

namespace Demo
{
    class Program
    {
        private static void Main(string[] args)
        {
            // Synthesis Hello world!
            Console.WriteLine("Hello world!");
            Flite.SpeakToFile("Hello world!", @"D:\HelloWorld.wav");

            // Synthesis with differnet speakers.

            Console.WriteLine("Awb says hello world!");
            Flite.SpeakToFile("Awb says hello world!", @"D:\Awb.wav", FliteSpeaker.Awb);

            Console.WriteLine("Kal says hello world!");
            Flite.SpeakToFile("Kal says hello world!", @"D:\Kal.wav", FliteSpeaker.Kal);

            Console.WriteLine("Rms says hello world!");
            Flite.SpeakToFile("Rms says hello world!", @"D:\Rms.wav", FliteSpeaker.Rms);

            Console.WriteLine("Slt says hello world!");
            Flite.SpeakToFile("Slt says hello world!", @"D:\Slt.wav", FliteSpeaker.Slt);

            return;
        }
    }
}

Advanced features
----------------------
For working with speech in memory use below methods:
* Flite.SpeakToWave(string text)
* Flite.SpeakToWave(string text, FliteSpeaker speaker)

Example:

using FliteNet;
using System;

namespace Demo
{
    class Program
    {
        private static void Main(string[] args)
        {
            // Synthesis.
            FliteWave wave = Flite.SpeakToWave("Numbers: ", FliteSpeaker.Rms);

            // Append 1 to 5.
            for (int i = 1; i <= 5; i++)
            {
                FliteWave w = Flite.SpeakToWave(i.ToString(), FliteSpeaker.Rms);

                // Append.
                wave.Append(w);
            }

            // Change samples.
            for (int i = 0; i < 1000; i++)
            {
                wave[i] = 0;
            }

            // Samples.
            short[] samples = wave.GetSamples();

            // Amplify (150%).
            wave.Amplify(1.5);

            // Resample to 44.1 KHz.
            wave.Resample(44100);

            // Waveform info.
            Console.WriteLine("Waveform info:");
            Console.WriteLine("Sampling Rate: {0}", wave.SamplingRate);
            Console.WriteLine("Channels Count: {0}", wave.ChannelsCount);
            Console.WriteLine("Bits Per Sample: {0}", wave.BitsPerSample);
            Console.WriteLine("Samples Count: {0}", wave.SamplesCount);
            Console.WriteLine("Length (Seconds): {0}", wave.Length);

            // Play.
            Console.WriteLine("Playing...");
            wave.Play();

            // Save wav.
            wave.Save(@"D:\Test.wav");

            // Save raw.
            wave.SaveRaw(@"D:\Test.raw");

            return;
        }
    }
}

See also
-----------
* http://www.cmuflite.org
