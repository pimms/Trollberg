#pragma once 
#include "Pim.h"


class MainMenuScene: public Pim::Scene, public Pim::ButtonCallback
{
public:
	MainMenuScene(float ambientPos = 0.f);
	~MainMenuScene();

	void loadResources();
	void startGame();

	void update(float dt);

private:
	// The rain sound is played and handled by the scene.
	Pim::Sound	*ambientRain;
	float ambientStartPos;
};