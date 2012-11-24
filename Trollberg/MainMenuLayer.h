#pragma once 
#include "Pim.h"
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
	void loadButtons();
	void loadSprites();
	void loadParallax();

	MenuButton* createButton(int xPos, int yPos, std::string buttonLabel);

	void buttonPressed(Pim::Button*);

	void update(float dt);
	void updateScroll(float dt);
	void updateButtons(float dt);

	/* DEBUG */
	void keyEvent(Pim::KeyEvent&);

private:
	Pim::SpriteBatchNode	*buttonSheet;
	Pim::SpriteBatchNode	*scrollSheet;

	Pim::Sound				*music;

	Pim::Font				*buttonFont;

	MenuButton				*menuButtons[NUMMENYBUTTONS];
	float					buttonYPos[NUMMENYBUTTONS];

	MenuButton				*menuButtonTest;

	// The light we will follow through the forest
	Pim::GameNode			*light;

	// The destination we're scrolling to
	bool					isScrolling;
	float					scrollDest;

	// How many times have we scrolled? 
	// 1 = show menu
	// 2 = go to game
	int						numScrolls;

	// Button dropdown
	bool					playIntro;
	bool					playOutro;
	bool					startGame;

	int						startLVL;
};