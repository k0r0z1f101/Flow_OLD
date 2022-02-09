#if defined(_WIN32) || defined(_WIN64)
#include "Include/SFML/Audio.hpp"
#include <Windows.h>
#include "conio.h"
#else
#include <SFML/Audio.hpp>
#include <unistd.h>
#include <ncurses.h>
using namespace sf;
#endif

#include <iostream>
//test
using namespace std;

SoundBuffer bufferSample(string sample)
{
	SoundBuffer buffer;
	if(!buffer.loadFromFile("../Sounds/"+sample))
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

int main()
{
	srand(int(time(0)));

	SoundBuffer buffer;
	buffer = bufferSample("Sine_wave_440.ogg");
	Sound sample1;
	sample1 = loadSample(buffer);

	SoundBuffer buffer2;
	buffer2 = bufferSample("kick-hip-hop-punchy-3.wav");
	Sound sample2;
	sample2 = loadSample(buffer2);

	int waitTime = rand()% 500000 + 50000;

	int wait;
	for(;;)
	{
		wait = rand() % 12;
		sample1.stop();
		sample2.stop();
		float pitch = 0.0f + float(wait/3);
		sample1.setPitch(pitch);
		sample1.play();
		sample2.play();
		usleep(waitTime);
	}

    return 0;
}
