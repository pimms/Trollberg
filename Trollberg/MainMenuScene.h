#pragma once 
#include "Pim.h"
#include "MainMenuLayer.h"

class MainMenuScene: public Pim::Scene, public Pim::ButtonCallback
{
public:
	MainMenuScene(float ambientPos = 0.f);
	~MainMenuScene();

	void loadResources();
	void startGame(int level);

	void update(float dt);


	//MainMenuLayer *mml;

private:
	// The rain sound is played and handled by the scene.
	Pim::Sound	*ambientRain;
	float ambientStartPos;
};