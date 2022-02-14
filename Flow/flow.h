#pragma once

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

//fonction pour avancer dans la trame du sample2 et jouer ou pas le son
void playSample(time_t startTime, int beatTime, time_t& sampleNextStep, time_t currentTime, float bpm, int measureCmp, int barCmp, Sound& sample, array<int,barSize> sampleBar,  int& sampleBarPlayed, bool pitch = false, bool linger = false, float volume = 5.f)
{
	string sampleBeat;
	for(int i = 0; i < barSize; ++i)
	{
		sampleBeat += to_string(sampleBar[i]) + " ";
	}
	cout << "*****Sample***** " << sampleBeat << endl;
	cout << "step: " << sampleBarPlayed << endl;
	cout << "sampleNextStep: " << sampleNextStep << endl;
//	if(!linger)
//		sample.stop();
	if(sampleBar[sampleBarPlayed] == 1)
	{
		if(pitch) sample.setPitch(0.5f + float((rand() % 501)/100));
		sample.setVolume(volume);
//		if(linger)
//			sample.stop();
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
			sampleBar[i] = notesLeft > 0 ? rand() % 2 : rand() % 2;
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
