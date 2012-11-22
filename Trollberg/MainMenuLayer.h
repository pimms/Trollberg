#pragma once 
#include "Pim.h"
#include "ParallaxLayer.h"
#include "MenuButton.h"

// Defines
#define NUMMENYBUTTONS 4
#define MENYSPEED 10

class MainMenuLayer: public Pim::Layer, public Pim::ButtonCallback
{
public:
	MainMenuLayer();
	~MainMenuLayer();

	void loadResources();
	void loadParallax();

	MenuButton* createButton(int xPos, int yPos, std::string buttonLabel);

	void buttonPressed(Pim::Button*);

	void update(float dt);
	void updateScroll(float dt);
	void updateButtons(float dt);

private:
	Pim::SpriteBatchNode	*buttonSheet;
	Pim::SpriteBatchNode	*scrollSheet;

	ParallaxLayer			*background;
	ParallaxLayer			*foreground;

	Pim::Font				*buttonFont;

	MenuButton				*menuButtons[NUMMENYBUTTONS];
	float					buttonYPos[NUMMENYBUTTONS];

	MenuButton				*menuButtonTest;

	bool					playIntro;
	bool					playOutro;
	bool					startGame;

	int						startLVL;
};