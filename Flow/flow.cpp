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
	SoundBuffer buffer;
	if(!buffer.loadFromFile(sample))
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

	float bpm = 120.0f;
	int waitTime = int((secsmins / bpm) * 1000000.f);

	int wait;
	int cmp = 0;
	for(;;)
	{
		time_t currentTime = getMilliseconds() * 1000;
		time_t shouldBe = ((startTime * 1000) + (waitTime * cmp));
		if(shouldBe <= currentTime)
		{
			clear_screen();
			cout << startTime << endl << waitTime << endl;
			cout << "supposed to be: " << shouldBe << endl;
			cout << "really is: " << "\t" << currentTime << endl;
			cout << "difference: " << (shouldBe - currentTime) << endl;
			cout << "bpm: " << bpm << endl;
			wait = rand() % 12;
			sample1.stop();
			sample2.stop();
			float pitch = 0.0f + float(wait/3);
			sample1.setPitch(pitch);
			sample1.play();
			sample2.play();
			++cmp;
		}
	}

    return 0;
}
