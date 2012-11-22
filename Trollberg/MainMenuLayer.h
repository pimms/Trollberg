#pragma once 
#include "Pim.h"
#include "MenuButton.h"
#define NUMMENYBUTTONS 4
#define MENYSPEED 10

class MainMenuLayer: public Pim::Layer, public Pim::ButtonCallback
{
public:
	MainMenuLayer();

	void loadResources();

	MenuButton* createButton(int xPos, int yPos, std::string buttonLabel);

	void buttonPressed(Pim::Button*);

	void MainMenuLayer::update(float dt);

private:
	Pim::SpriteBatchNode *buttonSheet;

	Pim::Button *mainButton;

	MenuButton *menuButtonsArray[NUMMENYBUTTONS];
	float menuButtonsAndPosYArray[NUMMENYBUTTONS];

	MenuButton *menuButtonTest;

	bool playIntro;
	bool playOutro;
	bool startGame;
};