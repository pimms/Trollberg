#pragma once 
#include "Pim.h"


class MainMenuScene: public Pim::Scene, public Pim::ButtonCallback{

private:


public:

	MainMenuScene();

	void loadMainMenu();
	void loadResources();
	void startGame();



};