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

#include "Code_Utilities.h"
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <array>

//test
using namespace std;
using namespace sf;
using namespace chrono;
using namespace BdB;

//globales
const float secsmins = 60.0f;
const int barSize = 4;
const int measureSize = 4;

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
	return duration_cast<chrono::milliseconds>(system_clock::now().time_since_epoch()).count();
}

//fonction pour avancer dans la trame du sample2 et jouer ou pas le son
void playSample(time_t startTime, int beatTime, time_t& sampleNextStep, time_t currentTime, float bpm, int measureCmp, int barCmp, Sound& sample, array<int,barSize> sampleBar,  int& sampleBarPlayed, bool pitch = false, bool linger = false)
{
	string sampleBeat;
	for(int i = 0; i < 4; ++i)
	{
		sampleBeat += to_string(sampleBar[i]) + " ";
	}
	cout << "*****Sample***** " << sampleBeat << endl;
	cout << "step: " << sampleBarPlayed << endl;
	cout << "sampleNextStep: " << sampleNextStep << endl;
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
void buildSampleBar(array<int,barSize>& sampleBar, string behaviour = "default")
{
	if(behaviour == "default")
	{
		int notesLeft = 2;
		for(int i = 0; i < barSize; ++i)
		{
			sampleBar[i] = notesLeft > 0 ? rand() % 2 : 0;
			sampleBar[i] = notesLeft == 2 && i == (barSize - 1) ? 1 : sampleBar[i];
			notesLeft = sampleBar[i] != 0 ? notesLeft - 1 : notesLeft;
		}
	}
}

//systeme de comportement des sample bars, cohésion a court terme
void sampleBehaviour(array<int,barSize>& sampleBar)
{
	//la succession des notes dans une mesure devrait etre controlée pour éviter un capharnaum total
	//les notes jouées devraient aussi avoir un minimum de cohésion entre les différents samples.

	//KICK DRUM BEHAVIOUR
	//IN MESURE
	//		beat repetitif simple le meme beat se repete, 1-1-1-1 (4 bars) DEFAULT
	//		beat repetitif double alternants, 1-2-1-2
	//		beat repetitif double successifs, 1-1-2-2 (4 bars), 1-1-1-1-2-2-2-2 (8 bars)
	//IN BAR
	//
	//le pitch du kick drum n'est pas nécéssaire, le volume pour représenter l'intensité pourrait l'être

	buildSampleBar(sampleBar);
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

	float bpm = 60.0f;
	int beatTime = int((secsmins / bpm) * 1000000.f);

	int wait;
	int barCmp = 0;
	int measureCmp = 0;
	for(;;)
	{
		time_t currentTime = getMilliseconds() * 1000;
		time_t bpmNextBar = time_t(((startTime * 1000) + (beatTime * (barCmp + 1))));
		time_t bpmNextMeasure = barCmp % measureSize == 0 ? time_t(((startTime * 1000) + ((beatTime * measureSize) * (measureCmp + 1)))) : bpmNextMeasure;

		//nettoyer l'écran si un sample joue
		if(time_t(nextSteps[0]) <= currentTime || time_t(nextSteps[1]) <= currentTime)
		{
			clear_screen();
			cout << "started: \t" << startTime << ", bar time: " << beatTime << endl;
			cout << "current time: " << "\t" << currentTime << endl;
			cout << "bpm: " << bpm << ", measure: " << (measureCmp + 1) << ", bar: " << (barCmp + 1) << endl;
		}

		//jouer le sample sine wave si c'est son temps
		nextSteps[0] = nextSteps[0] == 0 ? bpmNextBar : nextSteps[0];
		if(time_t(nextSteps[0]) <= currentTime)
		{
			playSample(startTime, beatTime, nextSteps[0], currentTime, bpm, measureCmp, barCmp, samples[0], sample1Bar, sampleBarsPlayed[0], true, (rand() % 2 == 0 ? false : true));
		}

		//jouer le sample kick drum si c'est son temps
		nextSteps[1] = nextSteps[1] == 0 ? bpmNextBar : nextSteps[1];
		if(time_t(nextSteps[1]) <= currentTime)
		{
			playSample(startTime, beatTime, nextSteps[1], currentTime, bpm, measureCmp, barCmp, samples[1], sample2Bar, sampleBarsPlayed[1], false);
		}

		//compteur de bar
		if(bpmNextBar <= currentTime)
		{
			sampleBehaviour(sample1Bar);
//			sampleBehaviour(sample2Bar);
			++barCmp;
			if(barCmp >= 120)
			{
				cout << "120 bars: " << (currentTime - (startTime * 1000)) << endl;
				cin >> wait;
			}
		}

		//compteur de mesures et reconstruction des samples bar
		if(bpmNextMeasure <= currentTime)
		{
//			sampleBehaviour(sample1Bar);
			sampleBehaviour(sample2Bar);
			++measureCmp;
		}
		//waitKeyToPress();
	}

    return 0;
}
