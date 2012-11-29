#pragma once 
#include "Pim.h"

class HighscoreLayer;

// Defines
#define NUMMENYBUTTONS 4
#define MENYSPEED 1

// Forward declaratins
class ButtonScroller;

class MainMenuLayer: public Pim::Layer, public Pim::ButtonCallback
{
public:
	MainMenuLayer();
	~MainMenuLayer();

	void loadResources();
	void loadButtons();
	void loadSprites();
	void loadParallax();

	void buttonPressed(Pim::Button*);

	void update(float dt);
	void updateScroll(float dt);

	/* DEBUG */
	void keyEvent(Pim::KeyEvent&);



private:
	Pim::SpriteBatchNode	*buttonSheet;
	Pim::SpriteBatchNode	*scrollSheet;

	Pim::Sound				*music;

	ButtonScroller			*buttonScroller;

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
	bool					startGame;

	int						startLVL;

	HighscoreLayer *theHighscoreLayer;
};