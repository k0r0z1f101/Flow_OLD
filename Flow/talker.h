#pragma once


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
