#include "flow.h"

//globales pour le talker
const int windowWidth = 800;
const int windowHeight = 600;


//le talker est un cercle animé représentant la créature qui va nous guider dans le jeu
class Talker
{
private:
	float happy = 0.f;
	float scared = 0.f;
	float angry = 0.f;
	float sad = 0.f;

public:
	float getHappy()
	{
		return happy;
	}
	float getScared()
	{
		return scared;
	}
	float getAngry()
	{
		return angry;
	}
	float getSad()
	{
		return sad;
	}
	void changeHappy(int points)
	{
		happy += points;
	}
	void changeScared(int points)
	{
		scared += points;
	}
	void changeAngry(int points)
	{
		angry += points;
	}
	void changeSad(int points)
	{
		sad += points;
	}
};

class Sample
{
private:
	time_t nextStep = 0;
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
};

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

	array<int,barSize> sample1Bar = { 1, 0, 0, 0 };

	array<int,barSize> sample2Bar = { 1, 0, 0, 0 };

	array<int,barSize> sample3Bar = { 1, 0, 0, 0 };

	//initialiser les samples et les buffers
	Sample sineWave;
	Sample kickDrum1;
	Sample kickDrum2;
	array<Sample, sampleNumbers> samplesObject = { sineWave, kickDrum1, kickDrum2 };

	array<string, sampleNumbers> sampleNames = { "ahh-choir-female-vocal-high_C_minor.wav", "juicy-kick-power-hit.wav", "kick-hip-hop-punchy-3.wav" };
	for(int i = 0; i < sampleNumbers; ++i)
	{
		samplesObject[i].initBuffer(sampleNames[i]);
		samplesObject[i].initSound();
	}

	float bpm = 60.0f;
	int beatTime = int((secsmins / bpm) * 1000000.f);

	int wait;
	int barCmp = 0;
	int measureCmp = 0;
	time_t bpmNextMeasure;

	//instanciation d'un talker et affichage dans une fenêtre
	Talker talkie;
	// create the window
	RenderWindow talkieWindow(VideoMode(windowWidth, windowHeight), "Talkie");

	// run the program as long as the window is open
	while (talkieWindow.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (talkieWindow.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				talkieWindow.close();
		}

		// clear the window with black color
		talkieWindow.clear(sf::Color::Black);

		// draw everything here...
		// window.draw(...);
		float talkerRadius = 50.f;
		CircleShape shape(talkerRadius);
		// set the shape color to green
		shape.setFillColor(sf::Color(100, 250, 50));
		shape.setPosition(windowWidth / 2 - talkerRadius, windowHeight - talkerRadius * 2);

		//
		//		for(;;)
		//		{
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

				//jouer les samples

				//jouer le sample 1 si c'est son temps
				nextSteps[0] = nextSteps[0] == 0 ? bpmNextBar : nextSteps[0];
				if(time_t(nextSteps[0]) <= currentTime)
				{
//					playSample(startTime, beatTime, nextSteps[0], currentTime, bpm, measureCmp, barCmp, samples[0], sample1Bar, sampleBarsPlayed[0], shape, true, true);
					playSample(startTime, beatTime, nextSteps[0], currentTime, bpm, measureCmp, barCmp, samplesObject[0].sample, sample1Bar, sampleBarsPlayed[0], shape, true, true);
				}

//				//jouer le sample 2 si c'est son temps
				nextSteps[1] = nextSteps[1] == 0 ? bpmNextBar : nextSteps[1];
				if(time_t(nextSteps[1]) <= currentTime)
				{
//					playSample(startTime, beatTime, nextSteps[1], currentTime, bpm, measureCmp, barCmp, samples[1], sample2Bar, sampleBarsPlayed[1], shape, false, false, 10.f);
					playSample(startTime, beatTime, nextSteps[1], currentTime, bpm, measureCmp, barCmp, samplesObject[1].sample, sample2Bar, sampleBarsPlayed[1], shape, true, true);
				}
//
//				//jouer le sample 3 si c'est son temps
				nextSteps[2] = nextSteps[2] == 0 ? bpmNextBar : nextSteps[2];
				if(time_t(nextSteps[2]) <= currentTime)
				{
					playSample(startTime, beatTime, nextSteps[2], currentTime, bpm, measureCmp, barCmp, samples[2], sample3Bar, sampleBarsPlayed[2], shape, false, false, 8.f);
					playSample(startTime, beatTime, nextSteps[2], currentTime, bpm, measureCmp, barCmp, samplesObject[2].sample, sample3Bar, sampleBarsPlayed[2], shape, true, true);
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

		//	}


		if(sampleBarsPlayed[1] == 1)
		{
			shape.setScale(1.2f,1.2f);
			shape.setPosition(windowWidth / 2 - talkerRadius * 1.2, windowHeight - talkerRadius * 1.2 * 2);
		}
		talkieWindow.draw(shape);

		// end the current frame
		talkieWindow.display();



	}


    return 0;
}
