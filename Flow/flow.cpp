#if defined(_WIN32) || defined(_WIN64)
#include <SFML/Audio.hpp>
#include <Windows.h>
#include "conio.h"
#else
#include <SFML/Audio.hpp>
#include <unistd.h>
#include <ncurses.h>
#include <sys/time.h>
#endif

#include <chrono>
#include <ctime>
#include <cstdlib>
#include <iostream>
//test
using namespace std;
using namespace sf;
using namespace chrono;

void clear_screen()
{
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#else
    // Assume POSIX
    std::system ("clear");
#endif
}

SoundBuffer bufferSample(string sample)
{
	string soundfolder;
#if defined(_WIN32) || defined(_WIN64)
	soundfolder = "../../Sounds/";
#else
    // Assume POSIX
	soundfolder = "../Sounds/";
#endif
	SoundBuffer buffer;
	if(!buffer.loadFromFile(soundfolder+sample))
		return buffer;
	return buffer;
}

Sound loadSample(SoundBuffer& buffer)
{
	Sound sample;
	sample.setBuffer(buffer);
	sample.setVolume(5.f);
	return sample;
}

time_t getMilliseconds()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

const float secsmins = 60.0f;

void playSample2(time_t startTime, int beatTime, int& sample2NextStep, time_t currentTime, float bpm, int bpmCmp, Sound& sample2, array<int,32> sample2Bar,  int& sample2BarPlayed)
{
	if(sample2Bar[sample2BarPlayed] == 1)
	{
		cout << "*****Sample 2*****" << endl;
		cout << startTime << endl << beatTime << endl;
		cout << "supposed to be: " << ((startTime * 1000) + ((bpmCmp + 1) * beatTime)) << endl;
		cout << "really is: " << "\t" << currentTime << endl;
		cout << "difference: " << (((startTime * 1000) + ((bpmCmp + 1) * beatTime)) - currentTime) << endl;
		cout << "bpm: " << bpm << ", bar: " << (bpmCmp + 1) << endl;
		sample2.stop();
		sample2.play();
	}
	++sample2BarPlayed;
	sample2BarPlayed = sample2BarPlayed >= 32 ? 0 : sample2BarPlayed;
	sample2NextStep += (beatTime - (32 - sample2BarPlayed));
}

int main()
{
	time_t startTime = getMilliseconds();
	srand(int(time(0)));

	SoundBuffer buffer;
	buffer = bufferSample("Sine_wave_440.ogg");
	Sound sample1;
	sample1 = loadSample(buffer);

	SoundBuffer buffer2;
	buffer2 = bufferSample("kick-hip-hop-punchy-3.wav");
	Sound sample2;
	sample2 = loadSample(buffer2);
	int sample2NextStep = 0;

	//test array for 1 bar of sample2 (kick drum)
	array<int,32> sample2Bar = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int sample2BarPlayed = 0;

	float bpm = 120.0f;
	int beatTime = int((secsmins / bpm) * 1000000.f);

	int wait;
	int bpmCmp = 0;
	for(;;)
	{
		time_t currentTime = getMilliseconds() * 1000;
		time_t bpmNextBar = time_t(((startTime * 1000) + (beatTime * (bpmCmp + 1))));
		sample2NextStep = sample2NextStep == 0 ? int(bpmNextBar) : sample2NextStep;
//		if(shouldBe <= currentTime)
//		{
//			clear_screen();
//			cout << startTime << endl << waitTime << endl;
//			cout << "supposed to be: " << shouldBe << endl;
//			cout << "really is: " << "\t" << currentTime << endl;
//			cout << "difference: " << (shouldBe - currentTime) << endl;
//			cout << "bpm: " << bpm << endl;
//			wait = rand() % 12;
//			sample1.stop();
//			sample2.stop();
//			float pitch = 0.0f + float(wait/3);
//			sample1.setPitch(pitch);
//			sample1.play();
//			sample2.play();
//			++bpmCmp;
//		}
		if(sample2NextStep <= currentTime)
		{
			clear_screen();
			playSample2(startTime, beatTime, sample2NextStep, currentTime, bpm, bpmCmp, sample2, sample2Bar, sample2BarPlayed);
		}
		if(bpmNextBar <= currentTime)
		{
			++bpmCmp;
			if(bpmCmp >= 120)
			{
				cout << "120 beats: " << (currentTime - (startTime * 1000)) << endl;
				cin >> wait;
			}
		}
	}

    return 0;
}
