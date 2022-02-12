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
#include <array>

//test
using namespace std;
using namespace sf;
using namespace chrono;

//globales
const float secsmins = 60.0f;
const int barSize = 4;

//fonction pour nettoyer l'écran cross-compatibility
void clear_screen()
{
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#else
    // Assume POSIX
    std::system ("clear");
#endif
}

//fonction pour charger un son dans le buffer
SoundBuffer bufferSample(string sample)
{
	string soundfolder;
#if defined(_WIN32) || defined(_WIN64)
	soundfolder = "../../Sounds/";
	//soundfolder = ""; //need this to compile a .exe and sounds need to be in the .exe folder
#else
    // Assume POSIX
	soundfolder = "../Sounds/";
#endif
	SoundBuffer buffer;
	if(!buffer.loadFromFile(soundfolder+sample))
		return buffer;
	return buffer;
}

//fonction pour charger le buffer dans un Sound object
Sound loadSample(SoundBuffer& buffer)
{
	Sound sample;
	sample.setBuffer(buffer);
	sample.setVolume(5.f);
	return sample;
}

//fonction pour avoir le temps en millisecondes depuis Epoch
time_t getMilliseconds()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//fonction pour avancer dans la trame du sample2 et jouer ou pas le son
void playSample(time_t startTime, int beatTime, time_t& sampleNextStep, time_t currentTime, float bpm, int bpmCmp, Sound& sample, array<int,barSize> sampleBar,  int& sampleBarPlayed, bool pitch = false)
{
	cout << "*****Sample*****" << endl;
	cout << startTime << endl << beatTime << endl;
	cout << "step: " << sampleBarPlayed << endl;
	cout << "sampleNextStep: \t" << sampleNextStep << endl;
	cout << "really is: " << "\t\t" << currentTime << endl;
	cout << "bpm: " << bpm << ", bar: " << (bpmCmp + 1) << endl;
	if(pitch) sample.setPitch(0.0f + float((rand() % 501)/100));
	if(sampleBar[sampleBarPlayed] == 1)
	{
		sample.stop();
		sample.play();
	}
	++sampleBarPlayed;
	sampleBarPlayed = sampleBarPlayed >= barSize ? 0 : sampleBarPlayed;
	sampleNextStep += (beatTime / barSize);
}

//fonction de départ pour randomiser un bar de sample
void buildSampleBar(array<int,barSize>& sampleBar)
{
	for(int i = 0;i < barSize;++i)
	{
		sampleBar[i] = rand() % 2;
	}
}

int main()
{
	time_t startTime = getMilliseconds();
	srand(int(time(0)));

	SoundBuffer buffer1;
	buffer1 = bufferSample("Sine_wave_440.ogg");
	Sound sample1;
	sample1 = loadSample(buffer1);
	time_t sample1NextStep = 0;

	SoundBuffer buffer2;
	buffer2 = bufferSample("kick-hip-hop-punchy-3.wav");
	Sound sample2;
	sample2 = loadSample(buffer2);
	time_t sample2NextStep = 0;

	//test array for 1 bar of sample1 (sine wave)
	array<int,barSize> sample1Bar = { 1, 0, 0, 0 };
	int sample1BarPlayed = 0;

	//test array for 1 bar of sample2 (kick drum)
	array<int,barSize> sample2Bar = { 1, 0, 0, 0 };
	int sample2BarPlayed = 0;

	float bpm = 120.0f;
	int beatTime = int((secsmins / bpm) * 1000000.f);

	int wait;
	int bpmCmp = 0;
	for(;;)
	{
		time_t currentTime = getMilliseconds() * 1000;
		time_t bpmNextBar = time_t(((startTime * 1000) + (beatTime * (bpmCmp + 1))));

		//nettoyer l'écran si un sample joue
		if(time_t(sample1NextStep) <= currentTime || time_t(sample2NextStep) <= currentTime)
			clear_screen();

		//jouer le sample sine wave si c'est son temps
		sample1NextStep = sample1NextStep == 0 ? bpmNextBar : sample1NextStep;
		if(time_t(sample1NextStep) <= currentTime)
		{
			playSample(startTime, beatTime, sample1NextStep, currentTime, bpm, bpmCmp, sample1, sample1Bar, sample1BarPlayed, true);
		}

		//jouer le sample kick drum si c'est son temps
		sample2NextStep = sample2NextStep == 0 ? bpmNextBar : sample2NextStep;
		if(time_t(sample2NextStep) <= currentTime)
		{
			playSample(startTime, beatTime, sample2NextStep, currentTime, bpm, bpmCmp, sample2, sample2Bar, sample2BarPlayed);
		}

		//compteur de bar et reconstruction des samples bar
		if(bpmNextBar <= currentTime)
		{
			buildSampleBar(sample1Bar);
			buildSampleBar(sample2Bar);
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
