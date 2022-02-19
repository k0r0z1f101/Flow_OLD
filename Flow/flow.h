#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "conio.h"
#else
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
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
#include "talker.h"

//namespaces
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
	return sample;
}

//fonction pour avoir le temps en millisecondes depuis Epoch
time_t getMilliseconds()
{
	return duration_cast<chrono::milliseconds>(system_clock::now().time_since_epoch()).count();
}

//controleur des différents sons, buffers et samples
class Sample
{
private:
	time_t sampleNextStep = 0;
	int sampleBarsPlayed = 0;
	string sampleBehaviour = "default";
	array<int, barSize> sampleBar = { 1, 0, 0, 0 };
public:
	SoundBuffer buffer;
	Sound sample;
	void initBuffer(string path)
	{
		buffer = bufferSample(path);
	}
	void initSound()
	{
		sample = loadSample(buffer);
	}
	SoundBuffer getBuffer()
	{
		return buffer;
	}
	Sound getSound()
	{
		return sample;
	}
	int getSampleBarsPlayed()
	{
		return sampleBarsPlayed;
	}
	time_t getNextStep()
	{
		return sampleNextStep;
	}
	void writeNextStep(time_t newStep)
	{
		sampleNextStep = newStep;
	}
	void playSample(int beatTime, bool pitch = false, float volume = 5.f)
	{
		string sampleBeat;
		for(int i = 0; i < barSize; ++i)
		{
			sampleBeat += to_string(sampleBar[i]) + " ";
		}
		cout << "*****Sample***** " << sampleBeat << endl;
		cout << "step: " << sampleBarsPlayed << endl;
		cout << "sampleNextStep: " << sampleNextStep << endl;
		if(sampleBar[sampleBarsPlayed] == 1)
		{
			if(pitch) sample.setPitch(0.5f + float((rand() % 501)/100));
			sample.setVolume(volume);
			sample.play();
		}
		++sampleBarsPlayed;
		sampleBarsPlayed = sampleBarsPlayed >= barSize ? 0 : sampleBarsPlayed;
		sampleNextStep += (beatTime / barSize);
	}
	void buildSampleBar(string behaviour = "default")
	{
		if(behaviour == "default")
		{
			int notesLeft = 2;
			for(int i = 0; i < barSize; ++i)
			{
				sampleBar[i] = notesLeft > 0 ? rand() % 2 : rand() % 2;
				sampleBar[i] = notesLeft == 2 && i == (barSize - 1) ? 1 : sampleBar[i];
				notesLeft = sampleBar[i] != 0 ? notesLeft - 1 : notesLeft;
			}
		}
	}
};
