#pragma once 
#include "Pim.h"
#include "MenuButton.h"
#define NUMMENYBUTTONS 4
#define MENYSPEED 10

class MainMenuLayer: public Pim::Layer, public Pim::ButtonCallback
{
public:
	MainMenuLayer();
	~MainMenuLayer();

	void loadResources();

	MenuButton* createButton(int xPos, int yPos, std::string buttonLabel);

	void buttonPressed(Pim::Button*);

	void update(float dt);
	void updateButtons(float dt);

private:
	Pim::SpriteBatchNode *buttonSheet;

	Pim::Font *buttonFont;

	MenuButton *menuButtonsArray[NUMMENYBUTTONS];
	float menuButtonsAndPosYArray[NUMMENYBUTTONS];

	MenuButton *menuButtonTest;

	bool playIntro;
	bool playOutro;
	bool startGame;

	int startLVL;
};