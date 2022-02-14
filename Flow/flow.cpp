#include "flow.h"

int main()
{
	time_t startTime = getMilliseconds();
	srand(int(time(0)));

	//sample variables //0: sine wave, 1: kick drum
	const size_t sampleNumbers = 3;
	//array<SoundBuffer, sampleNumbers> buffers = { bufferSample("Sine_wave_440.ogg"), bufferSample("kick-hip-hop-punchy-3.wav") };
	array<SoundBuffer, sampleNumbers> buffers = { bufferSample("ahh-choir-female-vocal-high_C_minor.wav"), bufferSample("juicy-kick-power-hit.wav"), bufferSample("kick-hip-hop-punchy-3.wav") };
	array<Sound, sampleNumbers> samples = { loadSample(buffers[0]), loadSample(buffers[1]), loadSample(buffers[2]) };
	array<time_t, sampleNumbers> nextSteps = { 0, 0, 0 };
	//array of array for sampleBars
	array<int, sampleNumbers> sampleBarsPlayed = { 0, 0, 0 };
	array<string,sampleNumbers> sampleBehaviours = { "default", "default", "default" };
	vector<array<int, barSize>> sampleBars = { { 1, 0, 0, 0 }, { 1, 0, 0, 0 }, { 1, 0, 0, 0 } }; //test

	char a;

	array<int,barSize> sample1Bar = { 1, 0, 0, 0 };

	array<int,barSize> sample2Bar = { 1, 0, 0, 0 };

	array<int,barSize> sample3Bar = { 1, 0, 0, 0 };

	float bpm = 60.0f;
	int beatTime = int((secsmins / bpm) * 1000000.f);

	int wait;
	int barCmp = 0;
	int measureCmp = 0;
	time_t bpmNextMeasure;
	for(;;)
	{
		time_t currentTime = getMilliseconds() * 1000;
		time_t bpmNextBar = time_t(((startTime * 1000) + (beatTime * (barCmp + 1))));
		bpmNextMeasure = barCmp % measureSize == 0 ? time_t(((startTime * 1000) + ((beatTime * measureSize) * (measureCmp + 1)))) : bpmNextMeasure;

		//nettoyer l'écran si un sample joue
		if(time_t(nextSteps[0]) <= currentTime || time_t(nextSteps[1]) <= currentTime || time_t(nextSteps[2]) <= currentTime)
		{
			clear_screen();
			cout << "started: \t" << startTime << ", bar time: " << beatTime << endl;
			cout << "current time: " << "\t" << currentTime << endl;
			cout << "bpm: " << bpm << ", measure: " << (measureCmp + 1) << ", bar: " << (barCmp + 1) << endl;
		}

		//jouer le sample 1 si c'est son temps
		nextSteps[0] = nextSteps[0] == 0 ? bpmNextBar : nextSteps[0];
		if(time_t(nextSteps[0]) <= currentTime)
		{
			playSample(startTime, beatTime, nextSteps[0], currentTime, bpm, measureCmp, barCmp, samples[0], sample1Bar, sampleBarsPlayed[0], true, true);
		}

		//jouer le sample 2 si c'est son temps
		nextSteps[1] = nextSteps[1] == 0 ? bpmNextBar : nextSteps[1];
		if(time_t(nextSteps[1]) <= currentTime)
		{
			playSample(startTime, beatTime, nextSteps[1], currentTime, bpm, measureCmp, barCmp, samples[1], sample2Bar, sampleBarsPlayed[1], false, false, 10.f);
		}

		//jouer le sample 3 si c'est son temps
		nextSteps[2] = nextSteps[2] == 0 ? bpmNextBar : nextSteps[2];
		if(time_t(nextSteps[2]) <= currentTime)
		{
			playSample(startTime, beatTime, nextSteps[2], currentTime, bpm, measureCmp, barCmp, samples[2], sample3Bar, sampleBarsPlayed[2], false, false, 8.f);
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
			sampleBehaviour(sample3Bar);
			++measureCmp;
		}
		//waitKeyToPress();
	}

    return 0;
}
