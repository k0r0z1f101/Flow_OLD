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

int main()
{
	time_t startTime = getMilliseconds();
	srand(int(time(0)));

	//sample variables //0: sine wave, 1: kick drum
	const size_t sampleNumbers = 3;

	//initialiser les samples et les buffers
	Sample sineWave;
	Sample kickDrum1;
	Sample kickDrum2;
	array<Sample, sampleNumbers> samplesObject = { sineWave, kickDrum1, kickDrum2 };
	array<string, sampleNumbers> sampleNames = { "ahh-choir-female-vocal-high_C_minor.wav", "juicy-kick-power-hit.wav", "kick-hip-hop-punchy-3.wav" };

	for(size_t i = 0; i < sampleNumbers; ++i)
	{
		samplesObject[i].initBuffer(sampleNames[i]);
		samplesObject[i].initSound();
	}

	int wait;
	float bpm = 60.0f;
	int beatTime = int((secsmins / bpm) * 1000000.f);
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

		time_t currentTime = getMilliseconds() * 1000;
		time_t bpmNextBar = time_t(((startTime * 1000) + (beatTime * (barCmp + 1))));
		bpmNextMeasure = barCmp % measureSize == 0 ? time_t(((startTime * 1000) + ((beatTime * measureSize) * (measureCmp + 1)))) : bpmNextMeasure;

		//nettoyer l'écran si un sample joue
		if(time_t(samplesObject[0].getNextStep()) <= currentTime || time_t(samplesObject[1].getNextStep()) <= currentTime || time_t(samplesObject[2].getNextStep()) <= currentTime)
		{
			clear_screen();
			cout << "started: \t" << startTime << ", bar time: " << beatTime << endl;
			cout << "current time: " << "\t" << currentTime << endl;
			cout << "bpm: " << bpm << ", measure: " << (measureCmp + 1) << ", bar: " << (barCmp + 1) << endl;
		}

		//jouer les samples
		for(size_t i = 0; i < sampleNumbers; ++i)
		{
			//jouer le sample si c'est son temps
			samplesObject[i].writeNextStep(samplesObject[i].getNextStep() == 0 ? bpmNextBar : samplesObject[i].getNextStep());
			if(time_t(samplesObject[i].getNextStep()) <= currentTime)
			{
				samplesObject[i].playSample(beatTime, true);
			}
		}

		//compteur de bar
		if(bpmNextBar <= currentTime)
		{
			samplesObject[0].buildSampleBar();
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
			samplesObject[1].buildSampleBar();
			samplesObject[2].buildSampleBar();
			++measureCmp;
		}

		if(samplesObject[0].getSampleBarsPlayed() == 1)
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
