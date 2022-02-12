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
void playSample(time_t startTime, int beatTime, time_t& sampleNextStep, time_t currentTime, float bpm, int barCmp, Sound& sample, array<int,barSize> sampleBar,  int& sampleBarPlayed, bool pitch = false, bool linger = false)
{
	cout << "*****Sample*****" << endl;
	cout << startTime << endl << beatTime << endl;
	cout << "step: " << sampleBarPlayed << endl;
	cout << "sampleNextStep: \t" << sampleNextStep << endl;
	cout << "really is: " << "\t\t" << currentTime << endl;
	cout << "bpm: " << bpm << ", bar: " << (barCmp + 1) << endl;
	if(pitch) sample.setPitch(0.0f + float((rand() % 501)/100));
	if(!linger)
		sample.stop();
	if(sampleBar[sampleBarPlayed] == 1)
	{
		if(!linger)
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

//systeme de comportement des sample bars, cohésion a court terme
void sampleBehaviour()
{
	//la succession des notes dans une mesure devrait etre controlée pour éviter un capharnaum total
	//les notes jouées devraient aussi avoir un minimum de cohésion entre les différents samples.

	//kick drum behaviour: beat repetitif simple le meme beat se repete, 1-1-1-1 (4 bars) DEFAULT
	//						beat repetitif double alternants, 1-2-1-2
	//						beat repetitif double successifs, 1-1-2-2 (4 bars), 1-1-1-1-2-2-2-2 (8 bars)
	//le pitch du kick drum n'est pas nécéssaire, le volume pour représenter l'intensité pourrait l'être

}

int main()
{
	time_t startTime = getMilliseconds();
	srand(int(time(0)));

	//sample variables //0: sine wave, 1: kick drum
	const size_t sampleNumbers = 2;
	array<SoundBuffer, sampleNumbers> buffers = { bufferSample("Sine_wave_440.ogg"), bufferSample("kick-hip-hop-punchy-3.wav") };
	array<Sound, sampleNumbers> samples = { loadSample(buffers[0]), loadSample(buffers[1]) };
	array<time_t, sampleNumbers> nextSteps = { 0 , 0 };
	//array of array for sampleBars
	array<int, sampleNumbers> sampleBarsPlayed = { 0, 0 };
	array<string,sampleNumbers> sampleBehaviours = { "default", "default" };

	array<int,barSize> sample1Bar = { 1, 0, 0, 0 };

	array<int,barSize> sample2Bar = { 1, 0, 0, 0 };

	float bpm = 120.0f;
	int beatTime = int((secsmins / bpm) * 1000000.f);

	int wait;
	int barCmp = 0;
	for(;;)
	{
		time_t currentTime = getMilliseconds() * 1000;
		time_t bpmNextBar = time_t(((startTime * 1000) + (beatTime * (barCmp + 1))));

		//nettoyer l'écran si un sample joue
		if(time_t(nextSteps[0]) <= currentTime || time_t(nextSteps[1]) <= currentTime)
			clear_screen();

		//jouer le sample sine wave si c'est son temps
		nextSteps[0] = nextSteps[0] == 0 ? bpmNextBar : nextSteps[0];
		if(time_t(nextSteps[0]) <= currentTime)
		{
			playSample(startTime, beatTime, nextSteps[0], currentTime, bpm, barCmp, samples[0], sample1Bar, sampleBarsPlayed[0], true, (rand() % 2 == 0 ? false : true));
		}

		//jouer le sample kick drum si c'est son temps
		nextSteps[1] = nextSteps[1] == 0 ? bpmNextBar : nextSteps[1];
		if(time_t(nextSteps[1]) <= currentTime)
		{
			playSample(startTime, beatTime, nextSteps[1], currentTime, bpm, barCmp, samples[1], sample2Bar, sampleBarsPlayed[1], true);
		}

		//compteur de bar et reconstruction des samples bar
		if(bpmNextBar <= currentTime)
		{
			buildSampleBar(sample1Bar);
			buildSampleBar(sample2Bar);
			++barCmp;
			if(barCmp >= 120)
			{
				cout << "120 bars: " << (currentTime - (startTime * 1000)) << endl;
				cin >> wait;
			}
		}
	}

    return 0;
}
