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

#include <chrono>
#include <sys/time.h>
#include <ctime>
#include <cstdlib>
#include <iostream>
//test
using namespace std;

void clear_screen()
{
#ifdef WINDOWS
    std::system("cls");
#else
    // Assume POSIX
    std::system ("clear");
#endif
}

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

time_t getMilliseconds()
{
	struct timeval time_now{};
	gettimeofday(&time_now, nullptr);
	time_t msecs_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);

//	cout << "seconds since epoch: " << time_now.tv_sec << endl;
//	cout << "milliseconds since epoch: "  << msecs_time << endl << endl;
//	cout << "nanoseconds since epoch: "  << (msecs_time * 1000) << endl << endl;
	return msecs_time;
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

	//int waitTime = rand()% 500000 + 50000;

	float bpm = 60.0f;
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
			//usleep(waitTime);
			++cmp;
		}
	}

    return 0;
}
